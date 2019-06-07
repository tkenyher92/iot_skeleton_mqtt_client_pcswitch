#ifndef SkeletonUtils_h
#define SkeletonUtils_h

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
  #include "WConstants.h"
#endif
#include "ESP8266WiFi.h"
#include "NTPClient.h"
#include "WiFiUdp.h"

// Uncomment to enable printing out nice debug messages.
//#define SKELETON_UTILS_DEBUG

// Define where debug output will be printed.
#define DEBUG_PRINTER Serial

// Setup debug printing macros.
#ifdef SKELETON_UTILS_DEBUG
  #define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
  #define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
  #define DEBUG_PRINT(...) {}
  #define DEBUG_PRINTLN(...) {}
#endif

// NTPClient parameters
#define NTP_OFFSET   60 * 60 * 1         // UTC + 1 
#define NTP_UPDATE_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "europe.pool.ntp.org"

//#define STANDBY_SIGNAL "SG_AWAKE"

class SkeletonUtils {
  public:
    SkeletonUtils();

    // Methods
    // TODO make it static if possible??? - test out
    /**
     * @return time in seconds since Jan. 1, 1970 - converted into char array
     */
    char* getEpochTime();
   
   /**
     * Configure the wifi connection and initate a connection. 
     */
   void setupWifi(const char* ssid, const char* password);
 
  private:
    WiFiUDP ntpUDP;
    NTPClient _timeClient = NTPClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_UPDATE_INTERVAL);
};

#endif