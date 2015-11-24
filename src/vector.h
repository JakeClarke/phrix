#ifndef _H_PHRIX_VECTOR
#define _H_PHRIX_VECTOR

#include "math.h"
#include <cmath>

namespace phrix {

	template<typename t>
	struct Vector2 {
		t X, Y;

		static Vector2 max(Vector2 a, Vector2 b) {
			return{
				phrix::max(a.X, b.X),
				phrix::max(a.Y, b.Y)
			};
		}

		static Vector2 min(Vector2 a, Vector2 b) {
			return{
				phrix::min(a.X, b.X),
				phrix::min(a.Y, b.Y)
			};
		}

		Vector2 operator+(Vector2 b) {
			return{ X + b.X, Y + b.Y };
		}

		Vector2 operator-(Vector2 b) {
			return{ X - b.X, Y - b.Y };
		}

		Vector2 operator*(t a) {
			return{ X * a, Y * a };
		}

		float length() {
			return sqrt((X * X) + (Y*Y));
		}
	};

	using Vector2F = Vector2<float>;

	template<typename t>
	struct Vector3
	{
		t X, Y, Z;

		Vector3 operator+(Vector3 b) {
			return{ X + b.X, Y + b.Y, Z + b.Z };
		}

		Vector3 operator+(Vector2<t> b) {
			return{ X + b.X, Y + b.Y };
		}

		static Vector3 up() {
			return{ 0.0f, 1.0f, 0.0f };
		}

		static Vector3 max(Vector3 a, Vector3 b) {
			return{
				phrix::max(a.X, b.X),
				phrix::max(a.Y, b.Y),
				phrix::max(a.Z, b.Z)
			};
		}

		static Vector3 min(Vector3 a, Vector3 b) {
			return{
				phrix::min(a.X, b.X),
				phrix::min(a.Y, b.Y),
				phrix::min(a.Z, a.Z)
			};
		}

		t length() {
			return sqrt((X*X) + (Y * Y) + (Z *Z));
		}
	};

	using Vector3F = Vector3<float>;

	template<typename t>
	struct Vector4 {
		t X, Y, Z, W;

		Vector4 operator+(Vector4 b) {
			return{ X + b.X, Y + b.Y,  Z + b.Z, W+ b.W};
		}

		Vector4 operator+(Vector2<t> b) {
			return{ X - b.X, Y - b.Y, Z, W};
		}

		Vector4 operator+(Vector3<t>) {
			return{ X - b.X, Y - b.Y, Z + b.Z, W };
		}

		Vector4 operator*(t a) {
			return{ X * a, Y * a, Z * a, W * a };
		}

		static Vector4 up() {
			return{ 0.0f, 1.0f, 0.0f. 0.0f };
		}

		static Vector4 max(Vector4 a, Vector4 b) {
			return{
				phrix::max(a.X, b.X),
				phrix::max(a.Y, b.Y),
				phrix::max(a.Z, b.Z),
				phrix::max(a.W, b.W)
			};
		}

		static Vector4 min(Vector4 a, Vector4 b) {
			return{
				phrix::min(a.X, b.X),
				phrix::min(a.Y, b.Y),
				phrix::min(a.Z, a.Z),
				phrix::min(a.W, b.W)
			};
		}

		t length() {
			return sqrt((X*X) + (Y * Y) + (Z *Z) + (W * W));
		}
	};

	using Vector4F = Vector4<float>;
}

#endif