/*
 *  
 */ 
 
#ifndef ARDUTASK_h
#define ARDUTASK_h
 
#include <avr/io.h>
 
class ArduTask {
  private:
    int maxTime;
    int interval;
  
  public:
    virtual void initialize();
    virtual void run();
    virtual int getMaxTime();
};
 
#endif
