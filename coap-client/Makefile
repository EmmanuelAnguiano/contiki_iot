CONTIKI_PROJECT = coap-client
all: $(CONTIKI_PROJECT)

CONTIKI = ../../../

# Include the CoAP implementation
include $(CONTIKI)/Makefile.dir-variables
MODULES += $(CONTIKI_NG_APP_LAYER_DIR)/coap

include $(CONTIKI)/Makefile.include
