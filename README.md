# Arduino-PT6958
PT6958 led driver Arduino library

## Sample

	PT6958::LedDriver ledDriver(10);

	ledDriver.ResetRam();
	ledDriver.DisplayOn(4);

	// switch on all segments
	for (byte address = 0; address < PT6958::RAM_SIZE; address++)
		ledDriver.Set(address, 0xFF);

	// read keyboard
	uint32_t data = ledDriver.ReadKeyboard();

	
	// update all segments at once
    byte array[PT6958::RAM_SIZE];

    for (byte address = 0; address < PT6958::RAM_SIZE; address++)
        array[address] = 0x55;
        
    ledDriver.Set(0, array, PT6958::RAM_SIZE);

