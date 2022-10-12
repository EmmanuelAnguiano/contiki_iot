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

RESOURCE(res_humidity,
         "title=\"Humidity sensor\";rt=\"Humidity\"",
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
    /* Humidity sensor reading */
    uint8_t humidity = read_humidity();
    
    /* Prepare CoAP message */
    coap_set_header_content_format(response, APPLICATION_JSON);
    snprintf((char *)buffer, COAP_MAX_CHUNK_SIZE, "{\"node\": %d, \"humidity\": %d}", node_id, humidity);
    coap_set_payload(response, buffer, strlen((char *)buffer));
}
