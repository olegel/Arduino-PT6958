#include "PT6958.h"

namespace PT6958
{

LedDriver::LedDriver( uint8_t csPin ) : _csPin(csPin)
{
    SPI.begin();
    pinMode(_csPin, OUTPUT);
    EndSpi();
}

// address 0..9
// max size < 10
// address + size <= RAM_SIZE(10)
void LedDriver::Set(uint8_t address, const uint8_t *p, uint8_t size)
{
    if(address >= RAM_SIZE)
        address = RAM_SIZE - 1;
    if(address + size > RAM_SIZE)
        size = RAM_SIZE - address;

    SetMode(CMD_MODE_WRITE_INCREMENT);

    StartSpi();
    SPI.transfer(CMD_SET_ADDR_0 + address);
    for(uint8_t i = 0; i < size; i++)
        SPI.transfer(p[i]);
    EndSpi();
}

void LedDriver::Set(uint8_t address, uint8_t data)
{
    if(address >= RAM_SIZE)
        address = RAM_SIZE - 1;

    SetMode(CMD_MODE_WRITE_FIXED_ADDRESS);
    StartSpi();
    SPI.transfer(CMD_SET_ADDR_0 + address);
    SPI.transfer(data);
    EndSpi();
}

void LedDriver::ResetRam()
{
    SetMode(CMD_MODE_WRITE_INCREMENT);
    StartSpi();
    SPI.transfer(CMD_SET_ADDR_0);
    for(uint8_t i = 0; i < RAM_SIZE; i++)
        SPI.transfer(0);
    EndSpi();
}

void LedDriver::DisplayOff()
{
    SendCommand(CMD_DISPLAY_OFF);
}

uint32_t LedDriver::ReadKeyboard()
{
    uint32_t data = 0;
    StartSpi();
    SPI.transfer(CMD_MODE_READ);
    delayMicroseconds(1);
    for(uint8_t i = 0; i < 3; i++)
    {
        data <<= i;
        data |= SPI.transfer(0);
    }
    EndSpi();
    return data;
}

// brightness 0..7
// 000: Pulse width = 1/16
// 001: Pulse width = 2/16
// 010: Pulse width = 4/16
// 011: Pulse width = 10/16
// 100: Pulse width = 11/16
// 101: Pulse width = 12/16
// 110: Pulse width = 13/16
// 111: Pulse width = 14/16
void LedDriver::DisplayOn( uint8_t brightness )
{
    SendCommand(CMD_DISPLAY_ON | (brightness & CMD_DISPLAY_ON_MASK) );
}

void LedDriver::StartSpi()
{
    SPI.beginTransaction(SPISettings(1000000, LSBFIRST, SPI_MODE2)); 
    digitalWrite(_csPin, LOW);
}

void LedDriver::EndSpi()
{
    digitalWrite(_csPin, HIGH);
    SPI.endTransaction();
}

void LedDriver::SetMode(uint8_t mode)
{
    if(_lastMode == mode)
        return;
    SendCommand(mode);
    _lastMode = mode;
}

void LedDriver::SendCommand(uint8_t cmd)
{
    StartSpi();
    SPI.transfer(cmd);
    EndSpi();
}

}
