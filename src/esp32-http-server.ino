#include <Arduino.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiClientSecure.h>
#include <Preferences.h>

const char *SSID = "Wokwi-GUEST";
const char *PASSWORD = "";

const char *BROKER_MQTT = "e89dd3b4d73248a29def221deeafac4c.s1.eu.hivemq.cloud";
const int BROKER_PORT = 8883;

const char *MQTT_USER = "ohmni";
const char *MQTT_PASSWORD = "Ohmni2024";

char ID_MQTT[13];
char TOPIC_SUBSCRIBE_ID[50];
char TOPIC_PUBLISH_DATA[50];

#define PIN_CORRENTE 36
#define PIN_TENSAO 39
#define PUBLISH_DELAY 500
LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiClientSecure espClient;
PubSubClient MQTT(espClient);
Preferences preferences;
unsigned long publishUpdate = 0;
bool conectado = false;

void updateSensorValues();
void initWiFi();
void initMQTT();
void callbackMQTT(char *topic, byte *payload, unsigned int length);
void reconnectMQTT();
void reconnectWiFi();
void checkWiFIAndMQTT();
void generateUniqueID();
void getStoredID();

void generateUniqueID()
{
  uint64_t chipId = ESP.getEfuseMac();
  uint32_t uniquePart = (uint32_t)(chipId & 0xFFFFFFFF);
  snprintf(ID_MQTT, sizeof(ID_MQTT), "%08X", uniquePart);
}

void getStoredID()
{
  preferences.begin("device", false);
  String storedID = preferences.getString("device_id", "");

  if (storedID == "")
  {
    generateUniqueID();
    preferences.putString("device_id", ID_MQTT);
    Serial.println("Novo ID gerado e salvo na NVS.");
  }
  else
  {
    storedID.toCharArray(ID_MQTT, sizeof(ID_MQTT));
    Serial.println("ID carregado da NVS.");
  }
  preferences.end();

  snprintf(TOPIC_SUBSCRIBE_ID, sizeof(TOPIC_SUBSCRIBE_ID), "iot/%s/connect", ID_MQTT);
  snprintf(TOPIC_PUBLISH_DATA, sizeof(TOPIC_PUBLISH_DATA), "iot/%s/data", ID_MQTT);
}

void clearStoredID()
{
  preferences.begin("device", false);
  if (preferences.remove("device_id"))
  {
    Serial.println("ID removido com sucesso!");
  }
  else
  {
    Serial.println("Nenhum ID encontrado para remover.");
  }
  preferences.end();
}

void updateSensorValues()
{
  float corrente = analogRead(PIN_CORRENTE) * (3.3 / 4095.0);
  float tensao = analogRead(PIN_TENSAO) * (3.3 / 4095.0);

  StaticJsonDocument<200> doc;
  doc["id"] = ID_MQTT;
  doc["corrente"] = corrente;
  doc["tensao"] = tensao;

  char buffer[200];
  serializeJson(doc, buffer);
  MQTT.publish(TOPIC_PUBLISH_DATA, buffer);
  Serial.println(buffer);
}

void initWiFi()
{
  Serial.print("Conectando ao WiFi: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado ao WiFi!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void initMQTT()
{
  espClient.setInsecure();
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(callbackMQTT);
}

void callbackMQTT(char *topic, byte *payload, unsigned int length)
{
  String msg = String((char *)payload).substring(0, length);
  Serial.printf("Mensagem recebida: %s do tópico: %s\n", msg.c_str(), topic);

  if (strcmp(topic, TOPIC_SUBSCRIBE_ID) == 0)
  {
    if (msg.equals("connect"))
    {
      conectado = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ID: ");
      lcd.print(ID_MQTT);
      MQTT.publish(TOPIC_SUBSCRIBE_ID, "Conectado!");
      Serial.println("Dispositivo conectado ao sistema!");
    }
    else if (msg.equals("disconnect"))
    {
      conectado = false;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Desconectado!");
      Serial.println("Dispositivo desconectado pelo servidor.");
      delay(2000);
    }
  }
}

void reconnectMQTT()
{
  while (!MQTT.connected())
  {
    Serial.print("Tentando conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);

    if (MQTT.connect(ID_MQTT, MQTT_USER, MQTT_PASSWORD))
    {
      Serial.println("Conectado ao Broker MQTT!");
      MQTT.subscribe(TOPIC_SUBSCRIBE_ID);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ID: ");
      lcd.print(ID_MQTT);
      lcd.setCursor(0, 1);
      lcd.print("Aguardando...");
    }
    else
    {
      int errorCode = MQTT.state();
      Serial.print("Falha ao conectar. Erro: ");
      Serial.println(errorCode);
      delay(2000);
    }
  }
}

void checkWiFIAndMQTT()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    reconnectWiFi();
  }
  if (!MQTT.connected())
  {
    reconnectMQTT();
  }
}

void reconnectWiFi()
{
  if (WiFi.status() == WL_CONNECTED)
    return;
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nReconectado ao WiFi!");
}

void setup()
{
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  getStoredID();
  pinMode(PIN_CORRENTE, INPUT);
  pinMode(PIN_TENSAO, INPUT);
  initWiFi();
  initMQTT();

  lcd.setCursor(0, 0);
  lcd.print("ID: ");
  lcd.print(ID_MQTT);
  lcd.setCursor(0, 1);
  lcd.print("Aguardando...");
}

void loop()
{
  checkWiFIAndMQTT();
  MQTT.loop();

  if (conectado)
  {
    lcd.setCursor(0, 1);
    lcd.print("Conectado!");
    if ((millis() - publishUpdate) >= PUBLISH_DELAY)
    {
      publishUpdate = millis();
      updateSensorValues();
    }
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ID: ");
    lcd.print(ID_MQTT);
    lcd.setCursor(0, 1);
    lcd.print("Aguardando...");
    delay(1000);
  }
}
