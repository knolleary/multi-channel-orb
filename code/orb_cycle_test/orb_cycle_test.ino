/*
  Cycles through each LED of the orb
  2011-10-05 <nick.oleary@gmail.com> 
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

#define LEDA(r1,g1,b1) PORTD = (PORTD&0x03)|(r1*l1rm)|(g1*l1gm)|(b1*l1bm)
#define LEDB(r2,g2,b2) PORTC = (PORTC&0xF8)|(r2*l2rm)|(g2*l2gm)|(b2*l2bm)
#define LEDC(r3,g3,b3) PORTC = (PORTC&0xC7)|(r3*l3rm)|(g3*l3gm)|(b3*l3bm)



void setup() {
  DDRD = DDRD | B11111100;
  DDRC = DDRC | B00111111;
  delay(1000);
}



void loop() {
  
  LEDA(HIGH,LOW,LOW);
  delay(500);
  LEDA(LOW,HIGH,LOW);
  delay(500);
  LEDA(LOW,LOW,HIGH);
  delay(500);
  LEDA(LOW,LOW,LOW);

  LEDB(HIGH,LOW,LOW);
  delay(500);
  LEDB(LOW,HIGH,LOW);
  delay(500);
  LEDB(LOW,LOW,HIGH);
  delay(500);
  LEDB(LOW,LOW,LOW);


  LEDC(HIGH,LOW,LOW);
  delay(500);
  LEDC(LOW,HIGH,LOW);
  delay(500);
  LEDC(LOW,LOW,HIGH);
  delay(500);
  LEDC(LOW,LOW,LOW);
}
