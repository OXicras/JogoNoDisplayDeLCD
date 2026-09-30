#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
const int pinBotaoCima = 11;
const int pinBotaoBaixo = 13;
const int pinBotaoReset = 12;
int estadoPlayer = 1;

void setup()
{
  pinMode(pinBotaoCima, INPUT_PULLUP);
  pinMode(pinBotaoBaixo, INPUT_PULLUP);
  pinMode(pinBotaoReset, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, estadoPlayer);
  lcd.print(">");
}

void loop()
{
  bool estadoBotaoCima = digitalRead(pinBotaoCima);
  bool estadoBotaoBaixo = digitalRead(pinBotaoBaixo);
  static bool estadoAnteriorCima = estadoBotaoCima;
  static bool estadoAnteriorBaixo = estadoBotaoBaixo;
  bool estadoBotaoReset = digitalRead(pinBotaoReset);
  static bool estadoAnteriorReset = estadoBotaoReset;
  unsigned long now = millis();
  static unsigned long tempoInicial = 0;
  unsigned long cooldownTiro = 200;
  static int linhaTiro = random(4);
  static int i = 20;
  static bool tiroAtivo = false;
  static bool gameOver = false;
  static bool gameOverTela = false;

  // ! MOVIMENTAR PERSONAGEM
  if (!gameOver)
  {
    if (estadoAnteriorCima != estadoBotaoCima)
    {
      if (!estadoBotaoCima)
      {
        if (estadoPlayer > 0)
        {
          lcd.setCursor(0, estadoPlayer);
          lcd.print(" ");
          estadoPlayer--;
          lcd.setCursor(0, estadoPlayer);
          lcd.print(">");
        }
      }
    }
    estadoAnteriorCima = estadoBotaoCima;

    if (estadoAnteriorBaixo != estadoBotaoBaixo)
    {
      if (!estadoBotaoBaixo)
      {
        if (estadoPlayer < 3)
        {
          lcd.setCursor(0, estadoPlayer);
          lcd.print(" ");
          estadoPlayer++;
          lcd.setCursor(0, estadoPlayer);
          lcd.print(">");
        }
      }
    }
    estadoAnteriorBaixo = estadoBotaoBaixo;

    // TODO: OBSTÁCULOS
    if (!tiroAtivo)
    {
      tiroAtivo = true;
      linhaTiro = random(4);
    }

    if (tiroAtivo && now >= tempoInicial + cooldownTiro)
    {
      tempoInicial = now;

      lcd.setCursor(i + 1, linhaTiro);
      lcd.print(" ");

      if (i >= 0)
      {
        lcd.setCursor(i, linhaTiro);
        lcd.print("#");
      }
      i--;

      if (i == 0 && linhaTiro == estadoPlayer)
        gameOver = true;

      if (i < -1)
      {
        i = 20;
        tiroAtivo = false;
      }
    }
  }
  else if (!gameOverTela)
  {
    gameOverTela = true;
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("GAME OVER!");
  }
  else
  {
    if (estadoAnteriorReset != estadoBotaoReset)
    {
      if (!estadoBotaoReset)
      {
        lcd.clear();
        gameOverTela = false;
        gameOver = false;
      }
    }
    estadoAnteriorReset = estadoBotaoReset;
  }
}