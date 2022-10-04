#include "main.hpp"
#include "neopatterns.hpp"

#define BACKLIGHT 13 // Backlight pin for LCD screen
#define NUM_LEDS 100 // Number LEDs in strip
#define ENVELOPE A1  // Envelope pin on sound card
#define LED_PIN 6    // Data pin for LED strip
#define IR_PIN 8     // Data pin for IR receiver
#define SENS 25      // Sensitivity of lights

// Define LCD pins
// Parameters:  (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 7, 5, 4, 3, 2);

// Define some NeoPatterns for the two rings and the stick
// as well as some completion routines
NeoPatterns strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Initialize IR Receiver
IRrecv irrecv(IR_PIN);
// Get results
decode_results results;

// List of all possible button inputs
String codes[22] = {"PWR", "VL+", "FNC", "LFT", "PP ", "RGT", "DWN", "VL-", "UP ",
                    "0  ", "EQ ", "ST ", "1  ", "2  ", "3  ", "4  ", "5  ", "6  ", "7  ", "8  ", "9  "};

// Initialize button id to error value
int id = -1;
// Initialize LED mode to default value
short mode = 0;

// Whether we need to update the mode - dynamic modes need this
bool modeChange = false;

// How bright the lights are
int stripBrightness = 200;
// Current hue (valid for modes 8, 9)
int hue = 125;
// Brightness string
char br[4];
// Hue string
char h[4];

const int pause = 1000;
unsigned long previousMillis = 0;

void codeToStr(long hexVal)
{
    switch(hexVal)
    {
        case 0:
            Serial.println("Repeat");
            if(strip.neopx->state == PWR_OFF) {
                // Unsure why but receiver will sometimes receive the repeat pattern,
                // possibly due to noise. Ignore it if power is off to prevent lights
                // from turning on when they shouldn't
                id = -1;
            }
        break;
        // These magic numbers are determined by pressing a button on the remote control
        // and noting down the corresponding output
        case 0xBA45FF00: id = 0; break;
        case 0xB946FF00: id = 1; break;
        case 0xB847FF00: id = 2; break;
        case 0xBB44FF00: id = 3; break;
        case 0xBF40FF00: id = 4; break;
        case 0xBC43FF00: id = 5; break;
        case 0xF807FF00: id = 6; break;
        case 0xEA15FF00: id = 7; break;
        case 0xF609FF00: id = 8; break;
        case 0xE916FF00: id = 9; break;
        case 0xE619FF00: id = 10; break;
        case 0xF20DFF00: id = 11; break;
        case 0xF30CFF00: id = 12; break;
        case 0xE718FF00: id = 13; break;
        case 0xA15EFF00: id = 14; break;
        case 0xF708FF00: id = 15; break;
        case 0xE31CFF00: id = 16; break;
        case 0xA55AFF00: id = 17; break;
        case 0xBD42FF00: id = 18; break;
        case 0xAD52FF00: id = 19; break;
        case 0xB54AFF00: id = 20; break;
        default: id = -1; break;
    }

    lcd.setCursor(7, 1);

    // If ID is unchanged, we got a bad input
    if (isOn())
    {
        if (id == -1)
            lcd.print("Bad");
        else
        {
            lcd.print(codes[id]);
        }
    }
}

int digits(int n)
{
    int count = 0;
    do
    {
        n /= 10;
        ++count;
    } while (n != 0);
    return count;
}

bool isOn()
{
    return strip.neopx->state != PWR_OFF;
}

void turnOn()
{
    strip.neopx->state = strip.neopx->flags.dynamic ? ON_DYNAMIC : ON_STATIC;
}

void performAction(int idx)
{
    if (idx == -1)
        return;

    // Power button pressed
    if (idx == 0)
    {
        if (isOn())
        {
            strip.neopx->state = PWR_OFF;
            lcd.clear();
            wipe();
        }
        else
        {
            setupLCD();
            turnOn();
        }
    }
    if (isOn())
    {
        if(idx == 2) {
            strip.incrementPage();
            performAction(9);
        }
        // Hue shifting
        if (strip.neopx->flags.hue)
        {
            // Hue shifting left
            if (idx == 3)
            {
                if (hue > 0)
                    hue--;
                else
                    hue = 255;
            }
            // Hue shifting right
            else if (idx == 5)
            {
                if (hue < 255)
                    hue++;
                else
                    hue = 0;
            }
        }
        // Volume up button pressed
        if (idx == 1)
        {
            if (stripBrightness + 10 < 255)
                stripBrightness += 10;
            else if (stripBrightness + 10 >= 255)
                stripBrightness = 0;
        }
        // Play/pause button pressed
        else if (idx == 4)
        {
            if (strip.neopx->state == ON_DYNAMIC)
            {
                strip.neopx->state = PAUSED;
            }
            else
            {
                turnOn();
            }
        }
        // Down button pressed
        else if (idx == 6)
        {
            if (mode == 0)
                mode = 9;
            else
                mode--;

            updateMode();
            turnOn();
        }
        // Volume down button pressed
        else if (idx == 7)
        {
            if (stripBrightness - 10 > 0)
                stripBrightness -= 10;
            else if (stripBrightness - 10 <= 0)
                stripBrightness = 255;
        }
        // Up button pressed
        else if (idx == 8)
        {
            if (mode == 9)
                mode = 0;
            else
                mode++;

            updateMode();
            turnOn();
        }
        // Map button index to mode index
        else if (idx == 9 || (idx >= 12 && idx <= 20))
        {
            if (idx == 9)
                mode = 0;
            else
                mode = idx - 11;

            updateMode();
            turnOn();
        }

        // Print mode
        lcd.setCursor(5, 0);
        lcd.print(strip.currentPage);
        lcd.setCursor(7, 0);
        lcd.print(mode);

        // Print brightness
        snprintf(br, sizeof(br), "%03d", stripBrightness);
        lcd.setCursor(13, 0);
        lcd.print(br);

        // Zero pad hue and print
        snprintf(h, sizeof(h), "%03d", hue);
        lcd.setCursor(13, 1);
        lcd.print("000");
        int pos = 16 - digits(hue);
        lcd.setCursor(pos, 1);
        lcd.print(hue);

        updateMode();

        if (strip.neopx->flags.dynamic)
            strip.update(mode);
    }
    idx = -1;
}

void updateMode()
{
    if(mode + 1 > strip.pages[strip.currentPage].modes) {
        mode = strip.pages[strip.currentPage].modes - 1;
    }

    strip.init(mode, stripBrightness, hue);
    strip.neopx->show();
}

void setupLCD()
{
    lcd.clear();

    snprintf(br, sizeof(br), "%03d", stripBrightness);
    snprintf(h, sizeof(h), "%03d", hue);

    lcd.setCursor(13, 0);
    lcd.print(br);

    lcd.setCursor(0, 0);
    lcd.print("Mode: .  Br: ");

    lcd.setCursor(5, 0);
    lcd.print(strip.currentPage);
    lcd.setCursor(7, 0);
    lcd.print(mode);

    lcd.setCursor(0, 1);
    lcd.print("Input:");

    lcd.setCursor(11, 1);
    lcd.print("H: ");
    lcd.setCursor(13, 1);
    lcd.print(hue);
}

void wipe()
{
    for (unsigned int i = 0; i < strip.neopx->numPixels(); i++)
    {
        strip.neopx->setPixelColor(i, strip.neopx->Color(0, 0, 0));
    }
    strip.neopx->show();
}

void setup()
{
    Serial.begin(9600);

    // Enable backlight
    pinMode(BACKLIGHT, OUTPUT);
    digitalWrite(BACKLIGHT, 1);

    // Start IR Receiver
    irrecv.enableIRIn();

    // Start LCD screen
    lcd.begin(16, 2);
    setupLCD();

    // Initialize all the pixelStrips
    strip.neopx->begin();

    // Start on default mode 0
    updateMode();
    turnOn();
}

// Main loop
void loop()
{
    digitalWrite(BACKLIGHT, isOn() ? 1 : 0);

    // Store index before receiving a signal
    int tempI = strip.neopx->getIndex();

    // If we get something from the receiver
    if (irrecv.decode())
    {
        // Any signal received during a dynamically updating pattern will be malformed, so we need a second input
        if (strip.neopx->flags.dynamic)
        {
            modeChange = true;
        }

        // Print input
        codeToStr(irrecv.decodedIRData.decodedRawData);

        // Do the appropriate action depending on the pressed button
        performAction(id);

        // Restore signal after updating mode
        if (id != mode)
            strip.neopx->setIndex(tempI);

        // Resume receiving input
        irrecv.resume();
    }

    // Get input from sound card, manipulate it to fit our number of leds
    if (strip.neopx->flags.sound)
    {
        int input = analogRead(ENVELOPE);
        Serial.println(input);
        unsigned int reading = (input * input) / SENS;
        strip.neopx->setNumAudioLeds(reading);
    }

    if (strip.neopx->flags.hue)
        strip.neopx->setInputHue(hue);

    strip.neopx->setBrightness(stripBrightness);

    // Give users one second to input another IR signal
    unsigned long currentMillis = millis();
    if (modeChange)
    {
        if (currentMillis - previousMillis >= pause)
        {
            previousMillis = currentMillis;
            modeChange = false;
        }
    }

    // Update strip if dynamic and user has not requested an input
    if (strip.neopx->state == ON_DYNAMIC && !modeChange)
    {
        strip.update(mode);
    }
}
