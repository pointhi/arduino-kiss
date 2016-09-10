/*
 *  This file is part of arduino-kiss.
 *
 *  arduino-kiss is free software: you can redistribute it and/or
 *  modify it under the terms of the GNU Affero General Public License
 *  as published by the Free Software Foundation, in version 3 of
 *  the License.
 *
 *  arduino-kiss is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with arduino-kiss.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  (C) 2016 by Thomas Pointhuber
 */

/*
 * http://www.semtech.com/images/datasheet/LoraDesignGuide_STD.pdf
 * http://www.hoperf.com/upload/rf/RFM95_96_97_98W.pdf
 * 
 * Register format: {REG-0x1D, REG-0x1E, REG-0x26}
 *
 * ************** REG-0x1D (RegModemConfig 1) **************
 * 
 * Bit 7-4 (Bandwidth)
 *
 *    |-------|-----------------|
 *    | Value | Bandwidth (kHz) |
 *    |-------|-----------------|
 *    | 0x00  | 7.8             |
 *    | 0x10  | 10.4            |
 *    | 0x20  | 15.6            |
 *    | 0x30  | 20.8            |
 *    | 0x40  | 31.2            |
 *    | 0x50  | 41.7            |
 *    | 0x60  | 62.5            |
 *    | 0x70  | 125             |
 *    | 0x80  | 250             | (not supported in the 169 MHz band)
 *    | 0x90  | 500             | (not supported in the 169 MHz band)
 *    |-------|-----------------|
 *
 * Bit 3-1 (Coding Rate)
 * 
 *    |-------|---------------|----------------|
 *    | Value | Cyclic Coding | Overhead Ratio |
 *    |-------|---------------|----------------|
 *    | 0x02  | 4/5           | 1.25           |
 *    | 0x04  | 4/6           | 1.5            |
 *    | 0x06  | 4/7           | 1.75           |
 *    | 0x08  | 4/8           | 2              |
 *    |-------|---------------|----------------|
 *
 * Bit 0 (Implicite Header Mode On)
 *
 *    |-------|---------------|
 *    | Value | Header Mode   |
 *    |-------|---------------|
 *    | 0x00  | Explicite     |
 *    | 0x01  | Implicite     |
 *    |-------|---------------|
 *
 *
 * ************** REG-0x1E (RegModemConfig 2) **************
 * 
 * Bit 7-4 (Spreading Factor)
 * 
 *    |-------|--------------|----------------------|
 *    | Value | Chips/Symbol | LoRa Demodulator SNR |
 *    |-------|--------------|----------------------|
 *    | 0x60  | 64           | -5 dB                |
 *    | 0x70  | 128          | -7.5 dB              |
 *    | 0x80  | 256          | -10 dB               |
 *    | 0x90  | 512          | -12.5 dB             |
 *    | 0xA0  | 1024         | -15 dB               |
 *    | 0xB0  | 2048         | -17.5 dB             |
 *    | 0xC0  | 4096         | -20 dB               |
 *    |-------|--------------|----------------------|
 *
 * Bit 3 (TX Continous Mode)
 *
 *    |-------|----------------|
 *    | Value | Continous Mode |
 *    |-------|----------------|
 *    | 0x00  | Normal         |
 *    | 0x08  | Continuos      |
 *    |-------|----------------|
 *
 * Bit 2 (RX Payload CRC ON)
 *
 *    |-------|-----------------|
 *    | Value | CRC Header Mode |
 *    |-------|-----------------|
 *    | 0x00  | CRC off         |
 *    | 0x04  | CRC on          |
 *    |-------|-----------------|
 *
 * Bit 1-0 (RX Time-Out MSB)
 *
 *
 * ************** REG-0x26 (RegModemConfig 3) **************
 * 
 * Bit 7-4 (Unused)
 * 
 * Bit 3 (Mobile Node)
 * 
 *    |-------|-----------|
 *    | Value | Node Type |
 *    |-------|-----------|
 *    | 0x00  | Static    |
 *    | 0x08  | Mobile    |
 *    |-------|-----------|
 * 
 * Bit 2 (Auto Gain Controll)
 * 
 *    |-------|-------------------|
 *    | Value |   Gain Controll   |
 *    |-------|-------------------|
 *    | 0x00  | LNAGain Register  |
 *    | 0x04  | internal AGC loop |
 *    |-------|-------------------|
 *
 * Bit 1-0 (Reserved)
 *
 *
 * *********************************************************
 */

/* 
 * Some basic Modem Configurations, which are included in the RadioHead source code
 */
#define RF95_MCFG_Bw125Cr45Sf128    {0x72, 0x74, 0x00}  // Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Default medium range.  (the chip default)
#define RF95_MCFG_Bw500Cr45Sf128    {0x92, 0x74, 0x00}  // Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range.
#define RF95_MCFG_Bw31_25Cr48Sf512  {0x48, 0x94, 0x00}  // Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. Slow+long range.
#define RF95_MCFG_Bw125Cr48Sf4096   {0x78, 0xC4, 0x00}  // Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, CRC on. Slow+long range.

/* 
 * arduino_kiss configuration (as found in the original source code)
 */
#define RF95_MCFG_Bw250Cr48Sf1024   {0x88, 0xA4, 0x00}  // Bw = 250 kHz, Cr = 4/8, Sf = 1024chips/symbol, CRC on.

/*
 * custom configurations
 */
#define RF95_MCFG_PW7_8Cr48Sf4096   {0x08, 0xC4, 0x00}  // Bw = 7.5 kHz, Cr = 4/8, Sf = 4096chips/symbol, CRC on.
