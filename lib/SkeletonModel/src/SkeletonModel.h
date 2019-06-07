#ifndef SkeletonModel_h
#define SkeletonModel_h

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
  #include "WConstants.h"
#endif

// Skeleton Message property names
#define MESSAGE_ID_LABEL    "messageId"
#define DEVICE_ID_LABEL     "deviceId"
#define TIMESTAMP_ID_LABEL  "epochTime"
#define MESSAGE_TYPE_LABEL  "messageType"
#define MESSAGE_LABEL       "payload"

// Skeleton message type codes
#define PAYLOAD_MSG_CODE  1001 // default message type for communication
#define SIGNAL_MSG_CODE   1002
#define ERROR_MSG_CODE    1003

// Skeleton Signal type codes
#define SLEEP_SGN_CODE          9001 // The device signals when it is going into deep sleep mode.
#define READY_SGN_CODE          9002 // The device signals every time it is finished running the program - after deep sleep and reboots 
#define UNCONFIGURED_SGN_CODE   9003 // If the configuration file was not found (or misformatted) on the device - SkeletonDevice data
#define ACKNOWLEDGE_SGN_CODE    9004 // Send an ACK signal to the server when a message was received by the device - message is always the received message id;

struct SkeletonSignal {
  const char* code;
  const char* message;
} ;

struct SkeletonWifi {
  const char* ssid;
  const char* password;
} ;

struct SkeletonMqtt {
  const char* server;
  int port = 1883;
  const char* user;
  const char* password;
} ;

struct SkeletonConfig {
  SkeletonWifi wifi;
  SkeletonMqtt mqtt;
} ;

struct SkeletonDevice {
  const char* deviceType;
  const char* deviceId;
  SkeletonConfig config;
} ;

class SkeletonModel {
  public:
    SkeletonModel();
  private:
   
};

#endif