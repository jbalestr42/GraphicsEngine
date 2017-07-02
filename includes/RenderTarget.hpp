#ifndef RENDERTARGET_HPP
# define RENDERTARGET_HPP

# include <cstddef>

class Color;

class RenderTarget
{
public:
	RenderTarget(std::size_t width, std::size_t height);
	virtual ~RenderTarget(void) = default;

	void setClearColor(Color const & color);
	void setViewport(void);
	void clear(void) const;
	virtual void bind(void) = 0;

	std::size_t getWidth(void) const;
	std::size_t getHeight(void) const;

	void setWidth(std::size_t width);
	void setHeight(std::size_t height);

protected:
	void setClearMask(std::size_t clearMask);

private:
	std::size_t		m_width;
	std::size_t		m_height;
	std::size_t		m_clearMask;

};

#endif
