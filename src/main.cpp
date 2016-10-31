/*

  По прерыванию считаем изменения состояния пина SENSOR_PIN (это спикер в трубке).
  Если за последнюю секунду было больше чем SENSOR_INTERRUPTS_THRESHOLD прерываний, то поднимаем трубку
  и открываем дверь.

*/


#include <ESP8266WiFi.h>
// #include <ESP8266HTTPClient.h>

#include "advancedSerial.h"

#include "Relay.h"

#define SENSOR_PIN D6
#define SENSOR_INTERRUPTS_THRESHOLD 400

#define ENABLE_WIFI 0 // WIFI пока выключен

Relay relay_phone_pickup(D4);
Relay relay_door_open(D8);

#if defined(ENABLE_WIFI)
  const char wifi_ssid[]      = "";
  const char wifi_password[]  = "";
#endif

volatile int sensor_interrupts_count = 0;
unsigned long last_check_at = 0;

void pin_as_input(int pin) {
  pinMode(pin, INPUT);
  pinMode(pin, INPUT_PULLUP);
}

void inc_counter() {
  sensor_interrupts_count += 1;
}

void open_door() {
  aSerial.l(Level::v).pln(F("Opening door..."));

  relay_phone_pickup.on(); aSerial.l(Level::v).pln(F("pickup"));

  delay(3500);

  relay_door_open.on(); aSerial.l(Level::v).pln(F("open"));

  delay(2000);

  relay_door_open.off();
  relay_pickup.off();

  delay(5000);
}

void setup() {
  Serial.begin(115200); aSerial.setPrinter(Serial); aSerial.setFilter(Level::vv);
  aSerial.l(Level::v).pln(F("Setup..."));

  relay_phone_pickup.init();
  relay_door_open.init();
  
  #if defined(ENABLE_WIFI)
    WiFi.begin(wifi_ssid, wifi_password);
  #endif

  pin_as_input(SENSOR_PIN);
  attachInterrupt(SENSOR_PIN, inc_counter, FALLING);
}

void loop() {
  #if defined(ENABLE_WIFI)
    if ( WiFi.status() != WL_CONNECTED ) {
      aSerial.l(Level::v).pln(F("Can't connect to WIFI!"));
    }
  #endif

  if( millis() - last_check_at > 1000 ) {
    aSerial.l(Level::v).pln(sensor_interrupts_count);

    if( sensor_interrupts_count > SENSOR_INTERRUPTS_THRESHOLD ) {
      open_door();
    }
    sensor_interrupts_count = 0;
    last_check_at = millis();
  }

}