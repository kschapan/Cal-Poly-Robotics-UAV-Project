/*
 * Cal Poly Robotics Club QuadRotor Project
 * Task Scheduler, responsible for running tasks
 *
 * @Modified Date: 11/20/11
 * @Modified By: Kevin Schapansky
 *
 * Change Log:
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
  while (taskList[currentTask]->executionRequested == 0)
    currentTask = (currentTask + 1) % (tasksAdded - 1);
  taskList[currentTask]->task->run();
  taskList[currentTask]->executionRequested = 0;
  taskList[currentTask]->timeSinceExecution = 0;
  currentTask = (currentTask + 1) % (tasksAdded - 1);
}

void TaskScheduler::initialize(int taskCount) {
  taskList = (Task **) malloc(taskCount * sizeof(Task **));
}

void TaskScheduler::taskMon() {
  for (int i = 0; i < tasksAdded; i++) {
    if ((taskList[i]->timeSinceExecution += RESOLUTION) >= taskList[i]->interval) {
      taskList[i]->executionRequested = 1;
    }
  }
}

void TaskScheduler::registerTask(ArduTask *newTask) {
  Task toAdd;
  
  toAdd.task = newTask;
  toAdd.interval = newTask->getInterval();
  toAdd.executionRequested = 1;
  toAdd.timeSinceExecution = 0;
  taskList[tasksAdded++] = &toAdd;
  newTask->initialize();
}

