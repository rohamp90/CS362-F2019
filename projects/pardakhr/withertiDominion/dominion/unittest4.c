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
/******************* Tribute UNIT TEST ******************/
/*******************************************************/
int main()
{
	int seed = 1000;
	int numPlayers = 2;
	int player = 0;
	int player2 = 1;				// other player
	struct gameState G1, G2;
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room };

	initializeGame(numPlayers, k, seed, &G1);

	printf("########### UNIT TEST 4 ###########\n");
	printf("########## Card: Tribute ###########\n");

	// Test 1:
	printf("**Test_1**: Other player has no card:\n");

	// Setup: initialize game state, player and cards
	memset(&G1, 23, sizeof(struct gameState));
	memset(&G2, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G1);
	memcpy(&G2, &G1, sizeof(struct gameState));

	G2.deckCount[player2] = 0;
	tributeEffect( player, player2, &G2);
	Validate("Player gained no bonus cards", G2.handCount[player] + 1 == G1.handCount[player]);
	Validate("Player gained no action cards", G2.numActions == G1.numActions);
	Validate("Player gained no coin", G2.coins == G1.coins);

	// Test 2
	printf("\n**Test_2**: Other player has Action card:\n");

	// Setup: initialize game state, player and cards
	memset(&G1, 23, sizeof(struct gameState));
	memset(&G2, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G1);
	memcpy(&G2, &G1, sizeof(struct gameState));

	G2.deckCount[player2] = 1;
	G2.deck[player2][0] = tribute;
	G2.discardCount[player2] = 0;

	tributeEffect(player, player2, &G2);
	Validate("Player gained no bonus cards", G2.handCount[player] + 1 == G1.handCount[player]);
	Validate("Player gained two action cards", G2.numActions == G1.numActions + 2);
	Validate("Player gained no coin", G2.coins == G1.coins);

	// Test 3
	printf("\n**Test_3**: Other player has Treasure card:\n");

	// Setup: initialize game state, player and cards
	memset(&G1, 23, sizeof(struct gameState));
	memset(&G2, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G1);
	memcpy(&G2, &G1, sizeof(struct gameState));

	G2.deckCount[player2] = 1;
	G2.deck[player2][0] = copper;
	G2.discardCount[player2] = 0;

	tributeEffect(player, player2, &G2);
	Validate("Player gained no bonus cards", G2.handCount[player] + 1 == G1.handCount[player]);
	Validate("Player gained no action cards", G2.numActions == G1.numActions);
	Validate("Player gained two coin", G2.coins == G1.coins + 2);

	// Test 4
	printf("\n**Test_3**: Other player has Victory card:\n");

	// Setup: initialize game state, player and cards
	memset(&G1, 23, sizeof(struct gameState));
	memset(&G2, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G1);
	memcpy(&G2, &G1, sizeof(struct gameState));

	G2.deckCount[player2] = 1;
	G2.deck[player2][0] = estate;
	G2.discardCount[player2] = 0;

	tributeEffect(player, player2, &G2);
	Validate("Player gained  bonus cards", G2.handCount[player] -1 == G1.handCount[player]);
	Validate("Player gained no action cards", G2.numActions == G1.numActions);
	Validate("Player gained no coin", G2.coins == G1.coins);

	return 0;
}
