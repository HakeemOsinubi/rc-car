#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

unsigned long lastPacketTime; // stores the last the time a valid data packet was recieved
unsigned long timeElapsed;
bool connectionLost = false;
struct MotorData { 
    float left;
    float right;
};
MotorData receivedData = {};

//callback function that runs automatically after recieving a message
void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    // validating size of packet
    if (len != sizeof(MotorData)) {
        Serial.println("Invalid packet size");
        return;
    }
    //incase connection was lost on a previous run
    if(connectionLost){
        Serial.println("Connection Restored");
    }
    connectionLost = false;
    memcpy(&receivedData, incomingData, sizeof(receivedData));
    lastPacketTime = millis();
    Serial.print("Left Motor: ");
    Serial.print(receivedData.left);
    Serial.print(" | Right Motor: ");
    Serial.println(receivedData.right);
    
}


void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);//puts the esp32 into station mode
    if(esp_now_init()!= ESP_OK){
       Serial.println("ESP NOW initialization failed");
       return;
    }
    Serial.println("ESP NOW initialized sucessfully");
    if (esp_now_register_recv_cb(onDataRecv) != ESP_OK) { //whenever a message arrive call my callback function
        Serial.println("Callback registration failed");
        return;
    }
    Serial.println("Callback registered successfully");
}

void loop() {
    // if its been more than 500ms since a valid packet has been recieved stop both motors
    timeElapsed = millis() - lastPacketTime;
    if(timeElapsed > 500 && !connectionLost){
        receivedData.left = 0;
        receivedData.right = 0;
        connectionLost = true;
        Serial.println("Connection lost");
    }

}

