#include <Wire.h>

#include <math.h>
#define CLK_PER 3333333ul // 20 MHz/6 = 3.333333 MHz
#define TIMEBASE_VALUE ((uint8_t) ceil(CLK_PER*0.000001))



#define MODULE_ID 0x49
#define LED 5
#define ADC_VOLTAGE_CHANNEL A5
#define ADC_CURRENT_CHANNEL A4


#define REG_POINTER_CONFIG (0x01)    ///< Configuration
#define REG_POINTER_CONVERT (0x00)   ///< Conversion


uint16_t ADC_value = 0;
uint8_t ADC_channel_number = 1;


void setup()
{
  // join i2c bus with address (7bit from 7 to 127)
  Wire.begin(MODULE_ID);  
           
  //register configuration event     
  Wire.onReceive(configEventInterrupt);  
  
  // register sending data event
  Wire.onRequest(sendEventInterrupt); 

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  delay(200);
  digitalWrite(LED, LOW);

#if defined(__AVR_ATmega328P__)
  Serial.begin(9600);           // start serial for output
  Serial.print("start");  
#endif

// Wire.setWireTimeout   //To be implemented in a future
}

void loop()
{
  uint16_t analog_temporary_read = 0;

  if(ADC_channel_number == 0){
    analog_temporary_read = (uint16_t)analogRead(ADC_CURRENT_CHANNEL); 
    delay(5);
  }else if (ADC_channel_number == 1) {
    analog_temporary_read = (uint16_t)analogRead(ADC_VOLTAGE_CHANNEL);
    delay(5);
  }
  
  ADC_value = analog_temporary_read;
}


//specify channel for ADC meassurement
void configEventInterrupt(int message_lenght){


#if defined(__AVR_ATmega328P__)
  Serial.println("on receive:");
#endif

int received_register = Wire.read();
if(received_register == REG_POINTER_CONFIG){
    ADC_channel_number = Wire.read();
}

#if defined(__AVR_ATmega328P__) 
    Serial.println(ADC_channel_number);       
#endif

}


//on request sends ADC value
void sendEventInterrupt()
{
#if defined(__AVR_ATmega328P__)
  Serial.println("wyslano"); 
#endif
  
  uint8_t send_buffer[2];
  send_buffer[0] = (ADC_value >> 8) & 0xFF;
  send_buffer[1] = ADC_value & 0xFF; 

#if defined(__AVR_ATmega328P__)
  Serial.println(send_buffer[0]);
  Serial.println(send_buffer[1]);
#endif

  Wire.write(send_buffer, 2);
}