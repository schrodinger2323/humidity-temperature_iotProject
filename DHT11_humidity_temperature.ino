#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

#define RX 2
#define TX 3
#define DHTPIN 8
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial esp8266(RX, TX);

String ssid = "Medine";          // Wifi adı
String password = "Ali.1223";          // Wifi şifresi
String apiKey = "HDB4LRB9F1GBRO6I";   // ThingSpeak API anahtarı
String host = "api.thingspeak.com";
String port = "80";

int countTrueCommand;
int countTimeCommand; 
boolean found = false; 

void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);

  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Baglaniyor...");

  dht.begin();

  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"", 20, "OK");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Wifi Baglandi!");
  delay(2000);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Nem: ");
  lcd.print(h);
  lcd.print(" %");
  lcd.setCursor(0, 1);
  lcd.print("Sicaklik: ");
  lcd.print(t);
  lcd.print(" C");

  String getData = "GET /update?api_key=" + apiKey + "&field1=" + String(h) + "&field2=" + String(t);

  sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + host + "\"," + port, 15, "OK");
  sendCommand("AT+CIPSEND=0," + String(getData.length() + 4), 4, ">");
  esp8266.println(getData);
  delay(2000);
  sendCommand("AT+CIPCLOSE=0", 5, "OK");

  delay(20000); // 20 saniyede bir veri gönderimi
}

void sendCommand(String command, int maxTime, char readReply[]) {
  Serial.print(countTrueCommand);
  Serial.print(". AT komutu: ");
  Serial.println(command);

  while (countTimeCommand < (maxTime * 1)) {
    esp8266.println(command);
    if (esp8266.find(readReply)) {
      found = true;
      break;
    }
    countTimeCommand++;
  }

  if (found) {
    Serial.println("OK");
    countTrueCommand++;
  } else {
    Serial.println("Basarisiz");
    countTrueCommand = 0;
  }

  countTimeCommand = 0;
  found = false;
}

