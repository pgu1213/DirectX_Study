#pragma once
#include <pch.h>

template<typename T>
class Singleton
{
protected:
	explicit Singleton() = default;
	virtual ~Singleton() = default;
public:
	static T* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new T;
		}

		return m_pInstance;
	}
	static BOOL DestoryInstance()
	{
		if (m_pInstance)
		{
			SafeDelete(m_pInstance);
		}

		return TRUE;
	}

private:
	static T* m_pInstance;
};

template<typename T>
T* Singleton<T>::m_pInstance = nullptr;