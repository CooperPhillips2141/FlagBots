#include "global_variables.h"

void FUTURE_SubRoutine() {

//"F"
Serial.println("F");
LeftMotor.moveTo(-90*stepsPerDeg);  
RightMotor.moveTo(0);
MoveSubRoutine();
delay(letterDelay);

//"U"
Serial.println("U");
LeftMotor.moveTo(-135*stepsPerDeg);  
RightMotor.moveTo(135*stepsPerDeg);
MoveSubRoutine();
delay(letterDelay);

//"T"
Serial.println("T");
LeftMotor.moveTo(-180*stepsPerDeg);  
RightMotor.moveTo(135*stepsPerDeg);
MoveSubRoutine();
delay(letterDelay);

//"U"
Serial.println("U");
LeftMotor.moveTo(-135*stepsPerDeg);  
RightMotor.moveTo(135*stepsPerDeg);
MoveSubRoutine();
delay(letterDelay);

//"R"
Serial.println("R");
LeftMotor.moveTo(-90*stepsPerDeg);  
RightMotor.moveTo(90*stepsPerDeg);
MoveSubRoutine();
delay(letterDelay);

//"E"
Serial.println("E");
LeftMotor.moveTo(-135*stepsPerDeg);  
RightMotor.moveTo(0);
MoveSubRoutine();
delay(letterDelay);


EndOfWordSubRoutine(); 
}
