#include "SkeletonFS.h"

// TODO Check if using DynamicJsonDocument would be better instead of StaticJsonDocument

SkeletonFS::SkeletonFS(){
  if (SPIFFS.begin()){
    DEBUG_PRINTLN("SkeletonFS: SPIFFS Active, FS mounted sucessfully.");
  }  //SPIFFS Active, FS mounted sucessfully
}

void SkeletonFS::setConfigFile(char* configFilePath)
{
    _configFilePath = configFilePath;
    DEBUG_PRINT("SkeletonFS: Configfile path is set to ");  
    DEBUG_PRINTLN(_configFilePath);
}

bool SkeletonFS::verifyConfigFilePath()
{
  bool fileExists = false;
  if (SPIFFS.exists(_configFilePath)){
    fileExists = true;
    DEBUG_PRINTLN("SkeletonFS: Config file exists");
  }
  else
  {
    DEBUG_PRINTLN("SkeletonFS: Config file missing");
  }
  return fileExists;
}

SkeletonDevice SkeletonFS::loadConfigFromSPIFFS()
{
      SkeletonDevice deviceData;
      File f = SPIFFS.open(_configFilePath, "r");
      if (!f) {
        // Unable to open for reading
        DEBUG_PRINTLN("SkeletonFS: Unable to open config for reading");
        return deviceData;
      } else {
        StaticJsonDocument<400> JSONdoc;
        String s;
        while (f.position() < f.size())
        {
          s = f.readStringUntil('\n');
          // Deserialize the JSON document
          DeserializationError error = deserializeJson(JSONdoc, s);
          // Test if parsing succeeds.
          if (error) {
            // Serial.print(F("deserializeJson() failed: "));
            // Serial.println(error.c_str());
            DEBUG_PRINTLN("SkeletonFS: deserializeJson() failed");
            DEBUG_PRINTLN(error.c_str());
            return deviceData;
          }
          // Parse JSONDoc into SkeletonDevice struct
          deviceData.deviceType =  JSONdoc["deviceType"];
          deviceData.deviceId = JSONdoc["deviceId"];
          DEBUG_PRINTLN("SkeletonFS: deviceId value is ");
          DEBUG_PRINTLN(deviceData.deviceId);
          deviceData.config.wifi.ssid = JSONdoc["config"]["wifi"]["ssid"];
          deviceData.config.wifi.password = JSONdoc["config"]["wifi"]["password"];
          deviceData.config.mqtt.server = JSONdoc["config"]["mqtt"]["server"];
          deviceData.config.mqtt.port = atoi(JSONdoc["config"]["mqtt"]["port"]);
          // TODO test if parsing not presented variables are cause an exception
          // deviceData.config.mqtt.user = JSONdoc["config"]["mqtt"]["user"];
          // deviceData.config.mqtt.password = JSONdoc["config"]["mqtt"]["password"];
        }
      }
      f.close();
      return deviceData;
}

void SkeletonFS::updateConfigInSPIFFS(SkeletonDevice config)
{
  DEBUG_PRINTLN("updateConfigInSPIFFS START");
  String serializedConfig;
  StaticJsonDocument<400> JSONdoc;
  // Serialize SkeletonDevice into JSONdoc struct
  JSONdoc["deviceType"] = config.deviceType;
  JSONdoc["deviceId"] = config.deviceId;
  JSONdoc["config"]["wifi"]["ssid"] = config.config.wifi.ssid;
  JSONdoc["config"]["wifi"]["password"] = config.config.wifi.password;
  JSONdoc["config"]["mqtt"]["server"] = config.config.mqtt.server;
  char text[8];
  sprintf(text,"%d",config.config.mqtt.port);
  JSONdoc["config"]["mqtt"]["port"] = text;

  serializeJson(JSONdoc, serializedConfig);

  // "w" mode should truncate the file so it should be empty
  // TODO confirm
  File f = SPIFFS.open(_configFilePath, "w");
  if (f) {
    // If \n not working try with println, it will ad \r\n
    f.print(serializedConfig + "\n");
  }
   f.close();
}
 /*
float SkeletonMessage::getFade()
{
  unsigned long currentTime = millis();
  float currentValue = lerp(_startTime, _startValue, _stopTime, _stopValue, currentTime);
  currentValue = constrain(currentValue, _startValue, _stopValue);
  return currentValue;
}
 
// Linear interpolation
float SkeletonMessage::lerp (float m1, float M1, float m2, float M2, float v1)
{
  float d  = M1 - m1;
  float c  = (M2 - m2) / d;
  float o = ( -(M2 * m1) + (m2 * m1) + m2 * d) / d;
  return v1 * c + o;
}
*/