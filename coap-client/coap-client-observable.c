#include "contiki.h"
#include "coap-engine.h"

/* Log configuration */
#include "sys/log.h"
#define LOG_MODULE "CoAP Client Observable"
#define LOG_LEVEL LOG_LEVEL_APP

#define SERVER_ENDPOINT "coap://[fe80::201:1:1:1]"
#define OBS_RESOURCE_URI "sensor/temperature/obs"
#define TOGGLE_INTERVAL (30 * CLOCK_SECOND)

static coap_observee_t *observee;

PROCESS(coap_client_observable, "CoAP Client Observable");
AUTOSTART_PROCESSES(&coap_client_observable);

static void
notification_callback(coap_observee_t *observee,
                      void *notification,
                      coap_notification_flag_t flag)
{
    int length = 0;
    const uint8_t *payload = NULL;

    LOG_INFO("Notification handler.\n");
    LOG_INFO("Observee URI: %s\n", observee->url);

    if(notification) {
        length = coap_get_payload(notification, &payload);
    }

    switch(flag) {
        case NOTIFICATION_OK:
            LOG_INFO("NOTIFICTION OK: %*s\n", length, (char *)payload);
            break;
        case OBSERVE_OK:
            LOG_INFO("OBSERVE_OK: %*s\n", length, (char *)payload);
            break;
        case OBSERVE_NOT_SUPPORTED:
            LOG_INFO("OBSERVE_NOT_SUPPORTED: %*s\n", length, (char *)payload);
            observee = NULL;
            break;
        case ERROR_RESPONSE_CODE:
            LOG_INFO("ERROR_RESPONSE_CODE: %*s\n", length, (char *)payload);
            observee = NULL;
            break;
        case NO_REPLY_FROM_SERVER:
            LOG_INFO("NO_REPLY_FROM_SERVER: removing observe registration with token %x%x\n",
                     observee->token[0], observee->token[1]);
            observee = NULL;
            break;
    }
}

void
toggle_observation(void)
{
    static coap_endpoint_t server_endpoint;

    if(observee) {
        LOG_INFO("Stopping observation.\n");
        coap_obs_remove_observee(observee);
        observee = NULL;

        return;
    }

    LOG_INFO("Starting observation.\n");
    coap_endpoint_parse(SERVER_ENDPOINT, strlen(SERVER_ENDPOINT), &server_endpoint);

    observee = coap_obs_request_registration(&server_endpoint,
                                             OBS_RESOURCE_URI,
                                             notification_callback,
                                             NULL);
}

PROCESS_THREAD(coap_client_observable, ev, data)
{
    static struct etimer timer;

    PROCESS_BEGIN();

    etimer_set(&timer, TOGGLE_INTERVAL);
    toggle_observation();

    while(1) {
        PROCESS_YIELD();

        if(etimer_expired(&timer)) {
            LOG_INFO("Toggle timer\n");
            toggle_observation();
            etimer_reset(&timer);
        }
    }

    PROCESS_END();

}