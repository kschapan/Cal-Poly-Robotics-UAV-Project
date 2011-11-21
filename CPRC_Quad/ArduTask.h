/*
 * Cal Poly Robotics Club QuadRotor Project
 * Header file for tasks
 *
 * @Modified Date: 11/20/11
 * @Modified By: Kevin Schapansky
 *
 * Change Log:
 *
 */
 
#ifndef ARDUTASK_h
#define ARDUTASK_h
 
#include <avr/io.h>
 
class ArduTask {
  protected:
    int maxTime;
    int interval;
  
  public:
    ArduTask();
    ~ArduTask();
    virtual void initialize();
    virtual void run();
    virtual int getMaxTime();
    virtual int getInterval();
};
 
#endif
