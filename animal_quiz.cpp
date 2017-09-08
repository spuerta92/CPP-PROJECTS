// Sebastian Puerta Hincapie
// Animal Quiz "Akira's Version"

#if 0

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

void get_animals(const string& an, vector<string>& v_an);
string shuffled(int r, vector<string>& vs, vector<char>& vl);
string sorted(vector<char>& vl);
void check(const string& s1, const string& s2, int& w, int& f);
string remove_one(int r, vector<string>& vs, vector<char>& vl);
string remove_two(int r, vector<string>& vs, vector<char>& vl);

int main() {
	string animal;
	vector<string> v_animal, v_select, v_answer;
	vector<char> v_letters;
	int success = 0, miss = 0, temp = 0;

	cout << "Enter at least give animal names, e.g., cat, dog ,etc..." << endl;

	// loop to read in animals
	while(true){
		cout << "-> ";
		getline(cin,animal);
		if(animal == "") {
			cout << "Try again!" << endl;
			continue;
		}
		get_animals(animal,v_animal);
		break;
	}

	cout << endl;

	// loop to display animals
	for(unsigned i = 0; i < v_animal.size(); ++i){cout << i + 1 << ": " << v_animal.at(i) << endl;}
	cout << endl;

	// loop to play game
	while(true){
		srand(time(0));
		string answer, shuffle_word, sort_word, sort2_word;
		v_letters.clear(); v_select.clear(); v_answer.clear();

		// loop to randomly select 3 animals
		int range = rand() % 3 + 1;		// variable definition for the range [1,3]
		for(int i = 0; i < range; ++i){
			v_select.push_back(v_animal[rand() % v_animal.size()]);	// vector holding randomly selected animals
		}
		cout << endl;
		shuffle_word = shuffled(range, v_select, v_letters);	// produces random string from the animals selected
		sort_word = sorted(v_letters);		// sorts to compare with user input
		v_letters.clear();					// resets vector to avoid repetition of letters

		if(success >= 2 && success < 4){
			v_letters.clear();
			shuffle_word = remove_one(range, v_select, v_letters);
		}
		else if(success >= 4){
			v_letters.clear();
			shuffle_word = remove_two(range, v_select, v_letters);
			++temp;		// increments if success is 4 or greater
		}
		else if (miss == 1){
			if(temp != 0){	// if temp is not 0 then use this funtion to show results
				shuffle_word = remove_two(range, v_select, v_letters);
			}
			else {	// if temp 0 then use this funtion to show result
				shuffle_word = remove_one(range, v_select, v_letters);
			}
		}
		else if(miss >= 2){
			if(temp != 0){
				shuffle_word = remove_one(range, v_select, v_letters);
				temp = 0;	// resets temp counter
			}
			else {
				shuffle_word = shuffled(range, v_select, v_letters);
			}
		}
		cout << "What are the animals in " << "'" << shuffle_word << "' ? ";

		getline(cin,answer);
		if(answer == "?"){ // Typing "?" will show names
			for(unsigned i = 0; i < v_animal.size(); ++i){
				cout << i + 1 << ": " << v_animal.at(i) << endl;
			}
			cout << "Your input is incorrect --- try again!" << endl;
			continue;	// restarts the loop (starts the game over again)
		}
		if(answer == "quit"){break;} // Typing "quit" as the answer exits game
		get_animals(answer,v_answer);	// gets tokens from the string
		v_letters.clear();			// clear vector to be reused again
		shuffled(v_answer.size(), v_answer, v_letters); // creates random string from user input
		sort2_word = sorted(v_letters);		// sorts out the string the compare

		check(sort_word,sort2_word,success,miss);	// checks for consecutive success and miss
		cout << "Cons. Success: " << success << ", Cons. Miss: " << miss << endl;
	}

	cout << endl << endl << "Bye...Thanks for playing!" << endl;

	return 0;
}

void get_animals(const string& an, vector<string>& v_an){
	string word;
	bool found = false;
	// loop to go through each character
	for(unsigned i = 0; i < an.length(); ++i){
		if(!isspace(an[i])){ //non-space char found -- word gets constructed
			word += an[i];
			found = true;
			continue;
		}
		// a space is found
		if(!found) continue; // skip space if true, move onto next lineh if false
		v_an.push_back(word);
		word = "";	// resets the word
		found = false;
	}
	if(found) v_an.push_back(word); // added so that the last word doesn't get cut off. BE CAREFUL
}
string shuffled(int r, vector<string>& vs, vector<char>& vl){
	string cw = "", shuff = "";
	for(int i = 0; i < r; ++i){
		cw += vs.at(i);									// concatenate words into one string
	}
	for(unsigned i = 0; i < cw.length(); ++i){
		vl.push_back(cw[i]);				// enters each letter of the conc. word into the vector
	}
	random_shuffle(vl.begin(),vl.end()); // to randomly shuffle chars in the vector
	for(unsigned i = 0; i < vl.size(); ++i){
		shuff += vl[i];				 // creates a string with the shuffle chars from the vector
	}
	return shuff;
}
string sorted(vector<char>& vl){
	string srt;
	sort(vl.begin(),vl.end());
	for(unsigned i = 0; i < vl.size(); ++i){
		srt += vl[i];
	}
	return srt;
}
void check(const string& s1, const string& s2, int& w, int& f){
	if(s1 == s2){
		cout << "Yes!" << endl;
		++w;	// success increments
		f = 0;	// sets miss to 0
		if(w == 2){
			cout << "Succeeded two consecutive times, challenge goes up!" << endl;
		}
		else if(w == 4){
			cout << "Succeeded four consecutive times, challenge goes up!" << endl;
		}
	}
	else {
		cout << "Nope!" << endl;
		++f;	// miss increments
		w = 0;	// sets success to 0
		if(f == 2){
			cout << "Missed two consecutive times, challenge goes down!" << endl;
		}
	}
}
string remove_one(int r, vector<string>& vs, vector<char>& vl){
	srand(time(0));
	string cw = "", shuff = "";
	for(int i = 0; i < r; ++i){
		cw += vs.at(i);									// concatenate words into one string
	}
	for(unsigned i = 0; i < cw.length(); ++i){
		vl.push_back(cw[i]);				// enters each letter of the conc. word into the vector
	}
	for(int i = 0; i < 1; ++i){
		vl[rand() % vl.size()] = '_';
	}
	random_shuffle(vl.begin(),vl.end()); // to randomly shuffle chars in the vector
	for(unsigned i = 0; i < vl.size(); ++i){
		shuff += vl[i];				 // creates a string with the shuffle chars from the vector
	}
	return shuff;
}
string remove_two(int r, vector<string>& vs, vector<char>& vl){
	srand(time(0));
	string cw = "", shuff = "";
	for(int i = 0; i < r; ++i){
		cw += vs.at(i);									// concatenate words into one string
	}
	for(unsigned i = 0; i < cw.length(); ++i){
		vl.push_back(cw[i]);				// enters each letter of the conc. word into the vector
	}
	for(int i = 0; i < 2; ++i){
		vl[rand() % vl.size()] = '_';
	}
	random_shuffle(vl.begin(),vl.end()); // to randomly shuffle chars in the vector
	for(unsigned i = 0; i < vl.size(); ++i){
		shuff += vl[i];				 // creates a string with the shuffle chars from the vector
	}
	return shuff;
}
#endif

