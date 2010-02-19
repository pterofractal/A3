#include "primitive.hpp"
#include <GL/gl.h>
#include <GL/glu.h>


Primitive::~Primitive()
{
}

Sphere::~Sphere()
{
}

void Sphere::walk_gl(bool picking) const
{
  	// Fill me in
	GLUquadricObj *q = gluNewQuadric();	// Create A New Quadratic
	gluQuadricNormals(q, GL_SMOOTH);	// Generate Smooth Normals For The Quad
	gluQuadricTexture(q, GL_TRUE);		// Enable Texture Coords For The Quad
	gluSphere(q, 1.0f, 32, 32);
}
