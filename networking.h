#ifndef _NETWORKING
#define _NETWORKING

#include <WiFi.h>
#include <HTTPClient.h>
#include <algorithm>

#include "serial_io.h"
#include "section.h"
#include "strings.h"
#include "uid.h"

class ServerApi
{
public:
    static constexpr char SERVER_ADDRESS[] = "https://tap-card-server-production.up.railway.app";

    using ServerResponse = Result<String, int>;

    explicit ServerApi() {}

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

    ServerResponse send_tap(Section section, Uid uid, uint32_t timeout_ms)
    {
        HTTPClient client;

        debugln("Connected to server.");

        String request = build_string(
            SERVER_ADDRESS,
            "/log/",
            section.to_char(),
            '/',
            uid.to_chars());

        client.begin(request);

        client.setTimeout(timeout_ms);

        int http_status = client.GET();

        if (http_status < 200 || http_status >= 300)
        {
            debug("Status: ");
            debugln(HTTPClient::errorToString(http_status));

            return ServerResponse::err(http_status);
        }

        const String response = client.getString();

        debug("Received: ");
        debugln(response);

        client.end();

        return ServerResponse::ok(response);
    }
};

#endif
