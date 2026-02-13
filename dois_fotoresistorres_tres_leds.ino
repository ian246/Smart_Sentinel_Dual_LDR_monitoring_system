// --- Definição dos Pinos ---
const int LDR_ESQ = A0;      // Jumper Laranja 1
const int LDR_DIR = A1;      // Jumper Laranja 2
const int LED_VERDE = 11;    // Pino PWM
const int LED_AMARELO = 10;  // Pino PWM
const int LED_VERMELHO = 9;  // Pino PWM

// --- Parâmetros de Calibração ---
const int TOLERANCIA_FALHA = 150; // Diferença máxima aceitável entre sensores

void setup() {
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  Serial.begin(9600);
  
  Serial.println("=====================================");
  Serial.println("   SISTEMA SENTINEL DUAL INICIADO    ");
  Serial.println("=====================================");
}

void loop() {
  // 1. Leitura dos Dados (Inputs)
  int s1 = analogRead(LDR_ESQ);
  int s2 = analogRead(LDR_DIR);
  int media = (s1 + s2) / 2;

  // 2. Cálculo de Discrepância (Diferença absoluta)
  int diferenca = abs(s1 - s2);

  // 3. Reporte via Serial
  Serial.print("LDR_0: "); Serial.print(s1);
  Serial.print(" | LDR_1: "); Serial.print(s2);
  Serial.print(" | MEDIA: "); Serial.print(media);
  
  // 4. Lógica de Segurança (Fail-Safe)
  if (diferenca > TOLERANCIA_FALHA) {
    Serial.println(" -> [ALERTA: FALHA DE SENSOR!]");
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, LOW);
    // Pisca o amarelo para indicar erro
    digitalWrite(LED_AMARELO, HIGH); delay(50);
    digitalWrite(LED_AMARELO, LOW);  delay(50);
  } 
  else {
    // 5. Lógica de Operação Normal (Ajustada para o seu ambiente)
    if (media > 40) {           // Se você colocar o celular perto, deve passar de 40
      digitalWrite(LED_VERDE, HIGH);
      digitalWrite(LED_AMARELO, LOW);
      digitalWrite(LED_VERMELHO, LOW);
      Serial.println(" -> STATUS: SEGURO (MUITO CLARO)");
    } 
    else if (media >= 20 && media <= 40) { // O estado atual do seu quarto (17-22)
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_AMARELO, HIGH);
      digitalWrite(LED_VERMELHO, LOW);
      Serial.println(" -> STATUS: ATENÇÃO (LUZ AMBIENTE)");
    } 
    else {                       // Se você tampar com a mão, vai cair para uns 10
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_AMARELO, LOW);
      digitalWrite(LED_VERMELHO, HIGH);
      Serial.println(" -> STATUS: ALERTA (ESCURO TOTAL)");
    }
    delay(150); 
  }
}