#!/usr/bin/env sh

pio run --target upload && \
pio run --target uploadfs && \
pio run --target monitor
