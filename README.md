# SemanaCiencia23CREA

Hecho con mucho love <3 por estudiantes pertenecientes al CREA (Club de Robótica, Electrónica y Automática) de la Escuela Ténica Superior de Ingeniería y Diseño Industrial de la Universidad Politécnica de Madrid.

**Autores:** 
+ Alejandro Cinque Rivas - Director, Diseño y Revisor del Código
+ Celia Saez de la Peña - Diseño
+ Yu Wang Wu - Código

**Proyecto:** Taller Semana de la Ciencia 2023

**Placa objetivo:** Arduino Nano

**Controlador:** ATMega328p

**Descripcion del proyecto:** 
Maceta inteligente controlado por un Arduino nano con un LDR, BMP180 y sensor capacitivo de humedad de tierra.
Muestra en una serie LEDs en el intervalo que se encuentra de humedad de tierra, luz y temperatura ambiente además de dar un resumen de una pantalla de matriz de LEDs con símbolos.

**Agradecimientos:** 
+ Carolina Tank
+ Gema Sánchez
+ Pablo Campoy
+ Paula Ramos
+ Jorge González 
+ Máximo Rivero
+ Gianmarco Barcelloni-Corte
+ Mateo Berizzo
+ Marcelo Luna
+ Y un largo etc pero la lista ya se está haciendo muy larga <3
## Material 

Material necesario para realizar el proyecto:

- Microcontrolador: 
    - Arduino Nano (o placa compatible con ATMega328p).
- Sensores: 
    - Módulo BMP180 I2C.
    - Sensor de humedad de tierra capacitivo, lectura analógica.
    - LDR.
- Actuadores: 
    - Pantalla matriz 8x8 LEDs con MAX7219xxx.
    - 6 LEDs rojos.
    - 3 LEDs verdes.
- Circuito:
    - Cables Du-Pont H-H, H-M y M-M.
    - Resistencia 10k Ohmios.
    - Resistencia 300 Ohmios.
    - Protoboard pequeña.

## Montaje del circuito

## Definiciones y variables del programa

En primera lugar se están incluyendo las librerías necesarias para que nuestro código funcione, tenemos que asegurarnos de que tenemos instaladas las librerias de LedControl y Adafruit_BMP085 en nuestro Arduino IDE para que nuestro código pueda compilar, si no están instaladas dará error. 

> [!WARNING]
> Las librerias de LedControl y Adafruit_BMP085 son esenciales para que se pueda compilar el programa.

La siguiente parte del código es la definición de los pines, aquí se le describe al programa dónde están conectados los componentes. Estas definiciones son válidas para las conexiones que se hacen en la guía de montaje. Las __macros__ normalmente tienen la siguiente sintaxis (en donde el apartado misc puede o no existir):

```
PIN_componente_misc_misc
```

El siguiente apartado son las funciones propias que se han definido para este programa. En lenguaje C es necesario definir el prototipo de la función (esto), pero en C++ y en la versión de arduino de este lenguaje no es necesario aunque lo incluyamos de todas maneras.

Entrando a la parte de las variables globales, hay un bloque que está marcado como NO EDITABLE y otro como EDITABLE. El bloque no editable contiene constantes que el programa necesita para funcionar e iniciaciones de variables. Las variables tienen la siguiente sintaxis:

```
VALOR_parámetro_misc
```

En la parte de variables globales EDITABLE es importante que definamos valores para los sensores específicos que tengamos ya que de momento se han puesto valores de prueba. Una vez que se sepan los datos que queremos poner simplemente se edita el número detrás del igual con cuidado de no borrar el puntocoma (;). También es posible editar el VALOR_RETARDO_LECTURA que define cada cuánto lee datos nuevos de los sensores.

```
<   RANGO LUZ ROJA    |    RANGO LUZ VERDE    |    RANGO LUZ ROJA   >
                      ^                       ^
            VALOR_parámetro_BAJA    VALOR_parámetro_ALTA
```
> [!IMPORTANT]
> Es importante que VALOR_parámetro_BAJA sea menor que VALOR_parámetro_ALTA, de la otra forma no tendría sentido y si no se definen bien aparecerá un mensaje de error en el monitor serial además de la imagen de error en la matriz de LEDs.

Yendo al siguiente bloque, dibujos para la pantalla, es EDITABLE y contiene una vector de vectores. Son "imágenes" para mostrarse en la pantalla de matriz de LEDs y hay 5 en total. Podemos ir a una página para editar estas imágenes o también editarlas a mano, un 0 es un led apagado y un 1 es un led encendido con las comas separando las columnas. 0b al principio simplemente indica que se está definiendo como binario.

### Cambiar las imágenes de la pantalla de matriz de LEDs

## Rutina del programa

Cuando inicia el programa se inicia la comunicación serial (a una velocidad de 9600 baud), se le dan parámetros de funcionamiento a la pantalla, se comprueba que haya conexion con el sensor BMP180 y se comprueban que las variables de VALOR_parámetro_misc tengan sentido. Si alguna de estas comprobaciones falla, el programa no continua y se puede leer en el monitor serial que es lo que ha fallado.

Después se entra al loop que se repite cada vez que llega al final:

1. Esperar el tiempo de retardo especificado en VALOR_RETARDO_LECTURA
2. Se leen los datos de los sensores y se guardan en sus respectivas variables con la función obtenerDatosSensores().
3. Comprobación de en que rango caen los valores de humedad de tierra, luz y temperatura de acuerdo a lo especificado por el usuario en VALOR_parámetro_BAJA/ALTA.
    + Si la comprobación determina que se sale del rango de luz verde marca la variable VALOR_PROBLEMA_parámetro como 1 y si está dentro del rango verde, como 0.
4. Se suman todos los VALOR_PROBLEMA_parámetro para obtener los problemas totales.
5. Dependiendo del número de problemas se dibuja en la pantalla de matriz de LEDs
    + Por defecto, si hay 0 problemas se pone la cara feliz, si hay 1 problema se pone la cara indiferente y si hay 2 o 3 problemas se pone la cara triste. Si se ha calculado un número diferente a 0, 1, 2 o 3 significa que hay un error y cae en el caso default de dibujar la pantalla de error.
6. Se imprimen todos los datos que se han leido de los sensores en el monitor serial.
