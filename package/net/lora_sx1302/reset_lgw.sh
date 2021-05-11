#!/bin/sh

#
# Usage examples:
#       ./reset_lgw.sh stop
#       ./reset_lgw.sh start

WAIT_GPIO() {
    sleep 0.1
}

init() {
    # setup GPIOs
    echo "high" > /tmp/GPIO_SSPBS_PWR/direction; WAIT_GPIO
}

reset() {
    echo "high" > /tmp/GPIO_SSPBS_PWR/direction; WAIT_GPIO
    echo "low" > /tmp/GPIO_SSPBS_PWR/direction; WAIT_GPIO
}

term() {
    echo "high" > /tmp/GPIO_SSPBS_PWR/direction; WAIT_GPIO
}

case "$1" in
    start)
    init
    reset
    ;;
    stop)
    reset
    term
    ;;
    *)
    echo "Usage: $0 {start|stop}"
    exit 1
    ;;
esac

exit 0
