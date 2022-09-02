#include "neopixel.hpp"

void CustomNeoPixel::colorSet(uint32_t color) {
    for (unsigned int i = 0; i < numPixels(); i++) {
        setPixelColor(i, color);
    }
    show();
}

void CustomNeoPixel::increment() {
    if (dir == FORWARD) {
        index++;
        if (index >= totalSteps) {
            index = 0;
        }
    }
    else {
        --index;
        if (index <= 0) {
            index = totalSteps - 1;
        }
    }
}

uint32_t CustomNeoPixel::getWheel(byte wheelPos) {
    wheelPos = 255 - wheelPos;
    double factor = (brightness / 255.0);
    if(wheelPos < 85) {
        return Color((255 - wheelPos * 3) * factor, 0, (wheelPos * 3) * factor);
    }
    else if(wheelPos < 170) {
        wheelPos -= 85;
        return Color(0, (wheelPos * 3) * factor, (255 - wheelPos * 3) * factor);
    }
    else {
        wheelPos -= 170;
        return Color((wheelPos * 3) * factor, (255 - wheelPos * 3) * factor, 0);
    }
}

void CustomNeoPixel::Red() {
    colorSet(Color(brightness, 0, 0));
    show();
}

void CustomNeoPixel::Green() {
    colorSet(Color(0, brightness, 0));
    show();
}

void CustomNeoPixel::Blue() {
    colorSet(Color(0, 0, brightness));
    show();
}

void CustomNeoPixel::White() {
    colorSet(Color(brightness, brightness, brightness));
    show();
}

void CustomNeoPixel::Hue() {
    extraInfo.hue = true;

    colorSet(gamma32(ColorHSV(map(inputHue, 0, 255, 0, 65535), 255, brightness)));
    show();
}

void CustomNeoPixel::RainbowCycle() {
    index = 0;
    totalSteps = 255;
    dir = FORWARD;

    extraInfo.dynamic = true;
}

void CustomNeoPixel::RainbowCycleUpdate() {
    for(unsigned int i = 0; i < numPixels(); i++) {
        setPixelColor(i, getWheel(((i * 256 / numPixels()) + index) & 255));
    }
    show();
    increment();
}

void CustomNeoPixel::JulyFourth() {
    index = 0;
    totalSteps = 450;
    dir = FORWARD;

    extraInfo.dynamic = true;
}

void CustomNeoPixel::JulyFourthUpdate() {
    uint32_t one, two, three;
    uint32_t red = Color(brightness, 0, 0);
    uint32_t blue = Color(0, brightness, 0);
    uint32_t white = Color(brightness, brightness, brightness);

    if(index <= 150) {
        one = red; two = blue; three = white;
    } else if(index <= 300) {
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
    increment();
}

void CustomNeoPixel::RainbowFade() {
    index = 0;
    totalSteps = 16383;
    dir = FORWARD;

    extraInfo.dynamic = true;
}

void CustomNeoPixel::RainbowFadeUpdate() {
    colorSet(gamma32(ColorHSV(map(index, 0, totalSteps, 0, 65535), 255, brightness)));
    show();
    increment();
}

void CustomNeoPixel::MusicLedHue() {
    index = 0;
    totalSteps = 65535;

    extraInfo.dynamic = true;
    extraInfo.sound = true;
}

void CustomNeoPixel::MusicLedHueUpdate() {
    // Turn on number of LEDs respective to how loud it is
    for(unsigned int i = 0; i < numPixels(); i++) {
        if (numAudioLeds > 1 && i <= numAudioLeds) {
            // Set HSV value of current pixel, with hue constantly updating and based on position in strip
            setPixelColor(i, gamma32(ColorHSV(map(i + index, 0, numPixels(), 0, 65535), 255, brightness)));
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
    if(numAudioLeds <= 20) {
        rateOfChange = 50;
    }
    else if(numAudioLeds > 20 && numAudioLeds <= 40) {
        rateOfChange = 35;
    }
    else if(numAudioLeds > 40 && numAudioLeds <= 60) {
        rateOfChange = 25;
    }
    else if(numAudioLeds > 60 && numAudioLeds <= 80) {
        rateOfChange = 10;
    }
    else if(numAudioLeds > 80 && numAudioLeds <= 90) {
        rateOfChange = 5;
    }
    else if(numAudioLeds > 90) {
        rateOfChange = 1;
    }

    if(millis() % rateOfChange == 0) {
        increment();
    }
}

void CustomNeoPixel::MusicFill() {
    index = 0;
    totalSteps = 65535;

    extraInfo.dynamic = true;
    extraInfo.sound = true;
    extraInfo.hue = true;
}

void CustomNeoPixel::MusicFillUpdate() {
    // Turn on number of LEDs respective to how loud it is
    for(unsigned int i = 0; i < numPixels(); i++) {
        if (numAudioLeds > 1 && i <= numAudioLeds) {
            // Set HSV value of current pixel, with hue determined by user input
            setPixelColor(i, gamma32(ColorHSV(map(inputHue, 0, 255, 0, 65535), 255, brightness)));
        }
        else {
            setPixelColor(i, gamma32(ColorHSV(map(inputHue + 25, 0, 255, 0, 65535), 255, brightness)));
        }
    }
    show();
}
