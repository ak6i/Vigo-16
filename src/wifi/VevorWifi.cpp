#include "VevorWifi.h"
#include <WiFi.h>
#include "../log/Logger.h"
#include <ESPmDNS.h>

void VevorWifi::startWifi(VevorConfig *config, VevorST7735 *tft)
{
    tft->addStatusLine("Connecting Wifi...");
    WiFi.mode(WIFI_MODE_APSTA);
    WiFi.disconnect();
    WiFi.softAPdisconnect();

    if (WiFi.setHostname(config->getHostName().c_str()))
    {
        log_println("HostName: " + config->getHostName());
        tft->addStatusLine("HostName: " + config->getHostName());
    }
    else
    {
        log_println("Could not set HostName: " + config->getHostName());
        tft->addStatusLine("Could not set HostName: " + config->getHostName());
    }

    if (config->getStaSsid() != "" && config->getStaPassword() != "")
    {
        log_println("Connecting to STA " + config->getStaSsid() + "...");
        tft->addStatusLine("Connecting to STA " + config->getStaSsid() + "...");
        WiFi.begin(config->getStaSsid().c_str(), config->getStaPassword().c_str());
        wl_status_t status = (wl_status_t)WiFi.waitForConnectResult(5000);
        if (WL_CONNECTED == status)
        {
            log_println("STA " + config->getStaSsid() + ", IP " + WiFi.localIP().toString());
            tft->addStatusLine("STA " + config->getStaSsid() + ", IP " + WiFi.localIP().toString());
        }
        else
        {
            log_println("Could not connect STA " + config->getStaSsid() + "!");
            tft->addStatusLine("Could not connect STA " + config->getStaSsid() + "!");
        }
    }
    else
    {
        log_println("STA not configured, skipping.");
        tft->addStatusLine("STA not configured, skipping.");
    }

    log_println("Creating AP " + config->getApSsid() + "...");
    tft->addStatusLine("Creating AP " + config->getApSsid() + "...");
    if (WiFi.softAP(config->getApSsid().c_str(), config->getApPassword().c_str()))
    {
        log_println("AP " + config->getApSsid() + ", PW " + config->getApPassword() + ", IP " + WiFi.softAPIP().toString());
        tft->addStatusLine("AP " + config->getApSsid() + ", PW " + config->getApPassword() + ", IP " + WiFi.softAPIP().toString());
    }
    else
    {
        log_println("Could not start AP " + config->getApSsid() + "!");
        tft->addStatusLine("Could not start AP " + config->getApSsid() + "!");
    }

    if (WiFi.isConnected())
        tft->addStatusLine("STA " + config->getStaSsid() + ", IP " + WiFi.localIP().toString());
    else
        tft->addStatusLine("STA: not connected!");

    if (MDNS.begin(config->getHostName().c_str()))
    {
        log_println("MDNS: " + config->getHostName() + ".local");
        tft->addStatusLine("MDNS: " + config->getHostName() + ".local");
    }
    else
    {
        log_println("Could not start MDNS: " + config->getHostName() + ".local");
        tft->addStatusLine("MDNS: could not enable!");
    }
}