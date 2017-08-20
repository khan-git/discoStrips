#ifndef FUNCTION_LIST_H
#define FUNCTION_LIST_H
#include <Arduino.h>
#include "Answer.h"

using namespace std;

struct FuncItem {
  void (*function)(String, Answer*);
  char* name;
  struct FuncItem* prev;
  struct FuncItem* next;
};

class FunctionList {

public:
  ~FunctionList()
  {
    while(functionList != NULL)
    {
      if(functionList->next != NULL)
      {
        functionList = functionList->next;
        delete[] functionList->prev;
        functionList->prev = NULL;
      }
      else
      {
        delete[] functionList;
        functionList = NULL;
      }
    }
  }

  void addFunction(void (*function)(String, Answer*), char * name)
  {
    struct FuncItem* newItem = new struct FuncItem;
    newItem->function = function;
    newItem->name = name;
    newItem->prev = newItem->next = NULL;
    if( functionList == NULL)
    {
      functionList = newItem;
    }
    else
    {
      struct FuncItem* walker = functionList;
      while(walker->next != NULL)
      {
        walker = walker->next;
      }
      walker->next = newItem;
      newItem->prev = walker;
    }
  }

  void call(FuncItem* item, String string, Answer* answer)
  {
    item->function(string, answer);
  }

  void call(char* name, String string, Answer* answer)
  {
    struct FuncItem* walker = functionList;
    while(walker != NULL)
    {
      if(strcmp(walker->name, name) == 0)
      {
        Serial.print("Calling ");
        Serial.println(walker->name);
        walker->function(string, answer);
        break;
      }
      walker = walker->next;
    }
  }

  void callAll(String string, Answer* answer)
  {
    struct FuncItem* walker = functionList;
    while(walker != NULL)
    {
      Serial.print("Calling ");
      Serial.println(walker->name);
      walker->function(string, answer);
      walker = walker->next;
    }
  }

  String getNames()
  {
    String names = "{\"functions\":[";
    struct FuncItem* walker = functionList;
    while(walker != NULL)
    {
      names += "\"";
      names += walker->name;
      names += "\"";
      if(walker->next != NULL)
      {
        names += ",";
      }
      walker = walker->next;
    }
    names += "]}";
    return names;
  }

  private:
    struct FuncItem* functionList = NULL;
};
#endif
