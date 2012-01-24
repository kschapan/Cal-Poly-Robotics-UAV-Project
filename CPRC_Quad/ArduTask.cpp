/*
 * Cal Poly Robotics Club QuadRotor Project
 * Parent class of all tasks to be executed
 * ALL TASKS MUST EXTEND THIS CLASS
 *
 */
 
#include "ArduTask.h"

ArduTask::ArduTask() {interval = DEFAULT_INTERVAL;}

ArduTask::~ArduTask() {}

void ArduTask::initialize() {
}

void ArduTask::run() {

}

int ArduTask::getMaxTime() {
  return maxTime;
}

unsigned long ArduTask::setInterval(unsigned long newInterval) {
  interval = newInterval;
}

unsigned long ArduTask::getInterval() {
  return interval;
}

