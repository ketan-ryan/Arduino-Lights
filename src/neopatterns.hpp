#pragma once
#include <Adafruit_NeoPixel.h>

enum STATE {PWR_OFF, PAUSED, ON_STATIC, ON_DYNAMIC};
// Pattern types supported:
enum pattern { NONE, RAINBOW_CYCLE, RAINBOW_FADE, FADE, MUSIC_LED_HUE, PULSE, MUSIC_FILL, JULY_FOURTH };
// Pattern directions supported:
enum direction { FORWARD, REVERSE };

/**
 * @brief Based on this implementation
 * https://learn.adafruit.com/multi-tasking-the-arduino-part-3/some-common-bits 
 */
class CustomNeoPixel : public Adafruit_NeoPixel {
    private:
        const int num_pulses = 10;
        public:

        // Member Variables:
        pattern  ActivePattern;   // which pattern is running
        direction Direction;      // direction to run the pattern

        unsigned long Interval;   // milliseconds between updates
        unsigned long lastUpdate; // last update of position

        uint8_t brightness;       // strip brightness
        uint8_t numLeds;          // num leds to turn on with audio

        long lColor1;             // signed color 1
        long lColor2;             // signed color 2
        long colorPad;            // enough room for a color +- some padding

        uint32_t Color1, Color2;  // What colors are in use
        uint16_t TotalSteps;      // total number of steps in the pattern
        uint16_t Index;           // current step within the pattern

        void (*OnComplete) ();    // Callback on completion of pattern

        // Set all pixels to a color (synchronously)
        void ColorSet(uint32_t color) {
            for (unsigned int i = 0; i < numPixels(); i++) {
                setPixelColor(i, color);
            }
            show();
        }

        // Returns the Red component of a 32-bit color
        uint8_t Red(uint32_t color) {
            return (color >> 16) & 0xFF;
        }

        // Returns the Green component of a 32-bit color
        uint8_t Green(uint32_t color) {
            return (color >> 8) & 0xFF;
        }

        // Returns the Blue component of a 32-bit color
        uint8_t Blue(uint32_t color) {
            return color & 0xFF;
        }

        // Increment the Index and reset at the end
        void Increment() {
            if (Direction == FORWARD) {
                Index++;
                if (Index >= TotalSteps) {
                    Index = 0;
                    if (OnComplete != NULL) {
                        OnComplete(); // call the completion callback
                    }
                }
            }
            else {
                // Direction == REVERSE
                --Index;
                if (Index <= 0) {
                    Index = TotalSteps-1;
                    if (OnComplete != NULL) {
                        OnComplete(); // call the comlpetion callback
                    }
                }
            }
        }

        // Input a value 0 to 255 to get a color value.
        // The colours are a transition r - g - b - back to r.
        uint32_t Wheel(byte WheelPos) {
            WheelPos = 255 - WheelPos;
            double factor = (brightness / 255.0);
            if(WheelPos < 85) {
                return Color((255 - WheelPos * 3) * factor, 0, (WheelPos * 3) * factor);
            }
            else if(WheelPos < 170) {
                WheelPos -= 85;
                return Color(0, (WheelPos * 3) * factor, (255 - WheelPos * 3) * factor);
            }
            else {
                WheelPos -= 170;
                return Color((WheelPos * 3) * factor, (255 - WheelPos * 3) * factor, 0);
            }
        }


    public:
        unsigned long getInterval() {
            return Interval;
        }

        unsigned long getLastUpdate() {
            return lastUpdate;
        }

        void setLastUpdate(unsigned long update) {
            lastUpdate = update;
        }

        CustomNeoPixel(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)())
        :Adafruit_NeoPixel(pixels, pin, type) {
            OnComplete = callback;
        }

        void Red(uint8_t interval, uint8_t br, int hue) {
            brightness = br;
            ActivePattern = NONE;
            ColorSet(Color(brightness, 0, 0));
            show();
        }

        void Green(uint8_t interval, uint8_t br, int hue) {
            brightness = br;
            ActivePattern = NONE;
            ColorSet(Color(0, brightness, 0));
            show();
        }

        void Blue(uint8_t interval, uint8_t br, int hue) {
            brightness = br;
            ActivePattern = NONE;
            ColorSet(Color(0, 0, brightness));
            show();
        }

        void White(uint8_t interval, uint8_t br, int hue) {
            brightness = br;
            ActivePattern = NONE;
            ColorSet(Color(brightness, brightness, brightness));
            show();
        }

        void Hue(uint8_t interval, uint8_t br, int hue) {
            brightness = br;
            ActivePattern = NONE;
            ColorSet(gamma32(ColorHSV(map(hue, 0, 255, 0, 65535), 255, brightness)));
            show();
        }

        // Initialize for a RainbowCycle
        void RainbowCycle(uint8_t interval, uint8_t br, int hue) {
            ActivePattern = RAINBOW_CYCLE;
            Interval = interval;
            TotalSteps = 255;
            Index = 0;
            Direction = FORWARD;
            brightness = br;
        }

        // Update the Rainbow Cycle Pattern
        void RainbowCycleUpdate()
        {
            for(unsigned int i = 0; i < numPixels(); i++)
            {
                setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
            }
            show();
            Increment();
        }

        // Init July 4th
        void JulyFourth(uint8_t interval, uint8_t br, int hue) {
            ActivePattern = JULY_FOURTH;
            Index = 0;
            Direction = FORWARD;
            brightness = br;
            TotalSteps = 450;
        }

        // Update July 4th
        void JulyFourthUpdate() {
            uint32_t one, two, three;
            uint32_t red = Color(brightness, 0, 0);
            uint32_t blue = Color(0, brightness, 0);
            uint32_t white = Color(brightness, brightness, brightness);

            if(Index <= 150) {
                one = red; two = blue; three = white;
            } else if(Index <= 300) {
                one = blue; two = white; three = red;
            } else {
                one = white; two = red; three = blue;
            }

            for(unsigned int i = 0; i < numPixels(); i++) {
                if(i % 10 <= 3) {
                    setPixelColor(i, one);
                } else if(i % 10 <= 6) {
                    setPixelColor(i, two);
                } else {
                    setPixelColor(i, three);
                }
            }
            show();
            Increment();
        }

        // Init rainbow fade
        void RainbowFade(uint8_t interval, uint8_t br, int hue) {
            ActivePattern = RAINBOW_FADE;
            Index = 0;
            TotalSteps = 16383;
            Direction = FORWARD;
            brightness = br;
        }

        // Update rainbow fade
        void RainbowFadeUpdate() {
            ColorSet(gamma32(ColorHSV(map(Index, 0, TotalSteps, 0, 65535), 255, brightness)));
            show();
            Increment();
        }

        // Init music reactivity changing hue and amount of illuminated LEDs
        void MusicLedHue(uint8_t interval, uint8_t br, int hue) {
            ActivePattern = MUSIC_LED_HUE;
            Index = 0;
            TotalSteps = 65535;
            Interval = interval;
            // Interval = 0;
            brightness = br;
        }

        // Update strip leds, hue to music
        void MusicLedHueUpdate() {
            // Turn on number of LEDs respective to how loud it is
            for(unsigned int i = 0; i < numPixels(); i++) {
                if (numLeds > 1 && i <= numLeds) {
                    // Set HSV value of current pixel, with hue constantly updating and based on position in strip
                    setPixelColor(i, gamma32(ColorHSV(map(i + Index, 0, numPixels(), 0, 65535), 255, brightness)));
                }
                else {
                    setPixelColor(i, Color(0, 0, 0));
                }
            }
            // Display strip
            show();

            // Rate at which hue updates depends on volume level
            int rateOfChange = 10;
            // The louder the volume, the faster the hue updates
            if(numLeds <= 20)
            rateOfChange = 50;
            else if(numLeds > 20 && numLeds <= 40)
            rateOfChange = 35;
            else if(numLeds > 40 && numLeds <= 60)
            rateOfChange = 25;
            else if(numLeds > 60 && numLeds <= 80)
            rateOfChange = 10;
            else if(numLeds > 80 && numLeds <= 90)
            rateOfChange = 5;
            else if(numLeds > 90)
            rateOfChange = 1;

            if(millis() % rateOfChange == 0) {
                Increment();
            }
        }

        void MusicFill(uint8_t interval, uint8_t br, int hue) {
            ActivePattern = MUSIC_FILL;
            Index = 0;
            TotalSteps = 65535;
            Interval = 0;
            Color2 = hue;
        }

        void MusicFillUpdate() {
            // Turn on number of LEDs respective to how loud it is
            for(unsigned int i = 0; i < numPixels(); i++) {
                if (numLeds > 1 && i <= numLeds) {
                    // Set HSV value of current pixel, with hue determined by user input
                    setPixelColor(i, gamma32(ColorHSV(map(Color2, 0, 255, 0, 65535), 255, brightness)));
                }
                else {
                    setPixelColor(i, gamma32(ColorHSV(map(Color2 + 25, 0, 255, 0, 65535), 255, brightness)));
                }
            }
            // Display strip
            show();
        }
};

class Page {
    public:
        enum ACTIVE_PATTERN {};
        ACTIVE_PATTERN ActivePattern;
        CustomNeoPixel *strip;

        // Array of function pointers to init functions
        void (CustomNeoPixel::*initializers[10]) (uint8_t brightness, uint8_t interval, int hue);
        // Array of function pointers to update functions
        void (CustomNeoPixel::*updates[10]) ();

        Page(CustomNeoPixel *st) {
            strip = st;
        }

        void init(int pattern, uint8_t br, uint8_t intr, int hue) {
            (strip->*initializers[pattern]) (br, intr, hue);
        }

        void update(int pattern) {
           (strip->*updates[pattern]) ();
        };
};

class NeoPatterns {
    public:
        int numPages = 4;
        int currentPage = 0;
        
        CustomNeoPixel *neopx; 
        
        NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)()) 
        : neopx( new CustomNeoPixel(pixels, pin, type, NULL)) {}
        
        Page pages[4] = {Page(neopx), Page(neopx), Page(neopx), Page(neopx)};

        void setup() {
            pages[0].initializers[0] = &CustomNeoPixel::Red;
            pages[0].initializers[1] = &CustomNeoPixel::Green;
            pages[0].initializers[2] = &CustomNeoPixel::Blue;
            pages[0].initializers[3] = &CustomNeoPixel::White;
            pages[0].initializers[4] = &CustomNeoPixel::RainbowFade;
            pages[0].initializers[5] = &CustomNeoPixel::RainbowCycle;
            pages[0].initializers[6] = &CustomNeoPixel::MusicLedHue;
            pages[0].initializers[7] = &CustomNeoPixel::JulyFourth;
            pages[0].initializers[8] = &CustomNeoPixel::Hue;
            pages[0].initializers[9] = &CustomNeoPixel::MusicFill;

            pages[0].updates[4] = &CustomNeoPixel::RainbowFadeUpdate;
            pages[0].updates[5] = &CustomNeoPixel::RainbowCycleUpdate;
            pages[0].updates[6] = &CustomNeoPixel::MusicLedHueUpdate;
            pages[0].updates[7] = &CustomNeoPixel::JulyFourthUpdate;
            pages[0].updates[9] = &CustomNeoPixel::MusicFillUpdate;
        }

        void init(int pattern, uint8_t br, uint8_t interval, int hue) {
            pages[currentPage].init(pattern, br, interval, hue);
        }

        void update(int pattern) {
            if((millis() - neopx->getLastUpdate()) > neopx->getInterval()) {
                neopx->setLastUpdate(millis());
                pages[currentPage].update(pattern);
            }
        }
};

