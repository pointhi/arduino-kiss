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
 * Some generic Configuration for arduino-kiss
 */
#define LED_PIN_ERROR             A6
#define LED_PIN_RECEIVE           A4
#define LED_PIN_TRANSMIT          A2
#define LED_PIN_HEARTBEAT         A0

#define SERIAL_BAUDRATE       115200 // Baud
#define HEARTBEAT_INTERVAL    500 // ms
#define RESET_INTERVAL        5*60*1000 // ms (optional)

/*
 * RF95/RF96 Configuration
 */
#include "rf95_modem_configurations.h"
#define RF95_MODEM_CONFIG     RF95_MCFG_PW7_8Cr48Sf4096 // please look into rf95_modem_configuratons.h for more informations
//#define RF95_PREAMBLE_LENGTH  8 // (optional)
#define RF95_PIN_RESET        7
#define RF95_FREQUENCY        434.0 // MHz
#define RF95_POWER            5 // dBm (optional)

