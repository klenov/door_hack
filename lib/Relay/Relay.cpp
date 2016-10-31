#include "relay.h"

Relay::Relay (int pinNum) : _pinNum(pinNum), _state(RELAY_NULL) {
}


void Relay::init () {
  pinMode(_pinNum, OUTPUT);

  off();
}

void Relay::changeState(int new_state) {
  if(_state == new_state)
    return;

  _state = new_state;
  digitalWrite(_pinNum, _state);
}


void Relay::on () {
  changeState(RELAY_ON);
}


void Relay::off () {
  changeState(RELAY_OFF);
};
