#ifndef NG_ROBOT_PACKET_CODING_XOR_H_
#define NG_ROBOT_PACKET_CODING_XOR_H_

class PacketCodingXor
{
public:
	void Encrypt(void* data, size_t data_len);

	void Decrypt(void* data, size_t data_len);
};

#endif