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

int main()
{
	int handpos = 0;
	int seed = 1000;
	int numPlayers = 2;
	int player = 0;
	struct gameState G1, G2;
	int result;
	int k[10] = { adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room };

	printf("########### UNIT TEST 5 ###########\n");
	printf("########### Card: Mine ############\n");

	// Test 1:
	printf("**Test_1**: Boundary condition, failed case:\n");

	// initialize a game state and player cards
	memset(&G1, 23, sizeof(struct gameState));
	memset(&G2, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G1);
	memcpy(&G2, &G1, sizeof(struct gameState));
	G2.hand[player][1] = copper;
	result = mineEffect(1, gold, player, &G2, handpos);
	Validate("Invalid gaining of treasure too high", result == -1);


	G2.hand[player][1] = curse;
	result = mineEffect( 1, silver, player, &G2, handpos);
	Validate("Invalid trashing of a curse card", result == -1);
	
	G2.hand[player][1] = silver;
	result = mineEffect(1, estate, player, &G2, handpos);
	Validate("Invalid gaining of non treasure card", result == -1);


	// Test 2:
	printf("\n**Test_2**: Positive test:\n");

	// Test proper interaction of cards

	memset(&G1, 23, sizeof(struct gameState));
	memset(&G2, 23, sizeof(struct gameState));
	initializeGame(numPlayers, k, seed, &G1);
	memcpy(&G2, &G1, sizeof(struct gameState));

	G2.hand[player][0] = copper;
	G2.hand[player][1] = copper;

	result = mineEffect(0, silver, player, &G2, handpos);
	Validate("One card discarded correctly", G2.handCount[player] == G1.handCount[player] - 1);
	Validate("Copper upgraded to silver correctly", result != -1);



	return 0;
}