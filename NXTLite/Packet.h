#pragma once
class Packet
{
private:
	int m_offset;
public:
	Packet();

	unsigned char m_array[512]{0};

	void writeByte(int);

	void writeShort(int);

	void writeInt(int);

	void writeBool(bool);

	int readShort();

	int readInt();

	void print();
};
