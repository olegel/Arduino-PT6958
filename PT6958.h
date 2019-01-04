#ifndef _PT6958_H_
#define _PT6958_H_

#include <SPI.h>

namespace PT6958
{

static const uint8_t RAM_SIZE = 10;

struct RamBuffer
{
    uint8_t _data[RAM_SIZE] = {0};

    inline void Set(uint8_t index, uint8_t data)
    {
        _data[index] = data;
    }
};

class LedDriver
{
private:
    static const uint8_t CMD_MODE_WRITE_INCREMENT = 0x40;
    static const uint8_t CMD_MODE_WRITE_FIXED_ADDRESS = 0x44;
    static const uint8_t CMD_MODE_READ = 0x42;
    static const uint8_t CMD_SET_ADDR_0 = 0xC0;
    static const uint8_t CMD_DISPLAY = 0x80;
    static const uint8_t CMD_DISPLAY_OFF = CMD_DISPLAY;
    static const uint8_t CMD_DISPLAY_ON = CMD_DISPLAY | 0x08;
    static const uint8_t CMD_DISPLAY_ON_MASK = 0x07;


public:
    LedDriver( uint8_t csPin ) : _csPin(csPin)
    {
        SPI.begin();
        pinMode(_csPin, OUTPUT);
        EndSpi();
    }

    void UpdateRam(const RamBuffer &ram)
    {
        SetMode(CMD_MODE_WRITE_INCREMENT);

        StartSpi();
        SPI.transfer(CMD_SET_ADDR_0);
        for(uint8_t i = 0; i < RAM_SIZE; i++)
            SPI.transfer(ram._data[i]);
        EndSpi();
    }

    void UpdateRam(uint8_t address, uint8_t data)
    {
        if(address >= RAM_SIZE)
            address = RAM_SIZE - 1;

        SetMode(CMD_MODE_WRITE_FIXED_ADDRESS);
        StartSpi();
        SPI.transfer(CMD_SET_ADDR_0 + address);
        SPI.transfer(data);
        EndSpi();
    }

    void ResetRam()
    {
        SetMode(CMD_MODE_WRITE_INCREMENT);
        StartSpi();
        SPI.transfer(CMD_SET_ADDR_0);
        for(uint8_t i = 0; i < RAM_SIZE; i++)
            SPI.transfer(0);
        EndSpi();
    }

    void DisplayOff()
    {
        SendCommand(CMD_DISPLAY_OFF);
    }

    uint32_t ReadKeyboard()
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
    void DisplayOn( uint8_t brightness )
    {
        SendCommand(CMD_DISPLAY_ON | (brightness & CMD_DISPLAY_ON_MASK) );
    }

private:
    uint8_t _csPin;
    uint8_t _lastMode = 0;

    void StartSpi()
    {
        SPI.beginTransaction(SPISettings(1000000, LSBFIRST, SPI_MODE2)); 
        digitalWrite(_csPin, LOW);
    }

    void EndSpi()
    {
        digitalWrite(_csPin, HIGH);
        SPI.endTransaction();
    }

    void SetMode(uint8_t mode)
    {
        if(_lastMode == mode)
            return;
        SendCommand(mode);
        _lastMode = mode;
    }

    void SendCommand(uint8_t cmd)
    {
        StartSpi();
        SPI.transfer(cmd);
        EndSpi();
    }
};

}

#endif
