#include "pch.h"
#include "packet.h"
#include <array>
#include <stdint.h>
#include <iomanip>

Packet::Packet()
{
	m_offset = 0;
	memset(this->m_array, 0, 512);
}

void Packet::writeByte(int32_t toWrite)
{
	m_array[++m_offset - 1] = (BYTE)toWrite;
}

void Packet::writeShort(int32_t toWrite)
{
	m_array[++m_offset - 1] = (BYTE)(toWrite >> 8);
	m_array[++m_offset - 1] = (BYTE)toWrite;
}

void Packet::writeInt(int toWrite)
{
	m_array[++m_offset - 1] = (BYTE)(toWrite >> 24);
	m_array[++m_offset - 1] = (BYTE)(toWrite >> 16);
	m_array[++m_offset - 1] = (BYTE)(toWrite >> 8);
	m_array[++m_offset - 1] = (BYTE)(toWrite);
}

void Packet::writeBool(bool toWrite)
{
	writeByte(toWrite ? 1 : 0);
	m_offset++;
}

int Packet::readShort()
{
	m_offset += 2;
	return ((m_array[m_offset - 2] & 0xff) << 8) + (m_array[m_offset - 1] & 0xff);
}

int Packet::readInt()
{
	m_offset += 4;
	return 	((m_array[m_offset -4]  & 0xff) << 24)  +
		((m_array[m_offset - 3] & 0xff) << 16)  +
		((m_array[m_offset - 2] & 0xff) << 8)   + 
		(m_array[m_offset - 1]  & 0xff);
}

void Packet::print()
{
	for (int i = 0; i < m_offset; ++i)
	{
		std::cout << std::setw(2) << std::setfill('0') << std::hex << (int)(m_array[i]) << " ";
	}
	std::cout << std::dec << "\n";
}
