#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include"headers/linker.h"

static int read_elf(const char *filename, uint64_t bufaddr) {
    FILE *fp;
    fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("open file error");
        exit(1);
    }

    // read text file line by line

    char line[MAX_ELF_FILE_WIDTH];
    int line_counter = 0;

    while (fgets(line, MAX_ELF_FILE_WIDTH, fp) != NULL) {
        int len = strlen(line);
        if ((len == 0) ||
            (len >= 1 && (line[0] == '\n' || line[0] == '\r')) ||
            (len >= 2 && (line[0] == '/' && line[1] == '/'))) {
            continue;
        }

        // check if is empty or white line
        int iswhite = 1;
        for (int i = 0; i < len; ++i) {
            iswhite = iswhite && (line[i] == ' ' || line[i] == '\t' || line[i] == '\r');
        }
        if (iswhite == 1) {
            continue;
        }

        // to this line, this line is not white and contains information
        if (line_counter < MAX_ELF_FILE_LENGTH) {
            // store this line to buffer[line_counter]
            uint64_t addr = bufaddr + line_counter * MAX_ELF_FILE_WIDTH * sizeof(char);
            char *linebuf = (char *) addr;

            int i = 0;
            while (i < len && i < MAX_ELF_FILE_WIDTH) {
                if ((line[i] == '\n') ||
                    (line[i] == '\r') ||
                    ((i + 1 < len) && (i + 1 < MAX_ELF_FILE_WIDTH) && line[i] == '/' && line[i + 1] == '/')) {
                    break;
                }
                linebuf[i] = line[i];
                i++;
            }
            linebuf[i] = '\0';
            line_counter++;
        } else {
            printf("elf file %s is too long (>%d)\n", filename, MAX_ELF_FILE_LENGTH);
            fclose(fp);
            exit(1);
        }
    }

    fclose(fp);
    return line_counter;
}


int main() {
    char buffer[MAX_ELF_FILE_LENGTH][MAX_ELF_FILE_WIDTH];
    int count = read_elf("..\\files\\sum.elf.txt", (uint64_t) &buffer);

    for (int i = 0; i < count; ++i) {
        printf("%s\n", buffer[i]);
    }
}