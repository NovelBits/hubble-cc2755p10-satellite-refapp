/*
 * Copyright (c) 2026 Novel Bits, LLC
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Host unit test for battery_percent() and pack_telemetry(). Compile and run
 * on the development machine (no target hardware needed):
 *
 *   cc -std=c11 -Wall -Wextra -Werror test/test_pack.c -o /tmp/test_pack && /tmp/test_pack
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "../src/telemetry.h"

static void test_battery_percent(void)
{
	/* Defaults: EMPTY = 2000 mV, FULL = 3000 mV. */
	assert(battery_percent(3000) == 100); /* at/above full */
	assert(battery_percent(3200) == 100); /* clamped high */
	assert(battery_percent(2000) == 0);   /* at/below empty */
	assert(battery_percent(1500) == 0);   /* clamped low */
	assert(battery_percent(2500) == 50);  /* midpoint */
	assert(battery_percent(2850) == 85);  /* 850/1000 */
	assert(battery_percent(2001) == 0);   /* 1/1000 truncates to 0 */
}

static void test_pack(void)
{
	uint8_t out[TELEMETRY_PAYLOAD_LEN];

	/* percent + battery mV (big-endian) + signed temp. */
	pack_telemetry(85, 2850, 25, out); /* 2850 = 0x0B22, 25 = 0x19 */
	assert(out[0] == 85);
	assert(out[1] == 0x0B && out[2] == 0x22);
	assert(out[3] == 0x19);

	/* Negative temperature packs as a signed byte: -10 = 0xF6. */
	pack_telemetry(50, 2500, -10, out); /* 2500 = 0x09C4 */
	assert(out[0] == 50);
	assert(out[1] == 0x09 && out[2] == 0xC4);
	assert(out[3] == 0xF6);

	/* Temperature clamps into the int8 range. */
	pack_telemetry(100, 3000, 200, out);
	assert(out[3] == 0x7F); /* +127 */
	pack_telemetry(0, 2000, -200, out);
	assert(out[3] == 0x80); /* -128 */

	/* Max voltage. */
	pack_telemetry(100, 0xFFFF, 0, out);
	assert(out[1] == 0xFF && out[2] == 0xFF);
}

int main(void)
{
	test_battery_percent();
	test_pack();
	printf("test_pack: all cases passed\n");
	return 0;
}
