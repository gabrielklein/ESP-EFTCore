#include "SetupRGB.h"


namespace eft {

SetupRGB::SetupRGB(int pinR,int pinG,int pinB) {

  this->pinR = pinR;
  this->pinG = pinG;
  this->pinB = pinB;

  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
  
  digitalWrite(pinR, LOW);
  digitalWrite(pinG, LOW);
  digitalWrite(pinB, LOW);

};

SetupRGB::~SetupRGB() {

};

void SetupRGB::error() {
  digitalWrite(pinR, HIGH);
  digitalWrite(pinG, LOW);
  digitalWrite(pinB, LOW);
};

void SetupRGB::success() {
  digitalWrite(pinR, LOW);
  digitalWrite(pinG, HIGH);
  digitalWrite(pinB, LOW);
};

void SetupRGB::client(String ip) {
  digitalWrite(pinR, LOW);
  digitalWrite(pinG, LOW);
  digitalWrite(pinB, HIGH);
};

void SetupRGB::inprogress() {
  digitalWrite(pinR, LOW);
  digitalWrite(pinG, LOW);
  digitalWrite(pinB, HIGH);
}


}
