#include <LiquidCrystal.h>
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE DHT11
#define RELAYTEMP 10
#define RELAYHUMI 11
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int selected = 1;
int x;
unsigned long counter = 5000;
int tset = 40;
int hset = 50;
int lastt = 0;
int lasth = 0;
// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons()
{
  adc_key_in = analogRead(0);      // read the value from the sensor
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  // For V1.1 us this threshold
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 250)  return btnUP;
  if (adc_key_in < 450)  return btnDOWN;
  if (adc_key_in < 650)  return btnLEFT;
  if (adc_key_in < 850)  return btnSELECT;

  // For V1.0 comment the other threshold and use the one below:
  /*
    if (adc_key_in < 50)   return btnRIGHT;
    if (adc_key_in < 195)  return btnUP;
    if (adc_key_in < 380)  return btnDOWN;
    if (adc_key_in < 555)  return btnLEFT;
    if (adc_key_in < 790)  return btnSELECT;
  */
  return btnNONE;  // when all others fail, return this...
}
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);        // select the pins used on the LCD panel
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  lcd.begin(16, 2);                       // start the library
  dht.begin();
  pinMode(RELAYTEMP, OUTPUT);
  pinMode(RELAYHUMI, OUTPUT);
}

void loop() {
  lcd_key = read_LCD_buttons();  // read the buttons
  if (lcd_key != btnNONE) {
    varsetTempHumi();
  }
  if (millis() >= counter) {
    counter = millis() + 5000;
    int h = int(dht.readHumidity());
    int t = int(dht.readTemperature());
    if (( t != lastt ) or (h != lasth)) {
      lastt = t;
      lasth = h;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.setCursor(8, 0);
      lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.print("Humi: ");
      lcd.setCursor(7, 1);
      lcd.print(" %");
      lcd.setCursor(6, 0);
      lcd.print(t);
      lcd.setCursor(6, 1);
      lcd.print(h);
      if ((t < tset) and (h < hset)) {
        digitalWrite(RELAYTEMP, HIGH);
        digitalWrite(RELAYHUMI, HIGH);
        lcd.setCursor(14, 0);
        lcd.print("ON");
        lcd.setCursor(14, 1);
        lcd.print("ON");
      }
      if ((t >= tset) and (h >= hset)) {
        digitalWrite(RELAYTEMP, LOW);
        digitalWrite(RELAYHUMI, LOW);
      }
      if ((t >= tset) and (h < hset)) {
        digitalWrite(RELAYTEMP, HIGH);
        digitalWrite(RELAYHUMI, LOW);
        lcd.setCursor(14, 1);
        lcd.print("ON");
      }
      if ((t < tset) and (h >= hset)) {
        digitalWrite(RELAYTEMP, LOW);
        digitalWrite(RELAYHUMI, HIGH);
        lcd.setCursor(14, 0);
        lcd.print("ON");
      }
    }
  }
}


void varsetTempHumi() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SetTemp: ");
  lcd.setCursor(11, 0);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("SetHumi: ");
  lcd.setCursor(11, 1);
  lcd.print("%");
  tset = constrain(tset, 10, 60);
  hset = constrain(hset, 10, 99);
  lcd.setCursor(9, 0);
  lcd.print(tset);
  lcd.setCursor(9, 1);
  lcd.print(hset);
  if (selected == 1) {
    lcd.setCursor(15, 0);
    lcd.cursor();
    lcd.blink();
  }
  if (selected == 2) {
    lcd.setCursor(15, 1);
    lcd.cursor();
    lcd.blink();
  }
  lcd_key = read_LCD_buttons();  // read the buttons
  switch (lcd_key)               // depending on which button was pushed, we perform an action
  {
    case btnRIGHT:
      {
        if (selected == 1) {
          tset++;
        }
        if (selected == 2) {
          hset++;
        }
        delay(200);
        break;
      }
    case btnLEFT:
      {
        if (selected == 1) {
          tset--;
        }
        if (selected == 2) {
          hset--;
        }
        delay(200);
        break;
      }
    case btnUP:
      {
        selected = 1;
        delay(200);
        break;
      }
    case btnDOWN:
      {
        selected = 2;
        delay(200);
        break;
      }
    case btnSELECT:
      {
        delay(200);
        break;
      }
    case btnNONE:
      {
        break;
      }
  }
  return;
}
