#include <Adafruit_BMP085.h>

#define PIN_HUMEDADTIERRA         A3
#define PIN_LDR                   A7

#define PIN_BAROMETRO_SCL         A5
#define PIN_BAROMETRO_SDA         A4

#define PIN_MATRIZLED_DIN         D11
#define PIN_MATRIZLED_CS          D10
#define PIN_MATRIZLED_CLK         D13

#define PIN_LED_TEMPERATURA_BAJA  D3 
#define PIN_LED_TEMPERATURA_OK    D2
#define PIN_LED_TEMPERATURA_ALTA  D1 

#define PIN_LED_LUZ_BAJA          D6
#define PIN_LED_LUZ_OK            D5
#define PIN_LED_LUZ_ALTA          D4

#define PIN_LED_HUMEDAD_BAJA      D9
#define PIN_LED_HUMEDAD_OK        D8
#define PIN_LED_HUMEDAD_ALTA      D7

Adafruit_BMP085 bmp;

int VALOR_HUMEDADTIERRA;
int VALOR_LDR;
int VALOR_BAROMETRO;
int VALOR_TEMPERATURA;

void obtenerDatosSensores();

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);

  if (!bmp.begin()) {
	  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	  while (1) {}
  }

  Serial.println("***********************************************");
  Serial.println("*************Comienzo del programa*************");
  Serial.println("***********************************************");

}

void loop() {
  // put your main code here, to run repeatedly:

  obtenerDatosSensores();

  Serial.println("******************************");
  Serial.print("Valor humedad tierra: ");
  Serial.println(VALOR_HUMEDADTIERRA);
  Serial.print("Valor LDR: ");
  Serial.println(VALOR_LDR);
  Serial.print("Valor barometro: ");
  Serial.println(VALOR_BAROMETRO);
  Serial.print("Valor temperatura: ");
  Serial.println(VALOR_TEMPERATURA);
  Serial.println("******************************");

  delay(2000);

}

void obtenerDatosSensores() {
  VALOR_HUMEDADTIERRA = analogRead(PIN_HUMEDADTIERRA);
  VALOR_LDR = analogRead(PIN_LDR);
  VALOR_BAROMETRO = bmp.readPressure();
  VALOR_TEMPERATURA = bmp.readTemperature();
}
