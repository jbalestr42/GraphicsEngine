#ifndef IDRAWABLE_HPP
# define IDRAWABLE_HPP

class IView;

class IDrawable
{
public:
	virtual void draw(IView const & view) = 0;

};

#endif
