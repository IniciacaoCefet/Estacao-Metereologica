
#include "DHT.h" //Biblioteca sensor de umidade e temperatura
#include <Adafruit_BMP085.h> // Biblioteca de pressão atmosférica
#include <Wire.h>

#define DHTPIN A1 //Pino analógico que o sensor de umidade e temperatura está conectado
#define DHTTYPE DHT11 //definindo o sensor de umidade DHT 11
DHT dht(DHTPIN, DHTTYPE); //Definindo o nome do sensor de umidade e temperatura 
Adafruit_BMP085 bmp180; // Definindo o nome do sensor de pressão atmosférica


#define pino_analogico_solo A0 //Definindo o pino analógico do sensor umidade do solo

int valor_analogico; //Variável de leitura do sensor de umidade do solo
int pino_d = 7; //Definindo o pino digital do sensor de preciptação da chuva
int pino_a = A3; //Definindo o pino analógico do sensor de preciptação da chuva
int sensor_vento =2; //Definindo o pino digital do sensor que faz a leitura do anemômetro



void setup() {
  Serial.begin(9600); //Inicia o serial monitor
  Serial.println("DHTxx test!");
  dht.begin(); //Inicia o sensor de umidade e temperatura
  pinMode(pino_analogico_solo, INPUT); //Define a porta do sensor (unmidade solo)como entrada de dados
  pinMode(pino_d, INPUT); // Define a porta do sensor(Chuva - digital) como entrada de dados
  pinMode(pino_a, INPUT); //Define a porta do sensor (Chuva - analógico) como entrada de dados
  pinMode(sensor_vento, INPUT); // Define a porta do sensor anemômetro como entrada de dados
  digitalWrite(sensor_vento, HIGH); //Liga o sensor anemômetro
  if(!bmp180.begin()){
    Serial.println("Sensor não encontrado!!");
    while(1){}
  }
  

}

void loop() {
  // put your main code here, to run repeatedly:

}

void Temperatura(){
  float temperatura = dht.readTemperature(); //Faz a leitura da temperatura do ambiente
  
  //Confere se o sensor foi encontrado
  if(isnan(temperatura)){ 
    Serial.println("Sensor de temperatura não encontrado");
  }else{
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" *C");
  }
}

void Umidade(){
  float umidade = dht.readHumidity(); //Faz a leitura da umidade do ambiente

  //Confere se o sensor foi encontrado
  if(isnan(umidade)){
    Serial.println("Sensor de umidade não encontrado");
  }else{
    Serial.print("Umidade: ");
    Serial.print(umidade);
    Serial.println(" %t");
  }
}

void UmidadeSolo(){
  //Lê o valor do pino analógico
  valor_analogico = analogRead(pino_analogico_solo);

  //Mostra o valor da porta analogica no serial monitor
  Serial.print("Porta analogica: ");
  Serial.print(valor_analogico);

  if (valor_analogico > 0 && valor_analogico <= 400){
    Serial.println(" Status: Solo umido");
  } else if (valor_analogico > 400 && valor_analogico <= 800){
    Serial.println(" Status: Umidade moderada");
  } else if (valor_analogico > 800 && valor_analogico <= 1024){
    Serial.println(" Status: Solo seco");
  }
  delay(100);
  
}


void PrecptacaoChuva(){
  int val_d = digitalRead(pino_d); //Vatiavel de leitura digital do sensor de preciptação da chuva
  int val_a = analogRead(pino_a); //Variavel de leitura analógica do sensor de preciptação da chuva
  
  Serial.print("Valor digital : ");
  Serial.print(val_d);
  Serial.print(" - Valor analogico : ");
  Serial.println(val_a);
  
  //Confere a intensidade da chuva
  if (val_a >900 && val_a <= 1023){
    Serial.println("Não há chuva");
  }else if (val_a >600 && val_a <=900){
    Serial.println("Chuva fraca");
  }else if (val_a >400 && val_a <=600){
    Serial.println("Chuva moderada");
  }else if (val_a <400){
    Serial.println("Chuva forte");
  }    
  delay(1000);
  
}

void Anemometro(){
  //Constantes
  const float pi = 3.14159265;//Número de pi
  int periodo = 5000;//Tempo de medida(miliseconds)
  int intervalo = 2000;//Invervalo entre as amostras (miliseconds)
  int raio = 180;//Raio do anemometro(mm)

  unsigned int amostras  = 0;//Armazena o número de amostras
  unsigned int RPM = 0;//Rotações por minuto
  unsigned int cont = 0;//Contador para o sensor
  float velocidade_ms = 0;//Velocidade do vento (m/s)
  float velocidade_km = 0;//Velocidade do vento (km/h)

  amostras++;
  Serial.print(amostras);
  Serial.print(": Inicia Leitura...");
  
  //Medindo a velocidade do vento e contando as amostras
  attachInterrupt(0, Contador, RISING);
  unsigned long millis();
  long startTime = millis();
  while (millis() < startTime + periodo) {}
  Serial.println("   Finalizado.");
  Serial.print("Contador: ");
  Serial.print(cont);
  
  //Calculando o RPM (Rotações por minuto)
  
  RPM = ((cont) * 60) / (periodo / 1000);
  Serial.print(";  RPM: ");
  Serial.print(RPM);

  
  //Calculando a velocidade em m/s
  velocidade_ms = ((2 * pi * raio * RPM) / 60) / 1000;
  Serial.print(";  Vel. Vento: ");
  Serial.print(velocidade_ms);
  Serial.print(" [m/s] ");

  //Calculando a velocidade em km/h
  velocidade_km = (((2 * pi * raio * RPM) / 60) / 1000) * 3.6;
  Serial.print(velocidade_km);
  Serial.print(" [km/h] ");

  delay(intervalo);
}

void Contador(){
  unsigned int cont = 0;//Contador para o sensor
  cont++;
}

void Altitude(){
  float altitude = bmp180.readAltitude();
  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.println(" m");
  delay(3000);  
}

void Pressao(){
  float pressao = bmp180.readPressure();
  Serial.print("Pressão Atmosférica: ");
  Serial.print(pressao);
  Serial.println(" Pa");
  delay(3000);  
}
