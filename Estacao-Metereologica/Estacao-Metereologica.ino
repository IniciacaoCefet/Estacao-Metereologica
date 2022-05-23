#include "DHT.h"
 
#define DHTPIN A1 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11
#define pino_sinal_analogico A0
#define pino_led_vermelho 5
#define pino_led_amarelo 6
#define pino_led_verde 7
#include <Wire.h>
#include <Adafruit_BMP085.h>
Adafruit_BMP085 bmp180;
 
 
int valor_analogico;
int pino_d = 7; //Pino ligado ao D0 do sensor
int pino_a = A3; //Pino ligado ao A0 do sensor
int val_d = 0; //Armazena o valor lido do pino digital
int val_a = 0; //Armazena o valor lido do pino analogico

// --- Constantes ---
const float pi = 3.14159265;     //Número de pi
int period = 5000;               //Tempo de medida(miliseconds)
int delaytime = 2000;            //Invervalo entre as amostras (miliseconds)
int radius = 180;                //Raio do anemometro(mm)

// --- Variáveis Globais ---
unsigned int Sample  = 0;        //Armazena o número de amostras
unsigned int counter = 0;        //Contador para o sensor
unsigned int RPM = 0;            //Rotações por minuto
float speedwind = 0;             //Velocidade do vento (m/s)
float windspeed = 0;             //Velocidade do vento (km/h)

 
// Conecte pino 1 do sensor (esquerda) ao +5V
// Conecte pino 2 do sensor ao pino de dados definido em seu Arduino
// Conecte pino 4 do sensor ao GND
// Conecte o resistor de 10K entre pin 2 (dados) 
// e ao pino 1 (VCC) do sensor
DHT dht(DHTPIN, DHTTYPE);
 
void setup() 
{
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  dht.begin();
  pinMode(pino_sinal_analogico, INPUT);
  pinMode(pino_led_vermelho, OUTPUT);
  pinMode(pino_led_amarelo, OUTPUT);
  pinMode(pino_led_verde, OUTPUT);
  pinMode(pino_d, INPUT);
  pinMode(pino_a, INPUT);
  pinMode(2, INPUT);        //configura o digital 2 como entrada
  digitalWrite(2, HIGH);    //internall pull-up active
  if (!bmp180.begin()) 
  {
    Serial.println("Sensor nao encontrado !!");
    while (1) {}
  }

}
 
void loop() 
{
  // A leitura da temperatura e umidade pode levar 250ms!
  // O atraso do sensor pode chegar a 2 segundos.
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // testa se retorno é valido, caso contrário algo está errado.
  if (isnan(t) || isnan(h)) 
  {
    Serial.println("Failed to read from DHT");
  } 
  else
  {
    Serial.print("Umidade: ");
    Serial.print(h);
    Serial.print(" %t");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" *C");
  }

  //Le o valor do pino A0 do sensor
  valor_analogico = analogRead(pino_sinal_analogico);
 
  //Mostra o valor da porta analogica no serial monitor
  Serial.print("Porta analogica: ");
  Serial.print(valor_analogico);
 
  //Solo umido, acende o led verde
  if (valor_analogico > 0 && valor_analogico < 400)
  {
    Serial.println(" Status: Solo umido");
    apagaleds();
    digitalWrite(pino_led_verde, HIGH);
  }
 
  //Solo com umidade moderada, acende led amarelo
  if (valor_analogico > 400 && valor_analogico < 800)
  {
    Serial.println(" Status: Umidade moderada");
    apagaleds();
    digitalWrite(pino_led_amarelo, HIGH);
  }
 
  //Solo seco, acende led vermelho
  if (valor_analogico > 800 && valor_analogico < 1024)
  {
    Serial.println(" Status: Solo seco");
    apagaleds();
    digitalWrite(pino_led_vermelho, HIGH);
  }
  delay(100);

  //Le e arnazena o valor do pino digital
  val_d = digitalRead(pino_d);
  //Le e armazena o valor do pino analogico
  val_a = analogRead(pino_a);
  //Envia as informacoes para o serial monitor
  Serial.print("Valor digital : ");
  Serial.print(val_d);
  Serial.print(" - Valor analogico : ");
  Serial.println(val_a);

  Sample++;
  Serial.print(Sample);
  Serial.print(": Inicia Leitura...");
  windvelocity();
  Serial.println("   Finalizado.");
  Serial.print("Contador: ");
  Serial.print(counter);
  Serial.print(";  RPM: ");
  RPMcalc();
  Serial.print(RPM);
  Serial.print(";  Vel. Vento: ");

  //*****************************************************************
  //print m/s
  WindSpeed();
  Serial.print(windspeed);
  Serial.print(" [m/s] ");

  //*****************************************************************
  //print km/h
  SpeedWind();
  Serial.print(speedwind);
  Serial.print(" [km/h] ");
  Serial.println();

  delay(delaytime); 
   
  Serial.print("Temperatura : ");
   if ( bmp180.readTemperature() < 10)
   {
     Serial.print(bmp180.readTemperature());
     Serial.println(" C");
   }
   else
   {
     Serial.print(bmp180.readTemperature(),1);
     Serial.println(" C");
   }
 
     
     Serial.print("Altitude : ");
     Serial.print(bmp180.readAltitude());
     Serial.println(" m");
    
   
     Serial.print("Pressao : ");
     Serial.print(bmp180.readPressure());  
     Serial.println(" Pa");
    
   delay(3000);  
}

void apagaleds()
{
  digitalWrite(pino_led_vermelho, LOW);
  digitalWrite(pino_led_amarelo, LOW);
  digitalWrite(pino_led_verde, LOW);
}

//Função para medir velocidade do vento
void windvelocity() {
  speedwind = 0;
  windspeed = 0;

  counter = 0;
  attachInterrupt(0, addcount, RISING);
  unsigned long millis();
  long startTime = millis();
  while (millis() < startTime + period) {}
}

//Função para calcular o RPM
void RPMcalc() {
  RPM = ((counter) * 60) / (period / 1000); // Calculate revolutions per minute (RPM)
}

//Velocidade do vento em m/s
void WindSpeed() {
  windspeed = ((2 * pi * radius * RPM) / 60) / 1000; //Calcula a velocidade do vento em m/s
} //end WindSpeed

//Velocidade do vento em km/h
void SpeedWind() {
  speedwind = (((2 * pi * radius * RPM) / 60) / 1000) * 3.6; //Calcula velocidade do vento em km/h
} //end SpeedWind

//Incrementa contador
void addcount() {
  counter++;
}
