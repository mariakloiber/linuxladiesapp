#include<iostream>
#include<fstream>
using namespace std;
#ifdef MACOSX
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<sstream>
#include "Opportunity.h"
#include <string>
#include<assert.h>

// window
double WIDTH = 700;  // width of the user window
double HEIGHT = 500;  // height of the user window
char programName[] = "Database";

// mini windows
int secondWIDTH = 700;  // width of the second user window
int secondHEIGHT = 270;  // height of the second user window
int currentOpp = 0;

// window info:  store the id of a window when it is created, and
//   set this id to zero when the window is destroyed
int mainWindowID = 0, secondWindowID = 0;
int createNewWindow(const char *windowTitle);  // declare this function
void drawSecondWindow(); // declare this function

// list area
double listMargin = 10;
double listWidth = WIDTH - 2*listMargin; // width of the list area
double listHeight = HEIGHT - 8*listMargin; // height of the list area
double listArea[] = { listMargin, listMargin*4,   listWidth, listHeight };

// item boxes
const int numItems = 6;
double itemMargin = 5;
double itemWidth = listWidth-itemMargin*2;
double itemHeight = ( listHeight - (itemMargin*(numItems+1)) ) / numItems;
double** itemList = new double* [numItems]; // allocated in main
bool overItem[numItems];

// triangles
bool overTriUp = false;
bool overTriDown = true;
double triUp[] = { WIDTH-50, 30,  WIDTH-30, 30,  WIDTH-40, 20 };
double triUpBox[] = { WIDTH-55, 15,  30, 20 };
double triDown[] = { WIDTH-50, HEIGHT-30,  WIDTH-30, HEIGHT-30,  WIDTH-40, HEIGHT-20 };
double triDownBox[] = { WIDTH-55, HEIGHT-35,  30, 20 };

// text
Data* db;
Data in, sc;
int scroll = 0;

// type buttons
char typeInt[] = "Internships";
char typeSchol[] = "Scholarships";
bool overInt = false;
bool overSchol = false;
double intBox[] = { 10,10,  120,30 };
double scholBox[] = { 140,10,  120,30 };

//------------------------------------------


// draw rectangle, given upper left vertex, width, and height
void drawBox(double x, double y, double width, double height) {
  glBegin(GL_POLYGON);
    glVertex2f(x, y);  // upper left
    glVertex2f(x, y + height);  // lower left
    glVertex2f(x + width, y + height);  // lower right
    glVertex2f(x + width, y);  // upper right
  glEnd();
}
void drawBox(double *pos) {
  drawBox(pos[0], pos[1], pos[2], pos[3]);
}

// draw triangle, given corners
void drawTriangle(double x1, double y1, double x2, double y2, double x3, double y3) {
  glBegin(GL_POLYGON);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glVertex2f(x3, y3);
  glEnd();
}
void drawTriangle(double *pos) {
  drawTriangle(pos[0], pos[1], pos[2], pos[3], pos[4], pos[5]);
}

// the drawText function draws some text at location x, y
void drawTextLarge(double x, double y, const char *text) {
  glRasterPos2f( x, y );
  int length = strlen(text);
  for (int i = 0; i < length; i++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
}

void drawTextSmall(double x, double y, const char *text) {
  glRasterPos2f( x, y );
  int length = strlen(text);
  for (int i = 0; i < length; i++)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
}



//------------------------------------------

void drawWindow() {
  // clear the buffer
  glClear(GL_COLOR_BUFFER_BIT);

  //draw the type buttons
  glColor3f(.25, .25, .25); // dark gray
  if ( ( overInt ) || (db->currentType == Internship) ) {
    drawBox(intBox);
    glColor3f(1,1,1);  // white
  }  else glColor3f(.75, .75, .75);  // light gray
  drawTextLarge(15,30,typeInt);
  glColor3f(.25, .25, .25);
  if ( ( overSchol ) || (db->currentType == Scholarship) ) {
    drawBox(scholBox);
    glColor3f(1,1,1);  // white
  }  else glColor3f(.75, .75, .75);  // light gray
  drawTextLarge(145,30,typeSchol);

  // draw the list area
  glColor3f(.25, .25, .25); // dark gray
  drawBox(listArea);

  //draw the list items
  for (int i=0; i<numItems; i++) {
    if ( overItem[i] ) glColor3f(1, 1, 1);
    else glColor3f(.75, .75, .75);
    drawBox(itemList[i]);
  }

  // draw triangle boxes
  glColor3f(.25, .25, .25); // dark gray
  drawBox(triUpBox);
  drawBox(triDownBox);

  // draw the page number
  glColor3f(.75, .75, .75);
  string page = "Page " + to_string(scroll/numItems+1);
  char* pageNum = new char [8];
  for (int i=0; i<8; i++)
    pageNum[i] = page[i];
  drawTextLarge(WIDTH-150, 25, pageNum);

  // draw the triangles
  if ( overTriUp ) glColor3f(1,1,1); else glColor3f(.75, .75, .75);
  drawTriangle(triUp);
  if ( overTriDown ) glColor3f(1,1,1); else glColor3f(.75, .75, .75);
  drawTriangle(triDown);

  // draw the text
  glColor3f(0, 0, 0); // black
  for (int i=scroll, j=0; (j<numItems) && (i<db->numElements); i++, j++) {
    char* title = db->opp[i]->getTitle();
    char* date = db->opp[i]->getDate();
    double h = itemList[j][1] + (itemHeight/2) + 5;
    drawTextLarge(itemList[j][0]+10, h, title);
    drawTextLarge(itemList[j][0]+listWidth-100, h, date);
    delete [] title;
    delete [] date;
  }

  // tell the graphics card that we're done-- go ahead and draw!
  // (technically, we are switching between two color buffers...)
  glutSwapBuffers();
}


// close the window and finish the program
void exitAll() {
  if ( secondWindowID ) {
    glutDestroyWindow(secondWindowID);
    secondWindowID = 0;
    glutSetWindow(mainWindowID);
    return;
   }
  else if ( mainWindowID ) {
    glutDestroyWindow(mainWindowID);
    exit(0);
  }
}


// process keyboard events
void keyboard( unsigned char c, int x, int y ) {
  switch(c) {
    case 'q':
    case 'Q':
    case 27:
      exitAll();
      break;
    default:
      break;
  }
  glutPostRedisplay();
}

// the reshape function handles the case where the user changes the size
//   of the window.  We need to fix the coordinate
//   system, so that the drawing area is still the unit square.
void reshape(int w, int h) {
  /*
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   WIDTH = w;  HEIGHT = h;
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0., WIDTH-1, HEIGHT-1, 0., -1.0, 1.0);
  */

   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   if ( mainWindowID == glutGetWindow() ) {
     WIDTH = w;  HEIGHT = h;
   } else {
     secondWIDTH = w;  secondHEIGHT = h;
   }
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0., w-1, h-1, 0., -1.0, 1.0);
}

// test whether point (x,y) is inside rectangle pos
bool onRect(int x, int y, double* pos) {
  return x >= pos[0]  && y >= pos[1] &&
         x <= pos[0] + pos[2] &&
         y <= pos[1] + pos[3];
}


//----------------------------------------


void mouse(int mouseButton, int state, int x, int y) {
  if ( GLUT_LEFT_BUTTON == mouseButton ) {
    if ( GLUT_DOWN == state ) {   // left mouse button is pressed


    } else {   // mouse is no longer pressed
      if ( overInt ) {db = &in; scroll = 0;}
      if ( overSchol ) {db = &sc; scroll = 0;}
      if ( (overTriUp) && (scroll>0) ) scroll-=numItems;
      if ( (overTriDown) &&
	   ((scroll+numItems) < db->numElements) ) scroll+=numItems;
      for (int i=0; i<numItems; i++) {
	if ( (overItem[i]) && (i+scroll < db->numElements) ) {
	  if ( 0 == secondWindowID ) { // create the second window
	    currentOpp = i+scroll;
	    glutInitWindowPosition(500,500);  // choose where to put it
	    glutInitWindowSize(secondWIDTH,secondHEIGHT); // how big?
	    secondWindowID = createNewWindow(db->opp[i+scroll]->getTitle());
	    glutDisplayFunc(drawSecondWindow); // assign the display function
	    glOrtho(0., secondWIDTH-1, secondHEIGHT-1, 0., -1.0, 1.0); // coords
	    glutPostWindowRedisplay(secondWindowID);
	    glutSetWindow(mainWindowID);    // select the main window again
	  }
	}
      }
    }
  } else if ( GLUT_RIGHT_BUTTON == mouseButton ) {
    // right mouse button pressed
  }
  glutPostRedisplay();
}

// gives mouse location while mouse is being dragged
void mouse_motion(int x,int y)
{
  // check if mouse is on buttons
  if ( onRect(x,y,triUpBox) ) overTriUp = true; else overTriUp = false;
  if ( onRect(x,y,triDownBox) ) overTriDown = true; else overTriDown = false;
  if ( onRect(x,y,intBox) ) overInt = true; else overInt = false;
  if ( onRect(x,y,scholBox) ) overSchol = true; else overSchol = false;

  // check if mouse is over item boxes
  for (int i=0; i<numItems; i++) {
    if ( (onRect(x,y,itemList[i])) && (i+scroll < db->numElements) )
      overItem[i] = true; else overItem[i] = false;
  }
  glutPostRedisplay();
}


// -----------------------------------------


// the createNewWindow function creates a window and returns its id
int createNewWindow(const char *windowTitle)
{
  int id = glutCreateWindow(windowTitle);
  assert( 0 != id );
  // clear the window to black
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  // begin setting up the coordinate system
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // allow blending (for transparent textures)
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  // register callbacks
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mouse_motion);
  glutPassiveMotionFunc(mouse_motion);

  return id;
}


void drawSecondWindow()
{
  glutSetWindow(secondWindowID);  // this call may not be necessary

  // clear the buffer
  glClear(GL_COLOR_BUFFER_BIT);

  // draw the box
  glColor3f(.75, .75, .75); //light gray
  drawBox(20,20,660,230);

  glColor3f(0,0,0); //black
  drawTextSmall(40,40,db->opp[currentOpp]->getTitle());
  drawTextSmall(40,80,db->opp[currentOpp]->getDate());
  drawTextSmall(40,120,db->opp[currentOpp]->getDescription());
  drawTextSmall(40,170,db->opp[currentOpp]->getQualifications());
  drawTextSmall(40,230,db->opp[currentOpp]->getLink());


  // tell the graphics card that we're done-- go ahead and draw!
  //   (technically, we are switching between two color buffers...)
  glutSwapBuffers();
}

//----------------------------------------



// the init function sets up the graphics card to draw properly
void init(void)
{
  // clear the window to black
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  // set up the coordinate system:  number of pixels along x and y
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0., WIDTH-1, HEIGHT-1, 0., -1.0, 1.0);
}

// init_gl_window is the function that starts the ball rolling, in
//  terms of getting everything set up and passing control over to the
//  glut library for event handling.  It needs to tell the glut library
//  about all the essential functions:  what function to call if the
//  window changes shape, what to do to redraw, handle the keyboard,
//  etc.
void init_gl_window() {
  char *argv[] = { programName };
  int argc = sizeof(argv) / sizeof(argv[0]);
  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
  glutInitWindowSize(WIDTH,HEIGHT);
  glutInitWindowPosition(400,300);
  mainWindowID = glutCreateWindow(programName);
  init();

  glutDisplayFunc(drawWindow);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(mouse_motion);
  glutPassiveMotionFunc(mouse_motion);
  glutMainLoop();
}

void storeInfo (ifstream &f, Data &d, elementType type) {
  string blank;
  f >> d.numElements;
  getline(f, blank);
  d.currentType = type;
  d.opp = new Opportunity*[d.numElements];
  for (int i=0; i<d.numElements; i++) {
    d.opp[i] = new Opportunity(f);
    cout << "opp complete" << endl;
  }
}


//----------------------------------------

int main() {

  // allocate item box sizes
  for (int i=0; i<numItems; i++) {
    double h = (itemMargin*(i+1)) + (itemHeight*i) + listArea[1];
    double size[] = { listMargin+itemMargin, h,   itemWidth, itemHeight };
    itemList[i] = new double [4];
    for (int j=0; j<4; j++)
      itemList[i][j] = size[j];
  }

  // store scholarship info
  ifstream g("Scholarships.txt");
  storeInfo(g, sc, Scholarship);

  // store internship info
  ifstream h("Internships.txt");
  storeInfo(h, in, Internship);

  db = &in;

  init_gl_window();

  return 0;
}
