#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static bool read_line(char *buffer, size_t size) {
    if (fgets(buffer, (int)size, stdin) == NULL) {
        return false;
    }

    size_t length = 0;
    while (buffer[length] != '\0' && buffer[length] != '\n') {
        length++;
    }

    if (buffer[length] == '\n') {
        buffer[length] = '\0';
    } else {
        int character;
        while ((character = getchar()) != '\n' && character != EOF) {
            /* Discard the rest of an overlong input line. */
        }
    }

    return true;
}

static bool read_integer(const char *prompt, int minimum, int maximum,
                         int *result) {
    char line[128];

    for (;;) {
        char *end = NULL;
        long value;

        printf("%s", prompt);
        fflush(stdout);

        if (!read_line(line, sizeof(line))) {
            return false;
        }

        errno = 0;
        value = strtol(line, &end, 10);

        while (end != NULL && isspace((unsigned char)*end)) {
            end++;
        }

        if (line[0] == '\0' || end == line || *end != '\0' ||
            errno == ERANGE || value < INT_MIN || value > INT_MAX) {
            printf("Please enter a whole number.\n");
            continue;
        }

        if (value < minimum || value > maximum) {
            printf("Choose a number from %d to %d.\n", minimum, maximum);
            continue;
        }

        *result = (int)value;
        return true;
    }
}

static bool read_replay_choice(void) {
    char line[128];

    for (;;) {
        printf("Play again? (y/n): ");
        fflush(stdout);

        if (!read_line(line, sizeof(line))) {
            return false;
        }

        if (line[0] != '\0' && line[1] == '\0') {
            char choice = (char)tolower((unsigned char)line[0]);
            if (choice == 'y') {
                return true;
            }
            if (choice == 'n') {
                return false;
            }
        }

        printf("Enter y or n.\n");
    }
}

static void choose_difficulty(int *maximum, int *attempts) {
    int choice;

    printf("\nChoose a difficulty:\n");
    printf("  1. Easy   (1-20, 6 guesses)\n");
    printf("  2. Medium (1-50, 7 guesses)\n");
    printf("  3. Hard   (1-100, 8 guesses)\n");

    if (!read_integer("Your choice: ", 1, 3, &choice)) {
        *maximum = 0;
        return;
    }

    switch (choice) {
        case 1:
            *maximum = 20;
            *attempts = 6;
            break;
        case 2:
            *maximum = 50;
            *attempts = 7;
            break;
        default:
            *maximum = 100;
            *attempts = 8;
            break;
    }
}

static int play_round(void) {
    int maximum;
    int attempts;
    int score = 0;
    int guess;

    choose_difficulty(&maximum, &attempts);
    if (maximum == 0) {
        return -1;
    }

    int secret = rand() % maximum + 1;
    printf("\nI'm thinking of a number from 1 to %d.\n", maximum);
    printf("You have %d guesses.\n\n", attempts);

    for (int turn = 1; turn <= attempts; turn++) {
        char prompt[64];
        snprintf(prompt, sizeof(prompt), "Guess %d/%d: ", turn, attempts);

        if (!read_integer(prompt, 1, maximum, &guess)) {
            return -1;
        }

        if (guess == secret) {
            score = attempts - turn + 1;
            printf("Correct! You got it in %d %s.\n", turn,
                   turn == 1 ? "guess" : "guesses");
            break;
        }

        if (guess < secret) {
            printf("Too low.\n");
        } else {
            printf("Too high.\n");
        }
    }

    if (score == 0) {
        printf("Out of guesses! The number was %d.\n", secret);
    }

    return score;
}

int main(void) {
    int total_score = 0;
    int rounds = 0;

    srand((unsigned int)time(NULL));

    printf("================================\n");
    printf("      NUMBER GUESSING GAME\n");
    printf("================================\n");

    do {
        int score = play_round();
        if (score < 0) {
            printf("\nGame ended.\n");
            break;
        }

        rounds++;
        total_score += score;
        printf("Round points: %d | Total points: %d\n", score, total_score);
    } while (read_replay_choice());

    if (rounds > 0) {
        printf("\nThanks for playing! You scored %d points across %d %s.\n",
               total_score, rounds, rounds == 1 ? "round" : "rounds");
    }

    return 0;
}



