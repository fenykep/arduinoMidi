#include <stdint.h>
#include "TouchScreen.h"
#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

// These are the pins for the shield!
#define YP A2  
#define XM A1  
#define YM 14  
#define XP 17 

bool flip = 0;

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

//channel 1-2 X,Y (B0, B1 for control change)
//channel 3 touchTrigger (status byte 92)
//MIDI.sendNoteOn(42, 127, 3);
//MIDI.sendNoteOff(42, 0, 3);

void setup(void) {
  MIDI.begin();
}

void loop(void) {
  // a point object holds x y and z coordinates
  TSPoint p = ts.getPoint();
  
  if (p.z > 0) {
     MIDI.sendControlChange(54,map(p.x, 170, 850, 0, 120),1);
     MIDI.sendControlChange(55,map(p.y, 120, 860, 0, 120),2);
     //55 and 54 are the channel or port or whatever of the "pots"
     if (flip==0){
      flip=1;
      MIDI.sendNoteOn(42, 127, 3);
     }
  }
  else{
    if (flip==1){
      flip=0;
      MIDI.sendNoteOff(42, 0, 3);
    }
  }
}
