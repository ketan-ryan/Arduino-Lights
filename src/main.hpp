#pragma once

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
#include <IRremote.h>
#include <stdlib.h>
#include <stdio.h>

/**
* Convert an IR value to string to be printed
* @param hexVal the IR input
* @returns a human-readable string representing the button pushed,
* or 'bad' if input could not be understood
**/
void codeToStr(long hexVal);

/**
* After determining the index of the string, perform appropriate action
* @param idx the pressed button
**/
void performAction(int idx);

/**
 * Update the strip based on mode
 *
 */
void updateMode();

/**
* Some reusable code for setting up LCD screen
**/
void setupLCD();

/**
 * Simple function to determine whether a mode needs constant updating
 * Returns true when:
 * Theme is default, modes are 4 - 9
 * Theme is Halloween, modes are
 * Theme is Xmas, modes are
 * @returns whether a mode needs constant updating
 */
bool isModeDynamic();

/**
* Clear all LEDs
**/
void wipe();