#include "DuinoREST.h"

DuinoREST::DuinoREST(int cmdLevels, int cmdLength)
{
  answer = new Answer(100);
  funcList = new FunctionList;
  commands = new SimpleVector<char>(cmdLevels, cmdLength);
}

DuinoREST::~DuinoREST()
{
  delete[] answer;
  delete[] funcList;
  delete[] commands;
}

void DuinoREST::clearCommands()
{
  for(int i = 0; i < commands->rows(); i++)
  {
    commands->set(i, 0, '\0');
  }
}

void DuinoREST::splitCommands(String line)
{
  if(line.startsWith("PUT") || line.startsWith("GET"))
  {
    int index = 0;
    int next = 0;
    int cmdIndex = 0;
    if(line.endsWith("HTTP/1.1"))
    {
      line.replace(" HTTP/1.1", "");
    }
    line.toCharArray(commands->get(cmdIndex), 4, 0);
    Serial.println(commands->get(cmdIndex));
    do
    {
      index = line.indexOf('/', next);
      if(index == -1)
      {
        break;
      }
      next = line.indexOf('/', index+1);
      if(next == -1)
      {
        next = line.length();
      }
      Serial.print(index);
      Serial.print("-");
      Serial.print(next);
      line.toCharArray(commands->get(++cmdIndex), next-index, index+1);
      Serial.println(commands->get(cmdIndex));
    } while(1);
  }
}

void DuinoREST::handle(WiFiClient& client)
{
  String currentLine = "";
  while (client.connected()) {
    if (client.available()) {             // if there's bytes to read from the client,
      char c = client.read();             // read a byte, then
      if (c == '\n')                     // if the byte is a newline character
      {
        // if(currentLine)
        Serial.println(currentLine);
        if(currentLine.startsWith("GET ") || currentLine.startsWith("PUT "))
        {
          splitCommands(currentLine);
          answer->clear();
          currentLine.replace("HTTP/1.1", "");
          if(currentLine.length() == 6)
          {
            answer->add(funcList->getNames());
          }
          else{
            // funcList->callAll(currentLine, answer);
            funcList->call(commands->get(1), currentLine, answer);
          }
          client.println("HTTP/1.1 200 OK");
          client.println("Content-type: application/json; charset=utf-8");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          // client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          if(answer->currentLength() > 0)
          {
            client.print(answer->getBuffer());
          }
          client.println();
        }

        // if the current line is blank, you got two newline characters in a row.
        // that's the end of the client HTTP request, so send a response:
        if (currentLine.length() == 0)
        {
          // break out of the while loop:
          break;
        }
        else {      // if you got a newline, then clear currentLine:
          currentLine = "";
        }
      }
      else if (c != '\r')
      {    // if you got anything else but a carriage return character,
        currentLine += c;      // add it to the end of the currentLine
      }
    }
  }
}

void DuinoREST::addHandler(char *name, void (*f)(String, Answer*))
{
  funcList->addFunction(f, name);
}
