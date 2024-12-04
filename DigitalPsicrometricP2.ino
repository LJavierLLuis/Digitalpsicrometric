#include <DHT.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <max6675.h>

// Definir el pin del sensor DHT22
#define DHTPIN 4         // El pin donde está conectado el DHT22
#define DHTTYPE DHT22    // Tipo de sensor (DHT22)

//Definir el tamaño de la pantalla OLED
#define SCREEN_WIDTH 128          //OLED display width, in pixels
#define SCREEN_HEIGHT 64          //OLED display height, in pixels

const int chipSelect = 5;  // Pin CS de la tarjeta microSD

DHT dht(DHTPIN, DHTTYPE);  // Crear una instancia del sensor DHT
RTC_DS3231 rtc;            // Crear una instancia del RTC DS3231

float Tbs = 0;         //Se crea una variable flotante para almacenar la temperatura de bulbo seco
float Tbh = 0;         //Se crea una variable flotante para almacenar la temperatura de bulbo humedo

float Texp = 0;
float Tcov = 0;

//Se definen los pines a usar para conectar el modulo MAX6675 para Tbs
int ktcSO=12;
int ktcCS=14;
int ktcCLK=13;

//Se definen los pines a usar para conectar el modulo MAX6675 para Tbh
int btcSO=12;
int btcCS=27;
int btcCLK=13;

//Se definen los pines a usar para conectar el modulo MAX6675 para Temperatura del sensor expuesto
int expSO = 12;
int expCS = 26;
int expCLK = 13;

//Se definen los pines a usar para conectar el modulo MAX6675 para Temperatura del sensor con cubierta
int covSO = 12;
int covCS = 25;
int covCLK = 13;

MAX6675 ktc(ktcCLK, ktcCS, ktcSO);
MAX6675 btc(btcCLK, btcCS, btcSO);

MAX6675 etc(expCLK, expCS, expSO);
MAX6675 ctc(covCLK, covCS, covSO);

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

  // Nombre del archivo en la tarjeta SD
  String filename = "/datos_sensor.txt";

  // Revisamos si el archivo ya existe
  if (SD.exists(filename)) {
    Serial.println("El archivo ya existe. Continuando con la escritura.");
  } else {
    Serial.println("El archivo no existe. Creando uno nuevo.");
    
    // Crear o abrir el archivo de datos en la tarjeta SD
    File archivo = SD.open(filename, FILE_WRITE);
    if (archivo) {
      archivo.println("Fecha y Hora, Temperatura (°C), Humedad (%), Tbs, Tbh, Tcov, Texp"); // Encabezado
      archivo.close();
      Serial.println("Encabezado agregado al archivo.");
    } else {
      Serial.println("No se pudo crear el archivo.");
    }
  }

  // Inicializar la pantalla OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))  {
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

  // Leer tbs
  Tbs = ktc.readCelsius();

  // Leer tbh
  Tbh = btc.readCelsius();

  //Leer Tcov
  Tcov = ctc.readCelsius();
   
  //Leer Texp
  Texp = etc.readCelsius();

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
  Serial.print("  HR: ");
  Serial.print(h);
  Serial.print("%  T: ");
  Serial.print(t);
  Serial.print("°C Real tbs: ");
  Serial.print(Tbs);
  Serial.print("°C Tbh: ");
  Serial.print(Tbh);
  Serial.print("°C Tcov: ");
  Serial.print(Tcov);
  Serial.print("°C Texp: ");
  Serial.print(Texp);
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
    archivo.print(h);
    archivo.print(", ");
    archivo.print(Tbs);
    archivo.print(", ");
    archivo.print(Tbh);
    archivo.print(", ");
    archivo.print(Tcov);
    archivo.print(", ");
    archivo.println(Texp);
    archivo.close();
    Serial.println("Datos guardados en la tarjeta SD.");
  } else {
    Serial.println("No se pudo abrir el archivo.");
  }

  // Mostrar datos en la pantalla OLED
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
  display.print(Tbs);
  display.print("C");
  display.setTextSize(2);
  display.setCursor(0,32);
  display.print("Tbh:");
  display.print(Tbh);
  display.print("C");
  display.setTextSize(1);
  display.setCursor(0,48);
  display.print("Tcv:");
  display.print(Tcov);
  display.print("C Tep:");
  display.print(Texp);
  display.print("C");
  display.setTextSize(1);
  display.setCursor(0,56);
  display.print("Luis Javier Luis Luis");
  display.display();
  // Esperar 2 segundos antes de la siguiente lectura
  delay(2000);
}
