#include <LiquidCrystal.h>  // Biblioteca para o display LCD
#include <Keypad.h>         // Biblioteca para o teclado matricial

// Configuração do display LCD
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Configuração do teclado matricial
const byte ROWS = 4; // Quatro linhas
const byte COLS = 4; // Quatro colunas
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {2, 3, 10, 11}; // Pinos das linhas do teclado
byte colPins[COLS] = {12, A0, A1, A2}; // Pinos das colunas do teclado

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

char palavra[12]; // Palavra a ser verificada
char original[12]; // Cópia da palavra original para exibição
int estado = 0; // Estado inicial

void ReplaceAt(char* input, char search, char replace, int start, int end) {
  for (int i = start; i < end; i++) {
    if (input[i] == search) {
      input[i] = replace;
    }
  }
}

void palindromeCheck() {
  strcpy(original, palavra); // Copia a palavra original para exibição
  lcd.setCursor(0, 0);
  lcd.print("Verificando...");
  delay(2000);
  bool result = true;
  int index = 0;
  bool finish = false;

  while (!finish) {
    char l = palavra[index];

    switch (estado) {
      case 0:
        if (l == '1') estado = 1;
        else if (l == '2') estado = 2;
        else if (l == '_' || l == '5'){
          estado = 8;
          finish = true;
          break;
        }else{
          finish = true;
          result = false;
          break;
        }  
        ReplaceAt(palavra, l, '_', index, index + 1);
        index++;
        break;

      case 1:
        if (l == '1' || l == '2') {
          estado = 3;
          index++;
        }else if (l == '_' || l == '5'){
          estado = 8;
          finish = true;
          break;
        }else{
          finish = true;
          result = false;
          break;
        }
        break;

      case 2:
        if (l == '1' || l == '2') {
          estado = 4;
          index++;
        }else if (l == '_' || l == '5'){
          estado = 8;
          finish = true;
          break;
        }else{
          finish = true;
          result = false;
          break;
        }
        break;

      case 3:
        if (l == '1' || l == '2') index++;
        else {
          estado = 5;
          index--;
        }
        break;

      case 4:
        if (l == '1' || l == '2') index++;
        else {
          estado = 6;
          index--;
        }
        break;

      case 5:
        if (l == '1') {
          ReplaceAt(palavra, l, '_', index, index + 1);
          index--;
          estado = 7;
        } else {
          result = false;
          finish = true;
        }
        break;

      case 6:
        if (l == '2') {
          ReplaceAt(palavra, l, '_', index, index + 1);
          index--;
          estado = 7;
        } else {
          result = false;
          finish = true;
        }
        break;

      case 7:
        if (l == '1' || l == '2') index--;
        else {
          index++;
          estado = 0;
        }
        break;
    }

    // Exibe o estado atual no LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Estado: ");
    lcd.print(estado);
    lcd.setCursor(0, 1);
    lcd.print(palavra);
    delay(2000); // Pausa para visualizar a mudança de estado
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(original);
  lcd.setCursor(0, 1);
  if (result) {
    lcd.print("Palavra aceita");
  } else {
    lcd.print("Palavra recusada");
  }
}

void setup() {
  lcd.begin(16, 2); // Define o número de colunas e linhas do display
  Serial.begin(9600); // Inicializa a comunicação serial
  lcd.setCursor(0, 0);
  lcd.print("Digite a palavra:");
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    int len = strlen(palavra);
    if (key == '#') {
      palavra[len] = '\0'; // Termina a palavra
      palindromeCheck(); // Chama a função de verificação de palíndromo
      delay(5000); // Espera 5 segundos para reiniciar
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Digite a palavra:");
      memset(palavra, 0, sizeof(palavra)); // Limpa a palavra
      estado = 0; // Reseta o estado
    } else if (len < sizeof(palavra) - 1) {
      palavra[len] = key; // Adiciona o caractere à palavra
      palavra[len + 1] = '\0';
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Digite a palavra:");
      lcd.setCursor(0, 1);
      lcd.print(palavra);
    }
  }
}

