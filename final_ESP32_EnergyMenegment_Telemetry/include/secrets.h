#include <pgmspace.h>
 
#define SECRET
#define THINGNAME "Hydrive_Telemetry_ESP32" //can be find on AWS
 
const char WIFI_SSID[] = "Esp32test";               //change this
const char WIFI_PASSWORD[] = "maslomaslo";           //change this
const char AWS_IOT_ENDPOINT[] = "***************";       //change this
 
// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
 
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                                               //change this
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
 
-----END CERTIFICATE-----
 
 
)KEY";
 
// Device Private Key                                               //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
 
-----END RSA PRIVATE KEY-----
 
 
)KEY";