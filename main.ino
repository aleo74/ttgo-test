#include "menu.h"
#include "chain.h"
#include "utils.h"
#include "AES.h"
#include "base64.h"


#include <HTTP_Method.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <Button2.h>

#include <TFT_eSPI.h>
#include <SPI.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#include"webservices.h"

#define ENC_TYPE_NONE true

#ifndef TFT_SLPIN
#define TFT_SLPIN   0x10
#endif

#define TFT_MOSI        19
#define TFT_SCLK        18
#define TFT_CS          5
#define TFT_DC          16
#define TFT_RST         23

#define TFT_BL          4  // Display backlight control pin
#define ADC_EN          14
#define ADC_PIN         34

#define BUTTON_1        35
#define BUTTON_2        0

TFT_eSPI tft = TFT_eSPI(135, 240);
Button2 btn1(BUTTON_1);
Button2 btn2(BUTTON_2);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
AES aes;

// supportet colors
#define ST7735_BLACK   0x0000
#define ST7735_GRAY    0x8410
#define ST7735_WHITE   0xFFFF
#define ST7735_RED     0xF800
#define ST7735_ORANGE  0xEC80
#define ST7735_YELLOW  0xFFE0  
#define ST7735_LIME    0x07FF
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_AQUA    0x04FF
#define ST7735_BLUE    0x001F
#define ST7735_MAGENTA 0xF81F
#define ST7735_PINK    0xF8FF
#define ST7735_CHYN    0x2D05

#define MENU_RAZ 5
#define ArrayByte 64
#define N_BLOCK 16
 
//Variables
int i = 0;

const char* serverName = "http://192.168.1.72:3001/oula";
byte byteArray[ArrayByte] = {0};

int btnCick = false;
int nbClick = 0;

HTTPClient http;    //Declare object of class HTTPClient

char buff[512];

byte key[] = { 0x2C, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6, 0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C };


void button_loop()
{
    btn1.loop();
    btn2.loop();
}

void show_ip_adress() {
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextSize(1);
  tft.drawString(WiFi.localIP().toString(), tft.width() / 2, tft.height() / 2);
}

void show_config() {
  String esid; // SSID
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(3);
  tft.drawString("Config", 2, 10);
  tft.setTextSize(2);
  tft.drawString(" SSID :", 2, 50);
  tft.drawString(esid, 2, 70);
  tft.drawString(" IP :", 2, 90);
  tft.setTextSize(1);
  tft.drawString(WiFi.localIP().toString(), 2, 105);
  tft.setTextSize(2);
  tft.drawString(" Nb serie :", 2, 120);
}

void test_block() {
  char b64data[200];
  byte cipher[1000];
  byte iv[N_BLOCK];
  byte my_iv[N_BLOCK] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  timeClient.update();
  
  Block myBlock(timeClient.getEpochTime());
  myBlock.generateIV(my_iv);
  Serial.println(base64_encode(b64data, (char *)my_iv, N_BLOCK));
  Serial.println(myBlock.getTimestamp());
  StaticJsonBuffer<300> JSONbuffer;   //Declaring static JSON buffer
  JsonObject& JSONencoder = JSONbuffer.createObject();

  JSONencoder["IV"] = String(b64data);
  JSONencoder["timestamp"] = myBlock.getTimestamp();

  JsonArray& values = JSONencoder.createNestedArray("values"); //JSON array
  values.add(20); //Add value to array
  values.add(21); //Add value to array
  values.add(23); //Add value to array

  /*JsonArray& timestamps = JSONencoder.createNestedArray("timestamps"); //JSON array
  timestamps.add("10:10"); //Add value to array
  timestamps.add("10:20"); //Add value to array
  timestamps.add("10:30"); //Add value to array*/

  char JSONmessageBuffer[500];
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));

  int b64len = base64_encode(b64data, (char *)JSONmessageBuffer,500);
  Serial.println (" Message in B64: " + String(b64data) );
  Serial.println (" The lenght is:  " + String(b64len) );
  aes.do_aes_encrypt((byte *)b64data, b64len , cipher, key, 128, my_iv);
  base64_encode(b64data, (char *)cipher, aes.get_size() );
  
  myBlock.setHash(String(b64data));
  myBlock.setData(String(b64data));
  //JsonArray& hash = JSONencoder.createNestedArray("hash");
  //hash.add(myBlock.getHash());

  

  http.begin("http://192.168.1.72:3001/oula");      //Specify request destination
  http.addHeader("Content-Type", "application/json");  //Specify content-type header

  int httpCode = http.POST(JSONmessageBuffer);   //Send the request
  String payload = http.getString();                                        //Get the response payload

  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  http.end(); 
}

void button_init()
{
    btn1.setPressedHandler([](Button2 & b) {
      Serial.println(btnCick);
        switch(nbClick) {
        case 1:
           show_ip_adress();
           break;
        case 2:
           test();
           break; 
        case 3:
          sleep();
          break;
        case 4:
          show_config();
        case 0:
          test_block();
          break;
        default:
          nbClick = 0;
          break;
        }
    
    
    });

    btn1.setLongClickHandler([](Button2 & b) {
      if (btnCick == MENU_RAZ) {
        Serial.println("coucou");
      }
      if(nbClick == 4) {
          tft.setTextColor(TFT_GREEN, TFT_BLACK);
          tft.fillScreen(TFT_BLACK);
          tft.setTextDatum(MC_DATUM);
          tft.setTextSize(2);
          tft.drawString("EraseEEPROM", 2, 10);
          tft.drawString("AR U SUR ?", 2, 50);
          btnCick = MENU_RAZ;
      }
    });
    

    btn2.setPressedHandler([](Button2 & b) {
        nbClick = Menu(nbClick);
        btnCick = false;
        if(nbClick > 4) {nbClick = 0;}
    });

    
}
 
void setup()
{
  if (TFT_BL > 0) { // TFT_BL has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
      pinMode(TFT_BL, OUTPUT); // Set backlight pin to output mode
      digitalWrite(TFT_BL, TFT_BACKLIGHT_ON); // Turn backlight on. TFT_BACKLIGHT_ON has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
  }
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(1);
  tft.drawString("loading...", tft.width() / 2, tft.height() / 2);
 
  Serial.begin(115200); //Initialising if(DEBUG)Serial Monitor
  EEPROM.begin(512); //Initialasing EEPROM
  delay(10);

  tft.init();
  button_init();
  // Chargement de la config wifi depuis l'EEPROM
  String esid; // SSID
  for (int i = 0; i < 32; ++i)
  {
    esid += char(EEPROM.read(i));
  }

  String epass = "";
  for (int i = 32; i < 96; ++i) // PASSWORD
  {
    epass += char(EEPROM.read(i));
  }
 
  WiFi.begin(esid.c_str(), epass.c_str());
  if (testWifi())
  {
    // Succesfully Connected!!!
    timeClient.begin();
    timeClient.update();
    Serial.println(timeClient.getEpochTime());
    return;
  }
  else
  {
    // Turning the HotSpot On
    launchWeb();
    setupAP();// Setup HotSpot
  }

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(1);
  Serial.println(WiFi.softAPIP());
  tft.drawString(WiFi.softAPIP().toString(), tft.width() / 2, tft.height() / 2);
  
  while ((WiFi.status() != WL_CONNECTED))
  {
    delay(100);
    server_handle();
  }

  Menu(1);
 
}
void loop() {
  if ((WiFi.status() == WL_CONNECTED))
  {
    if (btnCick) {
    }
    button_loop();
  }
  else
  {
  }
 
}
