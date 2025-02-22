#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_BUFFER 1024
#define FILE_A_PATH "/sys/kernel/proj4figpat/rejA"
#define FILE_W_PATH "/sys/kernel/proj4figpat/rejW"
#define FILE_S_PATH "/sys/kernel/proj4figpat/rejS"

unsigned int read_from_file(const char *file_path);
int write_to_file(const char *file_path, unsigned int value);
void run_tests(void);
unsigned int calculate_prime(unsigned int input_value);

int main(void) {
    run_tests();
    return 0;
}

unsigned int read_from_file(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_BUFFER];
    if (fgets(buffer, MAX_BUFFER, file) == NULL) {
        perror("Error reading from file");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fclose(file);

    return strtoul(buffer, NULL, 8); // Use base 8 for octal numbers
}

int write_to_file(const char *file_path, unsigned int value) {
    FILE *file = fopen(file_path, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
        
    }

    if (fprintf(file, "%o", value) < 0) { // Use %o format for octal numbers
        perror("Error writing to file");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    fclose(file);
    return 0;
}

unsigned int calculate_prime(unsigned int input_value) {
    write_to_file(FILE_A_PATH, input_value);

    unsigned int status;
    unsigned int result;

    while (1) {
        status = read_from_file(FILE_S_PATH);
        if (status == 2) {
            result = read_from_file(FILE_W_PATH);
            return result;
        }
    }
    return result; // Never reached, but added for completeness
}

void run_tests(void) {
    unsigned int input_values[] =    {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 25, 50, 100, 250, 500, 750, 1000};
    unsigned int expected_values[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 97, 229, 541, 1583, 3571, 5693, 7919};


    for (int i = 0; i < sizeof(input_values) / sizeof(input_values[0]); i++) {
        unsigned int result = calculate_prime(input_values[i]);
        if (result == expected_values[i]) {
            printf("Test %d: Input=%u -> Result=%u (Octal: 0%o) [PASSED]\n", i + 1, input_values[i], result, result);
        } else {
            printf("Test %d: Input=%u -> Expected=%u, Got=%u [FAILED]\n", i + 1, input_values[i], expected_values[i], result);
        }
    }
    printf("All tests executed.\n");
}