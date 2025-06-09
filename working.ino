// MUST be defined BEFORE including Blynk library
#define BLYNK_TEMPLATE_NAME "IOT PROJECT"
#define BLYNK_TEMPLATE_ID "TMPL6P2UKK_bs"
#define BLYNK_AUTH_TOKEN "nmOEg6NWQ4JmgsbCpJ-iZwfm-ejij6pr"

#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

#define WIFI_SSID "vivo Y18"
#define WIFI_PASS "kobe123456"
#define DHTPIN 5         // GPIO4 for DHT data
#define DHTTYPE DHT22    // Sensor type
#define BUZZER_PIN 15    // GPIO15 for buzzer
#define WATERPUMP_PIN 16       // GPIO16 for fan relay
#define LED_PIN 17       // GPIO17 for LED
#define TIMER_LED_PIN 18 // GPIO18 for timer-controlled LED
#define TEMP_THRESHOLD 30.0  // Temperature threshold
#define LIGHT_THRESHOLD 50   // Light percentage threshold for LED
#define TIMER_DURATION 180000 // 3 minutes in milliseconds (3 * 60 * 1000)

DHT dht(DHTPIN, DHTTYPE);
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
BlynkTimer timer;

// State variables
bool tempAlertActive = false;
bool lastTempStateBelowThreshold = true;
bool ledLowLightOn = false;
bool timerLEDActive = false;
unsigned long timerStartTime = 0;

// Blynk virtual pins
#define TIMER_LED_CONTROL V4
#define TIMER_STATUS V5

void readSensors() {
  // Read temperature and humidity
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Celsius

  // Read light intensity and convert to percentage
  sensors_event_t event;
  tsl.getEvent(&event);
  float lux = event.light;
  
  // Map lux to percentage (0-1000 lux → 0-100%)
  float lightPercent = 0;
  if (lux >= 0) {
    lightPercent = map(constrain(lux, 0, 1000), 0, 1000, 0, 100);
  } else {
    lightPercent = -1; // Error value
  }

  // Check if readings failed
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read DHT sensor!");
    return;
  }
  
  // Send to Blynk
  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, humidity);
  Blynk.virtualWrite(V3, lightPercent);
  
  // Serial monitor output
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print("°C\tHum: ");
  Serial.print(humidity);
  Serial.print("%\tLight: ");
  Serial.print(lightPercent);
  Serial.println("%");
  Serial.print("%\tTimer LED: ");
  Serial.println(timerLEDActive ? "ON" : "OFF");

  // FIRE ALARM logic (temperature threshold)
  if (temperature >= TEMP_THRESHOLD) {
    if (lastTempStateBelowThreshold) {
      // Just crossed threshold - activate fire alarm
      digitalWrite(BUZZER_PIN, HIGH);
      digitalWrite(WATERPUMP_PIN, HIGH);
      digitalWrite(LED_PIN, HIGH);
      Blynk.logEvent("fire_alarm", String("🔥 FIRE ALARM! Temperature reached ") + temperature + "°C!");
      tempAlertActive = true;
      lastTempStateBelowThreshold = false;
    }
  } else {
    if (!lastTempStateBelowThreshold) {
      // Temperature dropped below threshold - deactivate alarm
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(FAN_PIN, LOW);
      digitalWrite(LED_PIN, LOW);
      tempAlertActive = false;
      lastTempStateBelowThreshold = true;
    }
  }

  // Light level LED control (only if not in fire alarm mode)
  if (!tempAlertActive) {
    if (lightPercent < LIGHT_THRESHOLD && lightPercent >= 0) {
      digitalWrite(LED_PIN, HIGH);  // Turn on LED for low light
    } else {
      digitalWrite(LED_PIN, LOW);
    }
  }
}

 // Function to handle timer LED updates
void updateTimerLED() {
  if (timerLEDActive) {
    // Update remaining time in Blynk app
    unsigned long remaining = (TIMER_DURATION - (millis() - timerStartTime)) / 1000;
    Blynk.virtualWrite(TIMER_STATUS, String("ON - ") + remaining + "s left");
    
    // Check if timer has expired
    if (millis() - timerStartTime >= TIMER_DURATION) {
      // Timer expired - turn off LED
      digitalWrite(TIMER_LED_PIN, LOW);
      timerLEDActive = false;
      
      // Update Blynk app
      Blynk.virtualWrite(TIMER_LED_CONTROL, 0);
      Blynk.virtualWrite(TIMER_STATUS, "OFF (Auto)");
      
      Serial.println("Timer LED turned off after 3 minutes");
    }
  }
}

// Blynk app button handler for timer LED
BLYNK_WRITE(TIMER_LED_CONTROL) {
  int value = param.asInt(); // 0 or 1 from button
  
  if (value == 1) {
    // Turn on timer LED
    digitalWrite(TIMER_LED_PIN, HIGH);
    timerLEDActive = true;
    timerStartTime = millis();
    
    // Update Blynk status
    Blynk.virtualWrite(TIMER_STATUS, "ON - 180s left");
    
    Serial.println("Timer LED turned on via Blynk");
  } else {
    // Manual turn off
    digitalWrite(TIMER_LED_PIN, LOW);
    timerLEDActive = false;
    
    // Update Blynk status
    Blynk.virtualWrite(TIMER_STATUS, "OFF (Manual)");
    
    Serial.println("Timer LED turned off manually via Blynk");
  }
}

// Blynk app button handler for direct LED control
BLYNK_WRITE(V5) {
  int value = param.asInt(); // 0 or 1 from button
  
  // Control the SAME LED as the timer
  digitalWrite(TIMER_LED_PIN, value);
  
  // Update timer state to match
  timerLEDActive = (value == 1);
  
  if (value == 1) {
    timerStartTime = millis();
    Blynk.virtualWrite(TIMER_STATUS, "ON - Manual");
    Serial.println("Timer LED turned on via V5");
  } else {
    Blynk.virtualWrite(TIMER_STATUS, "OFF (Manual)");
    Serial.println("Timer LED turned off via V5");
  }
  
  // Sync the V4 button state
  Blynk.virtualWrite(TIMER_LED_CONTROL, value);
}

void setup() {
  Serial.begin(115200);
  
  // Initialize sensors
  dht.begin();
  if (!tsl.begin()) {
    Serial.println("Could not find TSL2561 sensor!");
    while (1);
  }
  
  // Configure light sensor
  tsl.enableAutoRange(true);
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);
  
  // Initialize control pins
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(TIMER_LED_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(FAN_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(TIMER_LED_PIN, LOW);

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS);
  Serial.println("Connected to Blynk");
  
  // Setup periodic reading (every 2 seconds)
  timer.setInterval(2000L, readSensors);
  timer.setInterval(1000L, updateTimerLED);    // Timer LED updates every 1 second


  Blynk.virtualWrite(TIMER_STATUS, "OFF");

}

void loop() {
  Blynk.run();
  timer.run();
}