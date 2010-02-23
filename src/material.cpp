#include "material.hpp"
#include <GL/gl.h>
#include <GL/glu.h>

Material::~Material()
{
}

PhongMaterial::PhongMaterial(const Colour& kd, const Colour& ks, double shininess)
  : m_kd(kd), m_ks(ks), m_shininess(shininess)
{
}

PhongMaterial::~PhongMaterial()
{
}

void PhongMaterial::apply_gl() const
{
  // Perform OpenGL calls necessary to set up this material.
	//std::cerr << m_kd << "\t " << m_ks;
		glColor3d(m_kd.R(), m_kd.G(), m_kd.B());
/*	
		float Am[4] = {m_kd.R(), m_kd.G(), m_kd.B(), 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Am );

		float Dm[4] = {m_kd.R(), m_kd.G(), m_kd.B(), 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Dm );

		float Sm[4] = {m_ks.R(), m_ks.G(), m_ks.B(), 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Sm );
*/		
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);
}
