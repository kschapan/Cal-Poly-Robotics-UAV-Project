/*
 * Cal Poly Robotics Club QuadRotor Project
 * Task Scheduler, responsible for running tasks
 *
 */

#include "TaskScheduler.h"

TaskScheduler::TaskScheduler() {
  tasksAdded = 0;
  currentTask = 0;
}

TaskScheduler::~TaskScheduler() {
  free(taskList);
}

void TaskScheduler::execute() {
  while (taskList[currentTask].executionRequested == 0)
    currentTask = (currentTask + 1) % (tasksAdded);
  taskList[currentTask].task->run();
  taskList[currentTask].executionRequested = 0;
  taskList[currentTask].timeSinceExecution = 0;
  currentTask = (currentTask + 1) % (tasksAdded);
}

void TaskScheduler::initialize(int taskCount) {
  taskList = (Task *) malloc(taskCount * sizeof(Task));
}

void TaskScheduler::taskMon() {
  for (int i = 0; i < tasksAdded; i++) {
    if ((taskList[i].timeSinceExecution += RESOLUTION) >= taskList[i].task->getInterval()) {
      taskList[i].executionRequested = 1;
      taskList[i].timeSinceExecution = taskList[i].task->getInterval();
    }
  }
}

void TaskScheduler::registerAndInitTask(ArduTask *newTask) {
  Task toAdd;
  
  toAdd.task = newTask;
  newTask->initialize();
  toAdd.executionRequested = 1;
  toAdd.timeSinceExecution = 0;
  taskList[tasksAdded++] = toAdd;
}

