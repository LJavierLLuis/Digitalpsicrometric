#include <DHT.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definir el pin del sensor DHT22
#define DHTPIN 4         // El pin donde está conectado el DHT22
#define DHTTYPE DHT22    // Tipo de sensor (DHT22)

//Definir el tamaño de la pantalla OLED
#define SCREEN_WIDTH 128          //OLED display width, in pixels
#define SCREEN_HEIGHT 64          //OLED display height, in pixels

const int chipSelect = 5;  // Pin CS de la tarjeta microSD

DHT dht(DHTPIN, DHTTYPE);  // Crear una instancia del sensor DHT
RTC_DS3231 rtc;            // Crear una instancia del RTC DS3231

//Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // Inicializar la comunicación serial
  Serial.begin(115200);
  while (!Serial) { }

  // Inicializar el sensor DHT
  dht.begin();

  // Inicializar el RTC
  if (!rtc.begin()) {
    Serial.println("¡No se pudo encontrar el RTC!");
    while (1);
  }

  // Inicializar la tarjeta SD
  if (!SD.begin(chipSelect)) {
    Serial.println("¡Error al inicializar la tarjeta SD!");
    return;
  }
  Serial.println("Tarjeta SD inicializada correctamente.");

  // Crear o abrir el archivo de datos en la tarjeta SD
  File archivo = SD.open("/datos_sensor.txt", FILE_WRITE);
  if (archivo) {
    archivo.println("Fecha y Hora, Temperatura (°C), Humedad (%)");
    archivo.close();
    Serial.println("Encabezado agregado al archivo.");
  } else {
    Serial.println("No se pudo crear el archivo.");
  }

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))  {// Addres 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);
}

void loop() {
  // Leer la temperatura y la humedad
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Comprobar si las lecturas son válidas
  if (isnan(h) || isnan(t)) {
    Serial.println("Error al leer del sensor DHT");
    return;
  }

  // Obtener la fecha y hora del RTC
  DateTime now = rtc.now();

  // Mostrar los datos en el monitor serial
  Serial.print("Fecha y Hora: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.print("  Humedad: ");
  Serial.print(h);
  Serial.print("%  Temperatura: ");
  Serial.print(t);
  Serial.println("°C");

  // Abrir el archivo para agregar datos
  File archivo = SD.open("/datos_sensor.txt", FILE_APPEND);
  if (archivo) {
    // Escribir los datos en el archivo
    archivo.print(now.year(), DEC);
    archivo.print('/');
    archivo.print(now.month(), DEC);
    archivo.print('/');
    archivo.print(now.day(), DEC);
    archivo.print(" ");
    archivo.print(now.hour(), DEC);
    archivo.print(':');
    archivo.print(now.minute(), DEC);
    archivo.print(':');
    archivo.print(now.second(), DEC);
    archivo.print(", ");
    archivo.print(t);
    archivo.print(", ");
    archivo.println(h);
    archivo.close();
    Serial.println("Datos guardados en la tarjeta SD.");
  } else {
    Serial.println("No se pudo abrir el archivo.");
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print(now.day(), DEC);
  display.print('/');
  display.print(now.month(), DEC);
  display.print('/');
  display.print(now.year(), DEC);
  display.print(" ");
  display.print(now.hour(), DEC);
  display.print(':');
  display.print(now.minute(), DEC);
  display.print(':');
  display.print(now.second(), DEC);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,8);
  display.print("HR: ");
  display.print(h);
  display.print("% ");
  display.print("T: ");
  display.print(t);
  display.print(" C");
  display.setTextSize(2);
  display.setCursor(0,16);
  display.print("Tbs:");
  display.print("22.10");
  display.print("C");
  display.setTextSize(2);
  display.setCursor(0,32);
  display.print("Tbh:");
  display.print("18.5");
  display.print("C");
  display.setTextSize(1);
  display.setCursor(0,56);
  display.print("Luis Javier Luis Luis");
  display.display();
  
  // Esperar 2 segundos antes de la siguiente lectura
  delay(2000);
}
