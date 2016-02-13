#pragma once
#include <objbase.h>
#include <atomic>
#include "ComModule.h"

template <typename Type, typename Interface, const CLSID* Clsid = nullptr>
class ComObject : public Interface
{
protected:
	std::atomic<int> referenceCount = 0;

public:
	ComObject()
	{
		ComModule::GetInstance().AddRef();
	}
	ComObject(const ComObject<Type, Interface, Clsid>& rhs) = delete;
	virtual ~ComObject()
	{
		ComModule::GetInstance().Release();
	}

	ComObject<Type, Interface, Clsid>& operator=(const ComObject<Type, Interface, Clsid>& rhs) = delete;

	HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override
	{
		if (ppvObject == nullptr)
			return E_POINTER;
		if (riid != IID_IUnknown &&
			riid != __uuidof(Interface))
			return E_NOINTERFACE;
		++referenceCount;
		*ppvObject = static_cast<Interface*>(this);
		return S_OK;
	}

	ULONG __stdcall AddRef() final
	{
		return ++referenceCount;
	}

	ULONG __stdcall Release() final
	{
		auto result = --referenceCount;
		if (result == 0)
		{
			OnFinalRelease();
			delete this;
		}
		return result;
	}

	virtual HRESULT OnFinalConstruct()
	{
		return S_OK;
	}

	virtual void OnFinalRelease()
	{
	}

	static HRESULT TryCreateInstance(REFIID riid, void** ppvObject)
	{
		if (ppvObject == nullptr)
			return E_POINTER;
		auto object = new Type();
		auto hr = object->OnFinalConstruct();
		if (SUCCEEDED(hr))
			hr = object->QueryInterface(riid, ppvObject);
		if (FAILED(hr))
			delete object;
		return hr;
	}

	static bool Is(REFCLSID rclsid)
	{
		return Clsid != nullptr && rclsid == *Clsid;
	}
};