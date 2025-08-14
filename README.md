# Multi-threaded Web Crawler

A high-performance, multi-threaded web crawler built in C++ with advanced graph analysis capabilities. This project demonstrates systems programming, concurrency, and algorithm implementation skills.

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)]()
[![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)]()
[![License](https://img.shields.io/badge/license-MIT-green.svg)]()

## 🚀 Features

- **Multi-threaded Architecture**: Concurrent crawling with configurable thread pools
- **Graph Analysis**: Implementation of PageRank, BFS/DFS, shortest paths, and connected components
- **URL Processing**: Robust URL parsing, validation, and normalization
- **Rate Limiting**: Respectful crawling with configurable delays
- **Thread-safe Data Structures**: Custom implementations of concurrent queues and graph storage
- **Performance Metrics**: Detailed statistics and benchmarking
- **Configurable**: YAML-based configuration with CLI overrides
- **Report Generation**: HTML reports with graph visualizations

## 🛠 Technologies Used

- **C++17** - Modern C++ with STL containers and threading
- **libcurl/cpp-httplib** - HTTP client for web requests
- **CMake** - Cross-platform build system
- **Multi-threading** - `std::thread`, `std::mutex`, `std::condition_variable`
- **Graph Algorithms** - Custom implementations of classic CS algorithms

## 📊 Algorithms Implemented

### Graph Algorithms
- **Breadth-First Search (BFS)** - Web crawling traversal
- **Depth-First Search (DFS)** - Cycle detection and connectivity
- **Dijkstra's Algorithm** - Shortest path analysis
- **Union-Find** - Connected components detection
- **PageRank** - Page importance ranking

### Data Structures
- **Thread-safe Queue** - Producer-consumer pattern
- **Adjacency List Graph** - Memory-efficient graph representation
- **Hash Tables** - URL deduplication and fast lookups
- **Thread Pool** - Efficient worker thread management

## 🏗 Architecture Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                        Main Application                         │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐  │
│  │   URL Parser    │  │  Crawler Engine │  │ Graph Analyzer  │  │
│  │   - Validate    │  │   - Orchestrate │  │ - Run algorithms│  │
│  │   - Normalize   │  │   - Manage      │  │ - Generate      │  │
│  │   - Extract     │  │     threads     │  │   reports       │  │
│  └─────────────────┘  └─────────────────┘  └─────────────────┘  │
└─────────────────────────────────────────────────────────────────┘
                                 │
                  ┌─────────────────────────────┐
                  │       Thread Pool           │
                  │  ┌────────┐  ┌────────┐     │
                  │  │Worker 1│  │Worker N│ ... │
                  │  └────────┘  └────────┘     │
                  └─────────────────────────────┘
                                 │
        ┌────────────────────────┼────────────────────────┐
        │                        │                        │
┌───────▼────────┐     ┌─────────▼────────┐     ┌─────────▼────────┐
│   URL Queue    │     │   HTTP Client    │     │   HTML Parser    │
│ - Thread-safe  │     │ - GET requests   │     │ - Extract links  │
│ - BFS ordering │     │ - Handle errors  │     │ - Clean HTML     │
│ - Deduplication│     │ - Respect robots │     │ - Follow rules   │
└────────────────┘     └──────────────────┘     └──────────────────┘
