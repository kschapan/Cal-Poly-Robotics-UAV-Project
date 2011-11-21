/*
 * Cal Poly Robotics Club QuadRotor Project
 * Parent class of all tasks to be executed
 * ALL TASKS MUST EXTEND THIS CLASS
 *
 * @Modified Date: 11/20/11
 * @Modified By: Kevin Schapansky
 *
 * Change Log:
 *
 */
 
#include "ArduTask.h"

ArduTask::ArduTask() {}

ArduTask::~ArduTask() {}

void ArduTask::initialize() {
}

void ArduTask::run() {

}

int ArduTask::getMaxTime() {
  return maxTime;
}

int ArduTask::getInterval() {
  return interval;
}

