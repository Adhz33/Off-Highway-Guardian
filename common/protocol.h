#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

// Protocol version
#define PROTOCOL_VERSION 1

// Alert types
#define ALERT_TYPE_IMPACT 0x01
#define ALERT_TYPE_ROLLOVER 0x02
#define ALERT_TYPE_SPIN 0x03
#define ALERT_TYPE_COMBINED 0x04

// Packet structure for RF transmission
typedef struct __attribute__((packed)) {
    uint8_t version;        // Protocol version
    uint8_t vehicleId[4];   // Unique vehicle identifier
    uint8_t alertType;      // Type of accident detected
    uint32_t timestamp;     // Milliseconds since boot
    uint8_t checksum;       // Simple XOR checksum
} EmergencyPacket;

// Calculate checksum for packet
inline uint8_t calculateChecksum(const EmergencyPacket* packet) {
    uint8_t checksum = 0;
    const uint8_t* data = (const uint8_t*)packet;
    for (size_t i = 0; i < sizeof(EmergencyPacket) - 1; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

// Verify packet checksum
inline bool verifyChecksum(const EmergencyPacket* packet) {
    return packet->checksum == calculateChecksum(packet);
}

#endif // PROTOCOL_H
