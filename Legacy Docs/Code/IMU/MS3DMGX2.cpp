#include "MS3DMGX2.h"

MS3DMGX2::MS3DMGX2(PinName tx, PinName rx) : DataLines(tx, rx),
    CommandByte(0xCF), ResponseLength(6), PacketSize(31), Continuous(0)//,
    
    ///////////////////
    //PC(USBTX,USBRX)
    //////////////////
    
{
    DataLines.baud(115200);
    DataLines.format(8, Serial::None, 1);
    DataLines.attach(this, &MS3DMGX2::FillSerialBuffer, Serial::RxIrq);
        
        ////////////////////
    //PC.baud(9600);
    ////////////////////////
}

MS3DMGX2::~MS3DMGX2()
{
    delete this;
}

bool MS3DMGX2::Mode(unsigned char Selection)
{
    bool Result;
    switch (Selection & 0x03)
    {
        case 0x00:
            CommandByte = 0xCF;
            ResponseLength = 6;
            PacketSize = 31;
            break;
                //euler angles and angular rates
        case 0x01:
            CommandByte = 0xD2;
            ResponseLength = 9;
            PacketSize = 43;
            break;
                //gyro-stabilized acceleration, angular rate and magnetometer vector
        case 0x02:
            CommandByte = 0xC8;
            ResponseLength = 15;
            PacketSize = 67;
            break;
                //acceleration, angular rate and orientation matrix
        case 0x03:
            CommandByte = 0xCC;
            ResponseLength = 18;
            PacketSize = 79;
            break;
                //acceleration, angular rate, magnetometer vector, and orientation matrix
    }
        //record desired packet command and packet length as number of 16 bit fields
    if (Selection & 0x04)
    {
        unsigned char lbuff = PacketSize;
        PacketSize = 87;
        
        DataLines.putc(0xC4);
        DataLines.putc(0xC1);
        DataLines.putc(0x29);
        DataLines.putc(CommandByte);
            //send the desired continuous mode command
        Continuous = 1;
            //set synchronous mode to true
        while ((Buffer[BufferStart] != 0xC4) || (Buffer[BufferStart + 1] != CommandByte))
        {
            if (((BufferStart + 2) % PacketSize) != BufferEnd)
            {
                BufferStart++;
                BufferStart%=PacketSize;
            }
        }
            //find the response header
        while (((BufferStart + 8) % PacketSize) != BufferEnd);
        BufferEnd = 0;
        Result = Checksum(BufferStart, 8);
        BufferStart = 0;
        PacketSize = lbuff;////////////////NOTE: if there wasn't a packet waiting while the async cmd was made, there will be no "extra data" here
    }
    else
    {
        if (Continuous)
        {
            DataLines.putc(0xFA);
                //send stop continuous mode command
            Continuous = 0;
                //set synchronous mode to true
        }
        Result = 1;
    }
        //put the IMU into continuous mode if the correct flag is set
        //Computer.printf("stop command success %d \n", Workspace[0]);
        //Computer.printf("sync mode %d \n", SyncMode);
    /*if (Selection & 0x08)
    {
        DataLines.attach(this, &MS3DMGX2::Interrupt, Serial::RxIrq);
            //attach automatic buffer-writing function to serial interrupt
    }
    else
    {
        DataLines.attach(NULL, Serial::RxIrq);
            //attach a null to detach any previous interrupt
    }*/
        //attaches or detaches interrupt function depending on interrupt flag
    return Result;
        //return success or failure
}

bool MS3DMGX2::Readable()
{
    return BufferStart == BufferEnd;
}

/*void MS3DMGX2::AttachInterruptBuffer(float* Buffer)
{
    InterruptBuffer = Buffer;
}
    //store user's data pointer for use in interrupt modes

void MS3DMGX2::AttachInterruptFunction()
{
    DataLines.attach(this, &MS3DMGX2::Interrupt, Serial::RxIrq);
}
void MS3DMGX2::AttachInterruptFunction(void (*Function)())
{
    DataLines.attach(Function);
}
template<class Class> void AttachInterruptFunction(Class* Object, void (Class::*Function)())
{
    DataLines.attach(Object, Function, Serial::RxIrq);
}*/
    //overloads start interrupt modes, allowing user all possible options

void MS3DMGX2::RequestSyncRead()
{
    if (Continuous)
    {
        DataLines.putc(0xFA);
        Continuous = 0;
    }
    DataLines.putc(CommandByte);
}
    //lazy switches to synchronous mode and sends polled mode command byte

bool MS3DMGX2::Read(float* Data)
{
    bool Result;
    unsigned char t = 0;
    
    while ((Buffer[BufferStart] != CommandByte) && (t < PacketSize))
    {
        BufferStart++;
        BufferStart %= PacketSize;
        t++;
    }
        //find the header byte
    Result = Checksum(BufferStart, PacketSize);
        //compute checksum
    BufferStart++;
    BufferStart %= PacketSize;
        //move past header byte
    if (t < PacketSize)
    {
        for (unsigned int i = 0; i < ResponseLength; i++)
        {
            for(unsigned int j = 3; j < 4; j--)
            {
                ((unsigned char*)&Data[i])[j] = Buffer[BufferStart];
                BufferStart++;
                BufferStart %= PacketSize;
            }
        }
            //convert big endian bytes to little endian floats
        BufferStart += 6;
            //move index past timer and checksum bytes
        BufferStart %= PacketSize;
    }
        //if the header search did not timeout
    
    return Result;
}

/*MS3DMGX2::operator float*()
{
    Read((float*)&Workspace[8]);
    return (float*)&Workspace[8];
}
    //conversion function acts as shorthand for Read()

void MS3DMGX2::Interrupt()
{
    Read(InterruptBuffer);
}*/
    //this will be called when in an interrupt mode and a serial interrupt is generated

bool MS3DMGX2::Checksum(unsigned char Index, unsigned char Length)
{
    unsigned short Sum = 0;
    for (unsigned char i = 0; i < Length - 2; i++)
    {
        Sum += Buffer[Index];
        Index++;
        Index %= PacketSize;
    }
    return (((unsigned char*)&Sum)[0] == Buffer[Index+1])
        && (((unsigned char*)&Sum)[1] == Buffer[Index]);
}

void MS3DMGX2::FillSerialBuffer()//if the interrupt recurs faster than the time to complete its code, the rest of the system will be suspended indefinitely
{
    while (DataLines.readable())
    {
        Buffer[BufferEnd] = DataLines.getc();
        BufferEnd++;
       
        BufferEnd %= PacketSize;
    }
}
    //this automatically reads in new serial data as it is received to
    //make a software buffer that is big enough to handle an entire
    //packet; the processor is about 1000 times faster than the data lines