# Noise Exposure Health Monitor

A wearable / clip-on device that logs sound levels throughout the day, tracks
cumulative noise exposure, and compares it against WHO/OSHA safe listening
thresholds — aimed at a demographic (teens) that no consumer wearable
currently targets.

This is an ongoing electrical/computer engineering project built to learn
embedded systems fundamentals: analog sensing, signal processing, PCB
design, and data visualization.

## Project Roadmap

- [x] **Phase 1 — Breadboard proof of concept** *(current)*
      MEMS mic → Arduino Uno → relative dB readings over Serial
- [ ] **Phase 2 — Analog frontend**
      Op-amp preamp + A-weighting filter, designed and simulated in LTspice
- [ ] **Phase 3 — Data logging + dashboard**
      Timestamped SD card logging, Python dashboard, NIOSH exchange rate
      exposure calculation
- [ ] **Phase 4 — Custom PCB**
      Full board design in KiCad, fabricated via JLCPCB, 3D-printed enclosure

## Phase 1: Breadboard Proof of Concept

**Goal:** confirm the full sensing pipeline works — microphone picks up
sound, Arduino reads it, and meaningful relative loudness values are
printed out — before adding any analog signal conditioning or calibration.

### Hardware
- ELEGOO UNO R3 (Arduino-compatible board)
- MAX9814 electret microphone breakout (MEMS mic with onboard amplifier)
- Breadboard + jumper wires

### Wiring
| Mic pin | Arduino pin |
|---|---|
| VCC / VDD | 5V |
| GND | GND |
| OUT | A0 |

*(A/R gain pin left unconnected — using default automatic gain control)*

See `photos/` for the physical breadboard layout.

### How it works
The Arduino samples the mic's analog output (`A0`) continuously over a
50ms window and finds the peak-to-peak voltage swing — a simple proxy for
how loud the sound is in that window. To reduce noise from short, isolated
spikes (e.g. a single bird chirp), five consecutive 50ms windows are
averaged together before being converted to a rough decibel-style value
and printed over Serial.

**Note:** the dB value at this stage is *relative*, not true dB SPL. It is
useful for confirming the sensor responds correctly to loud vs. quiet
sound, but has not yet been calibrated against a reference sound level
meter. Calibration is planned for a later phase.

### Code
See [`noise_monitor_phase1.ino`](./noise_monitor_phase1.ino).

### Result
Confirmed the sensor reliably distinguishes silence from conversation
from loud/sudden sounds (claps), with a noticeably steadier baseline
after adding multi-window averaging.

## Why this project

Noise-induced hearing loss is permanent, cumulative, and increasingly
common among teens — yet no consumer wearable tracks exposure for this
age group the way fitness trackers track steps or heart rate. This
project is both a hands-on way to learn embedded systems (the core of
my interest in ECE) and an attempt to build something that fills a real
gap.
