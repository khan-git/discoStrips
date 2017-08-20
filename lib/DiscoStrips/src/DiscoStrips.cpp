#include "DiscoStrips.h"

DiscoStrips::DiscoStrips(SimpleVector<CRGB>* ledArray)
{
  tick_time = 500;
  current_pattern = 0;
  last_tick = millis();
  brightness = 75;
  rest_prefix = String("/disco");
  this->ledArray = ledArray;
  discoPatterns = new SimpleVector<char>(ledArray->rows(), NUM_PATTERNS);
  create_pattern();
  enabled = false;
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

void DiscoStrips::enable()
{
  enabled = true;
}

void DiscoStrips::disable()
{
  enabled = false;
}

void DiscoStrips::tick()
{
  unsigned long curr_tick = millis();
  if((curr_tick - last_tick) > tick_time)
  {
    last_tick = curr_tick;
    if(enabled)
    {
      shift_pattern();
    }
  }
}

/*
* REST API
* disco/
*   brightness/<num>
*/
void DiscoStrips::rest_parsing(String line, Answer* answer)
{
  String work_line = line.substring(line.indexOf(' ')+1);

  if(!line.startsWith("PUT") && !line.startsWith("GET"))
  {
    return;
  }

  // Verify it's for Disco
  if(work_line.startsWith(rest_prefix))
  {
    Serial.println(work_line);
    work_line.replace(rest_prefix, "");
    Serial.println(work_line);
    if(work_line.startsWith("/brightness"))
    {
      Serial.println(work_line);
      handleBrightness(line, work_line, answer);
    }
    else if(work_line.startsWith("/tempo"))
    {
      Serial.println(work_line);
      handleTempo(line, work_line, answer);
    }
    else{
      answer->add("{\"enabled\":");
      answer->add(enabled?"true":"false");
      answer->add(",\"cmds\":[\"brightness\",\"tempo\"]}");
      Serial.println(answer->getBuffer());
    }
  }
}

void DiscoStrips::handleBrightness(String line, String work_line, Answer *answer)
{
  work_line = work_line.substring(work_line.indexOf('/')+1);
  if(line.startsWith("PUT"))
  {
    String msg = "{\"set\":{\"cmd\":\"brightness\", \"value\":";
    brightness = work_line.toInt();
    msg += brightness;
    msg += "}}";
    answer->add(msg);
    return;
  }
  else if(line.startsWith("GET"))
  {
    Serial.println("--Sent brightness");
    String msg = "{\"brightness\":";
    msg += brightness;
    msg += "}";
    answer->add(msg);
    return;
  }
}

void DiscoStrips::handleTempo(String line, String work_line, Answer *answer)
{
  work_line = work_line.substring(work_line.indexOf('/')+1);
  work_line = work_line.substring(work_line.indexOf('/')+1);
  if(line.startsWith("PUT"))
  {
    String msg = "{\"set\":{\"cmd\":\"tempo\", \"value\":";
    Serial.print("---new tempo: ");
    Serial.println(work_line);
    tick_time = work_line.toInt();
    msg += tick_time;
    msg += "}}";
    answer->add(msg);
    return;
  }
  else if(line.startsWith("GET"))
  {
    Serial.println("--Sent tempo");
    String msg = "{\"tempo\":";
    msg += tick_time;
    msg += "}";
    answer->add(msg);
    return;
  }
}
