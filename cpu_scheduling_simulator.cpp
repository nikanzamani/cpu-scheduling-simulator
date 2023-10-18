#include <bits/stdc++.h>
using namespace std;
#define vi vector<int>
#define qi queue<int>
#define pi pair<int,int>
#define vvi vector<vi>


class process{
    public:
    vi cpu_bursts;
    vi io_bursts;
    int total_time;
    int arive_time;
    int burst_count;
    int end_time;
    bool visited = false;
    int first_visit_time;

};


vector<int> RR(vector<process> &processes, int tq);


// all CPU utilization
// all Throughput
// proc Turnaround Time
// proc Waiting Time
// proc Response Time


int main()
{
    int n;
    cin>>n;
    getchar();
    string line,token;
    vector<process> processes(n);
    
    for (int i = 0; i < n; i++){
        getline(cin,line);
        stringstream line_stream(line);
        int j=0;
        while (getline(line_stream,token,' '))
        {
            int t=stoi(token);
            if (j==0){
                processes[i].arive_time=t;
            }
            else if (j%2){
                processes[i].cpu_bursts.push_back(t);
                processes[i].burst_count++;
                processes[i].total_time+=t;
            }
            else{
                processes[i].io_bursts.push_back(t);
            }            
            j++;
        }
        
    }

    auto rr=RR(processes,2);

    int total_time=0,total_burst_count=0;
    vi tt,wt,rt;
    int min_tt=INT_MAX,max_tt=0,min_wt=INT_MAX,max_wt=0,min_rt=INT_MAX,max_rt=0;
    int total_tt=0,total_wt=0,total_rt=0;
    for (int i = 0; i < n; i++)
    {
        total_burst_count+=processes[i].burst_count;
        total_time+=processes[i].total_time;

        tt.push_back(processes[i].end_time-processes[i].arive_time);
        min_tt=min(min_tt,tt[i]);
        max_tt=max(max_tt,tt[i]);
        total_tt+=tt[i];

        wt.push_back(tt[i]-processes[i].total_time);
        min_wt=min(min_wt,wt[i]);
        max_wt=max(max_wt,wt[i]);
        total_wt+=wt[i];

        rt.push_back(processes[i].first_visit_time-processes[i].arive_time);
        min_rt=min(min_rt,rt[i]);
        max_rt=max(max_rt,rt[i]);
        total_rt+=rt[i];

    }
    

    cout<< "Simulation time: "<<rr[0]-rr[1]<<endl;
    cout<< "CPU utilization: "<< total_time*100/(rr[0]-rr[1])<<endl;
    cout<< "CPU Throughput (per 1000 unit time): "<< total_burst_count*1000/(rr[0]-rr[1])<<endl;
    cout<< "Turnaround Time (min/max/avg): "<< min_tt<<" "<< max_tt<<" "<< (double)total_tt/n<<endl;
    cout<< "Waiting Time (min/max/avg): "<< min_wt<<" "<< max_wt<<" "<< (double)total_wt/n<<endl;
    cout<< "Response Time (min/max/avg): "<< min_rt<<" "<< max_rt<<" "<< (double)total_rt/n<<endl;

    return 0;
}

vector<int> RR(vector<process> &processes, int tq){
    int time=0,n=processes.size(),sim_start=INT_MAX;
    vi indices(n,0),burset_used(n,0);
    queue<int> cpu;
    priority_queue<pi,vector<pi>,greater<pi> > proc_handeler;
    for (int i = 0; i < n; i++){
        proc_handeler.push({processes[i].arive_time,i});
        processes[i].visited=false;
        sim_start=min(sim_start,processes[i].arive_time);
    }

    while (!proc_handeler.empty() || !cpu.empty())
    {
        if (cpu.empty()){
            time=proc_handeler.top().first;
            cpu.push(proc_handeler.top().second);
            proc_handeler.pop();
        }
        auto p= cpu.front();cpu.pop();
        if (!processes[p].visited)
        {
            processes[p].visited=true;
            processes[p].first_visit_time=time;
        }
        // cout<<p<<" "<<time<<" "<<burset_used[p]<<endl;
        time+=min(tq,processes[p].cpu_bursts[indices[p]]-burset_used[p]);
        burset_used[p]+=tq;

        
        while (!proc_handeler.empty() && proc_handeler.top().first<=time){
            cpu.push(proc_handeler.top().second);
            proc_handeler.pop();
        }
        if (burset_used[p]>=processes[p].cpu_bursts[indices[p]]){
            burset_used[p]=0;
            if (indices[p]+1<processes[p].burst_count){
                proc_handeler.push({time+processes[p].io_bursts[indices[p]],p});
                indices[p]+=1;
            }
            else{
                processes[p].end_time=time;
            }
            
        }
        else{cpu.push(p);}
    }
    vi ans={time,sim_start};
    return ans;
};


void initialize(){


}
