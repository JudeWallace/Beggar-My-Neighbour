#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

/**
 * shift_array - Moves the values in the players deck to the left one space
 *
 * @playerCards: A pointer to the memory location of the players deck
 *
 * Return - None
 */
void shift_array(int *playerCards)
{
    int size = 52;
    int i;
    // Loop over the memory locations and shift each element -1 places
    for (i = 0; i < size - 1; i++)
    {
        playerCards[i] = playerCards[i + 1];
    }
}

/**
 * pile_size - Calculate the length of the pile of cards
 *
 * @pile: Pointer to the memory location of the pile
 *
 * Return: An integer which is the length of the pile
 */
int pile_size(int *pile)
{
    int count = 0;
    int i;
    int pile_element = pile[0];
    // Count all valid card elements in the pile
    for (i = 0; pile_element != 0; i++)
    {
        count++;
        pile_element = pile[i + 1];
    }

    // If pile has no cards return zero
    if (count == 0)
    {
        return 0;
    }
    // Return the count if > 0, meaning the pile isn't empty
    return count;
}

/**
 * take_turn - Function responsible for the game logic of a player placing a card
 * on the pile and removing it from the players hand. It also takes into account
 * player needing to play X penalty cards on there turn
 *
 * @player: A pointer to the players hand who's turn it is
 * @pile: A pointer to the current pile of cards 'on the table'
 *
 * Return: If the player is required to lay down penalty cards; if they do so
 * without placing there own penalty card the return is the pile. If the player
 * just needs to play a single card or places a penalty card whilst laying down
 * penalty cards, the return is 0
 */
int *take_turn(int *player, int *pile)
{
    int curr_position = pile_size(pile) - 1;
    int penalty = 0;
    int player_top_card = player[0];
    int i, j;
    // If the pile is empty place a single card
    if (curr_position == -1)
    {
        pile[0] = player[0];
        shift_array(player);
        return 0;
    }

    int top_card = pile[curr_position];
    // If the top card is a penalty place a single card
    if (top_card < 11)
    {
        pile[curr_position + 1] = player[0];
        shift_array(player);
    }
    // If the card is a penalty card place cards required by the penalty
    else if (top_card >= 11)
    {
        switch (top_card)
        {
        case 11:
            penalty = 1;
            break;
        case 12:
            penalty = 2;
            break;
        case 13:
            penalty = 3;
            break;
        case 14:
            penalty = 4;
            break;
        }

        j = curr_position + 1;
        for (i = 0; i < penalty; i++)
        {
            player_top_card = player[0];
            // If while placing penalty cards the player lays a penalty card
            // themselves, return a null pointer
            if (player_top_card >= 11)
            {
                pile[j] = player[0];
                shift_array(player);
                return 0;
            }
            else
            {
                // Place a single card onto the pile
                pile[j] = player[0];
                shift_array(player);
                j++;
            }
        }
        // Return the pile pointer to denote the penalty cards was fully played through
        return pile;
    }
    // Return null pointer
    return 0;
}

/**
 * finished - Checks if one player has all the cards of the deck
 *
 * @players: An array of pointers which each position points to the memory location
 * of the players hand
 * @Nplayers: The number of players in the game
 *
 * Return: If one player has all the cards, 1 is returned denoting the game is
 * finished. Return of 0 if the cards aren't all held by one player
 */
int finished(int **players, int Nplayers)
{
    int count_0 = 0;  // Count of players with 0 cards
    int count_52 = 0; // Count of players with 52 cards
    int i = 0;
    int j = 0;
    // Loop over the players
    for (i = 0; i < Nplayers; i++)
    {
        // Count of cards for this player
        int count = 0;

        // Count the number of cards for this player
        for (j = 0; j < 52; j++)
        {
            int check_empty = players[i][j];
            if (check_empty != 0)
            {
                // 0 represents an empty card slot
                count++;
            }
        }

        // Update the counts for players with 0 or 52 cards
        if (count == 0)
        {
            count_0++;
        }
        else if (count == 52)
        {
            count_52++;
        }
    }

    // Check if the game is finished
    if (count_52 == 1 || count_0 == Nplayers - 1)
    {
        // Games has a winner
        return 1;
    }
    else
    {
        // Game has no winner yet
        return 0;
    }
}

/**
 * beggar - The main function of the game logic for the game beggar my neighbout
 * card game
 *
 * @Nplayers:
 * @deck: A pointer to a shuffled deck
 * @talkative: An integer for if the program should print what is happening
 * within the game
 *
 * Return: The amount of turns it took before a winner occurred in the game
 */
int beggar(int Nplayers, int *deck, int talkative)
{
    int i = 0;
    int j = 0;
    int turn = 0;
    int curr_turn = 0;
    int previous_turn = 0;
    int total_turns = 1;
    int *reward = 0;
    int top_card = 0;
    int empty_top_hand = 0;
    int curr_player_top_card = 0;
    int player_placed_penalty = 0;
    int pos_of_penalty = 0;
    int penalty = 0;
    int pile_length = 0;
    int reset = 0;

    // Allocate all the necessary memory to represent the pile and players hands
    int *pile = calloc(52, sizeof(int));
    if (pile == NULL)
    {
        fprintf(stderr, "Error with pile memory allocation\n");
        exit(1);
    }

    int **players_hands = malloc(Nplayers * sizeof(int *));
    if (players_hands == NULL)
    {
        fprintf(stderr, "Error with players_hands memory allocation\n");
        exit(1);
    }

    for (i = 0; i < Nplayers; i++)
    {
        players_hands[i] = calloc(52, sizeof(int));
        if (players_hands[i] == NULL)
        {
            fprintf(stderr, "Error with players hand memory allocation\n");
            exit(1);
        }
    }

    // Distribute one card to each player until the deck is exhausted
    for (i = 0; i < 52; i++)
    {
        int player_index = i % Nplayers;
        int card_index = i / Nplayers;
        players_hands[player_index][card_index] = deck[i];
    }

    // Start the game logic, and only stop when a winner of the game is found
    while (!finished(players_hands, Nplayers))
    {
        curr_turn = turn % Nplayers;
        pile_length = pile_size(pile);
        empty_top_hand = players_hands[curr_turn][0];

        if (pile_length >= 1)
        {
            top_card = pile[pile_length - 1];
        }
        else
        {
            top_card = pile[0];
        }

        // if player with turn next has 0 cards move to next player
        if (empty_top_hand == 0)
        {
            turn++;
            continue;
        }

        // If length of pile is 0, the pile is empty
        else if (pile_length == 0)
        {
            // Talkative message for an empty pile of cards
            if (talkative)
            {
                // If the pile is empty due to penalty cards turns ending, display relevant message
                if (reset)
                {
                    printf("Turn %d:         Previous player laid all cards without playing a penalty card, so the pile at the end of last turn is given to player %d\n", total_turns, player_placed_penalty);
                    printf("               Pile is empty so, player %d to lay a single card\n", curr_turn);
                }
                else
                {
                    printf("Turn %d:        Pile is empty so, player %d to lay a single card\n", total_turns, curr_turn);
                }
                printf("Pile: ");
                for (i = 0; i < 52; i++)
                {
                    if (pile[i] == 0)
                    {
                        break;
                    }
                    printf("%d ", pile[i]);
                }
                printf("\n");

                for (i = 0; i < Nplayers; i++)
                {
                    if (i == curr_turn)
                    {
                        printf("*  %d: ", i);
                    }
                    else
                    {
                        printf("   %d: ", i);
                    }
                    for (j = 0; j < 52; j++)
                    {
                        int players_card = players_hands[i][j];
                        if (players_card == 0)
                        {
                            break;
                        }
                        printf("%d ", players_hands[i][j]);
                    }
                    printf("\n");
                }
            }

            // Player to add a single card to the pile
            reward = take_turn(players_hands[curr_turn], pile);

            // Update necessary counter variablees
            turn++;
            total_turns++;
            reset = 0;
            continue;
        }

        // If top card is less than 11 player to lay a single card
        else if (top_card < 11)
        {
            if (talkative)
            {
                printf("Turn %d:         Top card is %d, so player %d should lay a single card\n", total_turns, top_card, curr_turn);

                printf("Pile: ");
                for (i = 0; i < 52; i++)
                {
                    if (pile[i] == 0)
                    {
                        break;
                    }
                    printf("%d ", pile[i]);
                }
                printf("\n");

                for (i = 0; i < Nplayers; i++)
                {
                    if (i == curr_turn)
                    {
                        printf("*  %d: ", i);
                    }
                    else
                    {
                        printf("   %d: ", i);
                    }
                    for (j = 0; j < 52; j++)
                    {
                        int players_card = players_hands[i][j];
                        if (players_card == 0)
                        {
                            break;
                        }
                        printf("%d ", players_hands[i][j]);
                    }
                    printf("\n");
                }
            }

            curr_player_top_card = players_hands[curr_turn][0];

            // If the player lays a penalty card store which player layed it
            if (curr_player_top_card >= 11)
            {
                player_placed_penalty = curr_turn;
            }

            reward = take_turn(players_hands[curr_turn], pile);
            // Update counter variables
            turn++;
            total_turns++;
            continue;
        }

        // If the card at the top of the pile is >= 11 make the player place
        // X penalty cards
        else if (top_card >= 11)
        {
            pos_of_penalty = pile_size(pile) - 1;
            // Find the value of the card and set the amount of penalty cards needed to be layed
            switch (top_card)
            {
            case 11:
                penalty = 1;
                break;
            case 12:
                penalty = 2;
                break;
            case 13:
                penalty = 3;
                break;
            case 14:
                penalty = 4;
                break;
            }
            // Display a message saying a penalty card has been layed
            if (talkative)
            {
                printf("Turn %d:	Top card is %d, so player %d should lay %d penalty cards\n", total_turns, top_card, curr_turn, penalty);
                printf("Pile: ");
                for (i = 0; i < 52; i++)
                {
                    if (pile[i] == 0)
                    {
                        break;
                    }
                    printf("%d ", pile[i]);
                }
                printf("\n");

                for (i = 0; i < Nplayers; i++)
                {
                    if (i == curr_turn)
                    {
                        printf("*  %d: ", i);
                    }
                    else
                    {
                        printf("   %d: ", i);
                    }
                    for (j = 0; j < 52; j++)
                    {
                        int players_card = players_hands[i][j];
                        if (players_card == 0)
                        {
                            break;
                        }
                        printf("%d ", players_hands[i][j]);
                    }
                    printf("\n");
                }
            }

            reward = take_turn(players_hands[curr_turn], pile);
            // Update the counters, and store the player of this turn as the previous player to have a turn
            previous_turn = turn % Nplayers;
            turn++;
            total_turns++;

            // Do this logic until the reward returns a non null pointer, meaning the penalty card logic
            // has been played through and pile can be emptied
            while (!reward)
            {
                curr_turn = turn % Nplayers;

                // Check current player has cards
                if (players_hands[curr_turn][0] == 0)
                {
                    // Move to next player if player is out of cards
                    turn++;
                    continue;
                }
                else
                {
                    // Display talkative with the player who played penalty card and new amount
                    // of penalty cards needed to be placed
                    if (talkative)
                    {
                        top_card = pile[pile_size(pile) - 1];

                        switch (top_card)
                        {
                        case 11:
                            penalty = 1;
                            break;
                        case 12:
                            penalty = 2;
                            break;
                        case 13:
                            penalty = 3;
                            break;
                        case 14:
                            penalty = 4;
                            break;
                        }

                        printf("Turn %d:        player %d layed a penalty card whilst laying a penalty, so player %d should lay %d penalty cards\n", total_turns, previous_turn, curr_turn, penalty);
                        printf("Pile: ");
                        for (i = 0; i < 52; i++)
                        {
                            if (pile[i] == 0)
                            {
                                break;
                            }
                            printf("%d ", pile[i]);
                        }
                        printf("\n");

                        for (i = 0; i < Nplayers; i++)
                        {
                            if (i == curr_turn)
                            {
                                printf("*  %d: ", i);
                            }
                            else
                            {
                                printf("   %d: ", i);
                            }
                            for (j = 0; j < 52; j++)
                            {
                                int players_card = players_hands[i][j];
                                if (players_card == 0)
                                {
                                    break;
                                }
                                printf("%d ", players_hands[i][j]);
                            }
                            printf("\n");
                        }
                    }

                    reward = take_turn(players_hands[curr_turn], pile);
                    // Update counter variables
                    previous_turn = turn % Nplayers;
                    turn++;
                    total_turns++;
                }
            }
            // Find the index of the final valid card in the players hand
            i = 0;
            while (players_hands[player_placed_penalty][i] != 0)
            {
                i++;
            }

            // Copy the non-zero values from the pile to the player's hand
            int j = 0;
            while (pile[j] != 0)
            {
                players_hands[player_placed_penalty][i] = pile[j];
                i++;
                j++;
            }

            // Empty the pile
            int k = 0;
            for (k = 0; k < 52; k++)
            {
                pile[k] = 0;
            }
            reset = 1;
        }
    }

    int player_with_all_cards = 0;
    int player_hand_card = 0;

    for (i = 0; i < Nplayers; i++)
    {
        // Count of cards for this player
        int count = 0;

        // Count the number of cards for this player
        for (j = 0; j < 52; j++)
        {
            player_hand_card = players_hands[i][j];
            // If the player has cards store the index of the player and break
            if (player_hand_card != 0)
            {
                count++;
                break;
            }
        }

        // update the index of the player with all the cards
        if (count >= 1)
        {
            player_with_all_cards = i;
            break;
        }
    }

    // Display a message saying the game has been won
    if (talkative)
    {
        printf("Turn %d:         player %d has won the game!!\n", total_turns, player_with_all_cards);
        printf("Pile: ");
        for (i = 0; i < pile_size(pile); i++)
        {
            printf("%d ", pile[i]);
        }
        printf("\n");

        for (i = 0; i < Nplayers; i++)
        {
            printf("   %d: ", i);

            for (j = 0; j < 52; j++)
            {
                if (players_hands[i][j] == 0)
                {
                    break;
                }
                printf("%d ", players_hands[i][j]);
            }
            printf("\n");
        }
    }

    // Free all allocated memory
    free(pile);

    for (i = 0; i < Nplayers; i++)
    {
        free(players_hands[i]);
    }

    free(players_hands);

    // Return the total turns before a winner was found
    return total_turns;
}
