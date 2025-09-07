# Tomasulo Simulator  

*A cycle-accurate Out-of-Order CPU simulator with Tomasulo’s Algorithm, ROB, LSQ, and cache hierarchy.*  

---

## Project Showcase  

<p align="center">
  <img src="assets/simulator_startup.png" alt="Simulator Startup Output" width="80%">
</p>

<p align="center">
  <img src="assets/gantt_chart.png" alt="Gantt Chart Visualization" width="48%">
  <img src="assets/trace_output.png" alt="Cycle-by-Cycle Trace" width="48%">
</p>

---

## Overview  
Concise 2–3 sentences explaining what the project is, why it exists, and its scope.  

---

## Features  
- Out-of-Order CPU simulation with Tomasulo’s Algorithm  
- Load/Store Queue (LSQ), Reorder Buffer (ROB), L1 Data Cache, Victim Cache  
- Configurable architectural parameters  
- Cycle-by-cycle tracing and statistics  
- Gantt chart and performance visualization  

---

## System Architecture  
A high-level schematic/diagram goes here.  

| Component          | Role in Simulation                           |
|--------------------|----------------------------------------------|
| Instruction Queue  | Holds decoded instructions before issue      |
| Reservation Stations | Tracks issued instructions awaiting operands |
| LSQ                | Handles memory ops with dependency checks    |
| ROB                | Maintains program order and ensures commit   |
| L1 Data Cache      | First-level memory access                    |
| Victim Cache       | Reduces conflict misses from L1D             |

---

## Experimental Setup  
- Workloads used (e.g., matrix multiplication, sorting, etc.)  
- Simulation parameters (cache size, associativity, ROB size, victim cache size, etc.)  
- Metrics collected (IPC, miss rate, hit rate, stall cycles, etc.)  

---

## Results & Analysis  

### Associativity vs Victim Cache  
Insert plot (PNG).  
Short takeaway sentence.  

---

### Victim Cache Hit Rate Across Workloads  
Insert plot (PNG).  
Short takeaway sentence.  

---

### Cycle-by-Cycle Execution Trace (Sample)  
Insert a snippet of output table/trace for illustration.  

---

## Example Output (Statistics Snapshot)  
```txt
Cycle 120:
  - Instruction: MUL R1, R2, R3 issued
  - ROB Status: ...
  - Cache Access: Miss in L1, Hit in Victim
