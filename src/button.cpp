// Copyright (C) 2025 Teemo Vaas
// 
// General concept from:
// https://github.com/ToyKeeper/anduril
// fsm-events.h: Event-handling functions for SpaghettiMonster.
// Copyright (C) 2017-2023 Selene ToyKeeper
// SPDX-License-Identifier: GPL-3.0-or-later
//
// Implementation 2025 Teemo Vaas 

#include <Arduino.h>
#include "button.h"

/*! \brief Read button states.
            Features: Press and release button debounce, click counting and button hold events.
            Should be called regularily.
    \returns Button related events. According to the event descriptions provided in events.h file.
*/
buttonevent Button::checkButton(){
    uint8_t buttonState=0;
    uint16_t currentMillis=(uint16_t)millis();
  
    if((lastEvent & B_TIMEOUT)>0) //check if the timeout was announced last time, then we can start fresh  
    {
      lastEvent=0;
    }
  
    if((uint16_t)(currentMillis-lastTransitionTime)> DEBOUNCETIME) 
    {    
      if(digitalRead(buttonPin)==LOW){
        buttonState=1;
      }
      else{
        buttonState=0;
      } 
      //detect change
      if(buttonState!=lastButtonState)//transition (every transition produces some kind of event, no doubt)
      {
        lastButtonState=buttonState;
        lastTransitionTime=currentMillis;//reset debounce, release timeout 
        lastRepeatTime=currentMillis;//and hold repeat timer
        if(buttonState==1)
        {
          lastEvent |= B_PRESS;//indicate new press        
          if ((lastEvent & B_COUNT) < (B_COUNT)){lastEvent ++;}  // increase click counter                       
        }
        else //(button release)
        { 
          lastEvent &= ~B_PRESS; //clear PRESS bit (this means also "release")  
          if((!!(lastEvent & (1 << B_HOLD_BIT)))>0)//if hold bit is set. if((lastEvent & B_HOLD)>0) 
          {
            lastEvent |= B_TIMEOUT;  //when releasing from hold there is no more timeout, so set this too immediately
          }
        }     
      }
      else//same key -> release timeout detection and Hold-down detection
      {
        // calculate repeat rate
        uint16_t elapsedHold =uint16_t(currentMillis - lastTransitionTime);
        uint16_t repeatInterval;        
        if (elapsedHold >= (KEY_REPEAT_LEVEL3_TRESHOLD)) {
            repeatInterval = KEY_REPEAT_RATE_LEVEL4;
        } else if (elapsedHold >= KEY_REPEAT_LEVEL2_TRESHOLD) {
            repeatInterval = KEY_REPEAT_RATE_LEVEL3;
        } else if (elapsedHold >= KEY_REPEAT_LEVEL1_TRESHOLD) {
            repeatInterval = KEY_REPEAT_RATE_LEVEL2;
        } else if (elapsedHold >= KEY_HOLD_TRESHOLD) {
            repeatInterval = KEY_REPEAT_RATE_LEVEL1;
        } else {
            repeatInterval = KEY_HOLD_TRESHOLD;
        }

        if(buttonState==0 && (lastEvent !=0) && (currentMillis-lastTransitionTime) > RELEASE_TIMEOUT)//release timeout check
        {              
          lastEvent |= B_TIMEOUT;  //set timeout bit, all other bits should already be taken care of
        }
        else if(buttonState==1 && (uint16_t)(currentMillis-lastRepeatTime)>repeatInterval)//hold-down detection (key is being held down at the moment)
        {
            lastEvent |=B_HOLD; //set hold bit                
            lastRepeatTime=currentMillis;//hold event is announced after every short interval
        }
        else
        {
          return 0;//no event this time(hold down is announced only once in a while, and no key is not announced)
        }
      }
    }
    else//debounce time
    {
      return 0;//no events allowed during button debounce
    }
  if(lastEvent !=0){
    lastEvent|=B_CLICK; //add click bit. Just for the events to look better in the terminal view. And makes it possible to identify release events
  }  
  return lastEvent;
  }