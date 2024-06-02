
//XLimit
//YLimit

void HomeSubRoutine() {
Serial.println("Homing Both Arms"); 

homeLeft();
homeRight();
//LeftMotor.moveTo(0);  
//RightMotor.moveTo(0);
//MoveSubRoutine();

// move off limit swtich
//while(digitalRead(XLimit)=HIGH||digitalRead(YLimit)=HIGH ){
//if (digitalRead(XLimit)=HIGH)LeftMotor.move(-10);
//if (digitalRead(YLimit)=HIGH)RightMotor.move(10);

//}

//while(digitalRead(XLimit)=LOW||digitalRead(YLimit)=LOW ){
//if (digitalRead(XLimit)=LOW)LeftMotor.move(1);
//if (digitalRead(YLimit)=LOW)RightMotor.move(-1);

//}
//LeftMotor.setCurrentPosition(0)
//RightMotor.setCurrentPosition(0)

}

