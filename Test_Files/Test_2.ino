#include <SoftwareSerial.h>

#define RX 2
#define TX 3
SoftwareSerial esp(RX, TX);

// Wi-Fi bilgileri
String ssid = "WiFi_ADIN";           // Wi-Fi adını buraya yaz
String password = "WiFi_SIFRE";      // Wi-Fi şifresini buraya yaz

// Thingspeak bilgileri
String host = "api.thingspeak.com";
String apiKey = "HDB4LRB9F1GBRO6I";  // Senin Thingspeak write API key'in
String port = "80";

void setup() {
  Serial.begin(9600);
  esp.begin(115200);

  Serial.println(">>> ESP8266 Thingspeak Test Başladı <<<");

  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");

  Serial.println("Wi-Fi'ya bağlanılıyor...");
  sendCommand("AT+CWJAP=\"Medine\",\"Ali.1223\"", 10, "OK");

  sendCommand("AT+CIFSR", 5, "OK");

  Serial.println("Thingspeak sunucusuna bağlanılıyor...");
  sendCommand("AT+CIPSTART=\"TCP\",\"" + host + "\"," + port, 10, "OK");

  // Rastgele veri üret
  int sicaklik = random(20, 30);
  int nem = random(40, 60);
  String getData = "GET /update?api_key=" + apiKey + "&field1=" + String(nem) + "&field2=" + String(sicaklik);

  Serial.println("Veri gönderiliyor:");
  Serial.println(getData);

  sendCommand("AT+CIPSEND=" + String(getData.length() + 4), 5, ">");

  esp.println(getData);
  delay(1000);

  Serial.println(">>> Gönderim tamamlandı. Yanıt bekleniyor...");
  delay(3000); // Yanıt bekleme

  Serial.println("Bağlantı kapatılıyor...");
  sendCommand("AT+CIPCLOSE", 5, "OK");
}

void loop() {
  // Tek seferlik test olduğu için loop boş.
}

void sendCommand(String command, int timeout, String expected) {
  Serial.println("Komut: " + command);
  esp.println(command);

  long int time = millis();
  String response = "";

  while ((time + timeout * 1000) > millis()) {
    while (esp.available()) {
      char c = esp.read();
      response += c;
    }
    if (response.indexOf(expected) != -1) {
      Serial.println(">> Başarılı ✅");
      return;
    }
  }

  Serial.println(">> Başarısız ❌");
  Serial.println("Cevap: " + response);
}

