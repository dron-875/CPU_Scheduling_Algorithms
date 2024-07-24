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
    int processno;
    int burstTimeBackup;
};

// Function to compare processes by arrival time
bool compareArrival(const Process& a, const Process& b) {
    return a.arrivalTime < b.arrivalTime;
}

///////////////////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////////////////

// Function to perform Round Robin (RR) scheduling
void roundRobin(vector<Process>& processes, int slice) {
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

            int executeTime = min(slice, processes[processIdx].remainingTime);
            currentTime += executeTime;
            processes[processIdx].remainingTime -= executeTime;

            if (processes[processIdx].remainingTime > 0) {
                // Push processes that have arrived during this slice
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

                // Push processes that have arrived during this slice
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

///////////////////////////////////////////////////////////////////////////////////////////

// Function to perform Shortest Jump First (SJF) scheduling
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

///////////////////////////////////////////////////////////////////////////////////////////

// Function to perform Shortest Remaining Time First (SRTF) scheduling
void shortestRemainingTimeFirst(vector<Process>& processes) {
    auto compare = [](const pair<int, int>& a, const pair<int, int>& b) {
        return a.first > b.first; // Min-heap based on remaining time
    };

    priority_queue<pair<int, int>, vector<pair<int, int>>, decltype(compare)> pq(compare);
    int currentTime = 0;
    int completedProcesses = 0;
    int n = processes.size();

    for (auto& process : processes) {
        process.remainingTime = process.burstTime;
    }

    while (completedProcesses < n) {
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrivalTime == currentTime) {
                pq.push(make_pair(processes[i].remainingTime, i));
            }
        }

        if (!pq.empty()) {
            int processIdx = pq.top().second;
            pq.pop();
            processes[processIdx].remainingTime -= 1;
            currentTime++;

            if (processes[processIdx].remainingTime == 0) {
                processes[processIdx].completionTime = currentTime;
                processes[processIdx].turnAroundTime = processes[processIdx].completionTime - processes[processIdx].arrivalTime;
                processes[processIdx].waitingTime = processes[processIdx].turnAroundTime - processes[processIdx].burstTime;
                processes[processIdx].normTurn = (double)processes[processIdx].turnAroundTime / processes[processIdx].burstTime;
                completedProcesses++;
            } else {
                pq.push(make_pair(processes[processIdx].remainingTime, processIdx));
            }
        } else {
            currentTime++;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////

// Function to perform Highest Priority (HP) scheduling 
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

///////////////////////////////////////////////////////////////////////////////////////////

// Function to perform Highest Priority (Preemptive) scheduling
void preemptiveHighestPriority(vector<Process>& processes) {
    int currentTime = 0;
    int completedProcesses = 0;
    int n = processes.size();

    for (auto& process : processes) {
        process.remainingTime = process.burstTime;
        process.isCompleted = false;
    }

    while (completedProcesses < n) {
        int idx = -1;
        int highestPriority = -1;

        // Find the highest priority process that has arrived and is not completed
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrivalTime <= currentTime && !processes[i].isCompleted) {
                if (processes[i].priority > highestPriority || 
                   (processes[i].priority == highestPriority && processes[i].arrivalTime < processes[idx].arrivalTime)) {
                    highestPriority = processes[i].priority;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            processes[idx].remainingTime -= 1;
            currentTime++;

            if (processes[idx].remainingTime == 0) {
                processes[idx].completionTime = currentTime;
                processes[idx].turnAroundTime = processes[idx].completionTime - processes[idx].arrivalTime;
                processes[idx].waitingTime = processes[idx].turnAroundTime - processes[idx].burstTime;
                processes[idx].normTurn = (double)processes[idx].turnAroundTime / processes[idx].burstTime;
                processes[idx].isCompleted = true;
                completedProcesses++;
            }
        } else {
            currentTime++;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////

// Function to perform Highest Response Ratio Next (HRRN) scheduling
void highestResponseRatioNext(vector<Process>& processes) {
    int currentTime = 0;
    int completedProcesses = 0;
    int n = processes.size();

    for (auto& process : processes) {
        process.isCompleted = false;
    }

    while (completedProcesses < n) {
        int idx = -1;
        double highestResponseRatio = -1.0;

        // Find the process with the highest response ratio that has arrived and is not completed
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrivalTime <= currentTime && !processes[i].isCompleted) {
                int waitingTime = currentTime - processes[i].arrivalTime;
                double responseRatio = (double)(waitingTime + processes[i].burstTime) / processes[i].burstTime;

                if (responseRatio > highestResponseRatio) {
                    highestResponseRatio = responseRatio;
                    idx = i;
                }
            }
        }

        if (idx != -1) {
            currentTime += processes[idx].burstTime;
            processes[idx].completionTime = currentTime;
            processes[idx].turnAroundTime = processes[idx].completionTime - processes[idx].arrivalTime;
            processes[idx].waitingTime = processes[idx].turnAroundTime - processes[idx].burstTime;
            processes[idx].normTurn = (double)processes[idx].turnAroundTime / processes[idx].burstTime;
            processes[idx].isCompleted = true;
            completedProcesses++;
        } else {
            currentTime++;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////

// Function to perform Longest Remaining Time First(LRTF) scheduling 
/* 
Function to find the process with the (largest remaining time) among all available processes at the current time
*/
int findLargestRemainingTime(const vector<Process>& processes, int currentTime) {
    int maxIndex = -1;
    int maxRemainingTime = -1;

    for (int i = 0; i < processes.size(); i++) {
        if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > maxRemainingTime && !processes[i].isCompleted) {
            maxRemainingTime = processes[i].remainingTime;
            maxIndex = i;
        }
    }
    
    return maxIndex;
}

void longestRemainingTimeFirst(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), compareArrival);

    int currentTime = 0;
    int totalProcesses = processes.size();
    int completedProcesses = 0;
    int prefinalTotal = 0;

    // Initialize remaining times and calculate the total burst time (prefinalTotal)
    for (auto& process : processes) {
        process.remainingTime = process.burstTime;
        process.burstTimeBackup = process.burstTime;
        process.isCompleted = false;
        prefinalTotal += process.burstTime;
    }

    while (completedProcesses < totalProcesses) {
        int index = findLargestRemainingTime(processes, currentTime);

        if (index != -1) {
           
            processes[index].remainingTime -= 1;
            currentTime++;

            if (processes[index].remainingTime == 0) {
                processes[index].completionTime = currentTime;
                processes[index].turnAroundTime = processes[index].completionTime - processes[index].arrivalTime;
                processes[index].waitingTime = processes[index].turnAroundTime - processes[index].burstTimeBackup;
                processes[index].normTurn = (double)processes[index].turnAroundTime / processes[index].burstTimeBackup;
                processes[index].isCompleted = true;
                completedProcesses++;
                
            }
            cout << endl;
        } else {
            currentTime++;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

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
        {"P1", 0, 2, 0, 0, 0, 0, 0, 0, 1, 0},    // Id,AT,BT,  CT,WT,TAT, NT,RT,priority, P_No,burstTimeBackup
        {"P2", 0, 3, 0, 0, 0, 0, 0, 0, 2, 0},
        {"P3", 2, 2, 0, 0, 0, 0, 0, 0, 3, 0},
        {"P4", 3, 5, 0, 0, 0, 0, 0, 0, 4, 0},
        {"P5", 4, 4, 0, 0, 0, 0, 0, 0, 5, 0},    
    
    };

    int choice;
    cout << "Choose scheduling algorithm:\n";
    cout << "1. First Come First Serve (FCFS)\n";
    cout << "2. Round Robin (RR)\n";
    cout << "3. Shortest Jump First[non-p] (SPN)\n";
    cout << "4. Shortest Remaining Time First[p] (SRTF)\n";
    cout << "5. Highest Priority[non-p]\n";
    cout << "6. Highest Priority[p]\n";
    cout << "7. Highest Response Ratio Next (HRRN)\n";
    cout << "8. Longest Remaining Time First(LRTF) [p]\n";
    cout << "Enter your choice: ";
    cin >> choice;

    int slice = 0;
    if (choice == 2) {
        cout << "Enter time slice for Round Robin: ";
        cin >> slice;
    }

    // Sort processes by arrival time
    sort(processes.begin(), processes.end(), compareArrival);

    // Perform scheduling based on user choice
    if (choice == 1) {
        firstComeFirstServe(processes);
    } else if (choice == 2) {
        roundRobin(processes, slice);
    } else if (choice == 3) {
        shortestProcessNext(processes);
    } else if (choice == 4) {
        shortestRemainingTimeFirst(processes);
    } else if (choice == 5) {
        highestPriority(processes);
    } else if (choice == 6) {
        preemptiveHighestPriority(processes);
    } else if (choice == 7) {
        highestResponseRatioNext(processes);
    } else if (choice == 8) {
        longestRemainingTimeFirst(processes);
    }

    else {
        cout << "Invalid choice.\n";
        return 1;
    }

    // Print statistics
    printStats(processes);

    // Print timeline
    
    
    int last_instant = 0;
    for (const auto& process : processes) {
        last_instant = max(last_instant, process.completionTime);
    }
    printTimeline(processes, last_instant);


    return 0;
}


