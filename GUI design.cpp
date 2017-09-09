// Sebastian Puerta Hincapie
// GUI Design

#define WIN32
#include "Simple_window.h"
#include "Graph.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace Graph_lib;

Simple_window win{Point{0,0},600,600,"Programming Assignment #6"};

void set_squares(Graph_lib::Rectangle& r){
	r.set_fill_color(Color::dark_red);		// colors in the square
	r.set_color(Color::invisible);		// gets rid of the border lines around the squares
	win.attach(r);		// attaches square to the window
}
void set_overlaps(Graph_lib::Rectangle& r){
	r.set_fill_color(Color::white);
	r.set_color(Color::invisible);
	win.attach(r);
	cout << "Overlapping..." << endl;
}

void display(){cout << "Whoops bad square! X: 0 or Y: 0, please enter a new input..." << endl;}

int main(){
	srand(time(0));
	int line_length = 0, rand_pos = 0;
	cout << "> Please enter a line length, line length must be (0,300)" << endl;	//in location is 299, the square will still fit within the window
	cout << "> If line length is zero or negative, the program will end" << endl;
	cout << "> If input is too big, it will simply ask you for a new input" << endl << endl;

	while(true){
		cout << "> Line Length: ";
		cin >> line_length;

		if(line_length <= 0) break;
		else if(line_length >= 300){
			cout << "Value is too large, try again...(0,300)" << endl;
			continue;
		}
		else if(line_length > 0 && line_length < 300){
			// SQUARE #1
			rand_pos = rand() % 301; // if line length is <= 300 it will still fit in the window
			int x1 = rand_pos, y1 = rand_pos;	// upper left most corner of square
			int x2 = rand_pos + line_length, y2 = rand_pos + line_length;	// lower right most corner of square
			Graph_lib::Rectangle r1{Point{rand_pos,rand_pos},line_length,line_length};	// creates the square
			set_squares(r1);

			// SQUARE #2
			rand_pos = rand() % 301;
			int x3 = rand_pos, y3 = rand_pos;
			int x4 = rand_pos + line_length, y4 = rand_pos + line_length;
			Graph_lib::Rectangle r2{Point{rand_pos,rand_pos},line_length,line_length};
			set_squares(r2);

			// SQUARE #3
			rand_pos = rand() % 301;
			int x5 = rand_pos, y5 = rand_pos;
			int x6 = rand_pos + line_length, y6 = rand_pos + line_length;
			Graph_lib::Rectangle r3{Point{rand_pos,rand_pos},line_length,line_length};
			set_squares(r3);

			// Overlapping creation
			if(x1 == x4 || y1 == y4){display();continue;}
			Graph_lib::Rectangle overlap1{Point{x1,y1},abs(x4-x1),abs(y4-y1)};
			if(x2 == x3 || y2 == y3){display();continue;}
			Graph_lib::Rectangle overlap2{Point{x3,y3},abs(x2-x3),abs(y2-y3)};
			if(x1 == x6 || y1 == y6){display();continue;}
			Graph_lib::Rectangle overlap3{Point{x1,y1},abs(x6-x1),abs(y6-y1)};
			if(x2 == x5 || y2 == y5){display();continue;}
			Graph_lib::Rectangle overlap4{Point{x5,y5},abs(x2-x5),abs(y2-y5)};
			if(x3 == x6 || y3 == y6){display();continue;}
			Graph_lib::Rectangle overlap5{Point{x3,y3},abs(x6-x3),abs(y6-y3)};
			if(x4 == x5 || y4 == y5){display();continue;}
			Graph_lib::Rectangle overlap6{Point{x5,y5},abs(x4-x5),abs(y4-y5)};

			//condition for r1 and r2
			if(x1 <= x4 && x2 >= x3 && y1 <= y4 && y2 >= y3){
				if(x1 > x3 && y1 > y3){set_overlaps(overlap1);}
				else if(x1 < x3 && y1 < y3){set_overlaps(overlap2);}
			}
			//condition for r1 and r3
			if(x1 <= x6 && x2 >= x5 && y1 <= y6 && y2 >= y5){
				if(x1 > x5 && y1 > y5){set_overlaps(overlap3);}
				else if(x1 < x5 && y1 < y5){set_overlaps(overlap4);}
			}
			//condition for r2 and r3
			if(x3 <= x6 && x4 >= x5 && y3 <= y6 && y4 >= y5){
				if(x3 > x5 && y3 > y5){set_overlaps(overlap5);}
				else if(x3 < x5 && y3 < y5){set_overlaps(overlap6);}
			}
			// Display window
			win.wait_for_button();

			// Detaching
			win.detach(r1); win.detach(r2); win.detach(r3);
			win.detach(overlap1); win.detach(overlap2); win.detach(overlap3);
			win.detach(overlap4); win.detach(overlap5); win.detach(overlap6);
		}
	}
	cout << endl << "Goodbye.." << endl;
	return 0;
}



