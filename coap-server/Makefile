CONTIKI_PROJECT = coap-server
all: $(CONTIKI_PROJECT)

PROJECT_SOURCEFILES += virtual-sensor.c

CONTIKI = ../../../

# Include CoAP resoures
MODULES_REL += ./resources

# Include the CoAP implementation
include $(CONTIKI)/Makefile.dir-variables
MODULES += $(CONTIKI_NG_APP_LAYER_DIR)/coap

include $(CONTIKI)/Makefile.include
