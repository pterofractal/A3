#include "scene.hpp"
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>

using namespace std;
int counter = 0;
SceneNode::SceneNode(const std::string& name)
  : m_name(name),
	rotX(0),
	rotY(0),
	rotZ(0),
	m_id(counter++),
	m_parent(NULL)
{
}

SceneNode::~SceneNode()
{
}

void SceneNode::walk_gl(bool picking) const
{
	// Fill me in
	glPushMatrix();
		glMultMatrixd(m_trans.transpose().begin());
		ChildList temp = m_children;
		ChildList::iterator i;
		SceneNode *tempNode = m_parent;
		for ( i = temp.begin() ; i != temp.end(); i++ )
		{
			if(picking)
				glPushName(m_id);

			(*i)->walk_gl(picking);

			if (picking)
				glPopName();	
		}
	glPopMatrix();	
}

SceneNode* SceneNode::get_child(std::string childName)
{
	ChildList temp = m_children;
	ChildList::iterator i;
	SceneNode *child;
	if (m_name == childName)
	{
//		std::cerr << "Found " << m_name << "\n";
		return this;	
	}
	else
	{
		for ( i = temp.begin() ; i != temp.end(); i++ )
		{			
			child = (*i)->get_child(childName);
			if (child != NULL)
				return child;
		}
	}
	return NULL;
}

SceneNode* SceneNode::get_child(int id)
{
	ChildList temp = m_children;
	ChildList::iterator i;
	SceneNode *child;
	std::cerr<<"\t\t" << m_id << "\t\t";
	if (m_id == id)
	{
		return this;	
	}
	else
	{
		for ( i = temp.begin() ; i != temp.end(); i++ )
		{			
			child = (*i)->get_child(id);
			if (child != NULL)
				return child;
		}
	}
	return NULL;
}

SceneNode* SceneNode::get_joint()
{
	if (m_parent == NULL)
		return NULL;
		
	ChildList temp = m_parent->get_children();
	ChildList::iterator i;
	
	for (i = temp.begin(); i != temp.end(); i++)
	{
		if ( (*i)->is_joint() )
			return *i;
	}
	
	return m_parent->get_joint();
}
void SceneNode::rotate(char axis, double angle)
{
//	std::cerr << "Stub: Rotate " << m_name << " around " << axis << " by " << angle << std::endl;
	Matrix4x4 temp;
	double radAngle = angle * M_PI / 180.0;
	if (axis == 'x')
	{
		rotX += angle;
		temp[1][2] = -1 * sin(radAngle);
		temp[1][1] = cos(radAngle);
		temp[2][2] = cos(radAngle);
		temp[2][1] = sin(radAngle);
	}
	else if (axis == 'y')
	{
		rotY += angle;
		temp[2][0] = -1 * sin(radAngle);
		temp[0][0] = cos(radAngle);
		temp[2][2] = cos(radAngle);
		temp[0][2] = sin(radAngle);
	}
	else if (axis == 'z')
	{
		rotZ += angle;
		temp[0][1] = -1 * sin(radAngle);
		temp[0][0] = cos(radAngle);
		temp[1][1] = cos(radAngle);
		temp[1][0] = sin(radAngle);
	}
	m_trans = m_trans * temp;
}

void SceneNode::scale(const Vector3D& amount)
{
//	std::cerr << "Stub: Scale " << m_name << " by " << amount << std::endl;
	
	// Fill me in
	Matrix4x4 temp;
	temp[0][0] = amount[0];
	temp[1][1] = amount[1];
	temp[2][2] = amount[2];

	m_trans = m_trans * temp;
}

void SceneNode::translate(const Vector3D& amount)
{
//	std::cerr << "Stub: Translate " << m_name << " by " << amount << std::endl;
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
	glPushMatrix();
		glMultMatrixd(m_trans.transpose().begin());
		ChildList temp = m_children;
		ChildList::iterator i;
		SceneNode *tempNode = m_parent;
		for ( i = temp.begin() ; i != temp.end(); i++ )
		{
			if(picking)
				glPushName(m_id);

			(*i)->walk_gl(picking);

			if (picking)
				glPopName();	
		}
	glPopMatrix();
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
	m_primitive->set_displayListId(m_id);
//	cerr << "Created a new sphere " << name << "\t" << m_id << "b\n";
}

GeometryNode::~GeometryNode()
{
}

void GeometryNode::walk_gl(bool picking) const
{
	// Fill me in
	glPushMatrix();
		glMultMatrixd(m_trans.transpose().begin());	
		
		if (selected)
		{
			PhongMaterial selectedMaterial(Colour(1, 1, 1), Colour(0.1, 0.1, 0.1), 0.5);
			selectedMaterial.apply_gl();	
		}
		else
		{
			m_material->apply_gl();
		}
	
		if (picking)
			glPushName(m_id);
			
		m_primitive->walk_gl(picking);	

		if (picking)
			glPopName();
		
	glPopMatrix();
}
 

