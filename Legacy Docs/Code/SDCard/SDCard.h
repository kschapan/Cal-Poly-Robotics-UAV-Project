//mbed Microcontroller Library
//SDCard Interface
//Copyright 2010
//Thomas Hamilton

#ifndef SDCardLibrary
#define SDCardLibrary

#include "stdint.h"
#include "mbed.h"
#include "FATFileSystem.h"

class SDCard : public FATFileSystem
{
    private:
        SPI DataLines;
        DigitalOut ChipSelect;
            //physical chip interface

        unsigned int t;
            //timeout counter
        unsigned int Timeout;
            //timeout limit
        bool CRCMode;
            //low: CRCs disabled, high: CRCs enabled
        bool Capacity;
            //low: low-capacity, high: high-capacity
        bool Version;
            //low: version 1, high: version 2
        unsigned char Status;
            //0x00: Ready, 0x01: not initialized
        unsigned char FSR[64];
            //function status register
        unsigned char CSD[16];
            //card-specific data register
        unsigned char OCR[4];
            //operating conditions register
        unsigned char DataCRCTable[512];
            //CRC16 CCITT lookup table
        unsigned char CommandCRCTable[256];
            //CRC7 SD command lookup table

        unsigned char Initialize();
            //complete all initialization operations
        void Command(unsigned char Index,
            unsigned int Argument, unsigned char* Response);
            //sends the SD command with the given Index and Argument to the SD
            //card and stores the SD Response
        void CommandCRC(unsigned char* IndexPtr,
            unsigned int* ArgumentPtr, unsigned char* Result);
            //calculates the SD proprietary CRC7 result of an SD command
            //(composed of a command index and argument) and stores the one-
            //byte solution in Result
        void DataCRC(
            unsigned short Length, unsigned char* Data, unsigned char* Result);
            //calculates the CRC16 CCITT result of the number of bytes in Data
            //given by Length and stores the two-byte solution in Result
            //assumes DataCRCTable has already been calculated
        void GenerateCRCTable(unsigned char Size,
            unsigned long long Generator, unsigned char* Table);
            //pre-calculates CRC results from the given Generator for efficient
            //checking; assumes pre-allocated Table is large enough to hold the
            //number of bytes given in Size

        virtual unsigned char disk_initialize();
        virtual unsigned char disk_status();
        virtual unsigned char disk_read(unsigned char* buff,
            unsigned long sector, unsigned char count);
        virtual unsigned char disk_write(const unsigned char* buff,
            unsigned long sector, unsigned char count);
        virtual unsigned char disk_sync();
        virtual unsigned long disk_sector_count();
        virtual unsigned short disk_sector_size();
        virtual unsigned long disk_block_size();
            //FAT system virtual functions that are called by the FAT module

    public:
        SDCard(PinName mosi, PinName miso, PinName sck, PinName cs,
            const char* DiskName);
            //constructor needs SPI pins, DigitalOut pins, and a directory name
        virtual ~SDCard();
            //destructor deallocates tables and registers
        unsigned char Format(unsigned int AllocationUnit);
            //formats the card FAT with given AllocationUnit in sectors; the
            //maximum is 32768 sectors
        unsigned char Log(unsigned char Control, unsigned char Data);
            //multipurpose single-byte raw data-logging method with three modes
            //Control   description
            //  0       synchronizes card and resets internal counter to
            //              finalize read or write operations
            //  1       successively write Data to a raw byte address in order
            //              starting at address 0
            //  2       successively read and return a raw data byte in order
            //              starting at address 0
            //return byte from sync or write operations and input Data of sync
            //or read operations are not used; writing with this function will
            //deformat the drive
        unsigned char Write(unsigned int Address, unsigned char* Data);
            //write the first 512 byte sector of Data to the card at the given
            //Address
        unsigned char Write(unsigned int Address,
            unsigned char SectorCount, unsigned char* Data);
            //write the first given SectorCount of 512 byte sectors of Data to
            //the card at the given Address
        unsigned char Read(unsigned int Address, unsigned char* Data);
            //read the first 512 byte sector from card at the given Address
            //into Data
        unsigned char Read(unsigned int Address,
            unsigned char SectorCount,  unsigned char* Data);
            //read the first given SectorCount of 512 byte sectors from the
            //card at the given Address into Data
        unsigned char SelectCRCMode(bool Mode);
            //toggle CRC mode; low: CRCs disabled, high: CRCs enabled, default:
            //CRCs disabled
        void SetTimeout(unsigned int Retries);
            //change the number of retries for interface functions; increase if
            //lines are unreliable; default: 1024, minimum: 1024
};

#endif