//mbed Microcontroller Library
//Pulse Width Modulation Input Interface
//Copyright 2010
//Thomas Hamilton

#ifndef PwmInLibrary
#define PwmInLibrary

#include "stdint.h"
#include "mbed.h"

class PwmIn : private InterruptIn, Timer
{
    private:
        unsigned int PeriodMeasurement;
        unsigned int PulseWidthMeasurement;

        void PulseStart();
        void PulseStop();

    public:
        PwmIn(PinName pwi);
        float read();
        float period();
        int period_ms();
        int period_us();
        float pulsewidth();
        int pulsewidth_ms();
        int pulsewidth_us();
};

#endif