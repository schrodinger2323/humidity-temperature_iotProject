#include <SoftwareSerial.h>

#define RX 2
#define TX 3

SoftwareSerial esp(RX, TX); // ESP8266 bağlantısı (TX->2, RX->3)

void setup() {
  Serial.begin(9600);       // Seri monitör için
  esp.begin(115200);          // 

  Serial.println(">>> ESP8266 Test Başladı <<<");

  sendCommand("AT", 5, "OK");  // Temel AT komutu
  sendCommand("AT+CWMODE=1", 5, "OK");  // İstasyon modu
  sendCommand("AT+CWJAP=\"Medine\",\"Ali.1223\"", 20, "OK"); // Wi-Fi bağlantısı

  sendCommand("AT+CIFSR", 5, "OK"); // IP adresi alınıyor

  Serial.println("İnternet bağlantısı deneniyor...");

  sendCommand("AT+CIPSTART=\"TCP\",\"httpbin.org\",80", 10, "OK"); // Bağlantı
  delay(1000);

  String httpRequest = 
    "GET /get HTTP/1.1\r\n"
    "Host: httpbin.org\r\n"
    "Connection: close\r\n\r\n";

  sendCommand("AT+CIPSEND=" + String(httpRequest.length()), 5, ">"); // Veri gönderimi başlat
  esp.print(httpRequest); // HTTP isteği gönder
  printResponse();        // Cevapları yazdır

  delay(500);
  sendCommand("AT+CIPCLOSE", 5, "OK"); // Bağlantıyı kapat
}

void loop() {
  // Loop'ta test yok. Sadece bir kere çalışması yeterli.
}

void sendCommand(String cmd, int timeout, String expected) {
  Serial.print("Komut: ");
  Serial.println(cmd);
  esp.println(cmd);

  long int time = millis();
  while ((millis() - time) < timeout * 1000) {
    if (esp.find(const_cast<char*>(expected.c_str()))) {
      Serial.println(">> Başarılı ✅");
      return;
    }
  }
  Serial.println(">> Başarısız ❌");
}

void printResponse() {
  long int time = millis();
  Serial.println(">>> Sunucu cevabı:");
  while ((millis() - time) < 5000) {
    while (esp.available()) {
      char c = esp.read();
      if (isPrintable(c)) {
        Serial.print(c);
      }
    }
  }
  Serial.println("\n>>> Yanıt sonu <<<");
}



