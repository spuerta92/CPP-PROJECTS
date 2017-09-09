// Sebastian Puerta Hincapie
// One Man Battleship

#include <iostream>
#include <iomanip>
#include <vector>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <regex>
#include <cstdlib>
#include <ctime>

using namespace std;

// Global definitions for default settings
const int DefaultBoardSize = 10;           // default board size is 10 x 10.
const int DefaultMinShipSize = 2;          // default ship size is [2, 4].
const int DefaultMaxShipSize = 4;          // default ship size
const int DefaultShipCount = 6;            // default ship count in a map is 6.

enum class Command { INIT, SHOW, QUIT, BOMB };

class Cell {
  enum mark { HIT, MISS, NONE, COORDINATE };		// added an extra mark
  mark m_status; // indicating bombed, missed, or not selected.
  bool m_ship;   // indicating presence of ship.

public:
  Cell() : m_status(NONE), m_ship(false) {}
  void reset() { m_status = NONE; m_ship = false; }
  void setship() { m_ship = true; }
  void setmark() { m_status = m_ship? HIT : MISS; }	// if m_ship == true --> HIT, m_ship == false --> MISS
  bool isship() const { return m_ship; }
  bool ishit() const { return m_status == HIT; }
  bool ismarked() const { return !(m_status == NONE); }

  void setshow() { m_status = COORDINATE;}			// added method to display ships
  void sethit() { m_status = HIT; }				// added method to display hit ships

  friend ostream& operator<<(ostream& o, const Cell& c) {
    switch (c.m_status) {
    case HIT:  o << '@'; break;
    case MISS: o << '-'; break;
    case NONE: o << '.'; break;
    case COORDINATE: o << '*'; break;		// to display location of ship segments
    default: assert(false);
    }
    return o << ' ';
  }
};

class Board {
  string m_name;               // map name.
  int m_size;                  // n x n sea map size.
  vector<vector<Cell> > m_map; // 2D sea map.
  vector<vector<Cell> > m_show;	// 2D mirror map
  int m_target;                // number of ship places.
  int m_bombed;                // number of hit places.

public:
  Board(const string& n ="my map", const int s = DefaultBoardSize) :
    m_name(n), m_size(s), m_target(0), m_bombed(0) {
    vector<Cell> a_row(m_size);
    for (int i = 0; i < m_size; ++i){
    	m_map.push_back(a_row);		// actual game map
    	m_show.push_back(a_row);	// mirror map used for show
    }
    init();		// sets ship when a board object is created
  }
  int get_target() const { return m_target;}
  int get_bombed() const { return m_bombed;}

  void init(const int n = DefaultShipCount) { // Random generation of ships
	  // Reseting board
	  m_target = 0;
	  m_bombed = 0;
	  for(int row = 0; row < DefaultBoardSize; ++row){
		  for(int col = 0; col < DefaultBoardSize; ++col){
			  m_map[row][col].reset();		// actual map
			  m_show[row][col].reset();		// mirror map to show location of all ships
		  }
	  }
	  // Setting the ships
	  int v = 1, h = 2;
	  int rand_size = 0, rand_start = 0, i = 0;
	  while(i < n){
		  int v_or_h = rand() % 2 + 1;			// random choice 1) vertical, 2) horizontal
		  // Vertical Ships
		  if(v_or_h == v){
			  rand_size = rand() % 3 + 2;		// random generation of ship size [2,4]
			  rand_start = rand() % DefaultBoardSize;		// random location for ship [0,9]
			  int col = 0;
			  while(col < rand_size){			// loop to create ship
				  if(m_map[rand_start + col][rand_start].isship() == false){	// checks if there is already a ship
					  if(rand_start < 7){
						  m_map[rand_start + col][rand_start].setship();
						  m_show[rand_start + col][rand_start].setshow();		// SHOW
						  ++m_target;
						  ++col;
					  }
					  else if(rand_start == 7 && rand_size < 4){
						  m_map[rand_start + col][rand_start].setship();
						  m_show[rand_start + col][rand_start].setshow();		// SHOW
						  ++m_target;
						  ++col;

					  }
					  else if(rand_start == 8 && rand_size < 3){
						  m_map[rand_start + col][rand_start].setship();
						  m_show[rand_start + col][rand_start].setshow();		// SHOW
						  ++m_target;
						  ++col;
					  }
					  else { break; }

					  if(col == rand_size){
						  ++i;
					  }
				  }
				  else {break;}		// if there is already a ship there, break out and loop over again
			  }
		  }
		  // Horizontal Ships
		  if(v_or_h == h){
			  rand_size = rand() % 3 + 2;
			  rand_start = rand() % 10;
			  int row = 0;
			  while(row < rand_size){
				  if(m_map[rand_start][rand_start + row].isship() == false){
					  if(rand_start < 7){
						  m_map[rand_start][rand_start + row].setship();
						  m_show[rand_start][rand_start + row].setshow();
						  ++m_target;
						  ++row;
					  }
					  else if(rand_start == 7 && rand_size < 4){
						  m_map[rand_start][rand_start + row].setship();
						  m_show[rand_start][rand_start + row].setshow();
						  ++m_target;
						  ++row;
					  }
					  else if(rand_start == 8 && rand_size < 3){
						  m_map[rand_start][rand_start + row].setship();
						  m_show[rand_start][rand_start + row].setshow();
						  ++m_target;
						  ++row;
					  }
					  else { break; }

				  	  if(row == rand_size){
						  ++i;
					  }
				  }
				  else {break;}
			  }
		  }
	  }
	  cout << "          ========== <ONE MAN BATTLE SHIP> ==========          " << endl;
	  cout << "There are " << i << " ships. You must bomb " << m_target << " places to destroy all..." << endl << endl;
  }

  bool bomb(const int row, const int col) {  // bomb the ship!
	bool ship = m_map[row][col].isship();
	bool marked = m_map[row][col].ismarked();

	// if it is already marked
	if(marked == true){
		cout << "You hit the same place...";
		return false;
	}
	// if there is no ship (miss)
	if(ship == false){
		m_map[row][col].setmark();
		return false;
	}
	// there is a ship (hit)
	m_map[row][col].setmark();
	++m_bombed;
	return true;
  }

  bool isover() const { return m_target <= m_bombed; }

  void show_board() {
      cout << "  <<" << m_name << ">>" << "\t\t\t<---SHIP ARRANGEMENT" << endl << "   ";
      for (int row = 0; row < m_size; ++row) cout << (char)('A' + row) << ' ';
      cout << endl;
      for (int row = 0; row < m_size; ++row) {
        for (int col = 0; col < m_size; ++col) {
        	if (!col) cout << setw(2) << right << 1 + row << ' ';
        	if(m_map[row][col].ishit()){
        		m_show[row][col].sethit();
        	}
        	cout << m_show[row][col];
        }
        cout << endl;
      }
  }

  friend ostream& operator<<(ostream& o, const Board& m) {
    o << "  <<" << m.m_name << ">>" << "\t\t\t<--- CURRENT STATUS" << endl << "   ";
    for (int row = 0; row < m.m_size; ++row) o << (char)('A' + row) << ' ';
    o << endl;
    for (int row = 0; row < m.m_size; ++row) {
      for (int col = 0; col < m.m_size; ++col) {
		if (!col) o << setw(2) << right << 1 + row << ' ';
		o << m.m_map[row][col];
      }
      o << endl;
    }
    return o;
  }
};

/* Input parsing logic for this application. This one uses so-called "regular
   expressions" to prune out inconsistent input (i.e., syntactic check) at
   first and then apply more application-related inspections (semantic check).
   Note that there are three return values - a return value and two non-const
   references in the parameter list. */

Command parse(const int size, int& row, int& col) {
  cout << "\nEnter position, \"show\", \"init\" or \"quit\": ";
  string line;
  regex pattern1("show|init|quit");          // one word input.
  regex pattern2("[0-9]+[[:blank:]]+[a-z]"); // two words with space(s) between.

  while (getline(cin, line)) { // read a whole line and convert to lower cases.
	cout << endl;
    transform(line.begin(), line.end(), line.begin(), ::tolower);
    if (regex_match(line, pattern1)) {       // match one word input?
      if (line.find("init") != string::npos) return Command::INIT;
      if (line.find("show") != string::npos) return Command::SHOW;
      if (line.find("quit") != string::npos) return Command::QUIT;
    } // you can use enum if you want above instead of a number return.

    if (regex_match(line, pattern2)) {       // match two word input?
      string c, r;
      istringstream iss(line);               // then read int two words.
      iss >> c >> r;                         // stringstream is the one to use!
      assert(!iss.fail());                   // this will not happen (why?)
      row = atoi(c.c_str()) - 1;             // apply semantic check for col, atoi() converts string to integer
      col = r[0] - 'a';                      // and row values.
      if (row >= 0 && row < size && col >= 0 && col < size)
	return Command::BOMB;
    }
    cout << "Input error, try again: ";
  }
  assert(false);                             // to make sure not to reach here.
}

int main() {
  srand(time(0));
  const int size = DefaultBoardSize;         // instantiate a board object with
  Board b("Pacific Ocean", size);            // game (war) name and board size.

  while (true) {
    cout << b << endl;								//displays the grid (game board)
    cout << "> TARGETS: " << b.get_target() << "  BOMBED: " << b.get_bombed() << endl;	// displays game status
    int row, col;
    bool check = false;
    switch (parse(size, row, col)) {
    case Command::QUIT: goto END;            // "quit" is to end the game.
    case Command::SHOW: check = true; break;   // "show" is to show the answer
    case Command::INIT: b.init(); continue;  // "init" is to restart the game.
    case Command::BOMB: break;               // other correct input.
    default: abort();                        // should not reach.
    }
    if(check == true){
    	b.show_board();
    	cout << endl;
    	continue;
    }
    bool result = b.bomb(row, col);               // otherwise do the game...
    cout << "You " << (result? "hit! :)" : "missed! :(") << endl << endl;

    if (b.isover()) {
      cout << "*****You have bombed all the ships, congratulations!*****" << endl << endl;
      break;
    }
  }

 END:
  cout << "Thanks for playing! goodbye..." << endl;
  return 0;
}


