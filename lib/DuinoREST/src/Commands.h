#ifndef COMMANDS_H
#define COMMANDS_H
#include "SimpleVector.h"

using namespace std;

class Commands {
  public:
    Commands(int cmdLevels, int cmdLength)
    {
      commands = new SimpleVector<char>(cmdLevels, cmdLength);
      maxLength = cmdLength;
      maxLevels = cmdLevels;
    }

    Commands(int cmdLevels): Commands(cmdLevels, 20) {}

    Commands(): Commands(5, 20) {}

    ~Commands()
    {
      delete[] commands;
    }

    int getMaxLength() {return maxLength;}
    int getMaxLevels() {return maxLevels;}

    const char* getCmd(int index)
    {
      if(index < maxLevels)
      {
        return commands->get(index);
      }
      return commands->get(0);
    }
  private:
    SimpleVector<char>* commands;
    int maxLength;
    int maxLevels;


};
#endif
