//mbed Microcontroller Library
//Max Botix Ultrasonic Range Finder MB1210 Interface
//Copyright 2010
//Thomas Hamilton

#include "MB1210.h"

MB1210::MB1210(PinName pw, PinName an, PinName tx, PinName rx) :
    OperatingMode(0x00), UnitFactor(1), PwmScalingFactor(17014.5),
    AnalogScalingFactor(1024), Range(0), InterruptBuffer(&Range)
{
    if (rx != NC)
    {
        SerialInput = new Serial(NC, rx);
        SerialInput->baud(9600);
        SerialInput->format(8, Serial::None, 1);
        SerialInput->attach(NULL, Serial::RxIrq);
        OperatingMode = 0x02;
    }
    if (an != NC)
    {
        AnalogInput = new AnalogIn(an);
        OperatingMode = 0x01;
    }
    if (pw != NC)
    {
        PwmInput = new PwmIn(pw);
        OperatingMode = 0x00;
    }
    if (tx != NC)
    {
        SerialOutput = new DigitalOut(tx);
        SerialOutput->write(0);
    }
}
    //constructor dynamically allocates memory and cpu time (interrupts)
    //to input objects depending on how the device is connected

MB1210::~MB1210()
{
    delete PwmInput;
    delete AnalogInput;
    delete SerialOutput;
    delete SerialInput;
    delete this;
}
    //input objects must be deallocated

void MB1210::Velocity(float MetersPerSecond)
{
    PwmScalingFactor = (UnitFactor * MetersPerSecond * 50);
}
    //set the velocity of sound for pwm readings

void MB1210::Voltage(float Volts)
{
    AnalogScalingFactor = (UnitFactor * 3379.2 / Volts);
}
    //set the voltage correction factor for analog readings

void MB1210::Unit(float UnitsPerMeter)
{
    PwmScalingFactor *= (UnitsPerMeter / UnitFactor / 100);
    AnalogScalingFactor *= (UnitsPerMeter / UnitFactor / 100);
    UnitFactor = UnitsPerMeter / 100;
}
    //set the unit factor to return the range in units other than cm

void MB1210::Mode(unsigned char Selection)
{
    OperatingMode = Selection & 0x03;
    if (SerialOutput)
    {
        SerialOutput->write(Selection & 0x04);
    }
        //synchronous modes can only be set if tx pin is connected
    if (SerialInput)
    {
        if (Selection & 0x08)
        {
            SerialInput->attach(this, &MB1210::Interrupt, Serial::RxIrq);
        }
        else
        {
            SerialInput->attach(NULL, Serial::RxIrq);
        }
            //attach or detach the interrupt function
    }
        //interrupts can only be generated if rx pin is connected
}
    //change the operating mode; SerialOutput controls synchronicity

void MB1210::AttachInterruptBuffer(float* Buffer)
{
    InterruptBuffer = Buffer;
}
    //user changes the pointer to their storage area to use the interrupt

void MB1210::AttachInterruptFunction()
{
    if (SerialInput)
    {
        SerialInput->attach(this, &MB1210::Interrupt, Serial::RxIrq);
    }
}
void MB1210::AttachInterruptFunction(void (*Function)())
{
    if (SerialInput)
    {
        SerialInput->attach(Function, Serial::RxIrq);
    }
}
template<class Class>
    void AttachInterruptFunction(Class* Object, void (Class::*Function)())
{
    if (SerialInput)
    {
        SerialInput->attach(Object, Function, Serial::RxIrq);
    }
}
    //overloads start interrupt modes, allowing user all possible options

void MB1210::RequestSyncRead()
{
    if (SerialOutput)
    {
        SerialOutput->write(1);
        wait_us(20);
        SerialOutput->write(0);
    }
}
    //hold pin high for at least 20 us to request a synchronous reading

void MB1210::DiscardSerialBuffer()
{
    while (SerialInput->readable())
    {
        SerialInput->getc();
    }
}
    //read characters from the buffer until it is empty

float MB1210::Read()
{
    switch (OperatingMode)
    {
        case 0x00:
            if (PwmInput)
            {
                return PwmInput->pulsewidth() * PwmScalingFactor;
            }
            else
            {
                return 0;
            }
        case 0x01:
            if (AnalogInput)
            {
                return AnalogInput->read() * AnalogScalingFactor;
            }
            else
            {
                return 0;
            }
        case 0x02:
            if (SerialInput)
            {
                unsigned char i = 0;
                while (SerialInput->readable()
                    && !SerialInput->scanf("R%3f", &Range) && (i < 32))
                {
                    SerialInput->getc();
                    i++;
                }
                    //find R and parse the range out
                return Range * UnitFactor;
            }
            else
            {
                return 0;
            }
        default:
            return 0;
    }
}
    //OperatingMode switches to desired output method;
    //result scaled according to voltage, the speed of sound, and unit

MB1210::operator float()
{
    return Read();
}
    //conversion function acts as shorthand for Read()

void MB1210::Interrupt()
{
    *InterruptBuffer = Read();
    DiscardSerialBuffer();
}
    //this is called whenever an interrupt mode is
    //set and a serial rx interrupt is generated;
    //it writes to the user's data storage area