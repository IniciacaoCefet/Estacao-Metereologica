
#include "DHT.h"                      //Biblioteca sensor de umidade e temperatura
#include <Adafruit_BMP085.h>          // Biblioteca de pressão atmosférica
#include <Wire.h>

#define DHTPIN A1                     //Pino analógico que o sensor de umidade e temperatura está conectado
#define DHTTYPE DHT11                 //definindo o sensor de umidade DHT 11
DHT dht(DHTPIN, DHTTYPE);             //Definindo o nome do sensor de umidade e temperatura 
Adafruit_BMP085 bmp180;               // Definindo o nome do sensor de pressão atmosférica


#define pino_analogico_solo A0        //Definindo o pino analógico do sensor umidade do solo

int valor_analogico;                  //Variável de leitura do sensor de umidade do solo
int pino_d = 7;                       //Definindo o pino digital do sensor de preciptação da chuva
int pino_a = A3;                      //Definindo o pino analógico do sensor de preciptação da chuva

const float pi = 3.14159265;          //Número de pi
int periodo = 5000;                   //Tempo de medida(miliseconds)
int delaytime = 2000;                 //Invervalo entre as amostras (miliseconds)
int raio = 180;                       //Raio do anemometro(mm)

// --- Variáveis Globais ---
unsigned int amostra  = 0;            //Armazena o número de amostras
unsigned int count = 0;               //Contador para o sensor
unsigned int RPM = 0;                 //Rotações por minuto
float velocidade_ms = 0;              //Velocidade do vento (m/s)
float velocidade_km = 0;              //Velocidade do vento (km/h)





void setup() {
  Serial.begin(9600);                 //Inicia o serial monitor
  Serial.println("DHTxx test!");
  dht.begin();                        //Inicia o sensor de umidade e temperatura
  pinMode(pino_analogico_solo, INPUT);//Define a porta do sensor (unmidade solo)como entrada de dados
  pinMode(pino_d, INPUT);             // Define a porta do sensor(Chuva - digital) como entrada de dados
  pinMode(pino_a, INPUT);             //Define a porta do sensor (Chuva - analógico) como entrada de dados
  pinMode(2, INPUT);                  // Define a porta do sensor anemômetro como entrada de dados
  digitalWrite(2, HIGH);              //Liga o sensor anemômetro
  if(!bmp180.begin()){                //Confere se o sensor de pressão está funcionando
    Serial.println("Sensor não encontrado!!");
    while(1){}
  }

}

void loop() {


  Temperatura();                      //Chama função que mede a Temperatura do ambiente
  Umidade();                          //Chama função que mede a Umidade do ambiente
  UmidadeSolo();                      //Chama função que mede a Umidade do solo
  Altitude();                         //Chama função que mede a Altitude
  Pressao();                          //Chama função que mede a pressão atmosférica
  PrecptacaoChuva();                  //Chama função que identifica a chuva
  Anemometro();                       //Chama função que mede a velocidade do vento
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  
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
  //Fazer a função map futuramente !!IMPORTANTE!!
}


void PrecptacaoChuva(){
  int val_d = digitalRead(pino_d);                //Variavel de leitura digital do sensor de preciptação da chuva
  int val_a = analogRead(pino_a);                 //Variavel de leitura analógica do sensor de preciptação da chuva
  
  Serial.print("Valor digital : ");               //Posso tirar - Era só para saber se estava ok !!IMPORTANTE!!
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

  //Acrescentar o sensor ultrassonico para medir a quantidade de chuva !!IMPORTANTE!!
}

void Anemometro(){

  amostra++; //Posso retirar depois - era so para conferir a contagem !! IMPORTANTE !!
  Serial.print(amostra);
  Serial.print(": Inicia Leitura...");
  Velocidade_Vento();
  Serial.println("   Finalizado.");
  Serial.print("Contador: ");
  Serial.print(count);
  Serial.print(";  RPM: ");
  RPMcalc();
  Serial.println(RPM);
  Serial.print("Velocidade do Vento: ");

  
  Velocidade_MS();
  Serial.print(velocidade_ms);
  Serial.print(" [m/s] ");


  Velocidade_KM();
  Serial.print(velocidade_km);
  Serial.print(" [km/h] ");
  Serial.println();

  delay(delaytime);  
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
  Serial.print("Pressão Atmosférica(Pa): ");
  Serial.print(pressao);
  Serial.println(" Pa");
  pressao = pressao / 100;
  Serial.print("Pressão Atmosférica(hPa): ");
  Serial.print(pressao);
  Serial.println(" hPa");
  delay(3000);  
}


void Velocidade_Vento() {
  velocidade_ms = 0;
  velocidade_km = 0;

  count = 0;
  attachInterrupt(0, addcount, RISING);
  unsigned long millis();
  long startTime = millis();
  while (millis() < startTime + periodo) {}
}


void RPMcalc() {
  RPM = ((count) * 60) / (periodo / 1000); // Calcula a rotação por minuto (RPM)
}



void Velocidade_MS() {
  velocidade_ms = ((2 * pi * raio * RPM) / 60) / 1000; //Calcula a velocidade do vento em m/s
} 



void Velocidade_KM() {
  velocidade_km = (((2 * pi * raio * RPM) / 60) / 1000) * 3.6; //Calcula velocidade do vento em km/h
}



void addcount() {
  count++;
}
