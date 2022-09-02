#pragma once

#include <Adafruit_NeoPixel.h>

/** State of the strip */
enum STATE { PWR_OFF, PAUSED, ON_STATIC, ON_DYNAMIC };

/** Pattern directions */
enum direction { FORWARD, REVERSE };

/**
 * @brief Contains extra information about a mode.
 * Whether it is dynamic, changes hue based on input, and/or uses soundcard input.
 */
struct info {
    bool sound = false;
    bool dynamic = false;
    bool hue = false;
};

/**
 * @brief Based on this implementation
 * https://learn.adafruit.com/multi-tasking-the-arduino-part-3/some-common-bits
 */
class CustomNeoPixel : public Adafruit_NeoPixel {
    private:
        // Member Variables:
        /** Direction to run the pattern */
        direction dir;

        /** Milliseconds between updates */
        unsigned long interval;
        /** Last update of position */
        unsigned long lastUpdate;

        /** Strip brightness */
        uint8_t brightness;
        /** Number of LEDs to turn on with audio */
        uint8_t numAudioLeds;

        /** Hue the user input */
        uint32_t inputHue;
        /** Total number of steps in the pattern */
        uint16_t totalSteps;
        /** Current step within the pattern */
        uint16_t index;

        /**
         * @brief Set all pixels to a color (synchronously)
         *
         * @param color 32-bit color value. Most significant byte is white (for RGBW pixels) or ignored (for RGB pixels), next is red, then green, and least significant byte is blue.
         */
        void colorSet(uint32_t color);

        /**
         * @brief Get the red component of a 32-bit color
         *
         * @param color 32-bit color value.
         * @return uint8_t 8 bits representing the red component
         */
        uint8_t getRed(uint32_t color) {
            return (color >> 16) & 0xFF;
        }

        /**
         * @brief Get the green component of a 32-bit color
         *
         * @param color 32-bit color value.
         * @return uint8_t 8 bits representing the green component
         */
        uint8_t getGreen(uint32_t color) {
            return (color >> 8) & 0xFF;
        }

        /**
         * @brief Get the blue component of a 32-bit color
         *
         * @param color 32-bit color value.
         * @return uint8_t 8 bits representing the blue component
         */
        uint8_t getBlue(uint32_t color) {
            return color & 0xFF;
        }

        /**
         * @brief Increments the strip index and resets at the end
         *
         */
        void increment();

        /**
         * @brief Input a value 0 to 255 to get a color value.
         * The colours are a transition r - g - b - back to r.
         *
         * @param WheelPos a value 0-255
         * @return uint32_t the resulting color value
         */
        uint32_t getWheel(byte WheelPos);


    public:
        /** Contains some extra info about the current active pattern */
        info extraInfo;

        /**
         * @brief Set the number of leds to turn on respective to audio level
         *
         * @param leds the number of leds to turn on
         */
        void setNumAudioLeds(uint8_t leds) {
            this->numAudioLeds = leds;
        }

        /**
         * @brief Set the hue from user selection
         *
         * @param hue the hue to set
         */
        void setInputHue(int hue) {
            this->inputHue = hue;
        }

        /**
         * @brief Get the current index
         *
         * @return uint16_t the current index
         */
        uint16_t getIndex() {
            return index;
        }

        /**
         * @brief Set the current index
         *
         * @param index the current index
         */
        void setIndex(uint16_t index) {
            this->index = index;
        }

        /**
         * @brief Set the strip brightness
         *
         * @param brightness the strip brightness
         */
        void setBrightness(uint8_t brightness) {
            this->brightness = brightness;
        }

        /**
         * @brief Get the strip update interval
         *
         * @return unsigned long the strip update interval
         */
        unsigned long getInterval() {
            return interval;
        }

        /**
         * @brief Sets the update interval in milliseconds
         *
         * @param interval how frequently the strip should update
         */
        void setInterval(unsigned long interval) {
            this->interval = interval;
        }

        /**
         * @brief Get the last update of the strip in millis
         *
         * @return unsigned long the last time the strip updated
         */
        unsigned long getLastUpdate() {
            return lastUpdate;
        }

        /**
         * @brief Set the time the strip last updated, in millis
         *
         * @param update the last time the strip updated
         */
        void setLastUpdate(unsigned long update) {
            lastUpdate = update;
        }

        /**
         * @brief Construct a new Custom Neo Pixel object
         *
         * @param pixels number of pixels in the strip
         * @param pin led data pin
         * @param type led data mode
         */
        CustomNeoPixel(uint16_t pixels, uint8_t pin, uint8_t type)
        :Adafruit_NeoPixel(pixels, pin, type) {
        }

        /**
         * @brief Fill the strip with solid red
         *
         */
        void Red();

        /**
         * @brief Fill the strip with solid green
         *
         */
        void Green();

        /**
         * @brief Fill the strip with solid blue
         *
         */
        void Blue();

        /**
         * @brief Fill the strip with solid white
         *
         */
        void White();

        /**
         * @brief Fill the strip with whatever hue the user has input
         *
         */
        void Hue();

        /**
         * @brief Initialize for a RainbowCyle
         *
         */
        void RainbowCycle();

        /**
         * @brief Update the Rainbow Cycle pattern
         *
         */
        void RainbowCycleUpdate();

        /**
         * @brief Initialize for the July 4th mode
         *
         */
        void JulyFourth();

        /**
         * @brief Update the July 4th mode
         *
         */
        void JulyFourthUpdate();

        /**
         * @brief Initialize the Rainbow Fade pattern
         *
         */
        void RainbowFade();

        /**
         * @brief Update the Rainbow Fade pattern
         *
         */
        void RainbowFadeUpdate();

        /**
         * @brief Init music reactivity changing hue and amount of illuminated LEDs
         *
         */
        void MusicLedHue();

        /**
         * @brief Update strip leds, hue to music
         *
         */
        void MusicLedHueUpdate();

        /**
         * @brief Initialize for music fill pattern
         *
         */
        void MusicFill();

        /**
         * @brief Fill amount of strip depending on music volume
         *
         */
        void MusicFillUpdate();
};
