#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <IRremote.hpp>

// Configuração do LCD e Servo
LiquidCrystal_I2C lcd(0x27, 16, 2); 
Servo myServo;

// Configuração dos Pinos
const int RECV_PIN = 4;  // Pino conforme sua última atualização
const int servoPin = 9;  
const int leftLed = 7; 
const int rightLed = 2;  

// Códigos Hexadecimais do seu Controle
const uint16_t BOTAO_ESQUERDA = 0x8; 
const uint16_t BOTAO_DIREITA  = 0x5A;
const uint16_t BOTAO_FRENTE   = 0x18;
const uint16_t BOTAO_TRAS     = 0x52;

// Variáveis de Controle
int angle = 90;            // Inicia em 90 (Reto)
const int stepSize = 180;  // Vai para o extremo com 1 clique

unsigned long previousTime = 0;
const int blinkInterval = 300;

void setup() {
  Serial.begin(9600);
  lcd.init();          
  lcd.backlight();     
  
  myServo.attach(servoPin);
  
  pinMode(leftLed, OUTPUT); 
  pinMode(rightLed, OUTPUT);
  
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
  
  myServo.write(angle); 
  
  lcd.setCursor(0, 0);
  lcd.print("Johnny The Maker");
}

void loop() {
  
  // ==========================================
  // 1. LEITURA DO CONTROLE IR
  // ==========================================
  if (IrReceiver.decode()) {
    uint16_t command = IrReceiver.decodedIRData.command;
    
    Serial.print("Comando recebido: 0x");
    Serial.println(command, HEX);

    if (command == BOTAO_ESQUERDA) {
      angle -= stepSize; // Vai para 0
    } 
    else if (command == BOTAO_DIREITA) {
      angle += stepSize; // Vai para 180
    }
    else if (command == BOTAO_FRENTE || command == BOTAO_TRAS) {
      angle = 90;        // <-- DETERMINA 90 GRAUS (Reto)
    }
    
    // Garante que o valor fique entre 0 e 180
    angle = constrain(angle, 0, 180);
    
    myServo.write(angle);
    IrReceiver.resume(); 
  }

  // ==========================================
  // 2. ATUALIZAÇÃO DO LCD
  // ==========================================
  lcd.setCursor(0, 1);
  lcd.print("Angle: ");
  lcd.print(angle);
  lcd.print("   "); 

  // ==========================================
  // 3. LÓGICA DOS LEDS (Sem Delay)
  // ==========================================
  unsigned long currentTime = millis();
  if (currentTime - previousTime >= blinkInterval) {
    previousTime = currentTime;
    
    // Se estiver virado para a esquerda (Ângulo baixo)
    if (angle < 84) {
      digitalWrite(rightLed, !digitalRead(rightLed)); // Pisca o LED da esquerda
      digitalWrite(leftLed, HIGH);                // Mantém o outro aceso ou apagado conforme desejar
    } 
    // Se estiver virado para a direita (Ângulo alto)
    else if (angle > 95) {
      digitalWrite(leftLed, !digitalRead(leftLed)); // Pisca o LED da direita
      digitalWrite(rightLed, HIGH);
    } 
    // Se estiver reto (90 graus)
    else {
      digitalWrite(leftLed, HIGH);
      digitalWrite(rightLed, HIGH);
    }
  }
}
