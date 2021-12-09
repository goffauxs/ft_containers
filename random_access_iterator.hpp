#include <cstddef>

template <typename T>
class random_access_iterator
{
public:
	typedef T value_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef T& reference;

private:
	pointer _ptr;

public:
	random_access_iterator() : _ptr(NULL) {};
	random_access_iterator(pointer ptr) : _ptr(ptr) {};
	random_access_iterator(const random_access_iterator& other) : _ptr(other._ptr) {};
	~random_access_iterator();
	random_access_iterator& opreator=(const random_access_iterator& rhs)
	{
		if (this != &rhs)
			this->_ptr = rhs._ptr;
		return *this;
	};

	// Comparisson operators
	bool operator==(const random_access_iterator& rhs) const { return (this->_ptr == rhs._ptr); }
	bool operator!=(const random_access_iterator& rhs) const { return (this->ptr != rhs._ptr); }
	bool operator>(const random_access_iterator& rhs) const { return (this->_ptr > rhs._ptr); }
	bool operator<(const random_access_iterator& rhs) const { return (this->_ptr < rhs._ptr); }
	bool operator>=(const random_access_iterator& rhs) const { return (this->_ptr >= rhs._ptr); }
	bool operator<=(const random_access_iterator& rhs) const { return (this->_ptr <= rhs._ptr); }

	// Dereference operators
	reference				operator*() const { return *(this->_ptr); }
	reference				operator[](difference_type rhs) const { return *(this->_ptr + rhs); }
	pointer 				operator->() const { return this->_ptr; }

	// Increment/Decremenet operators
	random_access_iterator& operator++() { this->_ptr++; return *this; }
	random_access_iterator& operator--() { this->_ptr--; return *this; }
	random_access_iterator	operator++(int)
	{
		random_access_iterator tmp(*this);
		this->ptr++;
		return tmp;
	}
	random_access_iterator	operator--(int)
	{
		random_access_iterator tmp(*this);
		this->_ptr--;
		return tmp;
	}

	// Arithmetic operators
	random_access_iterator& operator+(difference_type rhs) const
	{
		random_access_iterator tmp(*this);
		tmp._ptr += rhs;
		return tmp;
	}
	random_access_iterator&	operator-(difference_type rhs) const
	{
		random_access_iterator tmp(*this);
		tmp._ptr -= rhs;
		return tmp;
	}
	random_access_iterator& operator+=(difference_type rhs) { this->_ptr += rhs; return *this; }
	random_access_iterator& operator-=(difference_type rhs) { this->_ptr -= rhs; return *this; }
};