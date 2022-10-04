#pragma once

#include <Arduino.h>

#include "neopixel.hpp"

/**
 * @brief A collection of up to 9 modes for the LED strip.
 * Created because the IR remote only has buttons for 0-9.
 * Allows us to intuitively use the remote with > 9 modes.
 */
class Page
{
public:
    int modes = 0;

    /** Reference to the LED strip */
    CustomNeoPixel *strip;

    /**
     * @brief Construct a new Page object
     *
     * @param st the LED strip to whom the page belongs to
     */
    Page(CustomNeoPixel *st)
    {
        strip = st;
    }

    /** Array of function pointers to init functions */
    void (CustomNeoPixel::*initializers[10])();

    /** Array of function pointers to update functions */
    void (CustomNeoPixel::*updates[10])();

    /**
     * @brief Initializer function for a mode in the page
     *
     * @param pattern the index of pattern to initialize
     */
    void init(int pattern)
    {
        (strip->*initializers[pattern])();
    }

    /**
     * @brief Updates the selected pattern in the page
     *
     * @param pattern the index of the pattern in the page
     */
    void update(int pattern)
    {
        (strip->*updates[pattern])();
    };
};

/**
 * @brief Controls pages for the strip.
 * Holds all pages, the associated modes for each page, and the current page.
 */
class NeoPatterns
{
public:
    /** The number of pages associated with the strip */
    int numPages = 4;
    /** The index of the current page */
    int currentPage = 0;

    /** A pointer to the active strip */
    CustomNeoPixel *neopx;

    /**
     * @brief Construct a new Neo Patterns object
     *
     * @param pixels number of pixels in strip
     * @param pin data pin for led strip
     * @param type strip modes
     * @param callback completion callback, if any
     */
    NeoPatterns(uint16_t pixels, uint8_t pin, uint8_t type)
        : neopx(new CustomNeoPixel(pixels, pin, type))
    {
        setup();
    }

    /** Array of all pages associated with the strip */
    Page pages[4] = {Page(neopx), Page(neopx), Page(neopx), Page(neopx)};

    /**
     * @brief Assign modes to pages
     *
     */
    void setup();

    /**
     * @brief Calls Page.init() method for current page
     *
     * @param pattern index of the current pattern in the current page
     * @param br strip brightness
     * @param hue current hue
     */
    void init(int pattern, uint8_t br, uint8_t hue);

    /**
     * @brief Calls page.update() method for current page
     *
     * @param pattern index of the current pattern in the curren tpage
     */
    void update(int pattern);

    /**
     * @brief Shifts to the next page in the array.
     * Wraps back to the beginning if already on the last page when called.
     */
    void incrementPage();
};
