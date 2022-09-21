#include <iostream>
#include <string>
#include <deque>
#include <typeinfo>
#include <fstream>
#define STR(x) #x
#define STRMACRO(y) STR(y)

#ifdef STD
# include <vector>
# include <map>
# include <stack>
# include <set>
# define TESTED_NAMESPACE std
#else
# include "vector.hpp"
# include "stack.hpp"
# include "map.hpp"
# include "set.hpp"
# define TESTED_NAMESPACE ft
#endif

class foo
{
private:
	int* _mfoo;
public:
	foo() : _mfoo(new int(42)) {}
	~foo() { delete _mfoo; }
};

#define MAX_RAM 4294967296
#define BUFFER_SIZE 4096
struct Buffer
{
	int idx;
	char buff[BUFFER_SIZE];
};


#define COUNT (MAX_RAM / (int)sizeof(Buffer))

template<typename T>
class MutantStack : public TESTED_NAMESPACE::stack<T>
{
public:
	MutantStack() {}
	MutantStack(const MutantStack<T>& src) { *this = src; }
	MutantStack<T>& operator=(const MutantStack<T>& rhs) 
	{
		this->c = rhs.c;
		return *this;
	}
	~MutantStack() {}

	typedef typename TESTED_NAMESPACE::stack<T>::container_type::iterator iterator;

	iterator begin() { return this->c.begin(); }
	iterator end() { return this->c.end(); }
};

template <typename T>
bool fncomp (T lhs, T rhs) {return lhs<rhs;}

struct classcomp {
  bool operator() (const char& lhs, const char& rhs) const
  {return lhs<rhs;}
};

template <typename T>
void explicit_test(TESTED_NAMESPACE::vector<T> v)
{
	std::cout << v.capacity() << std::endl;
}

void vector_tests()
{
	std::cout << "\n===== VECTOR =====" << std::endl;
	{
		std::cout << "----- Constructors -----" << std::endl;
		std::allocator<int> a;

		// explicit constructors should not be called implicitly
		// explicit_test<int>(a);
		// explicit_test<int>(10);

		TESTED_NAMESPACE::vector<int> first; // empty vector
		TESTED_NAMESPACE::vector<int> second(4, 100); // four ints with value 100
		TESTED_NAMESPACE::vector<int> third(second.begin(), second.end()); // same as second
		TESTED_NAMESPACE::vector<int> fourth(third); // copy of third

		int myints[] = {16, 2, 77, 29};
		TESTED_NAMESPACE::vector<int> fifth(myints, myints + sizeof(myints) / sizeof(int)); // iterator constructor used with array pointers

		std::cout << "The contents of fifth are:";
		for (TESTED_NAMESPACE::vector<int>::iterator it = fifth.begin(); it != fifth.end(); it++)
			std::cout << " " << *it;
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- Assignment operator -----" << std::endl;
		TESTED_NAMESPACE::vector<int> foo(3, 0);
		TESTED_NAMESPACE::vector<int> bar(5, 0);

		bar = foo;
		foo = TESTED_NAMESPACE::vector<int>();

		std::cout << "Size of foo: " << int(foo.size()) << std::endl;
		std::cout << "Size of bar: " << int(bar.size()) << std::endl;
	}
	{
		std::cout << "\n----- begin/end -----" << std::endl;
		TESTED_NAMESPACE::vector<int> myvector;
		for (int i=1; i <= 5; i++) myvector.push_back(i);

		std::cout << "myvector contains:";
		for (TESTED_NAMESPACE::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
			std::cout << " " << *it;
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- rbegin/rend -----" << std::endl;
		TESTED_NAMESPACE::vector<int> myvector(5);

		int i = 0;
		TESTED_NAMESPACE::vector<int>::reverse_iterator rit = myvector.rbegin();
		for (; rit != myvector.rend(); ++rit)
			*rit = ++i;
		
		std::cout << "myvector contains:";
		for (TESTED_NAMESPACE::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
			std::cout << " " << *it;
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- size -----" << std::endl;
		TESTED_NAMESPACE::vector<int> myints;
		std::cout << "0. size: " << myints.size() << std::endl;

		for (int i = 0; i < 10; i++) myints.push_back(i);
		std::cout << "1. size: " << myints.size() << std::endl;

		myints.insert(myints.end(), 10, 100);
		std::cout << "2. size: " << myints.size() << std::endl;

		myints.pop_back();
		std::cout << "3. size: " << myints.size() << std::endl;
	}
	{
		std::cout << "\n----- size/max_size/capacity -----" << std::endl;
		TESTED_NAMESPACE::vector<int> myvector;

		for (int i=0; i<100; i++) myvector.push_back(i);

		std::cout << "size: " << myvector.size() << std::endl;
		std::cout << "capacity: " << myvector.capacity() << std::endl;
		std::cout << "max_size: " << myvector.max_size() << std::endl;
	}
	{
		std::cout << "\n----- resize -----" << std::endl;
		TESTED_NAMESPACE::vector<int> myvector;

		for (int i=1;i<10;i++) myvector.push_back(i);

		myvector.resize(5);
		myvector.resize(8,100);
		myvector.resize(12);

		std::cout << "myvector contains:";
		for (size_t i=0;i<myvector.size();i++)
			std::cout << ' ' << myvector[i];
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- empty -----" << std::endl;
		TESTED_NAMESPACE::vector<int> myvector;
		int sum (0);

		for (int i=1;i<=10;i++) myvector.push_back(i);

		while (!myvector.empty())
		{
			sum += myvector.back();
			myvector.pop_back();
		}

		std::cout << "total: " << sum << std::endl;
	}
	{
		std::cout << "\n----- reserve -----" << std::endl;
		TESTED_NAMESPACE::vector<int>::size_type sz;

		TESTED_NAMESPACE::vector<int> foo;
		sz = foo.capacity();
		std::cout << "making foo grow:" << std::endl;
		for (int i=0; i<100; ++i) {
			foo.push_back(i);
			if (sz!=foo.capacity()) {
			sz = foo.capacity();
			std::cout << "capacity changed: " << sz << std::endl;
			}
		}

		TESTED_NAMESPACE::vector<int> bar;
		sz = bar.capacity();
		bar.reserve(100);   // this is the only difference with foo above
		std::cout << "making bar grow:" << std::endl;
		for (int i=0; i<100; ++i) {
			bar.push_back(i);
			if (sz!=bar.capacity()) {
			sz = bar.capacity();
			std::cout << "capacity changed: " << sz << std::endl;
			}
		}
	}
	{
		std::cout << "\n----- operator[] -----" << std::endl;
		TESTED_NAMESPACE::vector<int> myvector (10);   // 10 zero-initialized elements

		TESTED_NAMESPACE::vector<int>::size_type sz = myvector.size();

		// assign some values:
		for (unsigned i=0; i<sz; i++) myvector[i]=i;

		// reverse vector using operator[]:
		for (unsigned i=0; i<sz/2; i++)
		{
			int temp;
			temp = myvector[sz-1-i];
			myvector[sz-1-i]=myvector[i];
			myvector[i]=temp;
		}

		std::cout << "myvector contains:";
		for (unsigned i=0; i<sz; i++)
			std::cout << ' ' << myvector[i];
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- at -----" << std::endl;
		TESTED_NAMESPACE::vector<int> myvector(10);

		for (unsigned i = 0; i < myvector.size(); i++)
			myvector.at(i) = i;

		std::cout << "myvector contains:";
		for (unsigned i = 0; i < myvector.size(); i++)
			std::cout << " " << myvector.at(i);
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- front -----" << std::endl;
		TESTED_NAMESPACE::vector<int> myvector;

		myvector.push_back(78);
		myvector.push_back(16);

		// now front equals 78, and back 16

		myvector.front() -= myvector.back();

		std::cout << "myvector.front() is now " << myvector.front() << std::endl;
	}
	{
		std::cout << "\n----- back -----" << std::endl;
		TESTED_NAMESPACE::vector<int> myvector;

		myvector.push_back(10);

		while (myvector.back() != 0)
		{
			myvector.push_back ( myvector.back() -1 );
		}

		std::cout << "myvector contains:";
		for (unsigned i=0; i<myvector.size() ; i++)
			std::cout << ' ' << myvector[i];
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- assign -----" << std::endl;
		TESTED_NAMESPACE::vector<int> first;
		TESTED_NAMESPACE::vector<int> second;
		TESTED_NAMESPACE::vector<int> third;

		first.assign (7,100);             // 7 ints with a value of 100

		TESTED_NAMESPACE::vector<int>::iterator it;
		it=first.begin()+1;

		second.assign (it,first.end()-1); // the 5 central values of first

		int myints[] = {1776,7,4};
		third.assign (myints,myints+3);   // assigning from array.

		std::cout << "Size of first: " << int (first.size()) << std::endl;
		std::cout << "Size of second: " << int (second.size()) << std::endl;
		std::cout << "Size of third: " << int (third.size()) << std::endl;
	}
	{
		std::cout << "\n----- push_back -----" << std::endl;
		TESTED_NAMESPACE::vector<int> myvector;

		for (int i = 0; i < 8; i++)
			myvector.push_back (rand());

		std::cout << "myvector stores " << int(myvector.size()) << " numbers." << std::endl;
	}
	{
		std::cout << "\n----- pop_back -----" << std::endl;
		TESTED_NAMESPACE::vector<int> myvector;
		int sum (0);
		myvector.push_back (100);
		myvector.push_back (200);
		myvector.push_back (300);

		while (!myvector.empty())
		{
			sum+=myvector.back();
			myvector.pop_back();
		}

		std::cout << "The elements of myvector add up to " << sum << std::endl;
	}
	{
		std::cout << "\n----- insert -----" << std::endl;
		TESTED_NAMESPACE::vector<int> myvector (3,100);
		TESTED_NAMESPACE::vector<int>::iterator it;

		it = myvector.begin();
		it = myvector.insert ( it , 200 );

		myvector.insert (it,2,300);

		// "it" no longer valid, get a new one:
		it = myvector.begin();

		TESTED_NAMESPACE::vector<int> anothervector (2,400);
		myvector.insert (it+2,anothervector.begin(),anothervector.end());

		int myarray [] = { 501,502,503 };
		myvector.insert (myvector.begin(), myarray, myarray+3);

		std::cout << "myvector contains:";
		for (it=myvector.begin(); it<myvector.end(); it++)
			std::cout << ' ' << *it;
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- erase -----" << std::endl;
		TESTED_NAMESPACE::vector<int> myvector;

		// set some values (from 1 to 10)
		for (int i=1; i<=10; i++) myvector.push_back(i);

		// erase the 6th element
		myvector.erase (myvector.begin()+5);

		// erase the first 3 elements:
		myvector.erase (myvector.begin(),myvector.begin()+3);

		std::cout << "myvector contains:";
		for (unsigned i=0; i<myvector.size(); ++i)
			std::cout << ' ' << myvector[i];
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- swap -----" << std::endl;
		TESTED_NAMESPACE::vector<int> foo (3,100);   // three ints with a value of 100
		TESTED_NAMESPACE::vector<int> bar (5,200);   // five ints with a value of 200

		foo.swap(bar);

		std::cout << "foo contains:";
		for (unsigned i=0; i<foo.size(); i++)
			std::cout << ' ' << foo[i];
		std::cout << std::endl;

		std::cout << "bar contains:";
		for (unsigned i=0; i<bar.size(); i++)
			std::cout << ' ' << bar[i];
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- clear -----" << std::endl;
		TESTED_NAMESPACE::vector<int> myvector;
		myvector.push_back (100);
		myvector.push_back (200);
		myvector.push_back (300);

		std::cout << "myvector contains:";
		for (unsigned i=0; i<myvector.size(); i++)
			std::cout << ' ' << myvector[i];
		std::cout << std::endl;

		myvector.clear();
		myvector.push_back (1101);
		myvector.push_back (2202);

		std::cout << "myvector contains:";
		for (unsigned i=0; i<myvector.size(); i++)
			std::cout << ' ' << myvector[i];
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- get_allocator -----" << std::endl;
		TESTED_NAMESPACE::vector<int> myvector;
		int * p;
		unsigned int i;

		// allocate an array with space for 5 elements using vector's allocator:
		p = myvector.get_allocator().allocate(5);

		// construct values in-place on the array:
		for (i=0; i<5; i++) myvector.get_allocator().construct(&p[i],i);

		std::cout << "The allocated array contains:";
		for (i=0; i<5; i++) std::cout << ' ' << p[i];
		std::cout << std::endl;

		// destroy and deallocate:
		for (i=0; i<5; i++) myvector.get_allocator().destroy(&p[i]);
		myvector.get_allocator().deallocate(p,5);
	}
	{
		std::cout << "\n----- relational operators -----" << std::endl;
		TESTED_NAMESPACE::vector<int> foo (3,100);   // three ints with a value of 100
		TESTED_NAMESPACE::vector<int> bar (2,200);   // two ints with a value of 200

		if (foo==bar) std::cout << "foo and bar are equal\n";
		if (foo!=bar) std::cout << "foo and bar are not equal\n";
		if (foo< bar) std::cout << "foo is less than bar\n";
		if (foo> bar) std::cout << "foo is greater than bar\n";
		if (foo<=bar) std::cout << "foo is less than or equal to bar\n";
		if (foo>=bar) std::cout << "foo is greater than or equal to bar\n";
	}
	{
		std::cout << "\n----- swap (non-member) -----" << std::endl;
		TESTED_NAMESPACE::vector<int> foo (3,100);   // three ints with a value of 100
		TESTED_NAMESPACE::vector<int> bar (5,200);   // five ints with a value of 200

		foo.swap(bar);

		std::cout << "foo contains:";
		for (TESTED_NAMESPACE::vector<int>::iterator it = foo.begin(); it!=foo.end(); ++it)
			std::cout << ' ' << *it;
		std::cout << std::endl;

		std::cout << "bar contains:";
		for (TESTED_NAMESPACE::vector<int>::iterator it = bar.begin(); it!=bar.end(); ++it)
			std::cout << ' ' << *it;
		std::cout << std::endl;
	}
}

void stack_tests()
{
	std::cout << "\n===== STACK =====" << std::endl;
	{
		std::cout << "\n----- Constructors -----" << std::endl;
		std::deque<int> mydeque (3,100);          // deque with 3 elements
		TESTED_NAMESPACE::vector<int> myvector (2,200);        // vector with 2 elements

		TESTED_NAMESPACE::stack<int> first;                    				// empty stack
		TESTED_NAMESPACE::stack<int, std::deque<int> > second (mydeque);	// stack initialized to copy of deque

		TESTED_NAMESPACE::stack<int,TESTED_NAMESPACE::vector<int> > third;  // empty stack using vector
		TESTED_NAMESPACE::stack<int,TESTED_NAMESPACE::vector<int> > fourth (myvector);

		std::cout << "size of first: " << first.size() << std::endl;
		std::cout << "size of second: " << second.size() << std::endl;
		std::cout << "size of third: " << third.size() << std::endl;
		std::cout << "size of fourth: " << fourth.size() << std::endl;
	}
	{
		std::cout << "\n----- empty -----" << std::endl;
		TESTED_NAMESPACE::stack<int> mystack;
		int sum (0);

		for (int i=1;i<=10;i++) mystack.push(i);

		while (!mystack.empty())
		{
			sum += mystack.top();
			mystack.pop();
		}

		std::cout << "total: " << sum << std::endl;
	}
	{
		std::cout << "\n----- size -----" << std::endl;
		TESTED_NAMESPACE::stack<int> myints;
		std::cout << "0. size: " << myints.size() << std::endl;

		for (int i=0; i<5; i++) myints.push(i);
		std::cout << "1. size: " << myints.size() << std::endl;

		myints.pop();
		std::cout << "2. size: " << myints.size() << std::endl;
	}
	{
		std::cout << "\n----- top -----" << std::endl;
		TESTED_NAMESPACE::stack<int> mystack;

		mystack.push(10);
		mystack.push(20);

		mystack.top() -= 5;

		std::cout << "mystack.top() is now " << mystack.top() << std::endl;
	}
	{
		std::cout << "\n----- push/pop -----" << std::endl;
		TESTED_NAMESPACE::stack<int> mystack;

		std::cout << "Pushing elements...";
		for (int i=0; i<5; ++i)
		{
			mystack.push(i);
			std::cout << " " << mystack.top();
		}
		std::cout << std::endl;

		std::cout << "Popping out elements...";
		while (!mystack.empty())
		{
			std::cout << ' ' << mystack.top();
			mystack.pop();
		}
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- relational operators -----" << std::endl;
		TESTED_NAMESPACE::stack<int> foo;   // three ints with a value of 100
		TESTED_NAMESPACE::stack<int> bar;   // two ints with a value of 200
		for (int i = 0; i < 5; i++)
			foo.push(i);
		for (int i = 0; i < 2; i++)
			bar.push(i);

		if (foo==bar) std::cout << "foo and bar are equal\n";
		if (foo!=bar) std::cout << "foo and bar are not equal\n";
		if (foo< bar) std::cout << "foo is less than bar\n";
		if (foo> bar) std::cout << "foo is greater than bar\n";
		if (foo<=bar) std::cout << "foo is less than or equal to bar\n";
		if (foo>=bar) std::cout << "foo is greater than or equal to bar\n";
	}
}

void map_tests()
{
	std::cout << "\n===== MAP =====" << std::endl;
	{
		std::cout << "\n----- Constructors -----" << std::endl;
		TESTED_NAMESPACE::map<char, int> first; // default constructor
		first['a'] = 10;
		first['b'] = 30;
		first['c'] = 50;
		first['d'] = 70;

		TESTED_NAMESPACE::map<char, int> second(first.begin(), first.end()); // Range constructor
		TESTED_NAMESPACE::map<char, int> third(second); // copy constructor
		TESTED_NAMESPACE::map<char, int, classcomp> fourth; // default with class as comparisson object
		bool (*fn_pt)(char, char) = fncomp;
		TESTED_NAMESPACE::map<char, int, bool(*)(char, char)> fifth (fn_pt); // function pointer as comparisson object

		std::cout << "first contains:";
		for (TESTED_NAMESPACE::map<char, int>::iterator it = first.begin(); it != first.end(); it++)
			std::cout << " {" << it->first << " = " << it->second << "}";
		std::cout << std::endl;

		std::cout << "second contains:";
		for (TESTED_NAMESPACE::map<char, int>::iterator it = second.begin(); it != second.end(); it++)
			std::cout << " {" << it->first << " = " << it->second << "}";
		std::cout << std::endl;

		std::cout << "third contains:";
		for (TESTED_NAMESPACE::map<char, int>::iterator it = third.begin(); it != third.end(); it++)
			std::cout << " {" << it->first << " = " << it->second << "}";
		std::cout << std::endl;

		std::cout << "fourth contains:";
		for (TESTED_NAMESPACE::map<char, int>::iterator it = fourth.begin(); it != fourth.end(); it++)
			std::cout << " {" << it->first << " = " << it->second << "}";
		std::cout << std::endl;

		std::cout << "fifth contains:";
		for (TESTED_NAMESPACE::map<char, int>::iterator it = fifth.begin(); it != fifth.end(); it++)
			std::cout << " {" << it->first << " = " << it->second << "}";
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- Assignment operator -----" << std::endl;
		TESTED_NAMESPACE::map<char,int> first;
		TESTED_NAMESPACE::map<char,int> second;

		first['x']=8;
		first['y']=16;
		first['z']=32;

		second=first;                // second now contains 3 ints
		first=TESTED_NAMESPACE::map<char,int>();  // and first is now empty

		std::cout << "Size of first: " << first.size() << std::endl;
		std::cout << "Size of second: " << second.size() << std::endl;
	}
	{
		std::cout << "\n----- begin/end -----" << std::endl;
		TESTED_NAMESPACE::map<char,int> mymap;

		mymap['b'] = 100;
		mymap['a'] = 200;
		mymap['c'] = 300;

		// show content:
		for (TESTED_NAMESPACE::map<char,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
			std::cout << it->first << " => " << it->second << std::endl;
	}
	{
		std::cout << "\n----- rbegin/rend -----" << std::endl;
		TESTED_NAMESPACE::map<char,int> mymap;

		mymap['b'] = 100;
		mymap['a'] = 200;
		mymap['c'] = 300;

		// show content:
		for (TESTED_NAMESPACE::map<char,int>::reverse_iterator it=mymap.rbegin(); it!=mymap.rend(); ++it)
			std::cout << it->first << " => " << it->second << std::endl;
	}
	{
		std::cout << "\n----- empty -----" << std::endl;
		TESTED_NAMESPACE::map<char,int> mymap;

		mymap['a']=10;
		mymap['b']=20;
		mymap['c']=30;

		while (!mymap.empty())
		{
			std::cout << mymap.begin()->first << " => " << mymap.begin()->second << std::endl;
			mymap.erase(mymap.begin());
		}
	}
	{
		std::cout << "\n----- size -----" << std::endl;
		TESTED_NAMESPACE::map<char,int> mymap;
		mymap['a']=101;
		mymap['b']=202;
		mymap['c']=302;

		std::cout << "mymap.size() is " << mymap.size() << std::endl;
	}
	{
		std::cout << "\n----- max_size -----" << std::endl;
		int i;
		TESTED_NAMESPACE::map<int,int> mymap;

		if (mymap.max_size()>1000)
		{
			for (i=0; i<1000; i++) mymap[i]=0;
			std::cout << "The map contains 1000 elements." << std::endl;
		}
		else
			std::cout << "The map could not hold 1000 elements." << std::endl;
	}
	{
		std::cout << "\n----- operator[] -----" << std::endl;
		TESTED_NAMESPACE::map<char,std::string> mymap;

		mymap['a']="an element";
		mymap['b']="another element";
		mymap['c']=mymap['b'];

		std::cout << "mymap['a'] is " << mymap['a'] << std::endl;
		std::cout << "mymap['b'] is " << mymap['b'] << std::endl;
		std::cout << "mymap['c'] is " << mymap['c'] << std::endl;
		std::cout << "mymap['d'] is " << mymap['d'] << std::endl;

		std::cout << "mymap now contains " << mymap.size() << " elements." << std::endl;
	}
	{
		std::cout << "\n----- insert -----" << std::endl;
		TESTED_NAMESPACE::map<char,int> mymap;

		// first insert function version (single parameter):
		mymap.insert ( TESTED_NAMESPACE::pair<char,int>('a',100) );
		mymap.insert ( TESTED_NAMESPACE::pair<char,int>('z',200) );

		TESTED_NAMESPACE::pair<TESTED_NAMESPACE::map<char,int>::iterator,bool> ret;
		ret = mymap.insert ( TESTED_NAMESPACE::pair<char,int>('z',500) );
		if (ret.second==false) {
			std::cout << "element 'z' already existed";
			std::cout << " with a value of " << ret.first->second << std::endl;
		}

		// second insert function version (with hint position):
		TESTED_NAMESPACE::map<char,int>::iterator it = mymap.begin();
		mymap.insert (it, TESTED_NAMESPACE::pair<char,int>('b',300));  // max efficiency inserting
		mymap.insert (it, TESTED_NAMESPACE::pair<char,int>('c',400));  // no max efficiency inserting

		// third insert function version (range insertion):
		TESTED_NAMESPACE::map<char,int> anothermap;
		anothermap.insert(mymap.begin(),mymap.find('c'));

		// showing contents:
		std::cout << "mymap contains:" << std::endl;
		for (it=mymap.begin(); it!=mymap.end(); ++it)
			std::cout << it->first << " => " << it->second << std::endl;

		std::cout << "anothermap contains:" << std::endl;
		for (it=anothermap.begin(); it!=anothermap.end(); ++it)
			std::cout << it->first << " => " << it->second << std::endl;
	}
	{
		std::cout << "\n----- erase -----" << std::endl;
		TESTED_NAMESPACE::map<char,int> mymap;
		TESTED_NAMESPACE::map<char,int>::iterator it;

		// insert some values:
		mymap['a']=10;
		mymap['b']=20;
		mymap['c']=30;
		mymap['d']=40;
		mymap['e']=50;
		mymap['f']=60;

		it=mymap.find('b');
		mymap.erase (it);                   // erasing by iterator

		mymap.erase ('c');                  // erasing by key

		it=mymap.find ('e');
		mymap.erase ( it, mymap.end() );    // erasing by range

		// show content:
		for (it=mymap.begin(); it!=mymap.end(); ++it)
			std::cout << it->first << " => " << it->second << std::endl;
	}
	{
		std::cout << "\n----- swap -----" << std::endl;
		TESTED_NAMESPACE::map<char,int> foo,bar;

		foo['x']=100;
		foo['y']=200;

		bar['a']=11;
		bar['b']=22;
		bar['c']=33;

		foo.swap(bar);

		std::cout << "foo contains:" << std::endl;
		for (TESTED_NAMESPACE::map<char,int>::iterator it=foo.begin(); it!=foo.end(); ++it)
			std::cout << it->first << " => " << it->second << std::endl;

		std::cout << "bar contains:" << std::endl;
		for (TESTED_NAMESPACE::map<char,int>::iterator it=bar.begin(); it!=bar.end(); ++it)
			std::cout << it->first << " => " << it->second << std::endl;
	}
	{
		std::cout << "\n----- clear -----" << std::endl;
		TESTED_NAMESPACE::map<char,int> mymap;

		mymap['x']=100;
		mymap['y']=200;
		mymap['z']=300;

		std::cout << "mymap contains:" << std::endl;
		for (TESTED_NAMESPACE::map<char,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
			std::cout << it->first << " => " << it->second << std::endl;

		mymap.clear();
		mymap['a']=1101;
		mymap['b']=2202;

		std::cout << "mymap contains:" << std::endl;
		for (TESTED_NAMESPACE::map<char,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
			std::cout << it->first << " => " << it->second << std::endl;
	}
	{
		std::cout << "\n----- key_comp -----" << std::endl;
		TESTED_NAMESPACE::map<char,int> mymap;

		TESTED_NAMESPACE::map<char,int>::key_compare mycomp = mymap.key_comp();

		mymap['a']=100;
		mymap['b']=200;
		mymap['c']=300;

		std::cout << "mymap contains:" << std::endl;

		char highest = mymap.rbegin()->first;     // key value of last element

		TESTED_NAMESPACE::map<char,int>::iterator it = mymap.begin();
		do {
			std::cout << it->first << " => " << it->second << std::endl;
		} while ( mycomp((*it++).first, highest) );
	}
	{
		std::cout << "\n----- value_comp -----" << std::endl;
		TESTED_NAMESPACE::map<char,int> mymap;

		mymap['x']=1001;
		mymap['y']=2002;
		mymap['z']=3003;

		std::cout << "mymap contains:" << std::endl;

		TESTED_NAMESPACE::pair<char,int> highest = *mymap.rbegin();          // last element

		TESTED_NAMESPACE::map<char,int>::iterator it = mymap.begin();
		do {
			std::cout << it->first << " => " << it->second << std::endl;
		} while ( mymap.value_comp()(*it++, highest) );
	}
	{
		std::cout << "\n----- find -----" << std::endl;
		TESTED_NAMESPACE::map<char,int> mymap;
		TESTED_NAMESPACE::map<char,int>::iterator it;

		mymap['a']=50;
		mymap['b']=100;
		mymap['c']=150;
		mymap['d']=200;

		it = mymap.find('b');
		if (it != mymap.end())
			mymap.erase (it);

		// print content:
		std::cout << "elements in mymap:" << std::endl;
		std::cout << "a => " << mymap.find('a')->second << std::endl;
		std::cout << "c => " << mymap.find('c')->second << std::endl;
		std::cout << "d => " << mymap.find('d')->second << std::endl;
	}
	{
		std::cout << "\n----- count -----" << std::endl;
		TESTED_NAMESPACE::map<char,int> mymap;
		char c;

		mymap ['a']=101;
		mymap ['c']=202;
		mymap ['f']=303;

		for (c='a'; c<'h'; c++)
		{
			std::cout << c;
			if (mymap.count(c)>0)
				std::cout << " is an element of mymap." << std::endl;
			else 
				std::cout << " is not an element of mymap." << std::endl;
		}
	}
	{
		std::cout << "\n----- lower_bound/upper_bound -----" << std::endl;
		TESTED_NAMESPACE::map<char,int> mymap;
		TESTED_NAMESPACE::map<char,int>::iterator itlow,itup;

		mymap['a']=20;
		mymap['b']=40;
		mymap['c']=60;
		mymap['d']=80;
		mymap['e']=100;

		itlow=mymap.lower_bound ('b');  // itlow points to b
		itup=mymap.upper_bound ('d');   // itup points to e (not d!)

		mymap.erase(itlow,itup);        // erases [itlow,itup]

		// print content:
		for (TESTED_NAMESPACE::map<char,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it)
			std::cout << it->first << " => " << it->second << std::endl;
	}
	{
		std::cout << "\n----- equal_range -----" << std::endl;
		typedef TESTED_NAMESPACE::map<char,int>::iterator itr;
		TESTED_NAMESPACE::map<char,int> mymap;

		mymap['a']=10;
		mymap['b']=20;
		mymap['c']=30;

		TESTED_NAMESPACE::pair<itr,itr> ret;
		ret = mymap.equal_range('b');

		std::cout << "lower bound points to: ";
		std::cout << ret.first->first << " => " << ret.first->second << std::endl;

		std::cout << "upper bound points to: ";
		std::cout << ret.second->first << " => " << ret.second->second << std::endl;
	}
	{
		std::cout << "\n----- get_allocator -----" << std::endl;
		int psize;
		TESTED_NAMESPACE::map<char,int> mymap;
		TESTED_NAMESPACE::pair<const char,int>* p;

		// allocate an array of 5 elements using mymap's allocator:
		p=mymap.get_allocator().allocate(5);

		// assign some values to array
		psize = sizeof(TESTED_NAMESPACE::map<char,int>::value_type)*5;

		std::cout << "The allocated array has a size of " << psize << " bytes." << std::endl;

		mymap.get_allocator().deallocate(p,5);
	}
}

void set_tests()
{
	std::cout << "\n===== SET =====" << std::endl;
	{
		std::cout << "\n----- Constructors -----" << std::endl;
		TESTED_NAMESPACE::set<int> first;                           // empty set of ints

		int myints[]= {10,20,30,40,50};
		TESTED_NAMESPACE::set<int> second (myints,myints+5);        // range

		TESTED_NAMESPACE::set<int> third (second);                  // a copy of second

		TESTED_NAMESPACE::set<int> fourth (second.begin(), second.end());  // iterator ctor.

		TESTED_NAMESPACE::set<int,classcomp> fifth;                 // class as Compare

		bool(*fn_pt)(int,int) = fncomp;
		TESTED_NAMESPACE::set<int,bool(*)(int,int)> sixth (fn_pt);  // function pointer as Compare

		std::cout << "first contains:";
		for (TESTED_NAMESPACE::set<int>::iterator it = first.begin(); it != first.end(); it++)
			std::cout << " {" << *it << "}";
		std::cout << std::endl;

		std::cout << "second contains:";
		for (TESTED_NAMESPACE::set<int>::iterator it = second.begin(); it != second.end(); it++)
			std::cout << " {" << *it << "}";
		std::cout << std::endl;

		std::cout << "third contains:";
		for (TESTED_NAMESPACE::set<int>::iterator it = third.begin(); it != third.end(); it++)
			std::cout << " {" << *it << "}";
		std::cout << std::endl;

		std::cout << "fourth contains:";
		for (TESTED_NAMESPACE::set<int>::iterator it = fourth.begin(); it != fourth.end(); it++)
			std::cout << " {" << *it << "}";
		std::cout << std::endl;

		std::cout << "fifth contains:";
		for (TESTED_NAMESPACE::set<int>::iterator it = fifth.begin(); it != fifth.end(); it++)
			std::cout << " {" << *it << "}";
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- Assignment operator -----" << std::endl;
		int myints[]={ 12,82,37,64,15 };
		TESTED_NAMESPACE::set<int> first (myints,myints+5);   // set with 5 ints
		TESTED_NAMESPACE::set<int> second;                    // empty set

		second = first;                          // now second contains the 5 ints
		first = TESTED_NAMESPACE::set<int>();                 // and first is empty

		std::cout << "Size of first: " << int (first.size()) << std::endl;
		std::cout << "Size of second: " << int (second.size()) << std::endl;
	}
	{
		std::cout << "\n----- begin/end -----" << std::endl;
		int myints[] = {75,23,65,42,13};
		TESTED_NAMESPACE::set<int> myset (myints,myints+5);

		std::cout << "myset contains:";
		for (TESTED_NAMESPACE::set<int>::iterator it=myset.begin(); it!=myset.end(); ++it)
			std::cout << ' ' << *it;

		std::cout << std::endl;
	}
	{
		std::cout << "\n----- rbegin/rend -----" << std::endl;
		int myints[] = {75,23,65,42,13};
		TESTED_NAMESPACE::set<int> myset (myints,myints+5);

		std::cout << "myset contains:";
		for (TESTED_NAMESPACE::set<int>::reverse_iterator it=myset.rbegin(); it!=myset.rend(); ++it)
			std::cout << ' ' << *it;

		std::cout << std::endl;
	}
	{
		std::cout << "\n----- empty -----" << std::endl;
		TESTED_NAMESPACE::set<int> myset;

		myset.insert(20);
		myset.insert(30);
		myset.insert(10);

		std::cout << "myset contains:";
		while (!myset.empty())
		{
			std::cout << ' ' << *myset.begin();
			myset.erase(myset.begin());
		}
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- size -----" << std::endl;
		TESTED_NAMESPACE::set<int> myints;
		std::cout << "0. size: " << myints.size() << std::endl;

		for (int i=0; i<10; ++i) myints.insert(i);
		std::cout << "1. size: " << myints.size() << std::endl;

		myints.insert (100);
		std::cout << "2. size: " << myints.size() << std::endl;

		myints.erase(5);
		std::cout << "3. size: " << myints.size() << std::endl;
	}
	{
		std::cout << "\n----- max_size -----" << std::endl;
		int i;
		TESTED_NAMESPACE::set<int> myset;

		if (myset.max_size()>1000)
		{
			for (i=0; i<1000; i++) myset.insert(i);
				std::cout << "The set contains 1000 elements." << std::endl;
		}
		else
			std::cout << "The set could not hold 1000 elements." << std::endl;
	}
	{
		std::cout << "\n----- insert -----" << std::endl;
		TESTED_NAMESPACE::set<int> myset;
		TESTED_NAMESPACE::set<int>::iterator it;
		TESTED_NAMESPACE::pair<TESTED_NAMESPACE::set<int>::iterator,bool> ret;

		// set some initial values:
		for (int i=1; i<=5; ++i) myset.insert(i*10);    // set: 10 20 30 40 50

		ret = myset.insert(20);               // no new element inserted

		if (ret.second==false) it=ret.first;  // "it" now points to element 20

		myset.insert (it,25);                 // max efficiency inserting
		myset.insert (it,24);                 // max efficiency inserting
		myset.insert (it,26);                 // no max efficiency inserting

		int myints[]= {5,10,15};              // 10 already in set, not inserted
		myset.insert (myints,myints+3);

		std::cout << "myset contains:";
		for (it=myset.begin(); it!=myset.end(); ++it)
			std::cout << ' ' << *it;
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- erase -----" << std::endl;
		TESTED_NAMESPACE::set<int> myset;
		TESTED_NAMESPACE::set<int>::iterator it;

		// insert some values:
		for (int i=1; i<10; i++) myset.insert(i*10);  // 10 20 30 40 50 60 70 80 90

		it = myset.begin();
		++it;                                         // "it" points now to 20

		myset.erase (it);

		myset.erase (40);

		it = myset.find (60);
		myset.erase (it, myset.end());

		std::cout << "myset contains:";
		for (it=myset.begin(); it!=myset.end(); ++it)
			std::cout << ' ' << *it;
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- swap -----" << std::endl;
		int myints[]={12,75,10,32,20,25};
		TESTED_NAMESPACE::set<int> first (myints,myints+3);     // 10,12,75
		TESTED_NAMESPACE::set<int> second (myints+3,myints+6);  // 20,25,32

		first.swap(second);

		std::cout << "first contains:";
		for (TESTED_NAMESPACE::set<int>::iterator it=first.begin(); it!=first.end(); ++it)
			std::cout << ' ' << *it;
		std::cout << std::endl;

		std::cout << "second contains:";
		for (TESTED_NAMESPACE::set<int>::iterator it=second.begin(); it!=second.end(); ++it)
			std::cout << ' ' << *it;
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- clear -----" << std::endl;
		TESTED_NAMESPACE::set<int> myset;

		myset.insert (100);
		myset.insert (200);
		myset.insert (300);

		std::cout << "myset contains:";
		for (TESTED_NAMESPACE::set<int>::iterator it=myset.begin(); it!=myset.end(); ++it)
			std::cout << ' ' << *it;
		std::cout << std::endl;

		myset.clear();
		myset.insert (1101);
		myset.insert (2202);

		std::cout << "myset contains:";
		for (TESTED_NAMESPACE::set<int>::iterator it=myset.begin(); it!=myset.end(); ++it)
			std::cout << ' ' << *it;
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- key_comp -----" << std::endl;
		TESTED_NAMESPACE::set<int> myset;
		int highest;

		TESTED_NAMESPACE::set<int>::key_compare mycomp = myset.key_comp();

		for (int i=0; i<=5; i++) myset.insert(i);

		std::cout << "myset contains:";

		highest=*myset.rbegin();
		TESTED_NAMESPACE::set<int>::iterator it=myset.begin();
		do {
			std::cout << ' ' << *it;
		} while ( mycomp(*(++it),highest) );

		std::cout << std::endl;
	}
	{
		std::cout << "\n----- value_comp -----" << std::endl;
		TESTED_NAMESPACE::set<int> myset;

		TESTED_NAMESPACE::set<int>::value_compare mycomp = myset.value_comp();

		for (int i=0; i<=5; i++) myset.insert(i);

		std::cout << "myset contains:";

		int highest=*myset.rbegin();
		TESTED_NAMESPACE::set<int>::iterator it=myset.begin();
		do {
			std::cout << ' ' << *it;
		} while ( mycomp(*(++it),highest) );

		std::cout << std::endl;
	}
	{
		std::cout << "\n----- find -----" << std::endl;
		TESTED_NAMESPACE::set<int> myset;
		TESTED_NAMESPACE::set<int>::iterator it;

		// set some initial values:
		for (int i=1; i<=5; i++) myset.insert(i*10);    // set: 10 20 30 40 50

		it=myset.find(20);
		myset.erase (it);
		myset.erase (myset.find(40));

		std::cout << "myset contains:";
		for (it=myset.begin(); it!=myset.end(); ++it)
			std::cout << ' ' << *it;
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- count -----" << std::endl;
		TESTED_NAMESPACE::set<int> myset;

		// set some initial values:
		for (int i=1; i<5; ++i) myset.insert(i*3);    // set: 3 6 9 12

		for (int i=0; i<10; ++i)
		{
			std::cout << i;
			if (myset.count(i)!=0)
			std::cout << " is an element of myset." << std::endl;
			else
			std::cout << " is not an element of myset." << std::endl;
		}
	}
	{
		std::cout << "\n----- lower_bound/upper_bound -----" << std::endl;
		TESTED_NAMESPACE::set<int> myset;
		TESTED_NAMESPACE::set<int>::iterator itlow,itup;

		for (int i=1; i<10; i++) myset.insert(i*10); // 10 20 30 40 50 60 70 80 90

		itlow=myset.lower_bound (30);                //       ^
		itup=myset.upper_bound (60);                 //                   ^

		myset.erase(itlow,itup);                     // 10 20 70 80 90

		std::cout << "myset contains:";
		for (TESTED_NAMESPACE::set<int>::iterator it=myset.begin(); it!=myset.end(); ++it)
			std::cout << ' ' << *it;
		std::cout << std::endl;
	}
	{
		std::cout << "\n----- equal_range -----" << std::endl;
		typedef TESTED_NAMESPACE::set<int>::const_iterator itr;
		TESTED_NAMESPACE::set<int> myset;

		for (int i=1; i<=5; i++) myset.insert(i*10);   // myset: 10 20 30 40 50

		TESTED_NAMESPACE::pair<itr, itr> ret;
		ret = myset.equal_range(30);

		std::cout << "the lower bound points to: " << *ret.first << std::endl;
		std::cout << "the upper bound points to: " << *ret.second << std::endl;
	}
	{
		std::cout << "\n----- get_allocator -----" << std::endl;
		TESTED_NAMESPACE::set<int> myset;
		int * p;
		unsigned int i;

		// allocate an array of 5 elements using myset's allocator:
		p=myset.get_allocator().allocate(5);

		// assign some values to array
		for (i=0; i<5; i++) p[i]=(i+1)*10;

		std::cout << "The allocated array contains:";
		for (i=0; i<5; i++) std::cout << ' ' << p[i];
		std::cout << std::endl;

		myset.get_allocator().deallocate(p,5);
	}
}

int main(int argc, char** argv)
{
// #ifdef STD
// 	std::cout << "\t---STD---\n" << std::endl;
// #else
// 	std::cout << "\t---FT---\n" << std::endl;
// #endif

	std::string nspace(STRMACRO(TESTED_NAMESPACE));
	nspace.append("_out");
	std::ofstream std_out(nspace);
	std::streambuf *backup = std::cout.rdbuf();
	std::cout.rdbuf(std_out.rdbuf());

	vector_tests();
	stack_tests();
	map_tests();
	set_tests();

	typedef TESTED_NAMESPACE::iterator_traits<TESTED_NAMESPACE::vector<int>::iterator > traits;
	if (typeid(traits::iterator_category) == typeid(TESTED_NAMESPACE::random_access_iterator_tag))
		std::cout << "vector<int>::iterator is a random access iterator" << std::endl;

	if (argc != 2)
	{
		std::cerr << "Usage: ./test seed" << std::endl;
		std::cerr << "Provide a seed please" << std::endl;
		std::cerr << "Count value:" << COUNT << std::endl;
		return 1;
	}
	const int seed = atoi(argv[1]);
	srand(seed);

	TESTED_NAMESPACE::vector<std::string> vector_str;
	TESTED_NAMESPACE::vector<int> vector_int;
	TESTED_NAMESPACE::stack<int> stack_int;
	TESTED_NAMESPACE::vector<Buffer> vector_buffer;
	TESTED_NAMESPACE::stack<Buffer, std::deque<Buffer> > stack_deq_buffer;
	TESTED_NAMESPACE::map<int, int> map_int;

	for (int i = 0; i < COUNT; i++)
	{
		vector_buffer.push_back(Buffer());
	}

	for (int i = 0; i < COUNT; i++)
	{
		const int idx = rand() % COUNT;
		vector_buffer[idx].idx = 5;
	}
	TESTED_NAMESPACE::vector<Buffer>().swap(vector_buffer);

	try
	{
		for (int i = 0; i < COUNT; i++)
		{
			const int idx = rand() % COUNT;
			vector_buffer.at(idx);
			std::cerr << "Error: THIS VECTOR SHOULD BE EMPTY!!" <<std::endl;
		}
	}
	catch(const std::exception& e)
	{
		//NORMAL ! :P
	}
	
	for (int i = 0; i < COUNT; ++i)
	{
		map_int.insert(TESTED_NAMESPACE::make_pair(rand() % COUNT, rand() % COUNT));
	}

	int sum = 0;
	for (int i = 0; i < 10000; i++)
	{
		int access = rand() % COUNT;
		sum += map_int[access];
	}
	std::cout << "should be constant with the same seed: " << sum << std::endl;

	{
		TESTED_NAMESPACE::map<int, int> copy = map_int;
	}
	MutantStack<char> iterable_stack;
	for (char letter = 'a'; letter <= 'z'; letter++)
		iterable_stack.push(letter);
	for (MutantStack<char>::iterator it = iterable_stack.begin(); it != iterable_stack.end(); it++)
	{
		std::cout << *it;
	}
	std::cout << std::endl;

	std_out.close();

	std::cout.rdbuf(backup);

	std::cout << "Output of tests written to " << STRMACRO(TESTED_NAMESPACE) << "_out." << std::endl;

	return (0);
}