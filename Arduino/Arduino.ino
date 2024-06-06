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
const int MAX_PARTS = 2;
String parts[MAX_PARTS];

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
  //HomeSubRoutine();
}//--(end setup )---

void loop() {
  // check for incoming serial data
  if (Serial.available()) {  
    // read command from serial port
    command = Serial.readString();  
    Serial.println("Command Recieved: " + command);

    // Split the input string
    int index = 0;
    int partIndex = 0;
    while (index < command.length() && partIndex < MAX_PARTS) {
      int commaIndex = command.indexOf(',', index);
      if (commaIndex == -1) {
        parts[partIndex] = command.substring(index);
        break;
      } else {
        parts[partIndex] = command.substring(index, commaIndex);
        index = commaIndex + 1;
      }
      partIndex++;
    }
    int numParts = partIndex + 1;
    // Parse the parts to integers
    if (numParts == MAX_PARTS) { // Ensure there are enough parts
      int leftAngle = parts[0].toInt();
      int rightAngle = parts[1].toInt();

      // set the target motor positions
      // left motor uses only negative numbers for some reason
      LeftMotor.moveTo(leftAngle*STEPS_PER_DEG);
      RightMotor.moveTo(rightAngle*STEPS_PER_DEG);
      MoveSubRoutine();

      // Send acknowledgment
      Serial.println("Arms in place");
    }
   }
}