#include <liberdm/display.h>
#include <stdio.h>
#include "Geometry.h"

Geometry	_G;		// Global for everybody

using namespace std;

Geometry::Geometry() {
  debug = 1;
  // these things are fixed: defined in liberdm/display.h
  screen.size(SCREEN_WIDTH, SCREEN_HEIGHT);
  top.size(screen.w, screen.h  - TOOLBAR_HEIGHT - PAGEBAR_HEIGHT);

  // factory defaults
  listerBorder = 51;
  titleHeight  = 54;
  itemHspacing = 4;	// Horizontal spacing within item
  pageItems    = 12;

  showInfo    = 0;

  recalc();
}


void Geometry::recalc() {

  if (pageItems < 4) pageItems  =  4;
  if (pageItems > 20) pageItems = 20;

  lister.size(top.w, top.h-listerBorder);	// top margin

  title.w = lister.w - 2*listerBorder;		// left and right margin
  title.h = titleHeight;

  int room = (lister.h - title.h) / pageItems;	// total room for one item
  itemSpacing = itemHspacing;
  item.size(lister.w, room-itemSpacing);

  cursor.size(listerBorder-itemHspacing, item.h);
  icon.size(item.h*2/3+2, item.h);	// is exact for original layout
  text.size(lister.w - cursor.w - icon.w - 2*itemHspacing - listerBorder, item.h);

  cursor.x = listerBorder - itemHspacing;
  cursor.y = listerBorder + title.h + itemSpacing;
  cursorStep = item.h + itemSpacing;
  
  if (debug) dump();
}

void Geometry::dump() {
  fprintf(stderr, "====== Geometry =======\n");
  fprintf(stderr, "Screen:           %3dx%3d\n", screen.w, screen.h);
  fprintf(stderr, "Top window:       %3dx%3d\n", top.w, top.h);
  fprintf(stderr, "Settings:\n");
  fprintf(stderr, "  listerBorder:   %3d\n", listerBorder);
  fprintf(stderr, "  titleHeight:    %3d\n", titleHeight);
  fprintf(stderr, "  itemSpacing:    %3d\n", itemSpacing);
  fprintf(stderr, "  pageItems:      %3d\n", pageItems);
  fprintf(stderr, "Computed:\n");
  fprintf(stderr, "  Lister:         %3dx%3d\n", lister.w, lister.h);
  fprintf(stderr, "  Title:          %3dx%3d\n", title.w, title.h);
  fprintf(stderr, "  Item:           %3dx%3d\n", item.w, item.h);
  fprintf(stderr, "    Cursor:       %3dx%3d\n", cursor.w, cursor.h);
  fprintf(stderr, "    Icon:         %3dx%3d\n", icon.w, icon.h);
  fprintf(stderr, "    Text:         %3dx%3d\n", text.w, text.h);
  fprintf(stderr, "  Cursor pos:     X0=%3d, Y0=%3d, Step=%3d\n", cursor.x, cursor.y, cursorStep);
}
