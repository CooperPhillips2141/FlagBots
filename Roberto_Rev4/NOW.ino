
#include "global_variables.h"

void NOW_SubRoutine() {

 //"N"
Serial.println("N") ;
LeftMotor.moveTo(-45*stepsPerDeg);  
RightMotor.moveTo(45*stepsPerDeg);
MoveSubRoutine();
delay(letterDelay);

//"O"
Serial.println("O");
LeftMotor.moveTo(-225*stepsPerDeg);  
RightMotor.moveTo(90*stepsPerDeg);
MoveSubRoutine();
delay(letterDelay);

//"W"
Serial.println("W");
LeftMotor.moveTo(-90*stepsPerDeg);  
RightMotor.moveTo(225*stepsPerDeg);
MoveSubRoutine();
delay(letterDelay);

EndOfWordSubRoutine();

//Send code to tell Roberta to "WHY NOW?"
for (int i = 0; i < 3; i++) {
 irsend.sendSony(0xf78, 12);
 delay(40);
}

irrecv.enableIRIn(); // Re-enable receiver



}
