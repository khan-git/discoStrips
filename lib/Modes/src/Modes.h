#ifndef MODES_H
#define MODES_H
#include <Arduino.h>
#include "DuinoRESTIf.h"

using namespace std;

class Modes: public DuinoRESTIf {
  public:
    enum Values {Off, On, Disco, Night};

    Modes()
    {
      currentMode = previousMode = Off;
      prefix = "/mode";
    }

    void restParsing(String line, Answer* answer) {
      // if(!isPutOrGet(line)) { return;}


    }

    Values getMode()
    {
      return currentMode;
    }

    Values pushedButton(Values button)
    {
      previousMode = currentMode;

      if(button == currentMode)
      {
        currentMode = Off;
      }
      else
      {
        currentMode = button;
      }
      return currentMode;
    }

  private:
    Values currentMode;
    Values previousMode;
};

#endif
