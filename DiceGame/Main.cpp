#include <iostream>
#include <string>
#include <ctime>

using namespace std;
bool AskQuestion(string question, string opt1 = "Yes", string opt2 = "No");
int TakeDeposit(int account, int low, int max);
int TakeWithdraw(int account, int low);

int main() {
	srand(time(0));
	int const lowBet = 100, medBet = 300, highBet = 500, maxAmount = 5000;
	int gameAccount = 0, totalWinnings = 0, gamesCounter = 0;
	bool wantToPlayAgain = false;
	
	cout << "Hello and welcome to DiceThrower!\n" << endl;
	bool wantToStartGame = AskQuestion("Would you like to start the game? Reply with the corresponding number below.");

	if (wantToStartGame) {
		cout << "I am glad you wanted to play against me, lets get started!\n" << endl;
		do {
			if (gameAccount < lowBet) {
				cout << 
					"Before we get started, you will need some money on your account in order to make bets during the game." << endl <<
					"The different bets you can make in the game are these: [" << lowBet << ", " << medBet << ", " << highBet << "]\n" << 
				endl;
				gameAccount += TakeDeposit(gameAccount, lowBet, maxAmount);
			}
			else
			{
				bool wantToManageAccount = AskQuestion("Before we get started, would you like to manage your account?");

				if (wantToManageAccount) {
					bool wantToDeposit = AskQuestion("Which action would you like to take?", "Deposit", "Withdraw");
					
					if (wantToDeposit) gameAccount += TakeDeposit(gameAccount, lowBet, maxAmount);
					else gameAccount -= TakeWithdraw(gameAccount, lowBet);
				}
			}

			// Place bets before starting the game [YOU & PC]

			string availableBets;
			bool isAllowedBet = false;
			int betArray[3] = {lowBet, medBet, highBet};
			int allowedBets[3];
			int currentGamePot = 0;
			int userBet;

			for (int i = 0; i < sizeof(betArray) / sizeof(betArray[0]); i++) {
				if (gameAccount >= betArray[i]) {
					availableBets += (i > 0 ? ", " + to_string(betArray[i]) : to_string(betArray[i]));
					allowedBets[i] = betArray[i];
				}	
			}

			cout << "Please place a bet. You can choose between the following: [" << availableBets << "]" << endl;

			do {
				cin.clear();
				cin.ignore(INT_MAX, '\n');
				cin >> userBet;

				for (int i = 0; i < sizeof(allowedBets) / sizeof(allowedBets[0]); i++) {
					if (userBet == allowedBets[i]) {
						gameAccount -= userBet;
						currentGamePot += userBet;
						isAllowedBet = true;
						break;
					}
				}
			} while (!cin.good() || isAllowedBet == false);

			int opponentRandomBet = betArray[rand() % (sizeof(betArray) / sizeof(betArray[0]))];
			currentGamePot += opponentRandomBet;
			cout << "While you were placing your bet your opponent also placed his bet, he placed " << opponentRandomBet << " in the pot!" << endl;
			cout << "Your bet of " << userBet << " have been placed in the pot, the pot is now at: " << currentGamePot << endl;
			
			// Start the game

			cout << endl << "Alright! Lets get started!" << endl;

			// score[0] = PLAYER | score[1] = PC
			int score[2] = { 0, 0 };
			int plyHighest = 0, pcHighest = 0;
			int roundCounter = 0;
			bool someoneWon = false;

			do {
				roundCounter++;
				cout << "Round " << roundCounter << endl;
				cout << "____________________________________________________" << endl;

				// Throw your two dices

				int dice_one_ply = rand() % 6 + 1;
				int dice_two_ply = rand() % 6 + 1;

				cout << "Your first dice rolls a [" << dice_one_ply << "] and your second rolls a [" << dice_two_ply << "]" << endl;
				if (dice_one_ply > dice_two_ply) plyHighest = dice_one_ply;
				else plyHighest = dice_two_ply;

				// Throw opponent dices
				
				int dice_one_pc = rand() % 6 + 1;
				int dice_two_pc = rand() % 6 + 1;

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

				for (int i = 0; i < 2; i++) if (score[i] == 2) someoneWon = true;
				cout << "____________________________________________________" << endl;
			} while (!someoneWon);

			cout << endl << endl;

			if (score[0] == 2) {
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
				totalWinnings -= userBet;

				cout << "________________________________" << endl;
				cout << "|           YOU LOST           |" << endl;
				cout << "________________________________" << endl;
				cout << endl << endl;
				cout << "You have lost, your bet of " << userBet << " and the rest of the pot have gone to your opponent." << endl <<
						"Your opponent won with a score of[" << score[1] << " - " << score[0] << "]" << endl <<
						"Your new account balance is: " << gameAccount << endl;
				
				if (score[0] == 1) cout << "Unlucky! You almost won, it could have gone either way seeing how close the game was!" << endl;
				else cout << "Arggh! Nice game, good luck in future games! :)" << endl;

				cout << endl << endl;
			}

			gamesCounter++;

			if (wantToPlayAgain && totalWinnings > 0) {
				cout << "In the " << gamesCounter << " games you have played, you have WON a total of: " << totalWinnings << endl;
			}
			else if (wantToPlayAgain && totalWinnings < 0) 
			{
				cout << "In the " << gamesCounter << " games you have played, you have LOST a total of: " << totalWinnings << endl;
			}

			cout << endl << endl;
			wantToPlayAgain = AskQuestion("Would you like to play again?");
		} while (wantToPlayAgain);
	}

	return 0;
}

int TakeWithdraw(int account, int low) {
	int moneyWithdrawable = account - low;
	int amount;

	cout << "You can withdraw anything between 0-" << moneyWithdrawable << endl;
	cout << "How much would you like to withdraw?" << endl;

	do {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cin >> amount;
	} while (!cin.good() || (amount > moneyWithdrawable || amount < 0));
	
	cout << "Your new account balance is: " << (account - amount) << endl;
	return amount;
}

int TakeDeposit(int account, int low, int max) {
	int moneyNeeded = low - account;
	int amount = 0;

	cout << "You can deposit anything between " << (account < low ? moneyNeeded : 0) << "-" << max << ", your current account balance is: " << account << endl;
	cout << "In order to make the lowest bet you will need to deposit: " << (account < low ? moneyNeeded : 0) << endl;
	cout << "How much would you like to deposit?" << endl;

	do {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cin >> amount;
	} while (!cin.good() || (amount < moneyNeeded || amount > max));

	cout << "Your new account balance is: " << (account + amount) << endl;
	return amount;
}

bool AskQuestion(string question, string opt1, string opt2) {
	int option;
	cout << question << endl << "1. " << opt1 << endl << "2. " << opt2 << endl;
	cin >> option;

	while (!cin.good() || (option < 1 || option > 2)) {
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cin >> option;
	}

	if (option == 1) return true;
	else if (option == 2) return false;
}