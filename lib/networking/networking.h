#ifndef _NETWORKING
#define _NETWORKING

#include <WiFi.h>
#include <HTTPClient.h>

#include "string_help.h"
#include "section.h"
#include "uid.h"

struct ServerResponse
{
    int status;
    String response;

    explicit ServerResponse(int status, String response) : status{status}, response{response} {}
};

class ServerApi
{
private:
    const char *_server_address;

public:
    explicit ServerApi(const char *server_address) : _server_address{server_address}
    {
    }

    void initialize(String ssid, String password)
    {
        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            debug(".");
        }

        debugln("Connected to the internet!");
    }

    ServerResponse send_tap(Uid uid, uint16_t timeout)
    {
        HTTPClient client;

        client.setTimeout(timeout);

        String address = build_string(
            _server_address,
            "/log/",
            uid.to_chars());

        client.begin(_server_address);

        int status = client.GET();
        String response = client.getString();

        client.end();

        return ServerResponse{
            status,
            response};
    }
};

#endif