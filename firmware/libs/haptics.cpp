#include <Arduino.h>

int vibePin=9;
bool vibeOn=false;
unsigned long vibeEnd=0;
unsigned long nowTime=0;
int vibePattern=0;
unsigned long phaseEnd=0;

void HapticsInit(int pin){
  vibePin=pin;
  pinMode(vibePin,OUTPUT);
  digitalWrite(vibePin,LOW);
}

void HapticsStart(int type){
  vibePattern=type;
  vibeOn=true;
  nowTime=millis();
  if(type==1){
    analogWrite(vibePin,220);
    vibeEnd=nowTime+80;
  }
  else if(type==2){
    analogWrite(vibePin,180);
    phaseEnd=nowTime+60;
    vibeEnd=nowTime+160;
  }
  else if(type==3){
    analogWrite(vibePin,255);
    vibeEnd=nowTime+250;
  }
}

void HapticsUpdate(){
  if(!vibeOn) return;
  nowTime=millis();
  if(vibePattern==2){
    if(nowTime>=phaseEnd && nowTime<vibeEnd){
      analogWrite(vibePin,220);
    }
  }
  if(nowTime>=vibeEnd){
    analogWrite(vibePin,0);
    vibeOn=false;
  }
}

void HapticsStop(){
  analogWrite(vibePin,0);
  vibeOn=false;
}