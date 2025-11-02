#pragma once

extern "C"
{
	void Encode(char* plaintext, char* key);
	void Decode(char* encrypted_text, char* key);
}