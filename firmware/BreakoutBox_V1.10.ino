/*
 * BreakoutBox_V1.10.ino
 * INTERFACE TECHNICIEN TACTILE
 * ESP32-C6 Touch LCD 1.9" (Waveshare)
 * 
 * CHANGELOG V1.10:
 * - FIX #1: Race condition g_touch_flag (portDISABLE_INTERRUPTS)
 * - FIX #2: Touch desactive pendant runTest() (detach/attachInterrupt)
 * - FIX #3: Diagnostic capteur sortie HS vs valve bloquee
 * - FIX #4: Debounce touch 1000ms (defense en profondeur)
 * 
 * CHANGELOG V1.9:
 * - Support ecran TACTILE 1.9" (CST816 touch controller)
 * - Nouveau pinout compatible LCD+Touch (GPIO0-3, 19-21, 23)
 * - Interface ultra-simple: toucher ecran = tester
 * - Gros boutons tactiles visuels
 * - Rapport one-liner pour IA
 * - I2C partage: INA219 (0x40) + CST816 (0x15)
 * 
 * CONSERVE V1.7:
 * - Protection isfinite() INA219
 * - Validation ADC (raw<10, saturation)
 * - Timeout STIM1 60s
 * - Watchdog 5s
 * 
 * PINOUT V1.10 (ESP32-C6 Touch LCD 1.9"):
 * LCD: GPIO4,5,6,7,14,15 (RESERVE - ne pas utiliser)
 * Touch I2C: GPIO8 (SCL), GPIO18 (SDA) - partage avec INA219
 * Touch INT: GPIO22
 * ADC: GPIO0,1,2,3 (libres)
 * Digital: GPIO19,20,21,23 (libres)
 */

#include <Wire.h>
#include <Adafruit_INA219.h>
#include <TFT_eSPI.h>
#include <Preferences.h>
#include <esp_task_wdt.h>
#include <math.h>

// =============================================================================
// CONFIGURATION PINS V1.10 - ESP32-C6 Touch LCD 1.9"
// =============================================================================
// GPIO reserves LCD: 4, 5, 6, 7, 14, 15
// GPIO reserves Touch: 8 (SCL), 18 (SDA), 22 (INT)

#define PIN_DIV_12V     0   // ADC - Diviseur 12V
#define PIN_DIV_5V      1   // ADC - Diviseur 5V  
#define PIN_OUT_DIV     2   // ADC - Diviseur sortie driver
#define PIN_AUTO_REQ    3   // Digital IN - Signal pression

#define PIN_CMD         19  // Digital IN - Commande active
#define PIN_STIM1       20  // Digital OUT - Stimulation pression
#define PIN_STIM2       21  // Digital OUT - Toggle manuel
#define PIN_MANUAL_REQ  23  // Digital IN - Requete manuelle

// I2C partage Touch + INA219
#define PIN_SDA         18  // I2C SDA (partage CST816 + INA219)
#define PIN_SCL         8   // I2C SCL (partage CST816 + INA219)
#define PIN_TOUCH_INT   22  // Touch interrupt

// =============================================================================
// CONSTANTES
// =============================================================================
#define DIV_12V_RATIO   5.545f
#define DIV_5V_RATIO    2.0f
#define DIV_OUT_RATIO   5.545f
#define ADC_VREF        3.3f
#define ADC_RES         4095.0f

// Seuils GO/NOGO technicien
#define V12_MIN         10.5f
#define V12_MAX         15.0f
#define V5_MIN          4.7f
#define V5_MAX          5.3f
#define VOUT_ON         10.0f

// Validation
#define V12_RAW_MIN     10
#define CURRENT_MIN     -100.0f
#define CURRENT_MAX     1000.0f
#define I2C_ERR_MAX     3
#define CAL_MIN         0.8f
#define CAL_MAX         1.2f
#define STIM_TIMEOUT    60000
#define TOUCH_DEBOUNCE  1000    // V1.10: Debounce 1s

// Couleurs
#define C_GREEN         0x07E0
#define C_RED           0xF800
#define C_ORANGE        0xFD20
#define C_GREY          0x4208
#define C_BLACK         0x0000
#define C_WHITE         0xFFFF
#define C_BLUE          0x001F

// Touch CST816
#define CST816_ADDR     0x15
#define CST816_REG_DATA 0x01

// =============================================================================
// OBJETS GLOBAUX
// =============================================================================
Adafruit_INA219 ina219;
TFT_eSPI tft = TFT_eSPI();
Preferences prefs;

// =============================================================================
// VARIABLES GLOBALES
// =============================================================================
float g_cal = 1.0f;
bool g_ina_ok = false;
uint8_t g_i2c_err = 0;
uint32_t g_stim_start = 0;

// Resultats test
bool g_tested = false;
bool g_pass = false;
bool g_alim_ok = false;
bool g_logic_ok = false;
bool g_press_ok = false;
bool g_valve_ok = false;
bool g_vout_err = false;    // V1.10: Capteur sortie HS
float g_v12 = 0, g_v5 = 0, g_vout = 0, g_ma = 0;

// Touch
volatile bool g_touch_flag = false;
uint16_t g_touch_x = 0, g_touch_y = 0;
uint32_t g_last_touch = 0;  // V1.10: Debounce timestamp

// =============================================================================
// INTERRUPTION TOUCH
// =============================================================================
void IRAM_ATTR onTouchInterrupt() {
    g_touch_flag = true;
}

// =============================================================================
// SETUP
// =============================================================================
void setup() {
    Serial.begin(115200);
    delay(100);
    
    // Watchdog
    esp_task_wdt_init(5, true);
    esp_task_wdt_add(NULL);
    
    // GPIO
    pinMode(PIN_AUTO_REQ, INPUT);
    pinMode(PIN_CMD, INPUT);
    pinMode(PIN_MANUAL_REQ, INPUT);
    pinMode(PIN_STIM1, OUTPUT);
    pinMode(PIN_STIM2, OUTPUT);
    pinMode(PIN_TOUCH_INT, INPUT_PULLUP);
    
    digitalWrite(PIN_STIM1, LOW);
    digitalWrite(PIN_STIM2, LOW);
    
    // ADC
    analogReadResolution(12);
    #if defined(ADC_ATTEN_DB_11)
        analogSetAttenuation(ADC_ATTEN_DB_11);
    #else
        analogSetAttenuation(ADC_11db);
    #endif
    
    // I2C (partage Touch + INA219)
    Wire.begin(PIN_SDA, PIN_SCL);
    Wire.setTimeOut(100);
    
    // INA219
    if (ina219.begin()) {
        g_ina_ok = true;
        ina219.setCalibration_32V_1A();
        Serial.println("INA219: OK (0x40)");
    } else {
        Serial.println("INA219: ABSENT");
    }
    
    // Touch interrupt
    attachInterrupt(digitalPinToInterrupt(PIN_TOUCH_INT), onTouchInterrupt, FALLING);
    
    // NVS calibration
    prefs.begin("breakout", false);
    g_cal = prefs.getFloat("cal", 1.0f);
    prefs.end();
    if (!isfinite(g_cal) || g_cal < CAL_MIN || g_cal > CAL_MAX) {
        g_cal = 1.0f;
    }
    
    // LCD
    tft.init();
    tft.setRotation(0);  // Portrait 170x320
    tft.fillScreen(C_BLACK);
    
    // Splash
    showSplash();
    delay(1500);
    
    // Ecran principal
    showMainScreen();
    
    Serial.println("BreakoutBox V1.10 TACTILE");
    Serial.println("Toucher ecran = Test");
    Serial.println("Serial: T=test, R=rapport");
}

// =============================================================================
// LOOP
// =============================================================================
void loop() {
    esp_task_wdt_reset();
    
    // Timeout STIM1 securite
    if (digitalRead(PIN_STIM1) && millis() - g_stim_start > STIM_TIMEOUT) {
        digitalWrite(PIN_STIM1, LOW);
        Serial.println("STIM1: Auto-OFF (timeout)");
    }
    
    // V1.10 FIX #1: Touch avec protection atomique
    if (g_touch_flag) {
        portDISABLE_INTERRUPTS();
        bool flag = g_touch_flag;
        g_touch_flag = false;
        portENABLE_INTERRUPTS();
        
        if (flag && readTouch()) {
            handleTouch();
        }
    }
    
    // Serial
    if (Serial.available()) {
        char c = Serial.read();
        if (c == 'T' || c == 't') runTest();
        if (c == 'R' || c == 'r') printReport();
        if (c == 'M' || c == 'm') showMainScreen();
    }
    
    delay(50);
}

// =============================================================================
// LECTURE TOUCH CST816
// =============================================================================
bool readTouch() {
    Wire.beginTransmission(CST816_ADDR);
    Wire.write(CST816_REG_DATA);
    if (Wire.endTransmission() != 0) return false;
    
    Wire.requestFrom(CST816_ADDR, 6);
    if (Wire.available() < 6) return false;
    
    uint8_t gesture = Wire.read();
    uint8_t points = Wire.read();
    uint8_t xh = Wire.read();
    uint8_t xl = Wire.read();
    uint8_t yh = Wire.read();
    uint8_t yl = Wire.read();
    
    (void)gesture;  // Unused, suppress warning
    
    if (points == 0) return false;
    
    g_touch_x = ((xh & 0x0F) << 8) | xl;
    g_touch_y = ((yh & 0x0F) << 8) | yl;
    
    Serial.printf("Touch: x=%d y=%d\n", g_touch_x, g_touch_y);
    return true;
}

// =============================================================================
// GESTION TOUCH - V1.10 FIX #4: Debounce
// =============================================================================
void handleTouch() {
    // V1.10: Debounce 1s (defense en profondeur)
    if (millis() - g_last_touch < TOUCH_DEBOUNCE) return;
    g_last_touch = millis();
    
    // Zone bouton TEST (grande zone centrale)
    // Ecran 170x320, bouton de y=80 a y=240
    if (g_touch_y >= 80 && g_touch_y <= 240) {
        runTest();
    }
    // Zone REFAIRE en bas
    else if (g_touch_y > 280 && g_tested) {
        showMainScreen();
    }
}

// =============================================================================
// ECRAN PRINCIPAL
// =============================================================================
void showMainScreen() {
    tft.fillScreen(C_BLACK);
    
    // Titre
    tft.setTextColor(C_WHITE);
    tft.setTextSize(2);
    tft.setCursor(20, 20);
    tft.print("VALVE TESTER");
    
    // Gros bouton TESTER
    tft.fillRoundRect(10, 80, 150, 160, 15, C_BLUE);
    tft.setTextColor(C_WHITE);
    tft.setTextSize(3);
    tft.setCursor(25, 130);
    tft.print("TOUCH");
    tft.setCursor(25, 170);
    tft.print("TEST");
    
    // Status en bas
    tft.setTextSize(1);
    tft.setTextColor(C_GREY);
    tft.setCursor(10, 260);
    tft.print("INA219: ");
    tft.print(g_ina_ok ? "OK" : "N/A");
    
    tft.setCursor(10, 275);
    tft.print("V1.10 Tactile");
    
    // Dernier resultat si existant
    if (g_tested) {
        tft.setCursor(10, 290);
        tft.setTextColor(g_pass ? C_GREEN : C_RED);
        tft.print("Dernier: ");
        tft.print(g_pass ? "OK" : "ECHEC");
    }
}

// =============================================================================
// SPLASH SCREEN
// =============================================================================
void showSplash() {
    tft.fillScreen(C_BLACK);
    
    tft.setTextColor(C_GREEN);
    tft.setTextSize(3);
    tft.setCursor(30, 80);
    tft.print("VALVE");
    tft.setCursor(20, 120);
    tft.print("TESTER");
    
    tft.setTextColor(C_WHITE);
    tft.setTextSize(2);
    tft.setCursor(50, 180);
    tft.print("V1.10");
    
    tft.setTextSize(1);
    tft.setTextColor(C_GREY);
    tft.setCursor(30, 250);
    tft.print("Ecran tactile");
    tft.setCursor(30, 265);
    tft.print("Toucher = Tester");
}

// =============================================================================
// LECTURES CAPTEURS
// =============================================================================
float readV12() {
    int r = analogRead(PIN_DIV_12V);
    if (r < V12_RAW_MIN) return -1.0f;
    float v = (r / ADC_RES) * ADC_VREF * DIV_12V_RATIO * g_cal;
    if (v > 16.5f) return -2.0f;
    return v;
}

float readV5() {
    int r = analogRead(PIN_DIV_5V);
    if (r < V12_RAW_MIN) return -1.0f;
    return (r / ADC_RES) * ADC_VREF * DIV_5V_RATIO * g_cal;
}

float readVOut() {
    int r = analogRead(PIN_OUT_DIV);
    if (r < V12_RAW_MIN) return -1.0f;
    float v = (r / ADC_RES) * ADC_VREF * DIV_OUT_RATIO * g_cal;
    if (v > 15.0f) return -2.0f;
    return v;
}

float readI() {
    if (!g_ina_ok) return 0.0f;
    
    float ma = ina219.getCurrent_mA();
    
    // Protection NaN (V1.7)
    if (!isfinite(ma)) {
        g_i2c_err++;
        if (g_i2c_err >= I2C_ERR_MAX) {
            g_ina_ok = false;
            Serial.println("INA219: Invalide (NaN)");
        }
        return 0.0f;
    }
    
    // Validation plage
    if (ma < CURRENT_MIN || ma > CURRENT_MAX) {
        return 0.0f;
    }
    
    g_i2c_err = 0;
    return ma;
}

// =============================================================================
// TEST COMPLET - V1.10 FIX #2: Protection touch pendant test
// =============================================================================
void runTest() {
    // V1.10 FIX #2: Desactiver touch pendant test
    detachInterrupt(digitalPinToInterrupt(PIN_TOUCH_INT));
    g_touch_flag = false;
    
    // Ecran test en cours
    tft.fillScreen(C_BLACK);
    tft.setTextColor(C_ORANGE);
    tft.setTextSize(3);
    tft.setCursor(30, 100);
    tft.print("TEST");
    tft.setCursor(10, 150);
    tft.print("EN COURS");
    
    // Barre progression
    for (int i = 0; i <= 150; i += 10) {
        tft.fillRect(10, 220, i, 25, C_ORANGE);
        tft.drawRect(10, 220, 150, 25, C_WHITE);
        delay(80);
    }
    
    // === MESURES ===
    g_v12 = readV12();
    g_v5 = readV5();
    g_alim_ok = (g_v12 >= V12_MIN && g_v12 <= V12_MAX);
    g_logic_ok = (g_v5 >= V5_MIN && g_v5 <= V5_MAX);
    
    // Stimulation pression
    digitalWrite(PIN_STIM1, HIGH);
    g_stim_start = millis();
    delay(500);
    
    g_press_ok = digitalRead(PIN_AUTO_REQ);
    
    // Attente valve
    delay(1000);
    
    // V1.10 FIX #3: Detection capteur sortie HS
    g_vout = readVOut();
    g_vout_err = (g_vout < 0);
    g_valve_ok = !g_vout_err && (g_vout >= VOUT_ON);
    
    g_ma = readI();
    
    // Fin stimulation
    digitalWrite(PIN_STIM1, LOW);
    
    // Verdict
    g_pass = g_alim_ok && g_logic_ok && g_press_ok && g_valve_ok;
    g_tested = true;
    
    // Afficher resultat
    showResult();
    
    // Rapport auto
    printReport();
    
    // V1.10 FIX #2: Reactiver touch apres test
    g_touch_flag = false;
    attachInterrupt(digitalPinToInterrupt(PIN_TOUCH_INT), onTouchInterrupt, FALLING);
}

// =============================================================================
// ECRAN RESULTAT - V1.10 FIX #3: Diagnostic capteur
// =============================================================================
void showResult() {
    if (g_pass) {
        // === ECRAN VERT ===
        tft.fillScreen(C_GREEN);
        
        tft.setTextColor(C_BLACK);
        tft.setTextSize(4);
        tft.setCursor(50, 60);
        tft.print("OK");
        
        tft.setTextSize(2);
        tft.setCursor(10, 130);
        tft.print("VALVE");
        tft.setCursor(10, 155);
        tft.print("FONCTIONNE");
        
    } else {
        // === ECRAN ROUGE ===
        tft.fillScreen(C_RED);
        
        tft.setTextColor(C_WHITE);
        tft.setTextSize(3);
        tft.setCursor(30, 50);
        tft.print("ECHEC");
        
        // Cause - V1.10: Ajout diagnostic capteur sortie
        tft.setTextSize(2);
        tft.setCursor(10, 110);
        if (!g_alim_ok)       tft.print("BATTERIE");
        else if (!g_logic_ok) tft.print("CIRCUIT");
        else if (!g_press_ok) tft.print("CAPTEUR P");
        else if (g_vout_err)  tft.print("CAPTEUR V");
        else if (!g_valve_ok) tft.print("VALVE");
        
        tft.setTextSize(1);
        tft.setCursor(10, 140);
        if (!g_alim_ok)       tft.print("Tension 12V incorrecte");
        else if (!g_logic_ok) tft.print("Tension 5V incorrecte");
        else if (!g_press_ok) tft.print("Signal pression absent");
        else if (g_vout_err)  tft.print("Capteur sortie HS");
        else if (!g_valve_ok) tft.print("Valve ne s'ouvre pas");
    }
    
    // Detail en bas
    tft.setTextSize(1);
    int y = 200;
    uint16_t col = g_pass ? C_BLACK : C_WHITE;
    
    tft.setTextColor(col);
    tft.setCursor(10, y);
    tft.printf("12V: %.1fV %s", g_v12, g_alim_ok ? "[OK]" : "[!]");
    
    tft.setCursor(10, y + 15);
    tft.printf("5V:  %.1fV %s", g_v5, g_logic_ok ? "[OK]" : "[!]");
    
    tft.setCursor(10, y + 30);
    if (g_vout_err) {
        tft.printf("OUT: ERR %s", "[HS]");
    } else {
        tft.printf("OUT: %.1fV %s", g_vout, g_valve_ok ? "[OK]" : "[!]");
    }
    
    tft.setCursor(10, y + 45);
    tft.printf("I:   %.0fmA", g_ma);
    
    // Bouton refaire
    tft.fillRoundRect(10, 280, 150, 35, 8, C_GREY);
    tft.setTextColor(C_WHITE);
    tft.setTextSize(2);
    tft.setCursor(25, 288);
    tft.print("REFAIRE");
}

// =============================================================================
// RAPPORT TEXTE POUR IA - V1.10: Ajout diagnostic capteur
// =============================================================================
void printReport() {
    if (!g_tested) {
        Serial.println("Aucun test. Toucher ecran.");
        return;
    }
    
    Serial.println();
    Serial.println("=== RAPPORT VALVE ===");
    
    // ONE-LINER
    Serial.print("COPIER: Valve ");
    Serial.print(g_pass ? "OK" : "ECHEC");
    if (!g_pass) {
        Serial.print(" - Cause: ");
        if (!g_alim_ok)       Serial.print("batterie");
        else if (!g_logic_ok) Serial.print("circuit 5V");
        else if (!g_press_ok) Serial.print("capteur pression");
        else if (g_vout_err)  Serial.print("capteur sortie HS");
        else if (!g_valve_ok) Serial.print("valve bloquee");
    }
    Serial.print(" | 12V=");
    Serial.print(g_v12, 1);
    Serial.print("V, 5V=");
    Serial.print(g_v5, 1);
    Serial.print("V, OUT=");
    if (g_vout_err) {
        Serial.print("ERR");
    } else {
        Serial.print(g_vout, 1);
    }
    Serial.print("V, I=");
    Serial.print(g_ma, 0);
    Serial.println("mA");
    
    Serial.println("=====================");
}

// =============================================================================
// FIN FIRMWARE V1.10
// =============================================================================
