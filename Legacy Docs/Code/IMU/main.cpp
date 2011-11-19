#include "mbed.h"
#include "MS3DMGX2.h"

MS3DMGX2 IMU(p9, p10);
DigitalOut led(LED1);
DigitalOut led4(LED4);
Serial PC(USBTX,USBRX);
int main()
{
led=0;
led4=0;
PC.baud(9600);
    float data[9];
    bool isvalid;
    wait(1);
    isvalid = IMU.Mode(0x05);
    PC.printf("Data Valid: %d\r\n\r\n", isvalid);
    //IMU.RequestSyncRead();
    wait(1);
    
    while (1)
    {
        while(!IMU.Readable());
        isvalid = IMU.Read(data);
        
        PC.printf("IMU Accel x Reads %f\r\n",data[0]);//NOTE: the compiler will not even calculate unused variables
        PC.printf("IMU accel y Reads %f\r\n",data[1]);//If the data retrieved above was not used, it wouldn't even be retrieved and the validity check would not pass
        PC.printf("IMU accel z Reads %f\r\n",data[2]);//this is called "compiler optimization"
        
        PC.printf("IMU ang rate x Reads %f\r\n",data[3]);
        PC.printf("IMU ang rate y Reads %f\r\n",data[4]);
        PC.printf("IMU ang rate z Reads %f\r\n",data[5]); 
        
        PC.printf("IMU mag x Reads %f\r\n",data[6]);
        PC.printf("IMU mag y Reads %f\r\n",data[7]);
        PC.printf("IMU mag z Reads %f\r\n",data[8]); 
        PC.printf("Validity: %d\r\n\r\n", isvalid);
        
        wait_ms(1000);
    }
}