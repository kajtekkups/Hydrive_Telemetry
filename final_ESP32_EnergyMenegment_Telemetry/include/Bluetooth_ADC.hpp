#ifndef BLUETOOTH_ADC_HPP
#define BLUETOOTH_ADC_HPP
#include "BluetoothSerial.h"
   


void Send_Bluetooth_String(String data){    
    
    if (SerialBT.available()) {
    data = SerialBT.readString();
    SerialBT.println("You had entered: ");
    SerialBT.println(data);
    }
}
#endif