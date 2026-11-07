#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

unsigned long lastPacketTime; // stores the last the time a valid data packet was recieved
portMUX_TYPE dataMux = portMUX_INITIALIZER_UNLOCKED; // lock that protects the variables shared between my callback and loop
struct MotorData { 
    float left;
    float right;
};
MotorData receivedData = {};
MotorData motorCommand = {};

//callback function that runs automatically after recieving a message
void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
    // validating size of packet
    if (len != sizeof(MotorData)) {
        return;
    }
    portENTER_CRITICAL(&dataMux); // prevents loop() from accessing recived data and last data packet cause they are currently being updated
    memcpy(&receivedData, incomingData, sizeof(receivedData)); 
    lastPacketTime = millis();
    portEXIT_CRITICAL(&dataMux);
    
}

void SetMotor(int pinA, int pinB, float speed){
    speed = constrain(speed, -1, 1); // alr between -1 and 1 so im prolly going to remove this
    int pwmSpeed = abs(speed)* 255;
    if (speed>0){ // spin foward
        analogWrite(pinB,0);
        analogWrite(pinA,pwmSpeed);
    }
    else if (speed<0){//reverse
        analogWrite(pinA,0);
        analogWrite(pinB,pwmSpeed);
    }
    else{
         analogWrite(pinA, 0);
         analogWrite(pinB, 0);
    }
}


void setup() {
    pinMode(25,OUTPUT); //M1A
    pinMode(26,OUTPUT); //M1B
    pinMode(27,OUTPUT); //M2A
    pinMode(14,OUTPUT); //M2B

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
    unsigned long packetTime;
    portENTER_CRITICAL(&dataMux);// prevents my callback from changing the recived data or packettime until loop() has gotten them
    motorCommand = receivedData;
    packetTime = lastPacketTime;// Copying the timestamp while locked so the callback can't change it.
    portEXIT_CRITICAL(&dataMux);
    // if we just started the program or its been more than 500ms since a valid packet has been recieved stop both motors
    if (packetTime == 0 || millis() - packetTime > 500) {
        motorCommand = {0, 0};
    }

    // Send motorCommand to the MDD3A
    SetMotor(25,26,motorCommand.left);
    SetMotor(27,14,-motorCommand.right); // Reverse the right motor because the motors face opposite directions  
    Serial.printf("Left: %.2f | Right: %.2f | Packet age: %lu ms\n",
              motorCommand.left,
              motorCommand.right,
              millis() - packetTime);
    delay(100);
}




