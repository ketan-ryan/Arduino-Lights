#pragma once

#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>
#include <IRremote.h>
#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Whether the strip is powered off
 *
 * @return true if the strip is powered off
 * @return false if the strip is in any other state
 */
bool isOn();

/**
 * @brief Update state to dynamic or static depending on the mode
 *
 */
void turnOn();

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
 * Clear all LEDs
 **/
void wipe();

/**
 * @brief Returns number of digits in an int.
 * Useful for displaying 0 padded numbers on the LCD screen
 *
 * @param n input int
 * @return number of digits in input
 */
int digits(int n);
