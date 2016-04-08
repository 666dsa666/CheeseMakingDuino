#include <LiquidCrystal.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define RELAY1 10
#define RELAY2 11
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int lcd_key = 0;
int adc_key_in = 0;
int selected = 1;
int tset = constrain(40, 10, 60);
int hset = constrain(20, 0, 100);
int varsetTempHumi = 0;
int HeaterON = 0;
int HumidifierON = 0;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);        // select the pins used on the LCD panel
DHT dht(DHTPIN, DHTTYPE);

int read_LCD_buttons(int msTimeOut) {
  unsigned long time = millis();
  while (time - millis() < msTimeOut)
  {
    adc_key_in = analogRead(0);
    if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 250)  return btnUP;
    if (adc_key_in < 450)  return btnDOWN;
    if (adc_key_in < 650)  return btnLEFT;
    if ((adc_key_in < 850)  && (varsetTempHumi == 0)) setTempHumi();
    if ((adc_key_in < 850)  && (varsetTempHumi == 1)) return btnSELECT;
  }
  return btnNONE;  // when all others fail, return this...
}

void setup() {
  lcd.begin(16, 2);                       // start the library
  dht.begin();
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.setCursor(8, 0);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Humi: ");
  lcd.setCursor(8, 1);
  lcd.print(" %");
  while (HeaterON = 1) {
    lcd.setCursor(15, 0);
    lcd.cursor();
    lcd.blink();
  }
  while (HumidifierON = 1) {
    lcd.setCursor(15, 1);
    lcd.cursor();
    lcd.blink();
  }
  delay(3000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    lcd.print("Failed!");
    return;
  }
  lcd.setCursor(6, 0);
  lcd.print(t);
  lcd.setCursor(6, 1);
  lcd.print(h);
  if (t < tset) {
    digitalWrite(RELAY1, LOW);
    HeaterON = 1;
  }
  else {
    digitalWrite(RELAY1, HIGH);
  }
  if (h < hset) {
    digitalWrite(RELAY2, LOW);
    HumidifierON = 1;
  }
  else {
    digitalWrite(RELAY2, HIGH);
  }
  return;
}

void setTempHumi() {
  while (1) {
    varsetTempHumi = 1;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SetTemp: ");
    lcd.setCursor(11, 0);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("SetHumi: ");
    lcd.setCursor(11, 1);
    lcd.print("%");
    while (varsetTempHumi == 1) {
      if (selected == 1) {
        lcd.setCursor(9, 0);
        lcd.print(tset);
        lcd.setCursor(15, 0);
      }
      if (selected == 2) {
        lcd.setCursor(9, 1);
        lcd.print(hset);
        lcd.setCursor(15, 1);
      }
      lcd.cursor();
      lcd.blink();
      lcd_key = read_LCD_buttons(5000);  // read the buttons; timeout = 5sec
      switch (lcd_key) {
        case btnUP: {
            selected = 1;
            break;
          }
        case btnDOWN: {
            selected = 2;
            break;
          }
        case btnRIGHT: {
            if (selected == 1) {
              tset++;
            }
            if (selected == 2) {
              hset++;
            }
            break;
          }
        case btnLEFT: {
            if (selected == 1) {
              tset--;
            }
            if (selected == 2) {
              hset--;
            }
            break;
          }
        case btnSELECT: {
            varsetTempHumi = 0;
            break;
          }
      }
    }
  }
  return;
}
