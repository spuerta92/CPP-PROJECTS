// Sebastian Puerta Hincapie
// Mexican Dice "Akira's Version"

#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

//random roll function
int roll() {return rand() % 6 + 1;} // rand() % 6 would give me [0,5] but adding the 1 changes the range to [1,6]
//This function concatenates the two values obtained from each roll
int value(int a, int b) {	// The greater value goes to a, the lesser value goes to b
	int times = 1;
	while(times <= b) {
		times *= 10;
	}
	return a*times + b;
}

int main(){
	srand(time(0));
	int comp_allow = 3000, user_allow = 3000;
	int comp_score = 0, user_score = 0, counter = 0;
	int Mexico = 21;
	char answer;

	cout << "===== MEXICA DICE =====" << endl << endl;
	// Iteration continues as long as the allowance > 0
	while(comp_allow > 0 && user_allow > 0){
		int comp_dice1 = roll();	// dice #1
		int comp_dice2 = roll();	// dice #2
		int user_dice1 = roll();	// dice #1
		int user_dice2 = roll();	// dice #2
		int probability = (rand() % 100 < 5); // 5% Probability of lie

		// Computer rolls
		if(comp_dice1 > comp_dice2) {comp_score = value(comp_dice1,comp_dice2);}
		else if (comp_dice1 < comp_dice2) {comp_score = value(comp_dice2,comp_dice1);}
		else {comp_score = comp_dice1 * 100;}	// if both values are the same, use either and mult by 100
		//User rolls
		if(user_dice1 > user_dice2) {user_score = value(user_dice1,user_dice2);}
		else if (user_dice1 < user_dice2) {user_score = value(user_dice2,user_dice1);}
		else {user_score = user_dice1 * 100;}	// if both values are the same, use either and mult by 100

		//Computer Mexico w/ 5% probability of lie
		if(probability) {
			cout << "Computer rolled Mexico!!!" << endl;
			cout << "Will you [c]hallenge or [a]accept: ";
			cin >> answer;

			while(answer != 'c' && answer != 'a') {
				cout << "Input Error -- type 'a' or 'c': ";
				cin >> answer;
			}

			if(answer == 'a') {
				comp_allow += 1000 - user_score;
				user_allow -= 1000 - user_score;
			}
			else {						// for the case of 'c' and checks if it is indeed "Mexico"
				if(comp_score == Mexico){
					cout << "Indeed MEXICO! you lose $1,500" << endl;
					comp_allow += 1500;
					user_allow -= 1500;
				}
				else {					// for the case of if not Mexico
					cout << "Hmm...How did you know! You win $1,500" << endl;
					comp_allow -= 1500;
					user_allow += 1500;
				}
			}
		}
		// Comparing scores
		else {							// for the case when we're not dealing with the probability
			if(user_score == Mexico){
				user_allow += 1000;
				cout << "You rolled MEXICO!!!" << endl;
			}
			else {						// for the cases when the user score is not 21 or "Mexico"
				if(comp_score > user_score){
					comp_allow += comp_score - user_score;
					user_allow -= comp_score - user_score;
				}
				else if(user_score > comp_score) {
					user_allow += user_score - comp_score;
					comp_allow -= user_score - comp_score;
				}
				else { cout << "It's a DRAW!!!" << endl;}
			}
		}
		// Results display
		cout << "Computer rolled: " << comp_score << endl;
		cout << "You rolled: " << user_score << endl;
		cout << "Computer Allowance: " << comp_allow << endl;
		cout << "Player Allowance: " << user_allow << endl;
		cout << endl;
		++counter;
	}
	cout << "Games played: " << counter << endl;			// ends faster if you keep 'c' challenging
	cout << "Bye...Thanks for playing!" << endl;			// exits out iteration
	return 0;
}



