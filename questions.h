#ifndef QUESTIONS_H_
#define QUESTIONS_H_

#include <stdbool.h>

#define MAX_LEN 256
#define NUM_CATEGORIES 3
#define NUM_QUESTIONS 12

// Structure to represent a question
typedef struct {
    char category[MAX_LEN];
    char question[MAX_LEN];
    char answer[MAX_LEN];
    int value;
    bool answered;
} question;

// List of 3 categories as an array of strings
static char categories[NUM_CATEGORIES][MAX_LEN] = {
    "programming",
    "algorithms",
    "databases"
};

// Array of 12 questions (4 for each category)
extern question questions[NUM_QUESTIONS];

// Function declarations
extern void initialize_game(void);
extern void display_categories(void);
extern void display_question_index(int index);
extern void display_question(char *category, int value);
extern bool valid_answer_index(int index, char* answer);
extern bool valid_answer(char *category, int value, char *answer);
extern bool already_answered(char *category, int value);
extern int get_question_index(char* category, int value);

#endif /* QUESTIONS_H_ */
