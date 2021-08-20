#include "DHT.h" //import Sensor DHT 11
#include <LiquidCrystal_PCF8574.h> //import Sensor LCD
#include <Wire.h> //komunikasi serial
 
#define DHTPIN 2    //pin 2 tempat sensor DHT
#define DHTTYPE DHT11   // Tipe Sensor DHT = DHT 11
LiquidCrystal_PCF8574 lcd(0x27); // Alamat I2C LCD
DHT dht(DHTPIN, DHTTYPE);
 
byte pic_termo[8] = //icon suhu
{
    B00100,
    B01010,
    B01010,
    B01110,
    B01110,
    B11111,
    B11111,
    B01110
};
 
byte pic_humd[8] = //icon tetesan air
{
    B00100,
    B00100,
    B01010,
    B01010,
    B10001,
    B10001,
    B10001,
    B01110,
};
 
int val; //Variabel untuk menyimpan nilai dari sensor soil moisture
void setup() {
  pinMode(13,OUTPUT); //pin 13 ,pin untuk relay
  pinMode(8,INPUT); //pin 8 ,pin untuk sensor soil moisture
  Serial.begin(9600); //kecepatan komunikasi serial
  Serial.println("IoT Penyiram - Sensor DHT 11"); //penulisan di serial monitor
  lcd.begin(16, 2); //mulai lcd
  lcd.setBacklight(255); //nyalakan cahaya backlight LCD
  lcd.setCursor(0,0); //baris 1 LCD
  lcd.print("IoT Penyiram"); //tampilkan Teks
  lcd.setCursor(0,1); //baris 2 LCD
  lcd.print("Sensor DHT-Soil"); //tampilkan Teks
  delay(2000); //waktu delay
  lcd.clear(); //bersihkan tampilan LCD
  lcd.createChar(1,pic_termo); //icon suhu
  lcd.createChar(2,pic_humd); //icon kelembapan
  dht.begin(); //nilai sensor DHT
}
 
void loop() {
  delay(2000);  //menunggu beberapa detik untuk pembacaan
  float humidity_1 = dht.readHumidity(); //Pembacaan untuk data kelembaban
  float celcius_1 = dht.readTemperature(); //Pembacaan suhu dalam format celcius (c)
  float fahrenheit_1 = dht.readTemperature(true); //pembacaan dalam format Fahrenheit
  //mengecek pembacaan data error atau tidak
  if (isnan(humidity_1) || isnan(celcius_1) || isnan(fahrenheit_1)) {
    Serial.println("Pembacaan data dari module sensor gagal!");
    return;
  }
  float htof = dht.computeHeatIndex(fahrenheit_1, humidity_1); //Prosedur pembacaaan data suhu dalam bentuk fahreheit
  float htoc = dht.computeHeatIndex(celcius_1, humidity_1, false); //Prosedur pembacaaan data indeks panas dalam bentuk celcius
 
  //pembacaan nilai pembacaan data kelembaban dalam serial monitor
  Serial.print("Kelembaban: ");
  Serial.print(humidity_1);
  Serial.print(" %\t");
  
  //pembacaan nilai pembacaan data suhu dalam serial monitor
  Serial.print("Suhu : "); 
  Serial.print(celcius_1); //format derajat celcius
  Serial.print("°"); //simbol derajat
  Serial.print("C / "); //celsius
  Serial.print(fahrenheit_1); //format derajat fahrenheit
  Serial.print("°"); //simbol derajat
  Serial.print("F"); //fagrenheit
  Serial.print("\t");
 
 
  //tampilan suhu dan kelembapan di LCD
  lcd.setCursor (0,1);
  lcd.write(1);
  lcd.setCursor (1,1);
  lcd.print(celcius_1); //data sensor suhu dalam celsius
  lcd.print((char)223); //simbol derajat
  lcd.print("C"); //celsius
  lcd.setCursor (9,1);
  lcd.write(2);
  lcd.setCursor (10,1);
  lcd.print(humidity_1); //data sensor kelembapan
  lcd.print("%"); //persen 
   
  val = digitalRead(8);  //Baca data dari Sensor Soil Moisture 
  if(val == LOW) 
  {
  digitalWrite(13,HIGH); //nilai ke relay
  lcd.setCursor (0,0); //baris 1
  lcd.print("Pompa air OFF"); //tampilkan teks
  }
  if(val == HIGH)
  {
  digitalWrite(13,LOW); //nilai ke relay
  lcd.setCursor (0,0); //baris 1
  lcd.print("Pompa air ON "); //tampilkan teks
  }
  delay(400);
}
