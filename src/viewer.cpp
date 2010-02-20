#include "viewer.hpp"
#include "algebra.hpp"
#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

Viewer::Viewer(std::string filename)
{
  Glib::RefPtr<Gdk::GL::Config> glconfig;

  // Ask for an OpenGL Setup with
  //  - red, green and blue component colour
  //  - a depth buffer to avoid things overlapping wrongly
  //  - double-buffered rendering to avoid tearing/flickering
  glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGB |
                                     Gdk::GL::MODE_DEPTH |
                                     Gdk::GL::MODE_DOUBLE);
  if (glconfig == 0) {
    // If we can't get this configuration, die
    std::cerr << "Unable to setup OpenGL Configuration!" << std::endl;
    abort();
  }

  // Accept the configuration
  set_gl_capability(glconfig);

  // Register the fact that we want to receive these events
  add_events(Gdk::BUTTON1_MOTION_MASK    |
             Gdk::BUTTON2_MOTION_MASK    |
             Gdk::BUTTON3_MOTION_MASK    |
             Gdk::BUTTON_PRESS_MASK      | 
             Gdk::BUTTON_RELEASE_MASK    |
             Gdk::VISIBILITY_NOTIFY_MASK);


	root = import_lua(filename);
	if (!root)
	{
		std::cerr << "Could not open " << filename << std::endl;
	}
	currMode = MODEL_TRANSLATE;
	mb1 = false;
	mb2 = false;
	mb3 = false;
translateX = 0;
translateY = 0;
translateZ = 0;
}

Viewer::~Viewer()
{
  // Nothing to do here right now.
}

void Viewer::invalidate()
{
  // Force a rerender
  Gtk::Allocation allocation = get_allocation();
  get_window()->invalidate_rect( allocation, false);
}

void Viewer::on_realize()
{
  // Do some OpenGL setup.
  // First, let the base class do whatever it needs to
  Gtk::GL::DrawingArea::on_realize();
  
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();
  
  if (!gldrawable)
    return;

  if (!gldrawable->gl_begin(get_gl_context()))
    return;

  glShadeModel(GL_SMOOTH);
  glClearColor( 0.4, 0.4, 0.4, 0.0 );
  glEnable(GL_DEPTH_TEST);

	sphere = glGenLists(1);
	if (sphere == 0)
	{
		std::cerr << "Something is bruck\n";
	}
	std::cerr << "\n\n\n\n" << sphere << "\n\n\n\n";
	glNewList(sphere, GL_COMPILE);
		q = gluNewQuadric();	// Create A New Quadratic
		gluQuadricNormals(q, GL_SMOOTH);	// Generate Smooth Normals For The Quad
		gluQuadricTexture(q, GL_TRUE);		// Enable Texture Coords For The Quad
		gluSphere(q, 1.0f, 32, 32);
	glEndList();
	
	root->set_id(sphere);

  gldrawable->gl_end();
}

bool Viewer::on_expose_event(GdkEventExpose* event)
{
	Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();
	
	if (!gldrawable) return false;
	
	if (!gldrawable->gl_begin(get_gl_context()))
		return false;
	
	// Set up for perspective drawing 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, get_width(), get_height());
	gluPerspective(40.0, (GLfloat)get_width()/(GLfloat)get_height(), 0.1, 1000.0);
	
	// change to model view for drawing
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	// Clear framebuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Set up lighting
	// set up lighting (if necessary)
	// Followed the tutorial found http://www.falloutsoftware.com/tutorials/gl/gl8.htm
	// to implement lighting
	
	// Initialize lighting settings
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_LIGHTING);
	
	// Create one light source
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	// Define properties of light 
	float ambientLight0[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float diffuseLight0[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float specularLight0[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	float position0[] = { 5.0f, 0.0f, 0.0f, 1.0f };	
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);
	glLightfv(GL_LIGHT0, GL_POSITION, position0);

	// Draw stuff
	glTranslated(translateX, translateY, translateZ);
	glPushMatrix();
		glMultMatrixd(root->get_transform().transpose().begin());
		root->walk_gl();
	glPopMatrix();
	draw_trackball_circle();

	

	
	// Swap the contents of the front and back buffers so we see what we
	// just drew. This should only be done if double buffering is enabled.
	gldrawable->swap_buffers();
	
	gldrawable->gl_end();
	
	return true;
}

bool Viewer::on_configure_event(GdkEventConfigure* event)
{
  Glib::RefPtr<Gdk::GL::Drawable> gldrawable = get_gl_drawable();

  if (!gldrawable) return false;

  if (!gldrawable->gl_begin(get_gl_context()))
    return false;

  // Set up perspective projection, using current size and aspect
  // ratio of display

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, event->width, event->height);
  gluPerspective(40.0, (GLfloat)event->width/(GLfloat)event->height, 0.1, 1000.0);

  // Reset to modelview matrix mode

  glMatrixMode(GL_MODELVIEW); 
  gldrawable->gl_end();

  return true;
}

bool Viewer::on_button_press_event(GdkEventButton* event)
{
	std::cerr << "Stub: Button " << event->button << " pressed" << std::endl;
	startPos[0] = event->x;
	startPos[1] = event->y;
	if (event->button == 1)
		mb1 = true;
	else if (event->button == 2)
		mb2 = true;
	else if (event->button == 3)
		mb3 = true;
	
	return true;
}

bool Viewer::on_button_release_event(GdkEventButton* event)
{
	std::cerr << "Stub: Button " << event->button << " released" << std::endl;
	
	if (event->button == 1)
		mb1 = false;
	else if (event->button == 2)
		mb2 = false;
	else if (event->button == 3)
		mb3 = false;
	
	return true;
}

bool Viewer::on_motion_notify_event(GdkEventMotion* event)
{
	std::cerr << "Stub: Motion at " << event->x << ", " << event->y << std::endl;
	Matrix4x4 temp;
	// Change in x
	double x2x1 = event->x - startPos[0];
	double y2y1 = event->y - startPos[1];
	// Scale it down a bit
	x2x1 /= 100.0;
	y2y1 /= 100.0;
	if (currMode == MODEL_TRANSLATE)
	{		
		int i = 0;
		if (mb1)
		{
			translateX += x2x1;
			translateY -= y2y1;
		}
		else if (mb2)
		{
			translateZ -= y2y1;
		}
		else if (mb3)	
		{
			SceneNode *child = root->get_child("lUpperArmShoulderJoint");
			if (child != NULL)
			{
				double minX = ((JointNode *) child)->get_min_x();
				double maxX = ((JointNode *) child)->get_max_x();
				double currRotX = child->get_parent()->getRotX();
				std::cerr << minX << "\t" << currRotX << "\t" << maxX << std::endl;
				if (currRotX + y2y1 > maxX || currRotX + y2y1 < minX)
					return true;
				
				y2y1 *= 10;
				child->get_parent()->rotate('x', y2y1);
				
				//child->rotate('y', y2y1);
			}
		}
		//root->set_transform(root->get_transform().transpose() * temp);
		
	}
	startPos[0] = event->x;
	startPos[1] = event->y;
	invalidate();
	return true;
}

void Viewer::draw_trackball_circle()
{
  int current_width = get_width();
  int current_height = get_height();
  
  // Set up for orthogonal drawing to draw a circle on screen.
  // You'll want to make the rest of the function conditional on
  // whether or not we want to draw the circle this time around.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glViewport(0, 0, current_width, current_height);
  glOrtho(0.0, (float)current_width, 
           0.0, (float)current_height, -0.1, 0.1);

  // change to model view for drawing
  glMatrixMode(GL_MODELVIEW);

  // Reset modelview matrix
  glLoadIdentity();

  // draw a circle for use with the trackball 
  glDisable(GL_LIGHTING);
  glEnable(GL_LINE_SMOOTH);
  glColor3f(1.0, 1.0, 1.0);
  double radius = current_width < current_height ? 
    (float)current_width * 0.25 : (float)current_height * 0.25;
  glTranslated((float)current_width * 0.5, (float)current_height * 0.5, 0);
  glBegin(GL_LINE_LOOP);
  for(size_t i=0; i<40; ++i) {
    double cosine = radius * cos(i*2*M_PI/40);
    double sine = radius * sin(i*2*M_PI/40);
    glVertex2f(cosine, sine);
  }
  glEnd();
  glColor3f(0.0, 0.0, 0.0);
  glDisable(GL_LINE_SMOOTH);
}
