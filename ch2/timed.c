/**
 * Exercise 2.1 asks to implement a program that queries the filesize of a 
 * file, allocates memory for it, loads the file into memory and counts the
 * occurences of the string 'abcabc'. Matches may overlap, so 'abcabcabc' has
 * 2 matches. The program should be as fast as possible.
 * 
 * To count the occurences I built a state machine that may transition into 
 * the next state with inputs a, b or c. Any other input will transition to
 * state 0. The state machine deals well with overlaps and it runs in Θ(n) 
 * time. To further speed the program up, calculating the next transition from
 * a state and an input is done without branching.
 *
 * The program does make a few assumptions: 
 *   - Characters are sizeof(char) bits. It probably breaks under encodings
 *     that use more space and espicially variable width.
 *   - The computer it runs on represents negative integers as two's complement
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// call this function to start a nanosecond-resolution timer
void timer_start(struct timespec *start_time){
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, start_time);
}

// call this function to end a timer, returning nanoseconds elapsed as a long
long timer_end(struct timespec start_time){
    struct timespec end_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
    long diffInNanos = end_time.tv_nsec - start_time.tv_nsec;
    return diffInNanos;
}

// file handling based on http://stackoverflow.com/questions/2029103/correct-way-to-read-a-text-file-into-a-buffer-in-c
long getFileSize(FILE *fp) {
    /* Go to the end of the file. */
    if (fseek(fp, 0L, SEEK_END) != 0) 
        return -1; // Error

    /* Get the size of the file. */
    long bufsize = ftell(fp);
    if (bufsize == -1) 
        return -1; // Error

    /* Go back to the start of the file. */
    if (fseek(fp, 0L, SEEK_SET) != 0) 
        return -1; // Error

    return bufsize;
}

long readFileToBuffer(char *fileName, char **buffer) {
    FILE *fp = fopen(fileName, "r");
    if (fp) {
        long bufsize = getFileSize(fp);
        if (bufsize == -1) 
            return -1; // Error

        /* Allocate our buffer to that size. */
        *buffer = malloc(sizeof(char) * (bufsize + 1));

        /* Read the entire file into memory. */
        size_t newLen = fread(*buffer, sizeof(char), bufsize, fp);
        if (newLen == 0)
            return -1; // Error

        (*buffer)[++newLen] = '\0'; /* Just to be safe. */
        fclose(fp);
    } else {
        return -1; // Error
    }
}

char match[] = "abcabc";
int mytherinsCountAbcabc(char *string)
{
    int state = 0;
    int count = 0;
    char *pos;
    for(pos = string; *pos; pos++)
    {
        state = state + (match[state] == *pos) - (match[state] != *pos) * state + (match[state] != *pos) * (match[0] == *pos);
        count = count + state / 6;
        state = state - state / 6 * 3;
    }
    return count;
}

int kaesvesCountAbcabc(char *data) {
    
    int states[6][4] = {
    /*     *, a, b, c */
    /*0*/{ 0, 1, 0, 0 },
    /*1*/{ 0, 1, 2, 0 },
    /*2*/{ 0, 1, 0, 3 },
    /*3*/{ 0, 4, 0, 0 },
    /*4*/{ 0, 1, 5, 0 },
    /*5*/{ 0, 1, 0, 3 }
    };

    int currentState = 0;
    int abcabcCount = 0;

    char *pos;
    for(pos = data; *pos; ++pos) {
        // If input is a, b or c, noTransition is 0, otherwise it is 1
        int noTransition = *pos < 'a' | *pos > 'c';

        // Assuming two's complement, -1 is 0xFF FF FF and 0 is 0x00 00 00. 
        // This results in the index being masked to 0 if the input is not a,
        // b or c. If it is, the resulting index will be 1, 2 or 3. 
        int transIndex = (noTransition - 1) & (*pos - 'a' + 1);
        int transition = states[currentState][transIndex];

        // If we are in the last state (5) and matched the right character
        // (transition is 3), we had a match and increase the count.
        abcabcCount += transition + currentState == 8;
        currentState = transition;
    }
    return abcabcCount;
}

int main(int argc, char ** args) {
    char *source;
    int count;
    long elapsed;
    struct timespec timer;
    int i;
    for (i = 1; i < argc; ++i) {
        timer_start(&timer);
        long filesize = readFileToBuffer(args[i], &source);
        elapsed = timer_end(timer);
        printf("%lu bytes of file '%s' loaded in %.2fms\n", filesize, args[i], elapsed/1000000.0);

        timer_start(&timer);
        count = kaesvesCountAbcabc(source);
        elapsed = timer_end(timer);
        printf("kaesve reports %d matches in %.2fms\n", count, elapsed/1000000.0);
        
        timer_start(&timer);
        count = mytherinsCountAbcabc(source);
        elapsed = timer_end(timer);
        printf("mytherin reports %d matches in %.2fms\n", count, elapsed/1000000.0);

        printf("\n");
        free(source); 
    }
}