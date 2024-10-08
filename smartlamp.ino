#include "DHT.h"

int ledPin = 23;        // Pino de saída para o LED
int ledValue = 10;     // Valor inicial do LED (0 a 100)

int ldrPin = 36;       // Pino de entrada para o LDR
int ldrMax = 4000;     // Valor máximo do LDR encontrado após testes

#define DHT_PIN 25
#define DHTTYPE DHT11

DHT dht(DHT_PIN, DHTTYPE);

float humidity, temperature;
String message;

void setup() {
    Serial.begin(9600);
    
    pinMode(ledPin, OUTPUT);
    pinMode(ldrPin, INPUT);

    // Inicializando o valor do LED com o valor inicial
    ledUpdate();  
}

// Função loop será executada infinitamente pelo ESP32
void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');  
        processCommand(command);  // Processa o comando recebido via serial
    }
}

// Função para processar comandos recebidos via serial
void processCommand(String command) {
    if (command.startsWith("SET_LED")) {
        int value = command.substring(8).toInt();  // Extrai o valor após o comando "SET_LED "
        if (value >= 0 && value <= 100) {
            ledValue = value;  // Atualiza o valor do LED
            ledUpdate();
            Serial.printf("RES SET_LED 1\n");  // Comando executado com sucesso
        } else {
            Serial.printf("RES SET_LED -1\n");  // Comando SET_LED inválido
        }
    } else if (command.equals("GET_LED")) {
        Serial.printf("RES GET_LED %d\n", ledValue);  // Retorna o valor atual do LED
        
    } else if (command.equals("GET_LDR")) {
        int ldrValue = ldrGetValue();
        Serial.printf("RES GET_LDR %d\n", ldrValue);  // Retorna o valor atual do LDR
        
    } else if (command.equals("GET_TEMP")) {
        temperature = dht.readTemperature();
        message = "RES GET_TEMP " + String(temperature, 2);
        Serial.println(message);  // Retorna o valor atual d
        
    }else if (command.equals("GET_HUM")) {
        humidity = dht.readHumidity();
        message = "RES GET_HUM " + String(humidity, 2);
        Serial.println(message);  // Retorna o valor atual d
    } else {
        Serial.printf("ERR Unknown command.\n");  // Comando inválido
    }
}

// Função para atualizar o valor do LED
void ledUpdate() {
    int normalizedValue = map(ledValue, 0, 100, 0, 255);  // Normaliza o valor do LED para 0 a 255
    analogWrite(ledPin, normalizedValue);                 // Define o valor do LED
}

// Função para ler o valor do LDR
int ldrGetValue() {
    int rawValue = analogRead(ldrPin);  // Lê o valor do sensor LDR
    int normalizedLDRValue = map(rawValue, 0, ldrMax, 0, 100);  // Normaliza entre 0 e 100
    return normalizedLDRValue;
}
