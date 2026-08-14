# Custom CPU Scheduler Simulator

## 📌 Project Overview
This project is a Custom CPU Scheduler Simulator implemented in C, designed to evaluate process scheduling performance with a strong emphasis on **Real-Time processing** and **Dynamic Priority Penalty (Aging)**. 

The scheduler uses a **Multi-Level Queue** architecture consisting of 5 distinct queues, heavily prioritizing real-time tasks by guaranteeing their non-preemptive execution, while applying a time-quantum-based penalty system for lower-priority tasks to prevent starvation and ensure fair CPU distribution.

## ⚙️ Scheduling Architecture

The system classifies processes into 5 queues (Q0 ~ Q4) based on their initial priority values:

*   **Q0 (Priority -10 ~ -1) : Real-Time Queue**
    *   **Policy:** Non-preemptive
    *   **Behavior:** Once a process in Q0 acquires the CPU, it is guaranteed its full `computing_time` without any time quantum interruptions. This ensures strict real-time deadlines are met.
*   **Q1 ~ Q3 (Priority 1 ~ 30) : Priority-based Queue with Penalty**
    *   **Policy:** Preemptive (Time Quantum-based)
    *   **Behavior:** Processes are guaranteed execution up to the time quantum. If a process does not finish within the quantum, it receives a **Penalty (+10 to priority)**, effectively demoting it to a lower queue to yield the CPU to other tasks. (Priority is capped at 31).
*   **Q4 (Priority 31) : Round-Robin Queue**
    *   **Policy:** Round-Robin (RR)
    *   **Behavior:** Standard RR scheduling. If a process exceeds the time quantum, it is re-enqueued at the back of Q4 without additional penalties.

## 🛠️ Data Structures
*   **Process Control Block (PCB):** Stores essential process metadata (`pid`, `priority`, `computing_time`, `remaining_time`, `arrival_time`, `end_time`).
*   **Doubly Linked List Queue:** Custom-built doubly linked list to support both standard FIFO enqueue/dequeue and priority-based insertion (`enqueue_by_priority`) for Q1~Q3.

## 📊 Performance Analysis
According to the simulation results using standard test sets:
*   The scheduler successfully guaranteed the immediate execution of critical tasks assigned to the real-time queue (Q0), completely isolating them from the overhead of time quanta.
*   The Normalized Turnaround Time (NTT) analysis demonstrated that this architecture is highly advantageous for systems where the strict real-time execution of high-priority processes is the absolute primary goal, compared to standard mixed-algorithm schedulers.

## 🚀 How to Build and Run
```bash
# Clone the repository
git clone [https://github.com/your-username/cpu-scheduler-simulator.git](https://github.com/your-username/cpu-scheduler-simulator.git)
cd cpu-scheduler-simulator

# Build the project using Makefile
make

# Run the simulator
./scheduler.out