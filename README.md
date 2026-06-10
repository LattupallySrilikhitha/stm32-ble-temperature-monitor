# STM32 FreeRTOS Temperature Monitoring Framework

## Overview

This project implements a modular embedded firmware framework on the STM32L4R9 platform using FreeRTOS.

The objective is to develop a scalable real-time sensor acquisition system using ADC, DMA, interrupt-driven processing, and RTOS-based task synchronization. 
The architecture is designed to support future communication interfaces such as BLE and Wi-Fi.

---

## Features Implemented

### RTOS Architecture

* FreeRTOS-based multitasking framework
* Dedicated Sensor Task
* Communication Task framework
* Queue-based inter-task communication
* Semaphore-based synchronization

### Sensor Acquisition

* ADC-based temperature acquisition
* DMA circular buffer operation
* Half-transfer DMA callback handling
* Full-transfer DMA callback handling
* Sensor data processing pipeline

### Data Management

* Global sensor database
* Temperature history storage
* Circular buffer implementation
* Sample management and tracking

### Software Design

* Modular driver abstraction
* Separation of acquisition, processing, and communication layers
* Scalable architecture for additional sensors and communication interfaces

---

## Current Status

### Completed

* FreeRTOS framework integration
* Task scheduling and synchronization
* Queue communication
* ADC-DMA acquisition framework
* Temperature processing framework
* Sensor history management
* End-to-end acquisition workflow

### In Progress

* Internal STM32 temperature sensor validation and calibration
* Communication layer enhancement
* Wi-Fi integration

---

## Debugging Activities

The project includes practical debugging and validation activities involving:

* RTOS task verification
* Queue and semaphore validation
* DMA callback verification
* ADC acquisition debugging
* Internal temperature sensor characterization
* End-to-end data flow validation

---

## Future Enhancements

* Wi-Fi telemetry support
* MQTT integration
* Cloud connectivity
* External sensor integration
* Real-time dashboard visualization

---

## Development Environment

* STM32L4R9 Discovery Board
* STM32CubeIDE
* STM32CubeMX
* FreeRTOS
* Embedded C
* Git

