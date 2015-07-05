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
        size_t newLen = fread(buffer, sizeof(char), bufsize, fp);
        if (newLen == 0)
            return -1; // Error

        buffer[++newLen] = '\0'; /* Just to be safe. */
        fclose(fp);
    } else {
        return -1;
    }
}

int countAbcabc(char *data) {
    
    int states[7][3] = {
         /*a, b, c*/   
    /*0*/{ 1, 0, 0 },
    /*1*/{ 1, 2, 0 },
    /*2*/{ 1, 0, 3 },
    /*3*/{ 4, 0, 0 },
    /*4*/{ 1, 5, 0 },
    /*5*/{ 1, 0, 3 }
    };

    int currentState = 0;
    int abcabcCount = 0;

    char *foo = malloc(sizeof(char) * 16);

    printf("foo: %d\n", foo[-200]);

    for(char *pos = data; *pos; ++pos) {
        int transIndex = *pos - 'a';
        // if character is not a, b or c, noTransition = 1, otherwise it is 0
        int noTransition = *pos < 'a' | *pos > 'c';

        // int transition = noTransition ? 0 : states[currentState][transIndex];
        // the following code assumes two's complement and is a branchless version of the
        // code above.

        // noTransition - 1 is 0xFF FF FF if noTransition was 0, otherwise it is 0x00 00 00
        int transition = (noTransition - 1) & states[currentState][transIndex];

        printf("char: %c, state: %d, t: %d, d: %d\n", *pos, currentState, transition, transition + currentState == 8);

        abcabcCount += transition + currentState == 8;
        currentState = transition;
    }
    return abcabcCount;
}

int main(int argc, char ** args) {
    //char *source;
    //long fileSize = readFileToBuffer(args[0], &source);

    while (argc --> 1)
        printf("count: %d\n\n", countAbcabc(args[argc]));


    //free(source); /* Don't forget to call free() later! */
}