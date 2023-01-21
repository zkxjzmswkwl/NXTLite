#include "pch.h"
#include "Buffer.h"
#include <array>

Buffer::Buffer()
{
	m_offset = 0;
}


void Buffer::writeByte(int toWrite)
{
	m_array[++m_offset - 1] = (BYTE)toWrite;
}

void Buffer::writeShort(int toWrite)
{
	m_array[++m_offset - 1] = (BYTE)(toWrite >> 8);
	writeByte(toWrite);
}

void Buffer::writeInt(int toWrite)
{
	m_array[++m_offset - 1] = (BYTE)(toWrite >> 24);
	m_array[++m_offset - 1] = (BYTE)(toWrite >> 16);
	m_array[++m_offset - 1] = (BYTE)(toWrite >> 8);
	writeByte(toWrite);
}

void Buffer::writeBool(bool toWrite)
{
	writeByte(toWrite ? 1 : 0);
}

void Buffer::print()
{
    for (size_t i = 0; i < std::end(m_array)-std::begin(m_array); ++i) {
        printf("%01x ", (uint8_t)m_array[i]);
    }
	printf("\n");
}
