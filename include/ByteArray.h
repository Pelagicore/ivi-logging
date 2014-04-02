#pragma once

#include <vector>
#include <string>
#include <string.h>
#include <assert.h>

namespace pelagicore {

std::string byteArrayToString(const void* buffer, size_t length);

/**
 * A vector-like class which uses a statically allocated buffer if the data is small enough.
 */
class ByteArray {

public:
	ByteArray() {
		m_dynamicData = NULL;
		m_length = 0;

#ifndef NDEBUG
		memset( m_staticData, 0, sizeof(m_staticData) );
#endif
	}

	ByteArray(const ByteArray& b) {
		// duplicate content
		append( b.getData(), b.size() );
	}

	ByteArray& operator=(const ByteArray& right) {
		resize( right.size() );
		memcpy( getData(), right.getData(), right.size() );
		return *this;
	}

	~ByteArray() {
		if ( !usesStaticBuffer() )
			delete (m_dynamicData);
	}

	size_t size() const {
		return ( usesStaticBuffer() ? m_length : m_dynamicData->size() );
	}

	bool usesStaticBuffer() const {
		return (m_dynamicData == NULL);
	}

	unsigned char* getData() {
		if ( usesStaticBuffer() )
			return m_staticData;
		else
			return &( (*m_dynamicData)[0] );
	}

	unsigned const char* getData() const {
		if ( usesStaticBuffer() )
			return m_staticData;
		else
			return &( (*m_dynamicData)[0] );
	}

	unsigned char& operator[](size_t index) {
		//		assert(index < m_length);
		return getData()[index];
	}

	void resize(size_t size) {

		if ( !usesStaticBuffer() ) {
			m_dynamicData->resize(size);
		} else {
			if ( size <= sizeof(m_staticData) ) {
				m_length = size;
			} else {
				m_dynamicData = new std::vector<unsigned char>();
				m_dynamicData->resize(size);
				memcpy(m_dynamicData->data(), m_staticData, m_length);
				m_length = -1; // this field is not relevant anymore
			}
		}

	}

	void writeAt(size_t position, const void* rawDataPtr, size_t sizeInByte) {
		assert(m_length >= position + sizeInByte);
		memcpy(&(getData()[position]), rawDataPtr, sizeInByte);
	}

	void append(const void* rawDataPtr, size_t sizeInByte) {
		auto i = size();
		resize(size() + sizeInByte);
		memcpy(getData() + i, rawDataPtr, sizeInByte);
	}

	void append(unsigned char v) {
		auto i = size();
		resize(size() + 1);
		getData()[i] = v;
	}

	size_t capacity() const {
		if ( usesStaticBuffer() )
			return sizeof(m_staticData);
		else
			return m_dynamicData->size();
	}

	/**
	 * Reserve space for a block of the given size and return the position of the block
	 */
	size_t skip(size_t length) {
		auto returnValue = size();
		char dummyData[length];
		memset(dummyData, 0, length);
		append( dummyData, sizeof(dummyData) );
		return returnValue;
	}

	std::string toString() const {
		return byteArrayToString( getData(), size() );
	}

private:
	std::vector<unsigned char>* m_dynamicData = NULL;
	unsigned char m_staticData[10];
	size_t m_length = 0;
};

typedef ByteArray ResizeableByteArray;

class LogData;

LogData& operator<<(LogData& log, const ByteArray& v);

}
