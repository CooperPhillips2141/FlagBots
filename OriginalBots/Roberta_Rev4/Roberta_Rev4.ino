//Robots Speak - Flag Bot 12/3/18
//David Schmoyer
//Run 12V & GND wire from power in to pins VIN & GND
//!!!! 12V MAX POWER SUPPLY !!!!

//6/29/2019
//new motor/gearbox unit: 1.8 degree/step; 400 steps/rev because we are half stepping; 15.3:1 gear ratio
//so there are 6120 steps/1 output rev; thus there are 6120/360 = 17 steps/ output degree
//infrared IR communication: on the Uno, pin 2 is RX, pin 3 is TX
//each code will be sent 3 times to ensure it is received by the other bot
//also use 12-bit (in hex 0xZZZ) codes, such that the 'device' is a VCR, by using
//an obsolete device, there is less chance that a random IR code can affect a surrounding Sony device
//all codes will end with an '8' which is the command for VCR: 0xYZ8

#include <IRremote.h>
#include <IRremoteInt.h>

int RECV_PIN = 12;
IRrecv irrecv(RECV_PIN);
decode_results results;
IRsend irsend;

#include <AccelStepper.h>
#include <Wire.h>

const int stepsPerDeg = 17;     
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

unsigned long lastTriggeredTime;
unsigned long currentTime;
const unsigned long noDetectionPeriod = 1200000;//was 1200000; //20 minutes
boolean motionDetected = false;
boolean startingConv = true; //boolean flag for Roberta to automatically start the conversation with "Attention"
int convCntr = 0;
boolean breakConvLoop = false;

//Pin definitions
#define LeftStep 6        // X-Axis step Left Arm
#define RightStep 4       // z-Axis step right Arm
#define LeftDir 5         // X-Axis direction Left Arm
#define RightDir 7         // y-Axis direction right Arm
#define Enable 8          // Enable motors
#define LeftHomeSwitch 9        // X-Axis limit Left Arm
#define RightHomeSwitch 10       // Y-Axis limit right Arm
#define PIR1pin 11        //motion detector signal pin, Analog A0 --> 14
#define PIR2pin 13        //Analog A1 --> 15


int LThomeVal = -1;
int RThomeVal = -1;

//#define LeftLimitCW 10       // X-Axis Softlimit clockwise Left Arm
//#define LeftLimitCCW 10       // X-Axis Softlimit Counterclockwise Left Arm
//#define RightLimitCW 10       // z-Axis Softlimit clockwise R ightArm
//#define RightLimitCCW 10       // z-Axis Softlimit Counterclockwise Right Arm

// Initialize pin using the AccelStepper with union cnc sheild
AccelStepper LeftMotor(1, LeftStep, LeftDir);
AccelStepper RightMotor(1, RightStep, RightDir);


void setup() {
  pinMode(Enable, OUTPUT); 
  digitalWrite(Enable, HIGH); 
  pinMode(PIR1pin, INPUT);
  pinMode(PIR2pin, INPUT);
  pinMode(LeftHomeSwitch, INPUT_PULLUP);
  pinMode(RightHomeSwitch, INPUT_PULLUP);
  
  irrecv.enableIRIn(); // Start the receiver
  
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
  //this top loop in the motion detection waiting loop
  //break out of this loop if either 1) this bot detects motion
  while(!motionDetected){
    digitalWrite(Enable, LOW); //keep motor drive output off
    if(digitalRead(PIR1pin) || digitalRead(PIR2pin)){
      motionDetected = true;
      Serial.println("Initial motion detected...");
      startingConv = true;
      //send code to BertO to exit motion waiting loop
      //for (int i = 0; i < 3; i++) {
        //irsend.sendSony(0x888, 12);
        //delay(40);
      //}
      //irrecv.enableIRIn(); // Re-enable receiver
      lastTriggeredTime = millis();
      break;
    }
  }

//each move sequence is triggered either by handheld remote OR code sent from the other robot
//Roberta starts the conversation automatically

  while(1){
    if(breakConvLoop){
      breakConvLoop = false;
      break;
    }
    
    //if(convCntr == 2){ //get rid of this block before testing with the PIRs
      //break;
    //} 
    if(startingConv){
      //delay(2000);
      digitalWrite(Enable, HIGH);
      AttentionSubRoutine();
      startingConv = false;
    }
    //check for if motion is detected again... if so, update last triggered time
    if(digitalRead(PIR1pin) || digitalRead(PIR2pin)){
      Serial.println("motion detected while already running...");
      Serial.println("updating the lastTriggeredTime...");
      motionDetected = true;
      lastTriggeredTime = millis();
    }

    if (irrecv.decode(&results)) {
      Serial.println(results.value, HEX);
      if(results.value == 0xFD08F7 || results.value == 0x288){ //1 on remote; only the remote starts the conversation
        digitalWrite(Enable, HIGH);
        AttentionSubRoutine();
      }
      if(results.value == 0xFD48B7 || results.value == 0xb38){ //3 on remote
        HELP_SubRoutine();
      }
      if(results.value == 0xFDA857 || results.value == 0xd58){//5 on remote
        FUTURE_SubRoutine();
      }
      if(results.value == 0xFD18E7 || results.value == 0xf78){//7 on remote
        //"WHY NOW?"
        WHY_SubRoutine();
        NOW_SubRoutine();

        //waiting loop below for the signal to home, 508, from Roberto
        //after getting that signal, both robots' arms will power down, 2 second delay, then home
        //at the same time
        //the power off/delay is to allow an arm that may have been bumped to fall down to close to 0
        //and thus be able to be homed to actual 0 position again
        while(1){
         if (irrecv.decode(&results)) {
            if(results.value == 0x508){ //
              digitalWrite(Enable, LOW); 
              delay(2000);
              digitalWrite(Enable, HIGH);
              HomeSubRoutine();
              break;
            }
          }
        }
        convCntr += 1;
       }
      irrecv.resume(); // Receive the next value
    }

    LThomeVal = digitalRead(LeftHomeSwitch);
    RThomeVal = digitalRead(RightHomeSwitch);

    if(convCntr == 2){
      //reset to 0 for repeat conversation process
      convCntr = 0;
      digitalWrite(Enable, LOW);
      Serial.println("now taking the 3 min break (but use 30s for testing");
      delay(180000); //was 5 mins, 300000ms
      
      //this is a safe time/position to check if no motion detected for at least 20 minutes
      currentTime = millis();
      if(currentTime - lastTriggeredTime >= noDetectionPeriod){
        //send code to roberto to "stop conv"
        Serial.println("no detection timeout reached...");
        Serial.println("breaking the conversing loop...");
        //for (int i = 0; i < 3; i++) {
          //irsend.sendSony(0x908, 12);
          //delay(40);
        //}
        motionDetected = false;
        breakConvLoop = true;
      } else {
        Serial.println("timeout not reached, continuing show...");
        startingConv = true;
        //send code to roberto to continue...
        //for (int i = 0; i < 3; i++) {
          //irsend.sendSony(0x718, 12);
          //delay(40);
        //}
      }
    }
  }
  
  


//--(Serial Input**START**)---
/*if(Serial.available()){
InputChar = Serial.readStringUntil("\n");

//*** Convert InputChar to absolute jog position ****
if (InputChar =="1") JogStepSize =1375;          // "1" - Jog 1 step
if (InputChar =="2") JogStepSize =10 ;        // "2" - Jog 1 step
if (InputChar =="3") JogStepSize =100 ;       // "3" - Jog 10 step
if (InputChar =="4") JogStepSize =200 ;       // "4" - Jog 200 step
if (InputChar =="5") JogStepSize =500 ;       // "5" - Jog 500 step
if (InputChar =="6") JogStepSize =1000 ;      // "6" - Jog 1000 step
if (InputChar =="7") JogStepSize =10000 ;     // "7" - Jog 2000 step

if (InputChar =="8") {
  Acceleration=Acceleration+50;
   LeftMotor.setAcceleration(Acceleration);
   RightMotor.setAcceleration(Acceleration);
    Serial.print("Acceleration:");
    Serial.println(Acceleration); 
  }
if (InputChar =="9") {
  Acceleration=Acceleration-50; 
   LeftMotor.setAcceleration(Acceleration);
   RightMotor.setAcceleration(Acceleration); 
    Serial.print("Acceleration:");
    Serial.println(Acceleration); 
  }

if (InputChar =="a"||InputChar =="A"){
  LeftPosition=LeftPosition+JogStepSize; 
  }
if (InputChar =="s"||InputChar =="S"){
  LeftPosition=LeftPosition-JogStepSize; 
  } 
if (InputChar =="d"||InputChar =="D"){
  RightPosition=RightPosition+JogStepSize; 
  } 
if (InputChar =="f"||InputChar =="F"){
  RightPosition=RightPosition-JogStepSize; 
  } 
  if (InputChar =="g"||InputChar =="G"){
  LeftMotor.moveTo(LeftPosition);
  RightMotor.moveTo(RightPosition); 
  }
MoveSubRoutine();
}*/
//--(Serial Input**END**)---
}


/*
 
IR Remote Button COdes
FD00FF == on/off
FD807F = vol+
FD40BF = func/stop
FD20DF = rewind
FDA05F = play/pause
FD609F = ffwd
FD10EF = down arrow
FD906F = vol -
FD50AF = arrow up
FD30CF = 0
FDB04F = eq
FD708F = st/rept
FD08F7 = 1
FD8877 = 2
FD48B7 = 3
FD28D7 = 4
FDA857 = 5
FD6897 = 6
FD18E7 = 7
FD9867 = 8
FD58A7 = 9
 */





