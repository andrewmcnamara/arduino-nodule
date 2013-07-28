/*--------------------------------------------------------------------------------------
  Includes
--------------------------------------------------------------------------------------*/
#include <SPI.h>        //SPI.h must be included as DMD is written by SPI (the IDE complains otherwise)
#include <DMD.h>        //
#include <TimerOne.h>   //
#include "Arial_black_16.h"
#include "SystemFont5x7.h"

//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 1
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);
boolean textComplete = false;
String inputString = "";         // a string to hold incoming data
char stuff[100]={};
int count= 0;

/*--------------------------------------------------------------------------------------
  Interrupt handler for Timer1 (TimerOne) driven DMD refresh scanning, this gets
  called at the period set in Timer1.initialize();
--------------------------------------------------------------------------------------*/
void ScanDMD()
{ 
  dmd.scanDisplayBySPI();
}

/*--------------------------------------------------------------------------------------
  setup
  Called by the Arduino architecture before the main loop begins
--------------------------------------------------------------------------------------*/
void setup(void)
{
    // initialize serial:
  Serial.begin(9600);

   //initialize TimerOne's interrupt/CPU usage used to scan and refresh the display
   Timer1.initialize( 5000 );           //period in microseconds to call ScanDMD. Anything longer than 5000 (5ms) and you can see flicker.
   Timer1.attachInterrupt( ScanDMD );   //attach the Timer1 interrupt to ScanDMD which goes to dmd.scanDisplayBySPI()

   //clear/init the DMD pixels held in RAM
   dmd.clearScreen( true );   //true is normal (all pixels off), false is negative (all pixels on)

}

/*--------------------------------------------------------------------------------------
  loop
  Arduino architecture main loop
--------------------------------------------------------------------------------------*/
void loop(void)
{
   byte b;
   while (Serial.available() && textComplete == false) {
     char inChar = (char)Serial.read();
     if(inChar == '~'){ // end character for toggle LED
      textComplete = true;
     }
     else{
       //inputString += inChar;
       stuff[count++]=inChar;
     }
   }
   if (!Serial.available() && textComplete == true)
   {
     
 
   // 10 x 14 font clock, including demo of OR and NOR modes for pixels so that the flashing colon can be overlayed
   //dmd.clearScreen( true );
   dmd.selectFont(Arial_Black_16);
   // display some text
   dmd.clearScreen( true );
   dmd.selectFont(System5x7);

//   for (byte x=0;x<DISPLAYS_ACROSS;x++) {
//     for (byte y=0;y<DISPLAYS_DOWN;y++) {
//       dmd.drawString(  2+(32*x),  1+(16*y), stuff, count, GRAPHICS_NORMAL );
//       dmd.drawString(  2+(32*x),  9+(16*y), "ronic", 5, GRAPHICS_NORMAL );
//     }
//   }
   dmd.drawMarquee(stuff,count,(32*DISPLAYS_ACROSS)-1,0);
   inputString="";
   textComplete =false;
   count =0;

   //delay( 7000 );

   }
}

