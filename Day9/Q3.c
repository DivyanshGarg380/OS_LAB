#include <stdio.h>

int main() {
    int base[5] = {1400, 6300, 4300, 3000, 4700};
    int limit[5] = {1000, 700, 400, 1300, 300};

    int segment, offset, physical;

    segment = 2;
    offset = 53;

    if(offset < limit[segment]) {
        physical = base[segment] + offset;
        printf("Segment %d, Offset %d -> Physical Address = %d\n", segment, offset, physical);
    } else {
        printf("Invalid address\n");
    }

    segment = 3;
    offset = 852;

    if(offset < limit[segment]){ 
        physical = base[segment] + offset;
        printf("Segment %d, Offset %d -> Physical Address = %d\n", segment, offset, physical);
    } else {
        printf("Invalid address\n");
    }

    segment = 0;
    offset = 1222;

    if(offset < limit[segment]) {
        physical = base[segment] + offset;
        printf("Segment %d, Offset %d -> Physical Address = %d\n", segment, offset, physical);
    } else {
        printf("Invalid address\n");
    }
    
    return 0;
}