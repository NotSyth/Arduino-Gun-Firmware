#include <Arduino.h>
#ifdef LEDS_USE_NEOPIXEL
#include <Adafruit_NeoPixel.h>
static Adafruit_NeoPixel* strip=nullptr;
#endif

static int ledPin=-1;
static bool useNeo=false;
static bool flashActive=false;
static unsigned long flashEnd=0;
static bool pulseActive=false;
static unsigned long pulseNext=0;
static bool pulseOn=false;
static uint16_t pulseOnMs=0;
static uint16_t pulseOffMs=0;
static uint8_t cr=255,cg=255,cb=255;

static void writeColor(uint8_t r,uint8_t g,uint8_t b){
#ifdef LEDS_USE_NEOPIXEL
  if(useNeo&&strip){
    for(uint16_t i=0;i<strip->numPixels();i++) strip->setPixelColor(i,strip->Color(r,g,b));
    strip->show();
    return;
  }
#endif
  uint8_t v=max(r,max(g,b));
  analogWrite(ledPin,v);
}

void LedsInit(int pin,int count){
  ledPin=pin;
#ifdef LEDS_USE_NEOPIXEL
  if(count>0){
    if(strip) delete strip;
    strip=new Adafruit_NeoPixel(count,pin,NEO_GRB+NEO_KHZ800);
    strip->begin();
    strip->clear();
    strip->show();
    useNeo=true;
    return;
  }
#endif
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);
  useNeo=false;
}

void LedsSetRGB(uint8_t r,uint8_t g,uint8_t b){
  cr=r;cg=g;cb=b;
  writeColor(r,g,b);
}

void LedsFlash(uint16_t ms){
  flashActive=true;
  flashEnd=millis()+ms;
  writeColor(255,255,255);
}

void LedsPulse(uint16_t onMs,uint16_t offMs,uint8_t r,uint8_t g,uint8_t b){
  cr=r;cg=g;cb=b;
  pulseActive=true;
  pulseOnMs=onMs;
  pulseOffMs=offMs;
  pulseOn=true;
  pulseNext=millis()+pulseOnMs;
  writeColor(cr,cg,cb);
}

void LedsStopPulse(){
  pulseActive=false;
  pulseOn=false;
}

void LedsOff(){
  flashActive=false;
  pulseActive=false;
#ifdef LEDS_USE_NEOPIXEL
  if(useNeo&&strip){strip->clear();strip->show();return;}
#endif
  analogWrite(ledPin,0);
}

void LedsUpdate(){
  unsigned long t=millis();
  if(flashActive&&t>=flashEnd){
    flashActive=false;
    if(!pulseActive) writeColor(0,0,0);
  }
  if(pulseActive&&t>=pulseNext){
    pulseOn=!pulseOn;
    pulseNext=t+(pulseOn?pulseOnMs:pulseOffMs);
    if(pulseOn) writeColor(cr,cg,cb); else writeColor(0,0,0);
  }
}