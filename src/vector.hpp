#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <memory>
#include <exception>
#include <iterator>
namespace sjtu 
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector 
{
public:
	//std::allocator<T> vec_allocate;
	size_t now_len, extended_len;
	T *vec_begin, *vec_end;
	/**
	 * TODO
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator 
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the 
	// iterator points to. 
	// STL algorithms and containers may use these type_traits (e.g. the following 
	// typedef) to work properly. In particular, without the following code, 
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	friend class vector<T>;
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

	private:
		vector <T> *vec;
		pointer vec_ptr;
		iterator (vector <T> *vec, pointer vec_ptr): vec(vec), vec_ptr(vec_ptr){}
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator operator+(const int &n) const 
		{
			iterator temp = *this;
			temp.vec_ptr += n;
			return temp;
		}
		iterator operator-(const int &n) const 
		{
			iterator temp = *this;
			temp.vec_ptr -= n;
			return temp;
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const 
		{
		  if (vec != rhs.vec) throw invalid_iterator();
			return vec_ptr - rhs.vec_ptr;
		}
		iterator& operator+=(const int &n) 
		{
			vec_ptr += n;
			return *this;
		}
		iterator& operator-=(const int &n) 
		{
			vec_ptr -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int) 
		{
			iterator temp = *this;
			vec_ptr++;
			return temp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++() 
		{
			vec_ptr++;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) 
		{
			iterator temp = *this;
			vec_ptr--;
			return temp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--() 
		{
			vec_ptr--;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const
		{
			return *vec_ptr;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const 
		{
			return vec_ptr == rhs.vec_ptr;
		}
		bool operator==(const const_iterator &rhs) const 
		{
			return vec_ptr == rhs.vec_ptr;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const 
		{
			return vec_ptr != rhs.vec_ptr;
		}
		bool operator!=(const const_iterator &rhs) const 
		{
			return vec_ptr != rhs.vec_ptr;
		}
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator 
	{

		friend class vector<T>;
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;
	
	private:
		/*TODO*/
		const vector <T> *vec;
		pointer vec_ptr;
		const_iterator (const vector <T> *vec,pointer vec_ptr): vec(vec), vec_ptr(vec_ptr){}

	public:
	  const_iterator operator+(const int &n) const 
		{
			const_iterator temp = *this;
			temp.vec_ptr += n;
			return temp;
		}
		const_iterator operator-(const int &n) const 
		{
			const_iterator temp = *this;
			temp.vec_ptr -= n;
			return temp;
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const const_iterator &rhs) const 
		{
		  if (vec != rhs.vec) throw invalid_iterator();
			return vec_ptr - rhs.vec_ptr;
		}
		const_iterator& operator+=(const int &n) 
		{
			vec_ptr += n;
			return *this;
		}
		const_iterator& operator-=(const int &n) 
		{
			vec_ptr -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int) 
		{
			const_iterator temp = *this;
			vec_ptr++;
			return temp;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++() 
		{
			vec_ptr++;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int) 
		{
			const_iterator temp = *this;
			vec_ptr--;
			return temp;
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--() 
		{
			vec_ptr--;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const
		{
			return *vec_ptr;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const 
		{
			return vec_ptr == rhs.vec_ptr;
		}
		bool operator==(const const_iterator &rhs) const 
		{
			return vec_ptr == rhs.vec_ptr;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const 
		{
			return vec_ptr != rhs.vec_ptr;
		}
		bool operator!=(const const_iterator &rhs) const 
		{
			return vec_ptr != rhs.vec_ptr;
		}
	};
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	
	
	vector() 
	{
		vec_begin = std::allocator<T>().allocate(1);
		vec_end = vec_begin;
		extended_len = 1;
		now_len = 0;
	}
	vector(const vector &other) 
	{
		vec_begin = std::allocator<T>().allocate(other.extended_len);
		vec_end = vec_begin + other.now_len;
		extended_len = other.extended_len;
		now_len = other.now_len;
		for (size_t i = 0; i < now_len; i++)
		{
			std::construct_at(vec_begin + i, other.vec_begin[i]);
		}
	}
	/**
	 * TODO Destructor
	 */
	~vector() 
	{
		if (vec_begin != nullptr)
		{
			for (size_t i = 0; i < now_len; i++)
			{
				std::destroy_at(vec_begin + i);
			}
			std::allocator<T>().deallocate(vec_begin, extended_len);
		}
	}
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other) 
	{
		if (this == &other) return *this;
		if (vec_begin != nullptr)
		{
			for (size_t i = 0; i < now_len; i++)
			{
				std::destroy_at(vec_begin + i);
			}
			std::allocator<T>().deallocate(vec_begin, extended_len);
		}
		vec_begin = std::allocator<T>().allocate(other.extended_len);
		vec_end = vec_begin + other.now_len;
		extended_len = other.extended_len;
		now_len = other.now_len;
		for (size_t i = 0; i < now_len; i++)
		{
			std::construct_at(vec_begin + i, other.vec_begin[i]);
		}
		return *this;
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos) 
	{
		if (pos < 0 || pos >= now_len) throw index_out_of_bound();
		return vec_begin[pos];
	}
	const T & at(const size_t &pos) const 
	{
		if (pos < 0 || pos >= now_len) throw index_out_of_bound();
		return vec_begin[pos];
	}
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos) 
	{
		if (pos < 0 || pos >= now_len) throw index_out_of_bound();
		return vec_begin[pos];
	}
	const T & operator[](const size_t &pos) const 
	{
		if (pos < 0 || pos >= now_len) throw index_out_of_bound();
		return vec_begin[pos];
	}
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const 
	{
		if (now_len == 0) throw container_is_empty();
		return vec_begin[0];
	}
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const 
	{
		if (now_len == 0) throw container_is_empty();
		return vec_end[-1];
	}
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin() 
	{
		return iterator(this, vec_begin);
	}
	const_iterator cbegin() const 
	{
		return const_iterator(this, vec_begin);
	}
	/**
	 * returns an iterator to the end.
	 */
	iterator end() 
	{
		return iterator(this, vec_end);
	}
	const_iterator cend() const 
	{
		return const_iterator(this, vec_end);
	}
	/**
	 * checks whether the container is empty
	 */
	bool empty() const 
	{
		return now_len == 0;
	}
	/**
	 * returns the number of elements
	 */
	size_t size() const 
	{
		return now_len;
	}
	/**
	 * clears the contents
	 */
	void clear() 
	{
		if (vec_begin != nullptr)
		{
			for (size_t i = 0; i < now_len; i++)
			{
				std::destroy_at(vec_begin + i);
			}
			std::allocator<T>().deallocate(vec_begin, extended_len);
		}
		vec_begin = std::allocator<T>().allocate(1);
		vec_end = vec_begin;
		extended_len = 1;
		now_len = 0;
	}
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value) 
	{
		if (pos.vec_ptr < vec_begin || pos.vec_ptr > vec_end) throw invalid_iterator();
		if (extended_len == now_len)
		{
			size_t corrected_extended_len = extended_len * 2;
			T *corrected_vec_begin = std::allocator<T>().allocate(corrected_extended_len);
			pos.vec_ptr = corrected_vec_begin + (pos.vec_ptr - vec_begin);
			for (size_t i = 0; i < now_len; i++)
			{
				std::construct_at(corrected_vec_begin + i, std::move(vec_begin[i]));
				std::destroy_at(vec_begin + i);
			}
			std::allocator<T>().deallocate(vec_begin, extended_len);
			vec_begin = corrected_vec_begin;
			vec_end = vec_begin + now_len;
			extended_len = corrected_extended_len;
		}
		for (T *it = vec_end; it != pos.vec_ptr; it--)
		{
			std::construct_at(it, std::move(*(it - 1)));
			std::destroy_at(it - 1);
		}
		std::construct_at(pos.vec_ptr, value);
		now_len++;
		vec_end++;
		return pos;
	}
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value) 
	{
		if (ind < 0 || ind > now_len) throw index_out_of_bound();
		if (extended_len == now_len)
		{
			size_t corrected_extended_len = extended_len * 2;
			T *corrected_vec_begin = std::allocator<T>().allocate(corrected_extended_len);
			for (size_t i = 0; i < now_len; i++)
			{
				std::construct_at(corrected_vec_begin + i, std::move(vec_begin[i]));
				std::destroy_at(vec_begin + i);
			}
			std::allocator<T>().deallocate(vec_begin, extended_len);
			vec_begin = corrected_vec_begin;
			extended_len = corrected_extended_len;
			vec_end = vec_begin + now_len;
		}
		for (T *it = vec_end; it != vec_begin + ind; it--)
		{
			std::construct_at(it, std::move(*(it - 1)));
			std::destroy_at(it - 1);
		}
		std::construct_at(vec_begin + ind, value);
		now_len++;
		vec_end++;
		return iterator(this, vec_begin + ind);
	}
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos) 
	{
		if (pos.vec_ptr < vec_begin || pos.vec_ptr > vec_end) throw invalid_iterator();
		for (T *it = pos.vec_ptr; it != vec_end - 1; it++)
		{
			std::construct_at(it, std::move(*(it + 1)));
			std::destroy_at(it + 1);
		}
		vec_end--;
		now_len--;
		if (now_len <= extended_len / 4 && now_len != 0)
		{
			T *corrected_vec_begin = std::allocator<T>().allocate(extended_len / 2);
			for (size_t i = 0; i < now_len; i++)
			{
				std::construct_at(corrected_vec_begin + i, std::move(vec_begin[i]));
				std::destroy_at(vec_begin + i);
			}
			std::allocator<T>().deallocate(vec_begin, extended_len);
			vec_begin = corrected_vec_begin;
			extended_len /= 2;
			vec_end = vec_begin + now_len;
		}
		return pos;
	}
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind) 
	{
		if (ind < 0 || ind > now_len) throw index_out_of_bound();
		for (T* it = vec_begin + ind;  it != vec_end - 1; it++)
		{
			std::construct_at(it, std::move(*(it + 1)));
			std::destroy_at(it + 1);
		}
		vec_end--;
		now_len--;
		if (now_len <= extended_len / 4 && now_len != 0)
		{
			T *corrected_vec_begin = std::allocator<T>().allocate(extended_len / 2);
			for (size_t i = 0; i < now_len; i++)
			{
				std::construct_at(corrected_vec_begin + i, std::move(vec_begin[i]));
				std::destroy_at(vec_begin + i);
			}
			std::allocator<T>().deallocate(vec_begin, extended_len);
			vec_begin = corrected_vec_begin;
			extended_len /= 2;
			vec_end = vec_begin + now_len;
		}
		return iterator(this, vec_begin + ind);
	}
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value) 
	{
		if (extended_len == now_len)
		{
			size_t corrected_extended_len = extended_len * 2;
			T *corrected_vec_begin = std::allocator<T>().allocate(corrected_extended_len);
			for (size_t i = 0; i < now_len; i++)
			{
				std::construct_at(corrected_vec_begin + i, std::move(vec_begin[i]));
				std::destroy_at(vec_begin + i);
			}
			std::allocator<T>().deallocate(vec_begin, extended_len);
			vec_begin = corrected_vec_begin;
			extended_len = corrected_extended_len;
			vec_end = vec_begin + now_len;
		}
		std::construct_at(vec_end, value);
		vec_end++;
		now_len++;
	}
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back() 
	{
		if (now_len == 0) throw container_is_empty();
		std::destroy_at(vec_end - 1);
		vec_end--;
		now_len--;
		if (now_len <= extended_len / 4 && now_len != 0)
		{
			T *corrected_vec_begin = std::allocator<T>().allocate(extended_len / 2);
			for (size_t i = 0; i < now_len; i++)
			{
				std::construct_at(corrected_vec_begin + i, std::move(vec_begin[i]));
				std::destroy_at(vec_begin + i);
			}
			std::allocator<T>().deallocate(vec_begin, extended_len);
			vec_begin = corrected_vec_begin;
			extended_len /= 2;
			vec_end = vec_begin + now_len;
		}
	}
};
}

#endif
