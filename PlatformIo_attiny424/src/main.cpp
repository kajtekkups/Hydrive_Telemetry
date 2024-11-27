#include <Wire.h>
#include <Arduino.h>

#define MODULE_ID 0x49
#define LED 5
#define ADC_VOLTAGE_CHANNEL A7
#define ADC_CURRENT_CHANNEL A4


/*=========================================================================
    POINTER REGISTERS
    -----------------------------------------------------------------------*/
#define REG_POINTER_CONV_VALUE (0x03)      ///< ADC Conversion value
#define REG_POINTER_CONVERT (0x00)   ///< Conversion complete
#define REG_POINTER_CONFIG (0x01)    ///< Configuration
/*=========================================================================*/

/*=========================================================================
    GLOBAL VARIABLES
    -----------------------------------------------------------------------*/
volatile uint16_t ADC_value = 0;
volatile uint8_t ADC_channel_number = 1;
volatile uint8_t ADC_conversion_complete = 0;
volatile uint8_t received_register = 0;
/*=========================================================================*/
void init_ADC0() {
    ADC_t* pADC;
    _fastPtr_d(pADC, &ADC0);
     /* On the 2-series maximum with internal reference is 3 MHz, so we will
     * target highest speed that doesn't exceed that and 16 ADC clocks sample
     * duration. */ 

    pADC->CTRLB  = ADC_PRESC_DIV2_gc;  //F_CPU = 1000000UL  1 MHz / 2 = 500 kHz
                             
    pADC->CTRLE = 15; // 15.5 without PGA, 16 with PGA, corresponding to 7.75 or 8 us.
    pADC->CTRLA = ADC_ENABLE_bm | ADC_LOWLAT_bm;
    /* Default low latency mode on
     * Users can turn it off if they care about power consumption while ADC is on
     * and chip is awake, since these parts don't have the perverse ADC-left-on
     * behavior of classic AVRs. */
    pADC->CTRLC = TIMEBASE_1US; // defined in Arduino.h.
    pADC->PGACTRL = ADC_PGABIASSEL_3_4X_gc | ADC_ADCPGASAMPDUR_15CLK_gc;
    /* Note that we don't *enable* it automatically in init().
     * 3/4th bias is good up to 4 MHz CLK_ADC, 15 ADC Clocks to sample the PGA
     * up to 5 MHz, so within the regime of speeds that have to be compatible
     * with internal references, we are in the clear there. */
}

void onReceiveFunction(int message_lenght);
void onRequestFunction();

void setup()
{
  // join i2c bus with address (7bit from 7 to 127)
  Wire.begin(MODULE_ID);  
           
  //register configuration event     
  Wire.onReceive(onReceiveFunction);  
  
  // register sending data event
  Wire.onRequest(onRequestFunction); 

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
  //interrupts may occure during conversion
  uint8_t current_channel = ADC_channel_number; 

  if(ADC_channel_number == 0){
    analog_temporary_read = (uint16_t)analogRead(ADC_CURRENT_CHANNEL); 
    delay(1);
  }else if (ADC_channel_number == 1) {
    analog_temporary_read = (uint16_t)analogRead(ADC_VOLTAGE_CHANNEL);
    delay(1);
  }
  
  ADC_value = analog_temporary_read;
  if(current_channel == ADC_channel_number) ADC_conversion_complete = 1;
}


void onReceiveFunction(int message_lenght){

#if defined(__AVR_ATmega328P__)
  Serial.println("on receive:");
#endif

received_register = Wire.read();

if(received_register == REG_POINTER_CONFIG){
  ADC_channel_number = Wire.read();
  ADC_conversion_complete = 0;
}

#if defined(__AVR_ATmega328P__) 
    Serial.println(ADC_channel_number);       
#endif

}


void onRequestFunction()
{
#if defined(__AVR_ATmega328P__)
  Serial.println("wyslano"); 
#endif

uint8_t send_buffer[2];
if(received_register == REG_POINTER_CONV_VALUE){
  send_buffer[0] = (ADC_value >> 8) & 0xFF;
  send_buffer[1] = ADC_value & 0xFF; 
}else if(received_register == REG_POINTER_CONVERT){
  if(ADC_conversion_complete){
    send_buffer[0] = 0;
    send_buffer[1] = 1;
  } else{
    send_buffer[0] = 0;
    send_buffer[1] = 0;
  }
}

#if defined(__AVR_ATmega328P__)
  Serial.println(send_buffer[0]);
  Serial.println(send_buffer[1]);
#endif

  Wire.write(send_buffer, 2);
}