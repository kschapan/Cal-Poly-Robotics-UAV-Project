/*
 * Cal Poly Robotics Club QuadRotor Project
 * Task Scheduler Header
 *
 */
 
#ifndef TASKSCHED_h
#define TASKSCHED_h
 
#include <stdlib.h>
#include "ArduTask.h"
#include "TimerOne.h"
 
#define RESOLUTION 1000

typedef struct {
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
    void registerAndInitTask(ArduTask *newTask);
    void taskMon();
  private:
    Task *taskList;
    int tasksAdded;
    int currentTask;
};
 
#endif
