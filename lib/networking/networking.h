#ifndef _NETWORKING
#define _NETWORKING

// https://esp32.com/viewtopic.php?t=28570
// https://github.com/espressif/esp-idf/blob/master/examples/protocols/http_request/main/http_request_example_main.c

#include <Arduino.h>

#include <WiFi.h>
#include <esp_http_client.h>
#include <esp_transport_ssl.h>

#include <memory>

#include "server_certificate.h"
#include "slice.h"
#include "string_help.h"
#include "serial_io.h"

#include "result.h"
#include "method.h"

struct HttpResponse
{
    Option<std::vector<uint8_t>> data;
};

class HttpClient
{
    class HttpWorker
    {
    private:
        using ByteBuffer = std::vector<uint8_t>;

        static esp_err_t _event_handler(esp_http_client_event_t *evt) noexcept
        {
            static bool new_request = true;

            ByteBuffer &output_buffer = *static_cast<ByteBuffer *>(evt->user_data);

            switch (evt->event_id)
            {
            case HTTP_EVENT_ON_DATA:
            {
                if (esp_http_client_is_chunked_response(evt->client))
                {
                    break;
                }

                if (new_request)
                {
                    output_buffer.clear();
                    new_request = false;
                }

                const Slice<uint8_t> data = Slice<uint8_t>(*static_cast<uint8_t *>(evt->data), evt->data_len);

                output_buffer.insert(output_buffer.end(), data.begin(), data.end());

                break;
            }

            case HTTP_EVENT_ON_FINISH:
            {
                new_request = true;
                break;
            }

            default:
            {
                break;
            }
            }

            return ESP_OK;
        }

        static constexpr esp_http_client_config_t _DEFAULT_CONFIG = {.url = "",
                                                                     .cert_pem = "",
                                                                     .cert_len = sizeof(""),
                                                                     .timeout_ms = 5000,
                                                                     .event_handler = HttpWorker::_event_handler,
                                                                     .user_data = nullptr,
                                                                     .is_async = true};

    public:
        HttpWorker() : _client{std::shared_ptr<esp_http_client>(esp_http_client_init(&_DEFAULT_CONFIG), esp_http_client_cleanup)}
        {
        }

    private:
        std::shared_ptr<esp_http_client> _client;
    };

    using SharedWorker = std::shared_ptr<HttpWorker>;

public:
    explicit HttpClient() : _worker{std::make_shared<HttpWorker>()} {}

    class Request
    {
    private:
        friend class HttpClient;

        explicit Request(
            SharedWorker worker,
            HttpMethod method,
            str url,
            str certificate,
            uint16_t timeout) : _worker{worker}, _method{method}, _url{url}, _certificate{certificate}, _timeout{timeout} {}

        SharedWorker _worker;
        HttpMethod _method;
        str _url;
        str _certificate;
        uint16_t _timeout;
    };

    Request request(HttpMethod method, str url, str certificate, uint16_t timeout = 5000)
    {
        return Request{
            _worker,
            method,
            url,
            certificate,
            timeout,
        };
    }

private:
    SharedWorker _worker;
};

void https_async()
{
    esp_http_client_handle_t client = esp_http_client_init(&config);
    // const char *post_data = "Using a Palantír requires a person with great strength of will and wisdom. The Palantíri were meant to "
    //                         "be used by the Dúnedain to communicate throughout the Realms in Exile. During the War of the Ring, "
    //                         "the Palantíri were used by many individuals. Sauron used the Ithil-stone to take advantage of the users "
    //                         "of the other two stones, the Orthanc-stone and Anor-stone, but was also susceptible to deception himself.";
    esp_http_client_set_method(client, HTTP_METHOD_GET);
    // esp_http_client_set_post_field(client, post_data, strlen(post_data));

    esp_err_t err = ESP_ERR_HTTP_EAGAIN;
    while (err == ESP_ERR_HTTP_EAGAIN)
    {
        err = esp_http_client_perform(client);
    }

    if (err == ESP_OK)
    {
        // ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %" PRId64,
        //          esp_http_client_get_status_code(client),
        //          esp_http_client_get_content_length(client));
    }
    else
    {
        debugln(esp_err_to_name(err));
        // ESP_LOGE(TAG, "Error perform http request %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);

    for (const uint8_t byte : output_buffer)
    {
        print(static_cast<char>(byte));
    }

    print("\n");
    println("Done!");
}

// void setup()
// {
//     debugln("Hello, world!");

//     WiFi.begin("Ang Network", "KZaccxEf");

//     while (WiFi.status() != WL_CONNECTED)
//     {
//         debug(".");
//         delay(500);
//     }

//     debugln("\nConnected!");
//     debugln("Starting http request...");
//     debugln("1-2-3-4-5");

//     https_async();
// }

// void loop()
// {
// }

#endif