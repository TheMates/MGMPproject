// Matouš Vrbík 182721
// Projekt: "Frogger game"

// popis v MGMPxvrbik00.docx

#include <GL/glut.h>
#include "Game.h"

// GLOBAL VARIABLES
Camera camera;
Player player(camera);
World world(player);
Game game;

// vlastnosti svetla
float lightPosition[] = { 0.0f, 20.0f, 0.0f, 1.0f };
float lightPositionZero[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightAmbient[] = { 0.6f, 0.6f, 0.6f, 1.0f };
float lightDiffuse[] = { 0.5f, 0.5f, 0.2f, 1.0f };
float lightSpecular[] = { 0.2f, 0.2f, 0.2f, 0.5f };


void onReshape(int w, int h)            
{
	camera.OnReshape();
}

void onInit(void)
{
	// steny
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT, GL_FILL);   // mod predni steny fill

	// nastaveni sten
	glShadeModel(GL_SMOOTH);            // smooth shading

	//light settings
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	glEnable(GL_FOG);

	glEnable(GL_BLEND);			//transparency on
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutSetCursor(GLUT_CURSOR_NONE);	//no cursor on screen

	const auto centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
	const auto centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;
	glutWarpPointer(centerX, centerY);


	//SET TEXUTRES
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );	//ještì je mozný GL_LINEAR když je mipmap nastavený na false
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);		//ale já ho budu mít nastavený na true
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	game.setWorld(world);
	game.setPlayer(player);
	world.LoadTextures();
	world.LoadModels();
	player.LoadModels();
	player.SetDebug(false);
	player.SetVerticalMovement(false);
}

void drawFog()
{
	GLfloat fogColor[] = { 0.5,0.5,0.5,1 };
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogi(GL_FOG_MODE, GL_EXP2);		//f= exp(-density*c^2)
	glFogf(GL_FOG_DENSITY, 0.01f);
}
void drawSun()
{
	glPushMatrix();
	glTranslatef(0, 40, -30);

	glLightfv(GL_LIGHT0, GL_POSITION, lightPositionZero);

	glDisable(GL_LIGHTING);
	GLUquadric * q;
	q = gluNewQuadric();
	glColor3f(1, 1, 0);
	gluSphere(q, 3, 20, 20);
	gluDeleteQuadric(q);

	glEnable(GL_LIGHTING);

	glPopMatrix();
}
void drawTest()
{
	// DRAW A TEST CUBE - RED ORIGIN
	int size = 2;
	glDisable(GL_LIGHTING);
	glPushMatrix();
	GLUquadric * q1;
	q1 = gluNewQuadric();
	glColor4f(1, 0, 0,0.5);
	glutSolidCube(size);
	// DRAW A TEST CUBE - GREEN X 20
	glTranslatef(20, 0, 0);
	q1 = gluNewQuadric();
	glColor4f(0, 1, 0, 0.5);
	glutSolidCube(size);
	// DRAW A TEST CUBE - BLUE Z 20
	glTranslatef(-20, 0, 20);
	q1 = gluNewQuadric();
	glColor4f(0, 0, 1, 0.5);
	glutSolidCube(size);
	// DRAW A TEST CUBE - CYAN Z -20
	glTranslatef(0, 0, -40);
	q1 = gluNewQuadric();
	glColor4f(0, 1, 1,0.5);
	glutSolidCube(size);
	
	// DRAW A TEST Cone - MAGENTA X -20
	glTranslatef(-20, 0, 20);
	q1 = gluNewQuadric();
	glColor4f(1, 0, 1, 0.5);
	glutSolidCone(3, 3, 10, 10);


	gluDeleteQuadric(q1);
	glPopMatrix();
	glEnable(GL_LIGHTING);

}
void drawTestTexture()
{
	glBindTexture(GL_TEXTURE_2D, *world.Textures(0));
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_BLEND);


	glBegin(GL_QUADS);	
	{
		glTexCoord2f(0, 0);
		glVertex3f(-10, 1, -10);


		glTexCoord2f(1, 0);
		glVertex3f(-10, 1, 10);

		glTexCoord2f(1, 1);;
		glVertex3f(10, 1, 10);

		glTexCoord2f(0, 1);
		glVertex3f(10, 1, -10);
	}

	glEnd();
}

void onDisplay(void)
{
	
	world.ClearBuffers();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// inicializace matice MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//DRAW FOG
	drawFog();

	// zapnuti osvetleni
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// TRANSFORMACE SVÌTA PODLE KAMERY
	player.PerformTransformation();

	// DRAW TEXTURES
	world.DrawTextures();

	//drawTest();

	world.DrawCars();
	world.DrawCrown();

	// DRAW SUN (WITHOUT LIGHT)
	drawSun();

	//LAST DRAW OVERLAYS
	player.DrawDebug();
	game.DrawStats();
	//player.drawPlayer();
	if (player.GetDebug())
	{
		MGMPproject::drawText("t,g - fov", glutGet(GLUT_WINDOW_WIDTH) - 120, 10);
		MGMPproject::drawText("r,f - camera sensitivity", glutGet(GLUT_WINDOW_WIDTH) - 120, 20);
		MGMPproject::drawText("z,h - movment speed", glutGet(GLUT_WINDOW_WIDTH) - 120, 30);
		MGMPproject::drawText("v - vertical movement", glutGet(GLUT_WINDOW_WIDTH) - 120, 40);
		MGMPproject::drawText("q - toggle fog", glutGet(GLUT_WINDOW_WIDTH) - 120, 50);
		MGMPproject::drawText("o - reset position", glutGet(GLUT_WINDOW_WIDTH) - 120, 60);
		MGMPproject::drawText("l - generate new level", glutGet(GLUT_WINDOW_WIDTH) - 120, 70);
	}
	glFlush();

	glutSwapBuffers();
}

void onMousePassiveMotion(int x, int y)
{
	y = glutGet(GLUT_WINDOW_HEIGHT) - y;

	const auto w = glutGet(GLUT_WINDOW_WIDTH) ;
	const auto h= glutGet(GLUT_WINDOW_HEIGHT) ;

	const auto centerX =  float(glutGet(GLUT_WINDOW_WIDTH)) / 2.0;
	const auto centerY = float(glutGet(GLUT_WINDOW_HEIGHT)) / 2.0;

	const int deltaX = x - centerX;
	const int deltaY = y - centerY;
	if (deltaX == 0 && deltaY == 0) return;

	player.Rotate(deltaX*camera.GetCamreaSensitivity(), deltaY*camera.GetCamreaSensitivity(), 0);

	glutWarpPointer(centerX, centerY);
}

void animationTimer(int value)	
{
	player.CheckKeyboardInput();
	player.Animate();
	world.Animate();
	world.CheckCollisions();
	glutPostRedisplay();
	glutTimerFunc(17, animationTimer, 0);	//17 ms - almost 60 FPS
}

void keyPressed(unsigned char key, int x, int y) 
{
	camera.KeyboardInput(key);
	player.KeyboardInput(key);
	world.KeyboardInput(key);
	switch (key)
	{
	case 'q': if (glIsEnabled(GL_FOG)) { glDisable(GL_FOG); }
			  else { glEnable(GL_FOG); }
			  break;
	default: break;
	}
}
void onSpecial(int key, int mx, int my)
{
	switch (key)
	{
	case GLUT_KEY_F3:
		player.SetDebug(!player.GetDebug());
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void createMenu(void(*func)(int value))
{
	game.createMenu(func);
}
void onMenu(int value)
{
	game.onMenu(value);
}



int main(int argc, char* argv[])
{
	glutInit(&argc, argv);										// inicializace knihovny GLUT
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	// init double buffer

	glutInitWindowSize(1280, 720);								// nastaveni pocatecni velikosti dale oteviranych oken
	glutInitWindowPosition(50, 50);								// nastaveni pocatecniho umisteni dale oteviranych oken

	glutCreateWindow("Frogger game");							// vytvoreni okna
	glutDisplayFunc(onDisplay);									// registrace funkce volane pri prekreslovani aktualniho okna
	glutReshapeFunc(onReshape);									// registrace funkce volane pri zmene velikosti aktualniho okna
	glutPassiveMotionFunc(onMousePassiveMotion);
	glutSpecialFunc(onSpecial);
	glutKeyboardFunc(keyPressed);
	createMenu(onMenu);
	onInit();
	glutTimerFunc(17, animationTimer, 0);
	glutMainLoop();												

	return 0;                           
}
