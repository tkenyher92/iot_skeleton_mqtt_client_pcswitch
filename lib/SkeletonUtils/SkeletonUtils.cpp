#include "SkeletonUtils.h"

// *********************** //
// ------- PUBLIC ------- //
// *********************** //

SkeletonUtils::SkeletonUtils()
{
    // Empty constructor
}

char *SkeletonUtils::getEpochTime()
{
    DEBUG_PRINTLN("SkeletonUtils: getEpochTime INIT");
    _timeClient.update();
    DEBUG_PRINTLN("SkeletonUtils: timecient updated");
    unsigned long epochTime = _timeClient.getEpochTime();
    DEBUG_PRINT("SkeletonUtils: epochTime(unsigned long):");
    DEBUG_PRINTLN(epochTime);
    char *buf = (char *)malloc(666);
    ltoa(epochTime, buf, 10);
    DEBUG_PRINTLN("SkeletonUtils: getEpochTime END");
    return buf;
}

void SkeletonUtils::setupWifi(const char *ssid, const char *password)
{
    delay(10);
    DEBUG_PRINTLN("SkeletonUtils: setupWifi INIT");
    DEBUG_PRINT("SkeletonUtils: Connecting to ");
    DEBUG_PRINTLN(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("SkeletonUtils: WiFi connected");
    DEBUG_PRINT("SkeletonUtils: IP address: ");
    DEBUG_PRINTLN(WiFi.localIP());
    DEBUG_PRINTLN("SkeletonUtils: setupWifi END");
}