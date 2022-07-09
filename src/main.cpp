#include "main.hpp"
#include "neopatterns.hpp"

#define BACKLIGHT 13   // Backlight pin for LCD screen
#define NUM_LEDS 100   // Number LEDs in strip
#define ENVELOPE A1    // Envelope pin on sound card
#define LED_PIN 6      // Data pin for LED strip
#define IR_PIN 8       // Data pin for IR receiver
#define SENS 25        // Sensitivity of lights

// Define LCD pins
// Parameters:  (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 7, 5, 4, 3, 2);

// Define some NeoPatterns for the two rings and the stick
// as well as some completion routines
NeoPatterns strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800, NULL);//, &Ring1Complete);

//Initialize IR Receiver
IRrecv irrecv(IR_PIN);
// Get results
decode_results results;

//If the backlight is turned on
bool powerOn = true;

//List of all possible button inputs
String codes[22] = {"PWR", "VL+", "FNC", "LFT", "PP ", "RGT", "DWN", "VL-", "UP ",
"0  ", "EQ ", "ST ", "1  ", "2  ", "3  ", "4  ", "5  ", "6  ", "7  ", "8  ", "9  "};

/**
 * Possible strip modes
 * 0: Red
 * 1: Grn
 * 2: Blu
 * 3: Wht
 * 4: Rainbow fade
 * 5: Rainbow chase
 * 6: Music bar fill, rainbow
 * 7: July 4th
 * 8: Adjust hue, solid
 * 9: Adjust hue, music
**/
short modes[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

// Initialize button id to error value
int id = -1;
// Initialize LED mode to default value
short mode = 0;

// Whether we need to update the mode - dynamic modes need this
bool modeChange = false;

// How bright the lights are
int stripBrightness = 200;
// Current hue (valid for modes 8, 9)
int hue = 235;
// Brightness string
char br[4];
// Hue string
char h[4];

const int pause = 1000;
unsigned long previousMillis = 0;

void codeToStr(long hexVal) {
  switch(hexVal) {
    case 0:
      Serial.println("Repeat");
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

    //If ID is unchanged, we got a bad input
    if(id == -1)
      lcd.print("Bad");
    else {
      lcd.print(codes[id]);
    }
}

void performAction(int idx) {
    // Power button pressed
    if (idx == 0) {
        powerOn = !powerOn;
        if(!powerOn) {
          lcd.clear();
          wipe();
        }
        else {
          setupLCD();
          updateMode();
          strip.neopx->show();
        }
    }
    if(powerOn) {
      //Hue shifting
      if(mode == 8 || mode == 9) {
        //Hue shifting left
        if(idx == 3) {
          if(hue > 0)
            hue--;
          else
            hue = 255;
        }
        //Hue shifting right
        else if(idx == 5) {
          if (hue < 255)
            hue++;
          else
            hue = 0;
        }
      }
      // Volume up button pressed
      if (idx == 1) {
        if(stripBrightness + 10 < 255)
          stripBrightness += 10;
        else if(stripBrightness + 10 >= 255)
          stripBrightness = 0;
      }
      // Down button pressed
      else if (idx == 6) {
        if(mode == 0)
          mode = 9;
        else
          mode--;
      }
      // Volume down button pressed
      else if(idx == 7) {
        if(stripBrightness - 10 > 0)
          stripBrightness -= 10;
        else if(stripBrightness - 10 <= 0)
          stripBrightness = 255;
      }
      // Up button pressed
      else if (idx == 8) {
        if (mode == 9)
          mode = 0;
        else
          mode++;
      }
      // Map button index to mode index
      else if (idx == 9 || (idx >= 12 && idx <= 20)) {
        if (idx == 9)
          mode = 0;
        else
          mode = idx - 11;
      }

      //Print mode
      lcd.setCursor(6, 0);
      lcd.print(mode);

      //Print brightness
      snprintf(br, sizeof(br), "%03d", stripBrightness);
      lcd.setCursor(13, 0);
      lcd.print(br);

      snprintf(h, sizeof(h), "%03d", hue);
      lcd.setCursor(13, 1);
      lcd.print(hue);

      updateMode();
    }
}

void updateMode() {
  if(mode <= 4 || mode == 6 || mode == 7) {
    strip.init(mode, stripBrightness, 0, 0);
  } else if(mode == 5) {
    strip.init(mode, stripBrightness, 100, 0);
  } else if(mode >= 8) {
    strip.init(mode, stripBrightness, 0, hue);
  }
  strip.neopx->show();
}

void setupLCD() {
  lcd.clear();

  snprintf(br, sizeof(br), "%03d", stripBrightness);
  snprintf(h, sizeof(h), "%03d", hue);

  lcd.setCursor(13, 0);
  lcd.print(br);

  lcd.setCursor(0, 0);
  lcd.print("Mode:    Br: ");

  lcd.setCursor(6, 0);
  lcd.print(mode);

  lcd.setCursor(0, 1);
  lcd.print("Input:");

  lcd.setCursor(11, 1);
  lcd.print("H: ");
  lcd.setCursor(13, 1);
  lcd.print(hue);
}

bool isModeDynamic() {
  return ((mode >= 4 && mode <= 7) || mode == 9);
}

void wipe() {
  for(unsigned int i = 0; i < strip.neopx->numPixels(); i++) {
    strip.neopx->setPixelColor(i, strip.neopx->Color(0, 0, 0));
  }
  strip.neopx->show();
}


void setup()
{
  Serial.begin(9600);

  // Enable backlight
  pinMode(BACKLIGHT, OUTPUT);
  digitalWrite(BACKLIGHT, powerOn);

  // Start IR Receiver
  irrecv.enableIRIn();

  // Start LCD screen
  lcd.begin(16,2);
  setupLCD();

  // Initialize all the pixelStrips
  strip.neopx->begin();
  // Start on default mode 0
  updateMode();
}

// Main loop
void loop()
{
  digitalWrite(BACKLIGHT, powerOn);

  // Get input from sound card, manipulate it to fit our number of leds
  if(mode == 6 || mode == 7 || mode == 9) {
    int input = analogRead(ENVELOPE);
    Serial.println(input);
    unsigned int reading = (input * input) / SENS;
    strip.neopx->numLeds = (reading);
  }

  if(mode == 9)
    strip.neopx->Color2 = hue;

  // Store index before receiving a signal
  int tempI = strip.neopx->Index;

  // If we get something from the receiver
  if(irrecv.decode()) {
    // Any signal received during a dynamically updating pattern will be malformed, so we need a second input
    if(isModeDynamic()) {
      modeChange = true;
    }

    // Print input
    if(powerOn)
      codeToStr(irrecv.decodedIRData.decodedRawData);

    // Do the appropriate action depending on the pressed button
    performAction(id);

    // Restore signal after updating mode
    if(id != mode)
      strip.neopx->Index = tempI;

    // Resume receiving input
    irrecv.resume();
  }

  // Give users one second to input another IR signal
  unsigned long currentMillis = millis();
  if(modeChange) {
    if(currentMillis - previousMillis >= pause) {
      previousMillis = currentMillis;
      modeChange = false;
    }
  }

  // Update strip if dynamic and user has not requested an input
  if(powerOn && !modeChange && isModeDynamic()) {
    strip.neopx->brightness = stripBrightness;
    strip.update(mode);
  }
}