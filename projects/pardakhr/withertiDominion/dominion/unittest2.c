#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

// Alternative to Assert for Gcov purposes
int Validation(char* msg, int x) {
	printf("Case: %s\n", msg);
	if (x != 1)
		printf(" > Error: Condition Failed!\n");

	else
		printf(" > Passed Successfully!\n");

}
/*******************************************************/
/******************* Minion UNIT TEST ******************/
/*******************************************************/
int main()
{
	int seed = time(0);
	int numPlayers = 2;
	int player = 0;
	int player2 = 1;		// other player
	int handpos = 0;

	struct gameState G1, G2;
	int k[10] = { minion, estate, council_room, feast, gardens, mine , remodel, smithy, village, great_hall };

	printf("########### UNIT TEST 2 ###########\n");
	printf("########## Card: Minion ###########\n");

	// Test 1:
	printf("**Test_1**: Gain +1 Actions and +2 Coins:\n");

	// Setup: initialize game state, player and cards
	memset(&G1, 23, sizeof(struct gameState));
	memset(&G2, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G1);
	memcpy(&G2, &G1, sizeof(struct gameState));

	minionEffect(1, 0, player, &G2, handpos);

	Validation("Coins increased by 2", (G2.coins == G1.coins + 2));
	Validation("Actions increased by 1", (G2.numActions == G1.numActions + 1));
	Validation("Hand count reduced by 1", (G2.handCount[0] == G1.handCount[0] - 1));


	// Test 2: 
	printf("**\nTest_2**: All players discard hand and gain +4 cards(Other players >5 card):\n");

	// Setup: initialize game state, player and cards
	memset(&G1, 23, sizeof(struct gameState));
	memset(&G2, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G1);
	memcpy(&G2, &G1, sizeof(struct gameState));
	G2.handCount[player2] = 5;				// Give other player 5 cards


	minionEffect(0, 1, player, &G2, handpos);
	Validation("Player gained new cards (Checking first card)", (G2.hand[player][0] != -1));
	Validation("Player has now 4 cards", G2.handCount[player] == 4);
	Validation("Other player gained new cards", (G2.hand[player2][0] != -1));
	Validation("Other player has 4 cards", G2.handCount[player + 1] == 4);

	// Only current player discards hand and draws 4
	printf("**\nTest_3**: Only player discard and draw new cards (Other players <5 card):\n");

	// Setup: initialize game state, player and cards
	memset(&G1, 23, sizeof(struct gameState));
	memset(&G2, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G1);
	memcpy(&G2, &G1, sizeof(struct gameState));
	
	G2.handCount[player2] = 3;
	int playerHandCount = G2.handCount[player];

	for (int i = 0; i < playerHandCount; i++) {
		G2.hand[player][i] = -1;			// set flag for player cards
	}


	minionEffect(0, 1, player, &G2, handpos);
	Validation("Player gained new cards (Checking first card)", (G2.hand[player][0] != -1));
	Validation("Player has now 4 cards", G2.handCount[player] == 4);
	Validation("Played count is 5", (G2.playedCardCount == G1.playedCardCount + playerHandCount));
	Validation("Other player's hand count did not change", G2.handCount[player2] == 3);

	return 0;
}