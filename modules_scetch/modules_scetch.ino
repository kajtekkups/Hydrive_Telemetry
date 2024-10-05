#include <Wire.h>

#define MODULE_ID 0x48
#define NUMBER_OF_BYTES 2

uint8_t ADC_value[2] = {0, 0};

void setup()
{
  Wire.begin(MODULE_ID);                // join i2c bus with address (7bit from 7 to 127)
  Wire.onReceive(configEventInterrupt);  //register configuration event
  Wire.onRequest(sendEventInterrupt); // register sending data event
  Serial.begin(9600);           // start serial for output
  Serial.print("start");  
// Wire.setWireTimeout   //musi zostac dodane w przyszlosci, ochrona przed blokada
}

void loop()
{
  //rob pomiary
  delay(100);
}

//specify channel for ADC meassurement
void configEventInterrupt(int message_lenght){
  Serial.print("dziala");    
  if(Wire.available() == 1){
    uint8_t channel_number = Wire.read(); 
    Serial.print(channel_number);       
  }
}

//on request sends ADC value
void sendEventInterrupt()
{
  Wire.write(ADC_value, NUMBER_OF_BYTES);
}