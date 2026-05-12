# ENPM818J-Temperature-Humidty-Sensor
Temperature and Humidity Monitoring using ESP32 (FreeRTOS + DHT11)
1. FreeRTOS Implementation on ESP32
The project will initialize and run FreeRTOS on the ESP-WROOM-32, creating separate
tasks for sensor reading, data processing, and communication to ensure real-time
performance and efficient multitasking.
2. Sensor Integration (DHT11)
A DHT11 temperature and humidity sensor will be interfaced with the ESP32 using C (ESPIDF environment on Linux), allowing periodic acquisition of environmental data through a
dedicated FreeRTOS task.
3. Data Processing and Task Management
The collected sensor data will be processed and formatted within the ESP32, using inter-task
communication mechanisms such as queues or semaphores to ensure synchronization and
reliability.
4. Server-Side Data Transmission and Visualization
The ESP32 will transmit temperature and humidity data over Wi-Fi to a server, where the
data will be stored and visualized (e.g., via a web dashboard or simple HTTP server
interface).
5. System Testing and Demonstration
The complete system will be tested for accuracy, reliability, and real-time performance,
demonstrating continuous monitoring and visualization of environmental data.
