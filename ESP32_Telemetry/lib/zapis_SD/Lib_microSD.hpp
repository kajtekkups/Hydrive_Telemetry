#ifndef LIB_MICROSD_HPP
#define LIB_MICROSD_HPP

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "Pin_Defines.hpp"


class SdFileSystem{
public:
  uint8_t begin();

  void writeFile(String message);

  void appendFile(String message);


private:
  std::string file_path;

};

extern SdFileSystem micro_sd_file;


#endif //LIB_MICROSD_HPP
