#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "questions.h"

question questions[NUM_QUESTIONS];
// Initialize questions for the game
void initialize_game(void) {
    questions[0] = (question){"programming", "What is the most commonly used programming language for developing Android applications?", "Java", 100, false};
        questions[1] = (question){"programming", "What is the process of breaking a problem into smaller subproblems to solve it called?", "Decomposition", 200, false};
        questions[2] = (question){"programming", "What is the term for a variable that is accessible only within the function where it is declared?", "Local variable", 300, false};
        questions[3] = (question){"programming", "What is the name of the programming paradigm where functions are treated as first-class citizens?", "Functional programming", 400, false};

        // Algorithms questions
        questions[4] = (question){"algorithms", "What is the time complexity of the quicksort algorithm in the worst-case scenario?", "O(n^2)", 100, false};
        questions[5] = (question){"algorithms", "What is the term for the process of selecting the 'best' element from a data structure?", "Selection", 200, false};
        questions[6] = (question){"algorithms", "What is the name of the algorithm used to find the strongly connected components in a directed graph?", "Tarjan's algorithm", 300, false};
        questions[7] = (question){"algorithms", "What is the computational complexity class that represents decision problems that can be solved by a nondeterministic Turing machine in polynomial time?", "NP", 400, false};

        // Databases questions
        questions[8] = (question){"databases", "What is the SQL command used to add new rows to a table?", "INSERT", 100, false};
        questions[9] = (question){"databases", "What is the process of optimizing a database design by reducing redundancy and ensuring data integrity called?", "Normalization", 200, false};
        questions[10] = (question){"databases", "What is the term for a set of rules that determines the relationships between database entities?", "Database schema", 300, false};
        questions[11] = (question){"databases", "What is the SQL command used to retrieve a single row from a database table?", "SELECT ... LIMIT 1", 400, false};
}

// Display remaining categories and question dollar values
void display_categories(void) {
    printf("These are the list of questions left to answer:\n");

    for (int i = 0; i < NUM_CATEGORIES; ++i) {
        printf("%d. %s\n", (i + 1), categories[i]);

        for (int j = 0; j < NUM_QUESTIONS; ++j) {
            if (strcmp(questions[j].category, categories[i]) == 0 && !questions[j].answered) {
                printf("For $%d, %s\n", questions[j].value, questions[j].question);
            }
        }

        printf("\n");
    }
}

// Display question for the given index
void display_question_index(int index) {
    printf("For $%d!\n%s\n", questions[index].value, questions[index].question);
}

// Display question for the given category and value
void display_question(char *category, int value) {
    for (int i = 0; i < NUM_QUESTIONS; ++i) {
        if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
            display_question_index(i);
            return;
        }
    }
}

// Check if the answer is correct for the question at the given index
bool valid_answer_index(int index, char* answer) {
    return strcmp(questions[index].answer, answer) == 0;
}

// Check if the answer is correct for the question in the given category and value
bool valid_answer(char *category, int value, char *answer) {
    for (int i = 0; i < NUM_QUESTIONS; ++i) {
        if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
            return valid_answer_index(i, answer);
        }
    }
    return false;
}

// Check if the question has already been answered
bool already_answered(char *category, int value) {
    for (int i = 0; i < NUM_QUESTIONS; ++i) {
        if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
            return questions[i].answered;
        }
    }
    return true;
}

// Get the index of the question in the array for the given category and value
int get_question_index(char* category, int value) {
    for (int i = 0; i < NUM_QUESTIONS; ++i) {
        if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
            return i;
        }
    }
    return -1;
}
