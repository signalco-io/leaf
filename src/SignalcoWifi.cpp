#include "SignalcoWifi.h"

WiFiClient espClient;

u8_t configured = 0;
u8_t otaConfigured = 0;

const char *hostname;
const char *ssid;
const char *password;

SignalcoWifi::SignalcoWifi(SignalcoConfiguration *configuration)
{
    this->configuration = configuration;
}

void SignalcoWifi::setupWifi()
{
    auto hostname = (configuration->wifi).hostname;
    auto ssid = (configuration->wifi).ssid;
    auto password = (configuration->wifi).password;

    configured = true;
    Serial.println("WiFi: Configuration available. Connecting...");

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
    auto hostname = (configuration->wifi).hostname;
    auto otaPassword = (configuration->wifi).otaPassword;

    ArduinoOTA.setHostname(hostname);
    ArduinoOTA.setPassword(otaPassword);
    ArduinoOTA.begin();
    otaConfigured = 1;

    Serial.println("OTA configured");
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