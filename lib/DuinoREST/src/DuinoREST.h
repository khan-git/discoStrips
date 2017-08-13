#ifndef DUINO_REST_H
#define DUINO_REST_H
#include <Arduino.h>
#include <WiFi101.h>
#include "Answer.h"

using namespace std;

class DuinoREST {
public:
  DuinoREST();
  ~DuinoREST();

  void handle(WiFiClient client);
  void function(char* name, int (*f)(String, Answer));
private:
  String answer;

};
#endif
