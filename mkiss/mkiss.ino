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
 *  (C) 2016 by folkert@vanheusden.com
 */

#include "kiss.h"
#include "configuration.h"

#include <SPI.h>
#include <RH_RF95.h> // LoRa device

// this is an example implementation using a "RadioHead"-driver for
// RF95 radio (a LoRa device).
// RadioHead: https://github.com/PaulStoffregen/RadioHead.git
static RH_RF95 rf95;

// the 'kiss'-class requires a couple of callback functions. these
// functions do the low-level work so that the kiss-class is generic

// a call-back function which you can adjust into something that
// peeks in the radio-buffer if anything is waiting
bool peekRadio() {
	return rf95.available();
}

// if there's data in your radio, then this callback should retrieve it
void getRadio(uint8_t *const whereTo, uint16_t *const n) {
	uint8_t dummy = *n;
	rf95.recv(whereTo, &dummy);
	*n = dummy;
}

void putRadio(const uint8_t *const what, const uint16_t size) {
	rf95.send(what, size);
	rf95.waitPacketSent();
}

// some arduino-platforms (teensy, mega) have multiple serial ports
// there you need to replace Serial by e.g. Serial2 or so
uint16_t peekSerial() {
	return Serial.available();
}

bool getSerial(uint8_t *const whereTo, const uint16_t n, const unsigned long int to) {
	for(uint16_t i=0; i<n; i++) {
		while(!Serial.available()) {
			if (millis() >= to)
				return false;
		}

		whereTo[i] = Serial.read();
	}

	return true;
}

void putSerial(const uint8_t *const what, const uint16_t size) {
	Serial.write(what, size);
}

bool initRadio() {
	if (rf95.init()) {
		delay(100);

		digitalWrite(LED_RECEIVE, LOW);
		digitalWrite(LED_TRANSMIT, LOW);
		digitalWrite(LED_ERROR, LOW);
		digitalWrite(LED_HEARTBEAT, LOW);

    const RH_RF95::ModemConfig cfg = RF95_MODEM_CONFIG;
    rf95.setModemRegisters(&cfg);
    rf95.setFrequency(RF95_FREQUENCY);
#ifdef RF95_POWER
    rf95.setTxPower(RF95_POWER); // radiohead default is 13
#endif
#ifdef RF95_PREAMBLE_LENGTH
    rf95.setPreambleLength(8);
#endif
		return true;
	}

	return false;
}

bool resetRadio() {
	digitalWrite(RF95_PIN_RESET, LOW);
	delay(1); // at least 100us, this is 1000us
	digitalWrite(RF95_PIN_RESET, HIGH);
	delay(5 + 1); // 5ms is required

	return initRadio();
}

// LoRa device can have a packetsize of 254 bytes
kiss k(254, peekRadio, getRadio, putRadio, peekSerial, getSerial, putSerial, resetRadio, LED_RECEIVE, LED_TRANSMIT, LED_ERROR);

void setup() {
	Serial.begin(SERIAL_BAUDRATE);

	pinMode(LED_RECEIVE, OUTPUT);
	digitalWrite(LED_RECEIVE, HIGH);
	pinMode(LED_TRANSMIT, OUTPUT);
	digitalWrite(LED_TRANSMIT, HIGH);
	pinMode(LED_ERROR, OUTPUT);
	digitalWrite(LED_ERROR, HIGH);
	pinMode(LED_HEARTBEAT, OUTPUT);
	digitalWrite(LED_HEARTBEAT, HIGH);

	pinMode(RF95_PIN_RESET, OUTPUT);
	digitalWrite(RF95_PIN_RESET, HIGH);

	k.begin();

	if (!resetRadio())
		k.debug("Radio init failed");

	k.debug("Go!");
}

void loop() {
	k.loop();

	const unsigned long int now = millis();
	static unsigned long int pHB = 0;

	if (now - pHB >= HEARTBEAT_INTERVAL) {
		static bool state = true;
		digitalWrite(LED_HEARTBEAT, state ? HIGH : LOW);
		state = !state;
		pHB = now;
	}

#ifdef RESET_INTERVAL
	static unsigned long int lastReset = 0;
	const unsigned long int resetInterval = RESET_INTERVAL;
	if (now - lastReset >= resetInterval) {
		k.debug("Reset radio");

		if (!resetRadio()) {
			for(byte i=0; i<3; i++) {
				digitalWrite(LED_ERROR, HIGH);
				delay(250);
				digitalWrite(LED_ERROR, LOW);
				delay(250);
			}
		}

		lastReset = now;
	}
#endif
}
