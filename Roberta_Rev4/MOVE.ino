#include "global_variables.h"

void MoveSubRoutine() {
Serial.print("Left arm:");
Serial.print(LeftMotor.distanceToGo());
Serial.print("   Right arm:");
Serial.println(RightMotor.distanceToGo());  
while(abs(LeftMotor.distanceToGo())>0.0 || abs(RightMotor.distanceToGo())>0.0){
  LeftMotor.run();
  RightMotor.run();
}

RThomeVal = digitalRead(RightHomeSwitch);
LThomeVal = digitalRead(LeftHomeSwitch);
}
