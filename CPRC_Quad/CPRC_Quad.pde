#include "TimerOne.h"
#include "TaskScheduler.h"

TaskScheduler sched;
void setup() {
  Serial.begin(115200);
  Timer1.initialize(100);
  sched.initialize();
}

void loop() {
  delay(1000);              // wait for a second
  Serial.write("Hi");
}
