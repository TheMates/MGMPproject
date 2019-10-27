#pragma once
#include <Gl/glut.h>
#include "MGMPprojectNamespace.h"


using namespace MGMPproject;

void MGMPproject::drawText(const char* text, int xPosition, int yPosition, FontSize fontSize)
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

	void* font;
	switch (fontSize)
	{
	case FontSize::Small: font = GLUT_BITMAP_HELVETICA_10; break;
	case FontSize::Middle: font = GLUT_BITMAP_HELVETICA_12; break;
	case FontSize::Large: font = GLUT_BITMAP_HELVETICA_18; break;
	default: font = GLUT_BITMAP_HELVETICA_10; break;
	}

	for (const char* c = text; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}

	//glBegin(GL_TRIANGLES);
	//	glColor3f(1.0, 1.0, 0.0);
	//	glVertex2i(50, 390);
	//	glColor3f(1.0, 0.5, 0.0);
	//	glVertex2i(10, 300);
	//	glColor3f(1.0, 0.0, 0.0);
	//	glVertex2i(90, 300);
	//glEnd();

	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void MGMPproject::drawText(const std::string text, int xPosition, int yPosition,FontSize fontSize)
{
	drawText(&(text.at(0)), xPosition, yPosition, fontSize);
}

void MGMPproject::drawBlenderObject(objl::Loader* loader, unsigned meshIndex)
{
	glPushMatrix();

	auto loadedMesh = &loader->LoadedMeshes[meshIndex];
	float materialAmbient[] = { loadedMesh->MeshMaterial.Ka.X,loadedMesh->MeshMaterial.Ka.Y,loadedMesh->MeshMaterial.Ka.Z,1 };
	float materialDiffuse[] = { loadedMesh->MeshMaterial.Kd.X,loadedMesh->MeshMaterial.Kd.Y,loadedMesh->MeshMaterial.Kd.Z ,1 };
	float materialSpecular[] = { loadedMesh->MeshMaterial.Ks.X,loadedMesh->MeshMaterial.Ks.Y,loadedMesh->MeshMaterial.Ks.Z ,1 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);
	//glMaterialf(GL_FRONT, GL_SHININESS, materialShininess);

	for (auto face = 0; face < loadedMesh->Indices.size(); face += 3)
	{
		glBegin(GL_TRIANGLES);
		glNormal3f(loadedMesh->Vertices[loadedMesh->Indices[face]].Normal.X, loadedMesh->Vertices[loadedMesh->Indices[face]].Normal.Y,
			loadedMesh->Vertices[loadedMesh->Indices[face]].Normal.Z);
		glVertex3f(loadedMesh->Vertices[loadedMesh->Indices[face]].Position.X, loadedMesh->Vertices[loadedMesh->Indices[face]].Position.Y,
			loadedMesh->Vertices[loadedMesh->Indices[face]].Position.Z);
		glVertex3f(loadedMesh->Vertices[loadedMesh->Indices[face + 1]].Position.X, loadedMesh->Vertices[loadedMesh->Indices[face + 1]].Position.Y,
			loadedMesh->Vertices[loadedMesh->Indices[face + 1]].Position.Z);
		glVertex3f(loadedMesh->Vertices[loadedMesh->Indices[face + 2]].Position.X, loadedMesh->Vertices[loadedMesh->Indices[face + 2]].Position.Y,
			loadedMesh->Vertices[loadedMesh->Indices[face + 2]].Position.Z);
		glEnd();

	}
	glPopMatrix();
}


