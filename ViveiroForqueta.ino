#include <Wire.h>               // Biblioteca utilizada para fazer a comunicação com o I2C
#include <LiquidCrystal_I2C.h>  // Biblioteca utilizada para fazer a comunicação com o display
#include <DHT11.h>
#include <Bonezegei_DS1307.h>
#include "ViveiroForqueta.h"

// Variáveis Globais
LiquidCrystal_I2C lcd(LCD_ADD, LCD_COL, LCD_LIN);  // Instanciando o visor LCD
Bonezegei_DS1307 rtc(RTC_ADD);

int leituraSensorUmidade;
int umidade_solo;
int umidade_valor_minimo_aceitavel = 120;
int ultimo_dia_lido = 0;
int ultima_hora_lida = 0;
int ultimo_minuto_lido = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando");
  inicializa_visor();
  inicializa_rtc();
  Serial.println("Configurando botões");
  pinMode(BOTAO1, INPUT);
  pinMode(BOTAO2, INPUT);
  pinMode(BOTAO3, INPUT);
  pinMode(PINO_RELE_VALVULA, OUTPUT);
  pinMode(PINO_RELE_BOMBA, OUTPUT);
  Serial.println("Setup Finalizado");
  lcd.autoscroll();
  lcd.setCursor(1, 0);       // Coloca o cursor do display na coluna 1 e linha 1
  lcd.print("TESTE 1 2 3");  // Comando de saída com a mensagem que deve aparecer na coluna 2 e linha 1.
  delay(5000);
  lcd.print("  OLHAAA QUI");
}

void loop() {
  busca_dispositivos_i2c();
  bool botao1 = digitalRead(BOTAO1);
  bool botao2 = digitalRead(BOTAO2);
  bool botao3 = digitalRead(BOTAO3);
  leituraSensorUmidade = analogRead(PINO_UMIDADE_ANALOG);     // Leitura do pino analógico
  umidade_solo = map(leituraSensorUmidade, 0, 1023, 0, 255);  // Mapeia o valor analógia para o intervalo 0 a 255
  if (ultimo_dia_lido != rtc.getDate()) {
    if (umidade_solo < 100) {
      digitalWrite(PINO_RELE_BOMBA, HIGH);
      digitalWrite(PINO_RELE_VALVULA, HIGH);
    }
  }
  if (botao1) {
    digitalWrite(PINO_RELE_BOMBA, HIGH);
  } else {
    digitalWrite(PINO_RELE_BOMBA, LOW);
  }
  if (botao2) {
    digitalWrite(PINO_RELE_VALVULA, HIGH);
  } else {
    digitalWrite(PINO_RELE_VALVULA, LOW);
  }
  if (botao3) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Sensor leitura:");
    lcd.print(umidade_solo);
    delay(1000);
  } else {
    mostra_data_hora_lcd();
  }
  if (DEBUG) {
    if (UMIDADE_SOLO) {
      Serial.print("Umidade do Solo: Valor Anal: ");
      Serial.print(leituraSensorUmidade);
      Serial.print("    Valor Digi: ");
      Serial.println(umidade_solo);
    }
    Serial.print("Botão 1:");
    Serial.print(botao1);
    Serial.print("  Botão 2:");
    Serial.print(botao2);
    Serial.print("  Botão 3:");
    Serial.println(botao3);
  }

  ultimo_dia_lido = rtc.getDate();
  ultima_hora_lida = rtc.getHour();
  ultimo_minuto_lido = rtc.getMinute();
  delay(2000);
}


void inicializa_visor() {
  if (LCD) {
    lcd.init();       // Serve para iniciar a comunicação com o display já conectado
    lcd.backlight();  // Serve para ligar a luz do display
    lcd.clear();      // Serve para limpar a tela do display
    lcd.setCursor(1, 0);
    lcd.print("INICIANDO");
  }
}

void inicializa_rtc() {
  if (RTC) {
    Serial.println("Inicializando relógio.");
    // Se precisar redefinir data e hora:
    /*
    rtc.setFormat(24);        // Formato 24h
    rtc.setTime("20:10:30");  //Set hora    hora:minuto:segundo
    rtc.setDate("5/15/26");   //Set data    mes/dia/ano
   */
    rtc.begin();
    ultimo_dia_lido = rtc.getDate();
    ultima_hora_lida = rtc.getHour();
    ultimo_minuto_lido = rtc.getMinute();
  }
}

void busca_dispositivos_i2c() {
  byte error, address;
  int nDevices = 0;
  Serial.println("Procurando dispositivos I2C...");

  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("Dispositivo encontrado no endereço 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      nDevices++;
    }
  }
  if (nDevices == 0) {
    Serial.println("Nenhum dispositivo I2C encontrado.");
  } else {
    Serial.println("Varredura concluída.");
  }
}

void mostra_data_hora_lcd() {
  if (RTC && LCD) {
    lcd.clear();
    if (rtc.getTime()) {
      char data[32];
      char horario[32];
      sprintf(horario, "%02d:%02d:%02d ", rtc.getHour(), rtc.getMinute(), rtc.getSeconds());
      sprintf(data, " %02d-%02d-%d  ", rtc.getMonth(), rtc.getDate(), rtc.getYear());
      if (DEBUG) {
        Serial.println(data);
        Serial.println(horario);
      }
      lcd.setCursor(1, 0);
      lcd.print(data);
      //lcd.setCursor(1, 1);
      lcd.print(horario);
    }
  }
}

bool solo_esta_seco(int umidade_digital) {
  return (umidade_digital < umidade_valor_minimo_aceitavel);
}

void atualiza_umidade_solo() {
}