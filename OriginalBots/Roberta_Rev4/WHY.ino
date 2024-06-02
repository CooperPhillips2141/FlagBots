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
}
