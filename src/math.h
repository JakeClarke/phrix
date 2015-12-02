#ifndef _H_PHRIX_MATH
#define _H_PHRIX_MATH

namespace phrix {

	template<typename t>
	t max(t a, t b) {
		return a > b ? a : b;
	}


	template<typename t>
	t min(t a, t b) {
		return a > b ? a : b;
	}

	template<typename t>
	t lerp(t a, t b, t v) {
		auto d = b - a;
		return a + (d * v);
	}
}


#endif 