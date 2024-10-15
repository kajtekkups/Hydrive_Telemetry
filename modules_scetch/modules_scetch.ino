#include <Wire.h>

#define MODULE_ID 0x48
#define LED 5
uint16_t ADC_value = 0;
uint8_t channel_number = 1;


void setup()
{
  Wire.begin(MODULE_ID);                // join i2c bus with address (7bit from 7 to 127)
  Wire.onReceive(configEventInterrupt);  //register configuration event
  Wire.onRequest(sendEventInterrupt); // register sending data event

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.begin(9600);           // start serial for output
  Serial.print("start");  

// Wire.setWireTimeout   //musi zostac dodane w przyszlosci, ochrona przed blokada
}

void loop()
{
  uint8_t analog_temporary_read = 0;

  if(channel_number == 0){
    analog_temporary_read = analogRead(A3);
  }else if (channel_number == 1) {
    analog_temporary_read = analogRead(A2);
  }
  
  ADC_value = analog_temporary_read;
  delay(5);
}

//specify channel for ADC meassurement
void configEventInterrupt(int message_lenght){
  Serial.println("dziala");    

  digitalWrite(LED, LOW);

  if(Wire.available() == 1){
    channel_number = Wire.read(); 
    Serial.print(channel_number);       
  }
}

//on request sends ADC value
void sendEventInterrupt()
{
  Serial.println("wyslano"); 
  uint8_t send_buffer[2];
  send_buffer[0] = (ADC_value >> 8) & 0xFF;
  send_buffer[1] = ADC_value & 0xFF;
  Serial.println(send_buffer[0]);  
  Serial.println(send_buffer[1]);  
  Wire.write(send_buffer, 2);
}