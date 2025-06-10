#include <stdio.h>
#include <stdlib.h>

void bubble_sort(int num_numbers, int numbers[]) {
    for (int i = 0; i < num_numbers - 1; ++i) {
        int s = 0;

        for (int j = 0; j < num_numbers - i - 1; ++j) {
            if (numbers[j] > numbers[j + 1]) {
                int temp = numbers[j];
                numbers[j] = numbers[j + 1];
                numbers[j+1] = temp;
                s = 1;
            }
        }

        if (s == 0) {
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <list of numbers>\n", argv[0]);
        return 1;
    }

    int num_numbers = argc - 1;
    int numbers[num_numbers];

    for (int i = 0; i < num_numbers; ++i) {
        numbers[i] = atoi(argv[i + 1]);
    }

    printf("Input numbers:\n");
    for (int i = 0; i < num_numbers; ++i) {
        printf("%d%s", numbers[i], i == num_numbers - 1 ? "\n" : ", ");
    }

    bubble_sort(num_numbers, numbers);

    printf("Output numbers:\n");
    for (int i = 0; i < num_numbers; ++i) {
        printf("%d%s", numbers[i], i == num_numbers - 1 ? "\n" : ", ");
    }

    return 0;
}
