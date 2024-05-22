//XLimit
//YLimit

#include "global_variables.h"

void EndOfWordSubRoutine() {


 //"End of work"
Serial.println("END OF WORD") ;
LeftMotor.moveTo(233);  //was 250
RightMotor.moveTo(-233); //was -250
MoveSubRoutine();
delay(letterDelay);

}
