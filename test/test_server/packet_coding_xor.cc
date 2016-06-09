#include "packet_coding_xor.h"
#include <common/include/typedef.h>

using namespace evl::def;

static const char* s_codes = "3V3E2GSDF@3GS3#DD";

void PacketCodingXor::Encrypt(void* data, size_t data_len)
{
	char* data_ptr = static_cast<char*>(data);

	static uint32 code_len = std::strlen(s_codes);
	for (uint32 n = 0; n < data_len; ++n)
	{
		data_ptr[n] ^= s_codes[n % code_len];
	}
}

void PacketCodingXor::Decrypt(void* data, size_t data_len)
{
	char* data_ptr = static_cast<char*>(data);

	static uint32 code_len = std::strlen(s_codes);
	for (uint32 n = 0; n < data_len; ++n)
	{
		data_ptr[n] ^= s_codes[n % code_len];
	}
}
