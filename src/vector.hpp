#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <new>  // For placement new

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
	// Type definitions for the vector
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;
	using size_type = size_t;
	using value_type = T;

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
	// About iterator_category: https://blog.csdn.net/u013164612/article/details/78587488
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::random_access_iterator_tag;

	private:
		pointer ptr;
		const vector<T>* vec;  // Pointer to the vector this iterator belongs to

	public:
		iterator() : ptr(nullptr), vec(nullptr) {}
		iterator(pointer p, const vector<T>* v) : ptr(p), vec(const_cast<vector<T>*>(v)) {}

		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator operator+(const int &n) const
		{
			return iterator(ptr + n, vec);
		}
		iterator operator-(const int &n) const
		{
			return iterator(ptr - n, vec);
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const
		{
			if (vec != rhs.vec) {
				throw sjtu::invalid_iterator();
			}
			return static_cast<int>(ptr - rhs.ptr);
		}
		iterator& operator+=(const int &n)
		{
			ptr += n;
			return *this;
		}
		iterator& operator-=(const int &n)
		{
			ptr -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		iterator operator++(int)
		{
			iterator tmp = *this;
			++ptr;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator& operator++()
		{
			++ptr;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int)
		{
			iterator tmp = *this;
			--ptr;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator& operator--()
		{
			--ptr;
			return *this;
		}
		/**
		 * TODO *it
		 */
		T& operator*() const
		{
			return *ptr;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const
		{
			return ptr == rhs.ptr;
		}
		bool operator==(const const_iterator &rhs) const
		{
			return ptr == rhs.get_ptr();
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const
		{
			return ptr != rhs.ptr;
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return ptr != rhs.get_ptr();
		}
		
		// Helper function to get raw pointer for const_iterator comparison
		pointer get_ptr() const { return ptr; }
	};
	/**
	 * TODO
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::random_access_iterator_tag;

	private:
		pointer ptr;
		const vector<T>* vec;  // Pointer to the vector this iterator belongs to

	public:
		const_iterator() : ptr(nullptr), vec(nullptr) {}
		const_iterator(pointer p, const vector<T>* v) : ptr(p), vec(v) {}
		const_iterator(const const_iterator& other) : ptr(other.ptr), vec(other.vec) {}
		
		// Allow construction from non-const iterator
		const_iterator(const iterator& it, const vector<T>* v = nullptr) : ptr(it.get_ptr()), vec(v) {}

		const_iterator operator+(const int &n) const
		{
			return const_iterator(ptr + n, vec);
		}
		const_iterator operator-(const int &n) const
		{
			return const_iterator(ptr - n, vec);
		}
		int operator-(const const_iterator &rhs) const
		{
			if (vec != rhs.vec) {
				throw sjtu::invalid_iterator();
			}
			return static_cast<int>(ptr - rhs.ptr);
		}
		int operator-(const iterator &rhs) const
		{
			return static_cast<int>(ptr - rhs.get_ptr());
		}
		
		const_iterator& operator+=(const int &n)
		{
			ptr += n;
			return *this;
		}
		const_iterator& operator-=(const int &n)
		{
			ptr -= n;
			return *this;
		}
		/**
		 * TODO iter++
		 */
		const_iterator operator++(int)
		{
			const_iterator tmp = *this;
			++ptr;
			return tmp;
		}
		/**
		 * TODO ++iter
		 */
		const_iterator& operator++()
		{
			++ptr;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		const_iterator operator--(int)
		{
			const_iterator tmp = *this;
			--ptr;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		const_iterator& operator--()
		{
			--ptr;
			return *this;
		}
		/**
		 * TODO *it
		 */
		const T& operator*() const
		{
			return *ptr;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const
		{
			return ptr == rhs.get_ptr();
		}
		bool operator==(const const_iterator &rhs) const
		{
			return ptr == rhs.ptr;
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const
		{
			return ptr != rhs.get_ptr();
		}
		bool operator!=(const const_iterator &rhs) const
		{
			return ptr != rhs.ptr;
		}
		
		// Helper function to get raw pointer
		pointer get_ptr() const { return ptr; }
	};
	
private:
	pointer data_;
	size_t size_;
	size_t capacity_;
	
	// Allocate and construct elements using placement new
	pointer allocate_and_copy(size_t n, const T* src)
	{
		pointer p = static_cast<pointer>(::operator new(n * sizeof(T)));
		for (size_t i = 0; i < n; ++i) {
			new (p + i) T(src[i]);
		}
		return p;
	}
	
	// Destroy elements but don't free memory
	void destroy_elements(pointer p, size_t n)
	{
		for (size_t i = 0; i < n; ++i) {
			p[i].~T();
		}
	}
	
	// Reallocate with more capacity
	void reallocate(size_t new_cap)
	{
		pointer new_data = static_cast<pointer>(::operator new(new_cap * sizeof(T)));
		// Move existing elements
		for (size_t i = 0; i < size_; ++i) {
			new (new_data + i) T(std::move(data_[i]));
			data_[i].~T();
		}
		::operator delete(data_);
		data_ = new_data;
		capacity_ = new_cap;
	}

public:
	/**
	 * TODO Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector() : data_(nullptr), size_(0), capacity_(0) {}
	
	vector(const vector &other) : data_(nullptr), size_(0), capacity_(0)
	{
		if (other.size_ > 0) {
			data_ = allocate_and_copy(other.size_, other.data_);
			size_ = other.size_;
			capacity_ = other.size_;
		}
	}
	
	/**
	 * TODO Destructor
	 */
	~vector()
	{
		if (data_ != nullptr) {
			destroy_elements(data_, size_);
			::operator delete(data_);
		}
	}
	
	/**
	 * TODO Assignment operator
	 */
	vector &operator=(const vector &other)
	{
		if (this != &other) {
			// Clear current data
			if (data_ != nullptr) {
				destroy_elements(data_, size_);
				::operator delete(data_);
				data_ = nullptr;
			}
			size_ = 0;
			capacity_ = 0;
			
			// Copy new data
			if (other.size_ > 0) {
				data_ = allocate_and_copy(other.size_, other.data_);
				size_ = other.size_;
				capacity_ = other.size_;
			}
		}
		return *this;
	}
	
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos)
	{
		if (pos >= size_) {
			throw sjtu::index_out_of_bound();
		}
		return data_[pos];
	}
	
	const T & at(const size_t &pos) const
	{
		if (pos >= size_) {
			throw sjtu::index_out_of_bound();
		}
		return data_[pos];
	}
	
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos)
	{
		if (pos >= size_) {
			throw sjtu::index_out_of_bound();
		}
		return data_[pos];
	}
	
	const T & operator[](const size_t &pos) const
	{
		if (pos >= size_) {
			throw sjtu::index_out_of_bound();
		}
		return data_[pos];
	}
	
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
	const T & front() const
	{
		if (size_ == 0) {
			throw sjtu::container_is_empty();
		}
		return data_[0];
	}
	
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const
	{
		if (size_ == 0) {
			throw sjtu::container_is_empty();
		}
		return data_[size_ - 1];
	}
	
	/**
	 * returns an iterator to the beginning.
	 */
	iterator begin()
	{
		return iterator(data_, this);
	}
	
	const_iterator begin() const
	{
		return const_iterator(data_, this);
	}
	
	const_iterator cbegin() const
	{
		return const_iterator(data_, this);
	}
	
	/**
	 * returns an iterator to the end.
	 */
	iterator end()
	{
		return iterator(data_ + size_, this);
	}
	
	const_iterator end() const
	{
		return const_iterator(data_ + size_, this);
	}
	
	const_iterator cend() const
	{
		return const_iterator(data_ + size_, this);
	}
	
	/**
	 * checks whether the container is empty
	 */
	bool empty() const
	{
		return size_ == 0;
	}
	
	/**
	 * returns the number of elements
	 */
	size_t size() const
	{
		return size_;
	}
	
	/**
	 * clears the contents
	 */
	void clear()
	{
		if (data_ != nullptr) {
			destroy_elements(data_, size_);
			::operator delete(data_);
			data_ = nullptr;
		}
		size_ = 0;
		capacity_ = 0;
	}
	
	/**
	 * inserts value before pos
	 * returns an iterator pointing to the inserted value.
	 */
	iterator insert(iterator pos, const T &value)
	{
		// Calculate position index
		size_t index = static_cast<size_t>(pos.get_ptr() - data_);
		if (index > size_) {
			index = size_;  // If pos is end, insert at end
		}
		return insert(index, value);
	}
	
	/**
	 * inserts value at index ind.
	 * after inserting, this->at(ind) == value
	 * returns an iterator pointing to the inserted value.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value)
	{
		if (ind > size_) {
			throw sjtu::index_out_of_bound();
		}
		
		// Expand if necessary
		if (size_ >= capacity_) {
			size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
			reallocate(new_cap);
		}
		
		// Shift elements to the right
		for (size_t i = size_; i > ind; --i) {
			new (data_ + i) T(std::move(data_[i - 1]));
			data_[i - 1].~T();
		}
		
		// Insert new element
		new (data_ + ind) T(value);
		++size_;
		
		return iterator(data_ + ind, this);
	}
	
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos)
	{
		pointer p = pos.get_ptr();
		if (p < data_ || p >= data_ + size_) {
			throw sjtu::index_out_of_bound();
		}
		return erase(static_cast<size_t>(p - data_));
	}
	
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind)
	{
		if (ind >= size_) {
			throw sjtu::index_out_of_bound();
		}
		
		// Destroy element at ind
		data_[ind].~T();
		
		// Shift elements to the left
		for (size_t i = ind; i < size_ - 1; ++i) {
			new (data_ + i) T(std::move(data_[i + 1]));
			data_[i + 1].~T();
		}
		
		--size_;
		return iterator(data_ + ind, this);
	}
	
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value)
	{
		// Expand if necessary
		if (size_ >= capacity_) {
			size_t new_cap = (capacity_ == 0) ? 1 : capacity_ * 2;
			reallocate(new_cap);
		}
		new (data_ + size_) T(value);
		++size_;
	}
	
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back()
	{
		if (size_ == 0) {
			throw sjtu::container_is_empty();
		}
		--size_;
		data_[size_].~T();
	}
};


}
#endif