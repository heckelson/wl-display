#!/usr/bin/env sh

pio run -e esp32dev --target upload && \
# pio run -e esp32dev --target uploadfs && \
pio run -e esp32dev --target monitor
