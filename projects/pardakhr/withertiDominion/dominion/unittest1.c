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
/******************* BARON UNIT TEST *******************/
/*******************************************************/
int main()
{
	int seed = time(0);
	int numPlayers = 2;
	int player = 0;

	struct gameState G1, G2;
	int k[10] = { baron, estate, council_room, feast, gardens, mine , remodel, smithy, village, great_hall };

	printf("########### UNIT TEST 1 ###########\n");
	printf("########### Card: Baron ###########\n");

	// Test 1:
	printf("**Test_1**: Player has estate in hand and not discard it:\n");
	
	// Setup: initialize game state, player and cards
	memset(&G1, 23, sizeof(struct gameState));
	memset(&G2, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G1);
	G1.hand[player][1] = estate;
	memcpy(&G2, &G1, sizeof(struct gameState));

	baronCardEffect(0, player, &G2);

	// Verify number of buys incremented
	Validation("gain 1 buy", (G2.numBuys == G1.numBuys + 1));

	// Verify estate gain
	Validation("Estate supply count decremented", (G2.supplyCount[estate] == G1.supplyCount[estate] - 1));
	Validation("Discard count incremented", (G2.discardCount[player] == G1.discardCount[player] + 1));
	Validation("Estate is placed in discard", (G2.discard[0][0] == estate));

	// Test 2: 
	printf("\n**Test_2**: Player has no estate in hand and not discard it:\n");

	// Setup: initialize game state, player and cards
	memset(&G1, 23, sizeof(struct gameState));
	memset(&G2, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G1);
	G1.supplyCount[estate] = 0;

	for (int i = 0; i < G1.handCount[player]; i++) { // set hand to all copper
		G1.hand[player][i] = copper;
	}
	memcpy(&G2, &G1, sizeof(struct gameState));

	baronCardEffect(0, player, &G2);

	Validation("Estate supply count did not change", (G2.supplyCount[estate] == G1.supplyCount[estate]));
	Validation("Discard count is not changed", (G2.discardCount[player] == G1.discardCount[player]));
	Validation("Estate is not placed in discard", (G2.discard[0][0] != estate));

	// Tesr 3:
	printf("\n**Test_3**: Player has no estate in hand and discards an estate:\n");

	// Setup: initialize game state, player and cards
	memset(&G1, 23, sizeof(struct gameState));
	memset(&G2, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G1);
	G1.supplyCount[estate] = 0;

	for (int i = 0; i < G1.handCount[player]; i++) { // set hand to all copper
		G1.hand[player][i] = copper;
	}
	memcpy(&G2, &G1, sizeof(struct gameState));

	baronCardEffect(1, player, &G2);
	Validation("4 coins should not be added", (G2.coins == G1.coins));
	Validation("Estate supply count did not change", (G2.supplyCount[estate] == G1.supplyCount[estate]));
	Validation("Discard count is not changed", (G2.discardCount[player] == G1.discardCount[player]));
	Validation("Estate is not placed in discard", (G2.discard[0][0] != estate));

	// Tesr 4:
	printf("\n**Test_4**: Player has estate in hand and discards an estate:\n");

	// Setup: initialize game state, player and cards
	memset(&G1, 23, sizeof(struct gameState));
	memset(&G2, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G1);
	G1.hand[player][1] = estate;
	memcpy(&G2, &G1, sizeof(struct gameState));

	baronCardEffect(1, player, &G2);

	Validation("4 coins should be added", (G2.coins == G1.coins + 4));
	Validation("Player should loses a card", (G2.handCount[player] < G1.handCount[player]));
	Validation("Estate is placed in discard", (G2.discard[0][0] == estate));
	Validation("Discard count incremented", (G2.discardCount[player] == G1.discardCount[player] + 1));

	// Tesr 5:
	printf("\n**Test_5**: Player gain estate but supply is empty:\n");
	memset(&G1, 23, sizeof(struct gameState));
	memset(&G2, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G1);
	for (int i = 0; i < G1.handCount[player]; i++) { // set hand to all copper
		G1.hand[player][i] = copper;
	}
	G1.supplyCount[estate] = 0;
	memcpy(&G2, &G1, sizeof(struct gameState));

	baronCardEffect(0, player, &G2);
	Validation("No estates gained", (G2.supplyCount[estate] == 0));
	Validation("Discard count is not changed", (G2.discardCount[player] == G1.discardCount[player]));
	Validation("Estate is not placed in discard", (G2.discard[0][0] != estate));

	return 0;
}