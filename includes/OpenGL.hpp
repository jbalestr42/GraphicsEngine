#ifndef OPENGL_HPP
# define OPENGL_HPP

// Mac don't need glew
# if defined(MACOS)
#  include <OpenGL/gl3.h>
# else
#  include <GL/glew.h>
# endif

#endif
