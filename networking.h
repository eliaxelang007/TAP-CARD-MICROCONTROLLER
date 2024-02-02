#ifndef _NETWORKING
#define _NETWORKING

#include <WiFi.h>
#include <HTTPClient.h>

#include "serial_io.h"
#include "uid.h"

struct ServerResponse
{
    int status;
    String response;
};

class ServerApi
{
public:
    ServerApi(const String server_address) : server_address{server_address} {}

    void initialize(String ssid, String password, size_t channel = 0)
    {
        debug("Connecting WiFi");
        WiFi.begin(ssid, password, channel);

        while (WiFi.status() != WL_CONNECTED)
        {
            debug(".");
            delay(500);
        }

        debugln("Connected!");
    }

    ServerResponse send_tap(Uid uid, uint32_t timeout_ms)
    {
        HTTPClient client;

        debugln("Connected to server.");

        String request = server_address + "/log/" + uid.to_string();

        client.begin(request);

        client.setTimeout(timeout_ms);

        int http_status = client.GET();

        if (http_status != HTTP_CODE_OK)
        {
            debug("Status: ");
            debugln(HTTPClient::errorToString(http_status));

            return ServerResponse{
                .status = http_status,
                .response = ""};
        }

        String response = client.getString();

        debug("Received: ");
        debugln(response);

        client.end();

        return ServerResponse{
            .status = http_status,
            .response = response};
    }

private:
    const String server_address;
};

#endif
