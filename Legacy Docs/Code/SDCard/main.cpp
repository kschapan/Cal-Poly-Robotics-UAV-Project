#include "mbed.h"
#include "stdint.h"
#include "DirHandle.h"
#include "SDCard.h"

SDCard Logger(p5, p6, p7, p8, "SDCard");

DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);
//Serial Computer(USBTX, USBRX);

int main()
{
//    Computer.baud(9600);
    
    Logger.SelectCRCMode(1);

    //for(int i = 0;i<1000;i++)
    //{Logger.Log(1, i);}
    //Logger.Format(32768);
    
    //mkdir("/SDCard/testdir", 1023);
    FILE *fp = fopen("/SDCard/message.txt", "w");
    fprintf(fp, "Hello, World!");
    fclose(fp);

    /*DIR *d = opendir("/SDCard/testdir");
    struct dirent *p;
    while ((p = readdir(d)) != NULL)
    {
          Computer.printf("%s\n", p->d_name);
    }
    closedir(d);
    remove("/SDCard/testdir/TEST.txt");*/
    
    /*int test = rename("/SDCard/message.txt", "/SDCard/message2.txt");
    fp = fopen("/SDCard/message.txt", "a");
    fprintf(fp, "  Result = %d", test);
    fclose(fp);*/
    
/////////////////////
    if (1)
    {
        while (1)
        {
            led1 = !led1;
            wait_ms(250);
            led2 = !led2;
            wait_ms(250);
            led3 = !led3;
            wait_ms(250);
            led4 = !led4;
            wait_ms(250);
        }
    }//victory dance
    else
    {
        while (1)
        {

            led1 = !led1;
            led2 = !led2;
            led3 = !led3;
            led4 = !led4;
            wait_ms(250);
        }
    }//failure
    //for testing
/////////////////////
}