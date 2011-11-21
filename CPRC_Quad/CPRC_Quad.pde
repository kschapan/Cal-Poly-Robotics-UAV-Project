/*
 * Cal Poly Robotics Club QuadRotor Project
 * Main class, contains master control loops and task initialization
 *
 * @Modified Date: 11/20/11
 * @Modified By: Kevin Schapansky
 *
 * Change Log:
 *
 */
 
#include "TaskScheduler.h"
#include "TestTask.h"
#include "TimerOne.h"

#define TASK_COUNT 1

TaskScheduler sched;
TestTask newTask;

void setup() {
  Serial.begin(115200);
  sched.initialize(TASK_COUNT);
  Timer1.initialize(RESOLUTION);
  Timer1.attachInterrupt(timerISR);
  sched.registerTask(&newTask);
}

void loop() {
  delay(1000);              // wait for a second
  Serial.write("Hi");
  Serial.println(newTask.getMaxTime());
}

void timerISR() {
  sched.taskMon();
}
