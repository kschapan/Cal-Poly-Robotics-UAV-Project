/*
 * Cal Poly Robotics Club QuadRotor Project
 * Task Scheduler Header
 *
 * @Modified Date: 11/20/11
 * @Modified By: Kevin Schapansky
 *
 * Change Log:
 *
 */
 
#ifndef TASKSCHED_h
#define TASKSCHED_h
 
#include <avr/io.h>
#include <stdlib.h>
#include "ArduTask.h"
#include "TimerOne.h"
 
#define RESOLUTION 1000

typedef struct {
  long interval;
  long timeSinceExecution;
  int executionRequested;
  ArduTask *task;
} Task;

class TaskScheduler {
  public:
    TaskScheduler();
    ~TaskScheduler();
    void execute();
    void initialize(int taskCount);
    void registerTask(ArduTask *newTask);
    void taskMon();
  private:
    Task **taskList;
    int tasksAdded;
    int currentTask;
};
 
#endif
