#include <SPI.h>
#include <RF24.h>
#include <BTLE.h>
#include <DHT.h>                                                           

#define DHTPIN 4                                                          
#define DHTTYPE DHT22                                                       
DHT dht(DHTPIN, DHTTYPE);

RF24 radio(7,6); // CE, CSN
BTLE btle(&radio);

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.print("BLE and DHT Starting... ");
  Serial.println("Send Temperature Data over BTLE");
  dht.begin();   
  btle.begin("My DHT");    // 8 chars max
  Serial.println("Successfully Started");
}

void loop() {                                              
  float temp = dht.readTemperature(); 
  if (isnan(temp)) {                                               
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print(" Temperature: ");  
  Serial.print(temp);  
  Serial.println("°C ");
  
  nrf_service_data buf;
  buf.service_uuid = NRF_TEMPERATURE_SERVICE_UUID;
  buf.value = BTLE::to_nRF_Float(temp);

  if (!btle.advertise(0x16, &buf, sizeof(buf))) {
    Serial.println("BTLE advertisement failed..!");
  }
  btle.hopChannel(); 
  delay(2000);
}
