#include "global_variables.h"

void AcknowledgeSubRoutine() {
Serial.println("Acknowledge");
LeftMotor.moveTo(-180*stepsPerDeg);  
RightMotor.moveTo(180*stepsPerDeg);
MoveSubRoutine();

LeftMotor.moveTo(0);  
RightMotor.moveTo(0);
MoveSubRoutine();

EndOfWordSubRoutine();

//Send code to tell Roberta to "HELP"
for (int i = 0; i < 3; i++) {
 irsend.sendSony(0xb38, 12);
 delay(40);
}

irrecv.enableIRIn(); // Re-enable receiver


}
   
