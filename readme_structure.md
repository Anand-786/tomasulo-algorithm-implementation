# Tomasulo Algorithm Implementation

👉 Related project: [Victim Cache Analysis in gem5](https://github.com/Anand-786/gem5-victim-cache)

An out-of-order CPU simulator implementing Tomasulo's dynamic scheduling algorithm with modern architectural enhancements. Features comprehensive pipeline modeling, configurable microarchitectural parameters, and detailed performance analysis capabilities.

## Table of Contents

- [Overview](#overview)
- [Architecture](#architecture)
- [Implementation Details](#implementation-details)
- [Experiments & Analysis](#experiments--analysis)
- [Build & Usage](#build--usage)
- [Performance Results](#performance-results)
- [Technical Documentation](#technical-documentation)
- [Contributing](#contributing)

## Overview

This simulator implements a sophisticated out-of-order execution engine based on Tomasulo's algorithm, enhanced with modern architectural features including:

- **Reorder Buffer (ROB)** for precise exception handling and speculation recovery
- **Load-Store Queue (LSQ)** with store-to-load forwarding optimization
- **Multi-level cache hierarchy** with L1 data cache and victim cache integration
- **Configurable reservation stations** for different functional units
- **Cycle-accurate simulation** with comprehensive performance metrics

The implementation prioritizes architectural accuracy while maintaining educational clarity, making it suitable for both research and instructional purposes.

## Architecture

### Core Components

```
┌─────────────────┐    ┌──────────────────┐    ┌─────────────────┐
│   Fetch/Decode  │ -> │ Reservation      │ -> │   Execute       │
│                 │    │ Stations         │    │                 │
└─────────────────┘    └──────────────────┘    └─────────────────┘
         │                       │                        │
         v                       v                        v
┌─────────────────┐    ┌──────────────────┐    ┌─────────────────┐
│ Reorder Buffer  │ <- │ Load-Store Queue │ <- │ Memory System   │
│                 │    │                  │    │ (L1 + Victim)   │
└─────────────────┘    └──────────────────┘    └─────────────────┘
```

### Key Features

- **Dynamic Scheduling**: Instructions execute as soon as operands are available
- **Speculative Execution**: Branch prediction with rollback capability
- **Memory Disambiguation**: Advanced load-store dependency resolution
- **Cache Hierarchy**: Realistic memory subsystem modeling

## Implementation Details

### Microarchitectural Parameters

| Component | Default Config | Range | Description |
|-----------|----------------|-------|-------------|
| ROB Size | 64 entries | 16-128 | Reorder buffer capacity |
| LSQ Size | 32 entries | 8-64 | Load-store queue depth |
| RS Count | 6 stations | 2-12 | Reservation stations per FU type |
| L1 Cache | 32KB, 8-way | - | Data cache configuration |
| Victim Cache | 8 entries | 4-16 | Fully associative victim buffer |

### Core Algorithms

**Tomasulo Scheduling**
```cpp
// Simplified dispatch logic
if (rs_available && rob_space) {
    dispatch_instruction(instr);
    update_dependencies();
}
```

**Store-to-Load Forwarding**
```cpp
// LSQ forwarding check
if (load_addr_matches_store() && store_data_ready()) {
    forward_data_from_store();
} else {
    access_cache_hierarchy();
}
```

## Experiments & Analysis

### 1. Reorder Buffer Scaling Analysis
**Objective**: Quantify ILP extraction vs. ROB size across different workload characteristics.

**Methodology**: 
- ROB sizes: 16, 32, 64, 128 entries
- Workloads: integer, floating-point, mixed
- Metrics: IPC, pipeline utilization, commit bandwidth

**Key Findings**:
- Diminishing returns beyond 64 entries for most workloads
- Memory-intensive applications benefit more from larger ROBs
- [Detailed results in `/results/rob_scaling/`]

### 2. Store-to-Load Forwarding Impact
**Objective**: Measure performance benefits of memory disambiguation techniques.

**Configurations**:
- Baseline: No forwarding
- Conservative: Address-based forwarding
- Aggressive: Speculative forwarding with rollback

**Metrics**:
- Load latency reduction
- Memory stall cycles
- Forwarding accuracy rates

### 3. Victim Cache Effectiveness Study
**Objective**: Analyze victim cache impact on cache hierarchy performance.

**Variables**:
- Victim cache sizes: 0, 4, 8, 16 entries
- L1 miss patterns
- Working set characteristics

**Analysis Focus**:
- Conflict miss reduction
- Average memory access time
- Cache hierarchy traffic patterns

### 4. Functional Unit Scaling
**Objective**: Determine optimal reservation station allocation.

**Parameters**:
- ALU/FPU reservation stations: 2-8 each
- Load/Store units: 1-4
- Branch units: 1-2

## Build & Usage

### Prerequisites
```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake

# macOS
brew install cmake
```

### Compilation
```bash
git clone https://github.com/yourusername/tomasulo-simulator.git
cd tomasulo-simulator
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Basic Usage
```bash
# Run with default configuration
./tomasulo_sim traces/example.trace

# Custom configuration
./tomasulo_sim --rob-size 128 --lsq-size 64 traces/workload.trace

# Generate detailed statistics
./tomasulo_sim --verbose --stats-file results.json traces/benchmark.trace
```

### Configuration Options
```bash
Usage: ./tomasulo_sim [OPTIONS] TRACE_FILE

Options:
  --rob-size SIZE        Reorder buffer entries (default: 64)
  --lsq-size SIZE        Load-store queue size (default: 32)
  --rs-count COUNT       Reservation stations per type (default: 6)
  --l1-size SIZE         L1 cache size in KB (default: 32)
  --victim-entries NUM   Victim cache entries (default: 8)
  --verbose              Enable cycle-by-cycle tracing
  --stats-file FILE      Output statistics to JSON file
```

## Performance Results

### Benchmark Performance Summary

| Workload | IPC | L1 Hit Rate | ROB Utilization | Notable Characteristics |
|----------|-----|-------------|-----------------|-------------------------|
| IntegerMM | 2.3 | 94.2% | 78% | High arithmetic intensity |
| FloatFFT | 1.8 | 87.5% | 85% | Memory streaming patterns |
| MixedSort | 2.1 | 91.3% | 72% | Control flow intensive |

### Scalability Analysis
- **ROB Size vs IPC**: Optimal at 64 entries for typical workloads
- **Cache Miss Impact**: 15% IPC degradation per 5% miss rate increase  
- **Forwarding Benefit**: 8-12% performance improvement with aggressive LSQ forwarding

## Technical Documentation

### Trace Format
Instructions follow a standardized format for reproducible simulation:
```
<PC> <opcode> <dest_reg> <src1_reg> <src2_reg> [immediate/address]
```

### Output Statistics
The simulator generates comprehensive metrics including:
- Pipeline stage utilization
- Cache hierarchy statistics  
- Branch prediction accuracy
- Memory disambiguation effectiveness
- Cycle-by-cycle execution traces

### Validation Methodology
Implementation correctness verified through:
- Unit tests for individual components
- Integration testing with known benchmarks
- Comparison against reference architectural simulators

## Contributing

We welcome contributions that enhance the simulator's capabilities or educational value. Please ensure:

- Code follows existing style conventions
- New features include corresponding test cases
- Performance modifications are validated against baseline results
- Documentation updates accompany functional changes

## License

MIT License - see LICENSE file for details.

---

*For questions regarding implementation details or experimental methodology, please open an issue or contact the maintainers.*