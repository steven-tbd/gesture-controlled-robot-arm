#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
// Include Dynamixel SDK headers
#include "dynamixel_sdk.h"
#include "port_handler.h"
#include "packet_handler.h"

// --- Control Table Addresses for AX-12A ---
#define ADDR_MX_TORQUE_ENABLE            24
#define ADDR_MX_GOAL_POSITION            30
#define ADDR_MX_MOVING_SPEED             32
#define ADDR_MX_PRESENT_POSITION         36

// --- Protocol Constants ---
#define TORQUE_ENABLE                    1     // Value for enabling the torque
#define TORQUE_DISABLE                   0     // Value for disabling the torque


class ofApp : public ofBaseApp {
public:
    // Standard openFrameworks functions
    void setup();
    void update();
    void draw();
    void exit();

    // OSC receiver
    ofxOscReceiver receiver;

    // Dynamixel communication variables
    dynamixel::PortHandler* portHandler;
    dynamixel::PacketHandler* packetHandler;

    // --- MODIFIED: Set your correct COM port here ---
    const char* DEVICENAME = "COM4"; // Check your device name in Windows Device Manager
    int BAUDRATE = 1000000;
    float protocolVersion = 1.0;

    // Servo configuration
    int dxl_id[4] = { 1, 2, 3, 4 };

    // --- MODIFIED: These are now initialized in setup() for a safe start ---
    int dxl_goal_position[4];
    int osc_goal_position[4];
};