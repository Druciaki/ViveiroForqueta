#define DEBUG true

// Definição do visor LCD (I2C)
#define LCD true     // LCD instalado?
#define LCD_COL 16    // número de colunas
#define LCD_LIN 2     // número de linhas
#define LCD_ADD 0x27  // Endereço I2C do dispositivo

// Definição do relógio (RTC - Real Time Clock)
#define RTC true       // RTC instalado? Nosso sensor utiliza I2C por isso não definimos pinagem
#define RTC_ADD 0x68 // (ou x68 x53

// Definição sensores
#define UMIDADE_SOLO true     // Sensor umidade de solo instalado?
#define PINO_UMIDADE_ANALOG 2  // Pino analógico de leitura
#define UMIDADE_MAXIMA 255     // Valor digital máximo
#define UMIDADE_MINIMA 0       // Valor digital mínimo
#define UMIDADE_E_TEMPERATURA false     // Sensor umidade de solo instalado?
#define PINO_SENSOR_UMIDADE_TEMPERATURA 2    // Pino de dados do sensor de temperatura e umidade

// Definição da pinagem dos relés
#define PINO_RELE_VALVULA 8
#define PINO_RELE_BOMBA 9

// Definição dos botões
#define BOTAO1 10
#define BOTAO2 11
#define BOTAO3 12

void inicializa_visor();
void inicializa_rtc();
void busca_dispositivos_i2c();
void verifica_data_hora();
bool solo_esta_seco(int umidade_digital);
void atualiza_umidade_solo();
bool eh_hora_de_irrigar();
void mostra_data_hora_lcd();