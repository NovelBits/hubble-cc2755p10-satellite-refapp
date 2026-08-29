/*
 * Copyright (c) 2026 Novel Bits, LLC
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Sampling half of the telemetry path: reads the on-chip sensors and packs the
 * Hubble payload.
 *
 * WHY THIS IS A SEPARATE FILE FROM telemetry.h. telemetry.h is deliberately
 * header-only and free of TI and FreeRTOS dependencies so that the packing and
 * battery-percent logic can be unit-tested on the host (test/test_pack.c).
 * Driver calls must NOT migrate into it. Everything that needs a TI driver
 * lives here instead.
 */

#ifndef TELEMETRY_SOURCE_H
#define TELEMETRY_SOURCE_H

#include <stdint.h>

#include "telemetry.h"

/*
 * One sensor sample, with the raw readings kept alongside the packed bytes.
 *
 * The raw fields exist so a caller can log what it measured next to what it
 * transmitted; `temp_c` is the UNCLAMPED reading, while payload[3] carries the
 * value clamped to a signed byte (see pack_telemetry).
 */
struct telemetry_reading {
    uint16_t battery_mv;                    /* MEASURED, millivolts */
    int16_t temp_c;                         /* MEASURED, whole degrees C */
    uint8_t battery_pct;                    /* DERIVED, see telemetry.h */
    uint8_t payload[TELEMETRY_PAYLOAD_LEN]; /* what actually goes on the air */
};

/*
 * Read supply voltage and die temperature, and pack them into r->payload.
 *
 * BOTH NETWORKS SEND THE SAME PAYLOAD. The Hubble dashboard tags every record
 * with the network it arrived on, so the payload does not have to serve as the
 * terrestrial-vs-satellite discriminator and both paths call this.
 *
 * Requires BatteryMonitor_init() and Temperature_init(). main() calls both
 * immediately after Board_init(), before any thread starts, so this is safe
 * from every task and in every MEASURE= build.
 */
void telemetry_sample(struct telemetry_reading *r);

#endif /* TELEMETRY_SOURCE_H */
