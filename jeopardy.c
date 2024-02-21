#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "questions.h"
#include "players.h"
#include "jeopardy.h"

#define BUFFER_LEN 256
#define NUM_PLAYERS 4

typedef enum {
    SELECT_TURN,
    PLAY_TURN,
    ANSWER_QUESTION,
    DISPLAY_RESULTS
} GameState;

int num_players = 0;
int questions_answered = 0;
player players[NUM_PLAYERS];
char buffer[BUFFER_LEN] = { 0 };

void tokenize(char *input, char **tokens){
    char* str = strtok(input, " ");

    if (str != NULL && (strcmp(str, "who") != 0 && strcmp(str, "what") != 0)) return;

    str = strtok(NULL, " ");
    if (str != NULL && strcmp(str, "is") != 0) return;

    *tokens = strtok(NULL, "\n");
}

int show_results_comp_func(const void* p1, const void* p2){
    int f = players[*((int*)p1)].score;
    int s = players[*((int*)p2)].score;
    if (f > s) return -1;
    if (f < s) return 1;
    return 0;
}

void show_results(player *players, int num_players){
    printf("These are the results:\n");

    int indices[NUM_PLAYERS];
    for (int i = 0; i < num_players; indices[i] = i, ++i);
    qsort(indices, num_players, sizeof(int), show_results_comp_func);

    for (int i = 0; i < num_players; ++i){
        player* p = players + indices[i];
        printf("%d. %s, $%d\n", (i + 1), p->name, p->score);
    }
}

void get_question(char* input, char** category, int* value){
    char c;
    int i = 0;
    int val_start = -1, val_end = -1;
    do {
        c = input[i];
        if (c >= '0' && c <= '9'){
            if (val_start == -1){
                val_start = i;
            }
        }
        else{
            if (val_start != -1 && val_end == -1){
                input[i] = '\0';
                val_end = i;
            }
        }
        i++;
    } while (c != '\0');
    *value = atoi(input + val_start);

    i -= 2;
    val_start = -1; val_end = -1;
    do {
        c = input[i];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')){
            if (val_start == -1){
                val_start = i;
                input[i + 1] = '\0';
            }
        }
        else{
            if (val_start != -1 && val_end == -1){
                val_end = i + 1;
                break;
            }
        }
        --i;
    } while (c != '\0');
    *category = input + val_end;
}

void clear_screen(){
    printf("\e[1;1H\e[2J");
}

bool get_input(char* buffer){
    printf("-- ");
    fflush(stdin);

    if (fgets(buffer, BUFFER_LEN, stdin) == NULL) return false;
    size_t len = strlen(buffer);
    if (len <= 1) return false;

    if (buffer[len - 1] == '\n'){
        buffer[len - 1] = '\0';
    }
    return true;
}

int main()
{
    memset(players, 0, sizeof(players));
    printf("Welcome to Jeopardy!\n");
    initialize_game();
    clear_screen();
    printf("How many players? (Up to %d): ", NUM_PLAYERS);
    scanf("%d", &num_players);
    num_players = (num_players > NUM_PLAYERS) ? NUM_PLAYERS : num_players;
    printf("\nEnter player names:\n");

    for (int i = 0; i < num_players; ++i){
        printf("%d. ", (i + 1));
        scanf("%s", players[i].name);
    }

    int player_index = -1;
    int question_index = -1;
    GameState state = SELECT_TURN;
    bool running = true;

    while (running){
        clear_screen();

        switch (state){
            case SELECT_TURN: {
                if (questions_answered >= NUM_QUESTIONS){
                    state = DISPLAY_RESULTS;
                    continue;
                }

                printf("Enter the next player\n");
                if (!get_input(buffer)) continue;
                player_index = player_exists(players, num_players, buffer);
                if (player_index == -1){
                    printf("That player doesn't exist...\n");
                    continue;
                }
                state = PLAY_TURN;
            } break;

            case PLAY_TURN: {
                display_categories();
                printf(
                    "\nIt's %s's turn!\n"
                    "Enter the dollar amount and category of the question you want to answer, "
                    "like; $100 in programming\n",
                    players[player_index].name
                );
                if (!get_input(buffer)) continue;

                char* category = NULL;
                int value = 0;
                get_question(buffer, &category, &value);

                question_index = get_question_index(category, value);
                if (question_index == -1){
                    printf("The question doesn't exist\n");
                    sleep(2);
                    continue;
                }

                if (already_answered(category, value)){
                    printf("The question has already been answered\n");
                    sleep(2);
                    continue;
                }
                state = ANSWER_QUESTION;
            } break;

            case ANSWER_QUESTION: {
                display_question_index(question_index);
                if (question_index < 0) continue;
                question* q = questions + question_index;

                printf("\nWhat's %s's answer?\n", players[player_index].name);
                if (!get_input(buffer)) continue;
                char* answer = NULL;
                tokenize(buffer, &answer);
                if (!answer){
                    printf("Invalid Input\n");
                    continue;
                }

                bool valid = valid_answer_index(question_index, answer);
                printf("\nYour answer is...\n");

                if (valid){
                    printf("Correct!!\nYou earned $%d\n", q->value);
                    update_score_index(players, player_index, q->value);
                    sleep(2);
                }
                else{
                    printf("Incorrect.\nThe correct answer: %s\n", q->answer);
                    sleep(2);
                }
                q->answered = true;
                questions_answered++;
                state = SELECT_TURN;
            } break;

            case DISPLAY_RESULTS: {
                printf("There are no more questions left to answer.\n\n");
                show_results(players, num_players);
                running = false;
            } break;
        }


    }
    return EXIT_SUCCESS;
}