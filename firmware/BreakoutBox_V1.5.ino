/*
 * BreakoutBox_V1.5.ino
 * Diagnostic VALVE v4.7
 * ESP32-C6-LCD-1.47
 * 
 * CHANGELOG V1.5:
 * - Calibration persistante NVS (Preferences)
 * - Watchdog 5s avec reset automatique
 * - Timeout I2C 100ms (anti-freeze)
 * - Timing STIM2 corrige: 100ms (etait 5ms)
 * - Commentaires francais sans accents
 * 
 * MATERIEL:
 * - ESP32-C6-LCD-1.47 (Waveshare)
 * - INA219 module I2C
 * - Connecteur J-BB vers VALVE
 */

#include <Wire.h>
#include <Adafruit_INA219.h>
#include <TFT_eSPI.h>
#include <Preferences.h>           // V1.5: NVS
#include <esp_task_wdt.h>          // V1.5: Watchdog

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
#define DIV_OUT_RATIO   5.545f    // Idem 12V
#define ADC_VREF        3.3f
#define ADC_RESOLUTION  4095.0f

// === CONSTANTES TIMING ===
#define WDT_TIMEOUT_S   5         // V1.5: Watchdog 5 secondes
#define I2C_TIMEOUT_MS  100       // V1.5: Timeout I2C
#define STIM_PULSE_MS   100       // V1.5: Pulse STIM (etait 5ms)
#define REFRESH_MS      250       // Rafraichissement ecran
#define STRESS_DELAY_MS 100       // Delai entre cycles stress

// === OBJETS GLOBAUX ===
Adafruit_INA219 ina219;
TFT_eSPI tft = TFT_eSPI();
Preferences prefs;                 // V1.5: Stockage NVS

// === VARIABLES GLOBALES ===
float g_calFactor = 1.0f;          // Facteur calibration ADC
float g_minCurrent = 9999.0f;      // Min Hold
float g_maxCurrent = -9999.0f;     // Max Hold
bool g_ina219_ok = false;          // Flag INA219 detecte

// === MODES ===
enum Mode { MODE_LIVE, MODE_STRESS, MODE_CAL, MODE_MENU };
Mode g_currentMode = MODE_LIVE;

// =============================================================================
// SETUP
// =============================================================================
void setup() {
    Serial.begin(115200);
    delay(100);
    Serial.println("BreakoutBox V1.5 - Init...");
    
    // --- V1.5: Watchdog ---
    esp_task_wdt_init(WDT_TIMEOUT_S, true);  // 5s, panic=true
    esp_task_wdt_add(NULL);                   // Ajoute tache courante
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
    
    // --- I2C avec timeout V1.5 ---
    Wire.begin(PIN_SDA, PIN_SCL);
    Wire.setTimeOut(I2C_TIMEOUT_MS);          // V1.5: Anti-freeze
    Serial.printf("I2C timeout: %dms\n", I2C_TIMEOUT_MS);
    
    // --- INA219 ---
    if (ina219.begin()) {
        g_ina219_ok = true;
        ina219.setCalibration_32V_1A();       // Plage 0-1A haute precision
        Serial.println("INA219 OK");
    } else {
        g_ina219_ok = false;
        Serial.println("INA219 ABSENT - Mesure courant desactivee");
    }
    
    // --- V1.5: Charger calibration NVS ---
    prefs.begin("breakout", false);           // Namespace "breakout"
    g_calFactor = prefs.getFloat("cal", 1.0f);
    Serial.printf("Calibration chargee: %.4f\n", g_calFactor);
    prefs.end();
    
    // --- Ecran ---
    tft.init();
    tft.setRotation(1);                       // Paysage
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    
    splashScreen();
    
    Serial.println("Init complete");
}

// =============================================================================
// LOOP
// =============================================================================
void loop() {
    // --- V1.5: Reset watchdog ---
    esp_task_wdt_reset();
    
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
            g_currentMode = MODE_LIVE;        // Retour auto apres stress
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
// FONCTIONS LECTURE
// =============================================================================
float readVoltage12V() {
    int raw = analogRead(PIN_DIV_12V);
    float v_gpio = (raw / ADC_RESOLUTION) * ADC_VREF;
    return v_gpio * DIV_12V_RATIO * g_calFactor;
}

float readVoltage5V() {
    int raw = analogRead(PIN_DIV_5V);
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
// FONCTIONS STIMULATION [MODIFIE V1.5]
// =============================================================================
void stimAuto(bool state) {
    digitalWrite(PIN_STIM1, state ? HIGH : LOW);
    Serial.printf("STIM1 (Auto): %s\n", state ? "ON" : "OFF");
}

void stimManual() {
    // V1.5: Pulse 100ms (etait 5ms, trop court pour RC 10ms)
    Serial.println("STIM2 (Manual): Pulse 100ms");
    digitalWrite(PIN_STIM2, HIGH);
    delay(STIM_PULSE_MS);                     // V1.5: 100ms
    digitalWrite(PIN_STIM2, LOW);
    delay(STIM_PULSE_MS);                     // V1.5: Attendre stabilisation
}

// =============================================================================
// MODE LIVE
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
    tft.println("=== LIVE MODE ===");
    tft.println();
    
    // Tensions
    tft.setTextColor(v12 > 10.0 ? TFT_GREEN : TFT_RED, TFT_BLACK);
    tft.printf("+12V: %.2fV\n", v12);
    
    tft.setTextColor(v5 > 4.5 ? TFT_GREEN : TFT_RED, TFT_BLACK);
    tft.printf("+5V:  %.2fV\n", v5);
    
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
    Serial.printf("12V=%.2f 5V=%.2f OUT=%.2f I=%.1fmA AUTO=%d MAN=%d CMD=%d\n",
                  v12, v5, vOut, iMa, autoReq, manReq, cmd);
}

// =============================================================================
// MODE STRESS TEST
// =============================================================================
void modeStress() {
    const int CYCLES = 20;
    const int FREQ_HZ = 10;
    int delayMs = 1000 / FREQ_HZ / 2;         // Demi-periode
    
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextColor(TFT_ORANGE, TFT_BLACK);
    tft.println("=== STRESS TEST ===");
    tft.printf("%d cycles @ %dHz\n\n", CYCLES, FREQ_HZ);
    
    Serial.printf("STRESS: %d cycles @ %dHz\n", CYCLES, FREQ_HZ);
    
    g_minCurrent = 9999.0f;
    g_maxCurrent = -9999.0f;
    
    for (int i = 0; i < CYCLES; i++) {
        esp_task_wdt_reset();                 // V1.5: Reset WDT dans boucle
        
        // ON
        stimAuto(true);
        delay(delayMs);
        float iOn = readCurrent();
        
        // OFF
        stimAuto(false);
        delay(delayMs);
        float iOff = readCurrent();
        
        // Affichage progression
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
    
    delay(3000);                              // Pause lecture
}

// =============================================================================
// MODE CALIBRATION [MODIFIE V1.5]
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
        esp_task_wdt_reset();                 // V1.5: Reset WDT
        if (millis() - start > 30000) {
            Serial.println("CAL: Timeout");
            return;
        }
        delay(100);
    }
    
    float vReal = Serial.parseFloat();
    if (vReal < 5.0 || vReal > 20.0) {
        Serial.println("CAL: Valeur hors plage (5-20V)");
        return;
    }
    
    // Lecture brute
    float vMeasured = readVoltage12V() / g_calFactor;  // Annuler cal actuelle
    
    // Nouveau facteur
    float newCal = vReal / vMeasured;
    g_calFactor = newCal;
    
    // V1.5: Sauvegarder en NVS
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
            // Reset min/max
            g_minCurrent = 9999.0f;
            g_maxCurrent = -9999.0f;
            Serial.println("Min/Max reset");
            break;
            
        case 'i':
        case 'I':
            // Info systeme
            Serial.println("=== INFO ===");
            Serial.printf("Firmware: V1.5\n");
            Serial.printf("Cal Factor: %.4f\n", g_calFactor);
            Serial.printf("INA219: %s\n", g_ina219_ok ? "OK" : "ABSENT");
            Serial.printf("Watchdog: %ds\n", WDT_TIMEOUT_S);
            Serial.printf("I2C Timeout: %dms\n", I2C_TIMEOUT_MS);
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
    tft.println("V1.5");
    tft.setCursor(20, 160);
    tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
    tft.println("VALVE Diag");
    
    if (!g_ina219_ok) {
        tft.setCursor(10, 200);
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
    Serial.println("=== BREAKOUT BOX V1.5 ===");
    Serial.println("Commandes:");
    Serial.println("  L - Mode LIVE (defaut)");
    Serial.println("  S - Mode STRESS (20 cycles 10Hz)");
    Serial.println("  C - Mode CALIBRATION");
    Serial.println("  a - STIM1 ON (simule pression)");
    Serial.println("  A - STIM1 OFF");
    Serial.println("  M - STIM2 pulse (toggle manuel)");
    Serial.println("  R - Reset min/max courant");
    Serial.println("  I - Info systeme");
    Serial.println("  H - Cette aide");
    Serial.println();
}

// =============================================================================
// FIN FIRMWARE V1.5
// =============================================================================
