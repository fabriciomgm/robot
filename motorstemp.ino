#include <Bluepad32.h>
#include <DHT.h>
#include "pitches.h"

// Define Motor Pins
int LF1 = 25; // LEFT FRONT POSITIVE
int LF2 = 26; 
int LB1 = 27; // LEFT BACK POSITIVE
int LB2 = 14;
int RF1 = 22; // RIGHT FRONT POSITIVE
int RF2 = 23;
int RB1 = 18; // RIGHT BACK POSITIVE
int RB2 = 19;

// Define other pins
int BUZZER_PIN = 13; // buzzer
int DHTPIN = 33; //humidity and temperature sensor
int PELTIER = 32; // peltier and fans

// Dependiendo del tipo de sensor
#define DHTTYPE DHT11
 
// Inicializamos el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);


// Variables to hold motor states
bool LF1ST = 0;
bool LF2ST = 0;
bool LB1ST = 0;
bool LB2ST = 0;
bool RF1ST = 0;
bool RF2ST = 0;
bool RB1ST = 0;
bool RB2ST = 0;

//variable to hold peltier state

bool PELTIER_ST = true;

GamepadPtr myGamepads[BP32_MAX_GAMEPADS];

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedGamepad(GamepadPtr gp) {

////////////////////////////////////////.......................

  bool foundEmptySlot = true;
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myGamepads[i] == nullptr) {
      Serial.printf("CALLBACK: Gamepad is connected, index=%d\n", i);
      // Additionally, you can get certain gamepad properties like:
      // Model, VID, PID, BTAddr, flags, etc.
      GamepadProperties properties = gp->getProperties();
      Serial.printf("Gamepad model: %s, VID=0x%04x, PID=0x%04x\n",
                    gp->getModelName().c_str(), properties.vendor_id,
                    properties.product_id);
      myGamepads[i] = gp;
      foundEmptySlot = 100; 
      
      break;
    }
  }
  if (!foundEmptySlot) {
    Serial.println(
        "CALLBACK: Gamepad connected, but could not found empty slot");
  }
}

void onDisconnectedGamepad(GamepadPtr gp) {

//////////////////////////................... cambiar a 0
  bool foundGamepad = false;

  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myGamepads[i] == gp) {
      Serial.printf("CALLBACK: Gamepad is disconnected from index=%d\n", i);
      myGamepads[i] = nullptr;

////////////////////............... cambiar a 100

      foundGamepad = true;
      break;
    }
  }

  if (!foundGamepad) {
    Serial.println(
        "CALLBACK: Gamepad disconnected, but not found in myGamepads");
  }
}

// Arduino setup function. Runs in CPU 1
void setup() {
  Serial.begin(115200);
  Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
  const uint8_t *addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2],
                addr[3], addr[4], addr[5]);

  // Setup the Bluepad32 callbacks
  BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);

  // "forgetBluetoothKeys()" should be called when the user performs
  // a "device factory reset", or similar.
  // Calling "forgetBluetoothKeys" in setup() just as an example.
  // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
  // But might also fix some connection / re-connection issues.
  BP32.forgetBluetoothKeys();


  // Set motor pins as outputs

  pinMode(LF1, OUTPUT);
  pinMode(LF2, OUTPUT);
  pinMode(LB1, OUTPUT);
  pinMode(LB2, OUTPUT);
  pinMode(RF1, OUTPUT);
  pinMode(RF2, OUTPUT);
  pinMode(RB1, OUTPUT);
  pinMode(RB2, OUTPUT);

  // set other pins as outputs
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(PELTIER, OUTPUT);

// Comenzamos el sensor DHT
  dht.begin();
 


  // Print to Serial Monitor
  Serial.println("Ready.");



// Turn no Peltier
  digitalWrite(PELTIER, PELTIER_ST);
            Serial.print("PELTIER are: ");
            Serial.println(PELTIER_ST);

}

// Arduino loop function. Runs in CPU 1
void loop() {
  // This call fetches all the gamepad info from the NINA (ESP32) module.
  // Just call this function in your main loop.
  // The gamepads pointer (the ones received in the callbacks) gets updated
  // automatically.
  BP32.update();


// temperature sensor

 float h = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  // Leemos la temperatura en grados Fahreheit
  float f = dht.readTemperature(true);
 
  // Comprobamos si ha habido algún error en la lectura
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }

  Serial.print("Humedad: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(" %\t");

  if (t >= 10.0) {
  digitalWrite(PELTIER, true);
  }
  else {
  digitalWrite(PELTIER, false);
  }


/////

  // It is safe to always do this before using the gamepad API.
  // This guarantees that the gamepad is valid and connected.
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    GamepadPtr myGamepad = myGamepads[i];

    if (myGamepad && myGamepad->isConnected()) {


       
      if (myGamepad->dpad() == 0x00 && myGamepad->buttons() == 0x0000) {
        Serial.println("NO dpad NO buttons");
        
        analogWrite(LF1, 0);
        analogWrite(LF2, 0);

        analogWrite(LB1, 0);
        analogWrite(LB2, 0);
        
        analogWrite(RF1, 0);
        analogWrite(RF2, 0);

        analogWrite(RB1, 0);
        analogWrite(RB2, 0);
        //vTaskDelay(700);
      }

      else {

        Serial.println();

        // FORWARD

        if (myGamepad->dpad() == 0x01) {
          Serial.println("dpad UP");
          //LF1ST = 100;
          //LF2ST = 0;
          analogWrite(LF1, 100);
          analogWrite(LF2, 0);

          analogWrite(LB1, 100);
          analogWrite(LB2, 0);
        
          analogWrite(RF1, 100);
          analogWrite(RF2, 0);

          analogWrite(RB1, 100);
          analogWrite(RB2, 0);
          
        }
        
        //BACK

        if (myGamepad->dpad() == 0x02) {
          Serial.println("dpad DOWN");
          //LF1ST = 0;
          //LF2ST = 100;
          analogWrite(LF1, 0);
          analogWrite(LF2, 100);

          
          analogWrite(LB1, 0);
          analogWrite(LB2, 100);
        
          analogWrite(RF1, 0);
          analogWrite(RF2, 100);

          analogWrite(RB1, 0);
          analogWrite(RB2, 100);



          //vTaskDelay(700);
        }

        // LEFT

        if (myGamepad->dpad() == 0x08) {
          Serial.println("dpad LEFT");

          analogWrite(LF1, 0);
          analogWrite(LF2, 120);

          
          analogWrite(LB1, 120);
          analogWrite(LB2, 0);
        
          analogWrite(RF1, 120);
          analogWrite(RF2, 0);

          analogWrite(RB1, 0);
          analogWrite(RB2, 120);




          //vTaskDelay(700);
        }

        //RIGHT

        if (myGamepad->dpad() == 0x04) {
          Serial.println("dpad RIGHT");

          analogWrite(LF1, 120);
          analogWrite(LF2, 0);

          
          analogWrite(LB1, 0);
          analogWrite(LB2, 120);
        
          analogWrite(RF1, 0);
          analogWrite(RF2, 120);

          analogWrite(RB1, 120);
          analogWrite(RB2, 0);
          //vTaskDelay(700);
        }

        //TURN LEFT

         if (myGamepad->buttons() == 0x0004) {
          Serial.println("buttons X turn left");

          analogWrite(LF1, 0);
          analogWrite(LF2, 100);

          
          analogWrite(LB1, 0);
          analogWrite(LB2, 100);
        
          analogWrite(RF1, 100);
          analogWrite(RF2, 0);

          analogWrite(RB1, 100);
          analogWrite(RB2, 0);


          //vTaskDelay(700);
        }

      //TURN RIGHT

         if (myGamepad->buttons() == 0x0002) {
          Serial.println("buttons B turn right");

          analogWrite(LF1, 100);
          analogWrite(LF2, 0);

          analogWrite(LB1, 100);
          analogWrite(LB2, 0);
        
          analogWrite(RF1, 0);
          analogWrite(RF2, 100);

          analogWrite(RB1, 0);
          analogWrite(RB2, 100);


          //vTaskDelay(700);
        }

         if (myGamepad->buttons() == 0x0080) {
          Serial.println("Honk");
          tone(BUZZER_PIN, 800);
          delay(500);
          noTone(BUZZER_PIN);
          
         }

          if (myGamepad->buttons() == 0x0001) {
            PELTIER_ST=!PELTIER_ST;
            digitalWrite(PELTIER, PELTIER_ST);
            Serial.print("PELTIER are: ");
            Serial.println(PELTIER_ST);
            vTaskDelay(1000);
          
            
            

          }

         
         
          



      }

    }
  }

  // The main loop must have some kind of "yield to lower priority task" event.
  // Otherwise the watchdog will get triggered.
  // If your main loop doesn't have one, just add a simple `vTaskDelay(1)`.
  // Detailed info here:
  // https://stackoverflow.com/questions/66278271/task-watchdog-got-triggered-the-tasks-did-not-reset-the-watchdog-in-time

  vTaskDelay(50);
  // delay(200);
}