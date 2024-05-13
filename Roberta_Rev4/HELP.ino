void HELP_SubRoutine() {

 //"H"
Serial.println("H") ;
LeftMotor.moveTo(-45*-stepsPerDeg);  
RightMotor.moveTo(90*stepsPerDeg);
MoveSubRoutine();
delay(LetterDelay);

//"E"
Serial.println("E");
LeftMotor.moveTo(-135*stepsPerDeg);  
RightMotor.moveTo(0);
MoveSubRoutine();
delay(LetterDelay);

//"L"
Serial.println("L");
LeftMotor.moveTo(-135*stepsPerDeg);  
RightMotor.moveTo(45*stepsPerDeg);
MoveSubRoutine();
delay(LetterDelay);

//"P"
Serial.println("P");
LeftMotor.moveTo(-180*stepsPerDeg);  
RightMotor.moveTo(90*stepsPerDeg);
MoveSubRoutine();
delay(LetterDelay);

EndOfWordSubRoutine();

//Send code to tell Roberto to "WHY"
for (int i = 0; i < 3; i++) {
 irsend.sendSony(0xc48, 12);
 delay(40);
}

irrecv.enableIRIn(); // Re-enable receiver



}
