#ifndef MS3DMGX2Library
#define MS3DMGX2Library

#include "stdint.h"
#include "mbed.h"

class MS3DMGX2
{
    private:
    /////////////////////
//Serial PC;
//////////////////////////

        Serial DataLines;
        volatile unsigned char Buffer[87];//volatile is for interrupted access
        volatile unsigned char BufferEnd;
        volatile unsigned char PacketSize;
        bool Checksum(unsigned char Index, unsigned char Length);
        void FillSerialBuffer();

    public:
        MS3DMGX2(PinName tx, PinName rx);
            //serial output, serial input
        ~MS3DMGX2();
            //release resources
        unsigned char BufferStart;
        
        unsigned char CommandByte;
        unsigned char ResponseLength;
        unsigned char Continuous;  
        
        
        bool Readable();
        
        bool Mode(unsigned char Selection);
            //argument sets operating mode;
            //set flag 0x08 to 0 for polled modes, or 1 for interrupt modes;
            //set flag 0x04 to 0 for synchronous modes, or 1 for asynchronous modes;
            //set flags 0x03 to 0 for pwm, 1 for analog, or 2 for serial;
            //asynchronous modes read user input command, measures and calculates
            //output, and writes the data packet to the serial buffer every 10 ms;
            //interrupt modes automatically read the packet into the buffer provided
            //by AttachInterruptBuffer with the selected input method every 10 ms
            //if in an asynchronous mode, or 10 ms after RequestSyncRead is called;
            //interrupt mode interrupts are generated if there are bytes on the
            //serial buffer and are only cleared if the serial buffer is emptied;
            //default is 0
        //void AttachInterruptBuffer(float* Buffer);
            //if interrupts are used, user must provide address to write result to
        //void AttachInterruptFunction();
            //this overload reattaches the native interrupt function
        //void AttachInterruptFunction(void (*Function)());
            //this overload attaches a function to the serial interrupt
        //template<class Class> void AttachInterruptFunction
        //    (Class* Object, void (Class::*Function)());
            //this overload attaches a member function to the serial interrupt;
            //to change the interrupt function, call one of the
            //"AttachInterruptFunction" overloads with pointers to the desired function;
            //changes polled mode to interrupt equivalent;
            //the interrupt will not be cleared until the serial buffer is emptied
        void RequestSyncRead();
            //this tells the device to prepare a synchronous reading;
            //must be called at least 10 ms before the reading is needed;
            //changes asynchronous mode to synchronous equivalent
        void DiscardSerialBuffer();
            //the serial port has a buffer and only the oldest data is read from it;
            //the buffer has limited space and, when full, the newest data is discarded;
            //this method allows the user to empty the buffer of old data so new data is used
        bool Read(float* Data);
            //get a reading from the device in the set mode;
            //RequestSyncRead() must be called at least 10 ms
            //before this method can be called in a synchronous mode;
            //may be called at any time during asynchronous mode;
            //it is assumed that the input buffer has enough
            //room to accomodate the desired data packet
        //operator float*();
            //shorthand for taking a reading;
            //ex: "float reading[packetlength]; reading = MB1210Object;"
};

#endif