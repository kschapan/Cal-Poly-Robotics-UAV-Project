/*
 * Cal Poly Robotics Club QuadRotor Project
 * Header file for tasks
 *
 */
 
#ifndef ARDUTASK_h
#define ARDUTASK_h
 
#include <avr/io.h>

#define DEFAULT_INTERVAL 1000000

class ArduTask {
  protected:
    int maxTime;
    unsigned long interval;
  
  public:
    ArduTask();
    ~ArduTask();
    virtual void initialize();
    virtual void run();
    virtual int getMaxTime();
    virtual unsigned long setInterval(unsigned long);
    virtual unsigned long getInterval();
};
 
#endif
