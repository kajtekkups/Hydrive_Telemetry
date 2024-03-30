#ifndef LIB_MICROSD_HPP
#define LIB_MICROSD_HPP

#include "FS.h"
#include "SD.h"
#include "SPI.h"


void listDir(fs::FS &fs, const char * dirname, uint8_t levels);


void createDir(fs::FS &fs, const char * path);


/******* nadpisz/stworz plik *******/
void writeFile(fs::FS &fs, const char * path, const char * message);


/******* dodaj conetent do pliku *******/
void appendFile(fs::FS &fs, const char * path, const char * message);


/******* Sprawadza czas odczytu danych z danej ścieżki *******/
void testFileIO(fs::FS &fs, const char * path);


void init_micro_sd();


#endif //LIB_MICROSD_HPP
