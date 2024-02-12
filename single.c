#include <stdio.h>

#include "beggar.h"
#include "shuffle.h"

/**
 * main - Takes user input of how many players are playing and if talkative
 * should be printed. It then plays through one game of beggar my neighbour
 */
int main()
{
    int num_players, talkative, i, j, turns;
    int card = 0;
    int deck[52] = {2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14};

    // shuffle the deck in place
    shuffle(deck, 52, -1);

    // Get user inputs for game
    printf("Enter the number of players: ");
    scanf("%d", &num_players);
    printf("Do you want talkative mode enables (1; Yes, 0; No): ");
    scanf("%d", &talkative);
    // play the game
    turns = beggar(num_players, deck, talkative);

    printf("The amount of turns was %d\n", turns);

    return 0;
}
