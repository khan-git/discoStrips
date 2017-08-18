#include "DuinoREST.h"

DuinoREST::DuinoREST()
{
  answer = new Answer(100);
  funcList = new FunctionList;
}

DuinoREST::~DuinoREST()
{
  delete[] answer;
  delete[] funcList;
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
          answer->clear();
          currentLine.replace("HTTP/1.1", "");
          if(currentLine.length() == 6)
          {
            answer->add(funcList->getNames());
          }
          else{
            funcList->callAll(currentLine, answer);
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
