#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"
#include <GL/gl.h>
#include <GL/glu.h>

class Primitive {
public:
  virtual ~Primitive();
  virtual void walk_gl(bool picking) const = 0;

	void set_displayListId(GLuint displayListId)
	{
		listId = displayListId;
	}
protected:
GLuint listId;

};

class Sphere : public Primitive {
public:
	virtual ~Sphere();
	virtual void walk_gl(bool picking) const;
};

#endif
