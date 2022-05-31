#include "SignalcoWifi.h"

WiFiClient espClient;

u8_t configured = 0;
u8_t otaConfigured = 0;

const char *hostname;
const char *ssid;
const char *password;

SignalcoWifi::SignalcoWifi()
{
}

void SignalcoWifi::setupWifi()
{
    if (!preferences.isKey("hostname") ||
        !preferences.isKey("wifissid") ||
        !preferences.isKey("wifipassword"))
    {
        Serial.println("WiFi: Configuration unavailable.");
        return;
    }

    auto hostnameStr = preferences.getString("hostname");
    auto ssidStr = preferences.getString("wifissid");
    auto passwordStr = preferences.getString("wifipassword");
    hostname = hostnameStr.c_str();
    ssid = ssidStr.c_str();
    password = passwordStr.c_str();

    configured = true;
    Serial.println("WiFi: Configuration available. Connecting...");
    Serial.println("Host name: " + hostnameStr);
    Serial.println("SSID: " + ssidStr);

    WiFi.setHostname(hostname);
    WiFi.setAutoReconnect(true);
    WiFi.mode(WIFI_STA);

    auto result = WiFi.begin(ssid, password);
    delay(100);

    if (result != 3)
    {
        WiFi.reconnect();
    }

    // Wait for WIFI for 30s
    while (WiFi.status() != WL_CONNECTED && millis() < 15000)
    {
        Serial.print(".");
        delay(50);
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("WiFi: Connected. Local IP:" + WiFi.localIP().toString());
    }
    else
    {
        Serial.println("Failed to connect to WiFi, will try again later...");
    }

    // MDNS.begin(HOSTNAME)
}

void SignalcoWifi::setupOta()
{
    if (preferences.isKey("hostname") &&
        preferences.isKey("otapassword"))
    {
        ArduinoOTA.setHostname(preferences.getString("hostname").c_str());
        ArduinoOTA.setPassword(preferences.getString("otapassword").c_str());
        ArduinoOTA.begin();
        otaConfigured = 1;

        Serial.println("OTA configured");
    }
}
void SignalcoWifi::loop()
{
    if (otaConfigured > 0)
    {
        ArduinoOTA.handle();
    }
}

bool SignalcoWifi::isConfigured()
{
    return WiFi.isConnected();
}