#include <stdio.h>

struct Task {
    int tid;
    int bt;
    int period;
    int remaining;
};

int gcd(int a,int b){
    while(b!=0){
        int t=b;
        b=a%b;
        a=t;
    }
    return a;
}

int lcm(int a,int b){
    return (a*b)/gcd(a,b);
}

int main(){

    int n;
    printf("Enter number of tasks: ");
    scanf("%d",&n);

    struct Task t[n];

    for(int i=0;i<n;i++){
        t[i].tid=i+1;

        printf("\nTask %d\n",i+1);

        printf("Execution Time: ");
        scanf("%d",&t[i].bt);

        printf("Period: ");
        scanf("%d",&t[i].period);

        t[i].remaining=0;
    }

    // Calculate Hyperperiod (LCM of periods)
    int hyper=t[0].period;
    for(int i=1;i<n;i++){
        hyper=lcm(hyper,t[i].period);
    }

    printf("\nHyperperiod = %d\n",hyper);
    printf("\nSchedule:\n");

    for(int time=0;time<hyper;time++){

        for(int i=0;i<n;i++){
            if(time % t[i].period == 0){
                if(t[i].remaining!=0){
                    printf("Deadline Miss T%d\n",t[i].tid);
                }
                t[i].remaining = t[i].bt;
            }
        }

        int idx=-1;
        int best=1e6;

        for(int i=0;i<n;i++){
            if(t[i].remaining>0 && t[i].period<best){
                best=t[i].period;
                idx=i;
            }
        }

        if(idx!=-1){
            printf("Time %d: T%d\n",time,t[idx].tid);
            t[idx].remaining--;
        }
        else{
            printf("Time %d: Idle\n",time);
        }
    }

    return 0;
}