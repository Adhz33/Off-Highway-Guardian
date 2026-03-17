/*
 * RF Range Testing Tool
 * Upload to vehicle unit for range testing
 * Use with receiver unit to measure actual RF range
 */

#include "../common/protocol.h"

#define RF_TX_PIN 17
#define RF_RX_PIN 16
#define LED_TX 2

const uint8_t VEHICLE_ID[4] = {0xFF, 0xFF, 0xFF, 0xFF}; // Test ID

unsigned long packetCount = 0;
unsigned long lastTransmitTime = 0;
const unsigned long TRANSMIT_INTERVAL = 1000; // 1 second

void setup() {
    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1, RF_RX_PIN, RF_TX_PIN);
    
    pinMode(LED_TX, OUTPUT);
    
    Serial.println("\n=================================");
    Serial.println("RF Range Testing Tool");
    Serial.println("=================================");
    Serial.println("Transmitting test packets every 1 second");
    Serial.println("Walk away with receiver unit and note maximum range");
    Serial.println();
}

void loop() {
    if (millis() - lastTransmitTime >= TRANSMIT_INTERVAL) {
        transmitTestPacket();
        lastTransmitTime = millis();
    }
    
    // Blink LED on transmit
    digitalWrite(LED_TX, HIGH);
    delay(50);
    digitalWrite(LED_TX, LOW);
}

void transmitTestPacket() {
    EmergencyPacket packet;
    packet.version = PROTOCOL_VERSION;
    memcpy(packet.vehicleId, VEHICLE_ID, 4);
    packet.alertType = ALERT_TYPE_IMPACT;
    packet.timestamp = millis();
    packet.checksum = calculateChecksum(&packet);
    
    Serial2.write((uint8_t*)&packet, sizeof(EmergencyPacket));
    
    packetCount++;
    Serial.print("Packet #");
    Serial.print(packetCount);
    Serial.print(" transmitted at ");
    Serial.print(millis() / 1000);
    Serial.println(" seconds");
}
