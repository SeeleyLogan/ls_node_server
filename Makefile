CC 	  = clang
CDBGC = gcc
DBG   = gdb

CFLAGS = -Wall -std=c89 -pedantic -D_GNU_SOURCE -I./
SRCDIR = ./src
BINDIR = ./bin

TARGET = $(BINDIR)/main
TARDBG = $(BINDIR)/main_dbg
SOURCE = $(SRCDIR)/main.c

.PHONY: all run clean debug run-debug

all: $(TARGET)

$(TARGET): $(SOURCE) $(BINDIR)
	$(CC) $(CFLAGS) $< -o $@;printf "\nSize of target:\n";size $(TARGET);echo

$(TARDBG): $(SOURCE) $(BINDIR)
	$(CDBGC) $(CFLAGS) $< -o $@ -g

debug: $(TARDBG)

$(BINDIR):
	mkdir -p $(BINDIR)

run: $(TARGET)
	@$(TARGET); EXIT_CODE=$$?; printf "\n\nEXIT CODE: 0x%x\n" $$EXIT_CODE

run-debug: $(TARDBG)
	@$(DBG) $(TARDBG); EXIT_CODE=$$?; printf "\n\nEXIT CODE: 0x%x\n" $$EXIT_CODE

clean:
	rm -rf $(BINDIR)
	$(MAKE) all
