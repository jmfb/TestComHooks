#pragma once
#include "Object.h"
#include "CppCom.h"
#include <iostream>

class FooLate : public Com::Object<FooLate, Com::InterfaceList<IFooLate>, &CLSID_FooLate>
{
public:
	FooLate()
	{
		std::cout << "C++ FooLate(ctor)" << std::endl;
	}

	~FooLate()
	{
		std::cout << "C++ ~FooLate()" << std::endl;
	}

	HRESULT __stdcall BarLate() final
	{
		std::cout << __FUNCTION__ << std::endl;
		return S_OK;
	}
};
