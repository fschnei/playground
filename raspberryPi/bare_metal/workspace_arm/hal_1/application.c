/*
 * main.c
 *
 *  Created on: 23.01.2018
 *      Author: fasch019
 */


#include "hal.h"


unsigned char IncrementInRange(unsigned char OldIndex)
{
	if (OldIndex >= 7)
	{
		return 0;
	}
	else
	{
		return ++OldIndex;
	}
}
unsigned char DecrementInRange(unsigned char OldIndex)
{
    if (OldIndex > 7 || OldIndex == 0)
    {
        return 7;
    }
    else
    {
        return --OldIndex;
    }
}
unsigned char GetNewIndex(unsigned char OldIndex, unsigned char* IndexDir, unsigned char Direction)
{
	// function increments the index and returns the new one (Value from 0 ... 7)
	    // Direction: 0: float 1: increment 2: decrement
	    // *IndexDir: 0: float increment 1: float decrement  -- this is the current float direction

	if (Direction == 0)
	{
		if (*IndexDir == 0)
		{
			// increment
			if (OldIndex >= 7)
			{
				*IndexDir = 1;
				return 6;
			}
			else
			{
				return IncrementInRange(OldIndex);
			}
		}
		else
		{
			// decrement
			if (OldIndex == 0 || OldIndex > 7)
			{
				*IndexDir = 0;
				return 1;
			}
			else
			{
				return DecrementInRange(OldIndex);
			}
		}
	}
	else if (Direction == 1)
	{
		return DecrementInRange(OldIndex);
	}
	else if (Direction == 2)
	{
		return IncrementInRange(OldIndex);
	}
	return 0;
}

void SetLeds(unsigned char Index, unsigned char Inverted)
{
    // function turn leds on and off depending on given Index
    // 
    
    signed int index;
    
    if (Inverted == 0)
    {
        // turn selected led on
        hal_switchLED(Index, 1);
        
        // turn any other led off
        for(index = 0; index < 7; index ++)
        {
            Index = IncrementInRange(Index);
            hal_switchLED(Index, 0);
        }
    }
    else
    {
        // turn selected led off
        hal_switchLED(Index, 0);
        
        // turn any other led on
        for(index = 0; index < 7; index ++)
        {
            Index = IncrementInRange(Index);
            hal_switchLED(Index, 1);
        }
    }
}



void application()
{

	unsigned int index;
	unsigned int index2;
    unsigned char direction;
    unsigned int period;
    unsigned char indexdirection; // 1st derivation
    
    index = 0;
    direction = 0;
    indexdirection = 0;

    
    
    while(1)
    {
    	if (hal_getSwitchState(1) == 1)
        {
            // just run right round
            direction = 1;
        }
        else if (hal_getSwitchState(2) == 1)
        {
            // just run left round
            direction = 2;
        }
        else
        {
            // float both directions
            direction = 0;
        }

        index = GetNewIndex(index, &indexdirection, direction);
        
        if (hal_getButtonState(0) == 0)
        {
			SetLeds(index, hal_getSwitchState(3));
        }
        
        
    	if (hal_getSwitchState(0) == 1)
        {
            period = 500;
        }
        else
        {
            period = 1000;
        }
        
        for(index2 = 0; index2 < period; index2++)
            hal_wait_1ms();
        
        
        // do nothing as long as button is pressed
        while(hal_getButtonState(3) == 1)
            hal_wait_1ms();
        
        
    }
    
}
