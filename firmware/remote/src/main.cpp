#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

float throttle;
float steering;
float left_Motor;
float right_Motor;

uint8_t carAddress[] = {0xB0, 0xCB, 0xD8, 0xCE, 0xAE, 0x50};//mac address of the car's ESP 32

struct MotorData { //data struct 
    float left;
    float right;
};

void setup(){
    Serial.begin(115200);/*Initialize serial communication at 115200 baud*/
    WiFi.mode(WIFI_STA); // putting the esp32 in station mode
    if(esp_now_init()!=ESP_OK){
        Serial.print("Initialization failed");
        return;
    }
    Serial.print("Initialization Successful");
    esp_now_peer_info_t peerInfo = {}; //peer struct 
    peerInfo.channel = 0; //wifi channel
    peerInfo.encrypt = false;
    memcpy(peerInfo.peer_addr, carAddress, 6);//copying the mac address to peer address
    //registering the peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
}
Serial.println("Peer registered successfully");
}

void loop(){
    int vertical = analogRead(34);
    // making a deadzone around the center
    if(abs((vertical-1930)) < 50){
        vertical = 1930; 
    }
    int horizontal = analogRead(35);
    if(abs((horizontal-1955)) < 50){ 
        horizontal = 1955; 
    }

    //normalizing the vertical/horizontal readings, so I can understand them
    if(vertical >= 1930){
        throttle = static_cast<float>(vertical-1930)/2165;
    }
    else{
        throttle = static_cast<float>(vertical-1930)/1930.0;
    }

    if(horizontal >= 1955){
        steering = static_cast<float>(1955-horizontal)/2140;
    }
    else{
        steering = static_cast<float>(horizontal-1955)/-1955;
    }

    //calculating the motor commands using differential steering
    left_Motor = constrain(throttle+steering,-1,1); // using clamping to make sure it stays between -1 and 1
    right_Motor = constrain(throttle-steering,-1,1);
    //printing the motor commands

    delay(100);//waiting a 100ms before sending the updated position

    // sending joystick readings to car's esp32
    MotorData motorData = {left_Motor, right_Motor};
    esp_err_t result = esp_now_send(
        carAddress,
        reinterpret_cast<const uint8_t*>(&motorData),
        sizeof(motorData)
    );
    if (result == ESP_OK) {
        Serial.println("queued motor commands");
    } else {
        Serial.println("failed to send ");
    }

}   