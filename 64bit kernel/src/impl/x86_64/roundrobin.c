/*
// Calculate turnaround time
int turnaroundtime(int processes[], int n, int bt[], int wt[], int tat[]){
    for (int i = 0; i < n; i++){
        tat[i] = bt[i] + wt[i];
    }
    return 1;
}

//Find the waiting time for all processes
int waitingtime(int processes[], int n, int bt[], int wt[], int quantum){
    int rem_bt[n];
    for (int i = 0; i < n; i++){
        rem_bt[i] = bt[i];
    }
    int t = 0;

    while (1) {
        bool done = true;
        for (int i = 0; i < n; i++){
            if (rem_bt[i] > quantum) {
                t += quantum;
                rem_bt[i] -= quantum;
            } else {
                t = t + rem_bt[i];
                wt[i] = t - bt[i];
                rem_bt[i] = 0;
            }
        }
        if (done == true) break;
    }
    return 1;
}

int findavgTime(int processes[], int n, int bt[], int quantum){
    int wt[n], tat[n], total_wt, total_tat = 0;
    waitingtime(processes, n, bt, wt, quantum);
    turnaroundtime(processes, n, bt, wt, tat);
    // Code to print and calculate time
    for (int i=0; i<n; i++){
        total_wt = total_wt + wt[i];
        total_tat = total_tat + tat[i];
    }
    return 1;
}*/