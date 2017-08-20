#include <Arduino.h>
#include "DiscoStrips.h"
#include "FastLED.h"
#include <SPI.h>
#include <WiFi101.h>
#include "Answer.h"
#include "DuinoREST.h"
#include "Modes.h"

using namespace std;

DiscoStrips* disco = NULL;
DuinoREST* rest = NULL;

#define NUM_STRIPS 1
#define NUM_LEDS 10

SimpleVector<CRGB>* ledArray;
int currentMode = 0;
int brightness = 25;
int colour = 4;

char ssid[] = "Resistance Is Futile 2"; // created AP name
char pass[] = "liss#dfe1AWdfew";      // AP password (needed only for WEP, must be exactly 10 or 26 characters in length)
int keyIndex = 0;                // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

// We loop to add the line for each strip
template< unsigned N > struct AddLeds{
  static void generate() __attribute__((always_inline)){
    FastLED.addLeds<NEOPIXEL, N-1>(ledArray->get(N-1), NUM_LEDS);
    AddLeds< N - 1 >::generate();
  }
};
template<> struct AddLeds<0>{ static inline void generate(){} };

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}

void discoFunc(String line, Answer* answer)
{
  disco->rest_parsing(line, answer);
}

void brightnessFunc(String line, Answer* answer)
{
  if(line.startsWith("PUT /brightness/"))
  {
    brightness = line.substring(16).toInt();
  }
  answer->add("{\"brightness\":");
  answer->add(String(brightness));
  answer->add("}");
}

String colours[] = {"red", "green", "yellow", "blue", "white"};
CRGB crgbs[] = {CRGB::Red, CRGB::DarkGreen, CRGB::Yellow, CRGB::Blue, CRGB::White};

void colourFunc(String line, Answer* answer)
{
  if(line.startsWith("PUT /colour/"))
  {
    String colourReq = line.substring(12);
    for(int i=0; i < 5; i++)
    {
      if(colourReq.startsWith(colours[i]))
      {
        colour = i;
        break;
      }
    }
  }
  answer->add("{\"colour\":");
  answer->add(colours[colour]);
  answer->add(",\"options\":[");
  for(int i=0; i < 5; i++)
  {
    answer->add("\"");
    answer->add(colours[i]);
    answer->add("\"");
    if(i < 4)
    {
      answer->add(", ");
    }
  }
  answer->add("]}");
}

void modesFunc(String line, Answer* answer)
{
  if(line.startsWith("GET /mode"))
  {
    answer->add("{\"modes\":[\"off\", \"on\", \"night\", \"fisco\"]}");
  }
  else if(line.startsWith("PUT /mode/on"))
  {
    currentMode = 1;
    answer->add("{\"mode\":\"on\"}");
  }
  else if(line.startsWith("PUT /mode/night"))
  {
    currentMode = 2;
    answer->add("{\"mode\":\"Night\"}");
  }
  else if(line.startsWith("PUT /mode/disco"))
  {
    currentMode = 3;
    answer->add("{\"mode\":\"disco\"}");
  }
  else
  {
    currentMode = 0;
    answer->add("{\"mode\":\"off\"}");
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Serial.println("Setup");
  delay(1000);
  Serial.println("Setup");
  ledArray = new SimpleVector<CRGB>(NUM_STRIPS, NUM_LEDS);
  AddLeds<NUM_STRIPS>::generate();

  disco = new DiscoStrips(ledArray);
  rest = new DuinoREST;

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // by default the local IP address of will be 192.168.1.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));

  // Create open network. Change this line if you want to create an WEP network:
  // status = WiFi.beginAP(ssid);
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
//    delay(1000);
  }

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();

  rest->addHandler("disco", discoFunc);
  rest->addHandler("mode", modesFunc);
  rest->addHandler("brightness", brightnessFunc);
  rest->addHandler("colour", colourFunc);
}

void allLedsOff()
{
  for(int i=0; i < ledArray->rows(); i++)
  {
    for(int j=0; j < ledArray->cols(); j++)
    {
      *ledArray->get(i, j) = CRGB::Black;
    }
  }

}

void allLedsOn()
{
  for(int i=0; i < ledArray->rows(); i++)
  {
    for(int j=0; j < ledArray->cols(); j++)
    {
      *ledArray->get(i, j) = crgbs[colour];
      *ledArray->get(i, j) %= brightness;
    }
  }
}

void loop()
{
  // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      byte remoteMac[6];

      // a device has connected to the AP
      Serial.print("Device connected to AP, MAC address: ");
      WiFi.APClientMacAddress(remoteMac);
      Serial.print(remoteMac[5], HEX);
      Serial.print(":");
      Serial.print(remoteMac[4], HEX);
      Serial.print(":");
      Serial.print(remoteMac[3], HEX);
      Serial.print(":");
      Serial.print(remoteMac[2], HEX);
      Serial.print(":");
      Serial.print(remoteMac[1], HEX);
      Serial.print(":");
      Serial.println(remoteMac[0], HEX);
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    rest->handle(client);
    // close the connection:
    delay(1);
    client.stop();
  }

  switch(currentMode)
  {
    case 3:
      disco->enable();
      break;

    case 2:
      disco->disable();
      break;

    case 1:
      disco->disable();
      allLedsOn();
      break;

    case 0:
    default:
      disco->disable();
      allLedsOff();
          break;
  }

  disco->tick();
  FastLED.show();
}
