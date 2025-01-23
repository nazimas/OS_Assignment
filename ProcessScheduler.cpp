// Operating System Assignment : Process Scheduler
// Member 1: Muhammad Luqman Irfan bin Ahmad Kamal Peong
// Member 2: Wisyal Faridz Aimizil bin Mohd Fauzi
// Member 3: Ameer Harith Bin Mohd Hazali
// Member 4: Abdul Nasser Kolathumkara Muhammed Nazim
#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;
// Simple structure to hold all the data of the process.
struct Process
{
    int id;
    int priority;
    int burstTime;
    int remainingTime;
    int finishedTime;
    int waitingTime;
    int turnaroundTime;
    int arrivalTime;
};
vector<Process> getProcesses();
void printGanttChart(vector<pair<int, int>> ganttChart);
void printResults(std::vector<Process> processes);
void roundRobin(std::vector<Process> &processes, int quantum);
void nonshortestJobFirst(std::vector<Process> &processes);
void nonPreemptivePriority(std::vector<Process> &processes);
void shortestRemainingTime(std::vector<Process> &processes);
// put your function definition here here guys.
int main()
{
    cout << "Welcome to Process Scheduler simulator\n";
    while (true)
    {
        cout << endl;
        cout << "Choose a process scheduler\n 1.Round Robin\n 2.Non-Preemptive Priority\n 3.SRT\n 4.SJF\n 5.Exit\n";
        int x;
        cin >> x;
        if (x == 1)
        {
            vector<Process> processList = getProcesses();
            int quantum;
            cout << "Enter time quantum: ";
            cin >> quantum;
            roundRobin(processList, quantum);
        }
        else if (x == 2)
        {
            vector<Process> processList = getProcesses();
            nonPreemptivePriority(processList);
            // TODO
        }
        else if (x == 3)
        {
            vector<Process> processList = getProcesses();
            shortestRemainingTime(processList);
            // TODO
        }
        else if (x == 4)
        {
            vector<Process> processList = getProcesses();
            nonshortestJobFirst(processList);
            // TODO
        }
        else
        {
            cout << "\nExiting Program";
            break;
        }
    }
}
// Function to get the arrival times and burst times of n number of processes
vector<Process> getProcesses()
{
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;
    vector<Process> processes(n);
    for (int i = 0; i < n; ++i)
    {
        processes[i].id = i;
        cout << "Enter arrival time for Process " << i << ": ";
        cin >> processes[i].arrivalTime;
        cout << "Enter burst time for Process " << i << ": ";
        cin >> processes[i].burstTime;
        processes[i].remainingTime = processes[i].burstTime; // Initialize remaining time
        cout << "Enter priority for Process " << i << ": ";
        cin >> processes[i].priority;
        cout << endl;
    }
    return processes;
}
// Function to print out the gantt chart.
void printGanttChart(vector<pair<int, int>> ganttChart)
{
    cout << "\n";
    // Print the Gantt chart
    cout << "\nGantt Chart:\n";
    cout << "|";
    for (const auto &entry : ganttChart)
    {
        cout << " P" << entry.first << " |";
    }
    cout << "\n";
    int currentTime = 0;
    cout << currentTime;
    for (const auto &entry : ganttChart)
    {
        currentTime += entry.second;
        if (currentTime >= 10)
        {
            cout << "   " << currentTime;
        }
        else
        {
            cout << "    " << currentTime;
        }
    }
}
// Function to print out the result table along with the totals and averages.
void printResults(std::vector<Process> processes)
{
    cout << "\n";
    int n = processes.size();
    // Declared as doubles since they will be divided for the averages later.
    double totalTurnaroundTime = 0;
    double totalWaitingTime = 0;
    // Print the results and calculate the totals
    cout << "Process\t  Arrival Time\t   Burst Time     Finished Time  Turnaround Time   Waiting Time\n";
    for (const auto &process : processes)
    {
        cout << "P" << process.id << "\t\t" << process.arrivalTime << "\t\t"
             << process.burstTime << "\t\t" << process.finishedTime << "\t\t"
             << process.turnaroundTime << "\t\t" << process.waitingTime << "\n";
        totalTurnaroundTime += process.turnaroundTime;
        totalWaitingTime += process.waitingTime;
    }
    // Print totals and averages
    cout << "Total Turnaround Time   : " << totalTurnaroundTime << endl;
    cout << "Average Turnaround Time : " << totalTurnaroundTime / n << endl;
    cout << "Total Waiting Time      : " << totalWaitingTime << endl;
    cout << "Average Waiting Time    : " << totalWaitingTime / n << endl;
}
// Function that implements the calculations of the round robin algorithm
void roundRobin(std::vector<Process> &processes, int quantum)
{
    // Queue to keep track of which processes go next
    std::queue<int> readyQueue;
    int time = 0;
    int n = processes.size();
    int completed = 0;
    // Sort processes by arrival time
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b)
         {
        if (a.arrivalTime == b.arrivalTime) {
            return a.priority < b.priority; // Break ties by priority
        }
        return a.arrivalTime < b.arrivalTime; });
    // Vector to track if a process is added to the queue
    vector<bool> addedToQueue(n, false);
    // Vector to store the Gantt chart (process ID, execution time)
    vector<pair<int, int>> ganttChart;
    while (completed < n)
    {
        // Add processes to the ready queue that have arrived by the current time
        for (int i = 0; i < n; ++i)
        {
            if (!addedToQueue[i] && processes[i].arrivalTime <= time)
            {
                readyQueue.push(i);
                addedToQueue[i] = true;
            }
        }
        if (!readyQueue.empty())
        {
            int index = readyQueue.front();
            readyQueue.pop();
            // Execute the process for the time quantum or its remaining time
            int executionTime = std::min(quantum, processes[index].remainingTime);
            processes[index].remainingTime -= executionTime;
            ganttChart.emplace_back(processes[index].id, executionTime); // Add to Gantt chart
            time += executionTime;
            // Add processes to the queue that arrived during execution
            // Since this done before considering whether the current process should be re-add it will always priorities processes that have never ran before
            for (int i = 0; i < n; ++i)
            {
                if (!addedToQueue[i] && processes[i].arrivalTime <= time)
                {
                    readyQueue.push(i);
                    addedToQueue[i] = true;
                }
            }
            // If the process still has remaining time, re-add it to the queue
            if (processes[index].remainingTime > 0)
            {
                readyQueue.push(index);
            }
            else
            {
                // Process completed, get finished time, calculates turnaround time and waiting time.
                completed++;
                processes[index].finishedTime = time;
                processes[index].turnaroundTime = time - processes[index].arrivalTime;
                processes[index].waitingTime = processes[index].turnaroundTime - processes[index].burstTime;
                // Ensure no negative waiting time
                if (processes[index].waitingTime < 0)
                {
                    processes[index].waitingTime = 0;
                }
            }
        }
        else
        {
            // If no process is ready continue until a process is acquired.
            time++;
        }
    }
    // Printing gantt chart and results.
    printGanttChart(ganttChart);
    printResults(processes);
}

// Non-Preemptive Priority Scheduling
void nonPreemptivePriority(std::vector<Process> &processes)
{
    int time = 0;
    int n = processes.size(); // Stores the no. of processes
    int completed = 0;        // Tracks the amount of processes that have finished executing.
    std::vector<pair<int, int>> ganttChart;
    // Sort the processes based on arrival time
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b)
         { return a.arrivalTime < b.arrivalTime; });
    // This section iterates through the processes and selecs the one with the highest priority
    // highest priority in this case means smallest priority value.
    while (completed < n)
    {
        int selectedProcess = -1;
        for (int i = 0; i < n; ++i)
        {
            if (processes[i].arrivalTime <= time && processes[i].remainingTime > 0)
            {
                if (selectedProcess == -1 || processes[i].priority < processes[selectedProcess].priority)
                {
                    selectedProcess = i;
                }
            }
        }
        // We can execute completely since non-preemptive scheduling

        if (selectedProcess != -1)
        {
            time += processes[selectedProcess].burstTime; // incremented by the burst time of a process
            processes[selectedProcess].remainingTime = 0; // this indicates that a process is finished executing
            processes[selectedProcess].finishedTime = time;
            processes[selectedProcess].turnaroundTime = time - processes[selectedProcess].arrivalTime;
            processes[selectedProcess].waitingTime = processes[selectedProcess].turnaroundTime - processes[selectedProcess].burstTime;
            // Add to Gantt Chart
            ganttChart.emplace_back(processes[selectedProcess].id, processes[selectedProcess].burstTime);
            completed++;
        }
        else
        {
            // Increments time if no processes are available.
            time++;
        }
    }
    // Print Gantt chart and results
    printGanttChart(ganttChart);
    printResults(processes);
}
void shortestRemainingTime(std::vector<Process> &processes)
{
    int n = processes.size();
    // size
    int time = 0;
    // time
    int completed = 0;
    vector<pair<int, int>> ganttChart;
    // gantt chart example process name time
    while (completed < n) // if not completed
    {
        int shortestRemainingIndex = -1;
        int minRemainingTime = INT_MAX;
        for (int i = 0; i < n; ++i) // loop
        {
            if (processes[i].arrivalTime <= time && processes[i].remainingTime > 0)
            { // if arrival time is less than time and process remaining time more than 0
                if (processes[i].remainingTime < minRemainingTime)
                { // and if process remaining time is less than minimum remaining time
                    minRemainingTime = processes[i].remainingTime;
                    // minimum remaining time = process remaining time
                    shortestRemainingIndex = i;
                    // SRT index
                }
            }
        } // loop ends
        if (shortestRemainingIndex != -1) // if SRT index is not equal -!(initialised)
        {
            ganttChart.emplace_back(processes[shortestRemainingIndex].id, 1);
            processes[shortestRemainingIndex].remainingTime--;
            time++;
            if (processes[shortestRemainingIndex].remainingTime == 0)
            {
                completed++;
                processes[shortestRemainingIndex].finishedTime = time;
                processes[shortestRemainingIndex].turnaroundTime = time - processes[shortestRemainingIndex].arrivalTime;
                processes[shortestRemainingIndex].waitingTime = processes[shortestRemainingIndex].turnaroundTime - processes[shortestRemainingIndex].burstTime;
            }
        }
        else
        {
            time++;
        }
    }
    printGanttChart(ganttChart);
    printResults(processes);
}
void nonshortestJobFirst(std::vector<Process> &processes)
{
    int n = processes.size();
    int time = 0;
    int completed = 0;

    // Sort processes by arrival time
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b)
         { return a.arrivalTime < b.arrivalTime; });

    // Vector to store the Gantt chart (process ID, execution time)
    vector<pair<int, int>> ganttChart;

    while (completed < n)
    {
        // Find the process with the shortest burst time that has arrived
        int index = -1;
        int minBurstTime = INT_MAX;

        for (int i = 0; i < n; ++i)
        {
            if (processes[i].arrivalTime <= time && processes[i].remainingTime > 0)
            {
                if (processes[i].burstTime < minBurstTime)
                {
                    minBurstTime = processes[i].burstTime;
                    index = i;
                }
            }
        }

        if (index != -1)
        {
            // Execute the process
            ganttChart.emplace_back(processes[index].id, processes[index].burstTime);
            time += processes[index].burstTime;

            // Update process details
            processes[index].remainingTime = 0;
            processes[index].finishedTime = time;
            processes[index].turnaroundTime = processes[index].finishedTime - processes[index].arrivalTime;
            processes[index].waitingTime = processes[index].turnaroundTime - processes[index].burstTime;

            // Ensure no negative waiting time
            if (processes[index].waitingTime < 0)
            {
                processes[index].waitingTime = 0;
            }

            completed++;
        }
        else
        {
            // If no process is ready, increment time
            time++;
        }
    }

    // Printing gantt chart and results.
    printGanttChart(ganttChart);
    printResults(processes);
}
