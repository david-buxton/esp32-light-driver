#!/usr/bin/env bash

#source ~/esp/esp-idf/export.sh
#bash build_and_test.sh /dev/ttyUSB0




if [[ $# -ne 1 || -z $1 ]]; then
    printf 'Usage: bash %s /dev/ttyUSB0\n' "${0##*/}" >&2
    exit 2
fi

if [[ ! -f CMakeLists.txt ]]; then
    printf 'Run this script from the ESP-IDF project root.\n' >&2
    exit 2
fi

if ! command -v idf.py >/dev/null 2>&1; then
    printf 'Load your ESP-IDF environment first, for example: source ~/esp/esp-idf/export.sh\n' >&2
    exit 2
fi

# ESP-IDF builds before flashing and starts the monitor after flashing.
exec idf.py -p "$1" build flash monitor
