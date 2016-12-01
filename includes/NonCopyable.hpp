#ifndef NONCOPYABLE_HPP
# define NONCOPYABLE_HPP

class NonCopyable
{
public:
	NonCopyable(void) = default;
	NonCopyable(NonCopyable const & nonCopyable) = delete;
	NonCopyable & operator=(NonCopyable const & nonCopyable) = delete;

};

#endif
