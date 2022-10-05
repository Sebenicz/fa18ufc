
#include <Joystick.h>
#include <Arduino.h>
#include <RotaryEncoder.h>

RotaryEncoder *encoder1 = nullptr;
RotaryEncoder *encoder2 = nullptr;

void checkPosition1()
{
  encoder1->tick(); // just call tick() to check the state.
}
void checkPosition2()
{
  encoder2->tick(); // just call tick() to check the state.
}

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  52, 0,                  // Button Count, Hat Switch Count
  true, true, true,     // X and Y, Z Axis
  true, true, true,   //  Rx, Ry, Rz
  false, true,          //rudder, no throttle
  false, false, false);  // No accelerator, brake, or steering


int buttonPins[] = {2,3,4,5,6,7,8,9,10,
                    11,12,14,15,16,17,18,
                    19,20,21,22,23,24,25,
                    26,27,44,45,46,49,48,42,43};
int buttonCount = 32;

int buttonButtons[] = {0,1,2,3,4,5,6,7,8,9,10,
                        11,12,13,14,15,16,17,18,19,20,21,
                        22,23,24,25,26,27,44,45,46,47};

//new/old encoder state left/right
                        
int oesl = 0;
int oesr = 0;
int posl = 0;
int posr = 0;
void setup() {
  pinMode(2, INPUT_PULLUP);//kb3 
  pinMode(3, INPUT_PULLUP);//kbclr
  pinMode(4, INPUT_PULLUP);//ob2
  pinMode(5, INPUT_PULLUP);//ob3
  pinMode(6, INPUT_PULLUP);//ob4
  pinMode(7, INPUT_PULLUP);//ob5
  pinMode(8, INPUT_PULLUP);//ob1
  pinMode(9, INPUT_PULLUP);//kbzb
  pinMode(10, INPUT_PULLUP);//ob0
  pinMode(11, INPUT_PULLUP);//kb a
  pinMode(12, INPUT_PULLUP); //kb g
  pinMode(14, INPUT_PULLUP);//kb5
  pinMode(15, INPUT_PULLUP);//kb8
  pinMode(16, INPUT_PULLUP);//kb2
  pinMode(17, INPUT_PULLUP);//kb0
  pinMode(18, INPUT_PULLUP);//kbent
  pinMode(19, INPUT_PULLUP);//kb4
  pinMode(20, INPUT_PULLUP);//kb7
  pinMode(21, INPUT_PULLUP);//kb1
  pinMode(22, INPUT_PULLUP);//kb f
  pinMode(23, INPUT_PULLUP);//kb c
  pinMode(24, INPUT_PULLUP);//kb e
  pinMode(25, INPUT_PULLUP);//kb d
  pinMode(26, INPUT_PULLUP);//enco b l
  pinMode(27, INPUT_PULLUP);//enco b r
  pinMode(28, INPUT_PULLUP);//enco l1
  pinMode(29, INPUT_PULLUP);//enco l2
  pinMode(30, INPUT_PULLUP);//enco r1
  pinMode(31, INPUT_PULLUP);//enco r2
  pinMode(32, INPUT_PULLUP);//3sw ufc up
  pinMode(33, INPUT_PULLUP);//3sw ufc dw
  pinMode(34, INPUT_PULLUP);//3sw up pd
  pinMode(35, INPUT_PULLUP);//3sw dw pd
  pinMode(36, INPUT_PULLUP);//3sw up lg
  pinMode(37, INPUT_PULLUP);//3sw dw lg
  pinMode(38, INPUT_PULLUP);//3sw up pg
  pinMode(39, INPUT_PULLUP);//3sw dw pg
  pinMode(40, INPUT_PULLUP);//2sw dw d
  pinMode(41, INPUT_PULLUP);//2sw dw g
  pinMode(42, INPUT_PULLUP);//2sw lew  sprg
  pinMode(43, INPUT_PULLUP);//2sw praw sprg
  pinMode(44, INPUT_PULLUP);//kb6
  pinMode(45, INPUT_PULLUP);//kb9
  pinMode(46, INPUT_PULLUP);//kb b
  pinMode(49, INPUT_PULLUP);//nowy sprg
  pinMode(48, INPUT_PULLUP);//nowy sprg
  encoder1 = new RotaryEncoder(28, 29, RotaryEncoder::LatchMode::FOUR3);
  encoder2 = new RotaryEncoder(30, 31, RotaryEncoder::LatchMode::FOUR3);
  attachInterrupt(digitalPinToInterrupt(28), checkPosition1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(29), checkPosition1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(30), checkPosition2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(31), checkPosition2, CHANGE);
  Joystick.begin(false);
}

void loop() {
  //ustawia wszystkie anale
  Joystick.setXAxis(analogRead(A0));
  Joystick.setYAxis(analogRead(A1));  
  Joystick.setZAxis(analogRead(A2));
  Joystick.setRxAxis(analogRead(A3));
  Joystick.setRyAxis(analogRead(A4));  
  Joystick.setRzAxis(analogRead(A5));
  Joystick.setThrottle(analogRead(A6));  
  Joystick.sendState();
  //ustawia buttony 0-27 oraz 44-47
  for(int i = 0; i<buttonCount; i++){ 
    
    if(digitalRead(buttonPins[i]) == LOW){
      Joystick.pressButton(buttonButtons[i]);
    }else{
      Joystick.releaseButton(buttonButtons[i]);      
    }
  }
  Joystick.sendState();
  //butt 28,29,30
  set3buttons(28, getSwitchState(32,33));
  //butt 31,32,33
  set3buttons(31, getSwitchState(34,35));
  //butt 34,35,36
  set3buttons(34, getSwitchState(36,37));
  //butt 37,38,39
  set3buttons(37, getSwitchState(38,39));

  //b 40,41
  set2buttons(40, 40);
  //b 42,43
  set2buttons(42, 41);
  //b 44,45
  //set2buttons(44, 42); stare
  //b 46,47 nic
  //set2buttons(46, 43);
  Joystick.sendState();
  //buttony enkodera 48,49 oraz 50, 51
  encoder1->tick();
  encoder2->tick();
  posl = blinkEncoder(48, oesl, encoder1->getPosition());
  posr = blinkEncoder(50, oesr, encoder2->getPosition());
  oesl = posl;
  oesr = posr;
}

int blinkEncoder(int buttonNr, int oldState, int newState){
  int encoDiff = oldState - newState;
  
  if(encoDiff != 0){
    
    if(encoDiff > 0){
      for(int i = encoDiff; i>0; i--){
        Joystick.pressButton(buttonNr);
        Joystick.sendState();
        delay(10);
        Joystick.releaseButton(buttonNr);
        Joystick.sendState();
        }
    }else{
      for(int i = encoDiff; i<0; i++){
        Joystick.pressButton(buttonNr + 1);
        Joystick.sendState();
        delay(10);
        Joystick.releaseButton(buttonNr + 1);
        Joystick.sendState();
        }
        }
    
    
    return newState;
  }else{return oldState;}
}

void set2buttons(int buttonNr, int pin){
  if(digitalRead(pin) == LOW){
    Joystick.releaseButton(buttonNr+1);
    Joystick.pressButton(buttonNr);
  }else{
    Joystick.releaseButton(buttonNr);
    Joystick.pressButton(buttonNr+1);
  }
}

void set3buttons(int buttonNr, int state){
  switch (state) {
  case 0:
    Joystick.releaseButton(buttonNr);
    Joystick.releaseButton(buttonNr+2);
    Joystick.pressButton(buttonNr+1);
    break;
  case 1:
    Joystick.releaseButton(buttonNr+1);
    Joystick.releaseButton(buttonNr+2);
    Joystick.pressButton(buttonNr);
    break;
  case 2:
    Joystick.releaseButton(buttonNr);
    Joystick.releaseButton(buttonNr+1);
    Joystick.pressButton(buttonNr+2);
  default:
    break;
  }
}

int getSwitchState(int pin1, int pin2) {

  //0-na srodku
  //1-u gory
  //2-na dole
  if(digitalRead(pin1) == HIGH  && digitalRead(pin2) == HIGH){
      return 0;
    }else if(digitalRead(pin1) == LOW && digitalRead(pin2) == HIGH){
      return 1;
    }else{
      return 2;
    }
      
}


  
