/*
  Cycles through each LED of the orb
   - using PWM to fade between them
   - uses a variation on the TCNT0/slot method discussed here:
     http://jeelabs.org/2010/10/03/software-pwm-at-1-khz/

  2011-11-19 <nick.oleary@gmail.com> 
  http://opensource.org/licenses/mit-license.php
*/

#define l1rm 0x80
#define l1gm 0x40
#define l1bm 0x20

#define l2rm 0x01
#define l2gm 0x02
#define l2bm 0x04

#define l3rm 0x08
#define l3gm 0x10
#define l3bm 0x20

byte current[9];

byte fadeTarget[9];
int fadeSteps[9];
int fadeStep;
int fadeStepCount = 20;
boolean fading;
int stepCount;

unsigned long timer;

byte portDSlots[256];
byte portCSlots[256];

void jumpTo(int r1,int g1,int b1,int r2,int g2,int b2,int r3,int g3,int b3) {
  current[0] = r1;
  current[1] = g1;
  current[2] = b1;
  current[3] = r2;
  current[4] = g2;
  current[5] = b2;
  current[6] = r3;
  current[7] = g3;
  current[8] = b3;
  setupSlots(r1,g1,b1,r2,g2,b2,r3,g3,b3);
}

void setupSlots(int r1,int g1,int b1,int r2,int g2,int b2,int r3,int g3,int b3) {
  current[0] = r1;
  current[1] = g1;
  current[2] = b1;
  current[3] = r2;
  current[4] = g2;
  current[5] = b2;
  current[6] = r3;
  current[7] = g3;
  current[8] = b3;
  
  memset(portDSlots,0,256);
  memset(portCSlots,0,256);
  
  for (int i=0;i<256;i++) {
    portDSlots[i] = ((r1>i)?l1rm:0)|((g1>i)?l1gm:0)|((b1>i)?l1bm:0);
    portCSlots[i] = ((r2>i)?l2rm:0)|((g2>i)?l2gm:0)|((b2>i)?l2bm:0)|
                    ((r3>i)?l3rm:0)|((g3>i)?l3gm:0)|((b3>i)?l3bm:0);
  }
}

void fadeTo(int r1,int g1,int b1,int r2,int g2,int b2,int r3,int g3,int b3) {
  fadeTarget[0] = r1;
  fadeTarget[1] = g1;
  fadeTarget[2] = b1;
  fadeTarget[3] = r2;
  fadeTarget[4] = g2;
  fadeTarget[5] = b2;
  fadeTarget[6] = r3;
  fadeTarget[7] = g3;
  fadeTarget[8] = b3;
  
  for (int i=0;i<9;i++) {
    fadeSteps[i] = (fadeTarget[i]-current[i])/fadeStepCount;
  }
  fadeStep = 0;
  fading = true;

}
void stepFade() {
  fadeStep++;
  if (fadeStep == fadeStepCount) {
    for (int i=0;i<9;i++) {
      current[i] = fadeTarget[i];
    }
    fading = false;

  } else {
    for (int i=0;i<9;i++) {
      current[i] += fadeSteps[i];
      if (fadeSteps[i] > 0 && current[i] > fadeTarget[i]) current[i] = fadeTarget[i];
      else if (fadeSteps[i] < 0 && current[i] < fadeTarget[i]) current[i] = fadeTarget[i];
    }
  }
  jumpTo(current[0],current[1],current[2],current[3],current[4],current[5],current[6],current[7],current[8]);
}


void setup() {
  DDRD = DDRD | B11111100;
  DDRC = DDRC | B00111111;
  fading = false;
  jumpTo(0,0,0,0,0,0,0,0,0);
  timer = millis();
}

int state = 0;

void loop() {
  if (millis()-timer > 1000) {
    timer = millis();
    if (state == 0) {
      fadeTo(255,0,0,0,0,0,0,0,0);
    } else if (state == 1) {
      fadeTo(0,255,0,0,0,0,0,0,0);
    } else if (state == 2) {
      fadeTo(0,0,255,0,0,0,0,0,0);
    } else if (state == 3) {
      fadeTo(0,0,0,255,0,0,0,0,0);
    } else if (state == 4) {
      fadeTo(0,0,0,0,255,0,0,0,0);
    } else if (state == 5) {
      fadeTo(0,0,0,0,0,255,0,0,0);
    } else if (state == 6) {
      fadeTo(0,0,0,0,0,0,255,0,0);
    } else if (state == 7) {
      fadeTo(0,0,0,0,0,0,0,255,0);
    } else if (state == 8) {
      fadeTo(0,0,0,0,0,0,0,0,255);
    }
    state++;
    if (state == 9) state = 0;
  }
  
  PORTD = (PORTD&0x03)|portDSlots[TCNT0];
  PORTC = (PORTC&0xC0)|portCSlots[TCNT0];
  
  if (fading) {
    if (TCNT0 == 255) {
      stepCount++;
    }
    if (stepCount == 16) {
      stepFade();
      stepCount = 0;
    }
  }
}
