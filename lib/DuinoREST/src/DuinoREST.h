#ifndef DUINO_REST_H
#define DUINO_REST_H
#include <Arduino.h>
#include <WiFi101.h>
#include "Answer.h"
#include "FunctionList.h"
#include "SimpleVector.h"

using namespace std;

class DuinoREST {
  public:
    DuinoREST(int cmdLevels, int cmdLength);
    DuinoREST(int cmdLevels): DuinoREST(cmdLevels, 20) {}
    DuinoREST(): DuinoREST(5, 20) {}
    ~DuinoREST();

    void handle(WiFiClient& client);
    void addHandler(char* name, void (*f)(String, Answer*));

  private:
    Answer* answer;
    FunctionList* funcList;
    SimpleVector<char>* commands;

    void clearCommands();
    void splitCommands(String line);

};
#endif
