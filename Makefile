XBE_TITLE = rsbsPLUS-xboxen
GEN_XISO = $(XBE_TITLE).iso
SRCS = $(CURDIR)/main.c
NXDK_DIR = /opt/nxdk
NXDK_SDL = y
include $(NXDK_DIR)/Makefile

TARGET += $(OUTPUT_DIR)/res
$(GEN_XISO): $(OUTPUT_DIR)/res
$(OUTPUT_DIR)/res: $(CURDIR)/res $(OUTPUT_DIR)
	rm -rf $(OUTPUT_DIR)/res
	$(VE)cp -rf '$<' '$@'
