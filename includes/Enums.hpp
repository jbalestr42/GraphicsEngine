#ifndef ENUMS_HPP
# define ENUMS_HPP

# include <cstddef>

// TODO use an enum
// GL_TEXTURE0 index
static const int DiffuseIndex = 0;
static const int SpecularIndex = 1;
static const int ShadowMapIndex = 2;

enum class ShaderId
{
	Debug = -1,
	Phong = 0,
	Depth = 1,
	Screen = 2
};

#endif
