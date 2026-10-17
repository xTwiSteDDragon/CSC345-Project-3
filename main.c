/* Zachary Kunkle and Arya Sekkapan */
/* CSC345-02 Project 3 */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

/* macros */
#define PHYS_MEM_SIZE 256 * 256
#define NUM_FRAMES 256
#define PAGE_SIZE 256  /* 2^8 bytes */
#define FRAME_SIZE 256 /* 2^8 bytes */
#define PAGE_TABLE_SIZE 256
#define TLB_SIZE 16

/* globals */
char physical_memory[NUM_FRAMES][FRAME_SIZE];
FILE *backing_store;
int g_page_table[PAGE_TABLE_SIZE][2]; /* array used as page table -- each entry points a page # to frame # */

typedef struct _TLB
{
    int page;
    int frame;
} TLB;

TLB sysTLB[TLB_SIZE];

int next_available = 0;

/* function prototypes */
void init_page_table(int table[256][2]);
void load_page_into_frame(int page_num);

int main(int argc, char **argv)
{
    backing_store = fopen("BACKING_STORE.bin", "r"); /* accessing simulated secondary storage */
    init_page_table(g_page_table);

    /* open addresses.txt for reading logical addresses */
    int filename = open("addresses.txt", O_RDONLY);
    if (filename < 0)
    {
        fprintf(stderr, "Error: addresses.txt could not be opened.");
        exit(1);
    }
    FILE *addresses = fdopen(filename, "r");
    char buffer[256];

    unsigned int addr;
    unsigned int page_num;
    unsigned int offset;

    /* page # and offset extraction and loading to memory */
    while ((fgets(buffer, sizeof(buffer), addresses)) != NULL)
    {
        addr = atoi(buffer);
        page_num = addr / PAGE_SIZE;
        offset = addr % PAGE_SIZE;

        //printf("Logical Address: %u\n", addr);
        //printf("Logical Page #: %u\n", page_num);
        //printf("Offset: %u\n", offset);

        /* if a page fault occurs, load page from BACKING_STORE.bin to RAM */
        if(g_page_table[page_num][1] == 0)
           load_page_into_frame(page_num);
        else if(g_page_table[page_num][1] == 1)
            /* change the below code to what needs to be done when a page is found in RAM */
            printf("Physical Frame #: %d\n", g_page_table[page_num][0]);
    }

    return 0;
}

/* initializes all page table valid/invalid bit to -1 to mark as invalid for page faults */
void init_page_table(int table[256][2])
{
    for (int i = 0; i < PAGE_TABLE_SIZE; ++i)
    {
        table[i][0] = -1;
        table[i][1] = 0;
    }
}

/* generated partially by AI to assist with loading to physical memory */
/* loads a virtual page onto a physical frame when */
void load_page_into_frame(int page_num)
{
    //printf("next available: %d\n", next_available);
    fseek(backing_store, page_num * PAGE_SIZE, SEEK_SET);
    fread(&physical_memory[next_available], sizeof(char), PAGE_SIZE, backing_store);
    g_page_table[page_num][0] = next_available; /* setting frame # in page table */
    g_page_table[page_num][1] = 1; /* setting valid bit in page table */
    next_available += 1;
}