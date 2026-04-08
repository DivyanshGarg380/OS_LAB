#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, frames;

    printf("Enter number of pages: ");
    scanf("%d", &n);

    int *pages = (int *)malloc(n * sizeof(int));

    printf("Enter page reference string:\n");
    for(int i = 0; i < n; i++) scanf("%d", &pages[i]);

    printf("Enter number of frames: ");
    scanf("%d", &frames);

    int *frame = (int *)malloc(frames * sizeof(int));
    int *time = (int *)malloc(frames * sizeof(int)); 

    for(int i = 0; i < frames; i++) {
        frame[i] = -1;
        time[i] = 0;
    }

    int faults = 0;
    int counter = 0;

    for(int i = 0; i < n; ++i) {
        int found = 0;
        for(int j = 0; j < frames; ++j) {
            if(frame[j] == pages[i]) {
                counter++;
                time[j] = counter;
                found = 1;
                break;
            }
        }

        if(!found) {
            int lru = 0;
            for(int j = 1; j < frames; ++j) {
                if(time[j] < time[lru]) lru = j;
            }

            frame[lru] = pages[i];
            counter++;
            time[lru] = counter;
            faults++;
        }
    }


    printf("Total page faults = %d\n", faults);
    printf("Hit Ratio = %.2f\n", (float)(n - faults) / n);
    return 0;
}