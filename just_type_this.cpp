// Sebastian Puerta Hincapie
// JUST TYPE THIS, AKIRA'S VERSION 

#include <iostream>
#include <chrono>
#include <cstdlib>	// rand()
#include <ctime>	// srand(time(0))
#include <vector>	// vector
#include <string>	// string

using namespace std;
using namespace chrono;

// rand function that generates a random string
// ASCII uppercase: 65-90, ASCII lowercase: 97-122
string rand_generate(int len_word, vector<char> vl){
	srand(time(0));
	string word;
	for(int k = 0; k < len_word; ++k){
		char a_letter = vl[rand() % 52];
		word += a_letter;
	}
	return word;
}

// Penalty function (when the user mistypes the words)
int penalty(string r_word, string u_word){
	int value = 0;
	// conditions to make sure that we always loop using the size of the larger string to ensure correct offset
	if(r_word.size() > u_word.size()){
		for(unsigned i = 0; i < r_word.size(); ++i){value += abs(int(r_word[i]) - int(u_word[i]));}
	}
	else if(r_word.size() < u_word.size()){
		for(unsigned i = 0; i < u_word.size(); ++i){value = abs(int(r_word[i]) - int(u_word[i]));}
	}
	return value;
}

int change_ws(int ws, int s, int f){
	// After three successful rounds regardless of the interval, the length of the word increases by one and the interval decreases by -250msec
	if(s == 3) {ws += 1;}
	// After three consecutive fails, the length of the word decreases by one and the interval increases by +250msec
	else if(f == 3) {ws -= 1;}
	return ws;
}
int change_ti(int ti, int s, int f){
	// After three successful rounds regardless of the interval, the length of the word increases by one and the interval decreases by -250msec
	if(s == 3) {ti -= 250;}
	// After three consecutive fails, the length of the word decreases by one and the interval increases by +250msec
	else if(f == 3) {ti += 250;}
	return ti;
}

int main() {
	// Initialization
	int user_points = 1000, word_size = 5, time_interval = 5000, offset = 0, success = 0, fail = 0;
	string user_word, rand_word;
	vector<char> v_letters;

	// To add uppercase letters into the vector
	for(unsigned i = 0; i < 26; ++i){
		char upper_case = 'A' + i;
		v_letters.push_back(upper_case);
	}
	// To add lowercase letters into the vector
	for(unsigned i = 0; i < 26; ++i){
		char lower_case = 'a' + i;
		v_letters.push_back(lower_case);
	}

	// loop for the continuation of the game
	while(user_points > 0){
		rand_word = rand_generate(word_size,v_letters);	// calls the function to create a random word
		// output
		cout << "Time Interval: " << time_interval << ", " << "Word Size: " << word_size << endl;
		cout << "Your current points: " << user_points << ", "
			 << "just type [" << word_size << "]" << " -> " << rand_word << ": ";
		auto t1 = system_clock::now();	// start time
		cin >> user_word;				// reading
		auto t2 = system_clock::now();	// end time
		auto type_time = duration_cast<milliseconds>(t2-t1).count();	// total reading time

		if(user_word == "end"){break;}	// condition to exit game

		cout << type_time << " milliseconds, you made it within the interval 5000...\n";
		cout << endl;

		// if you type the word accurately and within the interval you win 100 points
		if(type_time < time_interval && user_word == rand_word){
			user_points += 100;	// add 100 points to your previous score
			++success;			// counts consecutive successes
			fail = 0;			// resets fail to zero
			if(success == 2) {cout << "One more for the next challenge!!!" << endl << endl;;}// if two consecutive success, output a message
			word_size = change_ws(word_size,success, fail);	// calls function to check if there have been 3 success or fails and changes the word accordingly
			time_interval = change_ti(time_interval,success,fail);// calls function to check if there have been 3 success or fails and changes the interval accordingly
			if(success == 3){success = 0;}	// resets the success rate to start cycle again
		}

		// if you type the word accurately but not within the interval your score will stay the same
		else if(type_time > time_interval && user_word == rand_word){
			user_points += 0;	// score stays the same
			++success;
			fail = 0;
			if(success == 2) {cout << "One more for the next challenge!!!" << endl;}
			word_size = change_ws(word_size,success, fail);
			time_interval = change_ti(time_interval,success,fail);
			if(success == 3){success = 0;}	// resets the success rate

		}

		// if you do not type the word accurately but within the interval you get penalized by the offset
		else if(type_time < time_interval && user_word != rand_word){
			offset = penalty(rand_word,user_word);
			user_points -= offset;	// score minus the total offset
			++fail;					// counts consecutive fails
			success = 0;			// resets success since you obviously failed
			word_size = change_ws(word_size,success, fail);
			time_interval = change_ti(time_interval,success,fail);
			if(fail == 3){fail = 0;}	// resets the fail rate to start cycle again
		}

		// if you do not type the word accurately and also not within the interval you get penalized by 2xoffset
		else if(type_time > time_interval && user_word != rand_word){
			offset = penalty(rand_word,user_word) * 2;
			user_points -= offset;	// score minus twice the total offset
			++fail;
			success = 0;
			word_size = change_ws(word_size,success, fail);
			time_interval = change_ti(time_interval,success,fail);
			if(fail == 3){fail = 0;}
		}
	}
	cout << "Thanks for playing!" << endl;	// outputs end of game
	return 0;
}



