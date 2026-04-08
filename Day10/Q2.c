#include <stdio.h>
#include <stdlib.h>

int findOptimal(int frame[], int frames, int pages[], int n, int idx) {
    int farthest = idx;
    int pos = -1;

    for(int i = 0; i < frames; ++i) {
        int j;
        for(j = idx; j < n; ++j) {
            if(frame[i] == pages[j]) {
                if(j > farthest) {
                    farthest = j;
                    pos = i;
                }
                break;
            }
        }

        if(j == n) return i;
    }

    return (pos == -1) ? 0 : pos;
}

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
    for(int i = 0; i < frames; i++) frame[i] = -1;

    int faults = 0;

    for(int i = 0; i < n; ++i) {
        int found = 0;
        for(int j = 0; j < frames; ++j) {
            if(frame[j] == pages[i]) {
                found = 1;
                break;
            }
        }

        if(!found) {
            int pos = findOptimal(frame, frames, pages, n, i + 1);
            frame[pos] = pages[i];
            faults++;
        }
    }
    
    printf("Total Page Faults (optimal) = %d\n", faults);
    return 0;
}