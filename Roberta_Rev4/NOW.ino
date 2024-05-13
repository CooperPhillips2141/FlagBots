void NOW_SubRoutine() {

 //"N"
Serial.println("N") ;
LeftMotor.moveTo(-45*stepsPerDeg);  
RightMotor.moveTo(45*stepsPerDeg);
MoveSubRoutine();
delay(LetterDelay);

//"O"
Serial.println("O");
LeftMotor.moveTo(-225*stepsPerDeg);  
RightMotor.moveTo(90*stepsPerDeg);
MoveSubRoutine();
delay(LetterDelay);

//"W"
Serial.println("W");
LeftMotor.moveTo(-90*stepsPerDeg);  
RightMotor.moveTo(225*stepsPerDeg);
MoveSubRoutine();
delay(LetterDelay);

EndOfWordSubRoutine();

//Send code to tell Roberto to "FUTURE HELP"
for (int i = 0; i < 3; i++) {
 irsend.sendSony(0x088, 12);
 delay(40);
}

irrecv.enableIRIn(); // Re-enable receiver
irrecv.resume();

//HomeSubRoutine(); //TBD: how many times to repeat the conversation
//before homing both arms on each robot again..... 10 times, 20 times? 50 times?



}
