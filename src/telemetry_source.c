/*
 * Copyright (c) 2026 Novel Bits, LLC
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ti/drivers/BatteryMonitor.h>
#include <ti/drivers/Temperature.h>

#include "telemetry.h"
#include "telemetry_source.h"

void telemetry_sample(struct telemetry_reading *r)
{
    /*
     * BatteryMonitor and Temperature share the on-chip BATMON peripheral.
     * Both inits run in main() before any thread starts, so no init is
     * attempted here -- doing it per-sample would add work to a path that
     * runs inside the beacon's hourly rebuild.
     */
    r->battery_mv = BatteryMonitor_getVoltage();
    r->temp_c = Temperature_getTemperature();
    r->battery_pct = battery_percent(r->battery_mv);

    pack_telemetry(r->battery_pct, r->battery_mv, r->temp_c, r->payload);
}
