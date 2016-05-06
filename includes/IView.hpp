#ifndef IVIEW_HPP
# define IVIEW_HPP

class Vector3;
class Matrix;

class IView
{
public:
	virtual Matrix const & getProjectionMatrix(void) const = 0;
	virtual Matrix const & getViewMatrix(void) const = 0;
	virtual Vector3 const & getPosition(void) const = 0;

};

#endif
