#include <AccelStepper.h>

const int STEPS_PER_DEG = 17;     
String InputChar;
int MaxSpeed=1000;
int Speed=100; //was 200
int var = 0;
int LetterDelay = 2500; //was1500; 5000 for letter and arm position verification
int Acceleration=500; //was 1000
int JogStepSize =1;
int XLimit=1;
int ZLimit=2;
long LeftPosition=0;
long RightPosition=0;
String command = "";
int leftAngle = 0;
int rightAngle = 0;
string[] parts;

//Pin definitions
#define LeftStep 6        // X-Axis step Left Arm
#define RightStep 4       // z-Axis step right Arm
#define LeftDir 5         // X-Axis direction Left Arm
#define RightDir 7         // y-Axis direction right Arm
#define Enable 8          // Enable motors
#define LeftHomeSwitch 9        // X-Axis limit Left Arm
#define RightHomeSwitch 10       // Y-Axis limit right Arm\

int LThomeVal = -1;
int RThomeVal = -1;

// Initialize pin using the AccelStepper with union cnc sheild
AccelStepper LeftMotor(1, LeftStep, LeftDir);
AccelStepper RightMotor(1, RightStep, RightDir);

void setup() {
  pinMode(Enable, OUTPUT); 
  digitalWrite(Enable, HIGH); 
  pinMode(LeftHomeSwitch, INPUT_PULLUP);
  pinMode(RightHomeSwitch, INPUT_PULLUP);
  
  Serial.begin(9600);// set up Serial library at 9600 bps
  Serial.println("Flag Bot - serial port connected");
  
   // Configure each steppe
    LeftMotor.setMaxSpeed(MaxSpeed);
    LeftMotor.setAcceleration(Acceleration);
    LeftMotor.setSpeed(Speed);
    RightMotor.setMaxSpeed(MaxSpeed);
    RightMotor.setAcceleration(Acceleration);
    RightMotor.setSpeed(Speed);

  //MAKE SURE TO HOME BOTH ARMS HERE BEFORE THE START OF THE MAIN LOOP
  HomeSubRoutine();
  

}//--(end setup )---

void loop() {
  // check for incoming serial data
   if (Serial.available()) {  
      // read command from serial port
      command = Serial.readString();  

      // transform it into a left angle and a right angle
      parts = input.Split(',');

      // Parse the parts into integers
      leftAngle = int.Parse(parts[0]);
      rightAngle = int.Parse(parts[1]);

      // set the target motor positions
      // left motor uses only negative numbers for some reason
      LeftMotor.moveTo(leftAngle*STEPS_PER_DEG);
      RightMotor.moveTo(rightAngle*STEPS_PER_DEG);

      // now move the stepper motors!
      while(abs(LeftMotor.distanceToGo())>0.0 || abs(RightMotor.distanceToGo())>0.0){
        LeftMotor.run();
        RightMotor.run();
      }
   }
}