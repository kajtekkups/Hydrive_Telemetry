//Arduino Uno Rev3 do odbioru danych z pierwszego Arduino i wypisania ich na wyświetlaczu

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n'); // Odczytaj dane przesłane przez pierwsze Arduino

    // Podziel odczytane dane na części (prędkość1, prędkość2, napięcie)
    String velocity1_str = getValue(data, ',', 0);
    String velocity2_str = getValue(data, ',', 1);
    String voltage_str = getValue(data, ',', 2);

    // Konwertuj odczytane wartości na liczby zmiennoprzecinkowe
    float velocity1 = velocity1_str.toFloat();
    float velocity2 = velocity2_str.toFloat();
    float voltage = voltage_str.toFloat();

    // Wybierz większą z dwóch prędkości (jeśli obie są różne od zera)
    float selectedVelocity = 0;
    if (velocity1 != 0 || velocity2 != 0) {
      selectedVelocity = (velocity1 != 0 && velocity2 != 0) ? max(velocity1, velocity2) : (velocity1 != 0) ? velocity1 : velocity2;
    }

    // Wyświetl odczytaną prędkość na wyświetlaczu LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Velocity: ");
    lcd.print(selectedVelocity, 2);
    lcd.print(" km/h");

    delay(1000);
  }
}

// Funkcja do wyodrębniania wartości z ciągu znaków po podanym separatorze
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
