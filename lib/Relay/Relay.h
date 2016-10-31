#include <Arduino.h>

#define RELAY_ON   HIGH
#define RELAY_OFF  LOW
#define RELAY_NULL HIGH + LOW + 1


class Relay {
 public:
  Relay (int pinNum);

  void init ();

  void changeState(int);

  void on ();
  void off ();

 private:
  const int _pinNum;
  int _state;

};

