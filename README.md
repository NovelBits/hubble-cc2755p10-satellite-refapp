# Hubble Satellite Reference App — TI CC2755P10

Companion code for the Novel Bits tutorial **"Integrate Hubble's Satellite Network on Your TI CC2755P10."**

This is Hubble's stock `sat-dual-stack` sample with a small set of changes: it reads the chip's own supply voltage and die temperature, packs them into a 4-byte satellite payload, and logs the exact bytes so you can trace a reading from the bench, up to a satellite, and into the Hubble cloud.

## What changed vs. the stock sample

| File | Change |
|---|---|
| `src/telemetry.h` | **New.** Header-only, pure C: `battery_percent()` and `pack_telemetry()`. No TI or RTOS dependencies, so the packing logic is host-testable. |
| `src/main.c` | Reads the BatteryMonitor and Temperature drivers each pass, packs the 4 bytes, logs them, and hands the payload to the satellite API. |
| `test/test_pack.c` | **New.** Host unit test for the packing and percent logic. |
| `sat-dual-stack-cc27.syscfg` | Adds the `/ti/drivers/BatteryMonitor` and `/ti/drivers/Temperature` modules. |

Everything else is the stock sample verbatim.

## Prerequisites

- **Hubble's device SDK** — this app layers on top of it (it provides the satellite protocol, the TI radio port, the pass-prediction math, and the host tools). Clone it and point `HUBBLE_NETWORK_SDK` at it.
- **TI SimpleLink Low Power F3 SDK**, **TI Arm Clang**, **SysConfig**, and **UniFlash**.
- A device key, API token, and organization ID from the Hubble dashboard.

The full step-by-step (tool versions, environment variables, provisioning, and cloud readback) is in the tutorial.

## Build

Place this folder next to your Hubble device SDK clone, export the four environment variables (see the tutorial's appendix), then from this directory:

```sh
make -f cc2755p10.mk            # normal build
make -f cc2755p10.mk DEBUG=1    # bench build: schedules a fake pass every 120 s
```

The first build emits exactly one warning, the dummy-key warning. Bake your device key with the Hubble SDK's `embed_key_time.py` to generate `src/key.c`, then rebuild and the warning is gone. Flash `build/sat-dual-stack.hex` with UniFlash.

## Host test (no board needed)

```sh
clang -std=c11 -Wall -Wextra -Werror test/test_pack.c -o /tmp/test_pack && /tmp/test_pack
# expected: test_pack: all cases passed
```

## Tutorial

Full walkthrough: (link to the published Novel Bits tutorial)
