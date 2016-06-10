#ifndef SHADER_HPP
# define SHADER_HPP

# include <GL/glew.h>
# include <string>
# include <vector>
# include <map>

class Vector2;
class Vector3;
class Color;
class Matrix;
class DirectionalLight;
class PointLight;
class Material;
class IView;

class Shader
{
public:
	enum Attribute : std::size_t
	{
		PositionAtt = 0,
		TexCoordAtt = 1,
		NormalAtt = 2,
		ColorAtt = 3
	};

	Shader(void) = delete;
	Shader(std::string const & fragShader, std::string const & vertShader);
	Shader(Shader const & shader);
	virtual ~Shader(void);

	Shader & operator=(Shader const & shader);

	void bind(void);
	void unbind(void);

	void setParameter(std::string const & name, std::size_t value);
	void setParameter(std::string const & name, int value);
	void setParameter(std::string const & name, float value);
	void setParameter(std::string const & name, Vector2 const & vector);
	void setParameter(std::string const & name, Vector3 const & vector);
	void setParameter(std::string const & name, Color const & color);
	void setParameter(std::string const & name, Matrix const & matrix);
	void setParameter(std::string const & name, Material const & material);
	void setParameter(std::string const & name, std::size_t index, DirectionalLight & light);
	void setParameter(std::string const & name, std::size_t index, PointLight & light);
	void setParameter(std::string const & name, std::vector<DirectionalLight> & lights);
	void setParameter(std::string const & name, std::vector<PointLight> & lights);

	void setActive(bool active);

	int getAttribute(Attribute attribute) const;
	bool isActive(void) const;

private:
	typedef std::map<std::string, int>	ParamMap;
	static const std::size_t AttributeCount = 4;

	GLint		m_program;
	GLint		m_shaders[2];
	GLint		m_attributes[AttributeCount];
	ParamMap	m_params;
	bool		m_isActive;

	int getParamIndex(std::string const & name);
	void init(std::string const & fragShader, std::string const & vertShader);
	std::string readShader(std::string const & fileName);
	GLuint loadShader(std::string const & filename, GLenum shaderType);

};

#endif
