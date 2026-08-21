/*
 * Copyright (c) 2026 Novel Bits, LLC
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Telemetry payload packing for the Hubble satellite battery reference app.
 * Header-only and free of TI / FreeRTOS dependencies so the packing and
 * battery-percent logic can be unit-tested on the host (see test/test_pack.c).
 */

#ifndef TELEMETRY_H
#define TELEMETRY_H

#include <stdint.h>

/*
 * Battery-percent approximation window, in millivolts.
 *
 * IMPORTANT: battery percent is a DERIVED, MODELED value, not a measurement.
 * This maps supply voltage linearly to 0..100% between an "empty" and a "full"
 * terminal voltage. A real fuel gauge would use the cell's discharge curve (or
 * a dedicated fuel-gauge IC); a linear map is a coarse approximation, adequate
 * for a demonstration but not for production state-of-charge.
 *
 * It is only meaningful when the chip is powered DIRECTLY from the cell whose
 * voltage BatteryMonitor reads. A regulated USB battery pack holds the rail
 * constant, so percent would read flat. Set these to your battery's usable
 * full/empty terminal voltage. Defaults below are placeholders for a fresh vs.
 * depleted coin cell.
 */
#define BATTERY_FULL_MV  3000U
#define BATTERY_EMPTY_MV 2000U

/*
 * Hubble satellite payloads must be 0, 4, 9, or 13 bytes. This reference app
 * uses the 4-byte option. Layout (big-endian for multi-byte fields):
 *   out[0]    = battery percent, 0..100          (uint8, DERIVED, see above)
 *   out[1..2] = battery supply voltage, millivolts (uint16, MEASURED)
 *   out[3]    = die temperature, whole degrees C   (int8,  MEASURED, clamped)
 *
 * The raw millivolts is kept alongside the percent so the backend/reader can
 * verify the derivation; temperature is the "custom sensor" field.
 */
#define TELEMETRY_PAYLOAD_LEN 4U

/* Linear voltage-to-percent approximation, clamped to [0, 100]. */
static inline uint8_t battery_percent(uint16_t battery_mv)
{
	if (battery_mv >= BATTERY_FULL_MV) {
		return 100U;
	}
	if (battery_mv <= BATTERY_EMPTY_MV) {
		return 0U;
	}

	uint32_t span = (uint32_t)(BATTERY_FULL_MV - BATTERY_EMPTY_MV);
	uint32_t above = (uint32_t)(battery_mv - BATTERY_EMPTY_MV);

	return (uint8_t)((above * 100U) / span);
}

static inline void pack_telemetry(uint8_t percent, uint16_t battery_mv,
				  int16_t temp_c, uint8_t out[TELEMETRY_PAYLOAD_LEN])
{
	int16_t t = temp_c;

	/* Temperature is carried in one signed byte; clamp to its range. */
	if (t > 127) {
		t = 127;
	} else if (t < -128) {
		t = -128;
	}

	out[0] = percent;
	out[1] = (uint8_t)(battery_mv >> 8);
	out[2] = (uint8_t)(battery_mv & 0xFFU);
	out[3] = (uint8_t)(int8_t)t; /* two's-complement byte */
}

#endif /* TELEMETRY_H */
