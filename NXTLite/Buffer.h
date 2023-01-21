#pragma once
class Buffer
{
private:
	int m_array[512];
	int m_offset;
public:
	Buffer();
	void writeByte(int);
	void writeShort(int);
	void writeInt(int);
	void writeBool(bool);

	void print();
};

