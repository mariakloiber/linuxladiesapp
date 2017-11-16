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
#include"texture.h"
#include"post.h"

Forum fOld; // creates new Forum object for old posts
Forum fNew; // creates new Forum object for new posts

int WIDTH = 570;  // width of the user window
int HEIGHT = 300;  // height of the user window
char programName[] = "forum";
int darkTexture, lightTexture, redTexture;  // texture ids
int scroll = 0;
int i = 0; // keeps track of the current spot in the array of usernames&posts
int k = i;
string date;

// post button info
bool buttonIsPressed = false, overButton = false;
string atextInButton = "POST";
double buttonPos[] = { 20, 20,   80, 55 };  // upper left, width, height

// back button info
bool buttonnIsPressed = false, overButtonn = false;
string textInButtonn = "BACK";
double buttonnPos[] = { 20, 250,   75, 30 };  // upper left, width, height

// new post button info
bool buttonnnIsPressed = false, overButtonnn = false;
string textInButtonnn = "NEW POST";
double buttonnnPos[] = { 10, 10,   100, 30 };  // upper left, width, height

// slider info
bool sliderIsPressed = false, overSlider = false;
double sliderBox1[] = { 545, 48,   9, 430 };  // background of slider
double sliderBox2[] = { 540, 40,   20, 10 };  // foreground of slider
double sliderPos = 0;  // where the slider currently is located
double sliderStartPos;  // where the mouse first clicked on the slider
double sliderMax = 230;  // upper limit to the slider position


// username textbox info
bool overTextBox = false;
string textInBox = "Username: ";
string textPost = "";
string m[30]; // where the usernames are saved
double textBox1[] = { 120, 30,   200, 40 };  // outer box for text
double textBox2[] = { 125, 35,   190, 30 };  // inner box for text
const unsigned int MAX_NUM_CHARS_IN_TEXTBOX = 20;

// post textbox info
bool overTextBoxx = false;
string textInBoxx = "Post: ";
string textPost2 = "";
string n[30]; // where the posts are stored
double textBoxx1[] = { 120, 100,   400, 180 };
double textBoxx2[] = { 125, 105,   390, 170 };
const unsigned int A_MAX_NUM_CHARS_IN_TEXTBOX = 42;

// triangles

bool overTriUp = false;
bool overTriDown = false;
bool triUpPressed, triDownPressed = false;
double triUp[] = { WIDTH-65, 35,  WIDTH-45, 35,  WIDTH-55, 25 };
double triUpBox[] = { WIDTH-70, 20,  30, 20 };
double triDown[] = { WIDTH-65, HEIGHT+175,  WIDTH-45, HEIGHT+175,  WIDTH-55, HEIGHT+185 };
double triDownBox[] = { WIDTH-70, HEIGHT+170,  30, 20 };

// this function adds a new post to the forum GUI
void newPosts(Forum &fNew, string d, string n, string c) {
  fNew.posts[fNew.numPosts] = new Post(d,n,c);
  fNew.numPosts++;
}

// the following function draws a rectangle, given
//   the upper left vertex and the width and height
void drawBox(double x, double y, double width, double height)
{
  glBegin(GL_POLYGON);
    glVertex2f(x, y);  // upper left
    glVertex2f(x, y + height);  // lower left
    glVertex2f(x + width, y + height);  // lower right
    glVertex2f(x + width, y);  // upper right
  glEnd();
}
void drawBox(double *pos)
{
  drawBox(pos[0], pos[1], pos[2], pos[3]);
}

// the drawText function draws some text at location x, y
//   note:  the text to be drawn is a C-style string!
void drawText(double x, double y, const char *text)
{
  glRasterPos2f( x, y );
  int length = strlen(text);
  for (int i = 0; i < length; i++){
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
    //glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
  }
}

// the reshape function handles the case where the user changes the size
//   of the window.  We need to fix the coordinate
//   system, so that the drawing area is still the unit square.
void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   WIDTH = w;  HEIGHT = h;
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0., WIDTH-1, HEIGHT-1, 0., -1.0, 1.0);
}
   
// draw the main forum screen
void drawWindow3(){
  glClearColor(0.25f, 0.25f, 0.5f, 1.0f); 
  glClear(GL_COLOR_BUFFER_BIT);

  // draws the slider and changes the slider's colors depending on whether the slider is pressed
  if ( sliderIsPressed || overSlider ){
    glColor3f(.5, .5, .5);  // gray
  }
  else {
    glColor3f(.25, .25, .25);  // dark gray
  }
  drawBox(sliderBox1); // draws the slider background
  if ( overSlider ) {
    glColor3f(.95, .95, .95);  // very light gray
  }
  else {
    glColor3f(.75, .75, .75);  // light gray
  }
  // adjust the slider position so that the sliding function is smooth when the window reshapes
  // draws the slider foreground
  if(sliderPos > 100 && sliderPos < 200)
    drawBox(sliderBox2[0], sliderBox2[1] + sliderPos + 100, sliderBox2[2], sliderBox2[3]);
  else if(sliderPos > 200)
    drawBox(sliderBox2[0], sliderBox2[1] + sliderPos + 200, sliderBox2[2], sliderBox2[3]);
  else
    drawBox(sliderBox2[0], sliderBox2[1] + sliderPos, sliderBox2[2], sliderBox2[3]);
  
  // draws the New Post button
  if ( buttonnnIsPressed ) {
    glColor3f(1, 0, 0);
    drawBox(buttonnnPos);  
  }
  else if ( overButtonnn ) {
    glColor3f(.75, .75, .75);
    drawBox(buttonnnPos);
  }
  else {
    glColor3f(.45, .45, .45);
    drawBox(buttonnnPos);
  }
  glColor3f(.2, .2, .2);
  drawText(buttonnnPos[0]+14, buttonnnPos[1]+buttonnnPos[3]-10, textInButtonnn.c_str() );

  // draws new boxes for every post
  int x1 = 10;
  int y1 = 70;
  int w = 520;
  int he = 30;
  if(i > 0 && i <= 1){
  glColor3f(.75, .75, .75);
  drawBox(x1, 70,   w, he);
  glColor3f(0, 0, 0);
  drawText(x1+230, 70+20, m[k].c_str() );
  glColor3f(.75, .75, .75);
  drawBox(x1, 70+50,   w, he+70);
  glColor3f(0, 0, 0);
  drawText(x1+10, (70+50)+20, n[k].c_str() );
  }
  else if (i > 1){
  glColor3f(.75, .75, .75);
  drawBox(x1, 70,   w, he);
  glColor3f(0, 0, 0);
  drawText(x1+230, 70+20, m[k].c_str() );
  glColor3f(.75, .75, .75);
  drawBox(x1, 70+50,   w, he+70);
  glColor3f(0, 0, 0);
  drawText(x1+10, (70+50)+20, n[k].c_str() );
  glColor3f(.75, .75, .75);
  drawBox(x1, 270,   w, he);
  glColor3f(0, 0, 0);
  drawText(x1+230, 270+20, m[k+1].c_str() );
  glColor3f(.75, .75, .75);
  drawBox(x1, 270+50,   w, he+70);
  glColor3f(0, 0, 0);
  drawText(x1+10, (270+50)+20, n[k+1].c_str() );
  }

  /*ofstream out;
  out.open("Forum.txt");
  out << (i) << endl; // adds number of new posts to number of old posts
  for (int l=0; l<i; l++) { // adds old posts back in
    out << date << endl;
    out << m[l] << endl;
    out << n[l] << endl;
  }
  cout << "!";
  */
  glutSwapBuffers();
}

// function to draw the first window, the window where you can create a new post
void drawWindow()
{
  // clear the buffer
  glClear(GL_COLOR_BUFFER_BIT);

  // draw the Back button
  if ( buttonnIsPressed ){
    glColor3f(.75, 0, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }
  else if ( overButtonn ){
    glColor3f(.75, .75, .75);
  }
  else {
    glColor3f(.45, .45, .45);
  }
  drawBox(buttonnPos);
  glColor3f(.2, .2, .2);
  drawText( buttonnPos[0]+20, buttonnPos[1]+buttonnPos[3]-10, textInButtonn.c_str() );
  
  // draw the username textbox
  glColor3f(.25, .25, 1.);  // dark gray
  drawBox(textBox1);
  if ( overTextBox ) {
    glColor3f(1,1,1);
    drawBox(textBox2);
    glColor3f(0, 0, 1);
    drawText( textBox2[0]+5, textBox2[1]+textBox2[3]-10, textPost.c_str());
  } else {
    glColor3f(.75, .75, .75);  // light gray
    drawBox(textBox2);
    glColor3f(0, 0, 1);
    drawText( textBox2[0]+5, textBox2[1]+textBox2[3]-10, textInBox.c_str());
  }
  // control the writing of text within the username text box
  if ( overTextBox ) { // draw with a cursor
    string withCursor(textPost);
    withCursor += '|';
    drawText( textBox2[0]+5, textBox2[1]+textBox2[3]-10, withCursor.c_str() );
  } else {
    drawText( textBox2[0]+5, textBox2[1]+textBox2[3]-10, textPost.c_str());
  }

  // draw the post textbox
  glColor3f(.25, .25, 1.);  
  drawBox(textBoxx1);
  if ( overTextBoxx ) {
    glColor3f(1,1,1);  // white
    drawBox(textBoxx2);
    glColor3f(0, 0, 1);
    drawText( textBoxx2[0]+5, textBoxx2[1]+textBoxx2[3]-150, textPost2.c_str() );
  }
  else {
    glColor3f(.75, .75, .75);  // light gray
    drawBox(textBoxx2);
    glColor3f(0, 0, 1);
    drawText( textBoxx2[0]+5, textBoxx2[1]+textBoxx2[3]-150, textInBoxx.c_str() );
  }
  if ( overTextBoxx ) { // draw the text with a cursor in the post textbox
    string withCursor(textPost2);
    withCursor += '|';
    if(withCursor.length() <= 42)
      drawText( textBoxx2[0]+5, textBoxx2[1]+textBoxx2[3]-150, withCursor.c_str() );
    else if (withCursor.length() > 42 && withCursor.length() <= 80){
      //withCursor[43] = '\n';
      drawText( textBoxx2[0]+5, textBoxx2[1]+textBoxx2[3]-150, withCursor.c_str() );
      }
  } else {
    drawText( textBoxx2[0]+5, textBoxx2[1]+textBoxx2[3]-150, textPost2.c_str() );
  }
  
  // tell the graphics card that we're done-- go ahead and draw!
  //   (technically, we are switching between two color buffers...)
  glutSwapBuffers();
}

// close the window and finish the program
void exitAll()
{
  int win = glutGetWindow();
  glutDestroyWindow(win);
  exit(0);
}

// process keyboard events
void keyboard( unsigned char c, int x, int y )
{
  if ( overTextBox ) { // intercept keyboard press, to place in text box
    if ( 27==c ) exitAll();  // escape terminates the program, even in textbox
    if ( 13==c ) {
      m[i] = textPost; // adds the new username to the array of usernames
      textPost = ""; 
    } else if ( '\b'==c || 127==c ) { // handle backspace
      if ( textPost.length() > 0 ) textPost.erase(textPost.end()-1);
    } else if ( c >= 32 && c <= 126 ) { // check for printable character
      // check that we don't overflow the box
      if ( textPost.length() < MAX_NUM_CHARS_IN_TEXTBOX ) textPost += c;
    }
  } else {
    switch(c) {
      case 'q':
      case 'Q':
      case 27:
        exitAll();
        break;
      default:
        break;
    }
  }

  // code for post text area
  if ( overTextBoxx ) { // intercept keyboard press, to place in text box
    if ( 27==c ) exitAll();  // escape terminates the program, even in textbox
    if ( 13==c ) {
      n[i] = textPost2; // saves the new text post to the array of posts
      // adds a new post
      newPosts(fNew,"05/11/17",m[i], n[i]);
      ++i; // goes to the next spot in each array so that new usernames and posts can be saved
      textPost2 = "";
    } else if ( '\b'==c || 127==c ) { // handle backspace
      if ( textPost2.length() > 0 ) textPost2.erase(textPost2.end()-1);
    } else if ( c >= 32 && c <= 126 ) { // check for printable character
      // check that we don't overflow the box
      if ( textPost2.length() < A_MAX_NUM_CHARS_IN_TEXTBOX ) textPost2 += c;
    }
  } else {
    switch(c) {
      case 'q':
      case 'Q':
      case 27:
        exitAll();
        break;
      default:
        break;
    }
  }

  glutPostRedisplay();
}

void special_keyboard(int key,int x, int y)
{
  // update my current position
  switch (key) {
    case GLUT_KEY_UP:
      k--;
      break;
    case GLUT_KEY_DOWN:
      k++;
      break;
  }
}

// the following function tests whether a point at position x,y is inside
//   the rectangle on the screen corresponding to the button

bool onButtonn(int x, int y)
{
  return x >= buttonnPos[0]  && y >= buttonnPos[1] &&
         x <= buttonnPos[0] + buttonnPos[2] &&
         y <= buttonnPos[1] + buttonnPos[3];
}

bool onButtonnn(int x, int y)
{
  return x >= buttonnnPos[0]  && y >= buttonnnPos[1] &&
         x <= buttonnnPos[0] + buttonnnPos[2] &&
         y <= buttonnnPos[1] + buttonnnPos[3];
}

// the following function tests whether a point at position x,y is inside
//   the rectangle on the screen corresponding to the slider
bool onSliderForeground(int x, int y)
{
  return y >= sliderBox2[1]+sliderPos  && x >= sliderBox2[0] &&
         y <= sliderBox2[1]+sliderBox2[3]+sliderPos &&
         x <= sliderBox2[0]+sliderBox2[2];
}
bool onSliderBackground(int x, int y)
{
  return y >= sliderBox2[1]  && x >= sliderBox2[0] &&
         y <= sliderBox2[1]+sliderMax+sliderBox2[3] &&
         x <= sliderBox2[0]+sliderBox2[2];
}

// test whether point (x,y) is inside rectangle pos
bool onRect(int x, int y, double* pos) {
  return x >= pos[0]  && y >= pos[1] &&
         x <= pos[0] + pos[2] &&
         y <= pos[1] + pos[3];
}

bool onRect2(int x, int y) {
  return x >= 495  && y >= 465 &&
         x <= 525 &&
         y <= 490;
}

// the following function tests whether a point at position x,y is inside
//   the rectangle on the screen corresponding to the textBox background
bool onTextBox(int x, int y)
{
  return x >= textBox1[0] && y >= textBox1[1] &&
         x <= textBox1[0]+textBox1[2] &&
         y <= textBox1[1]+textBox1[3];
}
bool onTextBoxx(int x, int y) // code for new text area
{
  return y >= 105 && x >= 125 &&
         y <= 275 &&
         x <= 515;
}
// the mouse function is called when a mouse button is pressed down or released
void mouse(int mouseButton, int state, int x, int y)
{
  if ( GLUT_LEFT_BUTTON == mouseButton ) {
    if ( GLUT_DOWN == state ) {
      // the user just pressed down on the mouse-- do something
      if ( onButtonn(x,y) ) buttonnIsPressed = true;
      else buttonnIsPressed = false;
      if ( onButtonnn(x,y) ) buttonnnIsPressed = true;
      else buttonnnIsPressed = false;
      if ( (overTriUp) && (scroll>0) ) {
	triUpPressed = true;
      } else triUpPressed = false;
      if ( (overTriDown) && i>1 ) {
	triDownPressed = true;
      } else triDownPressed = false;
      // change the slider start position when the slider is pressed and moved
      if ( onSliderForeground(x,y) ) {
        sliderIsPressed = true;
        sliderStartPos = y - sliderPos;
      } else if ( onSliderBackground(x,y) ) {
        sliderIsPressed = true;
        sliderPos = y - sliderBox1[0]; 
        sliderStartPos = y - sliderPos;
      } else sliderIsPressed = false;
      
    } else {
      if ( onButtonn(x,y) && buttonnIsPressed ){
	//cout << "Back Button" << endl;
        glutDisplayFunc(drawWindow3);
      }
      buttonnIsPressed = false;
      if ( onButtonnn(x,y) && buttonnnIsPressed ){
        //cout << "New Forum Post Button" << endl;
	glutDisplayFunc(drawWindow);
      }
      buttonnnIsPressed = false;

      if(overTriDown && triDownPressed){
	//drawWindow3();
      }
      
      if ( sliderIsPressed )
        cout << "New slider position: " << sliderPos << endl;
      sliderIsPressed = false;
    
    }
  } else if ( GLUT_RIGHT_BUTTON == mouseButton ) {
  }
  glutPostRedisplay();
}

// the mouse_motion function is called when the mouse is being dragged,
//   and gives the current location of the mouse
void mouse_motion(int x,int y)
{
  // the mouse button is pressed, and the mouse is moving....
  if ( buttonnnIsPressed ) {
    // do nothing for new post button
  }
  else if ( sliderIsPressed ) {
    double newSliderPos = y - sliderStartPos;
    // check that the new slider pos does not go off the end of the slider
    if ( newSliderPos < 0 ) sliderPos = 0;
    else if ( newSliderPos > sliderMax ) sliderPos = sliderMax;
    // reshape window if slider position is moved past a certain point
    // aimed at instituting a scrolling functionality
    else if ( newSliderPos > 100 && newSliderPos < 200) {
      reshape(570, 400);
    }
    else if( newSliderPos < 100 ) {
      reshape(570, 300);
    }
    else if( newSliderPos > 200 ) {
      reshape(570, 500);
    }
    else sliderPos = newSliderPos;
  }
  else {
    if ( onButtonnn(x,y) ) overButtonnn = true;
    else overButtonnn = false;
    if ( onSliderBackground(x,y) ) overSlider = true;
    else overSlider = false;
    if ( onTextBox(x,y) ) overTextBox = true; // code for username text box
    else overTextBox = false;
    if ( onTextBoxx(x,y) ) overTextBoxx = true; // code for new post text box
    else overTextBoxx = false;
    if ( onRect(x,y,triUpBox) ) overTriUp = true;
    else overTriUp = false;
    if ( onRect2(x,y) ) overTriDown = true;
    else overTriDown = false;
  }

  if ( buttonnIsPressed ) {
    // do nothing for back button
  }
  else if ( sliderIsPressed ) {
    double newSliderPos = y - sliderStartPos;
    // check that the new slider pos does not go off the end of the slider
    if ( newSliderPos < 0 ) sliderPos = 0;
    else if ( newSliderPos > sliderMax ) sliderPos = sliderMax;
    else sliderPos = newSliderPos;
    }
  else {
    if ( onButtonn(x,y) ) overButtonn = true; // Back button
    else overButtonn = false;
    if ( onSliderBackground(x,y) ) overSlider = true;
    else overSlider = false;
    if ( onTextBox(x,y) ) overTextBox = true; // code for username text area
    else overTextBox = false;
    if ( onTextBoxx(x,y) ) overTextBoxx = true; // code for new post text area
    else overTextBoxx = false;
    if ( onRect(x,y,triUpBox) ) overTriUp = true;
    else overTriUp = false;
    if ( onRect2(x,y) ) overTriDown = true;
    else overTriDown = false;
  }

  glutPostRedisplay();
}

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

  // allow blending (for transparent textures)
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  // welcome message
  cout << "Welcome to the " << programName << ".  Type your username in the top text field," << endl;
  cout << " then type your post in the lower text field. You can also use the up/down arrow keys to navigate through the forum posts. " << endl;
}


// init_gl_window is the function that starts the ball rolling, in
//  terms of getting everything set up and passing control over to the
//  glut library for event handling.  It needs to tell the glut library
//  about all the essential functions:  what function to call if the
//  window changes shape, what to do to redraw, handle the keyboard,
//  etc.

void init_gl_window()
{
  char *argv[] = { programName };
  int argc = sizeof(argv) / sizeof(argv[0]);
  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
  glutInitWindowSize(WIDTH,HEIGHT);
  glutInitWindowPosition(100,100);
  glutCreateWindow(programName);
  init();

  darkTexture = loadTexture("buttonDark.pam");
  lightTexture = loadTexture("buttonLight.pam");
  redTexture = loadTexture("buttonRed.pam");

  glutDisplayFunc(drawWindow3);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special_keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(mouse_motion);
  glutPassiveMotionFunc(mouse_motion);
  
  glutMainLoop();
}

int main()
{
  // pulls post info
  ifstream in("Forum.txt");
  string blankline;

  in >> fOld.numPosts;
  getline(in, blankline);
  fOld.posts = new Post*[fOld.numPosts];
  i = 0;
  for (; i<fOld.numPosts; i++) {
    getline(in, date);
    getline(in, blankline);
    getline(in, m[i]);
    getline(in, n[i]);
    //getline(in, blankline);
  }
  in.close();
  
  //forum for new posts
  fNew.posts = new Post*[20];
  fNew.numPosts = 0;

  special_keyboard(0, 0, 0);
  
  init_gl_window();
}
