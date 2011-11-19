//mbed Microcontroller Library
//Pulse Width Modulation Input Interface
//Copyright 2010
//Thomas Hamilton

#include "PwmIn.h"

PwmIn::PwmIn(PinName pwi) : InterruptIn(pwi), PeriodMeasurement(0), PulseWidthMeasurement(1)
{
    mode(PullDown);
    rise(this, &PwmIn::PulseStart);
    fall(this, &PwmIn::PulseStop);
    start();
}

float PwmIn::read()
{
    return (float)PulseWidthMeasurement / PeriodMeasurement;
}

float PwmIn::period()
{
    return (float)PeriodMeasurement / 1000000;
}

int PwmIn::period_ms()
{
    return PeriodMeasurement / 1000;
}

int PwmIn::period_us()
{
    return PeriodMeasurement;
}

float PwmIn::pulsewidth()
{
    return (float)PulseWidthMeasurement / 1000000;
}

int PwmIn::pulsewidth_ms()
{
    return PulseWidthMeasurement / 1000;
}

int PwmIn::pulsewidth_us()
{
    return PulseWidthMeasurement;
}

void PwmIn::PulseStart()
{
    PeriodMeasurement = read_us();
    reset();
}

void PwmIn::PulseStop()
{
    PulseWidthMeasurement = read_us();
}