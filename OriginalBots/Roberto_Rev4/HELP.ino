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



irrecv.enableIRIn(); // Re-enable receiver


//HomeSubRoutine();
}
