CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -O3 -flto
ARCH = -march=native
#CFLAGS = -Wall -std=c17 -fsanitize=address -static-libasan -g3 -march=native
SRC = ../src
BUILD = ../build
OBJS = $(BUILD)/WY_IniMgr.o $(BUILD)/WY_IniIO.o $(BUILD)/WY_IniParseAgent.o $(BUILD)/WY_IniWriteAgent.o 
API_HEADERS = $(SRC)/WY_IniMgr.h 
HEADERS = $(SRC)/WY_IniMgr.h $(SRC)/WY_IniIO.h $(SRC)/WY_IniDefs.h $(SRC)/WY_IniParseAgent.h $(SRC)/WY_IniWriteAgent.h
TARGETLIB = $(BUILD)/lib_WY_IniMgr.a


.PHONY: clean distclean object_msg

all: $(BUILD)/demo $(TARGETLIB)

$(BUILD)/demo: $(SRC)/demo.c $(API_HEADERS) $(TARGETLIB)
	@echo Building demo...
	$(CC) $(CFLAGS) $(ARCH)  $(SRC)/demo.c $(TARGETLIB) -o $(BUILD)/demo

$(TARGETLIB): object_msg $(OBJS)
	@echo Building library...
	ar rcs $(TARGETLIB) $(OBJS)

$(BUILD)/WY_IniMgr.o: $(HEADERS) $(SRC)/WY_IniMgr.c
	$(CC) $(CFLAGS) $(ARCH)  -c $(SRC)/WY_IniMgr.c -o $(BUILD)/WY_IniMgr.o

$(BUILD)/WY_IniIO.o: $(HEADERS) $(SRC)/WY_IniIO.c
	$(CC) $(CFLAGS) $(ARCH)  -c $(SRC)/WY_IniIO.c -o $(BUILD)/WY_IniIO.o

$(BUILD)/WY_IniParseAgent.o: $(HEADERS) $(SRC)/WY_IniParseAgent.c
	$(CC) $(CFLAGS) $(ARCH)  -c $(SRC)/WY_IniParseAgent.c -o $(BUILD)/WY_IniParseAgent.o

$(BUILD)/WY_IniWriteAgent.o: $(HEADERS) $(SRC)/WY_IniWriteAgent.c
	$(CC) $(CFLAGS) $(ARCH)  -c $(SRC)/WY_IniWriteAgent.c -o $(BUILD)/WY_IniWriteAgent.o

object_msg:
	@echo Building objects...

clean:
	rm -f $(BUILD)/*.o

distclean: clean
	rm -f $(BUILD)/demo
	rm -f $(TARGETLIB)
