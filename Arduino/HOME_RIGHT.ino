void homeRight() {
  //set lower speed and accel for homing
  RightMotor.setSpeed(7);
  RightMotor.setAcceleration(50);
  RThomeVal = digitalRead(RightHomeSwitch);

  if(RThomeVal == 0){ //sensor on wedge
    //set move based on which arm is to be homed
    Serial.println("Started on wedge...");
    RightMotor.move(180*STEPS_PER_DEG);
    //add in SW debouncing to ensure we find the edge of the ramp
    boolean debounced = false;
    while(!debounced){
      while(RThomeVal == 0){ //move while still on wedge
        RightMotor.run();
        RThomeVal = digitalRead(RightHomeSwitch);
      }
      delay(100);
      RThomeVal = digitalRead(RightHomeSwitch);
      if(RThomeVal == 1){
        debounced = true;
        break;
      }
      RightMotor.move(180*STEPS_PER_DEG);
    }

    Serial.println("Limit reached!!");
    RightMotor.stop();
    //set position as temp 0
    RightMotor.setCurrentPosition(0);
    Serial.println(RThomeVal);             // debug value
    delay(1000);

    //now move 40 degrees CCW to actual 0 then set that as actual 0
    //40 is the theoretical half of the 80 degree wedge, but use 35 degrees as practical
    //value to account for sensor detection capabilities
    RightMotor.move(-58*STEPS_PER_DEG);
    //stepper.moveTo(0);
    MoveSubRoutine();
    RightMotor.setCurrentPosition(0); 

  } 

 RightMotor.setSpeed(Speed);
 RightMotor.setAcceleration(Acceleration);
 Serial.println("RIGHT ARM at Home and new 0 set!");

}
