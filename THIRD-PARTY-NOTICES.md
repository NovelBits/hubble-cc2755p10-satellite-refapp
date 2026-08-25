# Third-party notices

This repository is the reference application for the Novel Bits tutorial *Hubble satellite network on
the TI CC2755P10*. It is a modification of Hubble Network's stock `sat-dual-stack` sample and carries
files from three copyright holders. All of them are permissive and mutually compatible; each source
file keeps its own header, which is where the authoritative notice lives.

## Summary

| Copyright holder | Files | License |
|---|---|---|
| **Novel Bits, LLC** | `src/telemetry.h`, `src/telemetry_source.h`, `src/telemetry_source.c`, `test/test_pack.c` | Apache-2.0 |
| **Hubble Network, Inc.** | `src/main.c`, `src/app_ble.c`, `src/app_ble.h`, `src/app_main.h`, `src/hubble_config.h`, `src/hubble_ti_crypto.c`, `cc2755p10.mk`, `cc2340r5.mk` | Apache-2.0 |
| **Texas Instruments Incorporated** | `src/common/Startup/osal_icall_ble.c`, and the linker command files / SysConfig files derived from the SimpleLink Low Power F3 SDK | BSD-3-Clause |

The repository's own `LICENSE` is Apache-2.0 and applies to the Novel Bits contributions. It does not
override the license of any file whose header names a different one.

## What Novel Bits actually changed

Everything else is the stock sample. Our modifications are:

- `src/telemetry.h` (new) - the 4-byte payload layout and the battery-percent approximation, kept free
  of TI and FreeRTOS dependencies so it is host-testable
- `src/telemetry_source.h` / `.c` (new) - reads the on-chip supply voltage and die temperature and packs
  the payload; both the satellite and terrestrial paths call it
- `test/test_pack.c` (new) - host unit test for the packing and percent logic
- `src/main.c`, `src/app_ble.c` - call `telemetry_sample()`; the terrestrial beacon carries the same
  telemetry as the satellite transmission
- `sat-dual-stack-cc27.syscfg` - adds the `/ti/drivers/BatteryMonitor` and `/ti/drivers/Temperature` modules
- `cc2755p10.mk` - builds `telemetry_source.c`

## Texas Instruments Incorporated - BSD-3-Clause

Reproduced from `src/common/Startup/osal_icall_ble.c`:

    Copyright (c) 2013-2025, Texas Instruments Incorporated
     All rights reserved.

     Redistribution and use in source and binary forms, with or without
     modification, are permitted provided that the following conditions
     are met:

     Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.

     Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.

     Neither the name of Texas Instruments Incorporated nor the names of
        its contributors may be used to endorse or promote products derived
        from this software without specific prior written permission.

     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
     AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
     THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
     PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
     CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
     EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
     PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
     OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
     WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
     OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
     EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
