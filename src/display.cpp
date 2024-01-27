/*
 Adapted from the Adafruit graphicstest sketch, see original header at end
 of sketch.

 This sketch uses the GLCD font (font 1) only.

 Make sure all the display driver and pin connections are correct by
 editing the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
*/


#include "SPI.h"
#include "TFT_eSPI.h"
#include <string>
#include <iostream>
#include <map>

using namespace std;

TFT_eSPI tft = TFT_eSPI();

String from_id = "";
String chat_id = "";
String text = "";
String text_colour = "";
String colour_to_display = "";

std::map<String, String>id_to_colour;

unsigned long total = 0;
unsigned long tn = 0;

unsigned long testText();
void handleNewMessages(int numNewMessages);
void changeColours(String colour_to_display);

///////////////////////////////////////////////////
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <Arduino.h>

// Wifi network station credentials
#define WIFI_SSID "aaaaaa"
#define WIFI_PASSWORD "12345688"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "6838301250:AAETjWXjm260AU4RCIJZyNB8Lg-aAtspq1w"

const unsigned long BOT_MTBS = 1000; // mean time between scan messages

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime;          // last time messages' scan has been done

///////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println(""); Serial.println("");
  Serial.println("TFT_eSPI library test!");

  tft.init();

  tn = micros();
  tft.fillScreen(TFT_BLACK);


// attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    Serial.print(WiFi.status());
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
}

void loop(void) {
//   for (uint8_t rotation = 0; rotation < 4; rotation++) {
//     tft.setRotation(rotation);
//     testText();
//     delay(2000);
//   }
  tft.setRotation(3);
  testText();
  // so that the display wouldnt update & flicker every time
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  for (numNewMessages = 0; numNewMessages == 0;){
    delay(500);
    if (millis() - bot_lasttime > BOT_MTBS)
    {
        int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

        while (numNewMessages)
        {
        Serial.println("got response");
        // Serial.println(String(bot.messages[0].text)); <-- how to read the message sent
        handleNewMessages(numNewMessages);
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }

        bot_lasttime = millis();
    }
  }
}


unsigned long testText() {
  tft.fillScreen(TFT_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
//   tft.setTextColor(TFT_WHITE);  tft.setTextSize(1);
//   tft.println("Hello World!");
//   tft.setTextColor(TFT_PURPLE); tft.setTextSize(2);
//   tft.println(1234.56);
  tft.setTextColor(TFT_RED);    tft.setTextSize(3);
//   tft.println(0xDEADBEEF, HEX);
  tft.println("hmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm");
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  //tft.setTextColor(TFT_GREEN,TFT_BLACK);
  tft.setTextSize(1);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}

unsigned long updateText(String text_to_update) {
  tft.fillScreen(TFT_BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextSize(3);
  tft.println(text_to_update);
  return micros() - start;
}


void handleNewMessages(int numNewMessages)
{
  for (int i = 0; i < numNewMessages; i++)
  {
    from_id = bot.messages[0].from_id;
    chat_id = bot.messages[0].chat_id;
    text = bot.messages[0].text;

    if (text == "/colours")
    {
    //   String keyboardJson = "[[{ \"text\" : \"Black\", \"callback_data\" : \"TFT_BLACK\" }], [{ \"text\" : \"Navy\", \"callback_data\" : \"TFT_NAVY\" }], [{ \"text\" : \"Dark Green\", \"callback_data\" : \"TFT_DARKGREEN\" }], [{ \"text\" : \"Dark Cyan\", \"callback_data\" : \"TFT_DARKCYAN\" }], [{ \"text\" : \"Maroon\", \"callback_data\" : \"TFT_MAROON\" }], [{ \"text\" : \"Purple\", \"callback_data\" : \"TFT_PURPLE\" }], [{ \"text\" : \"Olive\", \"callback_data\" : \"TFT_OLIVE\" }], [{ \"text\" : \"Light Grey\", \"callback_data\" : \"TFT_LIGHTGREY\" }], [{ \"text\" : \"Dark Grey\", \"callback_data\" : \"TFT_DARKGREY\" }], [{ \"text\" : \"Blue\", \"callback_data\" : \"TFT_BLUE\" }], [{ \"text\" : \"Green\", \"callback_data\" : \"TFT_GREEN\" }], [{ \"text\" : \"Cyan\", \"callback_data\" : \"TFT_CYAN\" }], [{ \"text\" : \"Red\", \"callback_data\" : \"TFT_RED\" }], [{ \"text\" : \"Magenta\", \"callback_data\" : \"TFT_MAGENTA\" }], [{ \"text\" : \"Yellow\", \"callback_data\" : \"TFT_YELLOW\" }], [{ \"text\" : \"White\", \"callback_data\" : \"TFT_WHITE\" }], [{ \"text\" : \"Orange\", \"callback_data\" : \"TFT_ORANGE\" }], [{ \"text\" : \"Green Yellow\", \"callback_data\" : \"TFT_GREENYELLOW\" }], [{ \"text\" : \"Pink\", \"callback_data\" : \"TFT_PINK\" }], [{ \"text\" : \"Brown\", \"callback_data\" : \"TFT_BROWN\" }], [{ \"text\" : \"Gold\", \"callback_data\" : \"TFT_GOLD\" }], [{ \"text\" : \"Silver\", \"callback_data\" : \"TFT_SILVER\" }], [{ \"text\" : \"Sky Blue\", \"callback_data\" : \"TFT_SKYBLUE\" }], [{ \"text\" : \"Violet\", \"callback_data\" : \"TFT_VIOLET\" }]]";
         String keyboardJson = "[[{ \"text\" : \"Purple\", \"callback_data\" : \"TFT_PURPLE\" }], [{ \"text\" : \"Olive\", \"callback_data\" : \"TFT_OLIVE\" }], [{ \"text\" : \"Light Grey\", \"callback_data\" : \"TFT_LIGHTGREY\" }], [{ \"text\" : \"Blue\", \"callback_data\" : \"TFT_BLUE\" }], [{ \"text\" : \"Green\", \"callback_data\" : \"TFT_GREEN\" }], [{ \"text\" : \"Cyan\", \"callback_data\" : \"TFT_CYAN\" }], [{ \"text\" : \"Red\", \"callback_data\" : \"TFT_RED\" }], [{ \"text\" : \"Magenta\", \"callback_data\" : \"TFT_MAGENTA\" }], [{ \"text\" : \"Yellow\", \"callback_data\" : \"TFT_YELLOW\" }], [{ \"text\" : \"White\", \"callback_data\" : \"TFT_WHITE\" }], [{ \"text\" : \"Orange\", \"callback_data\" : \"TFT_ORANGE\" }], [{ \"text\" : \"Pink\", \"callback_data\" : \"TFT_PINK\" }], [{ \"text\" : \"Brown\", \"callback_data\" : \"TFT_BROWN\" }], [{ \"text\" : \"Gold\", \"callback_data\" : \"TFT_GOLD\" }], [{ \"text\" : \"Silver\", \"callback_data\" : \"TFT_SILVER\" }], [{ \"text\" : \"Sky Blue\", \"callback_data\" : \"TFT_SKYBLUE\" }], [{ \"text\" : \"Violet\", \"callback_data\" : \"TFT_VIOLET\" }]]";

      bot.sendMessageWithInlineKeyboard(chat_id, "Choose a colour from one of the following options", "", keyboardJson);
    }
    if (bot.messages[i].type == "callback_query")
        {
        id_to_colour[from_id] = text;
        }
    else
    {
      bot.sendMessage(bot.messages[i].chat_id, "Displaying: " + bot.messages[i].text, "");
    }
    colour_to_display = id_to_colour[from_id];
    changeColours(colour_to_display);
    updateText(text);
    Serial.println(from_id);

  }
}

void changeColours(String colour_to_display){
    if (colour_to_display == "TFT_BLACK"){tft.setTextColor(TFT_BLACK);}
    else if (colour_to_display == "TFT_NAVY"){tft.setTextColor(TFT_NAVY);}
    else if (colour_to_display == "TFT_DARKGREEN"){tft.setTextColor(TFT_DARKGREEN);}
    else if (colour_to_display == "TFT_DARKCYAN"){tft.setTextColor(TFT_DARKCYAN);} 
    else if (colour_to_display == "TFT_MAROON"){tft.setTextColor(TFT_MAROON);}
    else if (colour_to_display == "TFT_OLIVE"){tft.setTextColor(TFT_OLIVE);}
    else if (colour_to_display == "TFT_LIGHTGREY"){tft.setTextColor(TFT_LIGHTGREY);}
    else if (colour_to_display == "TFT_DARKGREY"){tft.setTextColor(TFT_DARKGREY);}
    else if (colour_to_display == "TFT_BLUE"){tft.setTextColor(TFT_BLUE);}
    else if (colour_to_display == "TFT_GREEN"){tft.setTextColor(TFT_GREEN);}
    else if (colour_to_display == "TFT_CYAN"){tft.setTextColor(TFT_CYAN);}
    else if (colour_to_display == "TFT_RED"){tft.setTextColor(TFT_RED);}
    else if (colour_to_display == "TFT_MAGENTA"){tft.setTextColor(TFT_MAGENTA);}
    else if (colour_to_display == "TFT_YELLOW"){tft.setTextColor(TFT_YELLOW);}
    else if (colour_to_display == "TFT_WHITE"){tft.setTextColor(TFT_WHITE);}
    else if (colour_to_display == "TFT_ORANGE"){tft.setTextColor(TFT_ORANGE);}
    else if (colour_to_display == "TFT_GREENYELLOW"){tft.setTextColor(TFT_GREENYELLOW);}
    else if (colour_to_display == "TFT_PINK"){tft.setTextColor(TFT_PINK);}
    else if (colour_to_display == "TFT_BROWN"){tft.setTextColor(TFT_BROWN);}
    else if (colour_to_display == "TFT_GOLD"){tft.setTextColor(TFT_GOLD);}
    else if (colour_to_display == "TFT_SILVER"){tft.setTextColor(TFT_SILVER);}
    else if (colour_to_display == "TFT_SKYBLUE"){tft.setTextColor(TFT_SKYBLUE);}
    else if (colour_to_display == "TFT_VIOLET"){tft.setTextColor(TFT_VIOLET);}
}