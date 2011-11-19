/*
 *  
 */ 
 
#ifndef TASKSCHED_h
#define TASKSCHED_h
 
#include <avr/io.h>
#include "ArduTask.h"
 
class TaskScheduler {
  public:
    void initialize();
    void registerTask(ArduTask newTask);
};
 
#endif
