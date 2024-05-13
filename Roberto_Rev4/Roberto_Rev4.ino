 //Robots Speak - Flag Bot 12/3/18
//David Schmoyer
//Run 12V & GND wire from power in to pins VIN & GND
//!!!! 12V MAX POWER SUPPLY !!!!

//6/29/2019
//new motor/gearbox unit: 1.8 degree/step; 400 steps/rev because we are half stepping; 15.3:1 gear ratio
//infrared IR communication: on the Uno, pin 2 is RX, pin 3 is TX
//each code will be sent 3 times to ensure it is received by the other bot
//12/9/2019: rev4 SW includes PIR motion detection and also a warning message/sound played just before the arms will move
//the PIR detection can occur in either or both Bots: whichever bot detects motion first, will send a code to the other bot
//telling it to exit the waiting (motor drive outputs disabled...) loop and begin the conversation loop
//a timer variable will need to be used to check if 20 minutes has elapsed since the last detected motion time and if so(checking this only when
//it is SAFE to do so, i.e. after homing after a conversation when the arms are straight down for sure....), it goes back into the motion waiting loop

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
int LetterDelay = 2500; //1500
int Acceleration=500; //was 1000
int JogStepSize =1;
int XLimit=1;
int YLimit=2;
long LeftPosition=0;
long RightPosition=0;

int convCntr = 0;
boolean motionDetected = false;
//boolean breakConvLoop = false;

//Pin definitions
#define LeftStep 6        // X-Axis step Left Arm
#define RightStep 4       // Y-Axis step right Arm
#define LeftDir 5         // X-Axis direction Left Arm
#define RightDir 7         // y-Axis direction right Arm
#define Enable 8          // Enable motors, LOW = disable, HIGH = enabled
#define LeftHomeSwitch 9        // X-Axis limit Left Arm
#define RightHomeSwitch 10       // Y-Axis limit right Arm

int LThomeVal = -1;
int RThomeVal = -1;

//#define LeftLimitCW 10       // X-Axis Softlimit clockwise Left Arm
//#define LeftLimitCCW 10       // X-Axis Softlimit Counterclockwise Left Arm
//#define RightLimitCW 10       // Y-Axis Softlimit clockwise R ightArm
//#define RightLimitCCW 10       // Y-Axis Softlimit Counterclockwise Right Arm

// Initialize pin using the AccelStepper with union cnc sheild
AccelStepper LeftMotor(1, LeftStep, LeftDir);
AccelStepper RightMotor(1, RightStep, RightDir);


void setup() {
  pinMode(Enable, OUTPUT);
  digitalWrite(Enable, HIGH);
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
    digitalWrite(Enable, LOW);
}//--(end setup )---

void loop() {
  //Berto just waits here for signal from Berta that she detected motion...
  /*while(!motionDetected){
    digitalWrite(Enable, LOW);
    //delay(500);
    if (irrecv.decode(&results)) {
      if(results.value == 0x888){ //put in a different code, for motion detection
        motionDetected = true;
        break;
      }
    }
  }*/

  while(1){
    //if(breakConvLoop){
      //breakConvLoop = false;
      //break;
    //}
    
    //if(convCntr == 2){ //get rid of this block before testing with the PIRs
     // break;
    //}
    
    //each move sequence is triggered either by handheld remote OR code sent from the other robot
    if (irrecv.decode(&results)) {
      if(results.value == 0xFD8877 || results.value == 0xa28){ //2 on remote for Acknowledge
       digitalWrite(Enable, HIGH);
       AcknowledgeSubRoutine();
      }
      if(results.value == 0xFD28D7 || results.value == 0xc48){ //4 on remote for "WHY"
      WHY_SubRoutine();
      }
      if(results.value == 0xFD6897 || results.value == 0xe68){//6 on remote for "NOW"
      NOW_SubRoutine();
      }
      if(results.value == 0xFD9867 || results.value == 0x088){//8 on remote for "WHY NOW?"
      //"FUTURE HELP"
      FUTURE_SubRoutine();
      HELP_SubRoutine();

      //send the signal to HOME to Roberta, then delay 10 seconds in between
      //before sending the signal Attention to Roberta to start conv again
      for (int i = 0; i < 3; i++) {
        irsend.sendSony(0x508, 12);
      delay(40);
      }
      
      digitalWrite(Enable, LOW);
      delay(2000);
      digitalWrite(Enable, HIGH);
      HomeSubRoutine();
      convCntr += 1;
      if(convCntr != 2){
        //delay 10 seconds here, then send code to Roberta to start covn again
        delay(10000);
        //Send code to tell Roberta to Attention
        for (int i = 0; i < 3; i++) {
          irsend.sendSony(0x288, 12);
          delay(40);
        }
      }
      
      
      }
      irrecv.enableIRIn(); // Re-enable receiver
      irrecv.resume(); // Receive the next value
    }

    LThomeVal = digitalRead(LeftHomeSwitch);
    RThomeVal = digitalRead(RightHomeSwitch);

    if(convCntr == 2){
      convCntr = 0;
      //delay for 5 minutes, disableoutputs to not generate heat
      //reset convCntr then repeat the process again
      digitalWrite(Enable, LOW);
      //delay(100000); //was 5 mins, 300000ms //set this less delay than roberta to make sure
      //that this bot gets to waiting loop before roberta sends signal...
      //delay(10000);
      //get the code from roberta here:
      /*while(1){
        if (irrecv.decode(&results)) {
          if(results.value == 0x908){ //break conv loop, no continued detection
            motionDetected = false;
            breakConvLoop = true;
            Serial.println("got code from roberta to BREAK, no motion");
            break;
          }
          if(results.value == 0x718){ //continue conversation...
            motionDetected = true;
            Serial.println("got code from roberta to CONTINUE");
            break;
          }
        }
      }*/
      
    
      //for (int i = 0; i < 3; i++) {
        //irsend.sendSony(0x288, 12);
        //delay(40);
      //}
      
      irrecv.enableIRIn(); // Re-enable receiver

      //whichever bot ceases to detect motion first, needs to send signal to other bot to
      //exit this loop and go back to the top waiting loop...
      //but should roberto simply send a "proceed" (i.e. motion has been detected within 20 minutes
      //or "done" (break the while(1) loop) signal to roberta after every 2 conversations?
    
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





