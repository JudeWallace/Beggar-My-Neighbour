#if !defined(BEGGAR_H)
#define BEGGAR_H

int beggar(int Nplayers, int *deck, int talkative);
int finished(int **players, int Nplayers);
int *take_turn(int *player, int *pile);
void shift_array(int *playerCards);
int pile_size(int *pile);
int player_hand_size(int *player);

#endif
