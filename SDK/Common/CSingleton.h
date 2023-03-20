#pragma once

#ifndef CSINGLETON_H
#define CSINGLETON_H

template <class T>
class CSingleton
{
protected:
	CSingleton(void) = default;
	CSingleton(const CSingleton&) = delete;
	CSingleton(CSingleton&&) = delete;

	CSingleton& operator=(const CSingleton&) = delete;
	CSingleton& operator=(CSingleton&&) = delete;

public:
	static T* GetInstance(void)
	{
		static T instance;
		return &instance;
	}
};

#endif