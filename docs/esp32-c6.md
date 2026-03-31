# ESP32-C6 Notes

## Purpose

This document collects ESP32-C6-specific notes relevant to PlantOS.

## Topics to document here

- memory map used by the project
- image/load assumptions
- GPIO / IO MUX / matrix model
- UART console notes
- watchdog behavior
- timer backend choice
- interrupt backend choice
- boot caveats discovered during bring-up

## Current practical notes

- real hardware bring-up is the active focus
- linker script is SoC-specific
- startup currently depends on ESP32-C6-specific early setup
- serial console has been useful for proving execution
