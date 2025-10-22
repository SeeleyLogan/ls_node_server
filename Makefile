CC 	  = clang
CDBGC = gcc
DBG   = gdb

CFLAGS = -Wall -std=c89 -pedantic -D_GNU_SOURCE -I./
SRCDIR = ./src
BINDIR = ./bin

TARGET_SERVER = $(BINDIR)/server
TARDBG_SERVER = $(BINDIR)/server_dbg

TARGET_CLIENT = $(BINDIR)/client
TARDBG_CLIENT = $(BINDIR)/client_dbg

SOURCE = $(SRCDIR)/main.c

.PHONY: all clean run-server run-client debug-server debug-client

all: $(TARGET_SERVER) $(TARGET_CLIENT) $(TARDBG_SERVER) $(TARDBG_CLIENT)

$(TARGET_SERVER): $(SOURCE) $(BINDIR)
	$(CC) $(CFLAGS) -DSERVER $< -o $@;printf "\nSize of TARGET_SERVER:\n";size $(TARGET_SERVER);echo

$(TARGET_CLIENT): $(SOURCE) $(BINDIR)
	$(CC) $(CFLAGS) -DCLIENT $< -o $@;printf "\nSize of TARGET_CLIENT:\n";size $(TARGET_CLIENT);echo

$(TARDBG_SERVER): $(SOURCE) $(BINDIR)
	$(CDBGC) $(CFLAGS) -DSERVER $< -o $@ -g

$(TARDBG_CLIENT): $(SOURCE) $(BINDIR)
	$(CDBGC) $(CFLAGS) -DCLIENT $< -o $@ -g

$(BINDIR):
	mkdir -p $(BINDIR)

run-server: $(TARGET_SERVER)
	@$(TARGET_SERVER); EXIT_CODE=$$?; printf "\n\nEXIT CODE: 0x%x\n" $$EXIT_CODE

run-client: $(TARGET_CLIENT)
	@$(TARGET_CLIENT); EXIT_CODE=$$?; printf "\n\nEXIT CODE: 0x%x\n" $$EXIT_CODE

debug-server: $(TARDBG_SERVER)
	@$(DBG) $(TARDBG_SERVER); EXIT_CODE=$$?; printf "\n\nEXIT CODE: 0x%x\n" $$EXIT_CODE

debug-client: $(TARDBG_CLIENT)
	@$(DBG) $(TARDBG_CLIENT); EXIT_CODE=$$?; printf "\n\nEXIT CODE: 0x%x\n" $$EXIT_CODE

clean:
	rm -rf $(BINDIR)
	$(MAKE) all
