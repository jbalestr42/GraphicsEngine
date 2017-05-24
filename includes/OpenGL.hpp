#ifndef OPENGL_HPP
# define OPENGL_HPP

# if defined(MACOS)
#  define __gl_h_
#  define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#  include <OpenGL/gl3.h>
# else
#  include <GL/glew.h>
# endif

#endif
