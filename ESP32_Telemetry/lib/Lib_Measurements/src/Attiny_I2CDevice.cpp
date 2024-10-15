#include "Attiny_I2CDevice.h"

/*!
 *    @brief  Create an I2C device at a given address
 *    @param  addr The 7-bit I2C address for the device
 *    @param  theWire The I2C bus to use, defaults to &Wire
 */
Attiny_I2CDevice::Attiny_I2CDevice(uint8_t addr, TwoWire *theWire) {
  _addr = addr;
  _wire = theWire;

  _maxBufferSize = I2C_BUFFER_LENGTH;
}

/*!
 *    @brief  Scans I2C for the address - note will give a false-positive
 *    if there's no pullups on I2C
 *    @return True if I2C initialized and a device with the addr found
 */
bool Attiny_I2CDevice::begin() {
  _wire->begin();

  // A basic scanner, see if it ACK's
  _wire->beginTransmission(_addr);
  if (_wire->endTransmission() == 0) {

  #ifdef DEBUG_SERIAL
      DEBUG_SERIAL.println(F("Detected"));
  #endif
    _begun = 1;
    return true;
  }

  #ifdef DEBUG_SERIAL
    DEBUG_SERIAL.println(F("Not detected"));
  #endif

  _begun = 0;
  return false;
}


/*!
 *    @brief  Write a buffer or two to the I2C device. Cannot be more than
 * maxBufferSize() bytes.
 *    @param  buffer Pointer to buffer of data to write. This is const to
 *            ensure the content of this buffer doesn't change.
 *    @param  len Number of bytes from buffer to write
 *    @param  prefix_buffer Pointer to optional array of data to write before
 * buffer. Cannot be more than maxBufferSize() bytes. This is const to
 *            ensure the content of this buffer doesn't change.
 *    @param  prefix_len Number of bytes from prefix buffer to write
 *    @param  stop Whether to send an I2C STOP signal on write
 *    @return True if write was successful, otherwise false.
 */
bool Attiny_I2CDevice::write(const uint8_t *buffer, size_t len, bool stop) {
  if(_begun == 0){
    return false;
  }

  if (len > maxBufferSize()) {

#ifdef DEBUG_SERIAL
    DEBUG_SERIAL.println(F("\tI2CDevice could not write such a large buffer"));
#endif

    return false;
  }

  _wire->beginTransmission(_addr);


  // Write the data
  if (_wire->write(buffer, len) != len) {

#ifdef DEBUG_SERIAL
    DEBUG_SERIAL.println(F("\tI2CDevice failed to write"));
#endif

    return false;
  }

#ifdef DEBUG_SERIAL

  DEBUG_SERIAL.print(F("\tI2CWRITE @ 0x"));
  DEBUG_SERIAL.print(_addr, HEX);
  DEBUG_SERIAL.print(F(" :: "));
  for (uint16_t i = 0; i < len; i++) {
    DEBUG_SERIAL.print(F("0x"));
    DEBUG_SERIAL.print(buffer[i], HEX);
    DEBUG_SERIAL.print(F(", "));
    if (i % 32 == 31) {
      DEBUG_SERIAL.println();
    }
  }

  if (stop) {
    DEBUG_SERIAL.print("\tSTOP");
  }
#endif

  if (_wire->endTransmission(stop) == 0) {
#ifdef DEBUG_SERIAL
    DEBUG_SERIAL.println();
    // DEBUG_SERIAL.println("Sent!");
#endif
    return true;
  } else {
#ifdef DEBUG_SERIAL
    DEBUG_SERIAL.println("\tFailed to send!");
#endif
    return false;
  }
}

/*!
 *    @brief  Read from I2C into a buffer from the I2C device.
 *    Cannot be more than maxBufferSize() bytes.
 *    @param  buffer Pointer to buffer of data to read into
 *    @param  len Number of bytes from buffer to read.
 *    @param  stop Whether to send an I2C STOP signal on read
 *    @return True if read was successful, otherwise false.
 */
bool Attiny_I2CDevice::read(uint8_t *buffer, size_t len, bool stop) {
    if(_begun == 0){
    return false;
  }
  
  size_t pos = 0;
  while (pos < len) {
    size_t read_len =
        ((len - pos) > maxBufferSize()) ? maxBufferSize() : (len - pos);
    bool read_stop = (pos < (len - read_len)) ? false : stop;
    if (!_read(buffer + pos, read_len, read_stop))
      return false;
    pos += read_len;
  }
  return true;
}

bool Attiny_I2CDevice::_read(uint8_t *buffer, size_t len, bool stop) {

  size_t recv = _wire->requestFrom((uint8_t)_addr, (uint8_t)len, (uint8_t)stop);

  if (recv != len) {
    // Not enough data available to fulfill our obligation!
#ifdef DEBUG_SERIAL
    DEBUG_SERIAL.print(F("\tI2CDevice did not receive enough data: "));
    DEBUG_SERIAL.println(recv);
#endif
    return false;
  }

  for (uint16_t i = 0; i < len; i++) {
    buffer[i] = _wire->read();
  }

#ifdef DEBUG_SERIAL
  DEBUG_SERIAL.print(F("\tI2CREAD  @ 0x"));
  DEBUG_SERIAL.print(_addr, HEX);
  DEBUG_SERIAL.print(F(" :: "));
  for (uint16_t i = 0; i < len; i++) {
    DEBUG_SERIAL.print(F("0x"));
    DEBUG_SERIAL.print(buffer[i], HEX);
    DEBUG_SERIAL.print(F(", "));
    if (len % 32 == 31) {
      DEBUG_SERIAL.println();
    }
  }
  DEBUG_SERIAL.println();
#endif

  return true;
}


/*!
 *    @brief  Returns the 7-bit address of this device
 *    @return The 7-bit address of this device
 */
uint8_t Attiny_I2CDevice::address(void) { return _addr; }
