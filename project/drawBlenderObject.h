#pragma once
#ifndef DRAWBLENDEROBJECT_H
#define DRAWBLENDEROBJECT_H
#include <GL/glut.h>
#include "OBJ_Loader.h"

/// <summary> Draws object from blender loaded by objl::Loader class from OBJ_Loader.h. To see the actual material, be sure to enable light 
/// before drawing the object. </summary>
/// <param name="loader"> Pointer to loader object, that contains loaded models. </param>
/// <param name="meshIndex"> Index of model, that should be drawn. If you have included multiple models with your loader file select which one you want to draw. </param>
inline void drawBlenderObject(objl::Loader* loader, unsigned meshIndex)
{
	glPushMatrix();
	
	auto loadedMesh = &loader->LoadedMeshes[meshIndex];
	float materialAmbient[] = { loadedMesh->MeshMaterial.Ka.X,loadedMesh->MeshMaterial.Ka.Y,loadedMesh->MeshMaterial.Ka.Z,1 };
	float materialDiffuse[] = { loadedMesh->MeshMaterial.Kd.X,loadedMesh->MeshMaterial.Kd.Y,loadedMesh->MeshMaterial.Kd.Z ,1 };
	float materialSpecular[] = { loadedMesh->MeshMaterial.Ks.X,loadedMesh->MeshMaterial.Ks.Y,loadedMesh->MeshMaterial.Ks.Z ,1 };
	
	glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular);

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

#endif
