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
  _deviceId = deviceData.deviceId;
  _mqttServer = deviceData.config.mqtt.server;
  _mqttPort = deviceData.config.mqtt.port;
  _mqttUser = deviceData.config.mqtt.user;
  _mqttPassword = deviceData.config.mqtt.password;
  _randomId = String(ESP.getChipId(), HEX);

  _pubSubClient.setServer(_mqttServer, _mqttPort);
}

void SkeletonMQTT::connect()
{
  while (!_pubSubClient.connected())
  {
    String devId = String(_deviceId);
    String clientId = devId + "-" + _randomId;
    if (_pubSubClient.connect(clientId.c_str(), _mqttUser, _mqttPassword))
    {
    }
    else
    {
      delay(5000);
    }
  }
}

void SkeletonMQTT::subscribe(const char *topicRoot, bool isConfigTopic = false, bool isSignalTopic = false, bool isDataTopic = false)
{
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