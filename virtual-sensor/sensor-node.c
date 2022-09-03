#include "contiki.h"
#include "sys/etimer.h"
#include "virtual-sensor.h"

#include <stdio.h>

PROCESS(sensor_node, "Sensor Reading Node");
AUTOSTART_PROCESSES(&sensor_node);

static struct etimer timer;

PROCESS_THREAD(sensor_node, ev, data)
{
  PROCESS_BEGIN();

  etimer_set(&timer, 5 * CLOCK_SECOND);
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    int temperature = read_temperature();
    int humidity = read_humidity();

    printf("Temperature reading: %d C\n", temperature);
    printf("Humidity reading: %d %%\n", humidity);

    etimer_reset(&timer);
  }

  PROCESS_END();
}
