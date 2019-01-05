#ifndef _PT6958_H_
#define _PT6958_H_

#include <SPI.h>

namespace PT6958
{

static const uint8_t RAM_SIZE = 10;

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
    // set CS pin for SPI
    LedDriver( uint8_t csPin );

    // address 0..9
    // max size < 10
    // address + size <= RAM_SIZE(10)
    void Set(uint8_t address, const uint8_t *p, uint8_t size);

    // address 0..9
    void Set(uint8_t address, uint8_t data);

    // fill RAM with 0
    void ResetRam();

    // read pin data into low 3 bytes
    uint32_t ReadKeyboard();

    // brightness 0..7
    // 000: Pulse width = 1/16
    // 001: Pulse width = 2/16
    // 010: Pulse width = 4/16
    // 011: Pulse width = 10/16
    // 100: Pulse width = 11/16
    // 101: Pulse width = 12/16
    // 110: Pulse width = 13/16
    // 111: Pulse width = 14/16
    void DisplayOn( uint8_t brightness );

    void DisplayOff();

private:
    uint8_t _csPin;
    uint8_t _lastMode = 0;

    void StartSpi();
    void EndSpi();

    void SetMode(uint8_t mode);
    void SendCommand(uint8_t cmd);
};

}

#endif
