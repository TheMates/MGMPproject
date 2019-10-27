#pragma once
#ifndef _DRAW_TEXT_H_
#define _DRAW_TEXT_H_
#include <string>
#include <GL/glut.h>

inline void drawText(const char* text, int xPosition, int yPosition)
{
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	glOrtho(0, w, 0, h, 1, -1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glDisable(GL_LIGHTING);

	glColor3f(0, 0, 0);
	glRasterPos2i(xPosition, h - yPosition);

	void* font = GLUT_BITMAP_HELVETICA_12;

	for (const char* c = text; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}

	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

inline void drawText(const std::string text, int xPosition, int yPosition)
{
	drawText(&(text.at(0)), xPosition, yPosition);
}


#endif
