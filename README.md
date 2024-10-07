# Telemetry system for Hydrive1 hydrogen prototype car
System measures voltage and currents in multiple points, velocity and position (not stable yet) and sends it via MQTT protocol
to the server. Data is visualized on a server or on local PC after configuration.

## Table of contents
* [General info](#general-info)
* [Technologies](#technologies)
* [Setup](#setup)
* [Visualization](#visualization)

## General info
The primary function of the system is to measure voltage (0-50V) and current (0-50A) simultaneously at multiple points and transmit the data for subsequent visualization. It also measures
velocity and is supposed to measure position and hydrogen flow in the future.
It is based on an ESP32 microcontroller (tested on a firebeetle and ESP32-S2-Saola boards) and self-designed meassuring modules with Attiny424. 
	![Telemetry system block diagram](https://github.com/user-attachments/assets/aa599f8f-31a5-4239-8527-fdc53b2a484e)

## Technologies
* ESP32 microcontroler
* c++ and arduino libraries
* platformio
* vscode
  
## Setup
In order to use this project:
1. Download vscode IDE
2. Install platformIO extention
3. Open project with platformIO extention
4. Check if following libraries are installed (platformIO):
	ArduinoJson
	MQTT
	PubSubClient
6. If using other board, update enviroment and board in platform.ini file
7. compile and flash
   
## Visualization
Data can be visualized with Grafana, below is an example data from one of race laps

![Screenshot 2024-10-07 220504](https://github.com/user-attachments/assets/e9bef0dd-21a6-4d9f-afe3-026344e8db41)

For more information and instalation instructions contact repository owner
