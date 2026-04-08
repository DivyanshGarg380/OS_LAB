#include <stdio.h>

int main() {
    int pageSize = 32;
    int address, page, offset;

    address = 204; // first example given in Q
    page = address / pageSize;
    offset = address % pageSize;

    printf("Address: %d -> Page: %d, Offset: %d\n", address, page, offset);

    address = 56; // second exaple given in Q
    page = address / pageSize;
    offset = address % pageSize;
    printf("Address: %d -> Page: %d, Offset: %d\n", address, page, offset);

    printf("\nBytes needed to represent address = 8 bytes\n");
    return 0;
}