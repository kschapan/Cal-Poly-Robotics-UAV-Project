
/*
 * Cal Poly Robotics Club QuadRotor Project
 * Main class, contains master control loops and task initialization
 *
 */
 
#include "TaskScheduler.h"
#include "TestTask.h"
#include <TimerOne.h>

#define TASK_COUNT 2

TaskScheduler sched;
TestTask newTask;
TestTask newTask2;

int test = 0;

void setup() {
  Serial.begin(115200);
  sched.initialize(TASK_COUNT);
  Timer1.initialize();
  Timer1.attachInterrupt(timerISR, RESOLUTION);
  newTask.setInterval(5000000);
  newTask2.setInterval(1000000);
  sched.registerAndInitTask(&newTask);
  sched.registerAndInitTask(&newTask2);
  Serial.write("init'd\n");
}

void loop() {
    sched.execute();
    Serial.println(newTask.getMaxTime());
    Serial.println(newTask2.getMaxTime());
}

void timerISR() {
  sched.taskMon();
}
