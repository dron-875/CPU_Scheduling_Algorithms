#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <climits>

using namespace std;

struct Process {
    string name; int arrivalTime; int burstTime; int completionTime; 
    int waitingTime; int turnAroundTime; double normTurn; int remainingTime; // Used for Shortest Process Next algorithm
    int priority; // Used for Highest Priority algorithm
    bool isCompleted;
};

// Function to compare processes by arrival time
bool compareArrival(const Process& a, const Process& b) {
    return a.arrivalTime < b.arrivalTime;
}

// Function to perform First Come First Serve (FCFS) scheduling
void firstComeFirstServe(vector<Process>& processes) {
    int time = 0;
    for (auto& process : processes) {
        if (time < process.arrivalTime) {
            time = process.arrivalTime;
        }
        process.completionTime = time + process.burstTime; // CT = t+ BT
        process.turnAroundTime = process.completionTime - process.arrivalTime; // TAT = CT-AT
        process.normTurn = (double)process.turnAroundTime / process.burstTime;  // NT = TAT/BT
        process.waitingTime = process.turnAroundTime - process.burstTime;
        time = process.completionTime; // updating currTime as latest completioni time
    }
}

// Function to perform Round Robin (RR) scheduling


void roundRobin(vector<Process>& processes, int quantum) {
    queue<int> q; // Store process indices
    int currentTime = 0;
    int idx = 0;
    int remainingProcesses = processes.size();

    // Initialize remaining times
    for (auto& process : processes) {
        process.remainingTime = process.burstTime;
    }

    // Push all processes that have arrived by time 0
    while (idx < processes.size() && processes[idx].arrivalTime <= currentTime) {
        q.push(idx);
        idx++;
    }

    while (remainingProcesses > 0) {
        if (!q.empty()) {
            int processIdx = q.front();
            q.pop();

            int executeTime = min(quantum, processes[processIdx].remainingTime);
            currentTime += executeTime;
            processes[processIdx].remainingTime -= executeTime;

            if (processes[processIdx].remainingTime > 0) {
                // Push processes that have arrived during this quantum
                while (idx < processes.size() && processes[idx].arrivalTime <= currentTime) {
                    q.push(idx);
                    idx++;
                }
                // Re-push the current process as it needs more execution time
                q.push(processIdx);
            } else {
                processes[processIdx].completionTime = currentTime;
                processes[processIdx].turnAroundTime = currentTime - processes[processIdx].arrivalTime;
                processes[processIdx].waitingTime = processes[processIdx].turnAroundTime - processes[processIdx].burstTime;
                processes[processIdx].normTurn = (double)processes[processIdx].turnAroundTime / processes[processIdx].burstTime;
                remainingProcesses--;

                // Push processes that have arrived during this quantum
                while (idx < processes.size() && processes[idx].arrivalTime <= currentTime) {
                    q.push(idx);
                    idx++;
                }
            }
        } else {
            currentTime++;
            // Push processes that have arrived during the idle time
            while (idx < processes.size() && processes[idx].arrivalTime <= currentTime) {
                q.push(idx);
                idx++;
            }
        }
    }
}

// Function to perform Shortest Process Next (SPN) scheduling
void shortestProcessNext(vector<Process>& processes) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // Pair of burst time and process index
    int idx = 0;
    int currentTime = 0;

    while (idx < processes.size() || !pq.empty()) {
        while (idx < processes.size() && processes[idx].arrivalTime <= currentTime) {
            pq.push(make_pair(processes[idx].burstTime, idx));
            idx++;
        }

        if (!pq.empty()) {
            pair<int, int> top = pq.top();
            pq.pop();
            int burstTime = top.first;
            int processIdx = top.second;

            processes[processIdx].completionTime = currentTime + burstTime;
            processes[processIdx].turnAroundTime = processes[processIdx].completionTime - processes[processIdx].arrivalTime;
           
            processes[processIdx].normTurn = (double)processes[processIdx].turnAroundTime / processes[processIdx].burstTime;

            currentTime += burstTime;
            processes[processIdx].waitingTime=processes[processIdx].turnAroundTime - processes[processIdx].burstTime;
        } else {
            currentTime++;
        }
    }
}


// Function to perform Highest Priority scheduling
void highestPriority(vector<Process>& processes) {
    int currentTime = 0;
    int completedProcesses = 0;
    int n = processes.size();

    for (auto& process : processes) {
        process.isCompleted = false;
    }

    while (completedProcesses < n) {
        int idx = -1;
        int highestPriority = -1;

        // Find the highest priority process that has arrived and is not completed
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrivalTime <= currentTime && !processes[i].isCompleted) {
                if (processes[i].priority > highestPriority) {
                    highestPriority = processes[i].priority;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            currentTime += processes[idx].burstTime;
            processes[idx].completionTime = currentTime;
            processes[idx].turnAroundTime = processes[idx].completionTime - processes[idx].arrivalTime;
            processes[idx].waitingTime=processes[idx].turnAroundTime - processes[idx].burstTime;
            processes[idx].normTurn = (double)processes[idx].turnAroundTime / processes[idx].burstTime;
            processes[idx].isCompleted = true;
            completedProcesses++;
        } else {
            currentTime++;
        }
    }
}

// Function to print the statistics
void printStats(const vector<Process>& processes) {
    cout << "P_ID     |";
    for (const auto& process : processes) {
        cout << " " << process.name << " |";
    }
    cout << "\nAT       |";
    for (const auto& process : processes) {
        cout << "" << setw(3) << process.arrivalTime << " |";
    }
    cout << "\nBT       |";
    for (const auto& process : processes) {
        cout <<""<< setw(3) << process.burstTime << " |";
    }

    cout << "\nCT       |";
    for (const auto& process : processes) {
        cout << "" << setw(3) << process.completionTime << " |";
    }

    cout << "\nTAT      |";
    double totalTurnAround = 0;
    for (const auto& process : processes) {
        cout << "" << setw(3) << process.turnAroundTime << " |";
        totalTurnAround += process.turnAroundTime;
    }
    cout << "" << fixed << setprecision(2) << totalTurnAround / processes.size() << " |";

    cout << "\nWT       |";
    for (const auto& process : processes) {
        cout << "" << setw(3) << process.waitingTime << " |";
    }

    cout << "\nNT       |";
    double totalNormTurn = 0;
    for (const auto& process : processes) {
        cout << "" << setw(4) << fixed << setprecision(2) << process.normTurn << " |";
        totalNormTurn += process.normTurn;
    }
    cout << "" << fixed << setprecision(2) << totalNormTurn / processes.size() << " |\n";
    
}


// Function to print the timeline
void printTimeline(const vector<Process>& processes, int last_instant) {
    for (int i = 0; i <= last_instant; i++) {
        cout << i % 10 << " ";
    }
    cout << "\n------------------------------------------------\n";
    for (const auto& process : processes) {
        cout << process.name << "     |";
        for (int j = 0; j < last_instant; j++) {
            if (j >= process.arrivalTime && j < process.completionTime) {
                cout << "*|";
            } else {
                cout << " |";
            }
        }
        cout << "\n";
    }
    cout << "------------------------------------------------\n";
}

int main() {
    vector<Process> processes = {
        {"P1", 0, 4, 0, 0, 0, 0, 0, 2},    // Id,AT,BT,  CT,WT,TAT, NT,RT,priority,
        {"P2", 1, 3, 0, 0, 0, 0, 0, 3},
        {"P3", 2, 1, 0, 0, 0, 0, 0, 4},
        {"P4", 3, 5, 0, 0, 0, 0, 0, 5},
        {"P5", 4, 2, 0, 0, 0, 0, 0, 5},
       
    };

    int choice;
    cout << "Choose scheduling algorithm:\n";
    cout << "1. First Come First Serve (FCFS)\n";
    cout << "2. Round Robin (RR)\n";
    cout << "3. Shortest Jump First[non-p] (SPN)\n";
    cout << "4. Highest Priority\n";
    cout << "Enter your choice: ";
    cin >> choice;

    int quantum = 0;
    if (choice == 2) {
        cout << "Enter time quantum for Round Robin: ";
        cin >> quantum;
    }

    // Sort processes by arrival time
    sort(processes.begin(), processes.end(), compareArrival);

    // Perform scheduling based on user choice
    if (choice == 1) {
        firstComeFirstServe(processes);
    } else if (choice == 2) {
        roundRobin(processes, quantum);
    } else if (choice == 3) {
        shortestProcessNext(processes);
    } else if (choice == 4) {
        highestPriority(processes);
    } else {
        cout << "Invalid choice.\n";
        return 1;
    }

    // Print statistics
    printStats(processes);

    // Print timeline
    /*
    
    int last_instant = 0;
    for (const auto& process : processes) {
        last_instant = max(last_instant, process.completionTime);
    }
    printTimeline(processes, last_instant);

*/
    return 0;
}
