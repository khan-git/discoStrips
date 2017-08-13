#include "DiscoStrips.h"

DiscoStrips::DiscoStrips(SimpleVector<CRGB>* ledArray)
{
  tick_time = 500;
  current_pattern = 0;
  last_tick = millis();
  brightness = 75;
  rest_prefix = String("/disco/");
  this->ledArray = ledArray;
  discoPatterns = new SimpleVector<char>(ledArray->rows(), NUM_PATTERNS);
  create_pattern();
}

DiscoStrips::~DiscoStrips()
{
  delete[] discoPatterns;
}

void DiscoStrips::create_pattern()
{
  char tmp = 0, next = 0;
  for(int i=0; i < ledArray->rows(); i++)
  {
    for(int j=0; j < NUM_PATTERNS; j++)
    {
      do {
        next = (char)random(0, NUM_COLORS);
      } while(next == tmp);
      *discoPatterns->get(i, j) = next;
    }
  }
}

void DiscoStrips::shift_pattern()
{
  current_pattern = current_pattern < NUM_PATTERNS? current_pattern+1: 0;
  for(int i=0; i < ledArray->rows(); i++)
  {
    for(int j=0; j < ledArray->cols(); j++)
    {
      *ledArray->get(i, j) = discoPallets[*discoPatterns->get(i, current_pattern)];
      *ledArray->get(i, j) %= brightness;
    }
  }
}

void DiscoStrips::tick()
{
  unsigned long curr_tick = millis();
  if((curr_tick - last_tick) > tick_time)
  {
    last_tick = curr_tick;
    shift_pattern();
  }
}

/*
* REST API
* disco/
*   brightness/<num>
*/
String DiscoStrips::rest_parsing(String line)
{
  String work_line = line.substring(line.indexOf(' ')+1);

  if(!line.startsWith("PUT") && !line.startsWith("GET"))
  {
    return "";
  }

  // Verify it's for Disco
  if(work_line.startsWith(rest_prefix))
  {
    work_line = work_line.substring(rest_prefix.length());
    if(work_line.startsWith("brightness"))
    {
      work_line = work_line.substring(work_line.indexOf('/')+1);
      if(line.startsWith("PUT"))
      {
        brightness = work_line.toInt();
        Serial.print("--Brightness PUT: ");
        Serial.println(brightness);
        return "";
      }
      else if(line.startsWith("GET"))
      {
        Serial.println("--Sent brightness");
        return String(brightness);
      }
    }
  }
}
