# Garbage Collection Simulator

This simulator manages a city's garbage collection by tracking bin fill levels, predicting overflows, and planning efficient collection routes. The system visualizes operations in real-time through a terminal interface.  

## Features

- **Route Optimization** - Finds shortest paths between bins and disposal sites
- **Overflow Prediction** - Uses historical data to forecast which bins need urgent attention
- **Dynamic Scheduling** - Adapts collection routes based on current bin states
- **Real-time Visualization** - Interactive terminal UI showing truck movement and bin status


Built with custom implementations of:
- Dijkstra's algorithm for pathfinding
- Min-heap priority queue for scheduling
- Hash tables for location mapping
- Graph data structure for city network
