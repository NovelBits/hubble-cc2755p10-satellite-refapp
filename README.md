# Hubble Satellite Reference App — TI CC2755P10

Companion code for the Novel Bits tutorial **"Integrate Hubble's Satellite Network on Your TI CC2755P10."**

This is Hubble's stock `sat-dual-stack` sample with a small set of changes: it reads the chip's own supply voltage and die temperature, packs them into a 4-byte satellite payload, and logs the exact bytes so you can trace a reading from the bench, up to a satellite, and into the Hubble cloud.

## What changed vs. the stock sample

| File | Change |
|---|---|
| `src/telemetry.h` | **New.** Header-only, pure C: `battery_percent()` and `pack_telemetry()`. No TI or RTOS dependencies, so the packing logic is host-testable. |
| `src/telemetry_source.h` / `.c` | **New.** Reads the on-chip supply voltage and die temperature and packs the payload. Separate from `telemetry.h` on purpose: the driver calls live here so `telemetry.h` stays host-testable. Both the satellite and terrestrial paths call `telemetry_sample()`. |
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

The first build emits exactly one warning, the dummy-key warning. Bake your device key with the Hubble SDK's `embed_key_time.py` to generate `src/key.c`, then rebuild and the warning is gone. Flash `build/sat-dual-stack.hex` with the **UniFlash GUI**: select the CC2755P10 with an XDS110 probe and let UniFlash build the session for you.

> **We flash with the GUI.** We did not get the `dslite.sh` command-line path working on this board: it stopped at *"Can't generate board data file ... An invalid processor ID has been found"*, with the probe enumerating correctly, the board-data cache cleared, `TGT VDD` on `XDS`, and both SWD-mode settings tried (UniFlash 9.6.0.5764, 2026-08-25). We did not chase it further, because the GUI flashes the same hex without complaint. If you have the CLI working against a CC2755P10, we would like to hear how.

## A note on `CONFIG_HUBBLE_SAT_NETWORK_DEVICE_TDR`

`src/hubble_config.h` carries `#define CONFIG_HUBBLE_SAT_NETWORK_DEVICE_TDR 500`, and that value is
**not the one your build uses.** It is overridden at build time: check
`build/ti_utils_build_compiler.opt` after a build and you will find `-DCONFIG_HUBBLE_SAT_NETWORK_DEVICE_TDR=10`.

The distinction matters if you are checking the tutorial's arithmetic. TDR is the assumed clock-drift
rate in ppm, and it sets how long a device can go without a time sync before the SDK adds an extra
transmission. At **10 ppm** that is about three weeks, which is the figure the tutorial quotes. At 500
it would be about eleven hours. Read the generated `.opt` file rather than the header.

The header is left exactly as Hubble ships it, so this note lives here instead of in the source.

## Host test (no board needed)

```sh
clang -std=c11 -Wall -Wextra -Werror test/test_pack.c -o /tmp/test_pack && /tmp/test_pack
# expected: test_pack: all cases passed
```

**This file is a superset of the one in the tutorial, so do not be surprised if it does not diff clean.**
The tutorial publishes a deliberately short version: the four assertions that carry the most weight,
flat in `main()`. The copy here splits them into `test_battery_percent()` and `test_pack()` and adds the
boundary cases the walkthrough does not stop on: exactly at full and exactly at empty, 2850 mV landing on
85 percent, both temperature clamps (`0x7F` and `0x80`), and a maximum-voltage case. Both versions
compile under `-Werror` and both print the same line.

Every other file here is byte-for-byte what the tutorial publishes, modulo nothing: the listings in the
post use four-space indentation and so do these.

## Tutorial

Full walkthrough: (link to the published Novel Bits tutorial)

## Licensing

Apache-2.0, see `LICENSE`.

This app is a modification of Hubble Network's stock `sat-dual-stack` sample and also carries files
from Texas Instruments under BSD-3-Clause. Every source file keeps its own header, and
`THIRD-PARTY-NOTICES.md` lists which files belong to whom and reproduces TI's notice.
