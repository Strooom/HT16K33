#ifndef HT16K33_h
#define HT16K33_h
#include <Arduino.h>

enum class HT16K33Command : uint8_t
    {
    SystemOscillatorOff = 0x20,
    SystemOscillatorOn = 0x21,

    DisplayOn = 0x81,			// use DisplayOn | Blink1Hz to set Blink rate. Default is BlinkOff
    DisplayOff = 0x80,
    BlinkOff = 0x00,
    Blink2Hz = 0x02,
    Blink1Hz = 0x04,
    Blink0dot5Hz = 0x06,

    SetBrightness = 0xE0,		// use SetBrightness | 3 to set  brightness to 3. Levels 1 .. 15 supported

    nmbrCommands
    };


enum class HT16K33Char : uint8_t
    {
    dash = 0x40,
    colon = 0x02,
    C = 0x01 | 0x20 | 0x10 | 0x08,
    revC = 0x01 | 0x02 | 0x04 | 0x08,
    TopBottom = 0x01 | 0x08,
    nmbrChars
    };


static const uint8_t HT16K33Number[] =
    {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
    0x77, // a
    0x7C, // b
    0x39, // C
    0x5E, // d
    0x79, // E
    0x71, // F
    };


class HT16K33
    {
    private:
        uint8_t I2CAddress;
        uint8_t brightness;

    public:
        uint16_t displayRAM[8];
        HT16K33(uint8_t I2CAddress, uint8_t brightness);		// Constructor
        ~HT16K33();												// Destructor

        void initialize();
        void refreshDisplay(void);
        void setColon(boolean On);
        void showNumber(uint16_t theNmbr);
        void showNumber(uint8_t left2Digits, uint8_t right2Digits);
        void showNumber(float theNmbr);
        void clearDisplay(void);
        void testDisplay(void);				//sets all segments on...
    };

#endif // !HT16K33_h
