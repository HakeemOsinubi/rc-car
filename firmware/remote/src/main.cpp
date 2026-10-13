#include <Arduino.h>

void setup(){
    Serial.begin(115200);/*Initialize serial communication at 115200 baud*/

}

void loop(){
    int vertical = analogRead(34);
    if(abs((vertical-1930)) < 50){ /* code to ignore center flunctuations */
        vertical = 1930; 
    };
    int horizontal = analogRead(35);
    if(abs((horizontal-1955)) < 50){ /* code to ignore center flunctuations */
        horizontal = 1955; 
    };
    Serial.print("Vertical: ");
    Serial.print(vertical);
    Serial.print("  |  ");
    Serial.print("Horizontal: ");
    Serial.println(horizontal);
    delay(100);/*wait a 100ms before displaying the updated position*/
}