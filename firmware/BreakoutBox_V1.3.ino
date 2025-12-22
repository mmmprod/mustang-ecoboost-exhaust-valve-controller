// ============================================================================
// BREAKOUT BOX VALVE CONTROLLER - FIRMWARE V1.3
// ============================================================================
// Hardware: Waveshare ESP32-C6-LCD-1.47 + INA219
// Target: Circuit VALVE v4.5 (Mustang EcoBoost)
// Date: Decembre 2025
//
// CHANGELOG V1.3:
// - STIM1/STIM2 via NPN open-collector (protection 5V -> 3.3V)
// - Protection INA219 (R serie + Zener sur PCB)
// - Mode STRESS TEST (10Hz pendant 2s)
// - Mode LIVE avec Min/Max Hold
// - Calibration ADC automatique
// - Logique STIM inversee (HIGH = actif avec NPN)
//
// CHANGELOG V1.2:
// - Diagnostic complet tous etages
// - 6 canaux ADC + INA219
// - Tests STIM_AUTO et STIM_MANUAL
// ============================================================================

#include <TFT_eSPI.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

// ============================================================================
// CONFIGURATION HARDWARE V1.3
// ============================================================================

// Ecran ST7789
TFT_eSPI tft = TFT_eSPI();
#define TFT_BL 22

// INA219 Current Sensor
Adafruit_INA219 ina219;
#define PIN_SDA 4
#define PIN_SCL 5

// Boutons navigation
#define PIN_BTN1 0   // BOOT button = Menu/Select
#define PIN_BTN2 21  // Optionnel = Back/Reset stats

// ADC Inputs (avec diviseurs sur Breakout Box)
#define PIN_ADC_12V     0   // +12V_PROT via div 10k/3.3k
#define PIN_ADC_5V      1   // +5V_LOG via div 10k/10k
#define PIN_ADC_AUTO    2   // AUTO_REQ via div 10k/10k
#define PIN_ADC_OUT     3   // OUT_DRIVER via div 10k/3.3k
#define PIN_ADC_CMD     4   // CMD via div 10k/10k
#define PIN_ADC_MANUAL  5   // MANUAL_REQ via div 10k/10k

// Stimulation Outputs (vers NPN open-collector sur Breakout Box)
#define PIN_STIM1 8   // -> Q1 base -> IN_A 74HC14 (simule pressostat)
#define PIN_STIM2 9   // -> Q2 base -> CLK CD4013 (simule bouton)

// Diviseurs de tension (R1/R2 -> ratio = (R1+R2)/R2)
#define DIV_15V  4.03f   // 10k/3.3k -> 0-15V range
#define DIV_6V   2.00f   // 10k/10k -> 0-6.6V range

// Calibration factor (ajuste par routine calibration)
float g_calFactor = 1.000f;

// Seuils validation
#define V12_MIN  10.5f
#define V12_MAX  15.0f
#define V5_MIN   4.75f
#define V5_MAX   5.25f
#define I_IDLE_MAX 10.0f  // mA au repos

// ============================================================================
// VARIABLES GLOBALES
// ============================================================================

enum Mode { MENU, LIVE, TEST_POWER, TEST_CURRENT, TEST_STIM_AUTO, 
            TEST_STIM_MANUAL, FULL_AUTO, STRESS, CALIBRATE };
Mode currentMode = MENU;
int menuIndex = 0;

// Statistiques Min/Max (Mode LIVE)
float g_v12_min = 99.0f;
float g_v12_max = 0.0f;
float g_v5_min = 99.0f;
float g_i_max = 0.0f;
unsigned long g_statsStartTime = 0;

// Resultats tests
bool testResults[6] = {false, false, false, false, false, false};
String testNames[6] = {"Power", "Current", "StimAuto", "StimMan", "Stress", "Calib"};

// ============================================================================
// FONCTIONS UTILITAIRES
// ============================================================================

void resetStats() {
  g_v12_min = 99.0f;
  g_v12_max = 0.0f;
  g_v5_min = 99.0f;
  g_i_max = 0.0f;
  g_statsStartTime = millis();
}

void updateStats(float v12, float v5, float iMa) {
  if(v12 > 1.0f && v12 < g_v12_min) g_v12_min = v12;
  if(v12 > g_v12_max) g_v12_max = v12;
  if(v5 > 1.0f && v5 < g_v5_min) g_v5_min = v5;
  if(iMa > g_i_max) g_i_max = iMa;
}

float readVoltage(int pin, float divider) {
  int raw = analogReadMilliVolts(pin);
  return (raw * divider / 1000.0f) * g_calFactor;
}

bool readLogic(int pin, float divider, float threshold = 2.5f) {
  return readVoltage(pin, divider) > threshold;
}

void stimAuto(bool active) {
  // Avec NPN: HIGH sur GPIO = NPN sature = IN_A tire vers GND = pressostat ON
  digitalWrite(PIN_STIM1, active ? HIGH : LOW);
}

void stimManual() {
  // Pulse CLK pour toggle CD4013
  // Avec NPN: HIGH = NPN sature = CLK tire vers GND (front descendant)
  // CD4013 trigger sur front MONTANT, donc on fait LOW->HIGH->LOW
  digitalWrite(PIN_STIM2, LOW);
  delay(5);
  digitalWrite(PIN_STIM2, HIGH);  // NPN ON = CLK LOW
  delay(50);
  digitalWrite(PIN_STIM2, LOW);   // NPN OFF = CLK HIGH (pull-up) = FRONT MONTANT
  delay(50);
}

void waitButton() {
  while(digitalRead(PIN_BTN1) == HIGH) delay(10);
  while(digitalRead(PIN_BTN1) == LOW) delay(10);
  delay(100);
}

void drawHeader(const char* title) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawCentreString(title, 86, 5, 2);
  tft.drawFastHLine(0, 25, 172, TFT_DARKGREY);
}

void drawResult(bool pass, const char* label, int y) {
  tft.setCursor(10, y);
  if(pass) {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.print("[OK] ");
  } else {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("[FAIL] ");
  }
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.print(label);
}

// ============================================================================
// MENU PRINCIPAL
// ============================================================================

const char* menuItems[] = {
  "1. LIVE Monitor",
  "2. Test Power",
  "3. Test Current", 
  "4. Stim AUTO",
  "5. Stim MANUAL",
  "6. STRESS Test",
  "7. FULL AUTO",
  "8. Calibrate"
};
#define MENU_COUNT 8

void showMenu() {
  drawHeader("BREAKOUT V1.3");
  
  for(int i = 0; i < MENU_COUNT; i++) {
    tft.setCursor(10, 35 + i * 22);
    if(i == menuIndex) {
      tft.setTextColor(TFT_BLACK, TFT_CYAN);
      tft.print("> ");
    } else {
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.print("  ");
    }
    tft.print(menuItems[i]);
  }
  
  // Instructions
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.drawCentreString("BOOT=Select  Long=Run", 86, 300, 1);
}

// ============================================================================
// MODE LIVE AVEC MIN/MAX HOLD
// ============================================================================

void showLive() {
  drawHeader("LIVE MONITOR");
  resetStats();
  
  unsigned long lastUpdate = 0;
  
  while(digitalRead(PIN_BTN1) == HIGH) {
    if(millis() - lastUpdate < 100) continue;  // 10Hz refresh
    lastUpdate = millis();
    
    // Lectures
    float v12 = readVoltage(PIN_ADC_12V, DIV_15V);
    float v5 = readVoltage(PIN_ADC_5V, DIV_6V);
    float vAuto = readVoltage(PIN_ADC_AUTO, DIV_6V);
    float vOut = readVoltage(PIN_ADC_OUT, DIV_15V);
    float vCmd = readVoltage(PIN_ADC_CMD, DIV_6V);
    float vMan = readVoltage(PIN_ADC_MANUAL, DIV_6V);
    float iMa = ina219.getCurrent_mA();
    if(iMa < 0) iMa = 0;
    
    // Update stats
    updateStats(v12, v5, iMa);
    
    // Affichage tensions principales
    int y = 35;
    char buf[32];
    
    // +12V
    tft.setTextColor((v12 >= V12_MIN && v12 <= V12_MAX) ? TFT_GREEN : TFT_RED, TFT_BLACK);
    snprintf(buf, sizeof(buf), "+12V: %5.2fV  ", v12);
    tft.drawString(buf, 10, y, 2); y += 22;
    
    // +5V
    tft.setTextColor((v5 >= V5_MIN && v5 <= V5_MAX) ? TFT_GREEN : TFT_RED, TFT_BLACK);
    snprintf(buf, sizeof(buf), " +5V: %5.2fV  ", v5);
    tft.drawString(buf, 10, y, 2); y += 22;
    
    // Courant
    tft.setTextColor((iMa <= I_IDLE_MAX * 50) ? TFT_GREEN : TFT_YELLOW, TFT_BLACK);
    snprintf(buf, sizeof(buf), "   I: %5.0fmA ", iMa);
    tft.drawString(buf, 10, y, 2); y += 28;
    
    // Signaux logiques
    tft.drawFastHLine(0, y, 172, TFT_DARKGREY); y += 5;
    
    bool bAuto = vAuto > 2.5f;
    bool bMan = vMan > 2.5f;
    bool bCmd = vCmd > 2.5f;
    bool bOut = vOut > 6.0f;
    
    tft.setTextColor(bAuto ? TFT_CYAN : TFT_DARKGREY, TFT_BLACK);
    tft.drawString("AUTO", 10, y, 2);
    
    tft.setTextColor(bMan ? TFT_CYAN : TFT_DARKGREY, TFT_BLACK);
    tft.drawString("MAN", 70, y, 2);
    
    tft.setTextColor(bCmd ? TFT_CYAN : TFT_DARKGREY, TFT_BLACK);
    tft.drawString("CMD", 120, y, 2);
    y += 25;
    
    tft.setTextColor(bOut ? TFT_GREEN : TFT_DARKGREY, TFT_BLACK);
    snprintf(buf, sizeof(buf), "OUT: %s (%.1fV)", bOut ? "ON " : "OFF", vOut);
    tft.drawString(buf, 10, y, 2);
    y += 30;
    
    // Min/Max stats
    tft.drawFastHLine(0, y, 172, TFT_DARKGREY); y += 5;
    tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
    
    snprintf(buf, sizeof(buf), "12V: %.1f-%.1fV", g_v12_min < 50 ? g_v12_min : 0, g_v12_max);
    tft.drawString(buf, 5, y, 1); y += 12;
    
    snprintf(buf, sizeof(buf), "5V min:%.2fV  I max:%.0fmA", g_v5_min < 50 ? g_v5_min : 0, g_i_max);
    tft.drawString(buf, 5, y, 1); y += 12;
    
    unsigned long elapsed = (millis() - g_statsStartTime) / 1000;
    snprintf(buf, sizeof(buf), "Uptime: %lus", elapsed);
    tft.drawString(buf, 5, y, 1);
    
    // Reset stats si BTN2
    if(digitalRead(PIN_BTN2) == LOW) {
      resetStats();
      tft.fillRect(0, 280, 172, 40, TFT_BLACK);
      tft.setTextColor(TFT_YELLOW, TFT_BLACK);
      tft.drawCentreString("Stats Reset!", 86, 290, 1);
      delay(300);
    }
  }
  waitButton();
}

// ============================================================================
// TEST POWER
// ============================================================================

bool testPower() {
  drawHeader("TEST POWER");
  
  float v12 = readVoltage(PIN_ADC_12V, DIV_15V);
  float v5 = readVoltage(PIN_ADC_5V, DIV_6V);
  
  char buf[32];
  int y = 50;
  
  snprintf(buf, sizeof(buf), "+12V_PROT: %.2fV", v12);
  bool ok12 = (v12 >= V12_MIN && v12 <= V12_MAX);
  drawResult(ok12, buf, y); y += 30;
  
  snprintf(buf, sizeof(buf), "+5V_LOG: %.2fV", v5);
  bool ok5 = (v5 >= V5_MIN && v5 <= V5_MAX);
  drawResult(ok5, buf, y); y += 30;
  
  // Seuils
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  snprintf(buf, sizeof(buf), "12V: %.1f-%.1fV", V12_MIN, V12_MAX);
  tft.drawString(buf, 20, y, 1); y += 15;
  snprintf(buf, sizeof(buf), "5V: %.2f-%.2fV", V5_MIN, V5_MAX);
  tft.drawString(buf, 20, y, 1);
  
  bool pass = ok12 && ok5;
  tft.setTextColor(pass ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.drawCentreString(pass ? "PASS" : "FAIL", 86, 200, 4);
  
  waitButton();
  return pass;
}

// ============================================================================
// TEST CURRENT
// ============================================================================

bool testCurrent() {
  drawHeader("TEST CURRENT");
  
  // S'assurer STIM OFF
  stimAuto(false);
  delay(200);
  
  float iIdle = ina219.getCurrent_mA();
  if(iIdle < 0) iIdle = 0;
  
  char buf[32];
  int y = 50;
  
  snprintf(buf, sizeof(buf), "I_idle: %.1f mA", iIdle);
  bool okIdle = (iIdle <= I_IDLE_MAX);
  drawResult(okIdle, buf, y); y += 30;
  
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  snprintf(buf, sizeof(buf), "Max: %.0f mA", I_IDLE_MAX);
  tft.drawString(buf, 20, y, 1);
  
  bool pass = okIdle;
  tft.setTextColor(pass ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.drawCentreString(pass ? "PASS" : "FAIL", 86, 200, 4);
  
  waitButton();
  return pass;
}

// ============================================================================
// TEST STIM AUTO (Pressostat simulation)
// ============================================================================

bool testStimAuto() {
  drawHeader("STIM AUTO");
  
  int y = 40;
  bool pass = true;
  char buf[40];
  
  // Etat initial: tout OFF
  stimAuto(false);
  delay(200);
  
  bool autoOff = !readLogic(PIN_ADC_AUTO, DIV_6V);
  bool cmdOff = !readLogic(PIN_ADC_CMD, DIV_6V);
  bool outOff = readVoltage(PIN_ADC_OUT, DIV_15V) < 3.0f;
  
  snprintf(buf, sizeof(buf), "Init: AUTO=%d CMD=%d OUT=%d", !autoOff, !cmdOff, !outOff);
  drawResult(autoOff && cmdOff && outOff, buf, y); y += 20;
  if(!(autoOff && cmdOff && outOff)) pass = false;
  
  // Activer STIM1 (simule pressostat ON)
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("Activating STIM1...", 10, y, 1); y += 15;
  
  stimAuto(true);
  delay(300);
  
  bool autoOn = readLogic(PIN_ADC_AUTO, DIV_6V);
  bool cmdOn = readLogic(PIN_ADC_CMD, DIV_6V);
  bool outOn = readVoltage(PIN_ADC_OUT, DIV_15V) > 8.0f;
  
  snprintf(buf, sizeof(buf), "Stim: AUTO=%d", autoOn);
  drawResult(autoOn, buf, y); y += 20;
  if(!autoOn) pass = false;
  
  snprintf(buf, sizeof(buf), "      CMD=%d", cmdOn);
  drawResult(cmdOn, buf, y); y += 20;
  if(!cmdOn) pass = false;
  
  float vOut = readVoltage(PIN_ADC_OUT, DIV_15V);
  snprintf(buf, sizeof(buf), "      OUT=%.1fV", vOut);
  drawResult(outOn, buf, y); y += 20;
  if(!outOn) pass = false;
  
  // Mesure courant sous charge
  float iLoad = ina219.getCurrent_mA();
  snprintf(buf, sizeof(buf), "I_load: %.0f mA", iLoad);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString(buf, 10, y, 1); y += 20;
  
  // Desactiver
  stimAuto(false);
  delay(300);
  
  bool autoBack = !readLogic(PIN_ADC_AUTO, DIV_6V);
  snprintf(buf, sizeof(buf), "Release: AUTO=%d", !autoBack);
  drawResult(autoBack, buf, y); y += 20;
  if(!autoBack) pass = false;
  
  // Verdict
  tft.setTextColor(pass ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.drawCentreString(pass ? "PASS" : "FAIL", 86, 260, 4);
  
  if(!pass) {
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    if(!autoOn) tft.drawCentreString("Check 74HC14", 86, 290, 1);
    else if(!cmdOn) tft.drawCentreString("Check OR diodes", 86, 290, 1);
    else if(!outOn) tft.drawCentreString("Check BTS5090", 86, 290, 1);
  }
  
  waitButton();
  return pass;
}

// ============================================================================
// TEST STIM MANUAL (Button simulation)
// ============================================================================

bool testStimManual() {
  drawHeader("STIM MANUAL");
  
  int y = 40;
  bool pass = true;
  char buf[40];
  
  // Etat initial
  bool manInit = readLogic(PIN_ADC_MANUAL, DIV_6V);
  snprintf(buf, sizeof(buf), "Init: MAN=%d", manInit);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(buf, 10, y, 1); y += 20;
  
  // Premier pulse (toggle)
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("Sending pulse 1...", 10, y, 1); y += 15;
  
  stimManual();
  delay(100);
  
  bool manAfter1 = readLogic(PIN_ADC_MANUAL, DIV_6V);
  bool toggled1 = (manAfter1 != manInit);
  snprintf(buf, sizeof(buf), "After P1: MAN=%d", manAfter1);
  drawResult(toggled1, buf, y); y += 20;
  if(!toggled1) pass = false;
  
  // Verifier CMD et OUT
  bool cmdOn = readLogic(PIN_ADC_CMD, DIV_6V);
  float vOut = readVoltage(PIN_ADC_OUT, DIV_15V);
  
  if(manAfter1) {
    snprintf(buf, sizeof(buf), "CMD=%d OUT=%.1fV", cmdOn, vOut);
    bool outOk = cmdOn && (vOut > 8.0f);
    drawResult(outOk, buf, y); y += 20;
    if(!outOk) pass = false;
  }
  
  // Second pulse (retour)
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("Sending pulse 2...", 10, y, 1); y += 15;
  
  stimManual();
  delay(100);
  
  bool manAfter2 = readLogic(PIN_ADC_MANUAL, DIV_6V);
  bool toggled2 = (manAfter2 == manInit);
  snprintf(buf, sizeof(buf), "After P2: MAN=%d", manAfter2);
  drawResult(toggled2, buf, y); y += 20;
  if(!toggled2) pass = false;
  
  // Verdict
  tft.setTextColor(pass ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.drawCentreString(pass ? "PASS" : "FAIL", 86, 260, 4);
  
  if(!pass) {
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.drawCentreString("Check CD4013", 86, 290, 1);
  }
  
  waitButton();
  return pass;
}

// ============================================================================
// STRESS TEST (10Hz pendant 2s)
// ============================================================================

bool runStressTest() {
  tft.fillScreen(TFT_RED);
  tft.setTextColor(TFT_WHITE, TFT_RED);
  tft.drawCentreString("!!! STRESS TEST !!!", 86, 30, 2);
  tft.drawCentreString("20 cycles @ 10Hz", 86, 60, 1);
  tft.drawCentreString("Valve will CLICK!", 86, 80, 1);
  
  delay(1000);
  
  float vMin = 20.0f;
  float vMax = 0.0f;
  float iMax = 0.0f;
  
  // Barre de progression
  tft.drawRect(20, 120, 132, 20, TFT_WHITE);
  
  for(int i = 0; i < 20; i++) {
    // ON
    stimAuto(true);
    delay(10);
    
    // Mesures sous charge (pic courant)
    float v = readVoltage(PIN_ADC_12V, DIV_15V);
    float iNow = ina219.getCurrent_mA();
    if(iNow < 0) iNow = 0;
    
    if(v > 1.0f && v < vMin) vMin = v;
    if(v > vMax) vMax = v;
    if(iNow > iMax) iMax = iNow;
    
    delay(40);  // Total 50ms ON
    
    // OFF
    stimAuto(false);
    delay(50);  // 50ms OFF
    
    // Update progress bar
    int progress = (i + 1) * 130 / 20;
    tft.fillRect(21, 121, progress, 18, TFT_YELLOW);
  }
  
  // Resultats
  tft.fillScreen(TFT_BLACK);
  drawHeader("STRESS RESULTS");
  
  int y = 50;
  char buf[40];
  
  snprintf(buf, sizeof(buf), "V min: %.2fV", vMin);
  drawResult(vMin > 10.0f, buf, y); y += 25;
  
  snprintf(buf, sizeof(buf), "V max: %.2fV", vMax);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(buf, 10, y, 2); y += 25;
  
  snprintf(buf, sizeof(buf), "V ripple: %.2fV", vMax - vMin);
  bool rippleOk = (vMax - vMin) < 2.0f;
  drawResult(rippleOk, buf, y); y += 25;
  
  snprintf(buf, sizeof(buf), "I peak: %.0fmA", iMax);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawString(buf, 10, y, 2); y += 35;
  
  // Verdict global
  bool pass = (vMin > 10.0f) && rippleOk;
  
  tft.setTextColor(pass ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.drawCentreString(pass ? "PASS" : "FAIL", 86, 220, 4);
  
  if(!pass) {
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    if(vMin <= 10.0f) {
      tft.drawCentreString("WEAK POWER SUPPLY", 86, 260, 1);
    } else {
      tft.drawCentreString("HIGH RIPPLE", 86, 260, 1);
    }
  }
  
  waitButton();
  return pass;
}

// ============================================================================
// CALIBRATION ADC
// ============================================================================

bool runCalibration() {
  drawHeader("ADC CALIBRATION");
  
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawCentreString("Measure +5V_LOG with", 86, 50, 1);
  tft.drawCentreString("a multimeter and note", 86, 70, 1);
  tft.drawCentreString("the exact value.", 86, 90, 1);
  
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawCentreString("Press BTN to calibrate", 86, 130, 1);
  tft.drawCentreString("with V_ref = 5.00V", 86, 150, 1);
  
  waitButton();
  
  // Lecture brute canal 5V (avant calibration)
  g_calFactor = 1.000f;  // Reset
  float rawV5 = readVoltage(PIN_ADC_5V, DIV_6V);
  
  // Calculer facteur (assume 5.00V reel)
  float targetV = 5.00f;
  g_calFactor = targetV / rawV5;
  
  char buf[40];
  int y = 180;
  
  snprintf(buf, sizeof(buf), "Raw: %.3fV", rawV5);
  tft.drawString(buf, 20, y, 2); y += 25;
  
  snprintf(buf, sizeof(buf), "Factor: %.4f", g_calFactor);
  tft.drawString(buf, 20, y, 2); y += 25;
  
  // Verification
  float newV5 = readVoltage(PIN_ADC_5V, DIV_6V);
  snprintf(buf, sizeof(buf), "Calibrated: %.3fV", newV5);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString(buf, 20, y, 2);
  
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawCentreString("OK - Factor saved", 86, 280, 1);
  
  waitButton();
  return true;
}

// ============================================================================
// FULL AUTO TEST
// ============================================================================

void runFullAuto() {
  drawHeader("FULL AUTO TEST");
  
  int y = 40;
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("Running all tests...", 10, y, 1);
  y += 25;
  
  // Test 1: Power
  tft.drawString("1. Power...", 10, y, 1);
  testResults[0] = testPower();
  tft.setCursor(130, y);
  tft.setTextColor(testResults[0] ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.print(testResults[0] ? "OK" : "FAIL");
  y += 20;
  
  // Test 2: Current
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("2. Current...", 10, y, 1);
  testResults[1] = testCurrent();
  tft.setCursor(130, y);
  tft.setTextColor(testResults[1] ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.print(testResults[1] ? "OK" : "FAIL");
  y += 20;
  
  // Test 3: Stim Auto
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("3. Stim Auto...", 10, y, 1);
  testResults[2] = testStimAuto();
  tft.setCursor(130, y);
  tft.setTextColor(testResults[2] ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.print(testResults[2] ? "OK" : "FAIL");
  y += 20;
  
  // Test 4: Stim Manual
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("4. Stim Manual...", 10, y, 1);
  testResults[3] = testStimManual();
  tft.setCursor(130, y);
  tft.setTextColor(testResults[3] ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.print(testResults[3] ? "OK" : "FAIL");
  y += 20;
  
  // Test 5: Stress
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString("5. Stress...", 10, y, 1);
  testResults[4] = runStressTest();
  tft.setCursor(130, y);
  tft.setTextColor(testResults[4] ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.print(testResults[4] ? "OK" : "FAIL");
  y += 30;
  
  // Resume
  drawHeader("TEST SUMMARY");
  y = 50;
  
  bool allPass = true;
  for(int i = 0; i < 5; i++) {
    drawResult(testResults[i], testNames[i].c_str(), y);
    y += 22;
    if(!testResults[i]) allPass = false;
  }
  
  y += 20;
  tft.setTextColor(allPass ? TFT_GREEN : TFT_RED, TFT_BLACK);
  tft.drawCentreString(allPass ? "ALL PASS" : "FAILED", 86, y, 4);
  
  if(allPass) {
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.drawCentreString("Ready for install!", 86, y + 50, 1);
  }
  
  waitButton();
}

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== BREAKOUT BOX V1.3 ===");
  
  // Backlight
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  
  // Boutons
  pinMode(PIN_BTN1, INPUT_PULLUP);
  pinMode(PIN_BTN2, INPUT_PULLUP);
  
  // Stimulation outputs (vers NPN)
  pinMode(PIN_STIM1, OUTPUT);
  pinMode(PIN_STIM2, OUTPUT);
  digitalWrite(PIN_STIM1, LOW);  // NPN OFF = pas de stimulation
  digitalWrite(PIN_STIM2, LOW);
  
  // ADC
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  
  // I2C + INA219
  Wire.begin(PIN_SDA, PIN_SCL);
  if(!ina219.begin()) {
    Serial.println("INA219 not found!");
  }
  ina219.setCalibration_32V_1A();
  
  // Ecran
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  
  // Splash
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.drawCentreString("BREAKOUT BOX", 86, 100, 2);
  tft.drawCentreString("V1.3", 86, 130, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawCentreString("VALVE Controller", 86, 180, 1);
  tft.drawCentreString("Mustang EcoBoost", 86, 200, 1);
  
  delay(2000);
  
  resetStats();
  currentMode = MENU;
}

// ============================================================================
// LOOP
// ============================================================================

void loop() {
  switch(currentMode) {
    case MENU:
      showMenu();
      
      // Navigation par appuis courts
      if(digitalRead(PIN_BTN1) == LOW) {
        unsigned long pressStart = millis();
        while(digitalRead(PIN_BTN1) == LOW) {
          if(millis() - pressStart > 500) break;  // Long press
        }
        unsigned long pressDuration = millis() - pressStart;
        
        if(pressDuration > 500) {
          // Long press = execute
          switch(menuIndex) {
            case 0: currentMode = LIVE; break;
            case 1: currentMode = TEST_POWER; break;
            case 2: currentMode = TEST_CURRENT; break;
            case 3: currentMode = TEST_STIM_AUTO; break;
            case 4: currentMode = TEST_STIM_MANUAL; break;
            case 5: currentMode = STRESS; break;
            case 6: currentMode = FULL_AUTO; break;
            case 7: currentMode = CALIBRATE; break;
          }
        } else {
          // Short press = navigate
          menuIndex = (menuIndex + 1) % MENU_COUNT;
        }
        delay(150);
      }
      break;
      
    case LIVE:
      showLive();
      currentMode = MENU;
      break;
      
    case TEST_POWER:
      testPower();
      currentMode = MENU;
      break;
      
    case TEST_CURRENT:
      testCurrent();
      currentMode = MENU;
      break;
      
    case TEST_STIM_AUTO:
      testStimAuto();
      currentMode = MENU;
      break;
      
    case TEST_STIM_MANUAL:
      testStimManual();
      currentMode = MENU;
      break;
      
    case STRESS:
      runStressTest();
      currentMode = MENU;
      break;
      
    case FULL_AUTO:
      runFullAuto();
      currentMode = MENU;
      break;
      
    case CALIBRATE:
      runCalibration();
      currentMode = MENU;
      break;
  }
}
