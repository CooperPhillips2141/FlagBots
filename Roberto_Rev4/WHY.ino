
void WHY_SubRoutine() {

//"W"
Serial.println("W");
LeftMotor.moveTo(-90*stepsPerDeg);  
RightMotor.moveTo(225*stepsPerDeg);
MoveSubRoutine();
delay(LetterDelay);

//"H"
Serial.println("H");
LeftMotor.moveTo(-45*-stepsPerDeg);  
RightMotor.moveTo(90*stepsPerDeg);
MoveSubRoutine();
delay(LetterDelay);

//"Y"
Serial.println("Y");
LeftMotor.moveTo(-90*stepsPerDeg);  
RightMotor.moveTo(135*stepsPerDeg);
MoveSubRoutine();
delay(LetterDelay);

EndOfWordSubRoutine();

//Send code to tell Roberta to "FUTURE"
for (int i = 0; i < 3; i++) {
 irsend.sendSony(0xd58, 12);
 delay(40);
}

irrecv.enableIRIn(); // Re-enable receiver

 
}
