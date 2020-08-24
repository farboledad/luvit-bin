all: snmpconfig

DEBUG = -ggdb #-O0
override LDFLAGS += -lcg -ljson-c

%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG) -fPIC -Wall -Werror -I. -std=gnu99 -c $(<) -o $(@)

snmpconfig: snmpconfig.o
	$(CC) $(CFLAGS) -o $(@) $(^) $(LDFLAGS)

clean:
	rm -f snmpconfig *.o

.PHONY: all clean
	
