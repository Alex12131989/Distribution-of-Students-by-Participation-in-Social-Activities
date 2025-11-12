#pragma once

extern "C"
{
	void Encode(char* key, char* plaintext);
	void Decode(char* key, char* encrypted_text);
	char* GetKey(char* source);
	extern char alphabet[];
	extern size_t alphabet_size;
}