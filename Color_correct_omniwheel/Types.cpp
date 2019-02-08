#include "Types.h"

#include "Functions.h"

boolean RGB::isColor(const RGB &color, int error) {
	if (inRange(color.red, red, error) && inRange(color.green, green, error)
			&& inRange(color.blue, blue, error)) {
		return true;
	}
	return false;
}

