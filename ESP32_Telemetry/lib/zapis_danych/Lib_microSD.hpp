#ifndef LIB_MICROSD_HPP
#define LIB_MICROSD_HPP

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "Pin_Defines.hpp"


class SdFileSystem{
public:

  void begin(){
    if(!SD.begin(0)){
      pinMode(MICROSD_STATE_LED, OUTPUT);
      digitalWrite(MICROSD_STATE_LED, HIGH);

      Serial.println("Card Mount Failed");
      return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
      pinMode(MICROSD_STATE_LED, OUTPUT);
      digitalWrite(MICROSD_STATE_LED, HIGH);

      Serial.println("No SD card attached");
      return;
    }
    
    uint8_t i = 0;
    std::string file_path_temporary = "/pomiary_0.txt";
    
    while (SD.exists(file_path_temporary.c_str()))
    {
      i++;
      file_path_temporary = "/pomiary_" + std::to_string(i) + ".txt";
    }
    
    file_path = file_path_temporary;
    writeFile("test"); 
  }


  void writeFile(const char * message){

    File file = SD.open(file_path.c_str(), FILE_WRITE);
    if(!file){
      Serial.println("Failed to open file for writing");
      return;
    }
    if(file.print(message)){
      Serial.println("File written");
    } else {
      Serial.println("Write failed");
    }
    file.close();
  }


  void appendFile(const char * message){

    File file = SD.open(file_path.c_str(), FILE_APPEND);
    if(!file){
      Serial.println("Failed to open file for appending");
      return;
    }
    if(file.print(message)){
        Serial.println("Message appended on sd card \n\r");
    } else {
      Serial.println("Append failed");
    }
    file.close();
  }


private:
  std::string file_path;

};

extern SdFileSystem micro_sd_file;


#endif //LIB_MICROSD_HPP
