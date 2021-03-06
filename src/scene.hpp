#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include "algebra.hpp"
#include "primitive.hpp"
#include "material.hpp"

class SceneNode {
public:
  	typedef std::list<SceneNode*> ChildList;
	SceneNode(const std::string& name);
	virtual ~SceneNode();

	virtual void walk_gl(bool picking = false) const;

	const Matrix4x4& get_transform() const { return m_trans; }
	const Matrix4x4& get_inverse() const { return m_invtrans; }

	void set_transform(const Matrix4x4& m)
	{
		m_trans = m;
		m_invtrans = m.invert();
	}

	void set_transform(const Matrix4x4& m, const Matrix4x4& i)
	{
		m_trans = m;
		m_invtrans = i;
	}

	void add_child(SceneNode* child)
	{
		m_children.push_back(child);
		child->set_parent(this);
	}

	void remove_child(SceneNode* child)
	{
		m_children.remove(child);
	}
	
	ChildList get_children()
	{
		return m_children;
	}
	
	void set_parent (SceneNode *parent)
	{
		m_parent = parent;
	}
	
	SceneNode* get_parent()
	{
		return m_parent;
	}
	
	double getRotX()
	{
		return rotX;
	}
	
	int get_id()
	{
		return m_id;
	}
	
	void set_id(int newId)
	{
		m_id = newId;
	}
	
	std::string get_name()
	{
		return m_name;
	}
	// Callbacks to be implemented.
	// These will be called from Lua.
	void rotate(char axis, double angle);
	void scale(const Vector3D& amount);
	void translate(const Vector3D& amount);

	SceneNode* get_child(std::string childName);
	SceneNode* get_child(int id);
	SceneNode* get_joint();
  // Returns true if and only if this node is a JointNode
  virtual bool is_joint() const;
  
protected:
  
	// Useful for picking
	int m_id;
	std::string m_name;

	// Transformations
	Matrix4x4 m_trans;
	Matrix4x4 m_invtrans;
	double rotX, rotY, rotZ;
	
  	// Hierarchy
  	ChildList m_children;
	SceneNode* m_parent;
	
};

class JointNode : public SceneNode {
public:
  JointNode(const std::string& name);
  virtual ~JointNode();

  virtual void walk_gl(bool bicking = false) const;

  virtual bool is_joint() const;

  void set_joint_x(double min, double init, double max);
  void set_joint_y(double min, double init, double max);

  struct JointRange {
    double min, init, max;
  };
	
	double get_min_x()
	{
		return m_joint_x.min;
	}
  
	double get_max_x()
	{
		return m_joint_x.max;
	}
protected:

  JointRange m_joint_x, m_joint_y;
};

class GeometryNode : public SceneNode {
public:
  GeometryNode(const std::string& name,
               Primitive* primitive);
  virtual ~GeometryNode();

  virtual void walk_gl(bool picking = false) const;

  const Material* get_material() const;
  Material* get_material();

	void set_material(Material* material)
	{
		m_material = material;
	}

	void set_selected(bool newVal)
	{
		selected = newVal;
	}


protected:
	Material* m_material;
	bool selected;
	Primitive* m_primitive;
};

#endif
