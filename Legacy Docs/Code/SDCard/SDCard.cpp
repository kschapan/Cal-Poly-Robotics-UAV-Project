//mbed Microcontroller Library
//SDCard Interface
//Copyright 2010
//Thomas Hamilton

#include "SDCard.h"

SDCard::SDCard(PinName mosi, PinName miso, PinName sck, PinName cs,
    const char* DiskName) :
    FATFileSystem(DiskName), DataLines(mosi, miso, sck), ChipSelect(cs),
    t(0), Timeout(1024), CRCMode(1), Capacity(0), Version(0), Status(0x00)
    //card always starts uninitialized and in CRC mode; version 1 low-capacity
    //card protocols are backwards-compatible with all other card protocols
    /////////////////////////////////
    ,Computer(USBTX,USBRX)
    ///////////////////////////////////
{
    DataLines.frequency(100000);
        //set universal speed
    ChipSelect.write(1);
        //deselect the chip
    GenerateCRCTable(1, 137, CommandCRCTable);
        //generate the CRC7 lookup table; polynomial x^7 + x^3 + 1 converts to
        //decimal 137
    GenerateCRCTable(2, 69665, DataCRCTable);
        //generate the crc16 lookup table; polynomial x^16 + x^12 + x^5 + 1
        //converts to decimal 69665
    Initialize();
        //run setup operations
}

SDCard::~SDCard()
    //delete all tables and card data registers
{
    delete[] CommandCRCTable;
    delete[] DataCRCTable;
    delete[] OCR;
    delete[] CSD;
    delete[] FSR;
    delete this;
}

unsigned char SDCard::disk_initialize()
    //give the FAT module access to the card setup routine
{
    if (Status == 0x01)
    {
        return Initialize();
    }
    else
    {
        return Status;
    }
}
unsigned char SDCard::disk_status()
    //return card initialization and availability status
{ return Status; }
unsigned char SDCard::disk_read(
    unsigned char* buff, unsigned long sector, unsigned char count)
    //give the FAT module access to the multiple-sector reading function
{ return Read((unsigned int)sector, count, buff); }
unsigned char SDCard::disk_write(
    const unsigned char* buff, unsigned long sector, unsigned char count)
    //give the FAT module access to the multiple-sector writing function
{ return Write((unsigned int)sector, count, (unsigned char*)buff); }
unsigned char SDCard::disk_sync()
    //the disk is always synchronized, so return "disk ready"
{ return 0x00; }
unsigned long SDCard::disk_sector_count()
    //calculate and return the number of sectors on the card from the CSD
{
    switch (CSD[0] & 0xC0)
    {
        case 0x00:
            //calculate sector count as specified for version 1 cards
            return ((((CSD[6] & 0x03) << 10) | (CSD[7] << 2)
                | ((CSD[8] & 0xC0) >> 6)) + 1)
                * (1 << ((((CSD[9] & 0x03) << 1)
                | ((CSD[10] & 0x80) >> 7)) + 2));
        case 0x40:
            //calculate sector count as specified for version 2 cards
            return ((((CSD[7] & 0x3F) << 16)
                | (CSD[8] << 8) | CSD[9]) + 1) * 1024;
        default:
            return 0;
    }
}
unsigned short SDCard::disk_sector_size()
    //fix the sector size to 512 bytes for all cards versions
{ return 512; }
unsigned long SDCard::disk_block_size()
    //calculate and return the number of sectors in an erase block from the CSD
{
    if (Version)
        //the erase sector size is the allocation unit for version 2 cards
    {
        return 1;
    }
    else
        //calculate the erase sector size for version 1 cards
    {
        return (CSD[10] << 1) | (CSD[11] >> 7) + 1;
    }
}

unsigned char SDCard::Format(unsigned int AllocationUnit)
    //call the FAT module formatting function
{
    if (format(AllocationUnit))
    {
        return 0x01;
    }
    else
    {
        return 0x00;
    }
}

unsigned char SDCard::Log(unsigned char Control, unsigned char Data)
{
    static unsigned char Workspace;
        //work area for card commands and data transactions
    static unsigned short Index = 0;
        //store last written byte number of current memory block
    static unsigned char Mode = 0x00;
        //store previous operating mode to determine current behavior

    SelectCRCMode(0);
        //CRC's are not used in raw data mode

    switch (Control)
    {
        case 0x00:
            //control code 0x00 synchronizes the card
            if (Mode)
                //if the card is in read or write mode, synchronize the card
            {
                ChipSelect.write(0);
                for (; Index < 512; Index++)
                    //get through the left over space, filling with 0xFF
                {
                    DataLines.write(0xFF);
                }
                DataLines.write(0xFF);
                DataLines.write(0xFF);
                    //get through the CRC
                ChipSelect.write(1);
                if (Mode == 0x01)
                    //if the card is in write mode, finish the current sector
                    //and finalize the writing operation
                {
                    ChipSelect.write(0);
                    t = 0;
                    do
                    {
                        t++;
                    } while (((DataLines.write(0xFF) & 0x11) != 0x01)
                        && (t < Timeout));
                        //get through the data response token
                    while (!DataLines.write(0xFF));
                        //get through the busy signal
                    DataLines.write(0xFD);
                    DataLines.write(0xFF);
                        //send the stop transmission token
                    while (!DataLines.write(0xFF));
                        //get through the busy signal
                    ChipSelect.write(1);
                    DataLines.write(0xFF);
                }
                else
                    //if the card is in read mode, finish the current sector
                    //and finalize the reading operation
                {
                    Command(12, 0, &Workspace);
                        //send the stop transmission command
                    ChipSelect.write(0);
                    while (!DataLines.write(0xFF));
                        //get through the busy signal
                    ChipSelect.write(1);
                    DataLines.write(0xFF);
                }
                Index = 0;
                Mode = 0x00;
                    //reset the index to the start and switch the mode to
                    //synchronized mode
            }
            return 0xFF;

        case 0x01:
                //control code 1 writes a byte
            if (Mode != 0x01)
                //if the previous call was not a write operation, synchronize
                //the card, start a new write block, and set the function to
                //write mode
            {
                Log(0, 0);
                Command(25, 0, &Workspace);
                Mode = 0x01;
            }
            if (Index == 0)
                //if the index is at the start, send the start block token
                //before the byte
            {
                ChipSelect.write(0);
                DataLines.write(0xFC);
                DataLines.write(Data);
                ChipSelect.write(1);
                Index++;
            }
            else if (Index < 511)
                //if the index is between the boundaries, write the byte
            {
                ChipSelect.write(0);
                DataLines.write(Data);
                ChipSelect.write(1);
                Index++;
            }
            else
                //if the index is at the last address, get through the CRC,
                //data response token, and busy signal and reset the index
            {
                ChipSelect.write(0);
                DataLines.write(Data);
                DataLines.write(0xFF);
                DataLines.write(0xFF);
                t = 0;
                do
                {
                    t++;
                } while (((DataLines.write(0xFF) & 0x11) != 0x01)
                    && (t < Timeout));
                while (!DataLines.write(0xFF));
                ChipSelect.write(1);
                Index = 0;
            }
            return 0xFF;

        case 0x02:
            //control code 2 reads a byte
            if (Mode != 0x02)
                //if the previous call was not a read operation, synchronise
                //the card, start a new read block, and set the function to
                //read mode
            {
                Log(0, 0);
                Command(18, 0, &Workspace);
                Mode = 0x02;
            }
            if (Index == 0)
                //if the index is at the start, get the start block token
                //and read the first byte
            {
                ChipSelect.write(0);
                t = 0;
                do
                {
                    t++;
                } while ((DataLines.write(0xFF) != 0xFE)
                    && (t < Timeout));
                Workspace = DataLines.write(0xFF);
                ChipSelect.write(1);
                Index++;
                return Workspace;
            }
            else if (Index < 511)
                //if the index is between the boundaries, read the byte
            {
                ChipSelect.write(0);
                Workspace = DataLines.write(0xFF);
                ChipSelect.write(1);
                Index++;
                return Workspace;
            }
            else
                //if the index is at the last address, get through the CRC and
                //reset the index
            {
                ChipSelect.write(0);
                Workspace = DataLines.write(0xFF);
                DataLines.write(0xFF);
                DataLines.write(0xFF);
                ChipSelect.write(1);
                Index = 0;
                return Workspace;
            }

        default:
            //undefined control codes will only return stuff bits
            return 0xFF;
    }
}

unsigned char SDCard::Write(unsigned int Address, unsigned char* Data)
{
    unsigned char Workspace[2];
        //work area for card commands and data transactions
    
    if (Capacity)
        //send the single-block write command addressed for high-capacity cards
    {
        Command(24, Address, Workspace);
    }
    else
        //send the single-block write command addressed for low-capacity cards
    {
        Command(24, Address * 512, Workspace);
    }
    if (Workspace[0])
        //if a command error occurs, return "parameter error"
    { return 0x04; }
    DataCRC(512, Data, Workspace);
        //calculate the CRC16
    ChipSelect.write(0);
    DataLines.write(0xFE);
        //write start block token
    for (unsigned short i = 0; i < 512; i++)
        //write the data to the addressed card sector
    {
        DataLines.write(Data[i]);
    }
    DataLines.write(Workspace[0]);
    DataLines.write(Workspace[1]);
        //write the data CRC16
    t = 0;
    do
    {
        Workspace[0] = DataLines.write(0xFF);
        t++;
    } while (((Workspace[0] & 0x11) != 0x01) && (t < Timeout));
        //gather the data block response token
    while (!DataLines.write(0xFF));
        //get through the busy signal
    ChipSelect.write(1);
    DataLines.write(0xFF);
    if (((Workspace[0] & 0x1F) != 0x05) || (t == Timeout))
        //if the data response token indicates error, return write error
    { return 0x01; }
    else
    { return 0x00; }
}
unsigned char SDCard::Write(
    unsigned int Address, unsigned char SectorCount, unsigned char* Data)
{
    unsigned char Workspace[5];
        //work area for card commands and data transactions
    static unsigned char CurrentSectorCount = 1;
        //store the last write sector count

    if (SectorCount != CurrentSectorCount)
        //set the expected number of write blocks if its different from
        //previous multiple-block write operations
    {
        Command(55, 0, Workspace);
        Command(23, SectorCount, Workspace);
        if (Workspace[0])
        { return 0x04; }
        CurrentSectorCount = SectorCount;
    }
    if (Capacity)
        //send the multiple-block write command addressed for high-capacity
        //cards
    {
        Command(25, Address, Workspace);
    }
    else
        //send the multiple-block write command addressed for low-capacity
        //cards
    {
        Command(25, Address * 512, Workspace);
    }
    if (Workspace[0])
        //if a command error occurs, return "parameter error"
    { return 0x04; }
    Workspace[4] = 0x00;
        //initialize the error detection variable
    for (unsigned char i = 0; i < SectorCount; i++)
        //write each data sector
    {
        DataCRC(512, &Data[i * 512], Workspace);
            //calculate the CRC16
        ChipSelect.write(0);
        DataLines.write(0xFC);
            //send multiple write block start token
        for (unsigned int j = i * 512; j < (i + 1) * 512; j++)
            //write each data block
        {
            DataLines.write(Data[j]);
        }
        DataLines.write(Workspace[0]);
        DataLines.write(Workspace[1]);
            //write the CRC16
        t = 0;
        do
        {
            Workspace[0] = DataLines.write(0xFF);
            t++;
        } while (((Workspace[0] & 0x11) != 0x01) && (t < Timeout));
            //gather the data block response token
        while (!DataLines.write(0xFF));
            //get through the busy signal
        ChipSelect.write(1);
        Workspace[4] |= Workspace[0];
            //record if any write errors that are detected in the data response
            //tokens
        if (t == Timeout)
            //if a block write operation times out, stop operations
        { break; }
    }
    ChipSelect.write(0);
    DataLines.write(0xFD);
    DataLines.write(0xFF);
        //send the stop transmission token
    while (!DataLines.write(0xFF));
        //get through the busy signal
    ChipSelect.write(1);
    DataLines.write(0xFF);
    if (((Workspace[4] & 0x1F) != 0x05) || (t == Timeout))
        //if a data response token indicated an error, return "write error"
    { return 0x01; }
    else
    { return 0x00; }
}

unsigned char SDCard::Read(unsigned int Address, unsigned char* Data)
{
    unsigned char Workspace[4];
        //work area for card commands and data transactions
    
    if (Capacity)
        //send the single-block read command addressed for high-capacity cards
    {
        Command(17, Address, Workspace);
    }
    else
        //send the single-block read command addressed for low-capacity cards
    {
        Command(17, Address * 512, Workspace);
    }
    if (Workspace[0])
        //if a command error occurs, return "parameter error"
    { return 0x04; }
    ChipSelect.write(0);
    t = 0;
    do
    {
        t++;
    } while ((DataLines.write(0xFF) != 0xFE) && (t < Timeout));
        //get to the start block token
    if (t == Timeout) {
        ChipSelect.write(1); DataLines.write(0xFF); return 0x01; }
    for (unsigned short i = 0; i < 512; i++)
    {
        Data[i] = DataLines.write(0xFF);
    }
        //read the data from the addressed card sector
    Workspace[2] = DataLines.write(0xFF);
    Workspace[3] = DataLines.write(0xFF);
        //read the CRC16
    ChipSelect.write(1);
    DataLines.write(0xFF);
    DataCRC(512, Data, Workspace);
        //calculate the CRC16
    if (CRCMode && ((Workspace[0] != Workspace[2])
        || (Workspace[1] != Workspace[3])))
        //if the CRC is invalid, return "read error"
    { return 0x01; }
    else
    { return 0x00; }
}
unsigned char SDCard::Read(
    unsigned int Address, unsigned char SectorCount, unsigned char* Data)
{
    unsigned char Workspace[5];
        //work area for card commands and data transactions
    
    if (Capacity)
        //send the multiple-block read command addressed for high-capacity
        //cards
    {
        Command(18, Address, Workspace);
    }
    else
        //send the multiple-block read command addressed for low-capacity
        //cards
    {
        Command(18, Address * 512, Workspace);
    }
    if (Workspace[0])
        //if a command error occurs, return "parameter error"
    { return 0; }
    Workspace[4] = 0x00;
        //initialize error detection variable
    for (unsigned char i = 0; i < SectorCount; i++)
        //read each data sector
    {
        ChipSelect.write(0);
        t = 0;
        do
        {
            t++;
        } while ((DataLines.write(0xFF) != 0xFE) && (t < Timeout));
            //get to the data block start token
        if (t == Timeout)
        {
            break;
        }
            //if a block read operation times out, stop operations
        for (unsigned int j = i * 512; j < (i + 1) * 512; j++)
        {
            Data[j] = DataLines.write(0xFF);
        }
            //read the data block
        Workspace[2] = DataLines.write(0xFF);
        Workspace[3] = DataLines.write(0xFF);
            //read the data CRC from the card
        ChipSelect.write(1);
        DataCRC(512, &Data[i * 512], Workspace);
            //calculate the CRC16 for each read data block
        Workspace[4] |= (CRCMode && ((Workspace[0] != Workspace[2])
            || (Workspace[1] != Workspace[3])));
            //record if any invalid CRCs are detected during the
            //transaction
    }
    Command(12, 0, Workspace);
        //send the stop transmission command
    ChipSelect.write(0);
    while (!DataLines.write(0xFF));
        //get through the busy signal
    ChipSelect.write(1);
    DataLines.write(0xFF);
    if ((Workspace[4]) || (t == Timeout))
        //if an invalid CRC was detected, return "read error"
    { return 0x01; }
    else
    { return 0x00; }
}

unsigned char SDCard::SelectCRCMode(bool Mode)
{
    unsigned char Response;
    
    if (CRCMode != Mode)
        //only send command if CRCMode has been changed
    {
        t = 0;
        do
        {
            Command(59, Mode, &Response);
                //send the set CRC mode command
            t++;
        } while (Response && (t < Timeout));
        CRCMode = Mode;
    }
    if (t == Timeout)
        //if the command times out, return "error"
    { return 0x01; }
    else
    { return 0x00; }
}

void SDCard::SetTimeout(unsigned int Retries)
{
    Timeout = Retries;
        //Set the global number of times for operations to be retried
}

unsigned char SDCard::Initialize()
{
    unsigned char Workspace[5];
        //work area for card commands and data transactions
    
    for (unsigned char i = 0; i < 16; i++)
        //clock card at least 74 times to power up
    {
        DataLines.write(0xFF);
    }

    t = 0;
    do
    {
        Command(0, 0, Workspace);
            //send the reset command to put the card into SPI mode
        t++;
    } while ((Workspace[0] != 0x01) && (t < Timeout));
        //check for command acceptance
    if (t == Timeout) { Status = 0x01; return Status; }

    t = 0;
    do
    {
        Command(59, 1, Workspace);
            //turn on CRCs
        t++;
    } while ((Workspace[0] != 0x01) && (Workspace[0] != 0x05)
        && (t < Timeout));
        //the set CRC mode command is not valid for all cards in idle state
    if (t == Timeout) { Status = 0x01; return Status; }

    t = 0;
    do
    {
        Command(8, 426, Workspace);
            //the voltage bits are 0x01 for 2.7V - 3.6V, the check pattern is
            //0xAA, 0x000001AA converts to decimal 426
        t++;
    } while (((Workspace[0] != 0x01) || ((Workspace[3] & 0x0F) != 0x01) ||
        (Workspace[4] != 0xAA)) && (Workspace[0] != 0x05) && (t < Timeout));
        //check the version, voltage acceptance, and check pattern
    if (t == Timeout) { Status = 0x01; return Status; }
    Version = Workspace[0] != 0x05;
        //store the card version
        ////////////////////////////////////////////////////////////////////////////
    Computer.printf("byte1: %d, byte2: %d, byte3: %d, byte4: %d, byte5: %d", Workspace[0],Workspace[1],Workspace[2],Workspace[3],Workspace[4]);
///////////////////////////////////////////////////////////////////////////////

    if (!Version)
    {
        t = 0;
        do
        {
            Command(16, 512, Workspace);
                //set the data-block length to 512 bytes
            t++;
        } while (Workspace[0] && (t < Timeout));
        if (t == Timeout) { Status = 0x01; return Status; }
    }

    t = 0;
    do
    {
        Command(58, 0, Workspace);
            //check the OCR
        t++;
    } while (((Workspace[0] != 0x01) ||
        !((Workspace[2] & 0x20) || (Workspace[2] & 0x10))) && (t < Timeout));
        //check for the correct operating voltage, 3.3V
    if (t == Timeout) { Status = 0x01; return Status; }

    t = 0;
    do
    {
        Command(55, 0, Workspace);
            //initialize card command is application-specific
        Command(41, 1073741824, Workspace);
            //specify host supports high capacity cards, 0x40000000 converts to
            //decimal 1073741824d
        t++;
    } while (Workspace[0] && (t < Timeout));
        //check if the card is ready
    if (t == Timeout) { Status = 0x01; return Status; }

    if (SelectCRCMode(1))
        //turn on CRCs for all cards
    { Status = 0x01; return Status; }

    t = 0;
    do
    {
        Command(58, 0, Workspace);
            //check the OCR again
        t++;
    } while ((Workspace[0] || !(Workspace[1] & 0x80)) && (t < Timeout));
        //check the power up status
    if (t == Timeout) { Status = 0x01; return Status; }
    for (unsigned char i = 0; i < 4; i++)
        //record the OCR
    {
        OCR[i] = Workspace[i + 1];
    }
    Capacity = (OCR[0] & 0x40) == 0x40;
        //record the capacity

    t = 0;
    do
    {
        do
        {
            Command(9, 0, Workspace);
                //read the CSD
            t++;
        } while (Workspace[0] && (t < Timeout));
        if (t == Timeout) { Status = 0x01; return Status; }
        ChipSelect.write(0);
        do
        {
            t++;
        } while ((DataLines.write(0xFF) != 0xFE) && (t < Timeout));
            //get to the start data block token
        if (t == Timeout) { ChipSelect.write(1); DataLines.write(0xFF);
            Status = 0x01; return Status; }
        for (unsigned char i = 0; i < 16; i++)
            //record the CSD
        {
            CSD[i] = DataLines.write(0xFF);
        }
        Workspace[2] = DataLines.write(0xFF);
        Workspace[3] = DataLines.write(0xFF);
            //save the CSD CRC16
        ChipSelect.write(1);
        DataLines.write(0xFF);
        DataCRC(16, CSD, Workspace);
            //calculate the CSD CRC16
        Workspace[4] = 0;
        for (unsigned char i = 0; i < 15; i++)
        {
            Workspace[4] = CommandCRCTable[Workspace[4]] ^ CSD[i];
        }
        Workspace[4] = CommandCRCTable[Workspace[4]] | 0x01;
            //calculate the CSD CRC7
        t++;
    } while (((Workspace[0] != Workspace[2]) || (Workspace[1] != Workspace[3])
        || (Workspace[4] != CSD[15])) && (t < Timeout));
        //perform all CSD CRCs
    if (t == Timeout) { Status = 0x01; return Status; }

    if (((CSD[3] & 0x07) > 0x02) ||
        (((CSD[3] & 0x78) > 0x30) && ((CSD[3] & 0x07) > 0x01)))
        //read the CSD card speed bits and speed up card operations
    {
        DataLines.frequency(25000000);
            //maximum frequency is 25MHz
    }
    else
    {
        Workspace[0] = 1;
        for (unsigned char i = 0; i < (CSD[3] & 0x07); i++)
        {
            Workspace[0] *= 10;
                //the first three bits are a power of ten multiplier for speed
        }
        switch (CSD[3] & 0x78)
        {
            case 0x08: DataLines.frequency(Workspace[0] * 100000); break;
            case 0x10: DataLines.frequency(Workspace[0] * 120000); break;
            case 0x18: DataLines.frequency(Workspace[0] * 140000); break;
            case 0x20: DataLines.frequency(Workspace[0] * 150000); break;
            case 0x28: DataLines.frequency(Workspace[0] * 200000); break;
            case 0x30: DataLines.frequency(Workspace[0] * 250000); break;
            case 0x38: DataLines.frequency(Workspace[0] * 300000); break;
            case 0x40: DataLines.frequency(Workspace[0] * 350000); break;
            case 0x48: DataLines.frequency(Workspace[0] * 400000); break;
            case 0x50: DataLines.frequency(Workspace[0] * 450000); break;
            case 0x58: DataLines.frequency(Workspace[0] * 500000); break;
            case 0x60: DataLines.frequency(Workspace[0] * 550000); break;
            case 0x68: DataLines.frequency(Workspace[0] * 600000); break;
            case 0x70: DataLines.frequency(Workspace[0] * 700000); break;
            case 0x78: DataLines.frequency(Workspace[0] * 800000); break;
            default: break;
        }
    }

    if (CSD[4] & 0x40)
        //check for switch command class support
    {
        t = 0;
        do
        {
            Command(6, 2147483649, Workspace);
                //switch to high-speed mode (SDR25, 50MHz)
            t++;
        } while (Workspace[0] && (Workspace[0] != 0x04) && (t < Timeout));
            //some cards that support switch class commands respond with
            //"illegal command"
        if (t == Timeout) { Status = 0x01; return Status; }
        if (!Workspace[0])
        {
            do
            {
                ChipSelect.write(0);
                do
                {
                    t++;
                } while ((DataLines.write(0xFF) != 0xFE) && (t < Timeout));
                    //get to the start data block token
                if (t == Timeout) { ChipSelect.write(1);
                    DataLines.write(0xFF); Status = 0x01; return Status; }
                for (unsigned char i = 0; i < 64; i++)
                    //gather the function status register
                {
                    FSR[i] = DataLines.write(0xFF);
                }
                Workspace[2] = DataLines.write(0xFF);
                Workspace[3] = DataLines.write(0xFF);
                    //record the CRC16
                ChipSelect.write(1);
                DataLines.write(0xFF);
                DataCRC(64, FSR, Workspace);
                    //calculate the CRC16
                t++;
            } while (((Workspace[0] != Workspace[2])
                || (Workspace[1] != Workspace[3])) && (t < Timeout));
                //perform the CRC
            if (t == Timeout) { Status = 0x01; return Status; }
            if ((FSR[13] & 0x02) && ((FSR[16] & 0x0F) == 0x01))
            {
                DataLines.frequency(50000000);
                    //increase the speed if the function switch was successful
            }
        }
    }

    if (SelectCRCMode(0))
    { Status = 0x01; return Status; }
        //turn off CRCs

    Status = 0x00;
    return Status;
}

void SDCard::Command(
    unsigned char Index, unsigned int Argument, unsigned char* Response)
{
    CommandCRC(&Index, &Argument, Response);
        //calculate the CRC7
    ChipSelect.write(0);
        //assert chip select low to synchronize the command
    DataLines.write(0x40 | Index);
        //the index is assumed valid, commands start with bits 01
    DataLines.write(((char*)&Argument)[3]);
    DataLines.write(((char*)&Argument)[2]);
    DataLines.write(((char*)&Argument)[1]);
    DataLines.write(((char*)&Argument)[0]);
        //send the argument bytes in order from MSB to LSB
    DataLines.write(*Response);
        //send the CRC7
    t = 0;
    do
    {
        Response[0] = DataLines.write(0xFF);
            //clock the card high to let it run operations, the first byte
            //will be busy (all high), the response will be sent later
        t++;
    } while ((Response[0] & 0x80) && (t < Timeout));
        //check for a response by testing if the first bit is low
    if ((Index == 8) || (Index == 13) || (Index == 58))
        //if the command returns a larger response, get the rest of it
    {
        for (unsigned char i = 1; i < 5; i++)
        {
            Response[i] = DataLines.write(0xFF);
        }
    }
    ChipSelect.write(1);
        //assert chip select high to synchronize command
    DataLines.write(0xFF);
        //clock the deselected card high to complete processing for some cards
}

void SDCard::CommandCRC(
    unsigned char* IndexPtr, unsigned int* ArgumentPtr, unsigned char* Result)
{
    if (CRCMode)
        //only calculate if data-checks are desired
    {
        Result[0] =
            CommandCRCTable[
                CommandCRCTable[
                    CommandCRCTable[
                        CommandCRCTable[
                            CommandCRCTable[
                                *IndexPtr | 0x40
                            ] ^ ((char*)ArgumentPtr)[3]
                        ] ^ ((char*)ArgumentPtr)[2]
                    ] ^ ((char*)ArgumentPtr)[1]
                ] ^ ((char*)ArgumentPtr)[0]
            ] | 0x01;
            //calculate the CRC7, SD protocol requires the last bit to be high
    }
    else
        //if no data-checking is desired, the return bits are not used
    {
        Result[0] = 0xFF;
    }
}

void SDCard::DataCRC(
    unsigned short Length, unsigned char* Data, unsigned char* Result)
{
    if (CRCMode)
        //only calculate if data-checks are desired
    {
        unsigned char Reference;
            //store the current CRC16 lookup value
        Result[0] = 0x00;
        Result[1] = 0x00;
            //initialize the result carrier
        for (unsigned short i = 0; i < Length; i++)
            //step through each byte of the data to be checked
        {
            Reference = Result[0];
                //record the current CRC16 lookup for both bytes
            Result[0] = DataCRCTable[2 * Reference] ^ Result[1];
                //the first byte result is XORed with the old second byte
            Result[1] = DataCRCTable[(2 * Reference) + 1] ^ Data[i];
                //the second byte result is XORed with the new data byte
        }
        for (unsigned char i = 0; i < 2; i++)
            //the final result must be XORed with two 0x00 bytes
        {
            Reference = Result[0];
            Result[0] = DataCRCTable[2 * Reference] ^ Result[1];
            Result[1] = DataCRCTable[(2 * Reference) + 1];
        }
    }
    else
        //if no data-checking is desired, the return bits are not used
    {
        Result[0] = 0xFF;
        Result[1] = 0xFF;
    }
}

void SDCard::GenerateCRCTable(unsigned char Size,
    unsigned long long Generator, unsigned char* Table)
{
    unsigned char Index[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        //this will hold information from the generator; the position indicates
        //the order of the encountered 1, the value indicates its position in
        //the generator, the 9th entry indicates the number of 1's encountered
    
    for (unsigned char i = 0; i < 64; i++)
        //shift generator left until the first bit is high
    {
        if (((char*)&Generator)[7] & 0x80)
        { break; }
        Generator = Generator << 1;
    }
    for (unsigned char i = 0; i < Size; i++)
        //initialize table
    {
        Table[i] = 0x00;
    }
    for (unsigned char i = 0; i < 8; i++)
        //increment through each generator bit
    {
        if ((0x80 >> i) & ((unsigned char*)&Generator)[7])
            //if a 1 is encountered in the generator, record its order and
            //location and increment the counter
        {
            Index[Index[8]] = i;
            Index[8]++;
        }
        for (unsigned char j = 0; j < (0x01 << i); j++)
            //each bit doubles the number of XOR operations
        {
            for (unsigned char k = 0; k < Size; k++)
                //we need to precalculate each byte in the CRC table
            {
                Table[(Size * ((0x01 << i) + j)) + k]
                    = Table[(Size * j) + k];
                    //each power of two is equal to all previous entries with
                    //an added XOR with the generator on the leftmost bit and
                    //on each succeeding 1 in the generator
                for (unsigned char l = 0; l < Index[8]; l++)
                    //increment through the encountered generator 1s
                {
                    Table[(Size * ((0x01 << i) + j)) + k] ^=
                        (((unsigned char*)&Generator)[7-k]
                        << (i + 1 - Index[l]));
                    Table[(Size * ((0x01 << i) + j)) + k] ^=
                        (((unsigned char*)&Generator)[6-k]
                        >> (7 - i + Index[l]));
                        //XOR the new bit and the new generator 1s
                }
            }
        }
    }
}