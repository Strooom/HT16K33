#include <Wire.h>
#include "HT16K33.h"

HT16K33::HT16K33(uint8_t theI2CAddress = 0x70, uint8_t theBrightness = 8) : brightness(theBrightness), I2CAddress(theI2CAddress)
    {
    clearDisplay();								// set displayRAM to all zeroes, blanks display
    }


HT16K33::~HT16K33()
    {
    }

void HT16K33::initialize(void)
    {
    Wire.beginTransmission(I2CAddress);
    Wire.write((uint8_t) HT16K33Command::SystemOscillatorOn);
    Wire.endTransmission();

    Wire.beginTransmission(I2CAddress);
    Wire.write((uint8_t)HT16K33Command::DisplayOn);				// Default is BlinkOff
    Wire.endTransmission();

    Wire.beginTransmission(I2CAddress);
    Wire.write((uint8_t)HT16K33Command::SetBrightness | brightness);	// Brightness to max

	Wire.endTransmission();
    }

void HT16K33::refreshDisplay(void)
    {
    Wire.beginTransmission(I2CAddress);			// First send the I2C address
    Wire.write((uint8_t)0x00);					// Then set HT16K33 displayRam address to 0

    for (uint8_t i = 0; i < 8; i++)				// iterate over all uint16 in displayRAM
        {
        Wire.write(displayRAM[i] & 0xFF);	// write LSB first
        Wire.write(displayRAM[i] >> 8);		// then write MSB
        }
    Wire.endTransmission();
    }

void HT16K33::setColon(boolean On)
    {
    if (On)
        {
        displayRAM[2] = 0x2;
        }
    else
        {
        displayRAM[2] = 0;
        }
    }

void HT16K33::showNumber(uint16_t theNmbr)
    {
    displayRAM[4] = HT16K33Number[theNmbr % 10];
    if (theNmbr >= 10)
        {
        displayRAM[3] = HT16K33Number[(theNmbr / 10) % 10];
        }
    else
        {
        displayRAM[3] = 0;
        }
    if (theNmbr >= 100)
        {
        displayRAM[1] = HT16K33Number[(theNmbr / 100) % 10];
        }
    else
        {
        displayRAM[1] = 0;
        }
    if (theNmbr >= 1000)
        {
        displayRAM[0] = HT16K33Number[(theNmbr / 1000) % 10];
        }
    else
        {
        displayRAM[0] = 0;
        }
    }

void HT16K33::showNumber(uint8_t left2Digits, uint8_t right2Digits)
    {
    displayRAM[4] = HT16K33Number[right2Digits % 10];
    displayRAM[3] = HT16K33Number[(right2Digits / 10) % 10];
    displayRAM[1] = HT16K33Number[left2Digits % 10];
    displayRAM[0] = HT16K33Number[(left2Digits / 10) % 10];
    }

void HT16K33::showNumber(float theNmbr)
    {
    uint32_t intNmbr;
    if (theNmbr >= 0)
        {
        intNmbr = (uint32_t)theNmbr;
        if (intNmbr >= 10000)
            {
            // Overflow - display '....'
            displayRAM[0] = 0x80;
            displayRAM[1] = 0x80;
            displayRAM[3] = 0x80;
            displayRAM[4] = 0x80;
            }
        else if (intNmbr >= 1000)
            {
            // 4 digits needed, so no decimal point and fractional digits
            displayRAM[0] = HT16K33Number[(intNmbr / 1000)];
            displayRAM[1] = HT16K33Number[(intNmbr / 100) % 10];
            displayRAM[3] = HT16K33Number[(intNmbr / 10) % 10];
            displayRAM[4] = HT16K33Number[intNmbr % 10];
            }
        else if (intNmbr >= 100)
            {
            // 3 digits needed, so decimal point and 1 fractional digit
            intNmbr = (uint32_t)(theNmbr * 10);
            displayRAM[0] = HT16K33Number[(intNmbr / 1000)];
            displayRAM[1] = HT16K33Number[(intNmbr / 100) % 10];
            displayRAM[3] = HT16K33Number[(intNmbr / 10) % 10] | 0x80;
            displayRAM[4] = HT16K33Number[intNmbr % 10];
            }
        else if (intNmbr >= 10)
            {
            // 2 digits needed, so decimal point and 2 fractional digits
            intNmbr = (uint32_t)(theNmbr * 100);
            displayRAM[0] = HT16K33Number[(intNmbr / 1000)];
            displayRAM[1] = HT16K33Number[(intNmbr / 100) % 10] | 0x80;
            displayRAM[3] = HT16K33Number[(intNmbr / 10) % 10];
            displayRAM[4] = HT16K33Number[intNmbr % 10];
            }
        else
            {
            // 1 digit needed, so decimal point and 3 fractional digits
            intNmbr = (uint32_t)(theNmbr * 1000);
            displayRAM[0] = HT16K33Number[(intNmbr / 1000)] | 0x80;
            displayRAM[1] = HT16K33Number[(intNmbr / 100) % 10];
            displayRAM[3] = HT16K33Number[(intNmbr / 10) % 10];
            displayRAM[4] = HT16K33Number[intNmbr % 10];
            }
        }
    else
        {
        intNmbr = (uint32_t)(-1 * theNmbr);
        if (intNmbr >= 1000)
            {
            // Overflow - display '-....'
            displayRAM[0] = 0xC0;
            displayRAM[1] = 0x80;
            displayRAM[3] = 0x80;
            displayRAM[4] = 0x80;
            }
        else if (intNmbr >= 100)
            {
            // '-'sign and 3 digits needed, so no decimal point and fractional digits
            displayRAM[0] = 0x40;	// '-' sign on first digit
            displayRAM[1] = HT16K33Number[(intNmbr / 100)];
            displayRAM[3] = HT16K33Number[(intNmbr / 10) % 10] | 0x80;
            displayRAM[4] = HT16K33Number[intNmbr % 10];
            }
        else if (intNmbr >= 10)
            {
            // '-'sign and 2 digits needed, so decimal point and 1 fractional digit
            intNmbr = (uint32_t)(theNmbr * -10);
            displayRAM[0] = 0x40;	// '-' sign on first digit
            displayRAM[1] = HT16K33Number[(intNmbr / 100)] | 0x80;
            displayRAM[3] = HT16K33Number[(intNmbr / 10) % 10];
            displayRAM[4] = HT16K33Number[intNmbr % 10];
            }
        else
            {
            // '-'sign and 1 digit needed, so decimal point and 2 fractional digits
            intNmbr = (uint32_t)(theNmbr * -100);
            displayRAM[0] = 0xC0;	// '-' sign on first digit
            displayRAM[1] = HT16K33Number[(intNmbr / 100)];
            displayRAM[3] = HT16K33Number[(intNmbr / 10) % 10];
            displayRAM[4] = HT16K33Number[intNmbr % 10];
            }
        }
    }

void HT16K33::clearDisplay(void)
    {
    for (uint8_t i = 0; i < 8; i++)				// iterate over all uint16 in displayRAM...
        {
        displayRAM[i] = 0;						// ... and initialize it to 0
        }
    }

void HT16K33::testDisplay(void)
    {
    displayRAM[0] = 0xFF;
    displayRAM[1] = 0xFF;
    displayRAM[2] = 0xFF;
    displayRAM[3] = 0xFF;
    displayRAM[4] = 0xFF;
    }