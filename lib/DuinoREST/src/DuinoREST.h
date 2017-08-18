#ifndef DUINO_REST_H
#define DUINO_REST_H
#include <Arduino.h>
#include <WiFi101.h>
#include "Answer.h"
#include "FunctionList.h"

using namespace std;

class DuinoREST {
public:
  DuinoREST();
  ~DuinoREST();

  void handle(WiFiClient& client);
  void addHandler(char* name, void (*f)(String, Answer*));

private:
  Answer* answer;
  FunctionList* funcList;

};
#endif
