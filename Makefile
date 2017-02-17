CPPFLAGS+=-Wall -Wextra -Wpedantic
CPPFLAGS+=-Wwrite-strings -Wstack-usage=1024 -Wfloat-equal
CPPFLAGS+=-Waggregate-return -Winline

CFLAGS+=-std=c11

LDLIBS+=-lm

BIN=zergmap
OBJS=zergmap.o binary.o decode.o encode.o packet.o list.o graph.o tree.o

.PHONY: clean debug profile

all: $(BIN)

zergmap: zergmap.o binary.o list.o graph.o tree.o
decode: decode.o binary.o packet.o
encode: encode.o binary.o packet.o

debug: CFLAGS+=-g
debug: $(BIN)

profile: CFLAGS+=-pg
profile: LDFLAGS+=-pg
profile: $(BIN)

clean:
	$(RM) $(OBJS) $(BIN)
