/* 
* Copyright 2023 CREA ETSIDI-UPM
*
* Autor: Yu Wang Wu para CREA ETSIDI-UPM
* Proyecto: Taller Semana de la Ciencia 2023
* Placa objetivo: Arduino Nano
* Controlador: ATMega328p
*
* Descripcion: 
* Maceta inteligente controlado por un arduino 
* nano con un LDR, BMP180 y sensor capacitivo 
* de humedad de tierra. Muestra en una serie de 
* LEDs los niveles de humedad de la tierra, 
* temperatura ambiente y luz que recibe en el 
* momento y tambien un dibujo en una pantalla 
* de matriz de LEDs como resumen de la situacion.
*/

// Librerias externas necesarias
// NO EDITABLE
#include <LedControl.h>
#include <Adafruit_BMP085.h>

// Definicion de pines
// NO EDITABLE (SALVO QUE OS LO DIGAN)
#define PIN_HUMEDADTIERRA         A3
#define PIN_LDR                   A7

#define PIN_BAROMETRO_SCL         A5    // No es necesaria esta definicion
#define PIN_BAROMETRO_SDA         A4    // No es necesaria esta definicion

#define PIN_MATRIZLED_DIN         11
#define PIN_MATRIZLED_CS          10
#define PIN_MATRIZLED_CLK         13

#define PIN_LED_TEMPERATURA_BAJA  3 
#define PIN_LED_TEMPERATURA_OK    2
#define PIN_LED_TEMPERATURA_ALTA  1 

#define PIN_LED_LUZ_BAJA          6
#define PIN_LED_LUZ_OK            5
#define PIN_LED_LUZ_ALTA          4

#define PIN_LED_HUMEDAD_BAJA      9
#define PIN_LED_HUMEDAD_OK        8
#define PIN_LED_HUMEDAD_ALTA      7

// Funciones propias
// NO EDITABLE
void obtenerDatosSensores();
void dibujarEstado(int parametro, bool estado[]);
void dibujarPantalla(int dibujo[]);

// Definiciones de comunicaciones con componentes
// NO EDITABLE
Adafruit_BMP085 termometro;
LedControl pantalla = LedControl(PIN_MATRIZLED_DIN, PIN_MATRIZLED_CLK, PIN_MATRIZLED_CS, 0);

// Variables globales
// NO EDITABLE
const bool ESTADO_POCO[] = {1, 0, 0};
const bool ESTADO_JUSTO[] = {0, 1, 0};
const bool ESTADO_MUCHO[] = {0, 0, 1};
const int humedad = 0;
const int luz = 1;
const int temperatura = 2;
int VALOR_PROBLEMA_HUMEDAD = 0;
int VALOR_PROBLEMA_LUZ = 0;
int VALOR_PROBLEMA_TEMPERATURA = 0;
int VALOR_PROBLEMAS = 0;
int VALOR_HUMEDADTIERRA;
int VALOR_LDR;
int VALOR_BAROMETRO;
int VALOR_TEMPERATURA;

// Variables globales
// EDITABLE
const int VALOR_HUMEDAD_BAJA = 100        ;
const int VALOR_HUMEDAD_ALTA = 200        ;
const int VALOR_LUZ_BAJA = 5              ;
const int VALOR_LUZ_ALTA = 10             ;
const int VALOR_TEMPERATURA_BAJA = 15     ;
const int VALOR_TEMPERATURA_ALTA = 25     ;
const int VALOR_RETARDO_LECTURA = 2000    ;

// Dibujos para la pantalla
// Orden de los dibujos:
//    0.- Todo OK
//    1.- Meh
//    2.- Mal
//    3.- Error
//    4.- CREA
// EDITABLE
const int dibujos[][8] = {
{
  0b00111100,
  0b01000010,
  0b10100101,
  0b10000001,
  0b10100101,
  0b10011001,
  0b01000010,
  0b00111100
},{
  0b00111100,
  0b01000010,
  0b10100101,
  0b10000001,
  0b10111101,
  0b10000001,
  0b01000010,
  0b00111100
},{
  0b00111100,
  0b01000010,
  0b10100101,
  0b10000001,
  0b10011001,
  0b10100101,
  0b01000010,
  0b00111100
},{
  0b11111011,
  0b11111011,
  0b11000011,
  0b11110011,
  0b11110011,
  0b11000000,
  0b11111011,
  0b11111011
},{
  0b00111100,
  0b01011010,
  0b00111100,
  0b10011001,
  0b11000011,
  0b11111111,
  0b01111110,
  0b10111101
}};

// Definicion nombre dibujos
// NO EDITABLE
const int DIBUJO_CARA_FELIZ = dibujos[0];
const int DIBUJO_CARA_MEH = dibujos[1];
const int DIBUJO_CARA_TRISTE = dibujos[2];
const int DIBUJO_ERROR = dibujos[3];
const int DIBUJO_CREA = dibujos[4];

/**********************************************************************************/
// Comienzo del programa
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Setup pantalla
  pantalla.shutdown(0, false);
  pantalla.setIntensity(0, 0);
  pantalla.clearDisplay(0);

  // Placeholder hehe
  dibujarPantalla(DIBUJO_CREA);

  // Comprobacion de conexion BMP180
  if (!termometro.begin()) {
	  Serial.println("No se ha podido encontrar el BMP180, revisa las conexiones!");
    dibujarPantalla(DIBUJO_ERROR);
	  while (1) {}
  }

  // Comprobacion de si los datos para los parametros tienen sentido
  if (VALOR_HUMEDAD_BAJA >= VALOR_HUMEDAD_ALTA) {
    Serial.println("Revise los valores introducidos para la humedad!");
    dibujarPantalla(DIBUJO_ERROR);
    while (1) {}
  }
  if (VALOR_LUZ_BAJA >= VALOR_LUZ_ALTA) {
    Serial.println("Revise los valores introducidos para la luz!");
    dibujarPantalla(DIBUJO_ERROR);
    while (1) {}
  }
  if (VALOR_TEMPERATURA_BAJA >= VALOR_TEMPERATURA_ALTA) {
    Serial.println("Revise los valores introducidos para la temperatura!");
    dibujarPantalla(DIBUJO_ERROR);
    while (1) {}
  }


  Serial.println("***********************************************");
  Serial.println("*************Comienzo del programa*************");
  Serial.println("***********************************************");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(VALOR_RETARDO_LECTURA);

  obtenerDatosSensores();

  // Comprobacion de estado humedad
  if (VALOR_HUMEDADTIERRA < VALOR_HUMEDAD_BAJA) {
    dibujarEstado(humedad, ESTADO_POCO);
    VALOR_PROBLEMA_HUMEDAD = 1;
  } else if (VALOR_HUMEDADTIERRA > VALOR_HUMEDAD_ALTA) {
    dibujarEstado(humedad, ESTADO_MUCHO);
    VALOR_PROBLEMA_HUMEDAD = 1;
  } else {
    dibujarEstado(humedad, ESTADO_JUSTO);
    VALOR_PROBLEMA_HUMEDAD = 0;
  }

  // Comprobacion de estado luz
  if (VALOR_LDR < VALOR_LUZ_BAJA) {
    dibujarEstado(luz, ESTADO_POCO);
    VALOR_PROBLEMA_LUZ = 1;
  } else if (VALOR_LDR > VALOR_LUZ_ALTA) {
    dibujarEstado(luz, ESTADO_MUCHO);
    VALOR_PROBLEMA_LUZ = 1;
  } else {
    dibujarEstado(luz, ESTADO_JUSTO);
    VALOR_PROBLEMA_LUZ = 0;
  }

  // Comprobacion de estado temperatura
  if (VALOR_TEMPERATURA < VALOR_TEMPERATURA_BAJA) {
    dibujarEstado(temperatura, ESTADO_POCO);
    VALOR_PROBLEMA_TEMPERATURA = 1;
  } else if (VALOR_TEMPERATURA > VALOR_TEMPERATURA_ALTA) {
    dibujarEstado(temperatura, ESTADO_MUCHO);
    VALOR_PROBLEMA_TEMPERATURA = 1;
  } else {
    dibujarEstado(temperatura, ESTADO_JUSTO);
    VALOR_PROBLEMA_TEMPERATURA = 0;
  }

  // Obtencion de problemas totales
  VALOR_PROBLEMAS = VALOR_PROBLEMA_HUMEDAD + VALOR_PROBLEMA_LUZ + VALOR_PROBLEMA_TEMPERATURA;

  // Dibujo en matriz de LEDs dependiendo de problemas totales
  switch (VALOR_PROBLEMAS) {
    case 0:
      dibujarPantalla(DIBUJO_CARA_FELIZ);
      break;
    
    case 1:
      dibujarPantalla(DIBUJO_CARA_MEH);
      break;

    case 2:
      dibujarPantalla(DIBUJO_CARA_TRISTE);
      break;

    case 3:
      dibujarPantalla(DIBUJO_CARA_TRISTE);
      break;

    default:
      dibujarPantalla(DIBUJO_ERROR);
      break;
  }

  // Imprimir estado de todos los sensores
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
  Serial.print("Problema humedad: ");
  Serial.println(VALOR_PROBLEMA_HUMEDAD);
  Serial.print("Problema luz: ");
  Serial.println(VALOR_PROBLEMA_LUZ);
  Serial.print("Problema temperatura: ");
  Serial.println(VALOR_PROBLEMA_TEMPERATURA);
  Serial.print("PROBLEMAS TOTALES: ");
  Serial.println(VALOR_PROBLEMAS);
  Serial.println("******************************");
  Serial.println("");
}


// Zona funciones especificas

// Lee sensores y guarda los datos en sus variables
void obtenerDatosSensores() {
  VALOR_HUMEDADTIERRA = analogRead(PIN_HUMEDADTIERRA);
  VALOR_LDR = analogRead(PIN_LDR);
  VALOR_BAROMETRO = termometro.readPressure();
  VALOR_TEMPERATURA = termometro.readTemperature();
}

// Enciende y apaga los LEDs de estado
// Valores parametro posibles: humedad (const 0), luz (const 1) o temperatura (const 2)
// Valores estado[] posibles: ESTADO_POCO, ESTADO_JUSTO o ESTADO_MUCHO
void dibujarEstado(int parametro, bool estado[]) {
  switch (parametro) {
    case 0:
      digitalWrite(PIN_LED_HUMEDAD_BAJA, estado[0]);
      digitalWrite(PIN_LED_HUMEDAD_OK, estado[1]);
      digitalWrite(PIN_LED_HUMEDAD_ALTA, estado[2]);
      break;

    case 1:
      digitalWrite(PIN_LED_LUZ_BAJA, estado[0]);
      digitalWrite(PIN_LED_LUZ_OK, estado[1]);
      digitalWrite(PIN_LED_LUZ_ALTA, estado[2]);
      break;

    case 2:
      digitalWrite(PIN_LED_TEMPERATURA_BAJA, estado[0]);
      digitalWrite(PIN_LED_TEMPERATURA_OK, estado[1]);
      digitalWrite(PIN_LED_TEMPERATURA_ALTA, estado[2]);
      break;

    default:
      dibujarPantalla(DIBUJO_ERROR);
      Serial.println("Error escribiendo un nuevo estado a LEDs de estado.");
      break;
  }
}

// Cambia la imagen en la matriz de LEDs
// Valores dibujo[] posibles: DIBUJO_CARA_FELIZ, DIBUJO_CARA_MEH, DIBUJO_CARA_TRISTE,
//                            DIBUJO_ERROR, DIBUJO_CREA
void dibujarPantalla(int dibujo[]) {
  pantalla.clearDisplay(0);
  for (int i = 0; i < 8; i++) {
    pantalla.setRow(0, i, dibujo[i]);
  }
}
