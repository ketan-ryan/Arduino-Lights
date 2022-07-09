// #pragma once

// #include <Adafruit_NeoPixel.h>

// typedef struct pulse {
//   int position;
//   int step = 0;
//   int center = 2000;
//   int out1 = 0;
//   int out2 = 0;
// } PulseStruct;


// // Pattern types supported:
// enum pattern { NONE, RAINBOW_CYCLE, RAINBOW_FADE, FADE, MUSIC_LED_HUE, PULSE, MUSIC_FILL, JULY_FOURTH };
// // Patern directions supported:
// enum direction { FORWARD, REVERSE };

// // NeoPattern Class - derived from the Adafruit_NeoPixel class
// class NeoPatterns : public Adafruit_NeoPixel {
//     private:
//         const int num_pulses = 10;
//         PulseStruct *pulses[10] = {NULL};

//     public:
//         // Member Variables:
//         pattern  ActivePattern;  // which pattern is running
//         direction Direction;     // direction to run the pattern

//         unsigned long Interval;   // milliseconds between updates
//         unsigned long lastUpdate; // last update of position

//         uint8_t brightness;   // strip brightness
//         uint8_t numLeds;    // num leds to turn on with audio

//         long lColor1;   // signed color 1
//         long lColor2;   // signed color 2
//         long colorPad;    // enough room for a color +- some padding

//         uint32_t Color1, Color2;  // What colors are in use
//         uint16_t TotalSteps;  // total number of steps in the pattern
//         uint16_t Index;  // current step within the pattern

//         void (*OnComplete) ();  // Callback on completion of pattern

//         // Constructor - calls base-class constructor to initialize strip
//         NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type, void (*callback)())
//         :Adafruit_NeoPixel(pixels, pin, type)
//         {
//             OnComplete = callback;
//         }

//         // Update the pattern
//         void Update();

//         // Increment the Index and reset at the end
//         void Increment();

//         // Reverse pattern direction
//         void Reverse();

//         // Initialize for a RainbowCycle
//         void RainbowCycle(uint8_t interval, uint8_t br, direction dir = FORWARD);

//         // Update the Rainbow Cycle Pattern
//         void RainbowCycleUpdate();
        
//         // Init July 4th
//         void JulyFourth(uint8_t br) {
//         ActivePattern = JULY_FOURTH;
//         Index = 0;
//         Direction = FORWARD;
//         brightness = br;
//         TotalSteps = 450;
//         }

//         // Update July 4th
//         void JulyFourthUpdate() {
//         uint32_t one, two, three;
//         uint32_t red = Color(brightness, 0, 0);
//         uint32_t blue = Color(0, brightness, 0);
//         uint32_t white = Color(brightness, brightness, brightness);

//         if(Index <= 150) {
//             one = red; two = blue; three = white;
//         } else if(Index <= 300) {
//             one = blue; two = white; three = red;
//         } else {
//             one = white; two = red; three = blue;
//         }

//         for(unsigned int i = 0; i < numPixels(); i++) {
//             if(i % 10 <= 3) {
//             setPixelColor(i, one);
//             } else if(i % 10 <= 6) {
//             setPixelColor(i, two);
//             } else {
//             setPixelColor(i, three);
//             }
//         }
//         show();
//         Increment();
//         }

//         // Init rainbow fade
//         void RainbowFade(uint8_t br) {
//         ActivePattern = RAINBOW_FADE;
//         Index = 0;
//         TotalSteps = 16383;
//         Direction = FORWARD;
//         brightness = br;
//         }

//         // Update rainbow fade
//         void RainbowFadeUpdate() {
//         ColorSet(gamma32(ColorHSV(map(Index, 0, TotalSteps, 0, 65535), 255, brightness)));
//         show();
//         Increment();
//         }

//         // Init music reactivity changing hue and amount of illuminated LEDs
//         void MusicLedHue() {
//         ActivePattern = MUSIC_LED_HUE;
//         Index = 0;
//         TotalSteps = 65535;
//         Interval = 0;
//         }

//         // Update strip leds, hue to music
//         void MusicLedHueUpdate() {
//         // Turn on number of LEDs respective to how loud it is
//         for(unsigned int i = 0; i < numPixels(); i++) {
//             if (numLeds > 1 && i <= numLeds) {
//             // Set HSV value of current pixel, with hue constantly updating and based on position in strip
//             setPixelColor(i, gamma32(ColorHSV(map(i + Index, 0, numPixels(), 0, 65535), 255, brightness)));
//             }
//             else
//             setPixelColor(i, Color(0, 0, 0));
//         }
//         // Display strip
//         show();

//         // Rate at which hue updates depends on volume level
//         int rateOfChange = 10;
//         // The louder the volume, the faster the hue updates
//         if(numLeds <= 20)
//         rateOfChange = 50;
//         else if(numLeds > 20 && numLeds <= 40)
//         rateOfChange = 35;
//         else if(numLeds > 40 && numLeds <= 60)
//         rateOfChange = 25;
//         else if(numLeds > 60 && numLeds <= 80)
//         rateOfChange = 10;
//         else if(numLeds > 80 && numLeds <= 90)
//         rateOfChange = 5;
//         else if(numLeds > 90)
//         rateOfChange = 1;

//         if(millis() % rateOfChange == 0) {
//             Increment();
//         }
//         }

//         void Pulse() {
//         ActivePattern = PULSE;
//         Index = 0;
//         TotalSteps = 7;
//         Color1 = 65535;
//         // lColor1 = 21844;
//         // colorPad = lColor1;
//         // lColor2 = lColor1;
//         }

//         void PulseUpdate() {
//         // 00000 - red
//         // 10922 - magenta
//         // 21844 - blue
//         // 32768 - cyan
//         // 43688 - green
//         // 54610 - yellow
//         // 65535 - red
//         for(unsigned int i = 0; i < numPixels(); i++) {
//             setPixelColor(i, ColorHSV(Color1, 255, brightness));
//         }

//         // If struct at index n is empty
//         // malloc some space for it
//         // n is determined by volume level

//         if(pulses[0] == NULL) {
//             pulses[0] = (PulseStruct *)malloc(sizeof(PulseStruct));
//             pulses[0]->position = 5;
//         }
//         else{
//             pulseLed(pulses[0], 0);
//         }
//         show();
//         if(millis() % 100 == 0)
//             Increment();
//         }

//         void pulseLed(PulseStruct *pulse, int idx) {
//         // 0 - center 2000
//         // 1 - 1 out, center 1500
//         // 2 - 2 out, center 1000
//         // 3 - 2 at 3/4 bright, center 500
//         // 4 - 2 at 1/2 bright, center off
//         // 5 - all off
//         int s = pulse->step;
//         if(s > 0 && s < 5) {
//             pulse->center -= 500;
//             if(s == 1)
//             pulse->out1 = 2000;
//             if(s == 2)
//             pulse->out2 = 2000;
//             if(s > 1)
//             pulse->out1 -= 500;
//         }
//         if(s > 2)
//             pulse->out2 -= 500;

//         setPixelColor(pulse->position - 2, ColorHSV(Color1 + pulse->out2, 255, brightness));
//         setPixelColor(pulse->position - 1, ColorHSV(Color1 + pulse->out1, 255, brightness));
//         setPixelColor(pulse->position, ColorHSV(Color1 + pulse->center, 255, brightness));
//         setPixelColor(pulse->position + 1, ColorHSV(Color1 + pulse->out1, 255, brightness));
//         setPixelColor(pulse->position + 2, ColorHSV(Color1 + pulse->out2, 255, brightness));

//         pulse->step++;

//         if(pulse->step == 5) {
//             free(pulse);
//             pulses[idx] = NULL;
//         }
//         }

//         /**
//          * Given a hue, updates it circularly
//          * @param color a reference to the color to update
//          * @param cPad the initial color. has room for addition of a cap
//          * @param cap how far to update the color before switching directions
//          * @param stepLen the factor to update the hue by
//          * @param dir a reference to the current direction we're moving
//          **/
//         void updateHue(long &color, long cPad, int32_t cap, int stepLen, bool startedFwd) {
//         Serial.println(color);
//         // Moving in the forward direction
//         if(Direction == FORWARD) {
//             // Increment color by stepLen until it overflows the cap
//             color += stepLen;
//             // At which point reverse direction
//             if(color >= (startedFwd ? cPad + cap : cPad))
//             Direction = REVERSE;
//         }
//         // Moving in the backwards direction
//         else {
//             // Decrement color by stepLen until it reaches the original colr
//             color -= stepLen;
//             // At which point reverse direction
//             if(color <= (startedFwd ? cPad : cPad - cap))
//             Direction = FORWARD;
//         }
//         }

//         void MusicFill(uint32_t h) {
//         ActivePattern = MUSIC_FILL;
//         Index = 0;
//         TotalSteps = 65535;
//         Interval = 0;
//         Color2 = h;
//         }

//         void MusicFillUpdate() {
//         // Turn on number of LEDs respective to how loud it is
//         for(unsigned int i = 0; i < numPixels(); i++) {
//             if (numLeds > 1 && i <= numLeds) {
//             // Set HSV value of current pixel, with hue determined by user input
//             setPixelColor(i, gamma32(ColorHSV(map(Color2, 0, 255, 0, 65535), 255, brightness)));
//             }
//             else
//             setPixelColor(i, gamma32(ColorHSV(map(Color2 + 25, 0, 255, 0, 65535), 255, brightness)));
//         }
//         // Display strip
//         show();
//         }

//         // Initialize for a Fade
//         void Fade(uint32_t color1, uint32_t color2, uint16_t steps, uint8_t interval, direction dir = FORWARD)
//         {
//             ActivePattern = FADE;
//             Interval = interval;
//             TotalSteps = steps;
//             Color1 = color1;
//             Color2 = color2;
//             Index = 0;
//             Direction = dir;
//         }

//         // Update the Fade Pattern
//         void FadeUpdate()
//         {
//             // Calculate linear interpolation between Color1 and Color2
//             // Optimise order of operations to minimize truncation error
//             uint8_t red = ((Red(Color1) * (TotalSteps - Index)) + (Red(Color2) * Index)) / TotalSteps;
//             uint8_t green = ((Green(Color1) * (TotalSteps - Index)) + (Green(Color2) * Index)) / TotalSteps;
//             uint8_t blue = ((Blue(Color1) * (TotalSteps - Index)) + (Blue(Color2) * Index)) / TotalSteps;

//             ColorSet(Color(red, green, blue));
//             show();
//             Increment();
//         }

//         // Calculate 50% dimmed version of a color (used by ScannerUpdate)
//         uint32_t DimColor(uint32_t color)
//         {
//             // Shift R, G and B components one bit to the right
//             uint32_t dimColor = Color(Red(color) >> 1, Green(color) >> 1, Blue(color) >> 1);
//             return dimColor;
//         }

//         // Set all pixels to a color (synchronously)
//         void ColorSet(uint32_t color)
//         {
//             for (unsigned int i = 0; i < numPixels(); i++)
//             {
//                 setPixelColor(i, color);
//             }
//             show();
//         }

//         // Returns the Red component of a 32-bit color
//         uint8_t Red(uint32_t color)
//         {
//             return (color >> 16) & 0xFF;
//         }

//         // Returns the Green component of a 32-bit color
//         uint8_t Green(uint32_t color)
//         {
//             return (color >> 8) & 0xFF;
//         }

//         // Returns the Blue component of a 32-bit color
//         uint8_t Blue(uint32_t color)
//         {
//             return color & 0xFF;
//         }

//         // Input a value 0 to 255 to get a color value.
//         // The colours are a transition r - g - b - back to r.
//         uint32_t Wheel(byte WheelPos)
//         {
//             WheelPos = 255 - WheelPos;
//             double factor = (brightness / 255.0);
//             if(WheelPos < 85)
//             {
//                 return Color((255 - WheelPos * 3) * factor, 0, (WheelPos * 3) * factor);
//             }
//             else if(WheelPos < 170)
//             {
//                 WheelPos -= 85;
//                 return Color(0, (WheelPos * 3) * factor, (255 - WheelPos * 3) * factor);
//             }
//             else
//             {
//                 WheelPos -= 170;
//                 return Color((WheelPos * 3) * factor, (255 - WheelPos * 3) * factor, 0);
//             }
//         }
// };