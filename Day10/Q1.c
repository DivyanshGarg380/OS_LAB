#include <stdio.h>
#include <stdlib.h>

int main() {
    int n, frames;

    printf("Enter number of pages: ");
    scanf("%d", &n);

    int *pages = (int *)malloc(n * sizeof(int));
    printf("Enter page reference string:\n");
    for(int i = 0; i <n; ++i) scanf("%d", &pages[i]);

    printf("Enter number of frames: ");
    scanf("%d", &frames);

    int *frame = (int *)malloc(frames * sizeof(int));
    for(int i = 0; i < frames; ++i) frame[i] = -1;
    
    int faults = 0;
    int idx = 0;
    for(int i = 0; i < n; ++i) {
        int flag = 0;
        for(int j = 0; j < frames; ++j) {
            if(frame[j] == pages[i]) {
                flag = 1;
                break;
            }
        }

        if(!flag) {
            frame[idx] = pages[i];
            idx = (idx + 1) % frames;
            faults++;
        }
    }

    printf("Total Page faults (FIFO) = %d\n", faults);
    return 0;
}