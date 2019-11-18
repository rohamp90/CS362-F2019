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
/***************** Tribute Random Test *****************/
/*******************************************************/
int main(int argc, char *argv[])
{
	srand(time(NULL));

	// Setup: initialize game state, player and cards
	int seed = time(NULL);
	int testNum = 0;
	int treasures, victory, actions;
	int cards[2] = { -1, -1 };
	int i;
	struct gameState G1;
	struct gameState G2;

	// Kingdom card array
	int k[10] = { adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall };

	printf("########## Random Test 3 ##########\n");
	printf("######### Card: Tribute ###########\n");

	while (testNum < 200)
	{
		// Initialize variables
		treasures = victory = actions = 0;
		cards[0] = -1;
		cards[1] = -1;
		
		memset(&G1, 23, sizeof(struct gameState));
		memset(&G2, 23, sizeof(struct gameState));

		//Initialize a new game
		int r = initializeGame(2, k, seed, &G1);

		//Initialize hand variables of player 0
		G1.handCount[0] = 1 + rand() % 10;					// random hand count between 1 to 10
		G1.hand[0][0] = tribute;							// give player 0 a tribute card

		//Initialize deck of player 1
		G1.deckCount[1] = rand() % 11;						// random deck count between 0-10
		
		// fill hand with random cards
		for (int i = 0; i < G1.deckCount[1]; i++){
			G1.deck[1][i] = 1 + rand() % 16;				// random card between estate to great hall
		}

		G1.discardCount[1] = rand() % 11;					// random discard count between 0-10

		// fill discard pile with random cards
		for (int j = 0; j < G1.discardCount[1]; j++){
			G1.discard[1][j] = 1 + rand() % 16;				// random card between estate to great hall
		}

		memcpy(&G2, &G1, sizeof(struct gameState));

		tributeEffect(0, 1, &G1);

		testNum++;
		printf("**Test Number %d**\n", testNum);

		// boundary conditions
		// player 2 has no card in deck and 1 card in discard pile
		if (G2.deckCount[1] == 0 && G2.discardCount[1] == 1){
			Validate("Discard pile is decremented", G1.discardCount[1] == G2.discardCount[1] - 1);
			cards[0] = G2.discard[1][0];
		}
		// player 2 has 1 card in deck and 2 cards in discard pile
		else if (G2.deckCount[1] == 0 && G2.discardCount[1] == 2){
			Validate("Deck is empty", G1.deckCount[1] == 0);
			Validate("2 cards in the discard pile", G1.discardCount[1] == 2);
			cards[0] = G2.discard[1][0];
			cards[1] = G2.discard[1][1];
		}
		// player 2 has 1 card in deck and 1 card in discard pile
		else if (G2.deckCount[1] == 1 && G2.discardCount[1] == 0) {
			Validate("Deck count is decremented", G1.deckCount[1] == G2.deckCount[1] - 1);
			cards[0] = G2.deck[1][0];
		}
		// player 1 reveal and discard top 2 cards
		else if (G2.deckCount[1] > 1){
			Validate("Deck is decremented by 2", G1.deckCount[1] == G2.deckCount[1] - 2);
			Validate("Discard count incremented by 2", G1.discardCount[1] == G2.discardCount[1] + 2);
			cards[0] = G2.deck[1][0];
			cards[1] = G2.deck[1][1];
		}

		// track player 0 gains
		for (i = 0; i < 2; i++){
			if (cards[i] == copper || cards[i] == silver || cards[i] == gold){
				treasures += 2;
			}
			else if (cards[i] == estate || cards[i] == duchy || cards[i] == province || cards[i] == gardens || cards[i] == great_hall){
				victory += 2;
			}
			else{
				actions += 2;
			}
		}

		Validate("Coinds added", G1.coins == G2.coins + treasures);
		Validate("Victory cards added", G1.handCount[0] == G2.handCount[0] + victory);
		Validate("Action cards added", G1.numActions == G2.numActions + actions);

		printf("\n\n");
	}
	return 0;
}