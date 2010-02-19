#include "scene.hpp"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

using namespace std;
SceneNode::SceneNode(const std::string& name)
  : m_name(name)
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::walk_gl(bool picking) const
{
	// Fill me in
	ChildList temp = m_children;
	ChildList::iterator i;
	for ( i = temp.begin() ; i != temp.end(); i++ )
	{
		glPushMatrix();
		glMultMatrixd((*i)->get_transform().transpose().begin());
		(*i)->walk_gl(picking);
		glPopMatrix();
	}
	
	
}

void SceneNode::rotate(char axis, double angle)
{
	std::cerr << "Stub: Rotate " << m_name << " around " << axis << " by " << angle << std::endl;
	Matrix4x4 temp;
	angle = angle * M_PI / 180.0;
	if (axis == 'x')
	{
		temp[1][2] = -1 * sin(angle);
		temp[1][1] = cos(angle);
		temp[2][2] = cos(angle);
		temp[2][1] = sin(angle);
	}
	else if (axis == 'z')
	{
		temp[0][1] = -1 * sin(angle);
		temp[0][0] = cos(angle);
		temp[1][1] = cos(angle);
		temp[1][0] = sin(angle);
	}
	else if (axis == 'y')
	{
		temp[2][0] = -1 * sin(angle);
		temp[0][0] = cos(angle);
		temp[2][2] = cos(angle);
		temp[0][2] = sin(angle);
	} 
	m_trans = m_trans * temp;
}

void SceneNode::scale(const Vector3D& amount)
{
	std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;
	
	// Fill me in
	Matrix4x4 temp;
	temp[0][0] = amount[0];
	temp[1][1] = amount[1];
	temp[2][2] = amount[2];

	m_trans = m_trans * temp;
}

void SceneNode::translate(const Vector3D& amount)
{
	std::cerr << "Stub: Translate " << m_name << " by " << amount << std::endl;
	Matrix4x4 temp;
 	temp[0][3] += amount[0];
	temp[1][3] += amount[1];
	temp[2][3] += amount[2];

	m_trans = m_trans * temp;
}

bool SceneNode::is_joint() const
{
  return false;
}

JointNode::JointNode(const std::string& name)
  : SceneNode(name)
{
}

JointNode::~JointNode()
{
}

void JointNode::walk_gl(bool picking) const
{
  // Fill me in
}

bool JointNode::is_joint() const
{
  return true;
}

void JointNode::set_joint_x(double min, double init, double max)
{
  m_joint_x.min = min;
  m_joint_x.init = init;
  m_joint_x.max = max;
}

void JointNode::set_joint_y(double min, double init, double max)
{
  m_joint_y.min = min;
  m_joint_y.init = init;
  m_joint_y.max = max;
}

GeometryNode::GeometryNode(const std::string& name, Primitive* primitive)
  : SceneNode(name),
    m_primitive(primitive)
{
	cerr << "Created a new sphere " << name << "\n";
}

GeometryNode::~GeometryNode()
{
}

void GeometryNode::walk_gl(bool picking) const
{
	// Fill me in
	glPushMatrix();
		m_material->apply_gl();
		m_primitive->walk_gl(picking);		
	glPopMatrix();
}
 
