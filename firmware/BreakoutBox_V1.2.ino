/*
 * BREAKOUT BOX V1.2 - ESP32-C6-LCD-1.47 Waveshare
 * 
 * Hardware: Waveshare ESP32-C6-LCD-1.47 (ecran integre)
 * Date: Decembre 2025
 * 
 * CHANGELOG V1.2:
 *   - Ajout lecture AUTO_REQ, CMD, MANUAL_REQ pour diagnostic complet
 *   - Test verifie CHAQUE etage (pas seulement OUT final)
 *   - Affichage diagnostic detaille si echec
 *   
 * CHANGELOG V1.1:
 *   - Migration vers ESP32-C6-LCD-1.47 (ecran integre)
 *   - ST7789 via TFT_eSPI
 */

#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

// ═══════════════════════════════════════════════════════════════════════════
// PINOUT - Affectation ADC pour diagnostic complet VALVE
// ═══════════════════════════════════════════════════════════════════════════
// ADC0 = +12V_PROT     (diviseur 10k/3.3k, plage 0-15V)
// ADC1 = +5V_LOG       (diviseur 10k/10k, plage 0-6V)
// ADC2 = AUTO_REQ      (sortie 74HC14, diviseur 10k/10k, 0-5V)
// ADC3 = OUT_DRIVER    (sortie BTS5090, diviseur 10k/3.3k, 0-15V)
// ADC4 = CMD           (noeud OR, diviseur 10k/10k, 0-5V)
// ADC5 = MANUAL_REQ    (CD4013 Q, diviseur 10k/10k, 0-5V)
//
// STIM1 = Tire IN_A (entree 74HC14) vers GND = simule pressostat ON
// STIM2 = Pulse sur CLK CD4013 = simule appui bouton

#define PIN_ADC_12V       0   // +12V_PROT
#define PIN_ADC_5V        1   // +5V_LOG
#define PIN_ADC_AUTO      2   // AUTO_REQ (sortie Schmitt)
#define PIN_ADC_OUT       3   // OUT_DRIVER
#define PIN_ADC_CMD       4   // CMD (noeud OR)
#define PIN_ADC_MANUAL    5   // MANUAL_REQ (CD4013 Q)

#define PIN_SDA          18
#define PIN_SCL          19
#define PIN_STIM1         8   // -> IN_A 74HC14
#define PIN_STIM2         9   // -> CLK CD4013
#define PIN_BTN_TEST     10
#define PIN_BTN_NEXT     20
#define PIN_BLK          22

// ═══════════════════════════════════════════════════════════════════════════
// CONSTANTES
// ═══════════════════════════════════════════════════════════════════════════

#define DIV_15V   4.03f   // 10k/3.3k pour 0-15V
#define DIV_6V    2.00f   // 10k/10k pour 0-6V

#define ADC_VREF        3.3f
#define ADC_RES         4095.0f
#define ADC_SAMP        16

// Seuils GO/NO-GO
#define V12_MIN      11.0f
#define V12_MAX      14.5f
#define V5_MIN       4.90f
#define V5_MAX       5.10f
#define I_MAX_MA     5.0f
#define LOGIC_HIGH   2.5f   // >2.5V = HIGH logique
#define LOGIC_LOW    1.0f   // <1.0V = LOW logique
#define OUT_ON_MIN  10.0f
#define OUT_ON_MAX  14.0f
#define OUT_OFF_MAX  0.5f

// ═══════════════════════════════════════════════════════════════════════════
// OBJETS
// ═══════════════════════════════════════════════════════════════════════════

TFT_eSPI tft = TFT_eSPI();
Adafruit_INA219 ina219;
bool ina_ok = false;

enum Mode { LIVE, POWER, CURRENT, STIM_A, STIM_M, FULL, MODE_CNT };
Mode mode = LIVE;
const char* modeName[] = {"LIVE", "POWER", "CURRENT", "STIM_A", "STIM_M", "FULL"};

// Mesures
float v12, v5, iMa, vOut, vAuto, vCmd, vMan;

// ═══════════════════════════════════════════════════════════════════════════
// SETUP
// ═══════════════════════════════════════════════════════════════════════════

void setup() {
  Serial.begin(115200);
  
  pinMode(PIN_STIM1, OUTPUT); digitalWrite(PIN_STIM1, LOW);
  pinMode(PIN_STIM2, OUTPUT); digitalWrite(PIN_STIM2, LOW);
  pinMode(PIN_BTN_TEST, INPUT_PULLUP);
  pinMode(PIN_BTN_NEXT, INPUT_PULLUP);
  pinMode(PIN_BLK, OUTPUT); digitalWrite(PIN_BLK, HIGH);
  
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawCentreString("BREAKOUT BOX", 86, 130, 2);
  tft.drawCentreString("V1.2", 86, 160, 2);
  delay(1000);
  
  Wire.begin(PIN_SDA, PIN_SCL);
  if (ina219.begin()) {
    ina_ok = true;
    ina219.setCalibration_32V_1A();
  }
  
  Serial.println("Breakout Box V1.2 ready");
}

// ═══════════════════════════════════════════════════════════════════════════
// LECTURES
// ═══════════════════════════════════════════════════════════════════════════

float readADC(int pin, float div) {
  long s = 0;
  for (int i = 0; i < ADC_SAMP; i++) { s += analogRead(pin); delayMicroseconds(100); }
  return ((float)s / ADC_SAMP / ADC_RES) * ADC_VREF * div;
}

float readI() {
  if (!ina_ok) return -1;
  float s = 0;
  for (int i = 0; i < 8; i++) { s += ina219.getCurrent_mA(); delay(5); }
  return s / 8.0f;
}

void readAll() {
  v12   = readADC(PIN_ADC_12V, DIV_15V);
  v5    = readADC(PIN_ADC_5V, DIV_6V);
  vAuto = readADC(PIN_ADC_AUTO, DIV_6V);
  vOut  = readADC(PIN_ADC_OUT, DIV_15V);
  vCmd  = readADC(PIN_ADC_CMD, DIV_6V);
  vMan  = readADC(PIN_ADC_MANUAL, DIV_6V);
  iMa   = readI();
}

void stim1(bool on) { digitalWrite(PIN_STIM1, on ? HIGH : LOW); }
void stim2(bool on) { digitalWrite(PIN_STIM2, on ? HIGH : LOW); }

// ═══════════════════════════════════════════════════════════════════════════
// TESTS
// ═══════════════════════════════════════════════════════════════════════════

bool testPower() {
  v12 = readADC(PIN_ADC_12V, DIV_15V);
  v5  = readADC(PIN_ADC_5V, DIV_6V);
  bool ok12 = (v12 >= V12_MIN && v12 <= V12_MAX);
  bool ok5  = (v5 >= V5_MIN && v5 <= V5_MAX);
  Serial.printf("PWR: 12V=%.2f(%s) 5V=%.2f(%s)\n", v12, ok12?"OK":"FAIL", v5, ok5?"OK":"FAIL");
  return ok12 && ok5;
}

bool testCurrent() {
  iMa = readI();
  bool ok = (iMa >= 0 && iMa <= I_MAX_MA);
  Serial.printf("CUR: %.1fmA (%s)\n", iMa, ok?"OK":"FAIL");
  return ok;
}

// Test stimulation AUTO: verifie toute la chaine
// STIM1 ON -> IN_A LOW -> 74HC14 -> AUTO_REQ HIGH -> OR -> CMD HIGH -> BTS5090 -> OUT HIGH
bool testStimAuto() {
  Serial.println("--- STIM AUTO ---");
  
  // Etat repos
  stim1(false); delay(100);
  float auto0 = readADC(PIN_ADC_AUTO, DIV_6V);
  float cmd0  = readADC(PIN_ADC_CMD, DIV_6V);
  float out0  = readADC(PIN_ADC_OUT, DIV_15V);
  Serial.printf("Repos: AUTO=%.2f CMD=%.2f OUT=%.2f\n", auto0, cmd0, out0);
  
  // Verif repos: AUTO=LOW (pressostat off), CMD depend de MANUAL
  bool reposOk = (auto0 < LOGIC_LOW);
  if (!reposOk) {
    Serial.println("FAIL: AUTO_REQ pas LOW au repos");
    return false;
  }
  
  // Activer STIM1 (simule pressostat ON = tire IN_A vers GND)
  stim1(true); delay(150);
  float auto1 = readADC(PIN_ADC_AUTO, DIV_6V);
  float cmd1  = readADC(PIN_ADC_CMD, DIV_6V);
  float out1  = readADC(PIN_ADC_OUT, DIV_15V);
  Serial.printf("Stim: AUTO=%.2f CMD=%.2f OUT=%.2f\n", auto1, cmd1, out1);
  
  // Verifier chaque etage
  bool autoOk = (auto1 > LOGIC_HIGH);
  bool cmdOk  = (cmd1 > LOGIC_HIGH);
  bool outOk  = (out1 >= OUT_ON_MIN && out1 <= OUT_ON_MAX);
  
  if (!autoOk) Serial.println("FAIL: 74HC14 ne repond pas");
  if (!cmdOk)  Serial.println("FAIL: OR gate ne repond pas");
  if (!outOk)  Serial.println("FAIL: BTS5090 ne commute pas");
  
  // Desactiver
  stim1(false); delay(150);
  float auto2 = readADC(PIN_ADC_AUTO, DIV_6V);
  float out2  = readADC(PIN_ADC_OUT, DIV_15V);
  Serial.printf("Off: AUTO=%.2f OUT=%.2f\n", auto2, out2);
  
  bool offOk = (auto2 < LOGIC_LOW) && (out2 < OUT_OFF_MAX || vMan > LOGIC_HIGH);
  // Note: OUT peut rester ON si MANUAL_REQ est actif
  
  bool pass = autoOk && cmdOk && outOk;
  Serial.printf("STIM_A: %s\n", pass?"PASS":"FAIL");
  return pass;
}

// Test stimulation MANUAL: verifie toggle CD4013
// STIM2 pulse -> CLK CD4013 -> MANUAL_REQ toggle -> OR -> CMD -> OUT
bool testStimManual() {
  Serial.println("--- STIM MANUAL ---");
  
  // Etat initial
  float man0 = readADC(PIN_ADC_MANUAL, DIV_6V);
  float out0 = readADC(PIN_ADC_OUT, DIV_15V);
  bool wasOn = (man0 > LOGIC_HIGH);
  Serial.printf("Init: MAN=%.2f(%s) OUT=%.2f\n", man0, wasOn?"ON":"OFF", out0);
  
  // Pulse STIM2 (simule appui bouton)
  stim2(true); delay(50); stim2(false); delay(200);
  
  float man1 = readADC(PIN_ADC_MANUAL, DIV_6V);
  float cmd1 = readADC(PIN_ADC_CMD, DIV_6V);
  float out1 = readADC(PIN_ADC_OUT, DIV_15V);
  bool isOn = (man1 > LOGIC_HIGH);
  Serial.printf("After: MAN=%.2f(%s) CMD=%.2f OUT=%.2f\n", man1, isOn?"ON":"OFF", cmd1, out1);
  
  bool toggled = (wasOn != isOn);
  if (!toggled) {
    Serial.println("FAIL: CD4013 n'a pas toggle");
  }
  
  // Verifier coherence CMD et OUT si MANUAL est ON
  bool cmdOk = true, outOk = true;
  if (isOn) {
    cmdOk = (cmd1 > LOGIC_HIGH);
    outOk = (out1 >= OUT_ON_MIN);
    if (!cmdOk) Serial.println("FAIL: CMD pas HIGH avec MANUAL ON");
    if (!outOk) Serial.println("FAIL: OUT pas ON avec MANUAL ON");
  }
  
  // Remettre etat initial
  stim2(true); delay(50); stim2(false); delay(200);
  
  bool pass = toggled && cmdOk && outOk;
  Serial.printf("STIM_M: %s\n", pass?"PASS":"FAIL");
  return pass;
}

// ═══════════════════════════════════════════════════════════════════════════
// FULL AUTO
// ═══════════════════════════════════════════════════════════════════════════

bool runFull() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.drawCentreString("FULL AUTO", 86, 10, 2);
  tft.drawFastHLine(0, 35, 172, TFT_WHITE);
  
  int y = 50;
  
  bool pOk = testPower();
  drawResult("Power", pOk, y); y += 25;
  
  bool cOk = testCurrent();
  drawResult("Current", cOk, y); y += 25;
  
  bool aOk = testStimAuto();
  drawResult("StimAuto", aOk, y); y += 25;
  
  bool mOk = testStimManual();
  drawResult("StimMan", mOk, y); y += 40;
  
  bool all = pOk && cOk && aOk && mOk;
  
  tft.setTextSize(3);
  tft.setTextColor(all ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.drawCentreString(all ? "ALL PASS" : "FAILED", 86, y, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  
  delay(3000);
  return all;
}

void drawResult(const char* name, bool pass, int y) {
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(name, 10, y);
  tft.setTextColor(pass ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.drawString(pass ? "PASS" : "FAIL", 120, y);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
}

// ═══════════════════════════════════════════════════════════════════════════
// LOOP
// ═══════════════════════════════════════════════════════════════════════════

void loop() {
  static bool lastN = true, lastT = true;
  bool btnN = digitalRead(PIN_BTN_NEXT);
  bool btnT = digitalRead(PIN_BTN_TEST);
  
  if (lastN && !btnN) { mode = (Mode)((mode + 1) % MODE_CNT); delay(200); }
  lastN = btnN;
  
  if (lastT && !btnT) { runTest(); delay(200); }
  lastT = btnT;
  
  if (mode == LIVE) showLive(); else showMenu();
  delay(50);
}

void runTest() {
  bool ok;
  switch (mode) {
    case POWER:   ok = testPower(); showRes("POWER", ok); break;
    case CURRENT: ok = testCurrent(); showRes("CURRENT", ok); break;
    case STIM_A:  ok = testStimAuto(); showRes("STIM AUTO", ok); break;
    case STIM_M:  ok = testStimManual(); showRes("STIM MAN", ok); break;
    case FULL:    runFull(); break;
    default: break;
  }
}

void showRes(const char* name, bool pass) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.drawCentreString(name, 86, 100, 2);
  tft.setTextSize(3);
  tft.setTextColor(pass ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.drawCentreString(pass ? "PASS" : "FAIL", 86, 150, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  delay(2000);
}

// ═══════════════════════════════════════════════════════════════════════════
// AFFICHAGES
// ═══════════════════════════════════════════════════════════════════════════

void showLive() {
  readAll();
  
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.drawCentreString("LIVE VIEW", 86, 5, 2);
  tft.drawFastHLine(0, 28, 172, TFT_WHITE);
  
  char buf[24];
  int y = 35;
  
  // +12V
  bool ok12 = (v12 >= V12_MIN && v12 <= V12_MAX);
  snprintf(buf, sizeof(buf), "12V:%.1f", v12);
  tft.setTextColor(ok12 ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.drawString(buf, 5, y); y += 22;
  
  // +5V
  bool ok5 = (v5 >= V5_MIN && v5 <= V5_MAX);
  snprintf(buf, sizeof(buf), " 5V:%.2f", v5);
  tft.setTextColor(ok5 ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.drawString(buf, 5, y); y += 22;
  
  // Courant
  if (iMa >= 0) {
    snprintf(buf, sizeof(buf), "  I:%.1fmA", iMa);
    tft.setTextColor((iMa <= I_MAX_MA) ? TFT_GREEN : TFT_RED, TFT_BLACK);
  } else {
    snprintf(buf, sizeof(buf), "  I:---");
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  }
  tft.drawString(buf, 5, y); y += 28;
  
  // Signaux logiques
  tft.drawFastHLine(0, y, 172, TFT_DARKGREY); y += 5;
  
  tft.setTextColor(vAuto > LOGIC_HIGH ? TFT_CYAN : TFT_DARKGREY, TFT_BLACK);
  tft.drawString("AUTO", 5, y);
  tft.setTextColor(vMan > LOGIC_HIGH ? TFT_CYAN : TFT_DARKGREY, TFT_BLACK);
  tft.drawString("MAN", 70, y);
  tft.setTextColor(vCmd > LOGIC_HIGH ? TFT_CYAN : TFT_DARKGREY, TFT_BLACK);
  tft.drawString("CMD", 125, y);
  y += 25;
  
  // OUT
  snprintf(buf, sizeof(buf), "OUT:%.1fV", vOut);
  tft.setTextColor(vOut > 1 ? TFT_GREEN : TFT_WHITE, TFT_BLACK);
  tft.drawString(buf, 5, y);
  tft.drawString(vOut > 5 ? "[ON]" : "[OFF]", 110, y);
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString("NEXT=mode  TEST=run", 10, 305);
}

void showMenu() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.drawCentreString("MODE", 86, 60, 2);
  tft.setTextSize(3);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawCentreString(modeName[mode], 86, 130, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawCentreString("NEXT       TEST", 86, 280, 2);
}

// FIN V1.2
