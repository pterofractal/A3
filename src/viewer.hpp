#ifndef CS488_VIEWER_HPP
#define CS488_VIEWER_HPP

#include <gtkmm.h>
#include <gtkglmm.h>
#include <iostream>
#include "scene_lua.hpp"

// The "main" OpenGL widget
class Viewer : public Gtk::GL::DrawingArea {
public:
  Viewer(std::string filename);
  virtual ~Viewer();

  // A useful function that forces this widget to rerender. If you
  // want to render a new frame, do not call on_expose_event
  // directly. Instead call this, which will cause an on_expose_event
  // call when the time is right.
  void invalidate();
  
protected:

  // Events we implement
  // Note that we could use gtkmm's "signals and slots" mechanism
  // instead, but for many classes there's a convenient member
  // function one just needs to define that'll be called with the
  // event.

  // Called when GL is first initialized
  virtual void on_realize();
  // Called when our window needs to be redrawn
  virtual bool on_expose_event(GdkEventExpose* event);
  // Called when the window is resized
  virtual bool on_configure_event(GdkEventConfigure* event);
  // Called when a mouse button is pressed
  virtual bool on_button_press_event(GdkEventButton* event);
  // Called when a mouse button is released
  virtual bool on_button_release_event(GdkEventButton* event);
  // Called when the mouse moves
  virtual bool on_motion_notify_event(GdkEventMotion* event);

  // Draw a circle for the trackball, with OpenGL commands.
  // Assumes the context for the viewer is active.
  void draw_trackball_circle();
  
private:
SceneNode *root;
void drawSphere(double r, int lats, int longs) {
     int i, j;
      for(i = 0; i <= lats; i++) {
          double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
          double z0  = sin(lat0);
          double zr0 =  cos(lat0);
 
          double lat1 = M_PI * (-0.5 + (double) i / lats);
          double z1 = sin(lat1);
          double zr1 = cos(lat1);
 
          glBegin(GL_QUAD_STRIP);
          for(j = 0; j <= longs; j++) {
              double lng = 2 * M_PI * (double) (j - 1) / longs;
              double x = cos(lng);
              double y = sin(lng);
 
              glNormal3f(x * zr0, y * zr0, z0);
              glVertex3f(x * zr0, y * zr0, z0);
              glNormal3f(x * zr1, y * zr1, z1);
              glVertex3f(x * zr1, y * zr1, z1);
          }
          glEnd();
      }
  }

};

#endif
