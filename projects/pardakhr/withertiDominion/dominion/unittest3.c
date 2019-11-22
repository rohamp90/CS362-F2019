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
/***************** Ambassador UNIT TEST ****************/
/*******************************************************/
int main()
{
	int handpos = 0;
	int seed = time(0);
	int numPlayers = 2;
	int player = 0;
	int player2 = 1;			// other player
	int result;
	struct gameState G1, G2;

	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room };

	int numOtherPlayers = numPlayers - 1; // change this

	printf("########### UNIT TEST 3 ###########\n");
	printf("########## Card: Ambassador ###########\n");

	// Test 1
	printf("**Test_1**: Reveal and return no estate cards to supply:\n");

	// initialize a game state and player cards
	memset(&G1, 23, sizeof(struct gameState));
	memset(&G2, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G1);
	G1.supplyCount[estate] = 8;
	G1.hand[player][1] = estate;
	memcpy(&G2, &G1, sizeof(struct gameState));

	result = ambassadorEffect(1, 0, player, &G2, handpos);
	Validate("No estate card was added to supply", G2.supplyCount[estate] == G1.supplyCount[estate]);
	Validate("Other player gained and estate card", G2.handCount[player2] > G1.handCount[player2]);
	Validate("Ambassador worked properly", result != -1);

	// Test 2
	printf("\n**Test_2**: Failure test, boundary condition  Have estate but return -1 cards to supply :\n");

	// initialize a game state and player cards
	memset(&G1, 23, sizeof(struct gameState));
	memset(&G2, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G1);
	G1.supplyCount[estate] = 8;
	G1.hand[player][1] = estate;
	memcpy(&G2, &G1, sizeof(struct gameState));

	result = ambassadorEffect(1, -1, player, &G2, handpos);
	Validate("Ambassador worked properly", result == -1);

	// Test 3
	printf("\n**Test_3**: Failure test, boundary condition dont have estate and return -1 cards to supply :\n");

	// initialize a game state and player cards
	memset(&G1, 23, sizeof(struct gameState));
	memset(&G2, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G1);
	for (int i = 1; i < G1.handCount[player]; i++) {
		G1.hand[player][i] = copper;					// fill hand with copper
	}
	memcpy(&G2, &G1, sizeof(struct gameState));

	result = ambassadorEffect(1, -1, player, &G2, handpos);
	Validate("Ambassador worked properly", result == -1);

	// Test 4
	printf("\n**Test_4**: Only 1 estate in hand, but return 2 to supply:\n");

	// initialize a game state and player cards
	memset(&G1, 23, sizeof(struct gameState));
	memset(&G2, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G1);
	memcpy(&G2, &G1, sizeof(struct gameState));

	G2.hand[player][1] = estate;

	for (int i = 1; i < G2.handCount[player]; i++){		
		G2.hand[player][i] = copper;					// fill hand with copper
	}

	result = ambassadorEffect(1, 2, player, &G2, handpos);
	Validate("Ambassador worked properly", result == -1);

	// Test 5
	printf("\n**Test_5**: Return one estate to supply:\n");

	// initialize a game state and player cards
	memset(&G1, 23, sizeof(struct gameState));
	memset(&G2, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G1);
	G2.hand[player][1] = estate;

	for (int i = 1; i < G2.handCount[player]; i++) {
		G2.hand[player][i] = copper;					// fill hand with copper
	}

	memcpy(&G2, &G1, sizeof(struct gameState));

	ambassadorEffect(1, 1, player, &G2, handpos);
	Validate("One estate card was added to supply", G2.supplyCount[estate] == G1.supplyCount[estate]);
	Validate("Other player gained and estate card", G2.handCount[player2] > G1.handCount[player2]);
	Validate("Player lost the estate card", G2.hand[player][1] != estate);

	return 0;
}