#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <FirebaseArduino.h>

LiquidCrystal_I2C lcd (0x27, 20,4);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

//Sensor Suhu dan Jarak
int jarak = 0;
float data = 0;
float rata = 0;
const int trigPin = 13; //D7
const int echoPin = 12; //D6
long duration;
int distance;

//Buzzer
int buzzer = 14;

//PIR Sensor
int pir1 = 16; // D0
int pir2 = 0; //D3

int in = 0, out = 0, count =0;

//Firebase
#define FIREBASE_HOST "your host"
#define FIREBASE_AUTH "yout key"

//Wifi Setting
#define WIFI_SSID "Apaja"
#define WIFI_PASSWORD "qwerty1234560"

void connect_wifi();

void setup() {
  Serial.begin(9600);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pir1, INPUT);
  pinMode(pir2, INPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(pir1, LOW);
  digitalWrite(pir2, LOW);

  connect_wifi();
  mlx.begin();

  lcd.begin();
  lcd.setCursor(6,1);
  lcd.print("Viscount");
  lcd.setCursor(4,2);
  lcd.print("Thermometer");
  delay(4000);
  lcd.clear();
 
}


void loop() {

      lcd.clear();
      lcd.setCursor(6,0);
      lcd.print("Total: ");
      lcd.setCursor(12,0);
      lcd.print(count);
      lcd.setCursor(2,1);
      lcd.print("in: ");
      lcd.setCursor(5,1);
      lcd.print(in);
      lcd.setCursor(11,1);
      lcd.print("out: ");
      lcd.setCursor(15,1);
      lcd.print(out);
      lcd.setCursor(3, 2);
      lcd.print("Cek Suhu Tubuh");
      lcd.setCursor(2, 3);
      lcd.print("Dekatkan Tangan");
      
      hitung_suhu();
      kirim_data();


      delay(4000);

  
}

  void connect_wifi(){

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println("Ip Address: ");
  Serial.print(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}




  void kirim_data(){
    
    long visin = digitalRead(pir1);
    long visout = digitalRead(pir2);
    
    if (visin == HIGH){
      digitalWrite(pir2, LOW);
      count++; 
      in = in+1;
      
      Firebase.setInt("Data/masuk", in);
      if (Firebase.failed()) {
      Serial.print("Gagal memasukan data 'masuk' ke firebase");
      Serial.println(Firebase.error());  
      return;
      }
      Firebase.setInt("Data/total", count);
      if (Firebase.failed()) {
        Serial.print("Gagal memasukan data 'total' ke firebase");
        Serial.println(Firebase.error());  
        return;
        }
        delay(2500);
     } 
     else if (visout == HIGH){
      digitalWrite(pir1, LOW);
      count--; 
      out = out+1;
      Firebase.setInt("Data/keluar", out);
      if (Firebase.failed()) {
        Serial.print("Gagal memasukan data 'keluar' ke firebase");
        Serial.println(Firebase.error());  
        return;
        }
        Firebase.setInt("Data/total", count);
        if (Firebase.failed()) {
          Serial.print("Gagal memasukan data 'total' ke firebase");
          Serial.println(Firebase.error());  
          return;
          }
          delay(2500); 
     }
     else if (visin == HIGH && visout == HIGH){
      digitalWrite(pir1, LOW);
      digitalWrite(pir2, LOW);
      delay(2000);
     }
     else {
      delay(1000);
     }
  }


  void hitung_suhu(){
    
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;

 
    if (distance <= 4){
      digitalWrite(buzzer, HIGH);
      delay(1000);
      digitalWrite(buzzer, LOW);
      data = mlx.readObjectTempC();
      delay(10);
      lcd.clear();
      lcd.setCursor(3, 1);
      lcd.print("Cek Suhu Tubuh");
      lcd.setCursor(2, 2);
      lcd.print("Suhu Tubuh: ");
      lcd.setCursor(13, 2);
      lcd.print(data);

      delay(1000);
      if (data >= 37){
        lcd.clear();
        for (int c = 0; c <=  10; c++){
          lcd.setCursor(0, 1);
          lcd.print("Suhu Melebihi Batas");
          lcd.setCursor(4,2);
          lcd.print("Suhu Tubuh: ");
          lcd.setCursor(16, 2);
          lcd.print(data);
//          Firebase.setFloat("Data/suhu", data);
//              if (Firebase.failed()) {
//        Serial.print("Gagal memasukan data 'suhu' ke firebase");
//        Serial.println(Firebase.error());  
//        return;
//        }
          delay(1000);
          }
      }
//            Firebase.setFloat("Data/suhu", data);
//          if (Firebase.failed()) {
//        Serial.print("Gagal memasukan data 'suhu' ke firebase");
//        Serial.println(Firebase.error());  
//        return;
//        }
      lcd.clear();
   }
  }