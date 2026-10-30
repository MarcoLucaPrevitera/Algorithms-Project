# GraphRanker

**An efficient graph-ranking system implemented in C11.**

GraphRanker is a command-line application that processes directed, weighted graphs and maintains a ranking of the top `k` graphs based on their shortest-path distances from a designated source node. It was developed as part of the Algorithms and Data Structures coursework project. 

The project was designed to evaluate algorithmic correctness and efficiency through automated testing, including execution-time and memory-consumption measurements.

## Table of Contents

- [Overview](#overview)
- [How It Works](#how-it-works)
- [Input Format](#input-format)
- [Example](#example)
- [Technologies](#technologies)
- [Compilation and Execution](#compilation-and-execution)

## Overview

The application receives a sequence of commands describing graphs and ranking queries. Each graph is represented by an adjacency matrix and assigned a unique integer index based on its insertion order.

The ranking is determined by the following metric:
**The sum of the shortest-path distances from node `0` to every other reachable node.**
Graphs with lower metric values receive higher rankings.

When multiple graphs have the same metric value, insertion order is used as a tie-breaker, giving priority to the graph added first.

This project demonstrates practical experience with:

- **Algorithms and Data Structures:** Applying algorithmic techniques to graph-processing problems.
- **Graph Theory:** Working with directed, weighted graphs and adjacency matrices.
- **Shortest-Path Algorithms:** Computing shortest-path distances from a source vertex.
- **Ranking and Data Management:** Maintaining a ranking under sequential insertions and queries.
- **Algorithmic Complexity:** Considering execution time and memory consumption.
- **Low-Level Programming:** Implementing a solution in C11 under strict library and execution constraints.

## How It Works

The program supports two commands:

### 1. `AggiungiGrafo`

Adds a new graph to the collection.

Each graph is represented by a `d × d` adjacency matrix, where `d` is the number of vertices.

Matrix entries represent edge weights. A weight of zero represents the absence of an edge, according to the input representation.

Each graph receives an incremental index, starting from `0`.

### 2. `TopK`

Returns the indices of the `k` graphs with the smallest metric values among those processed so far, calculated as defined in the project requirements.

## Input Format

The first line contains two integers:

| Parameter | Description |
|---|---|
| `d` | Number of vertices in each graph |
| `k` | Number of graphs to return in each ranking query |

The remaining input consists of a sequence of commands.

**Example command structure:**

```text
d,k
AggiungiGrafo
row_1
row_2
...
row_d
TopK
```

Each adjacency matrix row contains `d` comma-separated integer values.

## Example

**Input:**

```text
3,2
AggiungiGrafo
0,4,3
0,2,0
2,0,0
AggiungiGrafo
0,0,2
7,0,4
0,1,0
AggiungiGrafo
3,1,8
0,0,5
0,9,0
TopK
```

**Output:**

```text
0 1
```

The output contains the indices of the two graphs with the lowest metric values.

## Technologies

| Technology | Purpose |
|---|---|
| C11 | Core implementation |
| Standard C Library | Input/output and standard operations |
| GCC | Compilation |

**Implementation constraints:**
- No external libraries.
- No multithreading.
- Variable Length Arrays (VLAs) are permitted.
- Input is read from `stdin`.
- Output is written to `stdout`.

## Compilation and Execution

### Prerequisites

- GCC or another C11-compatible compiler.
- A Unix-like environment or compatible C development environment.

### Compile

```bash
gcc -std=c11 -O2 -Wall -Wextra main.c -o graphranker
```

### Run

```bash
./graphranker
```

The application reads commands from standard input and writes ranking results to standard output.

---
