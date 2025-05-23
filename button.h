#ifndef KEYPAD_H
#define KEYPAD_H
// Copyright (C) 2025 Teemo Vaas
//
// General concept of bit patterns from:
// https://github.com/ToyKeeper/anduril
// fsm-events.h: Event-handling functions for SpaghettiMonster.
// Copyright (C) 2017-2023 Selene ToyKeeper
// SPDX-License-Identifier: GPL-3.0-or-later
//
// This specific implementation 2024-2025 Teemo Vaas
//features: Press and release button debounce, click counting and button hold events.With multi speed repeat for hold down.

#include <Arduino.h>

// typedefs
typedef uint8_t buttonevent;

/* Event structure
 * Bit  7: 1 for a button input event, 0 for all others.
 * If bit 7 is 1:
 *     Bits 0,1,2,3:  Click counter.  Up to 15 clicks.
 *     Bit  4: 1 for a "press" event, 0 for a "release" event.
 *     Bit  5: 1 for a "hold" event, 0 otherwise.
 *     Bit  6: 1 for a "timeout" event, 0 otherwise.
 * If bit 7 is 0:
 *     Sort of ad-hoc, shown in #defines below.
 */

// event masks / bits
#define B_SYSTEM 0b00000000
#define B_CLICK 0b10000000
#define B_TIMEOUT 0b01000000
#define B_HOLD 0b00100000
#define B_PRESS 0b00010000
#define B_RELEASE 0b00000000
#define B_COUNT 0b00001111
#define B_FLAGS 0b11110000

#define B_CLICK_BIT 7  //    0b10000000
#define B_TIMEOUT_BIT 6  //    0b01000000
#define B_HOLD_BIT 5  //    0b00100000
#define B_PRESS_BIT 4  //    0b00010000
//B_RELEASE just clears the B_PRESS_BIT
//checking thhe B_PRESS_BIT can be used for general button press, with variable speed hold down repeat for scrolling etc.
//B_TIMEOUT bit actually indicates the end of the button sequence.

// Event types
#define EV_none 0

// Button press events
#define EV_click1_press (B_CLICK | B_PRESS | 1)
#define EV_click1_release (B_CLICK | B_RELEASE | 1)
#define EV_click1_complete (B_CLICK | B_TIMEOUT | 1)
#define EV_1click EV_click1_complete
#define EV_click1_hold (B_CLICK | B_HOLD | B_PRESS | 1)
#define EV_click1_hold_release (B_CLICK | B_HOLD | B_RELEASE | B_TIMEOUT | 1)
#define EV_hold EV_click1_hold

#define EV_click2_press (B_CLICK | B_PRESS | 2)
#define EV_click2_release (B_CLICK | B_RELEASE | 2)
#define EV_click2_complete (B_CLICK | B_TIMEOUT | 2)
#define EV_2clicks EV_click2_complete
#define EV_click2_hold (B_CLICK | B_HOLD | B_PRESS | 2)
#define EV_click2_hold_release (B_CLICK | B_HOLD | B_RELEASE | B_TIMEOUT | 2)

//various timing constants
#define DEBOUNCETIME 110  //debounce blocking time [ms]

#define RELEASE_TIMEOUT 288  //[ms], this is the time after which the click counter is reset back to 0

#define KEY_HOLD_TRESHOLD DEBOUNCETIME + 275  //[ms] //from this press becomes hold. Anduril default:384
#define KEY_REPEAT_LEVEL1_TRESHOLD KEY_HOLD_TRESHOLD + 175  //[ms]
#define KEY_REPEAT_LEVEL2_TRESHOLD KEY_REPEAT_LEVEL1_TRESHOLD + 500  //[ms]
#define KEY_REPEAT_LEVEL3_TRESHOLD KEY_REPEAT_LEVEL2_TRESHOLD + 1500  //[ms]

#define KEY_REPEAT_RATE_LEVEL1 275  //[ms] default 175, this determines how often event is announced during button hold
#define KEY_REPEAT_RATE_LEVEL2 184  //[ms]
#define KEY_REPEAT_RATE_LEVEL3 21  //[ms]
#define KEY_REPEAT_RATE_LEVEL4 7  //[ms]

class Button {
  private:
    uint8_t buttonPin;
    uint8_t lastButtonState;
    uint16_t keyStartTime;
    uint16_t lastTransitionTime;
    uint16_t lastRepeatTime;  //used for repeating the hold event
    buttonevent lastEvent;    //used to count clicks
  public:
    Button(uint8_t pin) : buttonPin(pin), lastButtonState(0) {
        pinMode(buttonPin, INPUT_PULLUP);
    };
    buttonevent checkButton();
};

#endif  // KEYPAD_H