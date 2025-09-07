👉  Related project : [Victim Cache Analysis in gem5](https://github.com/Anand-786/gem5-victim-cache)

# Tomasulo Simulator  

An Out-of-Order CPU simulator implementing Tomasulo’s Algorithm with LSQ, ROB, L1 Data Cache, and Victim Cache. Features configurable parameters, cycle-by-cycle tracing, statistics generation, and Gantt chart visualization. Designed for education and computer architecture research.


## Project Showcase  

<p align="center">
  <img src="assets/simulator_startup.png" alt="Simulator Startup Output" width="92%">
</p>

<p align="center">
  <img src="assets/gantt_chart.png" alt="Gantt Chart Visualization" width="46%">
  <img src="assets/trace_output.png" alt="Cycle-by-Cycle Trace" width="46%">
</p>

## System Architecture

<p align="left">
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

---

## Experiments & Results  

I performed 3 main experiments on the Simulator. Here are their objectives, setup, results and key takeaways.


## 1. Impact of ROB Size vs. IPC  

### Objective:  

To study how the benefit of increasing the Re-Order Buffer (ROB) size changes under workloads with different instruction latencies. 


### Experimental Setup:  

| Parameter              | Value                         |
|------------------------|-------------------------------|
| MUL Latency            | 1 cycle(low), 5 cycles(high)  |
| Reservation Stations   | 32                            |
| L1-D size              | 32 KB                         |
| #iterations            | 100                           |


<p align="left">
  <img src="assets/ipc-vs-rob.png" width="60%">
</p>


### Key Takeaway:  

When instruction latencies are high (e.g., long MUL operations), a larger ROB allows more overlap and better latency hiding, leading to clear performance gains. For low-latency instructions, however, the performance improvement saturates quickly, and increasing the ROB further offers little additional benefit.

---

## 2. Victim Cache Effectiveness and Thrashing Point  

### Objective:

To evaluate the effectiveness of a Victim Cache (VC) in reducing conflict misses penalties and to identify the point at which it stops being useful. 


### Experimental Setup:  

| Parameter           | Value                           |
|---------------------|---------------------------------|
| L1 D-Cache          | 4KB, Direct-Mapped, 64B Block   |
| Victim Cache        | 4 Entries                       |
| Mem Access Penalty  | 20 cycles                       |
| VC Hit Penalty      | 3 cycles                        |


<p align="left">
  <img src="assets/vc_vs_n.png" width="48%">
  <img src="assets/avg_lat_vs_n.png" width="48%">
</p>


### Key Takeaway:

The VC substantially reduced the Average Memory Access Time (AMAT) up to the point where the number of conflicting addresses exceeded its capacity (around N=6). Beyond this point, thrashing occurred and the benefit disappeared. This indicates that the VC works well in mitigating conflict misses, but only within the limits of its small storage capacity.

---

## 3. LSQ Effectiveness: Store-to-Load Forwarding  

### Objective:

To test whether the Load-Store Queue (LSQ) correctly performs Store-to-Load Forwarding (STLF), using a workload with explicit memory dependencies.


### Experimental Setup: 

| Parameter       | Value                          |
|-----------------|--------------------------------|
| L1 D-Cache      | 1KB, Direct-Mapped, 32B Block  |
| LSQ Size        | 32 Entries                     |
| Victim Cache    | Disabled                       |
| STLF            | Enabled                        |


<p align="left">
  <img src="assets/ipc_vs_stlf.png" width="60%">
</p>


### Key Takeaway:
  
With STLF enabled, dependent loads were able to receive data directly from earlier stores, avoiding cache access delays. This resulted in a 2.93× increase in IPC compared to the baseline workload. The result shows that STLF is not just a correctness feature but also an important optimization for workloads with memory-based dependencies.  

---

## Usage  

For full setup and run instructions, see [Usage Guide](./USAGE.md).  

## Sample Outputs   

Below is an example of the simulator’s terminal output when starting a run : 

```


    $$$$$$$$\  $$$$$$\  $$\      $$\  $$$$$$\   $$$$$$\  $$\   $$\ $$\       $$$$$$\         $$$$$$\  $$$$$$\ $$\      $$\ $$\   $$\ $$\        $$$$$$\ $$$$$$$$\  $$$$$$\  $$$$$$$\ 
    \__$$  __|$$  __$$\ $$$\    $$$ |$$  __$$\ $$  __$$\ $$ |  $$ |$$ |     $$  __$$\       $$  __$$\ \_$$  _|$$$\    $$$ |$$ |  $$ |$$ |      $$  __$$\\__$$  __|$$  __$$\ $$  __$$\ 
       $$ |   $$ /  $$ |$$$$\  $$$$ |$$ /  $$ |$$ /  \__|$$ |  $$ |$$ |     $$ /  $$ |      $$ /  \__|  $$ |  $$$$\  $$$$ |$$ |  $$ |$$ |      $$ /  $$ |  $$ |   $$ /  $$ |$$ |  $$ |
       $$ |   $$ |  $$ |$$\$$\$$ $$ |$$$$$$$$ |\$$$$$$\  $$ |  $$ |$$ |     $$ |  $$ |      \$$$$$$\    $$ |  $$\$$\$$ $$ |$$ |  $$ |$$ |      $$$$$$$$ |  $$ |   $$ |  $$ |$$$$$$$  |
       $$ |   $$ |  $$ |$$ \$$$  $$ |$$  __$$ | \____$$\ $$ |  $$ |$$ |     $$ |  $$ |       \____$$\   $$ |  $$ \$$$  $$ |$$ |  $$ |$$ |      $$  __$$ |  $$ |   $$ |  $$ |$$  __$$< 
       $$ |   $$ |  $$ |$$ |\$  /$$ |$$ |  $$ |$$\   $$ |$$ |  $$ |$$ |     $$ |  $$ |      $$\   $$ |  $$ |  $$ |\$  /$$ |$$ |  $$ |$$ |      $$ |  $$ |  $$ |   $$ |  $$ |$$ |  $$ |
       $$ |    $$$$$$  |$$ | \_/ $$ |$$ |  $$ |\$$$$$$  |\$$$$$$  |$$$$$$$$\ $$$$$$  |      \$$$$$$  |$$$$$$\ $$ | \_/ $$ |\$$$$$$  |$$$$$$$$\ $$ |  $$ |  $$ |    $$$$$$  |$$ |  $$ |
       \__|    \______/ \__|     \__|\__|  \__| \______/  \______/ \________|\______/        \______/ \______|\__|     \__| \______/ \________|\__|  \__|  \__|    \______/ \__|  \__|                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
                       =========================[ An Out-of-Order CPU Simulator based on Tomasulo's Algorithm with L1 & Victim Cache ]=========================


       
 [ CONFIGURATION ]
 +------------------------------------------------------------+
 | > L1 Cache Properties :                                    |
 |     - Size                  : 4 KB                         |
 |     - #Sets                 : 32                           |
 |     - Associativity         : 2-way                        |
 |     - Block Size            : 64 B                         |
 |     - Physical Address      : 32 bits                      |
 |     - Victim Cache Size     : 8                            |
 |     - Victim Cache Access   : 3 cycles                     |
 |     - Memory Fetch Latency  : 10 cycles                    |
 |     - Writeback Latency     : 5 cycles                     |
 |                                                            |
 | > Simulator Properties :                                   |
 |     - Program File          : program.asm                  |
 |     - #Iterations           : 1                            |
 |     - Victim Cache          : Enabled                      |
 |     - Logs                  : Enabled                      |
 |     - Gantt Chart           : Enabled                      |
 |     - Show Status Table     : On                           |
 |     - ROB entries           : 16                           |
 |     - LSQ entries           : 8                            |
 |     - Reservation Stations  : 16 (ALU), 8 (MUL), 4 (DIV)   |
 |     - Latencies (cycles)    : 1 (ALU), 5 (MUL), 10 (DIV)   |
 +------------------------------------------------------------+


 [ INFO ] Executing Program...
 [ DONE ] Simulation Completed Successfully.


 [ FINAL INSTRUCTION STATUS ]
 +-------------------------------------------------------------------------------------+
 |  Instruction               | Issue  | ExecStart  | ReadMem  | WriteCDB  | Commit    | 
 |-------------------------------------------------------------------------------------| 
 |  LOAD  R2, 0(R10)          | 1      | 2          | 12       | 13        | 14        | 
 |  LOAD  R3, 0(R12)          | 2      | 3          | 13       | 14        | 15        | 
 |  ADD   R4, R10, R11        | 3      | 4          | -        | 5         | 16        | 
 |  MUL   R5, R12, R13        | 4      | 5          | -        | 10        | 17        | 
 |  LOAD  R6, 0(R14)          | 5      | 6          | 14       | 15        | 18        | 
 |  LOAD  R7, 0(R16)          | 6      | 7          | 15       | 16        | 19        | 
 |  ADD   R8, R14, R15        | 7      | 8          | -        | 9         | 20        | 
 |  MUL   R9, R16, R17        | 8      | 10         | -        | 17        | 21        | 
 |  LOAD  R18, 0(R18)         | 9      | 10         | 16       | 18        | 22        | 
 |  LOAD  R19, 0(R20)         | 10     | 11         | 17       | 19        | 23        | 
 |  ADD   R20, R18, R19       | 11     | 20         | -        | 21        | 24        | 
 |  MUL   R21, R20, R21       | 12     | 22         | -        | 27        | 28        | 
 |  LOAD  R22, 0(R22)         | 13     | 14         | 18       | 20        | 29        | 
 |  LOAD  R23, 0(R24)         | 14     | 15         | 19       | 22        | 30        | 
 |  ADD   R24, R22, R23       | 15     | 23         | -        | 24        | 31        | 
 |  MUL   R25, R24, R25       | 16     | 27         | -        | 32        | 33        | 
 |  BEQ   R1, R0, LOOP        | 17     | 18         | -        | -         | 34        | 
 +-------------------------------------------------------------------------------------+


 [ SIMULATION SUMMARY ]
 +----------------------------------------------------------+
 |  - Total Execution Cycles          : 34                  |
 |  - Total Instructions Committed    : 17                  |
 |  - IPC (Instructions/Cycle)        : 0.50                |
 |  - Wall-Clock Time                 : 5.00 ms             |
 |  - Avg. Memory Access Time[AMAT]   : 2.12 cycles         |
 |  - L1 Cache Miss Rate              : 12.50 %             |
 |  - Avg. L1 Miss Penalty            : 9.00 cycles         |
 |  - Avg. Instruction Latency        : 14.18 cycles        |
 +----------------------------------------------------------+


 [ SIMULATION OUTPUT FILES ]
 +----------------------------------------------------------+
 |  > Detailed trace log:      results/trace.log            |
 |  > Full statistics report:  results/stats.txt            |
 +----------------------------------------------------------+



``` 

### Gantt Chart Visualization  

The simulator generates a Gantt chart to visualize instruction progress through pipeline stages.  

<p align="left">
  <img src="assets/gantt_chart.png" alt="Sample Gantt Chart" width="60%">
</p>  

Each row corresponds to an instruction, and colored bars denote its active stage per cycle, making pipeline overlap and stalls easy to inspect.

### Trace Log (Excerpt)  

Below is a snippet from the generated `trace.log` file, showing the cycle-by-cycle execution of instructions : 

```
Integer-ALU Reservation Stations
Busy  | Op      | Qj      | Qk      | Vj      | Vk      | Dest  
-----------------------------------------------------------------

Multiplier Reservation Stations
Busy  | Op      | Qj      | Qk      | Vj      | Vk      | Dest  
-----------------------------------------------------------------
Yes   | MUL     | ROB 14  | -       | -       | 0       | ROB 15

Divider Reservation Stations
Busy  | Op      | Qj      | Qk      | Vj      | Vk      | Dest  
-----------------------------------------------------------------

Reorder Buffer [ROB : (Head => Tail) OR (Oldest => Newest)]
Entry   | Type    | Dest      | Value     | Done  
--------------------------------------------------
ROB 10  | ADD     | R20       | 0         | Yes   
ROB 11  | MUL     | R21       | -         | No    
ROB 12  | LOAD    | R22       | 0         | Yes   
ROB 13  | LOAD    | R23       | 0         | Yes   
ROB 14  | ADD     | R24       | -         | No    
ROB 15  | MUL     | R25       | -         | No    
ROB 0   |         | R0        | 0         | Yes   

Load/Store Queue [LSQ : (Head => Tail) OR (Oldest => Newest)]
Entry   | Type    | EA    | Value     | Dest      | Offset   | Base Reg Value    | Ready   | CDB Write   
---------------------------------------------------------------------------------------------------------
LSQ 6   | LOAD    | 0     | 0         | ROB 12    | 0        | 0                 | YES     | DONE        
LSQ 7   | LOAD    | 0     | 0         | ROB 13    | 0        | 0                 | YES     | DONE        

Register File & Status
 R0     | R1     | R2     | R3     | R4     | R5     | R6     | R7     | R8     
--------------------------------------------------------------------------------
 Ready  | Ready  | Ready  | Ready  | Ready  | Ready  | Ready  | Ready  | Ready  
 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      | 0      

 Memory State
-

----------------------------------------------------------------------------------------------- Cycle 24 ---

```  

---

## Future Work  

Several directions can extend this simulator for deeper exploration:  

- **Branch Prediction**: Add static/dynamic prediction schemes and study misprediction penalties.
- **Multi-Core Support**: Extend the simulator to model multi-core with a shared cache hierarchy and coherence.
- **Visualization Enhancements**: Web-based dashboards for pipeline analysis.
- **Superscalar**: Add multi-issue and multiple CDB write support for increasing IPC>1.  

