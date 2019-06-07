#ifndef SkeletonMQTT_h
#define SkeletonMQTT_h

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
  #include "WConstants.h"
#endif
#include "Client.h"
#include "PubSubClient.h"
#include "ArduinoJson.h"
#include "string.h"
#include "SkeletonUtils.h"
#include "SkeletonModel.h"

// Uncomment to enable printing out nice debug messages.
//#define SKELETON_MQTT_DEBUG

// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef SKELETON_MQTT_DEBUG
  #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
  #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
  #define DEBUG_PRINT(...) {}
  #define DEBUG_PRINTLN(...) {}
#endif

//#define STANDBY_SIGNAL "SG_AWAKE"

class SkeletonMQTT {
  public:
    SkeletonMQTT(Client& espWifiClient);
    
    void connect();

    void setConfig(SkeletonDevice deviceData);
    
    void subscribe(const char* parentTopic, bool configTopic, bool signalTopic, bool dataTopic);

    void subscribeToTopic(const char* topic);

    bool sendSignal(SkeletonSignal signal);

    bool sendSignal(SkeletonSignal signal, const char* topic);

    // Can be scheduled to run reoccuring or once. Based on the recurring_seconds parameter value.
    // If recurring_seconds is 0 it will publish only once and asap.
    // If recurring_seconds is greater than 0 it will publish reoccurringly with the given offset
    //bool publishStandbySignal(const char* parentTopic, unsigned long recurring_seconds);

    //void fadeTo(float value, unsigned long duration);
    //float getFade();
 
  private:
    SkeletonUtils _skeletonUtils;
    PubSubClient _pubSubClient;
    char _JSONmessageBuffer[128];
    const char* _deviceId;
    const char* _mqttServer;
    int _mqttPort;
    const char* _mqttUser;
    const char* _mqttPassword;
    String _randomId;

    // Sub Topics path name
    const char* CONFIG_SUBTOPIC_NAME = "configuration";
    const char* SIGNAL_SUBTOPIC_NAME = "signal";
    const char* DATA_SUBTOPIC_NAME = "data";

    // Sub Topics absolute - GENERATED
    const char* _configTopic;
    const char* _signalTopic;
    const char* _dataTopic;

};

#endif