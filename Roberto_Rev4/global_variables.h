// This is only needed for VSCode intellisense, not needed for Arduino IDE

#pragma once

#include <AccelStepper.h>
#include <IRremote.h>

#define LeftHomeSwitch 9
#define RightHomeSwitch 10

extern AccelStepper LeftMotor;
extern AccelStepper RightMotor;
extern IRrecv irrecv;
extern IRsend irsend;

extern const int stepsPerDeg;
extern int letterDelay;
extern int LThomeVal;
extern int RThomeVal;
extern int Speed;
extern int Acceleration;

extern void MoveSubRoutine();
extern void EndOfWordSubRoutine();
extern void homeLeft();
extern void homeRight();
extern void HomeSubRoutine();
extern void AttentionSubRoutine();
extern void HELP_SubRoutine();
extern void FUTURE_SubRoutine();
extern void WHY_SubRoutine();
extern void NOW_SubRoutine();
extern void AcknowledgeSubRoutine();
