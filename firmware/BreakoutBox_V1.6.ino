/*
 * BreakoutBox_V1.6.ino
 * Diagnostic VALVE v4.8+
 * ESP32-C6-LCD-1.47
 * 
 * CHANGELOG V1.6 (Fault Injection Hardening):
 * - Validation INA219 plage [-100, 1000] mA
 * - Compteur erreurs I2C avec invalidation apres 3 echecs
 * - Validation NVS: isfinite() + plage [0.8, 1.2]
 * - Protection division par zero calibration (seuil 0.5V)
 * - Detection ADC anomalie: raw<10 (capteur HS), v>16.5V (saturation)
 * - Timeout STIM1 auto-OFF 60s (securite oubli)
 * - Codes retour negatifs pour erreurs ADC
 * 
 * CHANGELOG V1.5:
 * - Calibration persistante NVS (Preferences)
 * - Watchdog 5s avec reset automatique
 * - Timeout I2C 100ms (anti-freeze)
 * - Timing STIM2 corrige: 100ms
 * 
 * MATERIEL:
 * - ESP32-C6-LCD-1.47 (Waveshare)
 * - INA219 module I2C
 * - Connecteur J-BB vers VALVE
 */

#include <Wire.h>
#include <Adafruit_INA219.h>
#include <TFT_eSPI.h>
#include <Preferences.h>
#include <esp_task_wdt.h>
#include <math.h>  // V1.6: isfinite()

// === CONFIGURATION PINS ===
#define PIN_DIV_12V     4
#define PIN_DIV_5V      5
#define PIN_AUTO_REQ    6
#define PIN_CMD         7
#define PIN_MANUAL_REQ  10
#define PIN_OUT_DIV     11
#define PIN_STIM1       18
#define PIN_STIM2       19
#define PIN_SDA         21
#define PIN_SCL         22

// === CONSTANTES DIVISEURS ===
#define DIV_12V_RATIO   5.545f    // (15k + 3.3k) / 3.3k
#define DIV_5V_RATIO    2.0f      // (10k + 10k) / 10k
#define DIV_OUT_RATIO   5.545f
#define ADC_VREF        3.3f
#define ADC_RESOLUTION  4095.0f

// === CONSTANTES TIMING ===
#define WDT_TIMEOUT_S   5
#define I2C_TIMEOUT_MS  100
#define STIM_PULSE_MS   100
#define REFRESH_MS      250
#define STRESS_DELAY_MS 100

// === V1.6: CONSTANTES VALIDATION ===
#define CURRENT_MIN_VALID   -100.0f   // mA (courant inverse transitoire)
#define CURRENT_MAX_VALID   1000.0f   // mA (MAC 35A = 450mA, marge 2x)
#define I2C_ERROR_MAX       3         // Erreurs consecutives avant invalidation

#define CAL_FACTOR_MIN      0.8f      // Facteur calibration minimum
#define CAL_FACTOR_MAX      1.2f      // Facteur calibration maximum
#define CAL_VOLTAGE_MIN     0.5f      // Tension minimum pour calibration

#define V12_RAW_MIN         10        // ADC raw minimum (sinon capteur HS)
#define V12_FAULT_HIGH      16.5f     // Tension max plausible automobile

#define STIM_TIMEOUT_MS     60000     // Auto-OFF STIM1 apres 60s

// === CODES ERREUR ADC ===
#define ADC_ERR_SENSOR_HS   -1.0f
#define ADC_ERR_SATURATED   -2.0f

// === OBJETS GLOBAUX ===
Adafruit_INA219 ina219;
TFT_eSPI tft = TFT_eSPI();
Preferences prefs;

// === VARIABLES GLOBALES ===
float g_calFactor = 1.0f;
float g_minCurrent = 9999.0f;
float g_maxCurrent = -9999.0f;
bool g_ina219_ok = false;

// V1.6: Variables fault injection
uint8_t g_i2c_errors = 0;
uint32_t g_stim1_start = 0;

// === MODES ===
enum Mode { MODE_LIVE, MODE_STRESS, MODE_CAL, MODE_MENU };
Mode g_currentMode = MODE_LIVE;

// =============================================================================
// SETUP
// =============================================================================
void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("BreakoutBox V1.6 - Init...");
    
    // --- Watchdog ---
    esp_task_wdt_init(WDT_TIMEOUT_S, true);
    esp_task_wdt_add(NULL);
    Serial.println("Watchdog 5s active");
    
    // --- GPIO ---
    pinMode(PIN_AUTO_REQ, INPUT);
    pinMode(PIN_CMD, INPUT);
    pinMode(PIN_MANUAL_REQ, INPUT);
    pinMode(PIN_STIM1, OUTPUT);
    pinMode(PIN_STIM2, OUTPUT);
    digitalWrite(PIN_STIM1, LOW);
    digitalWrite(PIN_STIM2, LOW);
    
    // --- ADC ---
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);
    
    // --- I2C avec timeout ---
    Wire.begin(PIN_SDA, PIN_SCL);
    Wire.setTimeOut(I2C_TIMEOUT_MS);
    Serial.printf("I2C timeout: %dms\n", I2C_TIMEOUT_MS);
    
    // --- INA219 ---
    if (ina219.begin()) {
        g_ina219_ok = true;
        ina219.setCalibration_32V_1A();
        Serial.println("INA219 OK");
    } else {
        g_ina219_ok = false;
        Serial.println("INA219 ABSENT - Mesure courant desactivee");
    }
    
    // --- V1.6: Charger calibration NVS avec validation ---
    prefs.begin("breakout", false);
    g_calFactor = prefs.getFloat("cal", 1.0f);
    prefs.end();
    
    // Validation integrite NVS
    if (!isfinite(g_calFactor) || 
        g_calFactor < CAL_FACTOR_MIN || 
        g_calFactor > CAL_FACTOR_MAX) {
        Serial.printf("CAL CORRUPT: %.4f -> Reset 1.0\n", g_calFactor);
        g_calFactor = 1.0f;
        // Effacer valeur corrompue
        prefs.begin("breakout", false);
        prefs.putFloat("cal", 1.0f);
        prefs.end();
    } else {
        Serial.printf("Calibration chargee: %.4f\n", g_calFactor);
    }
    
    // --- Ecran ---
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    
    splashScreen();
    
    Serial.println("Init complete - Fault Injection Hardened");
}

// =============================================================================
// LOOP
// =============================================================================
void loop() {
    esp_task_wdt_reset();
    
    // --- V1.6: Timeout STIM1 securite ---
    if (digitalRead(PIN_STIM1) == HIGH) {
        if (millis() - g_stim1_start > STIM_TIMEOUT_MS) {
            digitalWrite(PIN_STIM1, LOW);
            Serial.println("STIM1: Auto-OFF (timeout 60s)");
        }
    }
    
    // --- Lecture serie ---
    if (Serial.available()) {
        char cmd = Serial.read();
        handleSerialCommand(cmd);
    }
    
    // --- Mode courant ---
    switch (g_currentMode) {
        case MODE_LIVE:
            modeLive();
            break;
        case MODE_STRESS:
            modeStress();
            g_currentMode = MODE_LIVE;
            break;
        case MODE_CAL:
            modeCalibration();
            g_currentMode = MODE_LIVE;
            break;
        default:
            modeLive();
    }
    
    delay(REFRESH_MS);
}

// =============================================================================
// FONCTIONS LECTURE [MODIFIE V1.6]
// =============================================================================
float readVoltage12V() {
    int raw = analogRead(PIN_DIV_12V);
    
    // V1.6: Detection capteur HS (ligne coupee ou GND)
    if (raw < V12_RAW_MIN) {
        Serial.println("ADC12V FAULT: raw < 10 (capteur HS ou GND)");
        return ADC_ERR_SENSOR_HS;
    }
    
    float v_gpio = (raw / ADC_RESOLUTION) * ADC_VREF;
    float v12 = v_gpio * DIV_12V_RATIO * g_calFactor;
    
    // V1.6: Detection saturation ADC
    if (v12 > V12_FAULT_HIGH) {
        Serial.printf("ADC12V SATURATED: %.1fV > 16.5V\n", v12);
        return ADC_ERR_SATURATED;
    }
    
    return v12;
}

float readVoltage5V() {
    int raw = analogRead(PIN_DIV_5V);
    
    // V1.6: Detection capteur HS
    if (raw < V12_RAW_MIN) {
        return ADC_ERR_SENSOR_HS;
    }
    
    float v_gpio = (raw / ADC_RESOLUTION) * ADC_VREF;
    return v_gpio * DIV_5V_RATIO * g_calFactor;
}

float readVoltageOut() {
    int raw = analogRead(PIN_OUT_DIV);
    float v_gpio = (raw / ADC_RESOLUTION) * ADC_VREF;
    return v_gpio * DIV_OUT_RATIO * g_calFactor;
}

float readCurrent() {
    if (!g_ina219_ok) return 0.0f;
    
    float mA = ina219.getCurrent_mA();
    
    // V1.6: Validation plage physique
    if (mA < CURRENT_MIN_VALID || mA > CURRENT_MAX_VALID) {
        g_i2c_errors++;
        Serial.printf("INA219 FAULT: %.0f mA hors plage [%d/%d]\n", 
                      mA, g_i2c_errors, I2C_ERROR_MAX);
        
        if (g_i2c_errors >= I2C_ERROR_MAX) {
            Serial.println("INA219: Trop d'erreurs consecutives, invalidation");
            g_ina219_ok = false;
        }
        return 0.0f;  // Valeur safe, pas d'update min/max
    }
    
    // Lecture OK, reset compteur erreurs
    g_i2c_errors = 0;
    
    // Update min/max
    if (mA < g_minCurrent) g_minCurrent = mA;
    if (mA > g_maxCurrent) g_maxCurrent = mA;
    
    return mA;
}

bool readAutoReq() {
    return digitalRead(PIN_AUTO_REQ) == HIGH;
}

bool readCmd() {
    return digitalRead(PIN_CMD) == HIGH;
}

bool readManualReq() {
    return digitalRead(PIN_MANUAL_REQ) == HIGH;
}

// =============================================================================
// FONCTIONS STIMULATION [MODIFIE V1.6]
// =============================================================================
void stimAuto(bool state) {
    if (state) {
        g_stim1_start = millis();  // V1.6: Timestamp pour timeout
    }
    digitalWrite(PIN_STIM1, state ? HIGH : LOW);
    Serial.printf("STIM1 (Auto): %s\n", state ? "ON" : "OFF");
}

void stimManual() {
    Serial.println("STIM2 (Manual): Pulse 100ms");
    digitalWrite(PIN_STIM2, HIGH);
    delay(STIM_PULSE_MS);
    digitalWrite(PIN_STIM2, LOW);
    delay(STIM_PULSE_MS);
}

// =============================================================================
// MODE LIVE [MODIFIE V1.6]
// =============================================================================
void modeLive() {
    float v12 = readVoltage12V();
    float v5 = readVoltage5V();
    float vOut = readVoltageOut();
    float iMa = readCurrent();
    bool autoReq = readAutoReq();
    bool cmd = readCmd();
    bool manReq = readManualReq();
    
    // --- Affichage ecran ---
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.println("=== LIVE V1.6 ===");
    tft.println();
    
    // V1.6: Affichage avec detection erreurs ADC
    if (v12 == ADC_ERR_SENSOR_HS) {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.println("+12V: SENSOR?");
    } else if (v12 == ADC_ERR_SATURATED) {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.println("+12V: SATURATED");
    } else {
        tft.setTextColor(v12 > 10.0f ? TFT_GREEN : TFT_RED, TFT_BLACK);
        tft.printf("+12V: %.2fV\n", v12);
    }
    
    if (v5 == ADC_ERR_SENSOR_HS) {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.println("+5V:  SENSOR?");
    } else {
        tft.setTextColor(v5 > 4.5f ? TFT_GREEN : TFT_RED, TFT_BLACK);
        tft.printf("+5V:  %.2fV\n", v5);
    }
    
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.printf("OUT:  %.2fV\n", vOut);
    
    // Courant avec min/max
    if (g_ina219_ok) {
        tft.setTextColor(TFT_YELLOW, TFT_BLACK);
        tft.printf("I: %.1fmA\n", iMa);
        tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
        tft.printf("  Min:%.0f Max:%.0f\n", g_minCurrent, g_maxCurrent);
    } else {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.println("INA219: N/A");
    }
    
    tft.println();
    
    // Signaux logiques
    tft.setTextColor(autoReq ? TFT_GREEN : TFT_DARKGREY, TFT_BLACK);
    tft.printf("AUTO: %s  ", autoReq ? "ON " : "OFF");
    
    tft.setTextColor(manReq ? TFT_GREEN : TFT_DARKGREY, TFT_BLACK);
    tft.printf("MAN: %s\n", manReq ? "ON " : "OFF");
    
    tft.setTextColor(cmd ? TFT_GREEN : TFT_DARKGREY, TFT_BLACK);
    tft.printf("CMD:  %s\n", cmd ? "ON " : "OFF");
    
    // --- Serial ---
    if (v12 >= 0 && v5 >= 0) {
        Serial.printf("12V=%.2f 5V=%.2f OUT=%.2f I=%.1fmA AUTO=%d MAN=%d CMD=%d\n",
                      v12, v5, vOut, iMa, autoReq, manReq, cmd);
    } else {
        Serial.println("ADC FAULT detected");
    }
}

// =============================================================================
// MODE STRESS TEST
// =============================================================================
void modeStress() {
    const int CYCLES = 20;
    const int FREQ_HZ = 10;
    int delayMs = 1000 / FREQ_HZ / 2;
    
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.println("=== STRESS TEST ===");
    tft.printf("%d cycles @ %dHz\n\n", CYCLES, FREQ_HZ);
    
    Serial.printf("STRESS: %d cycles @ %dHz\n", CYCLES, FREQ_HZ);
    
    g_minCurrent = 9999.0f;
    g_maxCurrent = -9999.0f;
    
    for (int i = 0; i < CYCLES; i++) {
        esp_task_wdt_reset();
        
        // ON
        stimAuto(true);
        delay(delayMs);
        float iOn = readCurrent();
        
        // OFF
        stimAuto(false);
        delay(delayMs);
        float iOff = readCurrent();
        
        tft.printf("Cycle %2d: ON=%.0f OFF=%.0f\n", i+1, iOn, iOff);
        Serial.printf("Cycle %d: ON=%.1fmA OFF=%.1fmA\n", i+1, iOn, iOff);
        
        delay(STRESS_DELAY_MS);
    }
    
    tft.println();
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.printf("Min: %.0fmA\n", g_minCurrent);
    tft.printf("Max: %.0fmA\n", g_maxCurrent);
    tft.println("\nTermine!");
    
    Serial.printf("STRESS DONE: Min=%.1f Max=%.1f\n", g_minCurrent, g_maxCurrent);
    
    delay(3000);
}

// =============================================================================
// MODE CALIBRATION [MODIFIE V1.6]
// =============================================================================
void modeCalibration() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
    tft.println("=== CALIBRATION ===");
    tft.println();
    tft.println("Mesurer +12V reel");
    tft.println("avec multimetre");
    tft.println();
    tft.println("Envoyer valeur");
    tft.println("via Serial (ex: 12.45)");
    
    Serial.println("CAL: Entrer tension 12V reelle (ex: 12.45):");
    
    // Attendre entree serie (timeout 30s)
    unsigned long start = millis();
    while (!Serial.available()) {
        esp_task_wdt_reset();
        if (millis() - start > 30000) {
            Serial.println("CAL: Timeout");
            return;
        }
        delay(100);
    }
    
    float vReal = Serial.parseFloat();
    if (vReal < 5.0f || vReal > 20.0f) {
        Serial.println("CAL: Valeur hors plage (5-20V)");
        return;
    }
    
    // V1.6: Lecture brute avec annulation calibration
    int raw = analogRead(PIN_DIV_12V);
    float v_gpio = (raw / ADC_RESOLUTION) * ADC_VREF;
    float vMeasured = v_gpio * DIV_12V_RATIO;  // Sans g_calFactor
    
    // V1.6: Protection division par zero
    if (vMeasured < CAL_VOLTAGE_MIN) {
        Serial.printf("CAL ERROR: Tension mesuree %.2fV < 0.5V\n", vMeasured);
        Serial.println("Verifier connexion +12V");
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.println("ERREUR CALIBRATION");
        tft.println();
        tft.printf("V mesure: %.2fV\n", vMeasured);
        tft.println("Trop bas (<0.5V)");
        tft.println();
        tft.println("Verifier connexion");
        delay(3000);
        return;
    }
    
    // Calcul nouveau facteur
    float newCal = vReal / vMeasured;
    
    // V1.6: Validation resultat avant stockage
    if (!isfinite(newCal) || newCal < CAL_FACTOR_MIN || newCal > CAL_FACTOR_MAX) {
        Serial.printf("CAL ERROR: Factor %.4f hors plage [0.8, 1.2]\n", newCal);
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.println("ERREUR CALIBRATION");
        tft.println();
        tft.printf("Factor: %.4f\n", newCal);
        tft.println("Hors plage valide");
        delay(3000);
        return;
    }
    
    g_calFactor = newCal;
    
    // Sauvegarder en NVS
    prefs.begin("breakout", false);
    prefs.putFloat("cal", g_calFactor);
    prefs.end();
    
    Serial.printf("CAL: Mesure=%.3fV Reel=%.3fV Factor=%.4f\n", 
                  vMeasured, vReal, g_calFactor);
    Serial.println("CAL: Sauvegarde NVS OK");
    
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration OK!");
    tft.printf("Factor: %.4f\n", g_calFactor);
    tft.println("Sauvegarde NVS OK");
    
    delay(2000);
}

// =============================================================================
// COMMANDES SERIE
// =============================================================================
void handleSerialCommand(char cmd) {
    switch (cmd) {
        case 'l':
        case 'L':
            g_currentMode = MODE_LIVE;
            Serial.println("Mode: LIVE");
            break;
            
        case 's':
        case 'S':
            g_currentMode = MODE_STRESS;
            Serial.println("Mode: STRESS");
            break;
            
        case 'c':
        case 'C':
            g_currentMode = MODE_CAL;
            Serial.println("Mode: CALIBRATION");
            break;
            
        case 'a':
            stimAuto(true);
            break;
            
        case 'A':
            stimAuto(false);
            break;
            
        case 'm':
        case 'M':
            stimManual();
            break;
            
        case 'r':
        case 'R':
            g_minCurrent = 9999.0f;
            g_maxCurrent = -9999.0f;
            g_i2c_errors = 0;  // V1.6: Reset compteur erreurs
            Serial.println("Min/Max + erreurs reset");
            break;
            
        case 'i':
        case 'I':
            Serial.println("=== INFO ===");
            Serial.printf("Firmware: V1.6 (Hardened)\n");
            Serial.printf("Cal Factor: %.4f\n", g_calFactor);
            Serial.printf("INA219: %s\n", g_ina219_ok ? "OK" : "ABSENT/INVALID");
            Serial.printf("I2C Errors: %d/%d\n", g_i2c_errors, I2C_ERROR_MAX);
            Serial.printf("Watchdog: %ds\n", WDT_TIMEOUT_S);
            Serial.printf("STIM Timeout: %ds\n", STIM_TIMEOUT_MS / 1000);
            break;
            
        case 'h':
        case 'H':
        case '?':
            printHelp();
            break;
            
        default:
            break;
    }
}

// =============================================================================
// ECRAN SPLASH
// =============================================================================
void splashScreen() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 40);
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(3);
    tft.println("BREAKOUT");
    tft.setCursor(50, 80);
    tft.println("BOX");
    tft.setTextSize(2);
    tft.setCursor(60, 130);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.println("V1.6");
    tft.setCursor(10, 155);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.println("Hardened");
    tft.setCursor(20, 180);
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    tft.println("VALVE Diag");
    
    if (!g_ina219_ok) {
        tft.setCursor(10, 210);
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.println("INA219 ABSENT!");
    }
    
    delay(2000);
}

// =============================================================================
// AIDE SERIE
// =============================================================================
void printHelp() {
    Serial.println();
    Serial.println("=== BREAKOUT BOX V1.6 (Hardened) ===");
    Serial.println("Commandes:");
    Serial.println("  L - Mode LIVE (defaut)");
    Serial.println("  S - Mode STRESS (20 cycles 10Hz)");
    Serial.println("  C - Mode CALIBRATION");
    Serial.println("  a - STIM1 ON (auto-OFF 60s)");
    Serial.println("  A - STIM1 OFF");
    Serial.println("  M - STIM2 pulse (toggle manuel)");
    Serial.println("  R - Reset min/max + erreurs");
    Serial.println("  I - Info systeme");
    Serial.println("  H - Cette aide");
    Serial.println();
    Serial.println("Protections V1.6:");
    Serial.println("  - INA219 plage [-100, 1000] mA");
    Serial.println("  - ADC fault detection");
    Serial.println("  - NVS corruption recovery");
    Serial.println("  - STIM1 timeout 60s");
    Serial.println();
}

// =============================================================================
// FIN FIRMWARE V1.6
// =============================================================================
