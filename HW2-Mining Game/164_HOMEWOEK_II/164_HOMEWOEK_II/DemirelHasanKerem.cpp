/*********
   CTIS164 - Template Source Program
----------
STUDENT : Hasan Kerem Demirel
SECTION : 003
HOMEWORK: Homework II
----------
PROBLEMS: --

          Note: The hook wont go if the player is trying to replace the roller to the right or left and this situation is done intentionally because the game would
          be so easy with the roller that can be moved while the hook is fired. Therefore, its not a problem, its done intentionally.

          Note 2: It is not a  minimum requirement but when the game is over, you are able to press F1 and restart the game. However, eventhough the 'game over' screen 
          should go immediately when the game is over, it activates when a button is pressed other then F1. (You are able to press F1 and restart
          when the game over screen activated too)
----------
ADDITIONAL FEATURES: 1)There are 5 different targets to aim at. (Diamond, Sapphire, Amethyst, Ruby and Bone).
                     2)These 5 different gems and bone have their different prices and in different colours, they all have individual counters and price counters
                     on the screen.
                     3)The total coins is shown according to the coins the player has collected in one game.
                     -Some specific targets doesnt increase the total coins abut lowers it (bone).
                     4)The best score is shown according to the coins the player has collected in per game.
                     5)The hook cannot go outside of the window, when it hits the bottom, it turns to its initial position (faster than when its thrown).
                     5)When the hook hits a gem, bone or the ground, the total score turns white and the hook turns green.
                     6)When the hook hits a gem or bone, the hook pulls the target that is hit to the roller. When the target comes to the roller it disappears.
                     7)There is a little plus sign in front of the hook that helps the player predict where the hook would go.
                     8)There is a game over screen after the game ends that shows you how many gems and bones you have collected in one game (the players can restart
                     the game when they press F1).
                     9)The game over screen gives you a specific message in terms of the most collected target.
                     10)The program stops running when pressed esc.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 2000/3

#define TIMER_PERIOD  10 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

//the points the targets bring when they are hit
#define DIACOST 1000    
#define SAPCOST 400
#define AMTCOST 300
#define RUBCOST 200
#define BONCOST -500

//the game is on or over depending on the times up or not
#define ON 1
#define OVER 0

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;

int  winWidth, winHeight;   // current Window width and height

/*The coordinats of the background*/
int  bgx = 0, bgy = 200,    
     layerx = 0, layery = 200;     

int  xM = 0, yM = -200,
     widthM = 1000, heightM = 800;

/*The coordinats of the information card below*/
int xIC = 0, yIC = -323,
    widthIC = WINDOW_WIDTH, heightIC = 20;

/*The coordinats of the targets*/
int coodiay = -120, coodiax = 600,      //the diamond's initial position
    coosapy = -75, coosapx = 544,       //the sapphire's initial position
    cooamty = -37, cooamtx = 570, xA = cooamtx, yA = cooamty, widthA = 10, heightA = 25,    //the amethyst's initial position and the size of the rectangle inside of the amethyst
    coorubx = 525, cooruby = 0, xR = coorubx, yR = cooruby, widthR = 15, heightR = 15,      //the ruby's initial position and the size of the square inside of the rub
    coobonx = 515, coobony = 50;         //the bone's initial position

/*The coordinats of the hook*/
int xH = 0, yH = 175, rH = 10,          //the hook's initial position
    hD = 1;                             //to fire the hook

int touchedgr = 0;                      //if the hook is touched the ground or anything else not

int timergo = 1;                        //if the time meter runs or not

int cntdia = 0, cntsap = 0, cntamt = 0, cntrub = 0, cntbon = 0;     //counter of diamond sapphire amethyst ruby and bone
int total, bestscore = 0;                                           //total amount of coins collected and bestscore

int second = 19, splitsecond = 99;      //the time that is left and shown in the up right corner

int gamemode = ON;                      //if the game is over or not

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

////
////void print(int x, int y, const char* string, void* font)
////{
////    int len, i;
////
////    glRasterPos2f(x, y);
////    len = (int)strlen(string);
////    for (i = 0; i < len; i++)
////    {
////        glutBitmapCharacter(font, string[i]);
////    }
////}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}



void drawBackGround()           //the brown background
{
   

    //the line
    glLineWidth(20);
    glColor3f(64/ 225., 47/225., 29/ 225.);
    glBegin(GL_LINE_LOOP);
    glVertex2i(bgx - 500, layery);
    glVertex2i(bgx + 500, layery);
    glEnd();

    //the mine
    glColor3f(97 / 225., 54 / 225., 19 / 225.);
    glRectf(xM - widthM / 2, yM - heightM / 2, xM + widthM / 2, yM + heightM / 2);
}

void drawInfoCard()         //the black info card that assist the player to know how much of a collection s(he) did
{
    if (touchedgr == 1)
        glColor3f(1, 1, 1);         //total writing turns white when hook touched somethink
    else
        glColor3f(0, 0, 0);
    
    total = cntdia * DIACOST + cntsap * SAPCOST + cntamt * AMTCOST + cntrub * RUBCOST + cntbon * BONCOST;
    
    vprint(-500, 211, GLUT_BITMAP_8_BY_13, "TOTAL COINS :%d", total);

    glColor3f(150 / 225., 150 / 225., 150 / 225.);
    vprint(-500, 320, GLUT_BITMAP_8_BY_13, "%d DIAMONDS COLLECTED: %d coins", cntdia, cntdia * DIACOST);    //shows the info about diamonds
    glColor3d(0, 0, 1);
    vprint(-500, 300, GLUT_BITMAP_8_BY_13, "%d SAPPPHIRES COLLECTED: %d coins", cntsap, cntsap * SAPCOST);  //shows the info about sapphires
    glColor3f(90/225., 36 / 225., 123 / 225.);
    vprint(-500, 280, GLUT_BITMAP_8_BY_13, "%d AMETHYSTS COLLECTED: %d coins", cntamt, cntamt * AMTCOST);   //shows the info about amethysts
    glColor3d(1, 0, 0);
    vprint(-500, 260, GLUT_BITMAP_8_BY_13, "%d RUBIES COLLECTED: %d coins", cntrub, cntrub * RUBCOST);      //shows the info about rubies
    glColor3d(100/225., 100/225., 0);
    vprint(-500, 240, GLUT_BITMAP_8_BY_13, "%d BONES COLLECTED: %d lost", cntbon, cntbon * BONCOST);        //shows the info about bones
    glColor3f(249 / 225., 240 / 225., 24 / 225.);
    vprint(-500, 200, GLUT_BITMAP_8_BY_13, "YOUR BEST SCORE IS: %d!", bestscore);                           //shows the best score

    glColor3f(0, 0, 0);
    vprint(320, 320, GLUT_BITMAP_8_BY_13, "Time Reamining: %d.%.2d", second, splitsecond);                  //shows the timer

    glColor3f(0, 0, 0);
    glRectf(xIC - widthIC / 2, yIC - heightIC / 2, xIC + widthIC / 2, yIC + heightIC / 2);
    glColor3f(1, 1, 0);
    vprint(-350, -323, GLUT_BITMAP_8_BY_13, "<Space> to throw the hook - <F1> to pause/resume (or to restart when the game is over)");

}

void drawRoller()
{
    //the circle
    glColor3f(0, 0, 1);
    circle(layerx, layery, 120);

    /*foot*/
    //left 
    glLineWidth(7);
    glColor3f(105 / 225., 105 / 225., 105 / 225.);
    glBegin(GL_LINE_LOOP);
    glVertex2i(layerx - 100, layery);
    glVertex2i(layerx - 100, layery + 40);
    glEnd();

    circle(layerx - 100, layery + 40, 5);

    glLineWidth(8);
    glColor3f(105 / 225., 105 / 225., 105 / 225.);
    glBegin(GL_LINE_LOOP);
    glVertex2i(layerx - 100, layery + 40);
    glVertex2i(layerx - 60, layery + 60);
    glEnd();

    //right
    glLineWidth(7);
    glColor3f(105 / 225., 105 / 225., 105 / 225.);
    glBegin(GL_LINE_LOOP);
    glVertex2i(layerx + 100, layery);
    glVertex2i(layerx + 100, layery + 40);
    glEnd();

    circle(layerx + 100, layery + 40, 5);

    glLineWidth(8);
    glColor3f(105 / 225., 105 / 225., 105 / 225.);
    glBegin(GL_LINE_LOOP);
    glVertex2i(layerx + 100, layery + 40);
    glVertex2i(layerx + 60, layery + 60);
    glEnd();

    /*edges*/
    //left
    glLineWidth(10);
    glColor3f(192 / 225., 192 / 225., 192 / 225.);
    glBegin(GL_LINE_LOOP);
    glVertex2i(layerx - 60, layery + 35);
    glVertex2i(layerx - 60, layery + 85);
    glEnd();

    circle(layerx - 60, layery + 85, 5);
    circle(layerx - 60, layery + 35, 5);

    //right
    glLineWidth(10);
    glColor3f(192 / 225., 192 / 225., 192 / 225.);
    glBegin(GL_LINE_LOOP);
    glVertex2i(layerx + 60, layery + 35);
    glVertex2i(layerx + 60, layery + 85);
    glEnd();

    circle(layerx + 60, layery + 85, 5);
    circle(layerx + 60, layery + 35, 5);

    /*rope*/
    //left
    for (int i = 51; i > 0; i -= 8)
    {
        glLineWidth(8);
        glColor3f(153 / 225., 121 / 225., 80 / 225.);
        glBegin(GL_LINE_LOOP);
        glVertex2i(layerx - i, layery + 40);
        glVertex2i(layerx - i, layery + 80);
        glEnd();

        circle(layerx - i, layery + 80, 4);
        circle(layerx - i, layery + 40, 4);
    }

    //right
    for (int i = 51; i > 0; i -= 8)
    {
        glLineWidth(8);
        glColor3f(153 / 225., 121 / 225., 80 / 225.);
        glBegin(GL_LINE_LOOP);
        glVertex2i(layerx + i, layery + 40);
        glVertex2i(layerx + i, layery + 80);
        glEnd();

        circle(layerx + i, layery + 80, 4);
        circle(layerx + i, layery + 40, 4);
    }
}

void drawDiamond()
{
    //up triangle
    glBegin(GL_TRIANGLES);
    glColor3f(1, 1, 0); // yellow
    glVertex2f(coodiax - 15, coodiay - 15);
    glColor3f(0, 0, 1); // blue
    glVertex2f(coodiax + 15, coodiay - 15);
    glColor3f(1, 1, 1); // white
    glVertex2f(coodiax, coodiay + 20);
    glEnd();

    //down triangle
    glBegin(GL_TRIANGLES);
    glColor3f(1, 1, 0); // yellow
    glVertex2f(coodiax - 15, coodiay - 15);
    glColor3f(0, 0, 1); // blue
    glVertex2f(coodiax + 15, coodiay - 15);
    glColor3f(225 / 225., 105 / 225., 180 / 225.); // pink
    glVertex2f(coodiax, coodiay - 40);
    glEnd();
}

void drawSapphire()
{
    //the circle
    glColor3f(30 / 255., 144 / 255., 1);
    circle(coosapx, coosapy, 15);

    //inside triangle
    glBegin(GL_TRIANGLES);
    glColor3f(135 / 225., 206 / 225., 250 / 225.);
    glVertex2f(coosapx - 10, coosapy - 8);
    glColor3f(135 / 225., 206 / 225., 250 / 225.);
    glVertex2f(coosapx + 10, coosapy - 8);
    glColor3f(135 / 225., 206 / 225., 250 / 225.);
    glVertex2f(coosapx, coosapy + 10);
    glEnd();

    //the lines that connects the triangle and the circle
    glLineWidth(2);
    glColor3f(0, 0, 1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(coosapx, coosapy + 10);
    glVertex2f(coosapx, coosapy + 15);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(coosapx - 9, coosapy - 8);
    glVertex2f(coosapx - 12, coosapy - 10);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(coosapx + 9, coosapy - 8);
    glVertex2f(coosapx + 12, coosapy - 10);
    glEnd();


}

void drawAmethyst()
{
    //the hexagon
    glBegin(GL_POLYGON);
    glColor3f(102 / 225., 53 / 225., 147 / 225.);
    glVertex2f(cooamtx, cooamty + 24);
    glVertex2f(cooamtx - 15, cooamty + 15);
    glVertex2f(cooamtx - 15, cooamty - 15);
    glVertex2f(cooamtx, cooamty - 24);
    glVertex2f(cooamtx + 15, cooamty - 15);
    glVertex2f(cooamtx + 15, cooamty + 15);
    glEnd();

    //the rectangle inside
    glColor3f(72 / 225., 23 / 225., 117 / 225.);
    glRectf(cooamtx - widthA / 2, cooamty - heightA / 2, cooamtx + widthA / 2, cooamty + heightA / 2);
}

void drawRuby()
{
    //the circle
    glColor3f(220 / 255., 20 / 255., 60 / 255.);
    circle(coorubx, cooruby, 15);

    //the square inside
    glColor3f(178 / 255., 34 / 255., 34 / 255.);
    glRectf(coorubx - widthR / 2, cooruby - heightR / 2, coorubx + widthR / 2, cooruby + heightR / 2);

}

void drawBone()
{
    //the body
    glLineWidth(7);
    glColor3f(240 / 225., 230 / 225., 140 / 225.);
    glBegin(GL_LINE_LOOP);
    glVertex2f(coobonx + 150, coobony + 22);    //150 22
    glVertex2f(coobonx + 150, coobony - 22);    //150 -22
    glEnd();

    //the two circles on the up and down
    circle(coobonx + 146, coobony + 22, 4);     //146 22
    circle(coobonx + 154, coobony + 22, 4);     //154 22
    circle(coobonx + 146, coobony - 22, 4);     //146 -22
    circle(coobonx + 154, coobony - 22, 4);     //154 -22
}

int ifTouchedround(int y)       //the hook hit the ground or not
{
    if (y == WINDOW_HEIGHT / -2)
    {
        touchedgr = 1;
    }

    if (touchedgr == 1)
    {
        if (y > 150 && y < 160)
        {
            hD = 1;
            touchedgr = 0;
        }
    }

    return touchedgr;
}

int ifTouchedDia(int x, int y)      //the hook hit the diamond or not
{
    if (coodiax - 15 <= x && x <= coodiax + 15)
    {
        if (coodiay - 40 <= y && y <= coodiay + 20)
        {
 
            touchedgr = 1;
            return 1;
        }
    }

    return 0;
     
}

int ifTouchedSap(int x, int y)      //the hook hit the sapphire or not
{
    if (coosapx - 15 <= x && x <= coosapx + 15)
    {
        if (coosapy - 15 <= y && y <= coosapy + 15)
        {
       
            touchedgr = 1;
            return 1;
        }
    }

    return 0;
}

int ifTouchedAmt(int x, int y)      //the hook hit the amethyst or not
{
    if (cooamtx - 20 <= x && x <= cooamtx + 20)
    {
        if (cooamty - 29 <= y && y <= cooamty + 29)
        {
            
            touchedgr = 1;
            return 1;
        }
    }

    return 0;
}

int ifTouchedRub(int x, int y)      //the hook hit the ruby or not
{
    if (coorubx - 20 <= x && x <= coorubx + 20)
    {
        if (cooruby - 20 <= y && y <= cooruby + 20)
        {
            
            touchedgr = 1;
            return 1;
        }
    }

    return 0;
}

int ifTouchedBon(int x, int y)      //the hook hit the bone or not
{
    if (coobonx - 4 <= x - 150 && x - 150 <= coobonx + 4)
    {
        if (coobony - 25 <= y + 22 && y - 22 <= coobony + 25)
        {
       
            touchedgr = 1;
            return 1;
        }
    }

    return 0;
}

void drawHook()
{
    //rope that connects the hook and the roller
    glLineWidth(5);
    glColor3f(153 / 225., 121 / 225., 80 / 225.);
    glBegin(GL_LINE_LOOP);
    glVertex2i(layerx, layery + 45);
    glVertex2i(xH, yH);
    glEnd();

    //hook
    if (touchedgr == 1)
        glColor3f(0, 1, 0);
    else
        glColor3f(125 / 225., 125 / 225., 125 / 225.);
    circle_wire(xH, yH - rH, rH);

    //prediction
    if (ifTouchedround(yH - rH) != 1 || ifTouchedround(yH - rH) == 2)
    {
        glColor3f(1, 0, 0);
        glBegin(GL_LINE_LOOP);
        glVertex2i(xH - 6, yH - rH - 100);
        glVertex2i(xH + 5, yH - rH - 100);
        glEnd();
        glBegin(GL_LINE_LOOP);
        glVertex2i(xH, yH - rH - 105);
        glVertex2i(xH, yH - rH - 94);
        glEnd();
    }

}




void displaygameover()      //the 'game over' screen
{
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1, 1, 0);
    vprint2(-200, 200, 0.5, "GAME OVER");

    vprint(-200, 100, GLUT_BITMAP_8_BY_13, "The Toal Coins You Have Collected : %d", cntbon * BONCOST + cntrub * RUBCOST + cntamt * AMTCOST + cntsap * SAPCOST + cntdia * DIACOST);

    vprint(-300, 0, GLUT_BITMAP_8_BY_13, "The Bones You Pulled : %d (%d)", cntbon, cntbon * BONCOST);
    vprint(-300, -20, GLUT_BITMAP_8_BY_13, "The Rubies You Pulled : %d (%d)", cntrub, cntrub * RUBCOST);
    vprint(-300, -40, GLUT_BITMAP_8_BY_13, "The Amethysts You Pulled : %d (%d)", cntamt, cntamt * AMTCOST);
    vprint(-300, -60, GLUT_BITMAP_8_BY_13, "The Sapphires You Pulled : %d (%d)", cntsap, cntsap * SAPCOST);
    vprint(-300, -80, GLUT_BITMAP_8_BY_13, "The Diamond You Pulled : %d (%d)", cntdia, cntdia * DIACOST);

    if (cntdia > cntbon && cntdia > cntrub && cntdia > cntamt && cntdia > cntsap)
        vprint(-300, -150, GLUT_BITMAP_8_BY_13, "Your Wife Will Be So Happy To See You Collect So Many Diamonds");

    else if (cntsap * SAPCOST > cntbon && cntsap > cntrub && cntsap > cntamt && cntsap > cntdia)
        vprint(-300, -150, GLUT_BITMAP_8_BY_13, "You Must Be A High Class Person Since You Collected So Many Sapphires");

    else if (cntamt > cntbon && cntamt > cntrub && cntamt > cntsap && cntamt > cntdia)
        vprint(-300, -150, GLUT_BITMAP_8_BY_13, "You Will Be So Rich In These Purples Since You Collected So Many Amethysts");

    else if (cntrub > cntbon && cntrub > cntamt && cntrub > cntsap && cntrub > cntdia)
        vprint(-300, -150, GLUT_BITMAP_8_BY_13, "I Hope You Would Get A Lot Of Luck In This Life With This Much Rubies You Collected");

    else if (cntbon > cntrub && cntbon > cntamt && cntbon > cntsap && cntbon > cntdia)
        vprint(-300, -150, GLUT_BITMAP_8_BY_13, "People Should Call You A Junk Collector Since You Pulled Bones The Most In A Mine Full Of Gems");

    else
        vprint(-300, -150, GLUT_BITMAP_8_BY_13, "I See You Collected Some Stuff At The Same Amount");

    vprint(-200, -200, GLUT_BITMAP_8_BY_13, "Please Press <F1> To Start Where You Left");
}

void displaygameon()        //the 'game on' screen
{
    glClearColor(1, 215 / 225., 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    //the roller
    drawRoller();

    //background
    drawBackGround();

    //Diamond
    drawDiamond();

    //Sapphire
    drawSapphire();

    //Amethyst
    drawAmethyst();

    //Ruby
    drawRuby();

    //Bone
    drawBone();

    //Hook
    drawHook();

    //Info Card
    drawInfoCard();
}

//
//
// To display onto window using OpenGL commands
//
void display() {

    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    if (gamemode == ON)
    {
        displaygameon();
    }

    else if (gamemode == OVER)
    {
        displaygameover();
    }

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);


    // space to throw the hook
    if (key == 32)
        if (hD == 1)
            hD = -1;

    // to rotate the roller
    if (right || left)
        hD = 0;

    



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP:
        up = true;
        break;
    case GLUT_KEY_DOWN:
        down = true;
        break;
    case GLUT_KEY_LEFT:
        left = true;
        break;
    case GLUT_KEY_RIGHT:
        right = true;
        break;
    case GLUT_KEY_F1 :      //F1 to pause/resume/restart
        {

            if (gamemode == ON)     //F1 to pause/resume the game
            {
                timergo *= -1;
                TIMER_ON == 0;
            }

            else if (gamemode == OVER)      //F1 to restart the game
            {
                gamemode = ON;

                second = 19;
                splitsecond = 99;

                if (total >= bestscore)
                {
                    bestscore = total;
                }

                cntbon = cntdia = cntrub = cntsap = cntamt = 0;

            }

        }
        break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP:
        up = false;
        break;
    case GLUT_KEY_DOWN:
        down = false;
        break;
    case GLUT_KEY_LEFT:
        left = false;
        break;
    case GLUT_KEY_RIGHT:
        right = false;
        break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);

    if (second != 0 || splitsecond != 0)        //checks the time is up or not (game is on)
    {
        if (timergo == 1)
        {
            if (left)
            {
                if (layerx >= -380)
                {
                    if (hD)                     
                    {
                        layerx--;               //rotates the hook to left
                        xH--;
                    }
                }
            }

            if (right)
            {
                if (layerx <= 380)
                {
                    if (hD)
                    {
                        layerx++;              //rotates the hook to right
                        xH++;
                    }
                }
            }

            if (hD == -1)
            {
                if (!left && !right)
                {
                    if (yH - rH >= WINDOW_HEIGHT / -2)
                    {
                        if (ifTouchedround(yH - rH) != 1 || ifTouchedround(yH - rH) == 2)
                        {
                            yH -= 2;        //throws the hook
                        }

                        if (yH - rH < 164)
                        {
                            if (ifTouchedround(yH - rH))
                            {
                                yH += 10;       //takes the hook back
                            }

                            if (ifTouchedDia(xH - rH, yH - rH))         //hits and pulls the diamond
                            {
                                coodiay = yH;
                                yH += 6;

                                if (yH > 149)
                                {
                                    coodiax = 600;
                                    cntdia++;
                                    srand(time(NULL));
                                    coodiay = rand() % 25 - 150;
                                    ifTouchedDia(xH - rH, yH - rH) == false;
                                }
                            }

                            else if (ifTouchedSap(xH - rH, yH - rH))        //hits and pulls the sapphire
                            {
                                coosapy = yH;
                                yH += 6;

                                if (yH > 149)
                                {
                                    coosapx = 544;
                                    cntsap++;
                                    srand(time(NULL));
                                    coosapy = rand() % 25 - 125;
                                    ifTouchedSap(xH - rH, yH - rH) == false;
                                }
                            }

                            else if (ifTouchedAmt(xH - rH, yH - rH))        //hits and pulls the amethyst
                            {
                                cooamty = yH;
                                yH += 6;

                                if (yH > 149)
                                {
                                    cooamtx = 544;
                                    cntamt++;
                                    srand(time(NULL));
                                    cooamty = rand() % 70 - 125;
                                    ifTouchedAmt(xH - rH, yH - rH) == false;
                                }
                            }

                            else if (ifTouchedRub(xH - rH, yH - rH))        //hits and pulls the ruby
                            {
                                cooruby = yH;
                                yH += 6;

                                if (yH > 149)
                                {
                                    coorubx = 515;
                                    cntrub++;
                                    srand(time(NULL));
                                    cooruby = rand() % 50 - 100;
                                    ifTouchedRub(xH - rH, yH - rH) == false;
                                }
                            }

                            else if (ifTouchedBon(xH - rH, yH - rH))        //hits and pulls the bone
                            {
                                coobony = yH;
                                yH += 6;

                                if (yH > 149)
                                {
                                    coobonx = 533;
                                    cntbon++;
                                    srand(time(NULL));
                                    coobony = rand() % 175 - 50;
                                    ifTouchedBon(xH - rH, yH - rH) == false;
                                }
                            }
                        }
                    }
                }
            }

            if (coodiax >= -515)            //sends back or moves the diamond
            {
                if (!ifTouchedDia(xH - rH, yH - rH))
                {
                    coodiax -= 5;
                }

                if (coodiax == -515)
                    coodiax = 600;
            }

            if (coosapx >= -515)             //sends back or moves the sapphire
            {
                if (!ifTouchedSap(xH - rH, yH - rH))
                {
                    coosapx -= 2;

                }

                if (coosapx == -514)
                    coosapx = 544;
            }

            if (cooamtx >= -515)            //sends back or moves the amethyst
            {
                if (!ifTouchedAmt(xH - rH, yH - rH))
                {
                    cooamtx --;

                }

                if (cooamtx == -515)
                    cooamtx = 533;
            }

            if (coorubx >= -515)            //sends back or moves the ruby
            {
                if (!ifTouchedRub(xH - rH, yH - rH))
                {
                    coorubx--;

                }

                if (coorubx == -515)
                    coorubx = 515;
            }

            if (coobonx >= -515 - 150)          //sends back or moves the bone
            {
                if (!ifTouchedBon(xH - rH, yH - rH))
                {
                    coobonx -= 1;

                }

                if (coobonx == -515 - 150)
                    coobonx = 515 - 150;
            }



            if (splitsecond > 0)        //timer
            {
                splitsecond--;

                if (splitsecond == 0)
                {
                    if (second > 0)
                    {
                        second--;
                        splitsecond = 99;
                    }

                }
            }


            // to refresh the window it calls display() function
            glutPostRedisplay(); // display()
        }
    }

    else
        gamemode = OVER;
    
    
}
#endif


void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(260, 90);
    glutCreateWindow("Gem Collector by Hasan Kerem Demirel");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}