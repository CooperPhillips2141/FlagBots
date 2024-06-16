void homeLeft() {
  //set lower speed and accel during homing
  LeftMotor.setSpeed(7);
  LeftMotor.setAcceleration(50);
  LThomeVal = digitalRead(LeftHomeSwitch);

  if(LThomeVal == 0){ //sensor on wedge
    //set move based on which arm is to be homed
    //test Left arm
    Serial.println("Started on wedge...");
    LeftMotor.moveTo(180*STEPS_PER_DEG);
    //add in SW debouncing to ensure we find the edge of the ramp
    boolean debounced = false;
    while(!debounced){
      while(LThomeVal == 0){ //move while still on wedge
        LeftMotor.run();
        LThomeVal = digitalRead(LeftHomeSwitch);
      }
      delay(100);
      LThomeVal = digitalRead(LeftHomeSwitch);
      if(LThomeVal == 1){
        debounced = true;
        break;
      }
      LeftMotor.move(180*STEPS_PER_DEG);
    }
    
    Serial.println("Limit reached!!");
    LeftMotor.stop();
    //set position as temp 0
    LeftMotor.setCurrentPosition(0);
    Serial.println(LThomeVal);             // debug value
    delay(1000);

    //now move 40 degrees CCW to actual 0 then set that as actual 0
    //40 is the theoretical half of the 80 degree wedge, but use TBD degrees as practical
    //value to account for sensor detection capabilities
    LeftMotor.move(49*STEPS_PER_DEG);
    //stepper.moveTo(0);
    MoveSubRoutine();
    LeftMotor.setCurrentPosition(0); 

  } 
 
 LeftMotor.setSpeed(Speed);
 LeftMotor.setAcceleration(Acceleration);
 Serial.println("LEFT ARM at Home and new 0 set!");

}
