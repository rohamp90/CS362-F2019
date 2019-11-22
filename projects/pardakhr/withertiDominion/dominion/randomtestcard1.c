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
/******************* Baron Random Test *****************/
/*******************************************************/

int main(int argc, char *argv[]){
	srand(time(NULL));

	// Setup: initialize game state, player and cards
	int testNum = 0;
	int handCount;
	int haveEstate;
	int choice;
	int estateCount;

	struct gameState G1;
	struct gameState G2;

	int seed = time(NULL);

	// Kingdom card array
	int k[10] = { adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall };

	printf("########## Random Test 1 ##########\n");
	printf("########## Card: Baron ############\n");

	while (testNum < 200){
		handCount = 1 + rand() % 10;					// random hand count between 1 to 10
		choice = rand() % 2;							// random choice between 0 and 1
		estateCount = rand() % 2;						// random choice between 0 and 1 (having or not having an estate)
		haveEstate = 0;									// flag for having an estate or not

		memset(&G1, 23, sizeof(struct gameState));
		memset(&G2, 23, sizeof(struct gameState));

		//Initialize a new game
		int r = initializeGame(2, k, seed, &G1);

		// Initialize hand
		G1.handCount[0] = handCount;
		G1.hand[0][0] = baron;							// give hand a baron card

		// fill hand with random card
		for (int i = 1; i < G1.handCount[0]; i++){
			G1.hand[0][i] = 1 + rand() % 16;			// random card between estate to great hall
			if (G1.hand[0][i] == estate) {
				haveEstate = 1;
			}
		}

		// random estate suply of 1 or 0
		G1.supplyCount[estate] = estateCount;

		memcpy(&G2, &G1, sizeof(struct gameState));

		baronCardEffect(choice, 0, &G1);

		testNum++;
		printf("**Test Number %d **:  Hand Count = %d -- Estate Supply Count = %d -- Player has estate: %d -- Choice Number: %d\n", testNum, handCount, estateCount, haveEstate, choice);

		Validate("Number of buys increased", G1.numBuys == G2.numBuys + 1);

		// player has estate and choose not discard
		if (choice == 0 && estateCount == 1){
			Validate("Estate in discard pile", G1.discard[0][0] == estate);
			Validate("Estate decremented from supply", G1.supplyCount[estate] == G2.supplyCount[estate] - 1);
		}

		// player has estate and choose not to discard
		if (choice == 1 && haveEstate == 1){
			Validate("Number of coins increased by 4", G1.coins == G2.coins + 4);
			Validate("Estate in discard pile", G1.discard[0][0] == estate);
			Validate("Discard pile count increased", G1.discardCount == G2.discardCount + 1);
			Validate("Estate no longer in hand", G1.hand[0][handCount] == -1);
			Validate("Hand count decremented by 1", G1.handCount[0] == G2.handCount[0] - 1);
		}

		// player has no estate and choose to discard
		if (choice == 1 && haveEstate == 0 && estateCount > 0){
			Validate("Estate in discard pile", G1.discard[0][0] == estate);
			Validate("Estate decremented from supply", G1.supplyCount[estate] == G2.supplyCount[estate] - 1);
		}

		printf("\n\n");
	}
	return 0;
}