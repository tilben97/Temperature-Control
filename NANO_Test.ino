
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Keypad.h>
#include <Keyboard.h>
#define ONE_WIRE_BUS  10    // szenzor bemenete

#define RED_LED       11    // piros LED bemenet
#define GREEN_LED     12    //zold LED bemenet

char* askii_vekt[100] =
{
  "0  ", "1  ", "2  ", "3  ", "4  ", "5  ", "6  ", "7  ", "8  ", "9  ",
  "10 ", "11 ", "12 ", "13 ", "14 ", "15 ", "16 ", "17 ", "18 ", "19 ",
  "20 ", "21 ", "22 ", "23 ", "24 ", "25 ", "26 ", "27 ", "28 ", "29 ",
  "30 ", "31 ", "32 ", "33 ", "34 ", "35 ", "36 ", "37 ", "38 ", "39 ",
  "40 ", "41 ", "42 ", "43 ", "44 ", "45 ", "46 ", "47 ", "48 ", "49 ",
  "50 ", "51 ", "52 ", "53 ", "54 ", "55 ", "56 ", "57 ", "58 ", "59 ",
  "60 ", "61 ", "62 ", "63 ", "64 ", "65 ", "66 ", "67 ", "68 ", "69 ",
  "70 ", "71 ", "72 ", "73 ", "74 ", "75 ", "76 ", "77 ", "78 ", "79 ",
  "80 ", "81 ", "82 ", "83 ", "84 ", "85 ", "86 ", "87 ", "88 ", "89 ",
  "90 ", "91 ", "92 ", "93 ", "94 ", "95 ", "96 ", "97 ", "98 ", "99 "
};
uint8_t number_vekt[100] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24 , 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35 , 36, 37, 38, 39,
    40 ,41, 42, 43, 44, 45, 46, 47, 48, 49,
    50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
    70, 71, 72 ,73, 74, 75 ,76, 77, 78, 79,
    80, 81, 82, 83, 84, 85, 86, 87, 88, 89,
    90, 91, 92 ,93, 94 ,95, 96 ,97, 98, 99
};
// Billengyuzet nagysaga
const byte ROWS = 4;  
const byte COLS = 3;
char keys[ROWS][COLS] =
{
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {2,3,4,5};
byte colPins[COLS] = {6,7,8};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);     // letrehoza a billengyuzet objektumot

OneWire onewire(ONE_WIRE_BUS);          // A homerseklet szenzor labanak definialasa
DallasTemperature temp_sensor(&onewire);  // Tipus definialas
float Celcius = 0;
float Fahrenheit = 0;
// set LCD address, number of columns and rows
int lcdColumns =  16;
int lcdRows    =   2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  // LCD I2C modul cime

float ref_temp_H = 0;
float ref_temp_L = 0;
byte options = 1;
char key;
bool settings = false;
bool settings_toogle = false;
char ref_string_H[2];
char ref_string_L[2];
bool ref_cnt = 0;
bool ref_cnt1 = 0;



uint8_t ascii_to_number(char* string_number)      // szoveg alapu szambol - szam erteket csinal
{
  uint8_t number = 0;
  for(int cnt = 0; cnt < 100; cnt++)
  {
      if(*string_number == *askii_vekt[cnt])
      {
        string_number++;
        askii_vekt[cnt]++;
          if(*(string_number) == *(askii_vekt[cnt]))
          {
            string_number--;
            askii_vekt[cnt]--;
            number = number_vekt[cnt];
            return number;
            break;
          }
        else
        {
          string_number--;
          askii_vekt[cnt]--;
        }
      }
  }
}


void setup(){
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  temp_sensor.begin();
}

void loop(){
  key = keypad.getKey();
  if(settings)          // ezzen belul lehet a felso es also erteket allitani
  {
    if(key == '#')      // valtas magas es alacsony ertek kozott
    {
      settings_toogle = !settings_toogle;
      ref_cnt = 0;
      ref_cnt1 = 0;
      lcd.clear();
    }
    else if(key == '*')   // beallitas mentese
    {
      //Convert and save
      ref_temp_H = ascii_to_number((char*)ref_string_H);
      ref_temp_L = ascii_to_number((char*)ref_string_L);
      settings = false;
      ref_cnt = 0;
      ref_cnt1 = 0;
      lcd.clear();
    }
    if(settings_toogle)   // magas ertek allitas
    {
       lcd.setCursor(0,0);
       lcd.print("HIGH TEMP: ");
       if(key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9' || key == '0')
       {
          if(ref_cnt >= 2)
          {
            ref_cnt = 0;
            settings_toogle = !settings_toogle;
          }
          ref_string_H[ref_cnt] = key;
          lcd.setCursor(ref_cnt, 1);
          lcd.print(key);
          ref_cnt++;
       }
    }
    else              // alacsony ertek allitas
    {
       lcd.setCursor(0,0);
       lcd.print("LOW TEMP: ");
       if(key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9' || key == '0')
       {
          if(ref_cnt1 >= 2)
          {
            ref_cnt1 = 0;
            settings_toogle = !settings_toogle;
          }
          ref_string_L[ref_cnt1] = key;
          lcd.setCursor(ref_cnt1, 1);
          lcd.print(key);
          ref_cnt1++;
       }
    }

  }
  else    // alap funkcio
  {
    if(key == '*')      // valtas a beallitas menure
      settings = true;
    if(key == '#')      // C, F, CF kiiras valtas
      options++;
    if(options == 4)
      options = 1;
    temp_sensor.requestTemperatures();
    Celcius = temp_sensor.getTempCByIndex(0);
    Fahrenheit = temp_sensor.toFahrenheit(Celcius);

    switch(options)           // kiirasi opciok
    {
    case 1:                   // Celziusz
       lcd.setCursor(0, 0);
       lcd.print("Celcius: ");
       lcd.setCursor(0, 1);
       lcd.print(Celcius);
      break;
    case 2:                   // Fahrenheit
       lcd.setCursor(0, 0);
       lcd.print("Fahrenheit: ");
       lcd.setCursor(0, 1);
       lcd.print(Fahrenheit);
      break;
    case 3:                 // Mind ketto
      lcd.setCursor(0, 0);
      lcd.print("Celcius: ");
      lcd.setCursor(12, 0);
      lcd.print(Celcius);
      lcd.setCursor(0, 1);
      lcd.print("Fahrenheit: ");
      lcd.setCursor(12, 1);
      lcd.print(Fahrenheit);
      break;
    }

  // LED allitas - piros ha alacsony a hofok (FUTES) - zold ha magas (HUTES) - ha megfelelo egyik sem dolgozik
    if(Celcius < ref_temp_L)
      digitalWrite(RED_LED, HIGH);
    else
      digitalWrite(RED_LED, LOW);
    if(Celcius > ref_temp_H)
      digitalWrite(GREEN_LED, HIGH);
    else
      digitalWrite(GREEN_LED, LOW);
    delay(70);    // LCD miatt lassitas
    lcd.clear();  // kepernyo torles
  }

 
}
