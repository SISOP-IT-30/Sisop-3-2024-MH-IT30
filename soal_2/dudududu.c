#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

// Declare num1 and num2 globally
int num1 = 0;
int num2 = 0;

char *number_to_words[] = {"", "satu", "dua", "tiga", "empat", "lima", "enam", "tujuh", "delapan", "sembilan"};

void convert_to_words(int num, char *result) {
    if (num >= 1 && num <= 9) {
        strcpy(result, number_to_words[num]);
    } else if (num >= 10 && num <= 19) {
        switch(num) {
            case 10: strcpy(result, "sepuluh"); break;
            case 11: strcpy(result, "sebelas"); break;
            default: {
                strcpy(result, number_to_words[num % 10]);
                strcat(result, " belas");
                break;
            }
        }
    } else if (num >= 20 && num <= 99) {
        strcpy(result, number_to_words[num / 10]);
        strcat(result, " puluh");
        if (num % 10 != 0) {
            strcat(result, " ");
            strcat(result, number_to_words[num % 10]);
        }
    }
}

int words_to_number(char *word) {
    int num = 0;
    for (int i = 1; i <= 9; i++) {
        if (strcmp(word, number_to_words[i]) == 0) {
            num = i;
            break;
        }
    }
    return num;
}

void format_time(char *buffer) {
    time_t now;
    struct tm *info;
    time(&now);
    info = localtime(&now);
    strftime(buffer, 20, "%d/%m/%y %H:%M:%S", info);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <operation>\n", argv[0]);
        return 1;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { // Child process
        close(pipefd[1]); // Close write end of the pipe
        int result;
        read(pipefd[0], &result, sizeof(result)); // Read result from parent

        char words[100];
        convert_to_words(result, words);

        // Output
        printf("hasil %s ", argv[1]);
        if (strcmp(argv[1], "-kali") == 0) {
            printf("%s dan %s adalah %s.\n", number_to_words[num1], number_to_words[num2], words);
        } else if (strcmp(argv[1], "-tambah") == 0) {
            printf("%s dan %s adalah %s.\n", number_to_words[num1], number_to_words[num2], words);
        } else if (strcmp(argv[1], "-kurang") == 0) {
            printf("%s dan %s adalah %s.\n", number_to_words[num1], number_to_words[num2], words);
        } else if (strcmp(argv[1], "-bagi") == 0) {
            printf("%s dan %s adalah %s.\n", number_to_words[num1], number_to_words[num2], words);
        }

        // Logging
        FILE *log_file = fopen("histori.log", "a");
        if (log_file != NULL) {
            char timestamp[20];
            format_time(timestamp);
            fprintf(log_file, "[%s] ", timestamp);
            if (strcmp(argv[1], "-kali") == 0) {
                fprintf(log_file, "[perkalian] hasil %s %s dan %s adalah %s.\n", argv[1], number_to_words[num1], number_to_words[num2], words);
            } else if (strcmp(argv[1], "-tambah") == 0) {
                fprintf(log_file, "[penjumlahan] hasil %s %s dan %s adalah %s.\n", argv[1], number_to_words[num1], number_to_words[num2], words);
            } else if (strcmp(argv[1], "-kurang") == 0) {
                fprintf(log_file, "[pengurangan] hasil %s %s dan %s adalah %s.\n", argv[1], number_to_words[num1], number_to_words[num2], words);
            } else if (strcmp(argv[1], "-bagi") == 0) {
                fprintf(log_file, "[pembagian] hasil %s %s dan %s adalah %s.\n", argv[1], number_to_words[num1], number_to_words[num2], words);
            }
            fclose(log_file);
        } else {
            printf("Failed to open log file.\n");
        }

        close(pipefd[0]); // Close read end of the pipe
    } else { // Parent process
        close(pipefd[0]); // Close read end of the pipe
char input[20];
        printf("Masukkan dua angka (dalam kata): ");
        fgets(input, sizeof(input), stdin);
        
        // Remove trailing newline
        input[strcspn(input, "\n")] = 0;

        char *token = strtok(input, " ");
        num1 = words_to_number(token);

        token = strtok(NULL, " ");
        num2 = words_to_number(token);

        int result;
        if (strcmp(argv[1], "-kali") == 0) {
            result = num1 * num2;
        } else if (strcmp(argv[1], "-tambah") == 0) {
            result = num1 + num2;
        } else if (strcmp(argv[1], "-kurang") == 0) {
            result = num1 - num2;
            if (result < 0) {
                printf("ERROR\n");
                close(pipefd[1]);
                return 0;
            }
        } else if (strcmp(argv[1], "-bagi") == 0) {
            if (num2 == 0) {
                printf("ERROR: Division by zero\n");
                close(pipefd[1]);
                return 0;
            }
            result = num1 / num2;
        }

        // Send result to child process
        write(pipefd[1], &result, sizeof(result));

        close(pipefd[1]); // Close write end of the pipe
        wait(NULL); // Wait for child process to finish
    }

    return 0;
}
