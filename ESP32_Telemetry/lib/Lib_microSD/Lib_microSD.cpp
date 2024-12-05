#include "Lib_microSD.hpp"
#include "Pin_Defines.hpp"

SdFileSystem micro_sd_file;

uint8_t SdFileSystem::setup(){
    if(!SD.begin(0)){
      pinMode(MICROSD_STATE_LED, OUTPUT);
      digitalWrite(MICROSD_STATE_LED, HIGH);

      Serial.println("Card Mount Failed");
      return 0;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
      pinMode(MICROSD_STATE_LED, OUTPUT);
      digitalWrite(MICROSD_STATE_LED, HIGH);

      Serial.println("No SD card attached");
      return 0;
    }
    
    uint8_t i = 0;
    std::string file_path_temporary = "/pomiary_0.txt";
    
    while (SD.exists(file_path_temporary.c_str()))
    {
      i++;
      file_path_temporary = "/pomiary_" + std::to_string(i) + ".txt";
    }
    
    file_path = file_path_temporary;
    return 1;
  }


void SdFileSystem::writeFile(const char * message){

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


void SdFileSystem::appendFile(const char * message){

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