#include "contiki.h"
#include "coap-engine.h"

/* Log configuration */
#include "coap-log.h"
#define LOG_MODULE "CoAP Server"
#define LOG_LEVEL LOG_LEVEL_APP

/* Resources to be activated */
extern coap_resource_t res_temperature;
extern coap_resource_t res_humidity;
extern coap_resource_t res_temperature_obs;

PROCESS(coap_server, "CoAP Server");
AUTOSTART_PROCESSES(&coap_server);

PROCESS_THREAD(coap_server, ev, data)
{
    PROCESS_BEGIN();

    LOG_INFO("Starting CoAP Server\n");

    /* Bind resources to their Uri-Path */
    coap_activate_resource(&res_temperature, "sensor/temperature");
    coap_activate_resource(&res_humidity, "sensor/humidity");
    coap_activate_resource(&res_temperature_obs, "sensor/temperature/obs");

    /* Wait for requests */
    while(1) {
        PROCESS_WAIT_EVENT();
    }

    PROCESS_END();
}
