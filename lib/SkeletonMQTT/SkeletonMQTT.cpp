#include "SkeletonMQTT.h"

// *********************** //
// ------- PUBLIC ------- //
// *********************** //

SkeletonMQTT::SkeletonMQTT(Client &client)
{
  PubSubClient _pubSubClient(client);
}
void SkeletonMQTT::setConfig(SkeletonDevice deviceData)
{
  DEBUG_PRINTLN("SkeletonMQTT: setConfig() INIT");
  _deviceId = deviceData.deviceId;
  DEBUG_PRINT("SkeletonMQTT: _deviceId set to ");
  DEBUG_PRINTLN(_deviceId);
  _mqttServer = deviceData.config.mqtt.server;
  _mqttPort = deviceData.config.mqtt.port;
  _mqttUser = deviceData.config.mqtt.user;
  _mqttPassword = deviceData.config.mqtt.password;
  _randomId = String(ESP.getChipId(), HEX);
  DEBUG_PRINT("SkeletonMQTT: _randomId(ChipId) set to ");
  DEBUG_PRINTLN(_randomId);
  _pubSubClient.setServer(_mqttServer, _mqttPort);
  DEBUG_PRINTLN("SkeletonMQTT: setConfig() END");
}

void SkeletonMQTT::connect()
{
  DEBUG_PRINTLN("SkeletonMQTT: connect() INIT");
  while (!_pubSubClient.connected())
  {
    DEBUG_PRINTLN("SkeletonMQTT: _pubSubClient not connected");
    String devId = String(_deviceId);
    DEBUG_PRINT("SkeletonMQTT: devId is ");
    DEBUG_PRINTLN(devId);
    String clientId = devId + "-" + _randomId;
    DEBUG_PRINT("SkeletonMQTT: clientId is ");
    DEBUG_PRINTLN(clientId);
    DEBUG_PRINT("SkeletonMQTT: clientId is ");
    DEBUG_PRINTLN(clientId.c_str());
    if (_pubSubClient.connect(clientId.c_str(), _mqttUser, _mqttPassword))
    {
      DEBUG_PRINTLN("Connection re-established");
    }
    else
    {
      DEBUG_PRINT("\tConnection failed, rc=");
      DEBUG_PRINT(_pubSubClient.state());
      DEBUG_PRINTLN(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  DEBUG_PRINTLN("SkeletonMQTT: connect() END");
}

void SkeletonMQTT::subscribe(const char *topicRoot, bool isConfigTopic = false, bool isSignalTopic = false, bool isDataTopic = false)
{
  DEBUG_PRINTLN("SkeletonMQTT: subscribe() INIT");
  if (isConfigTopic)
  {
    char *str3 = (char *)malloc(1 + strlen(topicRoot) + strlen(CONFIG_SUBTOPIC_NAME));
    strcpy(str3, topicRoot);
    strcat(str3, "/");
    strcat(str3, CONFIG_SUBTOPIC_NAME);
    _configTopic = str3;
    _pubSubClient.subscribe(str3);
  }

  if (isSignalTopic)
  {
    char *str3 = (char *)malloc(1 + strlen(topicRoot) + strlen(SIGNAL_SUBTOPIC_NAME));
    strcpy(str3, topicRoot);
    strcat(str3, "/");
    strcat(str3, SIGNAL_SUBTOPIC_NAME);
    _signalTopic = str3;
    _pubSubClient.subscribe(str3);
  }

  if (isDataTopic)
  {
    char *str3 = (char *)malloc(1 + strlen(topicRoot) + strlen(DATA_SUBTOPIC_NAME));
    strcpy(str3, topicRoot);
    strcat(str3, "/");
    strcat(str3, DATA_SUBTOPIC_NAME);
    _dataTopic = str3;
    _pubSubClient.subscribe(str3);
  }
  DEBUG_PRINTLN("SkeletonMQTT: subscribe() END");
}

void SkeletonMQTT::subscribeToTopic(const char *topic)
{
  _pubSubClient.subscribe(topic);
}

bool SkeletonMQTT::sendSignal(SkeletonSignal signal, const char* topic)
{
  bool publishSuccess = false;
  // Generate signal message
  StaticJsonDocument<400> JSONdoc;
  JSONdoc.clear();
  JSONdoc[MESSAGE_ID_LABEL] = String(random(0xffff), HEX);
  JSONdoc[DEVICE_ID_LABEL] = _deviceId;
  JSONdoc[TIMESTAMP_ID_LABEL] = _skeletonUtils.getEpochTime();
  JSONdoc[MESSAGE_LABEL]["code"] = signal.code;
  JSONdoc[MESSAGE_LABEL]["message"] = signal.message;
  JSONdoc[MESSAGE_TYPE_LABEL] = (char *)SIGNAL_MSG_CODE;
  serializeJson(JSONdoc, _JSONmessageBuffer);
  connect();
  // Send generated message to topic
  publishSuccess = _pubSubClient.publish(topic, _JSONmessageBuffer);
  return publishSuccess;
}

bool SkeletonMQTT::sendSignal(SkeletonSignal signal)
{
  return sendSignal(signal, _signalTopic);
}

/* 
bool SkeletonMQTT::publishStandbySignal(const char* parentTopic, unsigned long recurring_seconds)
{
  bool publishSuccess = false;
  if(recurring_seconds == 0){
    char* JSONmessage = generateSignalMessage();
    connect();
    publishSuccess = _pubSubClient.publish(_signalTopic, JSONmessage);
  }
  else{
    if(recurring_seconds == 60){
      if(_timeClient.getEpochTime() % 60 == 0){
        char* JSONmessage = generateSignalMessage();
        connect();
        publishSuccess = _pubSubClient.publish(_signalTopic, JSONmessage);
      }
    }
  }
  return publishSuccess;
}
*/