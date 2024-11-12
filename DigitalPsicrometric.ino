#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <DHT.h>                  //Cargamos la libreria DHT

//#include "max6675.h"

#include <SD.h>
#include <SPI.h>

// Pin CS (Chip Select) para la tarjeta microSD. Este puede variar dependiendo de la placa

const int chipSelect = 5;         // Asegúrate de que el pin corresponde a tu configuración
//File archivo;
#define SCREEN_WIDTH 128          //OLED display width, in pixels
#define SCREEN_HEIGHT 64          //OLED display height, in pixels

#define DHTTYPE DHT22             //Definimos el modelo DHT22
#define DHTPIN 4                  //Se define el pin D4 del ESP32 para conectar el sensor DHT22

//Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHT dht(DHTPIN, DHTTYPE, 22);

void setup() {
  Serial.begin(115200);
  dht.begin();
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))  {// Addres 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);

  // Inicia la tarjeta microSD
 /* Serial.print("Inicializando tarjeta SD...");
  if (!SD.begin(chipSelect)) {
    Serial.println("Fallo al inicializar la tarjeta SD");
    return;
  }
  
//  Serial.println("Tarjeta SD inicializada.");
//  archivo=SD.open("datos.txt", FILE_WRITE);

  

  // Leer el archivo desde la tarjeta SD
  dataFile = SD.open("PsicrometricDigital.txt");
  if (dataFile) {
    Serial.println("Contenido del archivo:");
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    dataFile.close();
  } else {
    Serial.println("Error abriendo el archivo para leer.");
  }
  // Escribir un archivo en la tarjeta SD
    File dataFile = SD.open("/PsicrometricDigital.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print("DHT22 ");
    dataFile.print("Humidity: ");
    dataFile.print(h);
    dataFile.print("%");
    dataFile.print("Temperature: ");
    dataFile.print(t);
    dataFile.print("°C");
    dataFile.close();
    Serial.println("Escritura en archivo exitosa.");
  } else {
    Serial.println("Error abriendo el archivo para escribir.");
  }
  */
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  //Display static text

  display.print("DHT22 T HR ");
  display.display();
  delay(100);


}

void loop() {
  
  float h = dht.readHumidity();     //Se lee la humedad y se asigna el valor a "h"
  float t = dht.readTemperature();  //Se lee la temperatura y se asigna el valor a "t"
  
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("HR: ");
  display.print(h);
  display.print("%");
  display.setTextSize(2);
  display.setCursor(0,16);
  display.print("T: ");
  display.print(t);
  display.print(" C");
  display.setTextSize(2);
  display.setCursor(0,32);
  display.print("Tbs:");
  display.print("22.10");
  display.print("C");
  display.setTextSize(2);
  display.setCursor(0,48);
  display.print("Tbh:");
  display.print("18.5");
  display.print("C");
  display.display();
  delay(1000);
}
