#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SkeletonUtils.h>
#include <SkeletonMQTT.h>
#include <SkeletonFS.h>
#include "FS.h"

#define CONFIGFILE "/device.conf"

#define GPIO_OUT_SW 2

#define OUT_STATE_ACTIVE 1 // use define so it's simple to change if signal should be active high or low (NPN or PNP BJT)
#define OUT_STATE_INACTIVE (!OUT_STATE_ACTIVE)

#define OUT_TOGGLE_DURATION_MS 1500 // how long to keep the pwr pin (GPIO_OUT_SW) active when triggered

// Deprecated
#define DEVICE_TOPIC "Home/Room1/PC"

#define DEVICE_TOPIC_ROOT "Home/Room1/PC"

const char *deviceId;

const char *wifiSsid;
const char *wifiPassword;
const char *mqttServer;
int mqttPort = 1883;
char *mqttUser = "esp";
char *mqttPassword = "The3robotlaws";

WiFiClient espClient;
PubSubClient client(espClient);
StaticJsonDocument<400> JSONdoc;
SkeletonUtils skeletonUtils;
SkeletonMQTT skeletonMQTT(espClient);
SkeletonFS skeletonFS;
// To be read by skeletonFS
SkeletonDevice deviceData;

void callback(char *topic, byte *payload, unsigned int length)
{
  // TODO add timeout for the signal setting (about 30 seconds) to prevent stopping the PC while booting up

  payload[length] = '\0';
  //Serial.print("Message arrived in topic: ");
  //Serial.print("Message:");
  char *message = (char *)payload;
  //Serial.println(message);

  if (strcmp(message, "SWITCH") == 0)
  {
    // Parse message (payload) into JSON doc
    // Check if message is valid - target device ID and other data are correct
    Serial.println("SWITCH command detected!");
    SkeletonSignal signal;
    // TODO Do it with itoa??
    signal.code = (char *)ACKNOWLEDGE_SGN_CODE;
    signal.message = "ReceivedMSGID123231";
    skeletonMQTT.sendSignal(signal);
    // Generating Acknowledge JSON message
    JSONdoc.clear();
    JSONdoc["deviceId"] = deviceId;
    //char *p = skeletonUtils.getEpochTime();
    JSONdoc["epochTime"] = skeletonUtils.getEpochTime();
    JSONdoc["message"] = (char *)ACKNOWLEDGE_SGN_CODE;
    JSONdoc["messageType"] = (char *)SIGNAL_MSG_CODE;
    //free(p);
    char JSONmessageBuffer[128];
    serializeJson(JSONdoc, JSONmessageBuffer);
    skeletonMQTT.connect();
    Serial.println(JSONmessageBuffer);
    if (client.publish(DEVICE_TOPIC, JSONmessageBuffer) == true)
    {
      Serial.println("\tSuccess sending message");
    }
    else
    {
      Serial.println("\tError sending message");
    }

    delay(500);
    digitalWrite(GPIO_OUT_SW, OUT_STATE_ACTIVE);
    delay(OUT_TOGGLE_DURATION_MS);
    digitalWrite(GPIO_OUT_SW, OUT_STATE_INACTIVE);
    Serial.println();
  }
  Serial.println("-----------------------");
}

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(2000);
  pinMode(GPIO_OUT_SW, OUTPUT);
  delay(500);
  // Wait for serial to initialize.
  while (!Serial)
  {
  }
  Serial.println("Setup INIT");
  skeletonFS.setConfigFile(CONFIGFILE);
  if (skeletonFS.verifyConfigFilePath())
  {
    Serial.println("skeletonFS.verifyConfigFilePath: true");
    //deviceData = skeletonFS.loadConfigFromSPIFFS();
    Serial.println(deviceData.deviceId);
    Serial.println(deviceData.deviceType);
    Serial.println(deviceData.config.wifi.ssid);
    Serial.println(deviceData.config.wifi.password);
    Serial.println(deviceData.config.mqtt.server);
    Serial.println(deviceData.config.mqtt.port);
    Serial.println(deviceData.config.mqtt.user);
    Serial.println(deviceData.config.mqtt.password);
  }
  else
  {
    Serial.println("skeletonFS.verifyConfigFilePath: false");
  }
  //loadConfigFromSPIFFS();
  skeletonUtils.setupWifi(deviceData.config.wifi.ssid, deviceData.config.wifi.password);
  skeletonMQTT.setConfig(deviceData);
  //client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  skeletonMQTT.connect();
  skeletonMQTT.subscribe(DEVICE_TOPIC_ROOT, true, true, true);

  // Deprecated
  client.subscribe(DEVICE_TOPIC);

  Serial.println("Setup END");
}

void loop()
{
  client.loop();
}
