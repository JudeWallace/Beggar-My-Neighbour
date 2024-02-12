#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <time.h>

#include "beggar.h"

struct Turns statistics(int Nplayer, int games, gsl_rng *r);

/**
 * Turns - a structure defined by me to store the shortest, longest, average
 * amount of turns in a game/games
 */
struct Turns
{
    int shortest_turns;
    int longest_turns;
    int average_turns;
};

/**
 * statistics - Takes in as params the number of players and the amount of games
 * to play with x players. It then plays the beggar my neighbour game, games times
 * with Nplayers. Every iteration of the game it uses the structure turns to record
 * the average turns, shortest turns and longest turns out of all the games played
 *
 * @Nplayer: The number of players in the game
 * @games: The amount of game iterations to play
 *
 * Return: The Turns structure with all the relevant data stored in it based on
 * the games just played
 */
struct Turns statistics(int Nplayer, int games, gsl_rng *r)
{
    struct Turns result;
    int deck[52] = {2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14};
    int i;
    int turns;

    // Loop through the games
    for (i = 0; i < games; i++)
    {
        // Check that the random number generator is properly initialized
        if (r == NULL)
        {
            printf("Error: random number generator not properly initialized\n");
            exit(1);
        }

        // shuffle the deck
        gsl_ran_shuffle(r, deck, 52, sizeof(int));

        // Run the game of beggar my neighbour
        turns = beggar(Nplayer, deck, 0);

        // Set the struct values based on the result of the game
        if (i == 0)
        {
            result.shortest_turns = turns;
            result.longest_turns = turns;
            result.average_turns = turns;
        }
        else
        {
            if (turns < result.shortest_turns)
            {
                result.shortest_turns = turns;
            }
            else if (turns > result.longest_turns)
            {
                result.longest_turns = turns;
            }

            result.average_turns = result.average_turns + turns;
        }
    }
    // Calculate the average turns of the games
    result.average_turns = round(result.average_turns / games);

    // Return the struct
    return result;
}

/**
 * main - Ask the user for input on now many players to go up to and how many
 * games to play for each number of players. It then runs the games with the
 * specified players and prints to the terminal the struct values, in a user
 * friendly format.
 */
int main()
{
    int num_players, games, i;

    printf("Enter the maximum number of players: ");
    scanf("%d", &num_players);
    printf("How many games should be played ");
    scanf("%d", &games);

    const gsl_rng_type *T;
    static gsl_rng *r = NULL;
    int seed = -1;

    if (r == NULL)
    {
        /* Create a random number generator */
        gsl_rng_env_setup();
        if (seed < 0)
            seed = time(NULL);
        gsl_rng_default_seed = seed;

        T = gsl_rng_default;
        r = gsl_rng_alloc(T);
    }

    // Play through the amount of players inputted
    for (i = 2; i < num_players + 1; i++)
    {
        // Run through the game and display the stats of it
        struct Turns stats = statistics(i, games, r);
        printf("Results based on %d players\n", i);
        printf("shortest amount of turns: %d\n", stats.shortest_turns);
        printf("longest amount of turns: %d\n", stats.longest_turns);
        printf("average amount of turns: %d\n", stats.average_turns);
        printf("\n");
    }
    // Free the number generator
    gsl_rng_free(r);

    return 0;
}
