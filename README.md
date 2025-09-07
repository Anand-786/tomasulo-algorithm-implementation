👉  Related project : [Victim Cache Analysis in gem5](https://github.com/Anand-786/gem5-victim-cache)

# Tomasulo Simulator  

An Out-of-Order CPU simulator implementing Tomasulo’s Algorithm with LSQ, ROB, L1 Data Cache, and Victim Cache. Features configurable parameters, cycle-by-cycle tracing, statistics generation, and Gantt chart visualization. Designed for education and computer architecture research.


## Project Showcase  

<p align="center">
  <img src="assets/simulator_startup.png" alt="Simulator Startup Output" width="90%">
</p>

<p align="center">
  <img src="assets/gantt_chart.png" alt="Gantt Chart Visualization" width="45%">
  <img src="assets/trace_output.png" alt="Cycle-by-Cycle Trace" width="45%">
</p>

## System Architecture

<p align="center">
  <img src="assets/arch1.png" alt="System Architecture Diagram" height="300px">
</p>  

<p align="center">
  <i>Source: ReViCe: Reusing Victim Cache to Prevent Speculative Cache Leakage - Scientific Figure on ResearchGate</i>
</p> 
 
## Components and Features  

| Component / Feature         | Config Parameter(s)                           |
|-----------------------------|-----------------------------------------------|
| Reorder Buffer (ROB)        | `rob_size`                                    |
| Load/Store Queue (LSQ)      | `lsq_size`                                    |
| L1 Data Cache               | `num_sets`, `associativity`, `block_size`     |
| Victim Cache                | `victim_cache_size`, `vc_access_latency`      |
| ALU RS                      | `num_alu_rs`, `alu_latency`                   |
| MUL RS                      | `num_mul_rs`, `mul_latency`                   |
| DIV RS                      | `num_div_rs`, `div_latency`                   |
| Memory Subsystem            | `mem_fetch_latency`, `wb_latency`             |
| Program Input               | `program_file_path`                           |
| Cycle-by-Cycle Trace        | `trace_file_enabled`                          |
| Gantt Chart                 | `gantt_chart_enabled`                         |
| Statistics Generation       | (auto-generated)                              |
| Register File               | (fixed)                                       |

## Experiments & Results  

### 1. Impact of ROB Size vs. IPC  

**Objective:**  
Analyze how the performance benefit of a large Re-Order Buffer (ROB) changes when executing instructions with high versus low latency.  

**Experimental Setup:**  

| Parameter              | Value                         |
|------------------------|-------------------------------|
| MUL Latency            | 1 cycle(low), 5 cycles(high)  |
| Reservation Stations   | 32                            |
| L1-D size              | 32 KB                         |
| #iterations            | 100                           |

<p align="center">
  <img src="assets/ipc-vs-rob.png" width="90%">
</p>

**Key Takeaway:**  
A large ROB is most valuable when hiding high instruction latencies. With low-latency instructions, performance saturates quickly with a much smaller ROB.  

### 2. Victim Cache Effectiveness and Thrashing Point  

**Objective:**  
Quantify the performance improvement from a Victim Cache (VC) and identify its breaking point under increasing memory conflict pressure.  

**Experimental Setup:**  

| Parameter           | Value                           |
|---------------------|---------------------------------|
| L1 D-Cache          | 4KB, Direct-Mapped, 64B Block   |
| Victim Cache        | 4 Entries                       |
| Mem Access Penalty  | 20 cycles                       |
| VC Hit Penalty      | 3 cycles                        |

<p align="center">
  <img src="assets/vc_vs_n.png" width="45%">
  <img src="assets/avg_lat_vs_n.png" width="45%">
</p>

**Key Takeaway:**  
The VC kept the Average Memory Access Time (AMAT) extremely low until the number of conflicting addresses exceeded its capacity at N=6, causing thrashing. This confirms the VC's effectiveness in mitigating conflict misses up to its design limit.  

### 3. LSQ Effectiveness: Store-to-Load Forwarding  

**Objective:**  
Demonstrate correct implementation of Store-to-Load Forwarding (STLF) by comparing a workload with a direct memory dependency to a control workload.  

**Experimental Setup:**  

| Parameter       | Value                          |
|-----------------|--------------------------------|
| L1 D-Cache      | 1KB, Direct-Mapped, 32B Block  |
| LSQ Size        | 32 Entries                     |
| Victim Cache    | Disabled                       |
| STLF            | Enabled                        |

<p align="center">
  <img src="assets/ipc_vs_stlf.png" width="90%">
</p>

**Key Takeaway:**  
The LSQ’s ability to forward data from a recent store to a dependent load resulted in a **2.93× IPC speedup**. This highlights STLF as a critical optimization for resolving memory-based data hazards.  

## Example Output (Statistics Snapshot)  
```txt
Cycle 120:
  - Instruction: MUL R1, R2, R3 issued
  - ROB Status: ...
  - Cache Access: Miss in L1, Hit in Victim
