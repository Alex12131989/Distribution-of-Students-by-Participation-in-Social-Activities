#pragma once
#include <cinttypes>

extern "C"
{
	int64_t* DetectBelowAverageUsers(float* gpas, float* incomes, int64_t* activity, int64_t number_of_persons);
	void FreeMemory(int64_t* ptr);
}