# 🌡️ DHT11 ile Sıcaklık & Nem Takibi ve ThingSpeak'e Veri Gönderimi

Bu projede, DHT11 sıcaklık ve nem sensörü kullanılarak elde edilen veriler, bir I2C LCD ekran üzerinde görüntülenmekte ve aynı zamanda ESP8266 WiFi modülü ile internet üzerinden ThingSpeak sunucusuna gönderilmektedir. Gönderilen veriler, ThingSpeak arayüzü üzerinden grafiksel olarak izlenebilmektedir. 

## 🔧 Kullanılan Devre Elemanları

-Arduino UNO

-DHT11 Nem ve Sıcaklık Sensörü

-I2C LCD (16x2)

-ESP8266 WiFi Modülü

-12-F/M. 5-M/M jumper kablo

### 📡 ESP8266 Nedir? Niçin Kullanılır?
ESP8266, üzerinde WiFi modülü bulunan küçük, düşük maliyetli ve oldukça güçlü bir mikrodenetleyicidir. Genellikle nesnelerin interneti (IoT) projelerinde kullanılır. İnternete bağlanmak ve veri alışverişi yapmak için yaygın olarak tercih edilir.\
Bizim projemizde sıcaklık ve nem sensörü (DHT11) ile ölçtüğümüz verileri internet üzerinden ThingSpeak platformuna göndermek istedik. Arduino’nun kendisi internete bağlanma yeteneğine sahip olmadığı için, bu işi yapmak üzere ESP8266 modülünü kullandık.Arduino ile ESP8266 arasında yazılımsal seri iletişim kurduk ve ESP8266’ya AT komutları göndererek WiFi ağına bağlanmasını, veri iletmesini sağladık.\
ESP8266 ile haberleşmek için AT komut seti kullanılır. AT komutları sayesinde modüle bağlanma, veri gönderme, bağlantıyı kapatma gibi işlemler yaptırılabilir.\
Aşağıda bu projede kullandığımız önemli AT komutlar ve işlevleri yer almaktadır:
- **AT**	ESP8266’nın çalışıp çalışmadığını test eder. Yanıt olarak OK dönmelidir.
- **AT+CWMODE=1**	ESP'yi Wi-Fi istemci moduna (Station Mode) geçirir.
- **AT+CWQAP**	Daha önce bağlanılan WiFi ağından çıkış yapar.
- **AT+RST**	ESP8266’yı yeniden başlatır (reset).
- **AT+CWJAP="SSID","ŞİFRE"**	Belirtilen Wi-Fi ağına bağlanmak için kullanılır.
- **AT+CIPSTART="TCP","IP",80**	Belirtilen IP adresi ve port üzerinden TCP bağlantısı kurar (bizim için bu, ThingSpeak sunucusu).
- **AT+CIPSEND=n**	n bayt uzunluğunda veri göndermek üzere hazırlar. > işareti alınırsa veri gönderimine hazırdır.
- **GET /update?... HTTP/1.0**	ThingSpeak'e gönderilecek HTTP GET isteğidir. Sensör verilerini bu yolla sunucuya yollarız.
- **AT+CIPCLOSE**	Açık olan TCP bağlantısını kapatır.
- **AT+CIPMUX=1** Çoklu bağlantı modunu etkinleştirir (birden fazla bağlantı açılabilir).

### 🌡️ DHT11 Nedir, Neden Kullandık?
DHT11, hem sıcaklık hem de nem ölçümü yapabilen düşük maliyetli ve kullanımı kolay dijital bir sensördür. İçerisinde bir nem sensörü ve termistör (sıcaklık sensörü) barındırır. Ölçtüğü verileri dijital olarak gönderdiği için ADC (Analog-Dijital Dönüştürücü) pinlerine ihtiyaç duymaz.\
Bu projede DHT11 sensörünü kullanarak ortamın anlık sıcaklık ve nem değerlerini ölçtük ve bu verileri LCD ekranda gösterip aynı zamanda internet aracılığıyla ThingSpeak'e göndererek grafiksel olarak uzaktan izlenebilir hale getirdik. Bu sayede, çevresel koşullarını izlemek istediğimiz akıllı sistemlerin temelini oluşturmuş olduk.

### 📺 I2C Modüllü LCD Ekran Kullanmanın Avantajları
Projede kullandığımız LCD ekran, I2C modülüne sahiptir. I2C modüllü LCD sayesinde devrenin hem donanımsal hem de görsel yönetimini daha verimli şekilde gerçekleştirdik. Normalde 16x2 LCD ekranlar çalışmak için 6-8 arası pin bağlantısı isterken, I2C modülü sayesinde sadece 2 pinle (SDA ve SCL) bağlantı sağlanır. Bu da şu avantajları beraberinde getirir:
- Daha az kablo kullanımı sayesinde karmaşıklık azalır, devrenin kurulumu sade ve düzenli olur.
- Diğer pinleri boş bırakılır, böylece Arduino’nun başka görevler için daha fazla pinini kullanabiliriz.
- I2C sayesinde aynı veri yolu üzerinden birden fazla cihaz kontrol edilebilir.
- Modülün üzerinde yer alan potansiyometre, LCD ekranın parlaklığını ayarlamamıza olanak tanır. Bu sayede yazıların okunabilirliği artırılabilir. Üstelik ek potansiyometre kullanımına gerek olmadan.

### 🌐 ThingSpeak Nedir, Bu Projede Neden Kullandık?
ThingSpeak, IoT (Nesnelerin İnterneti) projeleri için geliştirilmiş ücretsiz bir veri toplama ve görselleştirme platformudur. Özellikle sensörlerden gelen verileri bulutta saklamak, grafiklerle izlemek ve başka uygulamalara aktarmak için kullanılır. MathWorks tarafından desteklendiği için MATLAB entegrasyonu da sunar.\
Biz bu projede DHT11 sensöründen aldığımız sıcaklık ve nem verilerini internete yüklemek için ThingSpeak kullandık  

## ⚙️ Bağlantılar

-DHT11-> S (signal) arduinonun 8 nolu dijital pini, + (Vcc) 5V'a, -(GND) gndye baglanir

-ESP8266-01 Wi-Fi Module 3V3, EN-> arduino 3.3V, TX->2, RX-> 3, GND->GND, diger pinleri boşta

-I2C modüllü LCD GND->GND, VCC-> 5V, SDA-> A4, SCL-> A5

## DİKKAT!
esp8266 3V toleranslıdır. RX pini arduino'nun 5V veren pinlerine direkt olarak bağlanmamalıdır. voltaj regülatörü, adaptör kullanılabilir. biz 3 tane 1K direnç kullanarak voltaj bölücü devresi yaptık. böylece esp8266nın RX pinine yaklaşık 3.3V gönderebildik. aşağıda voltaj bölücü devrenin şematik gösterimi ve bütün modüllerin breadboard üzerinde gerçeklenmiş hali bulunmaktadır: 

![WhatsApp Image 2025-04-25 at 01 41 46](https://github.com/user-attachments/assets/69e11f07-1c94-4423-b1e9-2fc2c5188db7) ![WhatsApp Image 2025-04-25 at 01 36 41](https://github.com/user-attachments/assets/d15dc9d5-2da4-4b0e-acf6-50d05aa2dcbb)



## 📡 ThingSpeak Yapılandırması ve API anahtarı alma
1. https://thingspeak.com/ adresine gidin ve hesabınız yoksa bir hesap oluşturun. Hesabınıza giriş yapın.
2. "Channels" menüsünde "New Channel" tıklanır.
3. Yeni kanalı oluşturmak için ekranda beliren formda kanalın adı "Nem&Sicaklik" girilir.
4. field1 ve field2 alanlarına "nem" ve "sıcaklık" yazılır. her iki alan için onay kutuları işaretlenir.
5. sayfanın altında Save Channel basılarak kaydedilir. 
6. API Keys sekmesine gidilerek "Write API key" altındaki API numarası kaydedilir. bu daha sonra kod içerisinde apiKey değişkeninde saklanır. 


## 📦 1. Kütüphanelerin Dahil Edilmesi
```
#include <DHT.h>               // DHT11 sıcaklık ve nem sensörü kütüphanesi
#include <LiquidCrystal_I2C.h> // I2C üzerinden LCD ekran kontrolü için kütüphane
#include <SoftwareSerial.h>    // ESP8266 ile haberleşme için yazılım seri haberleşme
```
Bu kütüphaneler sayesinde sensör okumaları yapılabilir, LCD'ye veri yazılabilir ve ESP8266 modülü ile haberleşme sağlanabilir.

DHT.h -> https://github.com/adidax/dht11

LiquidCrystal_I2C.h -> https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library

SoftwareSerial.h-> arduino içinde hazırda var.\

yukarıda sıralanan kütüphaneler ilgili linklerden indirilip kopyalanıp arduino klasörünün altındaki library klasörüne yapıştırılması gerekir. veya arduino ide üzerinden sketch>include library> add .zip library... ile de dahil edilebilir. 



## 🌍 2. Global Değişken Tanimlamalari ve Nesneler
Bu kısımda sensörler, LCD ekran ve WiFi için gerekli tüm nesneler ve ayarlar tanımlanır:

```
#define RX 2              // ESP8266 RX pini (Arduino'dan TX çıkışı)
#define TX 3              // ESP8266 TX pini (Arduino'ya RX girişi)
#define DHTPIN 8          // DHT11 sensörünün bağlı olduğu dijital pin
#define DHTTYPE DHT11     // Kullanılan sensör tipi: DHT11

DHT dht(DHTPIN, DHTTYPE);               // DHT sensör nesnesi
LiquidCrystal_I2C lcd(0x27, 16, 2);     // LCD nesnesi (adres: 0x27, 16 sütun, 2 satır)
SoftwareSerial esp8266(RX, TX);         // ESP8266 ile yazılım seri portu oluşturuluyor

// WiFi ve ThingSpeak ayarları
String ssid = "TP-Link_4E66";                 // WiFi SSID (ağ adı)
String password = "wormhole";           // WiFi şifresi
String apiKey = "OEOBVWS4YRDPUZH9";     // ThingSpeak write API anahtarı
String host = "api.thingspeak.com";     // Sunucu adresi
String port = "80";                     // HTTP portu

// AT komutu yönetimi için sayaçlar
int countTrueCommand;
int countTimeCommand;
boolean found = false;
```
NOT:
- ssid ve password değişkenlerini kendi Wi-Fi ağınıza göre değiştirin.

- apiKey kısmına kendi ThingSpeak kanalınızın yazma API anahtarını girin.
## ⚙️ 3. setup() Fonksiyonu
Cihaz ilk açıldığında bir defaya mahsus çalışan başlatma bloğudur. LCD, DHT sensörü ve ESP8266 başlatılır, WiFi ağına bağlanılır.
```
void setup() {
  Serial.begin(9600);              // Arduino’nun seri haberleşmesi başlatılıyor
  esp8266.begin(115200);           // ESP8266 ile haberleşme başlatılıyor (baud rate 115200)

  lcd.begin();                     // LCD ekran başlatılıyor
  lcd.backlight();                 // Arka ışık açılıyor
  lcd.setCursor(0, 0);             
  lcd.print("Baglaniyor...");      // LCD'ye bilgi mesajı yazılıyor

  dht.begin();                     // DHT sensörü başlatılıyor

  // ESP8266’ya AT komutları gönderiliyor
  sendCommand("AT", 5, "OK");                                        // Modül çalışıyor mu kontrolü
  sendCommand("AT+CWMODE=1", 5, "OK");                               // İstemci modu
  sendCommand("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"", 20, "OK");  // WiFi bağlantısı

  lcd.clear();                    // LCD temizleniyor
  lcd.setCursor(0, 0);
  lcd.print("Wifi Baglandi!");    // Bağlantı başarılı mesajı
  delay(2000);                    // Mesajı göstermek için 2 saniye bekle
}
```
## 🔁 4. loop() ve sendCommand() Fonksiyonları
Ana döngüde her 20 saniyede bir sıcaklık ve nem verisi alınır, LCD’ye yazılır ve ThingSpeak sunucusuna gönderilir.
```
void loop() {
  float h = dht.readHumidity();        // Nem değeri okunur
  float t = dht.readTemperature();     // Sıcaklık değeri okunur

  lcd.clear();                         // LCD ekran temizlenir
  lcd.setCursor(0, 0);                 
  lcd.print("Nem: ");
  lcd.print(h);
  lcd.print(" %");

  lcd.setCursor(0, 1);
  lcd.print("Sicaklik: ");
  lcd.print(t);
  lcd.print(" C");

  // ThingSpeak’e gönderilecek veri paketi hazırlanır
  String getData = "GET /update?api_key=" + apiKey + "&field1=" + String(h) + "&field2=" + String(t);

  sendCommand("AT+CIPMUX=1", 5, "OK");                                // Çoklu bağlantı modu
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + host + "\"," + port, 15, "OK");  // TCP bağlantısı başlat
  sendCommand("AT+CIPSEND=0," + String(getData.length() + 4), 4, ">");     // Veri gönderimine hazırla
  esp8266.println(getData);                                           // GET isteği gönderilir
  delay(2000);                                                        // Gönderim sonrası bekleme
  sendCommand("AT+CIPCLOSE=0", 5, "OK");                              // Bağlantıyı kapat

  delay(20000); // 20 saniye sonra döngü yeniden başlar
}
```
🛠 sendCommand() Fonksiyonu\
ESP8266’ya AT komutu gönderip, cevap bekleyen yardımcı fonksiyondur.
```
void sendCommand(String command, int maxTime, char readReply[]) {
  Serial.print(countTrueCommand);
  Serial.print(". AT komutu: ");
  Serial.println(command);

  while (countTimeCommand < (maxTime * 1)) {   // Belirtilen süre kadar komutu tekrar dener
    esp8266.println(command);                  // Komut gönderilir
    if (esp8266.find(readReply)) {             // Cevap aranır
      found = true;                            // Cevap bulunduysa döngüden çıkılır
      break;
    }
    countTimeCommand++;
  }

  if (found) {
    Serial.println("OK");                      // Cevap başarılıysa seri ekrana yazılır
    countTrueCommand++;
  } else {
    Serial.println("Basarisiz");               // Cevap alınamazsa hata mesajı yazılır
    countTrueCommand = 0;
  }

  countTimeCommand = 0;
  found = false;                               // Değişkenler sıfırlanır
}
```
-Gerekli kütüphaneler dahil edilip Arduino IDE üzerinden kodlar derlenir.\


## 🔧 Nasıl Çalışır?

Dört ana bölümden oluşan bu projede:
- Sensör Okuma:
DHT11 sensörü, ortam sıcaklığını santigrat (°C) ve nem oranını yüzde (%) cinsinden ölçer.
- Veri İşleme:
Arduino Uno, bu verileri dht.readTemperature() ve dht.readHumidity() fonksiyonları ile alır ve LCD ekran üzerinden kullanıcıya sunar.
- Veri Aktarımı:
Ölçülen sıcaklık ve nem verileri, yazılımsal seri port (SoftwareSerial) aracılığıyla ESP8266 modülüne iletilir. ESP8266, bu verileri ThingSpeak sunucusuna bir HTTP GET isteği olarak gönderir.
- Veri Görselleştirme:
ThingSpeak platformu, gelen bu verileri işler ve zaman serisi grafikler şeklinde kullanıcıya sunar. Bu grafikler sayesinde ortam koşulları zamana bağlı olarak takip edilebilir.
do[ru alındığı taktirde ThingSpeak verileri aşağıdaki gibi grafik şeklinde görüntüler:

![Ekran görüntüsü 2025-04-25 022538](https://github.com/user-attachments/assets/4a2e49ce-b668-48a6-a11f-8ab68aa4e50b)

## 🔬 Test Süreci ve Çıkarımlar
1. Test Senaryosu 1 – ESP8266 modülünün temel fonksiyon testi 
Proje sırasında karşılaştığımız, sunucuya veri iletimi sorunlarının kaynağını tespit etmek amacıyla, ESP8266 modülünün temel fonksiyonlarını test etmek üzere özel bir test kodu geliştirdik. Amacımız; modülün donanımsal olarak mı yoksa yazılımsal olarak mı sorunlu olduğunu anlamaktı.
- 🧪 Uygulanan Test
Aşağıdaki adımları içeren bir test dizisi oluşturuldu:

  + ESP8266’nın AT komutlarına yanıt verip vermediği test edildi.

  + Wi-Fi ağına bağlanma durumu kontrol edildi.

  + TCP üzerinden bir web sunucusuna (httpbin.org) bağlantı kuruldu.

  + Sunucuya basit bir HTTP GET isteği gönderildi.

  + Sunucudan dönen cevabın doğru şekilde okunup okunmadığı analiz edildi.

  + Bağlantı sonlandırılmaya çalışıldı.

- 📋 Test Çıktısı ve Yorumu

![Ekran görüntüsü 2025-04-25 035622](https://github.com/user-attachments/assets/585f9b37-f938-4968-ae61-ff156f66befb)
  + ESP8266 donanımsal olarak çalışıyor ve Wi-Fi ağına başarıyla bağlanabiliyor.

  + TCP bağlantısı ve veri gönderimi başarılı şekilde tamamlandı.

  + Sunucudan gelen cevabın okunmasında bozulmalar gözlemlendi. Bu durum genellikle:

  + UART baud rate uyumsuzluğu, fazla veri alımı nedeniyle buffer taşması, karakter kodlaması veya zamanlama sorunları gibi nedenlerden kaynaklanabilir.

  + AT+CWMODE=1 ve AT+CIPCLOSE komutlarının başarısız olması, ESP'nin zamanlamaya hassas olduğunu ve bazen komutlara yeterli bekleme süresi verilmeden gönderim yapıldığını düşündürmektedir.
- Sonuç
Test süreci boyunca ESP8266'dan gelen bazı komut cevapları tutarsızlık gösterdi. Örneğin, bazen AT+CWMODE=1 komutu başarısız olarak raporlanırken, aslında komut arka planda başarıyla tamamlanmıştı. Aynı şekilde, bazı komutlar başarılı olarak işaretlenmesine rağmen beklenen etkiyi yaratmadı. Bu tutarsızlıkların temel nedeni, ESP8266'nın AT komutlarına verdiği cevapların zamanlaması ile ilgilidir.\

ESP8266, komutlara yanıt verirken bazen gecikebilir veya beklenen yanıtın (örneğin "OK") seri port üzerinden gecikmeli olarak gelmesi, yazılım tarafında zaman aşımına neden olabilir. Bu durumda komut zamanında cevaplanmış olsa bile, bizim yazdığımız kod bu cevabı geç algıladığı için “başarısız” olarak işaretlenebilir. Özellikle SoftwareSerial kütüphanesi kullanıldığında bu tür zamanlama problemleri daha sık yaşanabilir çünkü bu kütüphane donanımsal seri portlar kadar hızlı ve güvenilir değildir.\

Ayrıca bazı AT komutları (özellikle AT+CIPSTART veya AT+CIPSEND) işlem süresi daha uzun olduğu için yanıt vermeden önce daha fazla süreye ihtiyaç duyabilir. Eğer bu sürede komutun cevabı okunamazsa, sistem yanlışlıkla “komut başarısız” sonucuna varabilir.\

Tüm bu tutarsızlıklara rağmen genel olarak modülün doğru çalıştığı sonucuna varabildik. Çünkü:

 - AT komutlarına çoğunlukla yanıt alındı.

 - Wi-Fi ağına başarıyla bağlanıldı.

 - IP adresi alındı.

 - Sunucuya TCP bağlantısı kurulabildi ve HTTP cevabı alındı.

Bunlar, ESP8266’nın donanımsal olarak sağlıklı çalıştığını ve temel işlevlerini yerine getirebildiğinin göstergesidir. Bu test ile, donanımın sağlam olduğu, asıl problemin yazılım ve zamanlama yönetimi olduğu açıkça görülmüştür. Test sayesinde:

  - Sorunun kaynağı doğru şekilde tespit edilmiştir.

  - Gereksiz donanım değişikliklerinden kaçınılmıştır.

  - Daha stabil ve anlaşılır bir kod yapısı kurulmuştur.

2. Test Senaryosu 2 – ThingSpeak Veri Gönderim Testi
Bu testin amacı, ESP8266 modülünün ThingSpeak sunucusuna başarılı şekilde veri gönderip gönderemediğini kontrol etmektir. Bu sayede hem Wi-Fi bağlantısının hem de internet üzerinden veri iletiminin doğru çalıştığı ve verilerin sunucuda güncellendiği doğrulanmak istenmiştir.
- Test kodu şu adımları içerir:

  + ESP8266’nın AT komutlarına yanıt verip vermediğinin kontrolü

  + Wi-Fi istemci (station) moduna geçiş

  + Belirtilen SSID ve şifre ile Wi-Fi ağına bağlanma

  + IP adresi alınarak bağlantının doğrulanması

  + ThingSpeak sunucusuna TCP bağlantı kurulması

  + Rastgele üretilmiş sıcaklık ve nem verilerinin HTTP GET isteği ile gönderilmesi

  + Bağlantının kapatılması
    
- 📋 Elde Edilen Çıktılar ve Gözlemler
![Ekran görüntüsü 2025-04-25 041243](https://github.com/user-attachments/assets/d0884339-1bd8-4138-b39a-d0d1bb85f89c)

- Yorumlar ve Analiz
  + AT+CWMODE=1 ve AT+CIFSR komutları başarısız görünüyor ancak bu, seri porttan gelen cevabın bozuk okunması nedeniyle oluşmuştur. ESP8266 muhtemelen bu komutları işledi.

  + AT+CWJAP ve AT+CIPSTART komutları başarıyla tamamlandı; bu da modülün Wi-Fi ağına bağlanabildiğini ve internete çıkabildiğini gösteriyor.

  + AT+CIPSEND komutu başarısız olarak işaretlense de ThingSpeak üzerinde veri güncellendi, bu da GET isteğinin başarıyla gönderildiğini ve işlendiğini kanıtlar.
- Sonuç
Bu test sayesinde:

  + ESP8266'nın doğru çalıştığı, Wi-Fi bağlantısını kurabildiği ve veri iletebildiği doğrulanmıştır.

  + Çıktılarda görülen bazı başarısızlıklar, zamanlama sorunları veya SoftwareSerial kaynaklı karakter bozulmaları nedeniyle oluşmuştur.

  + Gerçek amaç olan ThingSpeak sunucusuna veri göndermek başarıyla gerçekleştirilmiştir.

 Bu test, yazılım tarafındaki yanıt kontrol mekanizmalarının iyileştirilmesi gerektiğini gösterse de, sistemin genel işlevselliğini başarıyla kanıtlamıştır.

## Notlar / Ek Gözlemler
1. ThingSpeak Kullanım ve API Sınırları Kontrolü
Test süreci sırasında, veri gönderim sorunlarının sadece ESP8266 kaynaklı olup olmadığını netleştirmek için ThingSpeak platformunun kullanım şartlarını ve API limitlerini de inceledik.
ThingSpeak’in ücretsiz kullanım lisansı altında, kullanıcıların:

  + Yılda maksimum 3 milyon mesaj gönderebileceği,

  + Maksimum 4 kanal açabileceği,

  + Bir kanalı her 15 saniyede bir güncelleyebileceği (ücretli kullanıcılar için bu sınır 1 saniyedir) belirtildiğini öğrendik.

Veri gönderiminin 15 saniyeden kısa aralıklarla yapılması durumunda, ThingSpeak’in yeni verileri kabul etmeme veya gecikmeli güncelleme yapma ihtimalini göz önünde bulundurarak, minimum 20 saniyelik aralıklarla veri göndererek sistem güncellemelerinin düzgün şekilde işlenmesini sağladık. Sonuç olarak, hem ESP8266'nın hem de ThingSpeak platformunun doğru çalıştığı net olarak doğrulanmış oldu.

2. ESP8266 Baud Rate Uyumsuzluğu
Başlangıçta hem esp.begin() hem de Serial.begin() komutlarını 9600 baud rate ile ayarlamıştık, ancak ESP8266'dan tutarsız ve bozuk yanıtlar alıyorduk. Bunun nedeni, ESP8266 modülünün fabrika çıkışı 115200 baud rate ile çalışıyor olmasıydı. Arduino ile ESP8266 arasında doğru iletişim kurabilmek için esp.begin(115200) olarak ayarladık. Bilgisayarla arduino arasında olan bağlantı için ise Serial.begin(9600) yeterli oldu. Bu değişiklik sonrasında ESP8266 ile iletişim sorunsuz şekilde sağlandı.

3. ESP8266 RX Pinine Yüksek Voltaj Uygulanması
Başlangıçta ESP8266'nın RX pinini doğrudan Arduino’nun TX pinine bağlamıştık. Ancak Arduino 5V ile çalıştığı için, ESP8266'nın 3.3V toleranslı RX pini bu voltaja maruz kaldı ve modül ciddi şekilde ısınmaya başladı. Bu nedenle modülün bozulduğunu düşündük ve gereksiz yere zaman kaybettik.

Daha sonra 1K ohmluk 3 dirençle basit bir voltaj bölücü devre kurarak Arduino’nun 5V çıkışını güvenli şekilde yaklaşık 3.3V’a düşürdük. Bu sayede ESP8266 stabil şekilde çalışmaya başladı.
Alternatif olarak:
  - 2 dirençle klasik voltaj bölücü yapılabilir (örneğin 1K + 2K)

  - Level shifter modülü kullanılabilir (daha güvenli ve uzun vadeli çözüm)

  - Arduino'nun 3.3V uyumlu versiyonları (ör. 3.3V Pro Mini) tercih edilebilir

Voltaj bölücü yapılmadan bazı durumlarda ESP8266 çalışıyor gibi görünse de, bu durum modülün zarar görmesine veya ömrünün kısalmasına neden olabilir. Bu yüzden 3.3V seviyesine uygun bir giriş mutlaka sağlanmalıdır.

4. ThingSpeak'e Tarayıcı Üzerinden GET İsteği Gönderme
Projeye ilk başladığımızda ESP8266 üzerinden ThingSpeak'e veri gönderimi yapmaya çalıştık. ilgili kodları Turcademy sitesinde bulunan "Nesnelerin Interneti" kitabındaki 5.projeden almıştık. Ancak bu kodda ESP8266 modülü ThingSpeak sunucusuyla bağlantı kuramadan bağlantıyı kapatıyordu. Yani ESP, komutu alıyordu ancak bağlantı isteği başarısız oluyordu.  HTTP isteğini şu şekilde doğrudan tarayıcıya yapıştırdık:
https://api.thingspeak.com/update?api_key=HDB4L*****RO6I&field1=50&field2=25\
Bu yöntem başarılı oldu — tarayıcıda ekrana bir sayı (örneğin 1347) döndü. Bu sayı, ThingSpeak’in veri tabanında oluşturduğu yeni kayıt numarasıydı. Bu bize şunu gösterdi:
ESP8266 başarılı olamasa da, doğru yapıdaki bir HTTP GET isteği ThingSpeak sunucusu tarafından doğrudan işlenebiliyor.
Bunun nedeni:
  - GET isteği zaten ThingSpeak’in HTTP API’siyle uyumluydu.
  - Tarayıcıda HTTPS üzerinden gönderilse bile, GET parametreleri doğruysa sunucu veriyi işliyor.
  - ThingSpeak cevaben kayıt numarası döndüğünde veri başarıyla işlendiği anlamına geliyor.
  - Cevap olarak 0 dönseydi bu ya veri hatası ya da zaman sınırı ihlali olurdu.
 
İlk koddaki sorunlar:
- Problem | Açıklama
    + TCP bağlantısı kurulamadı (CIPSTART) | Sunucuya bağlantı başarılı şekilde kurulamıyordu.
    + CIPSEND sırasında doğru veri uzunluğu verilmedi | Uzunluk hesabında \r\n karakterleri eksikti.
    + HTTP isteği eksikti | Host: başlığı ve HTTP/1.1 gibi detaylar yoktu.
    + Bağlantı kapanmadı | CIPCLOSE komutu kullanılmadı, bu da sonraki denemeleri etkiledi.

Güncel sürümde (DHT11_humidity_temperature.ino) aşağıdaki adımlar başarıyla uygulandı:
- Yapılan Doğru İşlem | Açıklama
    + CIPSTART ile bağlantı kuruldu | TCP bağlantısı başarılı şekilde açıldı.
    + CIPSEND için veri uzunluğu +4 ile belirtildi | \r\n karakterleri hesaba katıldı.
    + HTTP GET doğru formatta gönderildi | Sunucu parametreleri doğru şekilde aldı.
    + Bağlantı her seferinde kapatıldı (CIPCLOSE) | Bağlantı problemi yaşanmadı.
    + Veri gönderim aralığı korundu (delay(20000)) | ThingSpeak’in rate limit’ine uyuldu.

-  Genel Gözlem ve Öğrenim
  + ESP8266 ile HTTP istekleri yapılırken, veri uzunluğu, bağlantı sırası, ve paket formatı kritik önemdedir.
  + Basit bir GET isteği bile başarılı olabilmek için belli kurallara uygun yapılmalıdır.
  + Tarayıcıdan yapılan GET çağrıları, sadece testi doğrulamak için kullanışlıdır, çünkü onlar otomatik olarak tüm HTTP başlıklarını içerir.
  + ESP ise her detayı elle vermemizi ister: veri uzunluğu, bağlantı açılması, kapanması, HTTP satırlarının uygun formatı gibi.
