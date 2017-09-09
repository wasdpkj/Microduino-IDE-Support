#include <Arduino.h>
#include <cc1101.h>
#include <ccpacket.h>

// Attach CC1101 pins to their corresponding SPI pins
// Uno pins:
// CSN (SS) => 10
// MOSI => 11
// MISO => 12
// SCK => 13
// GD0 => A valid interrupt pin for your platform (defined below this)

#define CC1101_GDO0 D2

CC1101 radio;

byte syncWord[2] = {199, 10};
bool packetWaiting;

unsigned long lastSend = 0;
unsigned int sendDelay = 5000;

void messageReceived() {
    packetWaiting = true;
}

void setup() {
    Serial.begin(115200);
    delay(100);
    radio.init();
    radio.setSyncWord(syncWord);
    radio.setCarrierFreq(CFREQ_433);	//CFREQ_868 CFREQ_915 CFREQ_433
    radio.disableAddressCheck();
    radio.setTxPowerAmp(PA_LongDistance);

    Serial.print(F("CC1101_PARTNUM "));
    Serial.println(radio.readReg(CC1101_PARTNUM, CC1101_STATUS_REGISTER));
    Serial.print(F("CC1101_VERSION "));
    Serial.println(radio.readReg(CC1101_VERSION, CC1101_STATUS_REGISTER));
    Serial.print(F("CC1101_MARCSTATE "));
    Serial.println(radio.readReg(CC1101_MARCSTATE, CC1101_STATUS_REGISTER) & 0x1f);

    Serial.println(F("CC1101 radio initialized."));
    attachInterrupt(digitalPinToInterrupt(CC1101_GDO0), messageReceived, FALLING);
}

// Get signal strength indicator in dBm.
// See: http://www.ti.com/lit/an/swra114d/swra114d.pdf
int rssi(char raw) {
    uint8_t rssi_dec;
    // TODO: This rssi_offset is dependent on baud and MHz; this is for 38.4kbps and 433 MHz.
    uint8_t rssi_offset = 74;
    rssi_dec = (uint8_t) raw;
    if (rssi_dec >= 128)
        return ((int)( rssi_dec - 256) / 2) - rssi_offset;
    else
        return (rssi_dec / 2) - rssi_offset;
}

// Get link quality indicator.
int lqi(char raw) {
    return 0x3F - raw;
}

void loop() {
    if (packetWaiting) {
        detachInterrupt(digitalPinToInterrupt(CC1101_GDO0));
        packetWaiting = false;
        CCPACKET packet;
        if (radio.receiveData(&packet) > 0) {
            Serial.println(F("Received packet..."));
            if (!packet.crc_ok) {
                Serial.println(F("crc not ok"));
            }
            Serial.print(F("lqi: "));
            Serial.println(lqi(packet.lqi));
            Serial.print(F("rssi: "));
            Serial.print(rssi(packet.rssi));
            Serial.println(F("dBm"));

            if (packet.crc_ok && packet.length > 0) {
                Serial.print(F("packet: len "));
                Serial.println(packet.length);
                Serial.println(F("data: "));
                Serial.println((const char *) packet.data);
            }
        }

        attachInterrupt(digitalPinToInterrupt(CC1101_GDO0), messageReceived, FALLING);
    }
    unsigned long now = millis();
    if (now > lastSend + sendDelay) {
        detachInterrupt(digitalPinToInterrupt(CC1101_GDO0));

        lastSend = now;
        const char *message = "hello world";
        CCPACKET packet;
        // We also need to include the 0 byte at the end of the string
        packet.length = strlen(message)  + 1;
        strncpy((char *) packet.data, message, packet.length);

        radio.sendData(packet);
        Serial.println(F("Sent packet..."));

        attachInterrupt(digitalPinToInterrupt(CC1101_GDO0), messageReceived, FALLING);
    }
}
