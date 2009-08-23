#ifndef _Geometry_included_
#define _Geometry_included_

struct Geometry;
extern Geometry _G;

struct Rectangle {
  Rectangle(): x(0), y(0), w(0), h(0) {};
  void	size(int ww, int hh) { w=ww; h=hh;}
  int w, h;
  int x,y;
};

struct Geometry {
  Geometry();
  ~Geometry() {};
  void	recalc();	// recalc geometry of children
  void	dump();

  int		debug;		// debug flag
  Rectangle	screen;		// physical screen
  Rectangle	top;		// top window
  Rectangle	lister;		// top window
  Rectangle	title;		// size off complete title box
  Rectangle	item;		// size off one complete item
  Rectangle	cursor;		// size and pos for top normal cursor
  Rectangle	icon;		// size for icons
  Rectangle	text;		// text block for item
  
  int listerBorder;
  int titleHeight; 
  int itemSpacing;
  int itemHspacing;		// Hor spacing within item

  int cursorStep;

  int pageItems;		// max items on one page

  int showInfo;			// show info or not

};
#endif
