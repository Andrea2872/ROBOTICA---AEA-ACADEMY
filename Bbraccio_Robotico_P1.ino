#include "ServoEasing.hpp"
// Repo GitHub: https://github.com/ArminJo/ServoEasing

#define N_SERVOS 6 // Numero di servomotori

ServoEasing servos[N_SERVOS]; // Definizione array di servomotori
const int servoPins[N_SERVOS] = {3, 5, 6, 9, 10, 11}; // Pin collegati ai servomotori
bool servoConnected[N_SERVOS] = {false, false, false, false, false, false}; // Stato della connessione

const int INIT_POS[N_SERVOS] = {90, 90, 90, 90, 90, 90}; // Posizioni iniziali
const int POS_1[N_SERVOS] = {45, 120, 60, 30, 150, 75};  // Posizioni di destinazione
const int T_POS = 2000;  // Tempo di posizionamento dei servo (2000 ms)

const int MIN_ANGLE = 0;
const int MAX_ANGLE = 180;

void setup() {
    Serial.begin(9600); // Avvio comunicazione seriale
    Serial.println("🔄 Avvio programma...");

    // Configura il LED integrato per il debugging
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW); // LED spento all'inizio

    // Inizializza i servomotori
    for (int i = 0; i < N_SERVOS; i++) {
        if (servos[i].attach(servoPins[i])) { 
            servos[i].setEasingType(EASE_QUADRATIC_IN_OUT);
            servoConnected[i] = true;  
            Serial.print("✅ Servo ");
            Serial.print(i + 1);
            Serial.println(" connesso correttamente.");
        } else {
            Serial.print("❌ Errore: Servo ");
            Serial.print(i + 1);
            Serial.println(" non collegato!");
        }
    }

    delay(1000);

    // Sposta tutti i servomotori nella posizione di base
    Serial.println("🏁 Spostamento alla posizione iniziale...");
    moveAllServos(INIT_POS, T_POS);
    delay(2000);
}

void loop() {
    Serial.println("🔄 Muovo i servomotori nella posizione predefinita...");
    moveAllServos(POS_1, T_POS);
    delay(2000);

    Serial.println("↩️ Torno alla posizione iniziale...");
    moveAllServos(INIT_POS, T_POS);
    delay(2000);
}

// Funzione per attendere il completamento del movimento dei servomotori
void waitUntilEasingMovementComplete() {
    unsigned long startTime = millis();
    const unsigned long TIMEOUT = 5000; // Timeout massimo di attesa (5 sec)

    while (millis() - startTime < TIMEOUT) {
        bool allDone = true;
        for (int i = 0; i < N_SERVOS; i++) {
            if (servoConnected[i] && servos[i].isMoving()) {
                allDone = false;
                break;
            }
        }
        if (allDone) return; // Se tutti hanno finito, esce dalla funzione
        delay(20);
    }
    
    Serial.println("⚠️ Timeout! Alcuni servomotori non hanno completato il movimento.");
}

// Funzione per muovere tutti i servomotori contemporaneamente
void moveAllServos(const int targetPositions[], int duration) {
    digitalWrite(LED_BUILTIN, HIGH); // Accende il LED mentre i servi si muovono

    for (int i = 0; i < N_SERVOS; i++) {
        if (servoConnected[i]) {
            int safeAngle = constrain(targetPositions[i], MIN_ANGLE, MAX_ANGLE); // Limita tra 0 e 180
            Serial.print("🎯 Servo ");
            Serial.print(i + 1);
            Serial.print(" -> ");
            Serial.println(safeAngle);
            servos[i].startEaseTo(safeAngle, duration);
        }
    }

    waitUntilEasingMovementComplete();
    digitalWrite(LED_BUILTIN, LOW); // Spegne il LED quando il movimento è completato
}



