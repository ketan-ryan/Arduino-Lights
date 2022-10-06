#include "neopatterns.hpp"

void NeoPatterns::setup()
{
    pages[0].modes = 10;
    // First page initializers
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

    // First page updates
    pages[0].updates[4] = &CustomNeoPixel::RainbowFadeUpdate;
    pages[0].updates[5] = &CustomNeoPixel::RainbowCycleUpdate;
    pages[0].updates[6] = &CustomNeoPixel::MusicLedHueUpdate;
    pages[0].updates[7] = &CustomNeoPixel::JulyFourthUpdate;
    pages[0].updates[9] = &CustomNeoPixel::MusicFillUpdate;

    pages[1].modes = 2;
    // Second page initializers
    pages[1].initializers[0] = &CustomNeoPixel::HalloweenFade;
    pages[1].initializers[1] = &CustomNeoPixel::HalloweenStrip;

    // Second page updates
    pages[1].updates[0] = &CustomNeoPixel::HalloweenFadeUpdate;
    pages[1].updates[1] = &CustomNeoPixel::HalloweenStripUpdate;
}

void NeoPatterns::init(int pattern, uint8_t br, uint8_t hue)
{
    neopx->flags.dynamic = false;
    neopx->flags.hue = false;
    neopx->flags.sound = false;

    neopx->setInterval(10);
    neopx->setBrightness(br);
    neopx->setInputHue(hue);

    pages[currentPage].init(pattern);
}

void NeoPatterns::update(int pattern)
{
    if ((millis() - neopx->getLastUpdate()) > neopx->getInterval())
    {
        neopx->setLastUpdate(millis());
        pages[currentPage].update(pattern);
    }
}

void NeoPatterns::incrementPage()
{
    if (currentPage + 1 > numPages)
        currentPage = 0;
    else
        currentPage++;
}
