#include "virtual-sensor.h"
#include "lib/random.h"

int
random_value(int min, int max)
{
  int result = min + random_rand() % (max - min);

  return result;
}

int
read_temperature()
{
  return random_value(0, 35);
}

int
read_humidity()
{
  return random_value(40, 80);
}
