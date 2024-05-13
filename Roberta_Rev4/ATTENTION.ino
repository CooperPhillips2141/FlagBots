void AttentionSubRoutine() {
Serial.println("Get Attention");
LeftMotor.moveTo(-180*stepsPerDeg);  
RightMotor.moveTo(180*stepsPerDeg);
MoveSubRoutine();

LeftMotor.moveTo(0);  
RightMotor.moveTo(0);
MoveSubRoutine();

LeftMotor.moveTo(-180*stepsPerDeg);  
RightMotor.moveTo(180*stepsPerDeg);
MoveSubRoutine();

LeftMotor.moveTo(0);  
RightMotor.moveTo(0);
MoveSubRoutine();

EndOfWordSubRoutine();

//Send code to tell Roberto to "Acknowledge"
for (int i = 0; i < 3; i++) {
 irsend.sendSony(0xa28, 12);
 delay(40);
}

irrecv.enableIRIn(); // Re-enable receiver


}
   
