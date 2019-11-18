#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

// Alternative to Assert for Gcov purposes
int Validate(char* msg, int x) {
	printf("Case: %s\n", msg);
	if (x != 1)
		printf(" > Error: Condition Failed!\n");

	else
		printf(" > Passed Successfully!\n");
}

/*******************************************************/
/****************** Minion Random Test *****************/
/*******************************************************/

int main(int argc, char *argv[]){
	srand(time(NULL));

	int seed = time(NULL);

	// Setup: initialize game state, player and cards
	int i, j;
	int tesNum = 0;
	int choice1;
	int choice2;
	int playersNum;
	int cardTotal = 0;

	// Kingdom card array
	int k[10] = { adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall };

	//Declare game states
	struct gameState G1;
	struct gameState G2;

	printf("########## Random Test 2 ##########\n");
	printf("########## Card: Minion ############\n");

	while (tesNum < 200)
	{
		// Initialize total cards at 0
		cardTotal = 0;

		playersNum = 2 + rand() % 3;					// random number of players between 2 and 4
		choice1 = rand() % 2;							// random choice between 0 and 1
		choice2 = rand() % 2;							// random choice between 0 and 1

		memset(&G1, 23, sizeof(struct gameState));
		memset(&G2, 23, sizeof(struct gameState));

		// Initialize a new game
		int r = initializeGame(playersNum, k, seed, &G1);

		// Initialize hands
		G1.handCount[0] = 1 + rand() % 10;							// random hand count between 1 to 10
		G1.hand[0][0] = minion;										// give player 0 a minion card

		// fill random hand counts with random cards
		for (j = 1; j < playersNum; j++){
			G1.handCount[j] = 1 + rand() % 10;						// random hand count between 1 to 10
			for (i = 1; i < G1.handCount[j]; i++){
				G1.hand[j][i] = 1 + rand() % 16;					// random card between estate to great hall;
			}
		}

		memcpy(&G2, &G1, sizeof(struct gameState));

		minionEffect(choice1, choice2, 0, &G1, 0);

		tesNum++;
		printf("**Test Number %d **: Choice Number 1 = %d, Choice Number 2 = %d\n", tesNum, choice1, choice2);

		Validate("Number of action cards increased", G1.numActions == G2.numActions + 1);
		Validate("Minion card played", G1.playedCards[0] == minion);
		cardTotal++;

		// Player chose to discard hand, and gain 4 cards as well as all other players with 5 cards
		if (choice2 == 1){
			Validate("Player has 4 cards", G1.handCount[0] == 4);
			cardTotal += G2.handCount[0];

			for (i = 1; i < playersNum; i++){
				// other players have +5 cards
				if (G2.handCount[i] >= 5){
					Validate("Other players discard hand and redraw 4 cards", G1.handCount[i] == 4);
					cardTotal += G2.handCount[i];
				}
				else{
					Validate("Other players did not have 4 cards and no change", G1.handCount[i] == G2.handCount[i]);
				}
			}

			Validate("Number of cards played", G1.playedCardCount == cardTotal);
		}

		// Player choose to gain 2 coins
		if (choice1 == 1 && choice2 == 0){
			Validate("Number of coins increased by 2", G1.coins == G2.coins + 2);
			Validate("Minion card no longer in hand", G1.hand[0][G2.handCount[0]] == -1);
			Validate("Hand count decremented", G1.handCount[0] == G2.handCount[0] - 1);
			Validate("Card is played", G1.playedCardCount == G2.playedCardCount + 1);
		}

		printf("\n\n");
	}

	return 0;
}