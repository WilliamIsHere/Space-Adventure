#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
#include<string.h>
#include <windows.h>
#include <Parallax.h>
#include <instruction.h>
#include <Sounds.h>
#include <files.h>

#define PI 3.14159
#define GAME_SCREEN 0			//Constant to identify background color
#define MENU_SCREEN 4
#define MAX_STONES  100
#define MAX_STONE_TYPES 5
#define stoneRotationSpeed 20
#define SPACESHIP_SPEED 20

//parallax * P = new parallax();
instruction * I = new instruction();
Sounds * S = new Sounds();
files * F = new files();

int stoneTranslationSpeed=5;

GLint m_viewport[4];
GLint CI=0;
int x,y; //W
int i;
int randomStoneIndices[100];
int index;
int alienLife=100;
int GameLvl= 1;
float mouseX ,mouseY ;				//Cursor coordinates;
float LaserAngle=0 ,stoneAngle =0,lineWidth = 1;
float xOne=0,yOne=0;				//Spaceship coordinates
float xStone[MAX_STONES] ,yStone[MAX_STONES];//coordinates of stones
float xStart = 1200;				//Health bar starting coodinate
GLint stoneAlive[MAX_STONES];		//check to see if stone is killed
float screenWidth = GetSystemMetrics(SM_CYSCREEN);
float screenHeight = GetSystemMetrics(SM_CYSCREEN);


bool mButtonPressed= false,startGame=false,gameOver=false;		//boolean values to check state of the game
bool startScreen = true ,nextScreen=false,previousScreen=false;
bool gameQuit = false,instructionsGame = false, optionsGame = false;


GLfloat a[][2]={0,-50, 70,-50, 70,70, -70,70};
GLfloat LightColor[][3]={1,1,0,   0,1,1,   0,1,0};
GLfloat AlienBody[][2]={{-4,9}, {-6,0}, {0,0}, {0.5,9}, {0.15,12}, {-14,18}, {-19,10}, {-20,0},{-6,0}};
GLfloat AlienCollar[][2]={{-9,10.5}, {-6,11}, {-5,12}, {6,18}, {10,20}, {13,23}, {16,30}, {19,39}, {16,38},
						  {10,37}, {-13,39}, {-18,41}, {-20,43}, {-20.5,42}, {-21,30}, {-19.5,23}, {-19,20},
						  {-14,16}, {-15,17},{-13,13},  {-9,10.5}};
GLfloat ALienFace[][2]={{-6,11}, {-4.5,18}, {0.5,20}, {0.,20.5}, {0.1,19.5}, {1.8,19}, {5,20}, {7,23}, {9,29},
						{6,29.5}, {5,28}, {7,30}, {10,38},{11,38}, {11,40}, {11.5,48}, {10,50.5},{8.5,51}, {6,52},
						{1,51}, {-3,50},{-1,51}, {-3,52}, {-5,52.5}, {-6,52}, {-9,51}, {-10.5,50}, {-12,49}, {-12.5,47},
						{-12,43}, {-13,40}, {-12,38.5}, {-13.5,33},{-15,38},{-14.5,32},  {-14,28}, {-13.5,33}, {-14,28},
						{-13.8,24}, {-13,20}, {-11,19}, {-10.5,12}, {-6,11} } ;
GLfloat ALienBeak[][2]={{-6,21.5}, {-6.5,22}, {-9,21}, {-11,20.5}, {-20,20}, {-14,23}, {-9.5,28}, {-7,27}, {-6,26.5},
						{-4.5,23}, {-4,21}, {-6,19.5}, {-8.5,19}, {-10,19.5}, {-11,20.5} };

void display();
void StoneGenerate();

void SetDisplayMode(int modeToDisplay) {

		switch(modeToDisplay){
		case GAME_SCREEN:
           // P->drawSquare(1200,700);
		   // P->parallaxInit("images/plx.png");
		    glClearColor(0, 0, 0, 1);
		    //P->parallaxInit("images/plx.png");
		    break;
		case MENU_SCREEN :
		    glClearColor(1, 0 , 0, 1);

		    break;
	}
}
void initializeStoneArray() {
	//random stones index

	for(int i = 0;i < MAX_STONES ;i++) {
		randomStoneIndices[i]=rand()%MAX_STONE_TYPES;
		stoneAlive[i]=true;
	}

	xStone[0] = -(200*MAX_STONES)-600;             //START LINE for stone appearance

	for(int i = 0;i<MAX_STONES ;i++) {				//ramdom appearance yIndex for each stone
		yStone[i]=rand()%600;
		if(int(yStone[i])%2)
			yStone[i]*=-1;
		xStone[i+1] = xStone[i] + 200;				//xIndex of stone aligned with 200 units gap
	}
}
void DrawAlienBody()
{
	//glColor3f(0,1,0);   //BODY color
	//P->parallaxInit("images/plx.png");
	glBegin(GL_POLYGON);
	for(i=0;i<=8;i++)
		glVertex2fv(AlienBody[i]);
	glEnd();

	glColor3f(0,0,0);			//BODY Outline
	glLineWidth(1);
	glBegin(GL_LINE_STRIP);
	for(i=0;i<=8;i++)
		glVertex2fv(AlienBody[i]);
	glEnd();

	glBegin(GL_LINES);                //BODY effect
		glVertex2f(-13,11);
		glVertex2f(-15,9);
	glEnd();
}
void DrawAlienCollar()
{
	glColor3f(1,0,0);				//COLLAR
	glBegin(GL_POLYGON);
	for(i=0;i<=20 ;i++)
		glVertex2fv(AlienCollar[i]);
	glEnd();

	glColor3f(0,0,0);				//COLLAR outline
	glBegin(GL_LINE_STRIP);
	for(i=0;i<=20 ;i++)
		glVertex2fv(AlienCollar[i]);
	glEnd();
}
void DrawAlienFace()
{
	glColor3f(0,.5,0);  //face color
	glBegin(GL_POLYGON);
	for(i=0;i<=42 ;i++)
		glVertex2fv(ALienFace[i]);
	glEnd();

	glColor3f(0,0,0);				//FACE outline
	glBegin(GL_LINE_STRIP);
	for(i=0;i<=42 ;i++)
		glVertex2fv(ALienFace[i]);
	glEnd();

	glBegin(GL_LINE_STRIP);      //EAR effect
		glVertex2f(3.3,22);
		glVertex2f(4.4,23.5);
		glVertex2f(6.3,26);
	glEnd();
}
void DrawAlienBeak()
{
	glColor3f(1,1,0);				//BEAK color
	glBegin(GL_POLYGON);
	for(i=0;i<=14 ;i++)
		glVertex2fv(ALienBeak[i]);
	glEnd();

	glColor3f(0,0,0);				//BEAK outline
	glBegin(GL_LINE_STRIP);
	for(i=0;i<=14 ;i++)
		glVertex2fv(ALienBeak[i]);
	glEnd();
}
void DrawAlienEyes()
{

	glColor3f(0,1,1);

	glPushMatrix();
	glRotated(-10,0,0,1);
	glTranslated(-6,32.5,0);      //Left eye
	glScalef(2.5,4,0);
	glutSolidSphere(1,20,30);
	glPopMatrix();

	glPushMatrix();
	glRotated(-1,0,0,1);
	glTranslated(-8,36,0);							//Right eye
	glScalef(2.5,4,0);
	glutSolidSphere(1,100,100);
	glPopMatrix();
}
void DrawAlien()
{
	DrawAlienBody();
	DrawAlienCollar();
	DrawAlienFace();
	DrawAlienBeak();
	DrawAlienEyes();
}
void DrawSpaceshipBody()
{
	glColor3f(1,0,0);				//BASE

	glPushMatrix();
	glScalef(70,20,1);
	glutSolidSphere(1,50,50);
	glPopMatrix();

	glPushMatrix();							//LIGHTS
	glScalef(3,3,1);
	glTranslated(-20,0,0);			//1
	glColor3fv(LightColor[(CI+0)%3]);
	glutSolidSphere(1,1000,1000);
	glTranslated(5,0,0);					//2
	glColor3fv(LightColor[(CI+1)%3]);
	glutSolidSphere(1,1000,1000);
	glTranslated(5,0,0);					//3
	glColor3fv(LightColor[(CI+2)%3]);
	glutSolidSphere(1,1000,1000);
	glTranslated(5,0,0);					//4
	glColor3fv(LightColor[(CI+0)%3]);
	glutSolidSphere(1,1000,1000);
	glTranslated(5,0,0);					//5
	glColor3fv(LightColor[(CI+1)%3]);
	glutSolidSphere(1,1000,1000);
	glTranslated(5,0,0);					//6
	glColor3fv(LightColor[(CI+2)%3]);
	glutSolidSphere(1,1000,1000);
	glTranslated(5,0,0);					//7
	glColor3fv(LightColor[(CI+0)%3]);
	glutSolidSphere(1,1000,1000);
	glTranslated(5,0,0);					//8
	glColor3fv(LightColor[(CI+1)%3]);
	glutSolidSphere(1,1000,1000);
	glTranslated(5,0,0);					//9
	glColor3fv(LightColor[(CI+2)%3]);
	glutSolidSphere(1,1000,1000);

	glPopMatrix();
}
void DrawSteeringWheel()
{/*
	glPushMatrix();
	glLineWidth(3);
	glColor3f(0.20,0.,0.20);
	glScalef(7,4,1);
	glTranslated(-1.9,5.5,0);
	glutWireSphere(1,8,8);
	glPopMatrix();
*/
}
void DrawSpaceshipDoom()
{
	glColor4f(0.7,1,1,0.0011);
	glPushMatrix();
	glTranslated(0,30,0);
	glScalef(35,50,1);
	glutSolidSphere(1,50,50);
	glPopMatrix();
}
void DrawSpaceShipLazer() {

	glColor3f(1, 0, 0);
	glPushMatrix();
	glBegin(GL_POLYGON);           //Lazer stem
		glVertex2f(-55 ,10);
		glVertex2f(-55 ,30);
		glVertex2f(-50, 30);
		glVertex2f(-50 ,10);
	glEnd();

	float xMid =0,yMid =0;
	//Mid point of the lazer horizontal
	xMid = (55+50)/2.0;
	yMid = (25+35)/2.0;

	//Rotating about the point ,20
	glTranslated(-xMid, yMid, 0);
	glRotated(LaserAngle, 0, 0 ,1);
	glTranslated(xMid , -yMid ,0);

	//find mid point of top of lazer stem
	float midPoint = -(55+50)/2.0;

	glBegin(GL_POLYGON);           //Lazer horizontal stem
		glVertex2f(midPoint + 10 ,25);
		glVertex2f(midPoint + 10 ,35);
		glVertex2f(midPoint - 10 ,35);
		glVertex2f(midPoint - 10 ,25);
	glEnd();

	glPopMatrix();
}
void DrawLazerBeam() {

	float xMid = -(55+50)/2.0;
	float yMid = (25+35)/2.0;

	float mouseXEnd = -((- mouseX) + xOne);
	float mouseYEnd = -((- mouseY) + yOne);
	glLineWidth(5);   //----Laser beam width

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
		glVertex2f(xMid ,yMid);
		glVertex2f(mouseXEnd ,mouseYEnd);
	glEnd();
	glLineWidth(1);
}
void DrawStone(int StoneIndex)
{
	glPushMatrix();
	glLoadIdentity();
	switch(StoneIndex)                           //CHANGE INDEX VALUE FOR DIFFERENT STONE VARIETY;
	{
	case 0:

		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glColor3f(0.4f, 0.0f, 0.0f);
		glScalef(35,35,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(60,10,1);
		glutSolidSphere(1,5,50);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(10,60,1);
		glutSolidSphere(1,5,50);
		break;

	case 1:
		glColor3f(1.0f, 0.8f, 0.8f);
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(15,20,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(40,5,1);
		glutSolidSphere(1,5,50);
		break;

	case 2:
		glColor3f(0.2f, 0.2f, 0.0f);
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(60,25,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(25,60,1);
		glutSolidSphere(1,9,50);

		break;

	case 3:
		glColor3f(0.8f, 0.8f, 0.1f);
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(35,10,1);
		glutSolidSphere(1,10,7);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(50,20,1);
		glutSolidSphere(1,5,50);
		break;

	case 4:
		glColor3f(0.26f, 0.26f, 0.26f);
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(10,55,1);
		glutSolidSphere(1,9,50);

		glLoadIdentity();
		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(20,10,1);
		glutSolidSphere(1,9,50);
		glLoadIdentity();

		glTranslated(xStone[index] , yStone[index] ,0);
		glRotatef(stoneAngle+45 ,0, 0, 1);
		glTranslated(0, 0, 0);
		glScalef(25,10,1);
		glutSolidSphere(1,9,50);
		break;

	}

	glPopMatrix();
}
bool checkIfSpaceShipIsSafe() {
		for(int i =0 ;i<MAX_STONES-1 ;i++) {
		if(stoneAlive[i]&((xOne >= (xStone[i]/2 -70) && xOne <= (xStone[i]/2 + 70) && yOne >= (yStone[i]/2 -18 ) && yOne <= (yStone[i]/2 + 53)) || (yOne <= (yStone[i]/2 - 20) && yOne >= (yStone[i]/2 - 90) && xOne >= (xStone[i]/2 - 40) && xOne <= (xStone[i]/2 + 40))))
		{
			S->playSound("sounds/Explosion.wav");//W
			stoneAlive[i]=0;
			return false;
		}
	}
	return true;
}
void SpaceshipCreate(){
	glPushMatrix();
	glTranslated(xOne,yOne,0);
	if(!checkIfSpaceShipIsSafe() && alienLife ){
		alienLife-=10;
		xStart -= 230;
	}
	DrawSpaceshipDoom();
	glPushMatrix();
	glTranslated(4,19,0);
	DrawAlien();
	glPopMatrix();
	DrawSteeringWheel();
	DrawSpaceshipBody();
	DrawSpaceShipLazer();
	if(mButtonPressed) {
		DrawLazerBeam();
	}
	glEnd();
	glPopMatrix();
}
void DisplayHealthBar() {

	glColor3f(1 ,0 ,0);
	glBegin(GL_POLYGON);
		glVertex2f(-xStart ,700);
		glVertex2f(1200 ,700);
		glVertex2f(1200 ,670);
		glVertex2f(-xStart, 670);
	glEnd();
	char temp[40];
	glColor3f(0 ,0 ,1);
	sprintf(temp,"SCORE = %d", F->Score);
I->	displayText(-1100 ,600 ,0.4 ,temp);//<---display variable score ?
	sprintf(temp,"  LIFE = %d",alienLife);
I->	displayText(800 ,600 ,0.4 ,temp);
	sprintf(temp,"  LEVEL : %d",GameLvl);
I->	displayText(-100 ,600 ,0.4 ,temp);
	glColor3f(1 ,0 ,0);
}
void startScreenDisplay()
{
	glLineWidth(50);
	SetDisplayMode(MENU_SCREEN);

	glColor3f(0,0,0);
	glBegin(GL_LINE_LOOP);               //Border
		glVertex3f(-750 ,-500 ,0.5);
		glVertex3f(-750 ,550 ,0.5);
		glVertex3f(750 ,550 ,0.5);
		glVertex3f(750 ,-500, 0.5);
	glEnd();

	glLineWidth(1);
// /*
	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);				//START GAME PLOYGON
		glVertex3f(-200 ,300 ,0.5);
		glVertex3f(-200 ,400 ,0.5);
		glVertex3f(200 ,400 ,0.5);
		glVertex3f(200 ,300, 0.5);
	glEnd();
// */
	glBegin(GL_POLYGON);				//INSTRUCTIONS POLYGON
		glVertex3f(-200, 50 ,0.5);
		glVertex3f(-200 ,150 ,0.5);
		glVertex3f(200 ,150 ,0.5);
		glVertex3f(200 ,50, 0.5);
	glEnd();

	glBegin(GL_POLYGON);				//QUIT POLYGON
		glVertex3f(-200 ,-200 ,0.5);
		glVertex3f(-200 ,-100 ,0.5);
		glVertex3f(200, -100 ,0.5);
		glVertex3f(200, -200 ,0.5);
	glEnd();

	if(mouseX>=-100 && mouseX<=100 && mouseY>=150 && mouseY<=200){
		 glColor3f(0 ,0 ,1) ;
		if(mButtonPressed){
			startGame = true ;
			gameOver = false;
			mButtonPressed = false;
		}
	} else
		glColor3f(0 , 0, 0);

I->	displayText(-100 ,340 ,0.4 ,"Start Game");

	if(mouseX>=-100 && mouseX<=100 && mouseY>=30 && mouseY<=80) {
            glColor3f(0 ,0 ,1) ;//A
		if(mButtonPressed){
			instructionsGame = true ;
			mButtonPressed = false;
		}
	} else
		 glColor3f(0 , 0, 0);
I->	displayText(-120 ,80 ,0.4 ,"Instructions");

	if(mouseX>=-100 && mouseX<=100 && mouseY>=-90 && mouseY<=-40){
            glColor3f(0 ,0 ,1) ;
		if(mButtonPressed){
			gameQuit = true ;
			mButtonPressed = false;
		}
	}
	else
		glColor3f(0 , 0, 0);
I->	displayText(-100 ,-170 ,0.4 ,"    Quit");

}
void GameScreenDisplay()
{
	SetDisplayMode(GAME_SCREEN);
	DisplayHealthBar();
	glScalef(2, 2 ,0);
	if(alienLife){
		SpaceshipCreate();
		F->file();
	}
	else {
		gameOver=true;
		instructionsGame = false;
		startScreen = false;
	}								//<----------------------gameover screen

	StoneGenerate();

}

void GameOverScreen()
{
	SetDisplayMode(MENU_SCREEN);
	glColor3f(0,0,0);
	glLineWidth(50);

	glBegin(GL_LINE_LOOP);               //Border
		glVertex3f(-650 ,-500 ,0.5);
		glVertex3f(-650 ,520 ,0.5);
		glVertex3f(650 ,520 ,0.5);
		glVertex3f(650 ,-500, 0.5);
	glEnd();


	glLineWidth(1);
	stoneTranslationSpeed=5;
	glColor3f(0, 1, 0);
	glBegin(GL_POLYGON);				//GAME OVER
		glVertex3f(-550 ,810,0.5);
		glVertex3f(-550 ,610 ,0.5);
		glVertex3f(550 ,610 ,0.5);
		glVertex3f(550 ,810, 0.5);
	glEnd();

	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);				//RESTART POLYGON
		glVertex3f(-200, 50 ,0.5);
		glVertex3f(-200 ,150 ,0.5);
		glVertex3f(200 ,150 ,0.5);
		glVertex3f(200 ,50, 0.5);
	glEnd();

	glBegin(GL_POLYGON);				//QUIT POLYGON
		glVertex3f(-200 ,-200 ,0.5);
		glVertex3f(-200 ,-100 ,0.5);
		glVertex3f(200, -100 ,0.5);
		glVertex3f(200, -200 ,0.5);
	glEnd();


I->	displayText(-300 ,640 ,0.4 ,"G A M E    O V E R ! ! !");
	glColor3f(0 , 0, 1);
	char temp[40];

	sprintf(temp,"Score : %d",F->Score);
I->	displayText(-100 ,340 ,0.4 ,temp);
F->	readFromFile();
	char temp2[40];
	if(atoi(F->highScore) < F->Score){
		F->writeIntoFile();
		sprintf(temp2 ,"Highest Score :%d" ,F->Score);
	} else
		sprintf(temp2 ,"Highest Score :%s" ,F->highScore);

I->	displayText(-250 ,400 ,0.4 ,temp2);

	if(mouseX>=-100 && mouseX<=100 && mouseY>=25 && mouseY<=75){
		 glColor3f(0 ,0 ,1);
		if(mButtonPressed){                                                       //Reset game default values
			startGame = true ;
			gameOver=false;
			mButtonPressed = false;
			initializeStoneArray();
			alienLife=100;
			xStart=1200;
            F->Score=0;
			GameLvl=1;
			GameScreenDisplay();
		}
	} else
		glColor3f(0 , 0, 1);
I->	displayText(-70 ,80 ,0.4 ,"Restart");

	if(mouseX>=-100 && mouseX<=100 && mouseY>=-100 && mouseY<=-50){
	//	glColor3f(0 ,0 ,1);
		if(mButtonPressed){
			exit(0);
			mButtonPressed = false;
		}
	}
	else
		glColor3f(0 , 0, 1);
I->	displayText(-100 ,-170 ,0.4 ," Quit");

}
void StoneGenerate(){

		if(xStone[0]>=1200){      //If the last screen hits the end of screen then go to Nxt lvl
			GameLvl++;
			stoneTranslationSpeed+=3;
			F->Score+=50;
			initializeStoneArray();
			GameScreenDisplay();
		}

	for(int i=0; i<MAX_STONES ;i++){
		index = i;

		if(mouseX <= (xStone[i]/2+20) && mouseX >=(xStone[i]/2-20) && mouseY >= (yStone[i]/2-20) && mouseY <= (yStone[i]/2+20) && mButtonPressed){
			if(stoneAlive[i]){   // IF ALIVE KILL STONE
				stoneAlive[i]=0;
				F->Score++;
				if(F->Score%3==0) {
					stoneTranslationSpeed+=1;			//<--------------Rate of increase of game speed
				}
			}
		}
		xStone[i] += stoneTranslationSpeed;
		if(stoneAlive[i] )             //stone alive
			DrawStone(randomStoneIndices[i]);
	}
	stoneAngle+=stoneRotationSpeed;
	if(stoneAngle > 360) stoneAngle = 0;
}
void backButton() {
	if(mouseX <= -450 && mouseX >= -500 && mouseY >= -275 && mouseY <= -250){
			 glColor3f(0, 0, 0);
			if(mButtonPressed){
				mButtonPressed = false;
				instructionsGame = false;
				startScreenDisplay();
			}
	}
	else glColor3f(0, 0, 0);
I->	displayText(-1000 ,-550 ,0, "Back");
}
void InstructionsScreenDisplay()//A/W
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	 SetDisplayMode(MENU_SCREEN);
//	colorBackground();
	glColor3f(0, 0, 0);
I->	displayText(-900 ,400 ,0.4 ,"Key 'w' to move up.");
I->	displayText(-900 ,300 ,0.4 ,"Key 's' to move down.");
I->	displayText(-900 ,200 ,0.4 ,"Key 'd' to move right.");
I->	displayText(-900 ,100 ,0.4 ,"Key 'a' to move left.");

I->	displayText(-900 ,0.0 ,0.4 ,"Left mouse click to shoot laser");
	//displayRasterText(-900 ,-100 ,0.4 ,"The packet can be placed only when 's' is pressed before.");
I->	displayText(-900 ,-200 ,0.4 ,"You Get 1 point for shooting each object and 50 points for completing each lvl ");
I->	displayText(-900, -270,0.4,"The Objective is to score maximum points");
I->	displayText(-900 ,-400 ,0.4 ,"Key 'q' to quit.");//A
	backButton();
	if(previousScreen)
		nextScreen = false ,previousScreen = false; //as set by backButton()


}
void display() {//W/A


	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0,0,1200,700);

	if(startGame && !gameOver)
		GameScreenDisplay();

	else if(instructionsGame)
		InstructionsScreenDisplay();

	else if(gameOver)
		GameOverScreen();

	//Make spaceship bigger
	else if(startScreen){

			startScreenDisplay();
			if(gameQuit || startGame || optionsGame || instructionsGame){
				// startScreen = false;

				if(startGame){
					SetDisplayMode(GAME_SCREEN);
					startScreen = false;

				} else if(gameQuit)
					exit(0);

				} else if(instructionsGame) {
					SetDisplayMode(GAME_SCREEN);
					InstructionsScreenDisplay();
				}
		}

	//Reset Scaling values
	glScalef(1/2 ,1/2 ,0);
	glFlush();
	glLoadIdentity();
	glutSwapBuffers();
}
void somethingMovedRecalculateLaserAngle() {

	float mouseXForTan = (-50 - mouseX) + xOne;
	float mouseYForTan = (35 - mouseY) + yOne;
	float LaserAngleInRadian = atan(mouseYForTan/mouseXForTan);
	LaserAngle = (180/PI) * LaserAngleInRadian;
}
void keys(int key, int x, int y) // A
{

	if(key == GLUT_KEY_RIGHT)
    {
        S->playSound("sounds/Alien.wav");
        xOne+=SPACESHIP_SPEED;
    }
	if(key == GLUT_KEY_LEFT )
    {
        S->playSound("sounds/Alien.wav");
        xOne-=SPACESHIP_SPEED;
    }
	if(key == GLUT_KEY_UP)
	{
	    S->playSound("sounds/Alien.wav");
        yOne+=SPACESHIP_SPEED;
    }
	if(key == GLUT_KEY_DOWN )
	{
	    S->playSound("sounds/Alien.wav");
        yOne-=SPACESHIP_SPEED;
    }
	if(key ==  GLUT_KEY_DOWN || key == GLUT_KEY_UP || key == GLUT_KEY_RIGHT || key == GLUT_KEY_LEFT )
		somethingMovedRecalculateLaserAngle();

	display();

}
void keys(unsigned char key, int x, int y)//W
{
    if(key == 'q')
    {
        exit(0);
    }

	if(key == 'd')
    {
        S->playSound("sounds/Alien.wav");
        xOne+=SPACESHIP_SPEED;
    }
	if(key == 'a' )
    {
        S->playSound("sounds/Alien.wav");
        xOne-=SPACESHIP_SPEED;
    }
	if(key == 'w')
	{
	    S->playSound("sounds/Alien.wav");
        yOne+=SPACESHIP_SPEED;
    }
	if(key == 's' )
	{
	    S->playSound("sounds/Alien.wav");
        yOne-=SPACESHIP_SPEED;
    }
	if(key == 'd' || key == 'a' || key == 'w' || key == 's' )
		somethingMovedRecalculateLaserAngle();

	display();

}
void myinit()
{
	glClearColor(0.5,0.5,0.5,0);
	glColor3f(1.0,0.0,0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    gluOrtho2D(-1200,1200,-700,700);                   //<-----CHANGE THIS TO GET EXTRA SPACE
//  gluOrtho2D(-200,200,-200,200);
	glMatrixMode(GL_MODELVIEW);

	/*
    glPushMatrix();
    P->drawSquare(screenWidth,screenHeight);
    glPopMatrix();
*/
// P->parallaxInit("images/plx.png");




}
void passiveMotionFunc(int x,int y) {

	//when mouse not clicked
	mouseX = float(x)/(m_viewport[2]/1200.0)-600.0;  //converting screen resolution to ortho 2d spec
	mouseY = -(float(y)/(m_viewport[3]/700.0)-350.0);

	//Do calculations to find value of LaserAngle
	somethingMovedRecalculateLaserAngle();
	display();
}
 void mouseClick(int buttonPressed ,int state ,int x, int y) {

	if(buttonPressed == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		mButtonPressed = true;
		S->playSound("sounds/Laser.wav");//A
	}
	else
		mButtonPressed = false;
	display();
}

 void idleCallBack() {			//when no mouse or keybord pressed
	 display();
 }

 int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(1200,700);
	glutInitWindowPosition(90 ,0);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutCreateWindow("THE SPACESHIP SHOOTING GAME");
	glutDisplayFunc(display);
	glutSpecialFunc(keys);
	glutKeyboardFunc(keys);
	glutPassiveMotionFunc(passiveMotionFunc);
	glBlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA);
	glutIdleFunc(idleCallBack);
	glutMouseFunc(mouseClick);
	glGetIntegerv(GL_VIEWPORT ,m_viewport);
	myinit();
	SetDisplayMode(GAME_SCREEN);
	initializeStoneArray();
	glutMainLoop();
 }
