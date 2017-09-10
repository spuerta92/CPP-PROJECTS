// Sebastian Puerta 
// Graphic Object Manipulation with GUI

#define WIN32
#include "Simple_window.h"
#include "Window.h"
#include "Graph.h"
#include "GUI.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;
using namespace Graph_lib;

const int x = 1000; const int y = 600;
const int rec_pos = 200; const int text_x = 400; const int text_y = 300; // Window Size
const float k = .05;	// size change percentage
const float pi = 3.1415926535897; // value of PI
const float angle = pi/6; // angle of rotation

class Rectangle_window : Graph_lib::Window {
private:
	// Widgets
	Button quit_button, redraw_button, menu_button;
	Menu motion_menu;
	bool button_pushed;
	bool exp = false, shr = false, rot = false;
	vector<Shape*> v_shapes;

public:
	Rectangle_window(Point xy,int w, int h, const string& title) :
		Window{xy,w,h,title},
		quit_button{Point{x_max()-70,0},70,20,"Quit",cb_quit},
		redraw_button{Point{x_max()-140,0},70,20,"Redraw",cb_redraw},
		motion_menu{Point{x_max()-70,40},70,20,Menu::vertical,"Motions"},
		menu_button{Point{x_max()-90,40},90,20,"Motion Menu",cb_menu}
	{
		attach(quit_button);
		attach(redraw_button);
		motion_menu.attach(new Button{Point{0,0},0,0,"Expand",cb_expand});
		motion_menu.attach(new Button{Point{0,0},0,0,"Shrink",cb_shrink});
		motion_menu.attach(new Button{Point{0,0},0,0,"Rotate",cb_rotate});
		attach(motion_menu);
		motion_menu.hide();
		attach(menu_button);
		init();
	}
	bool get_pushed() const { return button_pushed;}
	bool get_exp() const { return exp;}
	bool get_shr() const { return shr;}
	bool get_rot() const { return rot;}
	void set_exp(bool c) {exp = c;}
	void set_shr(bool c) {shr = c;}
	void set_rot(bool c) {rot = c;}
	void init(){
		// makes sure that the vector is cleared out
		for(unsigned i = 0; i < v_shapes.size(); ++i){
			delete(v_shapes[i]);
		}
		v_shapes.clear();
	}
	void change(Color c){
		for(unsigned i = 0; i < v_shapes.size(); ++i){
			v_shapes[i]->set_color(c);
		}
	}
	void hide_menu(){
		motion_menu.hide();
		menu_button.show();
	}

	// actions invoked by callbacks
	void quit_pressed(){
		cout << "Goodbye..." << endl;
		hide();
	}
	void redraw_pressed(){
		cout << "Redraw..." << endl;
		set_exp(false); set_shr(false); set_rot(false);
		button_pushed = true;
	}

	void expand_pressed(){
		cout << "You pressed expand" << endl;
		set_exp(true); set_shr(false); set_rot(false);
		button_pushed = true;
		hide_menu();
	}
	void shrink_pressed(){
		cout << "You pressed shrink" << endl;
		set_shr(true); set_exp(false); set_rot(false);
		button_pushed = true;
		hide_menu();
	}
	void rotate_pressed(){
		cout << "You pressed rotate" << endl;
		set_rot(true); set_exp(false); set_shr(false);
		button_pushed = true;
		hide_menu();
	}
	void menu_pressed(){
		menu_button.hide();
		motion_menu.show();
	}

	//callback functions
	static void cb_quit(Address,Address pw){reference_to<Rectangle_window>(pw).quit_pressed();}
	static void cb_redraw(Address,Address pw){reference_to<Rectangle_window>(pw).redraw_pressed();}
	static void cb_expand(Address,Address pw){reference_to<Rectangle_window>(pw).expand_pressed();}
	static void cb_shrink(Address,Address pw){reference_to<Rectangle_window>(pw).shrink_pressed();}
	static void cb_rotate(Address,Address pw){reference_to<Rectangle_window>(pw).rotate_pressed();}
	static void cb_menu(Address,Address pw){reference_to<Rectangle_window>(pw).menu_pressed();}

	void rec_attach(Shape& r){attach(r);v_shapes.push_back(&r);}
	void text_attach(Shape& t){ attach(t); }
	void detach_func(Shape& r){detach(r);}

	bool win_wait(){
		show();
		button_pushed = false;
		while(!button_pushed) Fl::wait();
		Fl::redraw();
		return button_pushed;
	}
};

int main(){
	srand(time(0));
	Rectangle_window win{Point{0,0},x,y,"Programming Assignment #7"};
	int temp_w = 100, temp_h = 100;		// place holder values
	int w = 0; int h = 0;	// rectangle width and height
	int exp_w = 0, exp_h = 0;  // expanding width and height
	int shr_w = 0, shr_h = 0; 	// shrinking width and height
	int rot_w = 0, rot_h = 0; 	// rotating width and height
	int cx = 0, cy = 0;			// center of rotation
	bool expand = false,shrink = false, rotate = false;
	while(true){
		if(expand == true){
			temp_w = exp_w;
			temp_h = exp_h;
			goto HERE;
		}
		else if(shrink == true){
			temp_w = shr_w;
			temp_h = shr_h;
		}
		else if(rotate == true){
			w = rot_w; h = rot_h;
		}
		// initial width and height values
		w = rand() % 500 + 100;
		h = rand() % 300 + 50;

		HERE:		// skipping over new width and height values in order to allow expanding, shrinking and rotating

		// initial rectangle
		Graph_lib::Rectangle r{Point{rec_pos,rec_pos},w,h};
		r.set_color(Color::black);
		r.set_style(Line_style(Line_style::solid,4));
		ostringstream oss;
		oss << "Area is: " << w * h;
		Text a{Point{text_x,text_y},oss.str()};
		a.set_font(Font::times_bold);
		a.set_font_size(14);
		win.rec_attach(r); win.text_attach(a);

		//expand
		expand = win.get_exp();
		exp_w = temp_w + temp_w*k;
		exp_h = temp_h + temp_h*k;
		if(exp_w <= 0 || exp_h <= 0) continue;
		Graph_lib::Rectangle exp_r{Point{rec_pos,rec_pos},exp_w,exp_h};
		exp_r.set_color(Color::black);
		exp_r.set_style(Line_style(Line_style::solid,4));
		ostringstream oss1;
		oss1 << "Area is: " << exp_w * exp_h;
		Text exp_a{Point{text_x,text_y},oss1.str()};
		exp_a.set_font(Font::times_bold);
		exp_a.set_font_size(14);

		//shrink
		shrink = win.get_shr();
		shr_w = temp_w - temp_w*k;
		shr_h = temp_h - temp_h*k;
		if(shr_w <= 0 || shr_h <= 0) continue;
		Graph_lib::Rectangle shr_r{Point{rec_pos,rec_pos},shr_w,shr_h};
		shr_r.set_color(Color::black);
		shr_r.set_style(Line_style(Line_style::solid,4));
		ostringstream oss2;
		oss2 << "Area is: " << shr_w * shr_h;
		Text shr_a{Point{text_x,text_y},oss2.str()};
		shr_a.set_font(Font::times_bold);
		shr_a.set_font_size(14);

		//rotate
		rotate = win.get_rot();
		rot_w = w; rot_h = h;
		cx = rot_w/2; cy = rot_h/h;
		// angles
		float s = sin(angle);
		float c = cos(angle);
	    // translate point back to origin:
		rot_w -= cx;
		rot_h -= cy;
		// rotate point
		float xnew = rot_w * c - rot_h * s;
		float ynew = rot_w * s + rot_h * c;
		// translate point back:
		rot_w = xnew + cx;
		rot_h = ynew + cy;
		if(rot_w <= 0 || rot_h <= 0) continue;
		Graph_lib::Rectangle rot_r{Point{rec_pos,rec_pos},rot_w,rot_h};
		rot_r.set_color(Color::black);
		rot_r.set_style(Line_style(Line_style::solid,4));

		if(expand == true){
			win.detach_func(r), win.detach_func(exp_r), win.detach_func(shr_r), win.detach_func(rot_r);
			win.detach_func(a); win.detach_func(exp_a); win.detach_func(shr_a);
			win.rec_attach(exp_r);
			win.text_attach(exp_a);
			win.change(Color::red);
			cout << "Expanding..." << endl;
		}
		else if(shrink == true){
			win.detach_func(r), win.detach_func(exp_r), win.detach_func(shr_r), win.detach_func(rot_r);
			win.detach_func(a); win.detach_func(exp_a); win.detach_func(shr_a);
			win.rec_attach(shr_r);
			win.text_attach(shr_a);
			win.change(Color::blue);
			cout << "Shrinking..." << endl;
		}
		else if(rotate == true){
			win.detach_func(r), win.detach_func(exp_r), win.detach_func(shr_r), win.detach_func(rot_r);
			win.detach_func(a); win.detach_func(exp_a); win.detach_func(shr_a);
			win.rec_attach(rot_r);
			win.text_attach(a);
			win.change(Color::dark_green);
			cout << "Rotating..." << endl;
		}

		temp_w = w; temp_h = h;		// holds current values of for width and height
		win.win_wait();
		win.detach_func(r), win.detach_func(exp_r), win.detach_func(shr_r), win.detach_func(rot_r);
		win.detach_func(a); win.detach_func(exp_a); win.detach_func(shr_a);
	}
	return 0;
}



