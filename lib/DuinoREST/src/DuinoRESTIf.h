#ifndef DUINO_REST_IF_H
#define DUINO_REST_IF_H
#include "Answer.h"
#include "Commands.h"

class DuinoRESTIf {
  public:
    virtual void restParsing(const Commands* commands, Answer* answer) = 0;

    void parseREST(const Commands* commands, Answer* answer)
    {
      // this->commands = commands;
    }

  protected:
    bool isPutOrGet()
    {
      // if(commands.startsWith("PUT ") || line.startsWith("GET "))
      // {
      //   return true;
      // }
      return false;
    }

    int startsWith(int cmdIndex, const String cmpString)
    {
      // int result = 0;
      // int maxIndex = cmpString.length() > commands->getMaxLength()?commands->getMaxLength(): cmpStr.length();
      // const char* cmd = commands->getCmd(cmdIndex);
      // for(int index = 0; index < maxIndex; index++)
      // {
      //   result += cmd[index] - cmpString[index];
      // }
      // return result;
    }

    String getCmd(String line, int level)
    {
      return "";
    }

    String prefix;
    Commands* commands;
};

#endif
