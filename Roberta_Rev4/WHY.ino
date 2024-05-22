#include "global_variables.h"

void WHY_SubRoutine() {

//"W"
Serial.println("W");
LeftMotor.moveTo(-90*stepsPerDeg);  
RightMotor.moveTo(225*stepsPerDeg);
MoveSubRoutine();
delay(letterDelay);

//"H"
Serial.println("H");
LeftMotor.moveTo(-45*-stepsPerDeg);  
RightMotor.moveTo(90*stepsPerDeg);
MoveSubRoutine();
delay(letterDelay);

//"Y"
Serial.println("Y");
LeftMotor.moveTo(-90*stepsPerDeg);  
RightMotor.moveTo(135*stepsPerDeg);
MoveSubRoutine();
delay(letterDelay);


EndOfWordSubRoutine(); 
}
