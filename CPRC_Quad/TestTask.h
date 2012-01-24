/*
 * Cal Poly Robotics Club QuadRotor Project
 * Extremely lame task header
 *
 */
 
#ifndef TESTTASK_h
#define TESTTASK_h
 
#include <avr/io.h>
#include "ArduTask.h"
 
class TestTask: public ArduTask {
  public:
    void initialize();
    void run();
};
 
#endif
