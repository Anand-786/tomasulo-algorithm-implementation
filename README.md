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

## Experiments & Analysis  

### 1. Associativity vs Victim Cache  
Insert plot (PNG).  
Short takeaway sentence.  

---

### 2. Victim Cache Hit Rate Across Workloads  
Insert plot (PNG).  
Short takeaway sentence.  

---

### 3. Cycle-by-Cycle Execution Trace (Sample)  
Insert a snippet of output table/trace for illustration.  

---

## Example Output (Statistics Snapshot)  
```txt
Cycle 120:
  - Instruction: MUL R1, R2, R3 issued
  - ROB Status: ...
  - Cache Access: Miss in L1, Hit in Victim
