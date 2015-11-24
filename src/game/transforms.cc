#include "transforms.h"

using namespace phrix;
using namespace phrix::game;

phrix::game::Transforms::operator Mat4F() const
{
	auto root = m_parent ? *m_parent : Mat4F::indentity();
	return Mat4F::translate(this->m_position) * root;
}
