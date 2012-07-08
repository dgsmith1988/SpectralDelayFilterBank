#ifndef CIRCULARBUFFER_H_INCLUDED
#define CIRCULARBUFFER_H_INCLUDED

#include<iostream>
#include<vector>

/*!
* \brief Template for creating a circular buffer with numeric types 
* \details The number of items put in the buffer should not exceed the limits of an int in order to accomodate the indexing functions
* \author Graham
* \date 7/7/2012
*/
template<class T> class CircularBuffer
{
public:
	CircularBuffer();
	CircularBuffer(int size);
	~CircularBuffer(void);
	int getLength(void);
	void addData(T data);
	void changeSize(int newSize);
	void clearContents(void);
	//this operator refers to the most recent sample at [0] and the oldest at [length - 1]
	T& operator[](const unsigned int n);
private:

	/*!
	 * Stores the length of the buffer
	 */

	unsigned int length;

	/*!
	 * Vector containing the items in the buffer
	 */
	std::vector<double> buffer;

	/*!
	 * Write pointer indicating where the next sample will be written to the buffer (also the oldest sample)
	 */
	unsigned int write_ptr; 
};

/*!
 * Exception thrown if the index passed to the subscript operator is out of bounds or negative
 * \author Graham
 * \date 7/7/2012
 */

class Bad_index{};

/*!
 * Default constructor that creates a buffer of length 10 in order to satisfy certain compilers
 *
 * \author Graham
 * \date 7/7/2012
 *
 * \tparam T Type of data to be stored in the buffer
 *
 */

template<class T> CircularBuffer<T>::CircularBuffer()
{
	buffer = std::vector<T>(10);
	write_ptr = 0;
	length = 10;
}

/*!
 * Constructor to create a CircularBuffer
 *
 * \author Graham
 * \date 7/7/2012
 *
 * \tparam T Type of data to be stored in the buffer
 * \param size The size of the buffer
 *
 */

template<class T> CircularBuffer<T>::CircularBuffer(int size)
{
	buffer = std::vector<T>(size);
	write_ptr = 0;
	length = size;
}

/*!
 * Destructor
 *
 * \author Graham
 * \date 7/7/2012
 *
 */

template<class T> CircularBuffer<T>::~CircularBuffer(void)
{
}

/*!
 * Gets the length of the buffer
 *
 * \author Graham
 * \date 7/7/2012
 *
 * \tparam T Generic type parameter
 *
 * \return The length of the buffer
 */

template<class T> int CircularBuffer<T>::getLength(void)
{
	return length;
}

/*!
 * Adds data to the buffer replacing the oldest item currently in it
 *
 * \author Graham
 * \date 7/7/2012
 *
 * \tparam T Generic type parameter
 * \param data The data to be added
 */

template<class T> void CircularBuffer<T>::addData(T data)
{
	buffer[write_ptr] = data;
	//increment the pointer and wrap around if necessary
	write_ptr = (write_ptr == length - 1) ? 0 : ++write_ptr;
}

/*!
 * Clears the contents of the buffer by setting them to 0
 *
 * \author Graham
 * \date 7/7/2012
 *
 * \tparam T Generic type parameter
 */

template<class T> void CircularBuffer<T>::clearContents(void)
{
    for(unsigned int i = 0; i < length; ++i)
    {
        buffer[i] = 0;
    }
}

/*!
 * Changes the size of the buffer. If newSize is less than the old size then some of the elements stored in the buffer will be dropped.
 * Any important data should be saved before resizing the buffer and then copied back in afterwards.
 * 
 * \author Graham
 * \date 7/7/2012
 *
 * \tparam T Generic type parameter
 * \param newSize Size the buffer will be changed to
 */

template<class T> void CircularBuffer<T>::changeSize(int newSize)
{
    buffer.resize(newSize);
    length = newSize;
}

/*!
 * An overloaded subscripting operator. [0] refers to the most recent element in the array and [n] refers to element n samples before.
 *
 * \author Graham
 * \date 7/7/2012
 *
 * \exception Bad_index Thrown when a bad index error condition occurs.
 *
 * \param n The index of the desired data
 *
 * \return The indexed value.
 */

template<class T> T& CircularBuffer<T>::operator[](const unsigned int n)
{
	if( n < 0 || n > length - 1)
	{
		std::cerr << "Bad index passed as argument to a CircularBuffer, either negative or too big" << std::endl;
		throw Bad_index();
	}

	//determine the index of the most recent sample
	unsigned int newest = (write_ptr == 0) ? length - 1 : write_ptr - 1;

	//return the selected value, depending on the buffer size a single int might not be able to hold the offset
	int offset = newest - n;
	return (offset >= 0) ? buffer[offset] : buffer[length + offset];
}
#endif // CIRCULARBUFFER_H_INCLUDED