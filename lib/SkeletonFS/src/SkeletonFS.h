#ifndef SkeletonFS_h
#define SkeletonFS_h

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
  #include "WConstants.h"
#endif
#include "FS.h"
#include "ArduinoJson.h"
#include "string.h"
#include "SkeletonModel.h"

// Uncomment to enable printing out nice debug messages.
#define SKELETON_FS_DEBUG

// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef SKELETON_FS_DEBUG
  #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
  #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
  #define DEBUG_PRINT(...) {}
  #define DEBUG_PRINTLN(...) {}
#endif

class SkeletonFS {
  public:
    SkeletonFS();
    
    void setConfigFile(char* configFilePath);

    bool verifyConfigFilePath();

    SkeletonDevice loadConfigFromSPIFFS();
    
    // Writing file in SPIFFS wears aways the hardware so use this wisely.
    void updateConfigInSPIFFS(SkeletonDevice config);
 
  private:
    char* _configFilePath;
    SkeletonDevice _deviceData;
    //float lerp(float m1, float M1, float m2, float M2, float v1);
};

#endif