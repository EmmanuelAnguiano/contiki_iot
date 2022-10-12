#include "contiki.h"
#include "coap-engine.h"
#include "coap-blocking-api.h"

#include "coap-log.h"
#define LOG_MODULE "CoAP Client"
#define LOG_LEVEL LOG_LEVEL_APP

#define SERVER_ENDPOINT "coap://[fe80::201:1:1:1]"
#define REQUEST_INTERVAL 10

PROCESS(coap_client, "CoAP Client");
AUTOSTART_PROCESSES(&coap_client);

void
client_chunk_handler(coap_message_t *response)
{
    const uint8_t *chunk;

    if(response == NULL) {
        LOG_INFO("Request timed out.\n");
        return;
    }

    int length = coap_get_payload(response, &chunk);

    LOG_INFO("Response payload: ");
    LOG_INFO_COAP_STRING((char *)chunk, length);
    LOG_INFO_("\n");
}

PROCESS_THREAD(coap_client, ev, data)
{
    static coap_endpoint_t server_endpoint;
    static coap_message_t request[1];
    static struct etimer timer;

    PROCESS_BEGIN();

    coap_endpoint_parse(SERVER_ENDPOINT, strlen(SERVER_ENDPOINT), &server_endpoint);

    etimer_set(&timer, REQUEST_INTERVAL * CLOCK_SECOND);

    while(1) {
        PROCESS_YIELD();

        if(etimer_expired(&timer)) {
            LOG_INFO("Timer triggered a request to: ");
            LOG_INFO_COAP_EP(&server_endpoint);
            LOG_INFO_("\n");

            /* Initialize CoAP message */
            coap_init_message(request, COAP_TYPE_CON, COAP_GET, coap_get_mid());
            // coap_set_header_uri_path(request, "sensor/temperature/obs");
            coap_set_header_uri_path(request, "sensor/humidity");
            // coap_set_header_uri_path(request, ".well-known/core");

            COAP_BLOCKING_REQUEST(&server_endpoint, request, client_chunk_handler);

            LOG_INFO("Done with request.\n");
            etimer_reset(&timer);
        }
    }

    PROCESS_END();
}
