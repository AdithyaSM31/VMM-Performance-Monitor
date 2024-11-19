# VMM-Performance-Monitor
A real-time performance monitoring tool for Virtual Machine Monitors using C and GTK
Description
The Virtual Machine Monitor (VMM) Performance Monitor is a real-time tool designed to evaluate the performance of Virtual Machine Monitors such as Oracle VirtualBox and VMware. It tracks key system metrics, including CPU usage, memory usage, disk I/O, and network latency, to provide insights into the efficiency of VMMs. A performance score is calculated after a 30-second observation window, offering a comprehensive evaluation of each VMM.

Features
Real-Time Monitoring: Tracks and displays key metrics dynamically.
Color-Coded Graphs:
CPU Usage (Blue)
Memory Usage (Green)
Disk I/O Performance (Yellow)
Network Latency (Red)
Performance Score Calculation: Provides a stable score summarizing VMM performance.
User-Friendly Interface: Built using GTK for a lightweight and responsive GUI.
Project Objectives
Develop a Linux-based tool for monitoring and analyzing VMM performance.
Benchmark and compare the performance of different VMM platforms.
Provide actionable insights through detailed metrics and a scoring mechanism.
System Architecture
Components:
Data Collection Module: Captures metrics like CPU usage, memory usage, disk I/O, and network latency.
Processing Module:
Filters raw data.
Calculates a performance score.
Maintains historical data for visualization.
Visualization Module:
Displays real-time graphs.
Shows a stable performance score after analysis.
For a visual representation, refer to the Architecture Diagram.

How to Build and Run
Prerequisites
Linux Mint or any compatible Linux OS.
GTK 3.0 library installed.
Build Instructions
Clone the repository:
bash
Copy code
git clone https://github.com/YourUsername/VMM-Performance-Monitor.git
cd VMM-Performance-Monitor/src
Compile the code:
bash
Copy code
gcc -o vmm_monitor main.c `pkg-config --cflags --libs gtk+-3.0`
Run the application:
bash
Copy code
./vmm_monitor
Dependencies
GTK 3.0:
bash
Copy code
sudo apt install libgtk-3-dev

Usage
Launch the application.
Observe real-time updates in CPU, memory, disk I/O, and network graphs.
Review the performance score displayed after the 30-second analysis window.

Future Enhancements
Add support for monitoring GPU usage and energy consumption.
Extend compatibility to cloud-based virtualization platforms like AWS and Azure.
Integrate machine learning to predict performance bottlenecks.
Develop multi-VM monitoring capabilities.

License
This project is licensed under the MIT License - see the LICENSE file for details.

Contributors
Adithya Sankar Menon
Ananth S Mahadev
Akash V P
