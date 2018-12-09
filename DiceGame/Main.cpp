#include <iostream>
#include <string>
#include <ctime>

using namespace std;
bool AskQuestion(string question, string opt1 = "Yes", string opt2 = "No");
int TakeDeposit(int account, int low, int max);
int TakeWithdraw(int account, int low);

int main() {
	srand(time(0));

	// Initializing variables
	int const lowBet = 100, medBet = 300, highBet = 500, maxAmount = 5000;
	int gameAccount = 0, totalWinnings = 0, gamesCounter = 0;
	bool wantToPlayAgain = false;
	
	cout << "Hello and welcome to DiceThrower!\n" << endl;

	// Ask if player wants to start the game [Function at bottom]
	bool wantToStartGame = AskQuestion("Would you like to start the game? Reply with the corresponding number below.");
	if (wantToStartGame) {
		cout << "I am glad you wanted to play against me, lets get started!\n" << endl;
		do {

			// Check if the players account has enough money to perform the lowest bet, if it doesnt demand the player to add funds to their account
			if (gameAccount < lowBet) {
				cout << 
					"Before we get started, you will need some money on your account in order to make bets during the game." << endl <<
					"The different bets you can make in the game are these: [" << lowBet << ", " << medBet << ", " << highBet << "]\n" << 
				endl;

				// Deposit to the game account [Function at bottom]
				gameAccount += TakeDeposit(gameAccount, lowBet, maxAmount);
			}
			else
			{
				// If the player has money on their account, ask if they would still like to manage their account to add or remove funds. [Function at bottom]
				bool wantToManageAccount = AskQuestion("Before we get started, would you like to manage your account?");
				if (wantToManageAccount) {
					bool wantToDeposit = AskQuestion("Which action would you like to take?", "Deposit", "Withdraw");
					
					// Take a deposit or withdraw from the game account [Functions at bottom]
					if (wantToDeposit) gameAccount += TakeDeposit(gameAccount, lowBet, maxAmount);
					else gameAccount -= TakeWithdraw(gameAccount, lowBet);
				}
			}

			// Place bets before starting the game [YOU & PC]
			// Initialize and declare variables
			bool isAllowedBet = false;
			int betArray[3] = {lowBet, medBet, highBet};
			int allowedBets[3];
			int currentGamePot = 0;
			int userBet;
			string availableBets;

			// Check which bets can be performed based on the players account balance. 
			// Also gets a string of the bets available to the user to display later.
			for (int i = 0; i < sizeof(betArray) / sizeof(betArray[0]); i++) {
				if (gameAccount >= betArray[i]) {
					availableBets += (i > 0 ? ", " + to_string(betArray[i]) : to_string(betArray[i]));
					allowedBets[i] = betArray[i];
				}	
			}

			// Displays the string of available bets before the user to choose from.
			cout << "Please place a bet. You can choose between the following: [" << availableBets << "]" << endl;

			// Checks if the user choses one of the allowed bets.
			do {
				cin.clear();
				cin.ignore(INT_MAX, '\n');
				cin >> userBet;

				for (int i = 0; i < sizeof(allowedBets) / sizeof(allowedBets[0]); i++) {
					// If the bet is OK, remove the money from account, add the money to the pot and break out of loop.
					if (userBet == allowedBets[i]) {
						gameAccount -= userBet;
						currentGamePot += userBet;
						isAllowedBet = true;
						break;
					}
				}
			} while (!cin.good() || isAllowedBet == false);

			// If an allowed bet has been chosen, the computer will also perform its random bet.
			int opponentRandomBet = betArray[rand() % (sizeof(betArray) / sizeof(betArray[0]))];
			currentGamePot += opponentRandomBet;
			cout << "While you were placing your bet your opponent also placed his bet, he placed " << opponentRandomBet << " in the pot!" << endl;
			cout << "Your bet of " << userBet << " have been placed in the pot, the pot is now at: " << currentGamePot << endl;
			
			// Start the game

			cout << endl << "Alright! Lets get started!" << endl;

			// score[0] = PLAYER | score[1] = PC
			// Initialize variables
			int score[2] = { 0, 0 };
			int plyHighest = 0, pcHighest = 0;
			int roundCounter = 0;
			bool someoneWon = false;

			do {
				// Count the number of rounds played
				roundCounter++;
				cout << "Round " << roundCounter << endl;
				cout << "____________________________________________________" << endl;

				// Throw your two dices

				int dice_one_ply = rand() % 6 + 1;
				int dice_two_ply = rand() % 6 + 1;

				// Let the user know the dice rolls and decide highest dice
				cout << "Your first dice rolls a [" << dice_one_ply << "] and your second rolls a [" << dice_two_ply << "]" << endl;
				if (dice_one_ply > dice_two_ply) plyHighest = dice_one_ply;
				else plyHighest = dice_two_ply;

				// Throw opponent dices
				
				int dice_one_pc = rand() % 6 + 1;
				int dice_two_pc = rand() % 6 + 1;

				// Let the user know the opponents highest dice rolls and decide opponents highest dice
				cout << "Your opponents first dice rolls a [" << dice_one_pc << "] and the second rolls a [" << dice_two_pc << "]" << endl;
				if (dice_one_pc > dice_two_pc) pcHighest = dice_one_pc;
				else pcHighest = dice_two_pc;

				// Determine winner of round
				if (plyHighest == pcHighest) {
					cout << "You both rolled the same highest dice, we will play another round to determine the winner!" << endl;
				}
				else if (plyHighest > pcHighest)
				{
					score[0]++;
					cout << "You win the round because you rolled the single highest dice with: [" << plyHighest << "]" << endl;
				}
				else 
				{
					score[1]++;
					cout << "Your opponent wins the round because he scored the single highest dice with [" << pcHighest << "]" << endl;
				}

				// Check if someone has won the game
				for (int i = 0; i < 2; i++) if (score[i] == 2) someoneWon = true;
				cout << "____________________________________________________" << endl;
			} while (!someoneWon);

			cout << endl << endl;

			// Check who won the game
			if (score[0] == 2) {
				// Add money to account after win, and to the total winnings.
				gameAccount += currentGamePot;
				totalWinnings += currentGamePot;

				cout << "________________________________" << endl;
				cout << "|           YOU WON            |" << endl;
				cout << "________________________________" << endl;
				cout << endl << endl;
				cout << "You have won the game with a score of [" << score[0] << "-" << score[1] << "]" << endl;
				cout << currentGamePot << " have been added to your account putting your current account balance at: " << gameAccount << endl;
			}
			else 
			{
				// Remove money from the total winnings.
				totalWinnings -= userBet;

				cout << "________________________________" << endl;
				cout << "|           YOU LOST           |" << endl;
				cout << "________________________________" << endl;
				cout << endl << endl;
				cout << "You have lost, your bet of " << userBet << " and the rest of the pot have gone to your opponent." << endl <<
						"Your opponent won with a score of [" << score[1] << " - " << score[0] << "]" << endl <<
						"Your new account balance is: " << gameAccount << endl;
				
				if (score[0] == 1) cout << "Unlucky! You almost won, it could have gone either way seeing how close the game was!" << endl;
				else cout << "Arggh! Nice game, good luck in future games! :)" << endl;

				cout << endl << endl;
			}

			// Count the amount of games that have been played.
			gamesCounter++;

			// Let the user know its total amount won or lost in their total amount of games played. (Does not show when only one game has been played)
			if (wantToPlayAgain && totalWinnings > 0) {
				cout << "In the " << gamesCounter << " games you have played, you have WON a total of: " << totalWinnings << endl;
			}
			else if (wantToPlayAgain && totalWinnings < 0) 
			{
				cout << "In the " << gamesCounter << " games you have played, you have LOST a total of: " << to_string(totalWinnings).substr(1) << endl;
			}

			cout << endl << endl;

			// Ask to play again
			wantToPlayAgain = AskQuestion("Would you like to play again?");
		} while (wantToPlayAgain);
	}

	return 0;
}

// Takes a withdraw from the players game account
int TakeWithdraw(int account, int low) {

	// Makes sure the withdrawed amount will not go below the lowest bet.
	int moneyWithdrawable = account - low;
	int amount;
	
	cout << "You can withdraw anything between 0-" << moneyWithdrawable << endl;
	cout << "How much would you like to withdraw?" << endl;

	// Makes sure you input a valid amount
	do {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cin >> amount;
	} while (!cin.good() || (amount > moneyWithdrawable || amount < 0));
	
	// Lets you know your new account balance
	cout << "Your new account balance is: " << (account - amount) << endl;
	return amount;
}

// Takes a deposit to the players game account
int TakeDeposit(int account, int low, int max) {

	// Makes sure the deposited money is always enough to perform the lowest bet.
	int moneyNeeded = low - account;
	int amount = 0;

	cout << "You can deposit anything between " << (account < low ? moneyNeeded : 0) << "-" << max << ", your current account balance is: " << account << endl;
	cout << "In order to make the lowest bet you will need to deposit: " << (account < low ? moneyNeeded : 0) << endl;
	cout << "How much would you like to deposit?" << endl;

	// Makes sure you input a valid amount
	do {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cin >> amount;
	} while (!cin.good() || (amount < moneyNeeded || amount > max));

	// Lets you know your new account balance
	cout << "Your new account balance is: " << (account + amount) << endl;
	return amount;
}

// Asks the user a question
bool AskQuestion(string question, string opt1, string opt2) {
	int option;
	// Sets up the question with the two different options, options and question depend on what was entered on function call
	cout << question << endl << "1. " << opt1 << endl << "2. " << opt2 << endl;
	cin >> option;

	// Makes sure you input a valid option
	while (!cin.good() || (option < 1 || option > 2)) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cin >> option;
	}

	// Checks if the option was 1 or 2 and returns accordingly.
	if (option == 1) return true;
	else if (option == 2) return false;
}