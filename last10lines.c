#include "last10lines.h"

#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define BUFSIZE 32
#define BUFSIZE 17

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
Считать последние n строк файла(по переводу каретки) и вернуть в виде 
динанического массива.
 */
char **read_last_n_lines(FILE *source, int n) {
    assert(source);

    char **lines = calloc(n, sizeof(*lines));
    int line_len = 0;
    int line_index = 0;

    int read_size = BUFSIZE - 1;
    char *tmp = NULL;
    fseek(source, 0, SEEK_END);
    int file_size = ftell(source);
    if (read_size > file_size) {
        read_size = file_size;
    }

    bool first = true;

    // длина выделененой строки
    int ch_num = 0;

    for(int i = 0; i < n; ++i) {
        char chunk[BUFSIZE] = {0};
        char *chunk_last = &chunk[BUFSIZE - 1];
        if (fseek(source, -read_size * (i + 1), SEEK_END) == -1) {
            break;
        }
        int read_bytes = fread(chunk, read_size, 1, source);
        if (read_bytes != read_size) {
            printf("some fread() error, %d != %d\n", read_bytes, read_size);
        }

        // удаление перевода последней строки в файле
        if (first && *(chunk_last - 1) == '\n') {
            first = false;
            chunk_last -= 2;
        }

        if (!lines[line_index]) {
            line_len = BUFSIZE;
            lines[line_index] = calloc(sizeof(char), line_len);
            tmp = lines[line_index];
        }

        while (chunk_last + 1 != chunk) {
            // ch - готовый символ, полученный из потока
            char ch = *chunk_last--;

            if (ch == 0)
                continue;

            ch_num++;

            // буфер закончился, увеличить размер буфера
            if (ch_num + 1 == line_len) {
                line_len *= 2;
                // длина записанной строки
                int offset = tmp - lines[line_index];
                printf("offset %d\n", offset);
                tmp = lines[line_index] = realloc(
                    lines[line_index], sizeof(char) * line_len
                );
                //memset(tmp + offset + 1, 0, line_len - offset);
                tmp += offset;
            }

            // новая строка, выделить новый буфер
            if (ch == '\n') {
                rotate(lines[line_index]);
                line_index++;
                line_len = BUFSIZE;
                tmp = lines[line_index] = calloc(sizeof(char), line_len);
                ch_num = 0;
            } else {
                // копирование символа в результирующую строку
                *tmp++ = ch;
            }

            //printf("%c", ch);
            //printf("tmp '%s'\n", lines[line_index]);
        }
       
        //printf("\n");
        //printf("line '%s'\n", line);
    }

    return lines;
}
