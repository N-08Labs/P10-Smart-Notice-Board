#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

const char* ssid = "N-08 LABS "; 	//your wifi id
const char* password = "n08labs";	//your wifi password

const int nw = 2;



#define CHAT_ID1 "685XXXXXXXX"		//your Telegarm chat ID
#define BOTtoken "752XXXXXXXXXXXXXXXXXXXxXXXXXXXX"  //your bot token

#ifdef ESP8266
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif


WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID1) {
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    String text = bot.messages[i].text;
    Serial.println(text);
    String from_name = bot.messages[i].from_name;
    delay(1000);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(nw, OUTPUT);
  digitalWrite(nw, LOW);

#ifdef ESP8266
  configTime(0, 0, "pool.ntp.org");                   // get UTC time via NTP
  client.setTrustAnchors(&cert);                      // Add root certificate for api.telegram.org
#endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  delay(500);
  digitalWrite(nw, HIGH);
  delay(500);
  digitalWrite(nw, LOW);
  delay(500);
  digitalWrite(nw, HIGH);
  delay(500);
  digitalWrite(nw, LOW);
  delay(1000);
}

void loop() {
  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
