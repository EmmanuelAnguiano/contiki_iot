#include "contiki.h"
#include "net/netstack.h"
#include "net/routing/routing.h"
#include "net/ipv6/simple-udp.h"
#include "node-id.h"
#include "virtual-sensor.h"

#include "sys/log.h"
#define LOG_MODULE "RPL root node"
#define LOG_LEVEL LOG_LEVEL_INFO

#define SEND_INTERVAL (10 * CLOCK_SECOND)

static struct simple_udp_connection connection;

PROCESS(sensor_node , "RPL sensor node");
AUTOSTART_PROCESSES(&sensor_node);

PROCESS_THREAD(sensor_node, ev, data){
    
    static struct etimer periodic_timer;
    uint8_t temperature;
    uint8_t humidity;
    static char buffer[64];
    uip_ipaddr_t server_address;

    PROCESS_BEGIN();

    simple_udp_register(&connection, CLIENT_PORT, NULL, SERVER_PORT, NULL);
    etimer_set(&periodic_timer, SEND_INTERVAL);

    while(1){
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

        uint8_t is_node_reachable = NETSTACK_ROUTING.node_is_reachable();
        uint8_t has_server_address = NETSTACK_ROUTING.get_root_ipaddr(&server_address);

        if(is_node_reachable && has_server_address){
            LOG_INFO("SENDING REQUEST TO");
            LOG_INFO_6ADDR(&server_address);
            LOG_INFO_("\n");

            temperature = read_temperature();
            humidity = read_humidity();

            snprintf(buffer, sizeof(buffer), "{\"node\": %d, \"temperature\": %d, \"humidity\": %d}", node_id, temperature, humidity);
            simple_udp_sendto(&connection, buffer, strlen(buffer), &server_address);
            }else{
                LOG_INFO("ROOT NODE NOT REACHABLE YET.\n");
            }

            etimer_reset(&periodic_timer);
    }
    PROCESS_END();
}
