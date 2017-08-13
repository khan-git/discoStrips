#include <Arduino.h>
#include "DiscoStrips.h"
#include "FastLED.h"
#include <SPI.h>
#include <WiFi101.h>
#include "Answer.h"

using namespace std;

DiscoStrips* disco = NULL;
#define NUM_STRIPS 1
#define NUM_LEDS 10

SimpleVector<CRGB>* ledArray;

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

void printIndex(WiFiClient &client) {
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();

  // the content of the HTTP response follows the header:
  client.print("<html>\n");
  client.print("<head><script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js\"></script></head>");
  client.print("<body>\n");
  client.print("<script type=\"text/javascript\">"
  "function postBrightness(){"
    "console.info('Hello');"
    "$.post('/brightness', {'values':'43'});"
  "}"
  "</script>");
  client.print("<form action=\"brightness\" method=\"post\">\n");
  client.print("<input id=\"brightness\" type=\"range\" value=\"45\" onchange=\"postBrightness()\">\n");
  client.print("</form>\n");
  // client.print(bluePushed);
  client.print("</body></html>");
  // client.print("Click <a href=\"/L\">here</a> turn the LED off<br>");

  // The HTTP response ends with another blank line:
  client.println();
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Setup");
  ledArray = new SimpleVector<CRGB>(NUM_STRIPS, NUM_LEDS);
  AddLeds<NUM_STRIPS>::generate();

  disco = new DiscoStrips(ledArray);
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
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        if (c == '\n')                     // if the byte is a newline character
        {
          Serial.println(currentLine);
          String response = disco->rest_parsing(currentLine);
          if(response.length() > 0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/text");
            client.println();
            client.println(response);
          }

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0)
          {
            // printIndex(client);
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
        else
        {
          // Serial.println(currentLine);
          // disco->rest_parsing(currentLine);
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }

  disco->tick();
  FastLED.show();
}
