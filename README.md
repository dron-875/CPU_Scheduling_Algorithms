# CPU_Scheduling_Algorithms
This project implements various CPU scheduling algorithms in C++, including First-Come-First-Serve (FCFS), Shortest Job First (SJF), Round Robin, Priority Scheduling, Shortest Remaining Job First (SRJF), and Highest Response Ratio Next (HRRN). It includes a user-interactive menu to select the desired algorithm and displays detailed process information and a Gantt chart for visualization. The project showcases the understanding and practical application of different scheduling techniques in operating systems.

I have provided the sample test cases in the (input.txt) file for verifying the output.

The main.c file contains the code.

Here are the concise definitions, advantages, and disadvantages of each scheduling algorithm:

1. **First Come First Serve (FCFS) scheduling**
   - **Definition:** Processes are executed in the order they arrive.
   - **Advantage:** Simple and easy to implement.
   - **Disadvantage:** Can lead to the "convoy effect," where short processes wait behind long ones.

2. **Round Robin (RR) scheduling**
   - **Definition:** Each process is assigned a fixed time slice and is rotated through in a cyclic order.
   - **Advantage:** Fair and responsive to all processes.
   - **Disadvantage:** Can lead to high context switching overhead.

3. **Shortest Job First (SJF) scheduling**
   - **Definition:** Processes with the shortest burst time are executed first.
   - **Advantage:** Minimizes average waiting time.
   - **Disadvantage:** Can lead to starvation of longer processes.

4. **Shortest Remaining Time First (SRTF) scheduling**
   - **Definition:** Preemptive version of SJF where the process with the shortest remaining time is executed next.
   - **Advantage:** Further reduces average waiting time compared to SJF.
   - **Disadvantage:** Can lead to even more severe starvation issues.

5. **Highest Priority (HP) scheduling**
   - **Definition:** Processes are executed based on priority, with the highest priority process going first.
   - **Advantage:** Ensures critical tasks are completed first.
   - **Disadvantage:** Lower priority processes may suffer from starvation.

6. **Highest Priority (Preemptive) scheduling**
   - **Definition:** Highest priority process is executed first, and it can preempt running lower priority processes.
   - **Advantage:** Ensures high-priority tasks are immediately addressed.
   - **Disadvantage:** Increased complexity and potential for higher overhead.

7. **Highest Response Ratio Next (HRRN) scheduling**
   - **Definition:** Process with the highest response ratio (waiting time + burst time / burst time) is executed next.
   - **Advantage:** Balances between SJF and FCFS, reducing starvation.
   - **Disadvantage:** More complex to calculate and implement.

8. **Longest Remaining Time First (LRTF) scheduling**
   - **Definition:** Processes with the longest remaining burst time are executed first.
   - **Advantage:** Can be useful in certain specialized scenarios where longer tasks are prioritized.
   - **Disadvantage:** Highly inefficient for general-purpose use and can lead to significant waiting times for shorter tasks.
