#include "contiki.h"
#include "coap-engine.h"
#include "virtual-sensor.h"
#include "node-id.h"
#include <stdio.h>

static void
res_get_handler(coap_message_t *request, coap_message_t *response,
                uint8_t *buffer, uint16_t preferred_size,
                int32_t *offset);

static void
res_periodic_handler(void);

#define MAX_AGE 60

PERIODIC_RESOURCE(res_temperature_obs,
                  "title=\"Periodic temperature sensor\";rt=\"Temperature\";obs",
                  res_get_handler,
                  NULL,
                  NULL,
                  NULL,
                  5000,
                  res_periodic_handler);

static void
res_get_handler(coap_message_t *request,
                coap_message_t *response,
                uint8_t *buffer,
                uint16_t preferred_size,
                int32_t *offset)
{
    /* Temperature sensor reading */
    uint8_t temperature = read_temperature();
    
    /* Prepare CoAP message */
    coap_set_header_content_format(response, APPLICATION_JSON);
    snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE, 
             "{\"node\": %d, \"temperature\": %d}", node_id, temperature);
    coap_set_payload(response, buffer, strlen((char *)buffer));
    coap_set_header_max_age(response, MAX_AGE);
}

static void
res_periodic_handler()
{
    coap_notify_observers(&res_temperature_obs);
}
