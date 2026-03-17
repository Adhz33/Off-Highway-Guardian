/*
 * Receiver Range Testing Tool
 * Upload to receiver unit for range testing
 * Logs all received packets with RSSI
 */

#include "../common/protocol.h"

#define RF_TX_PIN 17
#define RF_RX_PIN 16
#define LED_RX 2

unsigned long packetsReceived = 0;
unsigned long lastPacketTime = 0;
unsigned long testStartTime = 0;

void setup() {
    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1, RF_RX_PIN, RF_TX_PIN);
    
    pinMode(LED_RX, OUTPUT);
    
    Serial.println("\n=================================");
    Serial.println("RF Receiver Range Test");
    Serial.println("=================================");
    Serial.println("Listening for test packets...");
    Serial.println("Format: Packet# | Time | Gap | Success Rate");
    Serial.println();
    
    testStartTime = millis();
}

void loop() {
    if (Serial2.available() >= sizeof(EmergencyPacket)) {
        receivePacket();
    }
    
    // Check for packet loss (> 2 seconds since last packet)
    if (packetsReceived > 0 && (millis() - lastPacketTime > 2000)) {
        unsigned long gap = (millis() - lastPacketTime) / 1000;
        if (gap > 2 && gap < 10) {
            Serial.print("*** PACKET LOSS: ");
            Serial.print(gap);
            Serial.println(" seconds gap ***");
        }
    }
}

void receivePacket() {
    EmergencyPacket packet;
    Serial2.readBytes((uint8_t*)&packet, sizeof(EmergencyPacket));
    
    // Verify packet
    if (packet.version != PROTOCOL_VERSION) {
        Serial.println("ERROR: Invalid protocol version");
        return;
    }
    
    if (!verifyChecksum(&packet)) {
        Serial.println("ERROR: Checksum failed");
        return;
    }
    
    // Valid packet
    packetsReceived++;
    unsigned long now = millis();
    unsigned long gap = (lastPacketTime > 0) ? (now - lastPacketTime) : 0;
    lastPacketTime = now;
    
    // Calculate success rate (assuming 1 packet per second)
    unsigned long expectedPackets = (now - testStartTime) / 1000;
    float successRate = (expectedPackets > 0) ? 
        (packetsReceived * 100.0 / expectedPackets) : 100.0;
    
    // Blink LED
    digitalWrite(LED_RX, HIGH);
    
    // Print results
    Serial.print("Packet #");
    Serial.print(packetsReceived);
    Serial.print(" | Time: ");
    Serial.print(now / 1000);
    Serial.print("s | Gap: ");
    Serial.print(gap);
    Serial.print("ms | Success: ");
    Serial.print(successRate, 1);
    Serial.println("%");
    
    digitalWrite(LED_RX, LOW);
}
