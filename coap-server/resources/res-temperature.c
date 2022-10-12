#include "contiki.h"
#include "coap-engine.h"
#include "node-id.h"
#include "virtual-sensor.h"

#include <stdio.h>
#include <string.h>

static void
res_get_handler(coap_message_t *request,
                coap_message_t *response,
                uint8_t *buffer,
                uint16_t preferred_size,
                int32_t *offset);

RESOURCE(res_temperature,
         "title=\"Temperature sensor\";rt=\"Temperature\"",
         res_get_handler,
         NULL,
         NULL,
         NULL);

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
}
