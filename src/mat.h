#ifndef _H_PHRIX_MAT
#define _H_PHRIX_MAT

#include "vector.h"
#include <cmath> 

namespace phrix {

	template<typename t>
	struct Mat3 {
		Vector3<t> m[3];

		Mat3 operator*(Mat3<t> b) {
			return{
				{
					(m[0].X * b.m[0].X) + (m[0].Y * b.m[1].X) + (m[0].Z * b.m[2].X),
					(m[0].X * b.m[0].Y) + (m[0].Y * b.m[1].Y) + (m[0].Z * b.m[2].Y),
					(m[0].X * b.m[0].Z) + (m[0].Y * b.m[1].Z) + (m[0].Z * b.m[2].Z)
				},
				{
					(m[1].X * b.m[0].X) + (m[1].Y * b.m[1].X) + (m[1].Z * b.m[2].X),
					(m[1].X * b.m[0].Y) + (m[1].Y * b.m[1].Y) + (m[1].Z * b.m[2].Y),
					(m[1].X * b.m[0].Z) + (m[1].Y * b.m[1].Z) + (m[1].Z * b.m[2].Z)
				},
				{
					(m[2].X * b.m[0].X) + (m[2].Y * b.m[1].X) + (m[2].Z * b.m[2].X),
					(m[2].X * b.m[2].Y) + (m[2].Y * b.m[1].Y) + (m[2].Z * b.m[2].Y),
					(m[2].X * b.m[0].Z) + (m[2].Y * b.m[1].Z) + (m[2].Z * b.m[2].Z)
				}
			}
		}

		static Mat3 indentity() {
			return{
				{1, 0, 0},
				{0, 1, 0},
				{0, 0, 1}
			};
		}
	};



	template<typename t>
	struct Mat4 {
		Vector4<t> m[4];

		Mat4 operator*(Mat4 second) {
			return{ { {
					(m[0].X * second.m[0].X) + (m[0].Y * second.m[1].X) + (m[0].Z * second.m[2].X) + (m[0].W * second.m[3].X),
					(m[0].X * second.m[0].Y) + (m[0].Y * second.m[1].Y) + (m[0].Z * second.m[2].Y) + (m[0].W * second.m[3].Y),
					(m[0].X * second.m[0].Z) + (m[0].Y * second.m[1].Z) + (m[0].Z * second.m[2].Z) + (m[0].W * second.m[3].Z),
					(m[0].X * second.m[0].W) + (m[0].Y * second.m[1].W) + (m[0].Z * second.m[2].W) + (m[0].W * second.m[3].W)
				}, {
					(m[1].X * second.m[0].X) + (m[1].Y * second.m[1].X) + (m[1].Z * second.m[2].X) + (m[1].W * second.m[3].X),
					(m[1].X * second.m[0].Y) + (m[1].Y * second.m[1].Y) + (m[1].Z * second.m[2].Y) + (m[1].W * second.m[3].Y),
					(m[1].X * second.m[0].Z) + (m[1].Y * second.m[1].Z) + (m[1].Z * second.m[2].Z) + (m[1].W * second.m[3].Z),
					(m[1].X * second.m[0].W) + (m[1].Y * second.m[1].W) + (m[1].Z * second.m[2].W) + (m[1].W * second.m[3].W)
				}, {
					(m[2].X * second.m[0].X) + (m[2].Y * second.m[1].X) + (m[2].Z * second.m[2].X) + (m[2].W * second.m[3].X),
					(m[2].X * second.m[0].Y) + (m[2].Y * second.m[1].Y) + (m[2].Z * second.m[2].Y) + (m[2].W * second.m[3].Y),
					(m[2].X * second.m[0].Z) + (m[2].Y * second.m[1].Z) + (m[2].Z * second.m[2].Z) + (m[2].W * second.m[3].Z),
					(m[2].X * second.m[0].W) + (m[2].Y * second.m[1].W) + (m[2].Z * second.m[2].W) + (m[2].W * second.m[3].W)
				}, {
					(m[3].X * second.m[0].X) + (m[3].Y * second.m[1].X) + (m[3].Z * second.m[2].X) + (m[3].W * second.m[3].X),
					(m[3].X * second.m[0].Y) + (m[3].Y * second.m[1].Y) + (m[3].Z * second.m[2].Y) + (m[3].W * second.m[3].Y),
					(m[3].X * second.m[0].Z) + (m[3].Y * second.m[1].Z) + (m[3].Z * second.m[2].Z) + (m[3].W * second.m[3].Z),
					(m[3].X * second.m[0].W) + (m[3].Y * second.m[1].W) + (m[3].Z * second.m[2].W) + (m[3].W * second.m[3].W)
				} } };
		}

		static Mat4 indentity() {
			return{ {
				{1,0,0,0},
				{0,1,0,0},
				{0,0,1,0},
				{0,0,0,1}
			} };
		}

		static Mat4 ortho(float right, float left, float top, float bottom, float near, float far) {
			return{
				{2 / (right - left), 0, 0, -((right + left) / right - left)},
				{0, 2 / (top - bottom), 0, -((top + bottom) / (top - bottom))},
				{0, 0, -2 / (far - near), -((far + near) / (far - near))},
				{0, 0, 0, 1}
			};
		}

		static Mat4 perspective(float fov, float aspect, float near, float far) {
			float depth = far - near;
			float oneOverDepth = (1 / depth);
			float uh = 1 / tan(0.5f * fov);
			return{
				{1 * uh / aspect, 0, 0, 0},
				{0, uh , 0, 0},
				{0, 0, far * oneOverDepth, 1},
				{0, 0, (-far * near) * oneOverDepth, 0}
			};
		}

		static Mat4 translate(Vector3<t> pos) {
			return{
				{{1,0,0, pos.X},
				{0,1,0, pos.Y},
				{0,0,1, pos.Z},
				{0,0,0, 1}}
			};
		}
	};

	using Mat4F = Mat4<float>;
}


#endif