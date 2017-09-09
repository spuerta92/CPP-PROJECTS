// Sebastian Puerta Hincapie
// Crazy Eight

#define DEBUG
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <cassert>
#include <ctime>

using namespace std;

class Card {
  char m_suit;       // card suit, either 'S', 'C', 'D', 'H'.
  int  m_num;        // 1 through 13.
public:
  Card(const char s = '\0', const int n = 0) : m_suit(s), m_num(n) {}
  char suit() const { return m_suit; }
  int num() const { return m_num; }
  bool operator<(const Card& lhs) const { return m_num < lhs.m_num; }
  friend ostream& operator<<(ostream& o, const Card& c);	// output operator overload
  friend bool operator!=(const Card& rhs, const Card& lhs);	// operator overload to compare the suits
  friend bool operator==(const int& rhs, const Card& lhs);	// operator overload to compare the ranks
  friend bool operator==(const Card& rhs, const Card& lhs); // operator overload used on the "PLAYER'S TURN"
  friend bool operator==(const char& rhs, const Card& lhs);	// operator overload to compare suit characters
};
ostream& operator<<(ostream& o, const Card& c) {
  o << c.m_suit;
  switch(c.m_num) {
  case 0:  o << '*'; break; // used to show wild number.
  case 1:  o << 'A'; break; // Ace.
  case 11: o << 'J'; break; // Jack.
  case 12: o << 'Q'; break; // Queen.
  case 13: o << 'K'; break; // King
  default: o << c.m_num;
  }
  return o;
}
bool operator!=(const Card& rhs, const Card& lhs){
	return rhs.m_suit != lhs.m_suit;
}
bool operator==(const int& rhs, const Card& lhs){
	return rhs == lhs.m_num;
}
bool operator==(const Card& rhs, const Card& lhs){
	return rhs.m_num == lhs.m_num || rhs.m_suit == lhs.m_suit;
}
bool operator==(const char& rhs, const Card& lhs){
	return rhs == lhs.m_suit;
}


class Player {
  vector<Card> hand; // holding cards
public:
  Player() {}        // default Constructor.
  void push_back(const Card& c) { hand.push_back(c); }
  bool empty() { return hand.empty();}
  int size() const { return hand.size(); }
  Card remove(const int p) { // simply remove the specified position.
    Card c = hand[p];
    hand[p] = hand.back();   // swap with the tail of vector.
    hand.pop_back();         // then remove the tail element.
    return c;                // the one removed will be turned back.
  }
  // other methods come here...
  friend ostream& operator<<(ostream& o, const Player& p) {
    for (int i = 0, len = p.hand.size(); i < len; ++i)
      o << setw(3) << '(' << (char)('a' + i) << ") " << p.hand[i];
    return o;
  }
  Card operator[](const int idx) const {return hand[idx];}	// operator overload for vector indicing

};

int main() {
  const int NumPlayers = 4; 	// total number of players
  const int NumInCards = 6;		// number of cards for each player at the beginning
  const int crazy_eight = 8;
  srand(time(0));

	// FOR PLAYER'S TURN PART
	vector<char> choices;	// vector containing initial choices
	for(int i = 0; i < 7; ++i){	// [a,b,c,d,e,f,g = draw] (including draw option)
		choices.push_back('a' + i);
	}

	// creating vectors in which to store the 'letter' choices of the  players
	vector<char> p1 = choices;
	vector<char> p2 = choices;
	vector<char> p3 = choices;
	vector<int> p;
	for(int i = 0; i < NumPlayers; ++i){
		p.push_back(i);
	}

	// Vector containing suits
	vector<char> suits;
	suits.push_back('S');
	suits.push_back('D');
	suits.push_back('H');
	suits.push_back('C');

  cout << "> You may exit the game at any point, by typing in the pound character (#)" << endl << endl;
  // DECK OF CARDS CREATION
  vector<Card> deck; // initialize a standard 52 card deck.
  for (int i = 1; i <= 13; ++i) deck.push_back(Card('S', i));
  for (int i = 1; i <= 13; ++i) deck.push_back(Card('C', i));
  for (int i = 1; i <= 13; ++i) deck.push_back(Card('D', i));
  for (int i = 1; i <= 13; ++i) deck.push_back(Card('H', i));
  random_shuffle(deck.begin(), deck.end()); // shuffles cards.

  // ASSIGNING CARDS TO PLAYERS
  vector<Player> players(NumPlayers); // Players in the game.
  for (int i = 0; i < NumPlayers; ++i){
	for (int j = 0; j < NumInCards; ++j) { // Each Player 6 cards in the beginning.
	  players[i].push_back(deck.back());	// assign cards to player from the back of the deck
	  deck.pop_back();		// deletes cards from the deck assigned to a player
	}
  }
  Card pile = deck.back();
  // condition in case the initial card is a wildcard
  while(crazy_eight == pile){
	  random_shuffle(deck.begin(),deck.end());
	  pile = deck.back();
	  if(crazy_eight != pile) break;
  }
  deck.pop_back();
  char which;

  // LOOP TO GO THROUGH THE GAME CYCLE
  while (true) {
	if (deck.size() <= 0) {
		cout << "No more cards, GAME OVER!" << endl;
		goto END; // easiest version to move out nested loop.
	}
	cout << "Pile has " << pile << " <--- your turn" << endl;
	bool I_played = false;
	bool u_check = false;

	// LOOP TO GO THROUGH USER OPTIONS
	while (true) {
		// Creating draw card option
		char draw = 'a' + players[0].size();
		cout << endl << players[0] << "  (" << draw << ") draw" << endl;

		// Prompt user to make a choice
		cout << "\nChoose a card: ";
		cin >> which;
		cout << endl;

		// If user chooses draw
		if (which == draw) {
			if (deck.size() <= 0) {	// first check if we have cards in the deck
				cout << "No more cards, GAME OVER!" << endl;
				goto END;				// if no game over
			}
			players[0].push_back(deck.back()); 	// if yes, assign the last card of the deck to user
			deck.pop_back();						// deletes that card from the deck
			u_check = true;
			cout << "You are drawing..." << endl;
			break;
		}
		// If user chooses a card from the options given
		else if (which >= 'a' && which < draw){
			Card select = players[0][which-'a'];		// The card that the player selected
			char suit;
			if(crazy_eight == select){					// if player selects a "wildcard" ( value of 8)
				cout << "Select new suit(S,D,H,C): "; cin >> suit;
				if(suit == 'S' || suit == 'D' || suit == 'H' || suit == 'C'){	// uppercase user suit selection
					pile = Card(suit,0);
					players[0].remove(which - 'a');		// remove card from the user's deck
					I_played = true;
					break;
				}
				else if(suit == 's' || suit == 'd' || suit == 'h' || suit == 'c'){	// lowercase user suit selection
					pile = Card(toupper(suit),0);
					players[0].remove(which - 'a');		// remove card from the user's deck
					I_played = true;
					break;
				}
				else {
					cout << "Wrong input!, try again" << endl;	// simply prompts you input new value that matches your options
					continue;
				}
			}
			else if(select == pile && I_played == false) break;			//	if you select a card with the same number as the one on the pile
			else if(pile != select && I_played == false){				//  if you select a suit that does not match the pile suit
				cout << "Invalid choice, card must be of the same suit or rank!" << endl;
				u_check = true;
				break;
			}
			break;
		}
		// condition in case user input is '?'
		else if (which == '?'){
			for(int i = 1; i < NumPlayers; ++i){
				cout << "Player " << i << ": " << players[i] << endl;
			}
			continue;
		}
		// condition to exit the game
		else if(which == '#'){
			cout << "You opted to exit the game...goodbye!" << endl;
			goto END;

		}
		// Condition in case of invalid user input
		else if(which < 'a' || which > draw){
			cout << "Invalid input! Try again..." << endl;
			u_check = true;
			break;
		}
	}
	if(u_check == true){	// condition that restarts the cycle(tailoring output purposes)
		continue;
	}
	if(I_played == false){	// if same suit or rank then apply this condition
		pile = players[0][which-'a'];
		players[0].remove(which - 'a');		// remove card from the user's deck
		//pile = players[0].remove(which - 'a');		// remove that card from the user's deck
		// condition in case user wins the game
	}
	if(players[0].empty()){
		cout << "YOU WON! CONGRATULATIONS" << endl;
		goto END;
	}

	// PLAYER'S TURN
	int i = 1;
	bool p_check = false;
	int d_count = 0;

	// LOOP TO GO THROUGH PLAYER'S OPTIONS
	while(i < NumPlayers){
		// creating 3 players to assign letter vectors and know which vector to update
		char p_which;
		Card player_select;
		if(p[i] == 1){
			p_which = p1[rand() % p1.size()];	// generates a letter choice for the player
			player_select = players[i][p_which - 'a'];
		}
		else if(p[i] == 2){
			p_which = p2[rand() % p2.size()];	// generates a letter choice for the player
			player_select = players[i][p_which - 'a'];
		}
		else if(p[i] == 3){
			p_which = p3[rand() % p3.size()];	// generates a letter choice for the player
			player_select = players[i][p_which - 'a'];
		}

		// Creating draw card option for players
		char p_draw = 'a' + players[i].size();	// would be the last card in the players hand


		// checks first if there any cards left in the deck
		if (deck.size() <= 0) {
			cout << endl << "No more cards, GAME OVER!" << endl;
			goto END; // easiest version to move out nested loop.
		}

		else if(players[i].empty()){
			cout << "Player " << i << " WON! Better luck next time..." << endl;
			goto END;
		}
		else if(p_which >= 'a' && p_which < p_draw){
			// if player select crazy eight
			if(crazy_eight == player_select){
				cout << "Pile has " << pile << endl;
				if(p_check == true){
					for(int j = 0; j < d_count; ++j){
						cout << setw(12) << "Player " << i << " drawing..." << endl;
					}
					p_check = false; d_count = 0;
				}
				char d_suit = suits[rand() % suits.size()];
				cout << setw(12) << "Player " << i << " chose " << player_select << endl;
				cout << setw(12) << "Player " << i << " declared suit " << d_suit << endl;
				pile = Card(d_suit,0);
				players[i].remove(p_which - 'a');
				// remove one of the choices
				if(p[i] == 1){p1.pop_back();}
				if(p[i] == 2){p2.pop_back();}
				if(p[i] == 3){p3.pop_back();}
				++i;
				continue;
			}
			else if(player_select == pile){		// if players selects a card that matches suit or number
				cout << "Pile has " << pile << endl;
				if(p_check == true){
					for(int j = 0; j < d_count; ++j){
						cout << setw(12) << "Player " << i << " drawing..." << endl;
					}
					p_check = false; d_count = 0;
				}
				cout << setw(12) << "Player " << i << " chose " << player_select << endl;
				pile = player_select;
				players[i].remove(p_which - 'a');
				// remove one of the choices
				if(p[i] == 1){p1.pop_back();}
				if(p[i] == 2){p2.pop_back();}
				if(p[i] == 3){p3.pop_back();}
				++i;
				continue;
			}
			else if(!(player_select == pile)) continue;	// if players selects a card that does not match suit or number
		}
		else if(p_which == p_draw) {
			if (deck.size() <= 0) {	// first check if we have cards in the deck
				cout << "No more cards, GAME OVER!" << endl;
				goto END;				// if no game over
			}
			players[i].push_back(deck.back());	// adds new card to player
			deck.pop_back();					// deletes that card from the deck

			// we want to add an extra choice for the players
			if(p[i] == 1){p1.push_back(char(p_draw + 1));}
			else if(p[i] == 2){p2.push_back(char(p_draw + 1));}
			else if(p[i] == 3){p3.push_back(char(p_draw + 1));}
			p_check = true;
			++d_count;
			continue;
		}
	}
  }
  // display all at the end of the game
  END:
  cout << "\n[ Final card distributions ]" << endl;
  for (int i = 0; i < NumPlayers; ++i) cout << "Player " << i << players[i] << endl << endl;
  cout << "Thanks for playing!" << endl;

  return 0;
}




