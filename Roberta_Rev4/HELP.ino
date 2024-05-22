
#include "global_variables.h"

void HELP_SubRoutine() {

 //"H"
Serial.println("H") ;
LeftMotor.moveTo(-45*-stepsPerDeg);  
RightMotor.moveTo(90*stepsPerDeg);
MoveSubRoutine();
delay(letterDelay);

//"E"
Serial.println("E");
LeftMotor.moveTo(-135*stepsPerDeg);  
RightMotor.moveTo(0);
MoveSubRoutine();
delay(letterDelay);

//"L"
Serial.println("L");
LeftMotor.moveTo(-135*stepsPerDeg);  
RightMotor.moveTo(45*stepsPerDeg);
MoveSubRoutine();
delay(letterDelay);

//"P"
Serial.println("P");
LeftMotor.moveTo(-180*stepsPerDeg);  
RightMotor.moveTo(90*stepsPerDeg);
MoveSubRoutine();
delay(letterDelay);

EndOfWordSubRoutine();

//Send code to tell Roberto to "WHY"
for (int i = 0; i < 3; i++) {
 irsend.sendSony(0xc48, 12);
 delay(40);
}

irrecv.enableIRIn(); // Re-enable receiver



}
