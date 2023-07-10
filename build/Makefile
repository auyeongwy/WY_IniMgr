CC = gcc
CFLAGS = -std=c17 -Wall -O2 -march=native
#CFLAGS = -Wall -O2 -std=c17 -fsanitize=address -static-libasan -g3 -march=native
SRC = ../src
BUILD = ../build
OBJS = $(BUILD)/WY_IniMgr.o $(BUILD)/WY_IniIO.o
HEADERS = $(SRC)/WY_IniMgr.h $(SRC)/WY_IniIO.h
TARGETLIB = $(BUILD)/WY_IniMgr.a


.PHONY: clean distclean object_msg

all: $(BUILD)/demo $(TARGETLIB)

$(BUILD)/demo: $(SRC)/main.c $(HEADERS) $(TARGETLIB)
	@echo Building demo...
	$(CC) $(CFLAGS) $(SRC)/main.c $(TARGETLIB) -o $(BUILD)/demo

$(TARGETLIB): object_msg $(OBJS)
	@echo Building library...
	ar rcs $(TARGETLIB) $(OBJS)

$(BUILD)/WY_IniMgr.o: $(HEADERS) $(SRC)/WY_IniDefs.h $(SRC)/WY_IniMgr.c
	$(CC) $(CFLAGS) -c $(SRC)/WY_IniMgr.c -o $(BUILD)/WY_IniMgr.o

$(BUILD)/WY_IniIO.o: $(HEADERS) $(SRC)/WY_IniIO.c
	$(CC) $(CFLAGS) -c $(SRC)/WY_IniIO.c -o $(BUILD)/WY_IniIO.o

object_msg:
	@echo Building objects...

clean:
	rm -f $(BUILD)/*.o

distclean: clean
	rm -f $(BUILD)/demo
	rm -f $(TARGETLIB)
