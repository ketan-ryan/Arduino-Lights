// #include "neopixels.hpp"

// void NeoPatterns::Update() {
//   if((millis() - lastUpdate) > Interval) // time to update
//   {
//       lastUpdate = millis();
//       switch(ActivePattern)
//       {
//           case RAINBOW_CYCLE:
//               RainbowCycleUpdate();
//               break;
//           case FADE:
//               FadeUpdate();
//               break;
//           case RAINBOW_FADE:
//               RainbowFadeUpdate();
//               break;
//           case MUSIC_LED_HUE:
//               MusicLedHueUpdate();
//               break;
//           case PULSE:
//               PulseUpdate();
//               break;
//           case MUSIC_FILL:
//               MusicFillUpdate();
//               break;
//           case JULY_FOURTH:
//               JulyFourthUpdate();
//               break;
//           default:
//               break;
//       }
//   }
// }

// void NeoPatterns::Increment() {
//   if (Direction == FORWARD) {
//     Index++;
//     if (Index >= TotalSteps) {
//       Index = 0;
//       if (OnComplete != NULL) {
//         OnComplete(); // call the completion callback
//       }
//     }
//   }
//   else // Direction == REVERSE
//   {
//     --Index;
//     if (Index <= 0) {
//       Index = TotalSteps-1;
//       if (OnComplete != NULL) {
//         OnComplete(); // call the comlpetion callback
//       }
//     }
//   }
// }

// void NeoPatterns::Reverse() {
//   if (Direction == FORWARD) {
//       Direction = REVERSE;
//       Index = TotalSteps-1;
//   }
//   else {
//       Direction = FORWARD;
//       Index = 0;
//   }
// }

// void NeoPatterns::RainbowCycle(uint8_t interval, uint8_t br, direction dir = FORWARD) {
//   ActivePattern = RAINBOW_CYCLE;
//   Interval = interval;
//   TotalSteps = 255;
//   Index = 0;
//   Direction = dir;
//   brightness = br;
// }

// void NeoPatterns::RainbowCycleUpdate() {
//   for(unsigned int i = 0; i < numPixels(); i++) {
//       setPixelColor(i, Wheel(((i * 256 / numPixels()) + Index) & 255));
//   }
//   show();
//   Increment();
// }