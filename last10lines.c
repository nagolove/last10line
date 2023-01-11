#include "last10lines.h"

#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 32
//#define BUFSIZE 32 * 1024 * 1024

char *rotate(char *s) {
    int len = strlen(s);
    for (int i = 0; i < len / 2; ++i) {
        char tmp = s[i];
        s[i] = s[len - i - 1];
        s[len - i - 1] = tmp;
    }
    return s;
}

/*
printf("%s\n", rotate(strdup("1")));
printf("%s\n", rotate(strdup("123")));
printf("%s\n", rotate(strdup("1234")));
printf("%s\n", rotate(strdup("")));
*/

/*
Считать последние n строк файла(по переводу каретки) и вернуть в виде 
динанического массива.
 */
char **read_last_n_lines(FILE *source, int n) {
    assert(source);

    char **lines = calloc(n, sizeof(*lines));
    int line_len = 0;
    int line_index = 0;

    int read_size = BUFSIZE - 1;
    fseek(source, 0, SEEK_END);
    int file_size = ftell(source);
    if (read_size > file_size) {
        read_size = file_size;
    }

    bool first = true;
    for(int i = 0; i < n; ++i) {
        //printf("i = %d\n", i);
        char chunk[BUFSIZE] = {0};
        char *chunk_last = &chunk[BUFSIZE - 1];
        if (fseek(source, -read_size * (i + 1), SEEK_END) == -1) {
            break;
        }
        fread(chunk, read_size, 1, source);

        if (first && *(chunk_last - 1) == '\n') {
            first = false;
            chunk_last -= 2;
        }

        if (!lines[line_index]) {
            line_len = BUFSIZE;
            lines[line_index] = calloc(sizeof(char), line_len);
            //strcpy(lines[line_index], "hello");
        }
        //char *line_last = &lines[line_index][line_len - 1];
        char *tmp = lines[line_index];

        int ch_num = 0;
        while (chunk_last + 1 != chunk) {
            char ch = *chunk_last--;
            ch_num++;

            *tmp = ch;
            //printf("%c", ch);
            printf("tmp '%s'\n", tmp);
        }
       
        //printf("\n");
        //printf("line '%s'\n", line);
    }

    return lines;
}
