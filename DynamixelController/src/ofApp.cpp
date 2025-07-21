#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetWindowTitle("Dynamixel OSC Control");
    ofSetVerticalSync(true);
    receiver.setup(10000); // Listen on the port you are sending from TouchDesigner

    // Initialize Dynamixel communication
    portHandler = dynamixel::PortHandler::getPortHandler(DEVICENAME);
    packetHandler = dynamixel::PacketHandler::getPacketHandler(protocolVersion);

    if (portHandler->openPort()) {
        ofLogNotice("setup") << "Succeeded to open the port: " << DEVICENAME;
    }
    else {
        ofLogError("setup") << "Failed to open the port!";
        ofSystemAlertDialog("Failed to open the port! Check DEVICENAME in ofApp.h");
        ofExit();
    }

    if (portHandler->setBaudRate(BAUDRATE)) {
        ofLogNotice("setup") << "Succeeded to change the baudrate to " << BAUDRATE;
    }
    else {
        ofLogError("setup") << "Failed to change the baudrate!";
        ofSystemAlertDialog("Failed to change the baudrate!");
        ofExit();
    }

    // Enable Torque for all servos
    for (int id : dxl_id) {
        uint8_t dxl_error = 0;
        int dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, id, ADDR_MX_TORQUE_ENABLE, TORQUE_ENABLE, &dxl_error);
        if (dxl_comm_result != COMM_SUCCESS || dxl_error != 0) {
            ofLogError("setup") << "Failed to enable torque for servo " << id;
        }
        else {
            ofLogNotice("setup") << "Dynamixel servo ID " << id << " torque has been successfully enabled";
        }
    }

    // --- ADDED: Set a moderate default moving speed for smoother motion ---
    int default_speed = 150; // Value from 0 (stop) to 1023 (max speed)
    ofLogNotice("setup") << "Setting default moving speed to " << default_speed;
    for (int id : dxl_id) {
        packetHandler->write2ByteTxRx(portHandler, id, ADDR_MX_MOVING_SPEED, default_speed, nullptr);
    }


    // --- ADDED: Read initial positions for a safe, bumpless start ---
    ofLogNotice("setup") << "Reading initial positions from servos...";
    for (int i = 0; i < 4; ++i) {
        uint16_t initial_pos = 0;
        uint8_t dxl_error = 0;
        int dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, dxl_id[i], ADDR_MX_PRESENT_POSITION, &initial_pos, &dxl_error);

        if (dxl_comm_result == COMM_SUCCESS && dxl_error == 0) {
            // If read was successful, set this as our starting goal
            dxl_goal_position[i] = initial_pos;
            osc_goal_position[i] = initial_pos;
            ofLogNotice("setup") << "Servo " << dxl_id[i] << " initial position is " << initial_pos;
        }
        else {
            // If we fail to read, default to a safe middle position (512)
            dxl_goal_position[i] = 512;
            osc_goal_position[i] = 512;
            ofLogError("setup") << "Failed to read initial position for servo " << dxl_id[i] << ". Defaulting to 512.";
        }
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    // Check for waiting OSC messages
    while (receiver.hasWaitingMessages()) {
        ofxOscMessage m;
        receiver.getNextMessage(m);

        // Route OSC messages to the correct servo's goal position
        if (m.getAddress() == "/servo1" && m.getNumArgs() > 0 && m.getArgType(0) == OFXOSC_TYPE_FLOAT) {
            osc_goal_position[0] = (int)m.getArgAsFloat(0);
        }
        else if (m.getAddress() == "/servo2" && m.getNumArgs() > 0 && m.getArgType(0) == OFXOSC_TYPE_FLOAT) {
            osc_goal_position[1] = (int)m.getArgAsFloat(0);
        }
        else if (m.getAddress() == "/servo3" && m.getNumArgs() > 0 && m.getArgType(0) == OFXOSC_TYPE_FLOAT) {
            osc_goal_position[2] = (int)m.getArgAsFloat(0);
        }
        else if (m.getAddress() == "/servo4" && m.getNumArgs() > 0 && m.getArgType(0) == OFXOSC_TYPE_FLOAT) {
            osc_goal_position[3] = (int)m.getArgAsFloat(0);
        }
    }

    // Send updated goal positions to the Dynamixel servos
    for (int i = 0; i < 4; ++i) {
        // Only send a new command if the goal has changed
        if (dxl_goal_position[i] != osc_goal_position[i]) {
            dxl_goal_position[i] = osc_goal_position[i];
            packetHandler->write2ByteTxRx(portHandler, dxl_id[i], ADDR_MX_GOAL_POSITION, dxl_goal_position[i], nullptr);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0);
    ofSetColor(255);
    string msg = "Dynamixel OSC Controller\nListening on port 10000\n\n";
    msg += "Goal Positions:\n";
    msg += "/servo1 (ID " + ofToString(dxl_id[0]) + "): " + ofToString(dxl_goal_position[0]) + "\n";
    msg += "/servo2 (ID " + ofToString(dxl_id[1]) + "): " + ofToString(dxl_goal_position[1]) + "\n";
    msg += "/servo3 (ID " + ofToString(dxl_id[2]) + "): " + ofToString(dxl_goal_position[2]) + "\n";
    msg += "/servo4 (ID " + ofToString(dxl_id[3]) + "): " + ofToString(dxl_goal_position[3]) + "\n";
    ofDrawBitmapString(msg, 20, 20);
}

//--------------------------------------------------------------
void ofApp::exit() {
    // Disable torque and close port when exiting
    ofLogNotice("exit") << "Disabling torque and closing port...";
    for (int id : dxl_id) {
        packetHandler->write1ByteTxOnly(portHandler, id, ADDR_MX_TORQUE_ENABLE, TORQUE_DISABLE);
    }
    portHandler->closePort();
    ofLogNotice("exit") << "Cleanup complete.";
}

//--------------------------------------------------------------
int main() {
    ofSetupOpenGL(600, 400, OF_WINDOW);
    ofRunApp(new ofApp());
}