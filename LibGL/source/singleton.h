// Singleton class written by Osama Elsharqawy 1/1/2025

#pragma once

#include <cassert>
#include <cstdint>

template <typename T> class CSingleton
{
private:
	static T* ms_pSingleton;

public:
	CSingleton()
	{
		assert(!ms_pSingleton);
		std::uintptr_t offset = (std::uintptr_t)(T*)1 - (std::uintptr_t)(CSingleton<T>*)(T*)1;
		ms_pSingleton = (T*)((std::uintptr_t) this + offset);
	}

	virtual ~CSingleton()
	{
		assert(ms_pSingleton);
		ms_pSingleton = nullptr;
	}

	static T& Instance()
	{
		assert(ms_pSingleton);
		return (*ms_pSingleton);
	}

	static T* InstancePtr()
	{
		assert(ms_pSingleton);
		return (ms_pSingleton);
	}
};

template <typename T> T* CSingleton <T>::ms_pSingleton = nullptr;