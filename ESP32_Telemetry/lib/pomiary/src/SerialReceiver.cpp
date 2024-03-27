#include "SerialReceiver.hpp"
#include "MQTT_Communication.hpp"

void receiveData() {
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

    // Wyślij odebrane dane na serwer MQTT
    String synchronizedData = String(velocity1) + "," + String(velocity2) + "," + String(voltage);
    publish_MQTT_message(MQTT_PUBLISH_TOPIC, synchronizedData);
  }
}
