// Pines utilizados
#define LEDPir 9     // LED de encendido del PIR
#define LEDSensor 12      // LED de encendido del Sensor Ultra sonido

#define LEDVerde 10       // LED de parqueo libre
#define LEDRojo 11        // LED de parqueo no disponible

#define PIRPin 5       // pin de entrada (for PIR sensor)
#define TRIGGER 6           // pin de entrada Trigger del Sensor Ultrasonido
#define ECHO 7            // pin de entrada Echo del Sensor Ultrasonido

//Constantes para el sensor PIR
int pirState = LOW;           // de inicio no hay movimiento en el PIR
int val = 0;                  // estado del pin

// Constantes para el sensor de Ultra sonido
const float sonido = 34300.0; // Velocidad del sonido en cm/s
const float umbral1 = 30.0; // Umbral de distancia más lejana, apenas se detecta el auto.
const float umbral2 = 20.0; // Umbral de distancia media, utilizado para alertas de proximidad.
const float umbral3 = 10.0; // Umbral de distancia corta, utilizado para alertas de proximidad.

void setup() {
  //Iniciar el monitor serie
  Serial.begin(9600);

  // Modo entrada/salida de los pines
  pinMode(LEDPir, OUTPUT);
  pinMode(LEDSensor, OUTPUT);
  pinMode(LEDVerde, OUTPUT);
  pinMode(LEDRojo, OUTPUT);
  pinMode(PIRPin, INPUT);
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);

  // Apagamos todos los LEDs
  apagarLEDs();
}

void loop() {

  val = LOW;
  val = digitalRead(PIRPin);

  //Si está activado el sensor PIR
  if (val == HIGH) {

    //Encienda el LED de indicador de PIR
    digitalWrite(LEDPir, HIGH);

    Serial.println("Sensor activado");
    
    digitalWrite(LEDSensor, HIGH);
    //Se inicia el sensor de Ultrasonido

    // Preparamos el sensor de ultrasonidos
    iniciarTrigger();

    // Obtenemos la distancia
    float distancia = calcularDistancia();

    // Apagamos todos los LEDs de parqueo
    digitalWrite(LEDVerde, LOW);
    digitalWrite(LEDRojo, LOW);

    /* Si el sensor detecta que el objeto se encuentra en el umbral de parqueo*/
    if (distancia < umbral1) {
      // Revisamos el estado de los leds y alertas
      parqueo(distancia);
    } else {
      //Si no, es porque el parqueadero esta libre
      digitalWrite(LEDVerde, HIGH);
      digitalWrite(LEDRojo, LOW);
      digitalWrite(LEDSensor, LOW);

    }
  }

  //Si por el contrario el sensor está desactivado
  else {

    //LED del PIR desactivado
    digitalWrite(LEDPir, LOW);
    Serial.println("Sensor parado");

    
  }
}

//Metodo para apagar todos los LEDS
void apagarLEDs() {
  // Apagamos todos los LEDs
  digitalWrite(LEDPir, LOW);
  digitalWrite(LEDSensor, LOW);
  digitalWrite(LEDVerde, LOW);
  digitalWrite(LEDRojo, LOW);
}

// Método que inicia la secuencia del Trigger para comenzar a medir
void iniciarTrigger() {
  // Ponemos el Trigger en estado bajo y esperamos 2 ms
  digitalWrite(TRIGGER, LOW);
  delayMicroseconds(2);

  // Ponemos el pin Trigger a estado alto y esperamos 10 ms
  digitalWrite(TRIGGER, HIGH);
  delayMicroseconds(10);

  // Comenzamos poniendo el pin Trigger en estado bajo
  digitalWrite(TRIGGER, LOW);
}

// Función que comprueba las distancias de parqueo
/*const float umbral1 = 30.0;
  const float umbral2 = 20.0;
  const float umbral3 = 10.0; */
void parqueo(float distancia) {
  if (distancia < umbral1 && distancia >= umbral2) {
    // Encendemos el LED rojo de parqueo y apagamos el LED verde
    digitalWrite(LEDRojo, HIGH);
    digitalWrite(LEDVerde, LOW);

  } else if (distancia < umbral2 && distancia > umbral3) {
    // Encendemos el LED rojo de parqueo y apagamos el LED verde
    /*Esta opcion se deja en caso de querer añadir una alerta de proximidad al sensor*/
    digitalWrite(LEDRojo, HIGH);
    digitalWrite(LEDVerde, LOW);

  } else if (distancia <= umbral3) {
    // Encendemos el LED rojo de parqueo y apagamos el LED verde
    /*Esta opcion se deja en caso de querer añadir una alerta de proximidad al sensor*/
    digitalWrite(LEDRojo, HIGH);
    digitalWrite(LEDVerde, LOW);
  }
}

// Método que calcula la distancia a la que se encuentra un objeto.
// Devuelve una variable tipo float que contiene la distancia
float calcularDistancia() {
  // La función pulseIn obtiene el tiempo que tarda en cambiar entre estados, en este caso a HIGH
  unsigned long tiempo = pulseIn(ECHO, HIGH);

  // Obtenemos la distancia en cm, hay que convertir el tiempo en segundos ya que está en microsegundos
  // por eso se multiplica por 0.000001
  float distancia = tiempo * 0.000001 * sonido / 2.0;
  Serial.print(distancia);
  Serial.print("cm");
  Serial.println();
  delay(500);

  return distancia;
}
