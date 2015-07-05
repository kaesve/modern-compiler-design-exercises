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

int countAbcabc(char *data) {
    
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

    for(char *pos = data; *pos; ++pos) {
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
    for (int i = 1; i < argc; ++i) {
        long fileSize = readFileToBuffer(args[i], &source);
        printf("'%s' has %d matches\n", args[i], countAbcabc(source));
        free(source); 
    }
}