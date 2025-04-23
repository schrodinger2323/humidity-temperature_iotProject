# 🌡️ DHT11 ile Sıcaklık & Nem Takibi ve ThingSpeak'e Veri Gönderimi

Bu projede, DHT11 sıcaklık ve nem sensörü kullanılarak elde edilen veriler, bir I2C LCD ekran üzerinde görüntülenmekte ve aynı zamanda ESP8266 WiFi modülü ile internet üzerinden ThingSpeak sunucusuna gönderilmektedir. Gönderilen veriler, ThingSpeak arayüzü üzerinden grafiksel olarak izlenebilmektedir. 

## 🔧 Kullanılan Devre Elemanları

-Arduino UNO

-DHT11 Nem ve Sıcaklık Sensörü

-I2C LCD (16x2)

-ESP8266 WiFi Modülü

-Bağlantı kabloları

## ⚙️ Bağlantılar

-DHT11-> S (signal) arduinonun 8 nolu dijital pini, + (Vcc) 5V'a, -(GND) gndye baglanir

-ESP8266-01 Wi-Fi Module 3V3, EN-> arduino 3.3V, TX->2, RX-> 3, GND->GND, diger pinleri boşta

-I2C modüllü LCD GND->GND, VCC-> 5V, SDA-> A4, SCL-> A5

## 📡 ThingSpeak Yapılandırması ve API anahtarı alma
1. https://thingspeak.com/ adresine gidin ve hesabınız yoksa bir hesap oluşturun. Hesabınıza giriş yapın.
2. "Channels" menüsünde "New Channel" tıklanır.
3. Yeni kanalı oluşturmak için ekranda beliren formda kanalın adı "Nem&Sicaklik" girilir.
4. field1 ve field2 alanlarına "nem" ve "sıcaklık" yazılır. her iki alan için onay kutuları işaretlenir.
5. sayfanın altında Save Channel basılarak kaydedilir. 
6. API Keys sekmesine gidilerek "Write API key" altındaki API numarası kaydedilir. bu daha sonra kod içerisinde char *api_anahtar = "BURAYA_API_KEY"; satırına yazılır.


## 🧰 Kullanılan Kütüphaneler

- #include <DHT.h>  
- #include <Wire.h  
- #include <LiquidCrystal_I2C.h>  
- #include <Timer.h>  
- #include <SoftwareSerial.h>

DHT.h -> https://github.com/adidax/dht11

LiquidCrystal_I2C.h -> https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library

Timer.h-> https://github.com/JChristensen/Timer/tree/master

SoftwareSerial.h-> arduino içinde hazırda var.

yukarıda sıralanan kütüphaneler ilgili linklerden indirilip kopyalanıp arduino klasörünün altındaki library klasörüne yapıştırılması gerekir. veya arduino ide üzerinden sketch>include library> add .zip library... ile de dahil edilebilir. 

## Global değişken tanımlamaları

**LiquidCrystal_I2C lcd(0x27, 16, 2);** lcd: I2C protokolüyle haberleşen 16 sütun 2 satır LCD ekranı tanımlar. 0x27, LCD'nin I2C adresidir. Ekran üzerinden sıcaklık ve nem verileri gösterilecek.

**SoftwareSerial Serial1(2, 3);** Serial1: ESP8266 ile haberleşmek için yazılımsal seri port. Arduino'nun dijital pin 2'si TX, 3'ü RX olarak ayarlanmıştır.

**#define DHT11pin 8**                 
**#define DHTTYPE DHT11**               
**DHT dht(DHT11pin, DHTTYPE);**

DHT11 sensörünün S (Signal) pininin bağlı olduğu arduino pin numarası (8), Kullanılan sensör tipi (DHT11), Sıcaklık ve nem ölçümlerini gerçekleştirecek nesne tanımlanmıştır. dht.readTemperature() ve dht.readHumidity() ile veri okunur.

**char *api_anahtar = "HDB4L********";** api_anahtar: Verilerin gönderileceği ThingSpeak kanalının özel anahtarı. Kanalın güvenli şekilde veri almasını sağlar. kanal oluştururken kaydettiğimiz api key numarası buraya yapıştırılır. 

**Timer t;**  t: Belirli aralıklarla (örneğin 20 saniyede bir) işlem yapılmasını sağlayan zamanlayıcı nesnesi. Donanımsal değil, yazılımsal zamanlama kullanılır.

**int nem, sicaklik;** nem, sicaklik: DHT11 sensöründen okunan sıcaklık ve nem değerlerini saklamak için tanımlanmış global değişkenler.

- **void httpGet(String ip, String path, int port = 80);**      ESP8266 üzerinden ThingSpeak'e HTTP GET isteği gönderir. 
- **void send2server();**                                       Ölçülen sıcaklık ve nem değerlerini URL’ye yerleştirip httpGet() fonksiyonunu çağırır.
- **void connect_wifi(String cmd, int t);**                     ESP8266'ya AT komutları göndererek modül ile iletişim kurulmasını sağlar.

## 🚀 setup() Fonksiyonu
  ```
  lcd.begin();           // LCD başlatılır (I2C üzerinden haberleşme)
  lcd.backlight();       // LCD'nin arka ışığı açılır
  lcd.clear();           // LCD ekran temizlenir
  lcd.print(" Nem&Sicaklik");
  lcd.setCursor(0, 1);   // İkinci satırın başına geç
  lcd.print(" Uygulamasi ");
  delay(4000);           // 4 saniye bekleme (bilgi ekranı gösterimi)
  ```
  ```
  lcd.clear();
  lcd.print("DHT11&Thingspeak");
  lcd.setCursor(0, 1);
  lcd.print(" NIT ");    // Projeye ait ikinci karşılama mesajı
  delay(4000);           // 4 saniye bekleme
  ```
  ```
  - Serial1.begin(115200);   // ESP8266 ile yazılımsal seri port üzerinden iletişim başlatılır
  - Serial.begin(115200);    // Arduino'nun ana seri portu (USB üzerinden bilgisayar bağlantısı)
  ```
  ```
  lcd.clear();
  lcd.print("Wifi baglaniyor");
  lcd.setCursor(0, 1);
  lcd.print("Lutfen bekleyin....");
  Serial.println("Wifi baglaniyor....")
  ```
  ```
  connect_wifi("AT", 1000);                         // ESP8266’ya temel AT komutuyla cevap veriyor mu testi
  connect_wifi("AT+CWMODE=1", 1000);                // ESP’yi WiFi istemci (station) moduna alır
  connect_wifi("AT+CWQAP", 1000);                   // Mevcut WiFi bağlantısını koparır
  connect_wifi("AT+RST", 8000);                     // ESP8266’yı resetler
  connect_wifi("AT+CWJAP=\"WIFI_ADI\",\"WIFI_SIFRESI\"", 10000); // Belirtilen WiFi ağına bağlanır
  ```
  ```
  Serial.println("Wifi bağlandı.");
  lcd.clear();
  lcd.print("Wifi baglandi.");                      // Ekrana bağlantı durumu yazılır
  ```
  ```
  delay(2000);                 // Kısa bekleme
  ```
  ```
  **t.every(20000, send2server);**       // Her 20 saniyede bir sunucuya veri gönderme zamanlaması
  ```
## 🔁 loop() Fonksiyonu Açıklaması
   ```
  dht.begin();
  ```
  DHT11 sensörü her döngüde başlatılıyor (opsiyonel, sadece ilk kez setup() içinde yapılması genellikle yeterlidir ama burada güvenlik için tekrar çağrılmış).
  
   ```
  lcd.setCursor(0, 0);
  lcd.print("Nem: ");
  ```
  LCD'nin birinci satırının başına gidilir ve "Nem: " yazısı yazılır.
  
  ```
  nem = dht.readHumidity();
  lcd.print(nem);
  lcd.print(" %  ");
  ```
  DHT11 sensöründen nem verisi okunur, LCD’ye yüzde formatında yazdırılır.

  ```
  lcd.setCursor(0, 1);
  lcd.print("Sicaklik: ");
  sicaklik = dht.readTemperature();
  lcd.print(sicaklik);
  lcd.print(" C ");
  ```
İkinci satıra geçilir. Sıcaklık değeri okunur ve derece formatında ekrana yazılır.

```
delay(1000); Ekran yanıp sönmesin diye 1 saniye beklenir.\
t.update(); // Zamanlayıcıları çalıştır. Daha önce ayarlanan zamanlayıcılar (send2server() gibi) burada çalıştırılır. Bu sayede belirli aralıklarla ThingSpeak’e veri gönderilir.
```
## ☁️ send2server() Fonksiyonu Açıklaması
```
void send2server() 
char sicaklikStr[8];
char nemStr[8];
```
Sıcaklık ve nem değerlerini metin formatına (char dizisi) çevirmek için gerekli değişkenler tanımlanır.

```
dtostrf(sicaklik, 5, 3, sicaklikStr);
dtostrf(nem, 5, 3, nemStr);
```
Float tipindeki sıcaklık ve nem değerleri string’e çevrilir. Bu işlem, veriyi HTTP adresine ekleyebilmek için gerekir.

```
char postUrl[150];
sprintf(postUrl, "update?api_key=%s&field1=%s&field2=%s", api_anahtar, nemStr, sicaklikStr);
```
ThingSpeak'e gönderilecek olan URL hazırlanır. API anahtarı ve sensör verileri bu linke gömülür.

```
httpGet("184.106.153.149", postUrl, 80); // ThingSpeak IP adresi
```
Hazırlanan URL ile HTTP GET isteği oluşturularak veri gönderilir.

## 🌐 httpGet() Fonksiyonu Açıklaması
```
void httpGet(String ip, String path, int port) {
String atHttpGetCmd = "GET /" + path + " HTTP/1.0\r\n\r\n";
```
GET komutu oluşturulur. Bu, sunucuya veri istemek/göndermek için kullanılır.

```
String atTcpPortConnectCmd = "AT+CIPSTART=\"TCP\",\"" + ip + "\"," + port;
connect_wifi(atTcpPortConnectCmd, 1000);
```
ESP8266, verilen IP ve port üzerinden TCP bağlantısı kurar.

```
int len = atHttpGetCmd.length();
String atSendCmd = "AT+CIPSEND=";
atSendCmd += len;
connect_wifi(atSendCmd, 1000);
```
ESP'ye kaç karakterlik veri göndereceğimizi bildiriyoruz.

```
connect_wifi(atHttpGetCmd, 1000);
```
GET isteği gönderilir (URL içeren komut).

```
Serial1.println("AT+CIPCLOSE"); // Bağlantıyı kapat
```
Sunucu ile bağlantı kapatılır. Kaynak kullanımını azaltmak için bu önemli bir adımdır.




















