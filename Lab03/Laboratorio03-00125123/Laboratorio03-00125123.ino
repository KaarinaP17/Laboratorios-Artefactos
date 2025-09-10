//Importar las librerias a utilizar
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
//Inicializa la configuración de la pantalla
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); 

float LM35 = A1;
float lect = 0;
float temp = 0;

const int pinLM35 = A0;            // pin analógico donde está conectado LM35 Vout
const unsigned long intervalo = 1000; // intervalo de actualización en ms
unsigned long ultimaMillis = 0;

void setup()   {
  Serial.begin(9600);
  pinMode(LM35, INPUT);


  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
  display.setContrast (255); // dim display
 
  // Clear the buffer.
  display.clearDisplay();
  
  // Save to update screen changes
  display.display();
}

float leer(){
  unsigned long ahora = millis();
  float tempC;
  if (ahora - ultimaMillis >= intervalo) {
    ultimaMillis = ahora;

    int lectura = analogRead(pinLM35);              // 0..1023
    const float Vref = 5.0;                         // ajustar si tu referencia ADC es distinta
    float voltaje = lectura * (Vref / 1023.0); // voltaje en V
    tempC = voltaje * 100.0;    
    // Imprime en formato legible
    Serial.print("Temperatura: ");
    Serial.print(tempC, 2);   // 2 decimales
    Serial.println(" °C");
  }
  return tempC;
}

void loop() {
  float lect = analogRead(LM35);

  temp = ((lect * 5.0)/1024)*100;
  Serial.println(lect);
  //set text size, 1 es 8px, 2 es 16px, 3, es 32px, etc.
  display.setTextSize(1); 
  //Set text color define el color del texto de la pantalla
  display.setTextColor(SH110X_WHITE); 
  //Set cursor coloca el cursor en donde se va a comenzar escribir (px ancho, px alto)
  display.setCursor(20, 0);
  //Println permite escribir en la posicion en la que se encuentra el cursor
  display.println("Temperatura ");

  //Para imprimir otro dato, se coloca nuevamente el cursor bajando la columna
  display.setCursor(20, 30);
  display.println(leer());
  delay(1000);
  
  //Para guardar los cambios hechos en la pantalla
  display.display();
  
  //Se borra la pantalla antes de finalizar el loop para que no se monten los datos
  display.clearDisplay();
}
