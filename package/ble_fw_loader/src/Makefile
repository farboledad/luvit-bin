all: ble_fw_loader

OBJS = $(patsubst %.c,%.o,$(wildcard *.c))
BINS := $(patsubst %.o,%,$(OBJS))

ble_fw_loader: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG) -fPIC -Wall -Werror -I. -std=gnu99 -c $(<) -o $(@)

$(BINS): %: %.o
	$(CC) $(CFLAGS) -o $(@) $(^) $(LDFLAGS)

clean:
	rm -f $(OBJS) $(BINS)

.PHONY: all clean
