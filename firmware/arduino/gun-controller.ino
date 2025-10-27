#include <Mouse.h>

const int TRIGGER_PIN=2;
const int RELOAD_PIN=3;
const int VIBE_PIN=9;
const int FLASH_PIN=6;
const int JOY_X=A0;
const int JOY_Y=A1;

const int DEADZONE=60;
const int SENS=10;
const int LOOP_MS=8;

int ammoMax=12;
int ammo=12;
bool reloading=false;
unsigned long reloadEnd=0;
const unsigned long reloadTime=1500;

bool lastTrig=false;
bool lastReload=false;

bool vibeActive=false;
unsigned long vibeEnd=0;
const unsigned long vibeDur1=60;
const unsigned long vibeGap=40;
const unsigned long vibeDur2=80;
unsigned long vibePhaseEnd=0;
int vibePhase=0;

bool flashActive=false;
unsigned long flashEnd=0;
const unsigned long flashDur=45;

void setup(){
  pinMode(TRIGGER_PIN,INPUT_PULLUP);
  pinMode(RELOAD_PIN,INPUT_PULLUP);
  pinMode(VIBE_PIN,OUTPUT);
  pinMode(FLASH_PIN,OUTPUT);
  digitalWrite(VIBE_PIN,LOW);
  digitalWrite(FLASH_PIN,LOW);
  Mouse.begin();
  delay(50);
}

void startVibePattern(){
  vibeActive=true;
  vibePhase=1;
  vibeEnd=millis()+vibeDur1+vibeGap+vibeDur2;
  vibePhaseEnd=millis()+vibeDur1;
  analogWrite(VIBE_PIN,200);
}

void updateVibe(){
  if(!vibeActive) return;
  unsigned long t=millis();
  if(vibePhase==1){
    if(t>=vibePhaseEnd){
      vibePhase=2;
      vibePhaseEnd=t+vibeDur2;
      analogWrite(VIBE_PIN,0);
    } else {
      analogWrite(VIBE_PIN,200);
    }
  } else if(vibePhase==2){
    if(t>=vibePhaseEnd){
      vibeActive=false;
      analogWrite(VIBE_PIN,0);
    } else {
      analogWrite(VIBE_PIN,220);
    }
  }
}

void startFlash(){
  flashActive=true;
  flashEnd=millis()+flashDur;
  digitalWrite(FLASH_PIN,HIGH);
}

void updateFlash(){
  if(!flashActive) return;
  if(millis()>=flashEnd){
    flashActive=false;
    digitalWrite(FLASH_PIN,LOW);
  }
}

void shoot(){
  if(reloading) return;
  if(ammo>0){
    Mouse.click(MOUSE_LEFT);
    startVibePattern();
    startFlash();
    ammo--;
  } else {
    startVibePattern();
    startFlash();
  }
}

void beginReload(){
  if(reloading) return;
  reloading=true;
  reloadEnd=millis()+reloadTime;
  startVibePattern();
}

void updateReload(){
  if(!reloading) return;
  if(millis()>=reloadEnd){
    reloading=false;
    ammo=ammoMax;
    for(int i=0;i<3;i++){
      analogWrite(VIBE_PIN,200);
      delay(80);
      analogWrite(VIBE_PIN,0);
      delay(60);
    }
  }
}

void loop(){
  int rawX=analogRead(JOY_X)-512;
  int rawY=analogRead(JOY_Y)-512;
  int moveX=0,moveY=0;
  if(abs(rawX)>DEADZONE) moveX=map(rawX,-512,512,-SENS,SENS);
  if(abs(rawY)>DEADZONE) moveY=map(rawY,-512,512,-SENS,SENS);
  if(moveX||moveY) Mouse.move(moveX,-moveY);
  bool trig=digitalRead(TRIGGER_PIN)==LOW;
  bool reloadBtn=digitalRead(RELOAD_PIN)==LOW;
  if(trig && !lastTrig){
    shoot();
  }
  lastTrig=trig;
  if(reloadBtn && !lastReload){
    beginReload();
  }
  lastReload=reloadBtn;
  updateVibe();
  updateFlash();
  updateReload();
  delay(LOOP_MS);
}