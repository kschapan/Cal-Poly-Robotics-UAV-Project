/*
 * Cal Poly Robotics Club QuadRotor Project
 * Extremely lame task header
 *
 * @Modified Date: 11/20/11
 * @Modified By: Kevin Schapansky
 *
 * Change Log:
 *
 */
 
#ifndef TESTTASK_h
#define TESTTASK_h
 
#include <avr/io.h>
#include "ArduTask.h"
 
class TestTask: public ArduTask {
  public:
    void initialize();
};
 
#endif
