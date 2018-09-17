#include "stdafx.h"
#include "Killer.h"

#define ENSURE_INITED if (!m_inited) return

static void SplitPathAndFileName(std::wstring& path, std::wstring& name)
{
	while (!path.empty() && path.back() != '\\')
	{
		name.insert(name.begin(), path.back());
		path.pop_back();
	}
}

static bool IsFileExists(const std::wstring& path)
{
	WIN32_FIND_DATA nextInfo;
	auto file = ::FindFirstFile(path.c_str(), &nextInfo);

	return file != INVALID_HANDLE_VALUE;
}

Killer::~Killer()
{
	SetParent(m_spiderWindow, nullptr);
}

std::shared_ptr<Killer> Killer::Create()
{
	return std::shared_ptr<Killer>(new Killer());
}

bool Killer::Init()
{
	if (!GetPath())
	{
		m_inited = false;
		return false;
	}

	m_inited = true;

	return true;
}

void Killer::Window()
{
	ENSURE_INITED;

	// 没有设置包含窗口或者红蜘蛛窗口
	if (m_topWindow == nullptr || m_spiderWindow == nullptr)
	{
		return;
	}

	::SetParent(m_spiderWindow, m_topWindow);
}

void Killer::Hide()
{
	ENSURE_INITED;

	::ShowWindow(m_topWindow, SW_HIDE);

	m_hided = true;
}

void Killer::Show()
{
	ENSURE_INITED;

	::ShowWindow(m_topWindow, SW_SHOW);

	m_hided = false;
}

void Killer::SetTopWindow(HWND window)
{
	m_topWindow = window;
}

bool Killer::SetSpiderWindow(HWND window)
{
	ENSURE_INITED false;

	// 获取窗口对应的可执行文件的路径
	TCHAR szName[MAX_PATH] = { 0 };
	if (window)
	{
		DWORD dwId = 0;
		::GetWindowThreadProcessId(window, &dwId);
		HANDLE handle = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 0, dwId);
		if (handle)
		{
			DWORD dwLen = MAX_PATH;
			::QueryFullProcessImageName(handle, 0, szName, &dwLen);
		}
	}

	// 跟获取到的一样
	if (std::wstring(szName) == m_rootPath + m_fileName)
	{
		m_spiderWindow = window;
		return true;
	}
	else
	{
		return false;
	}
}

void Killer::SetFilePath(const std::wstring& filePath)
{
	m_rootPath = filePath;
	SplitPathAndFileName(m_rootPath, m_fileName);

	m_inited = true;
}

bool Killer::IsHide()
{
	ENSURE_INITED false;

	return m_hided;
}

bool Killer::IsInited()
{
	return m_inited;
}

bool Killer::GetPath()
{
	for (auto pathInRegister : PathsInRegister)
	{
		m_rootPath.clear();
		m_fileName.clear();

		HKEY key;
		auto status = ::RegOpenKeyEx(pathInRegister.rootPath, pathInRegister.subPath.c_str(), 0, KEY_READ, &key);
		if (status != ERROR_SUCCESS)
		{
			continue;
		}

		DWORD type;
		DWORD size;
		status = ::RegQueryValueEx(key, pathInRegister.key.c_str(), NULL, &type, NULL, &size);
		if (status != ERROR_SUCCESS)
		{
			::RegCloseKey(key);
			continue;
		}

		std::unique_ptr<BYTE[]> value(new BYTE[size]);
		status = ::RegQueryValueEx(key, pathInRegister.key.c_str(), NULL, &type, value.get(), &size);
		if (status != ERROR_SUCCESS)
		{
			::RegCloseKey(key);
			continue;
		}

		::RegCloseKey(key);

		for (unsigned int i = 0; i < size; i++)
		{
			if (value[i] != '\0')
			{
				m_rootPath.push_back(value[i]);
			}
		}

		SplitPathAndFileName(m_rootPath, m_fileName);

		if (!IsFileExists(m_rootPath + m_fileName))
		{
			continue;
		}

		return true;
	}

	return false;
}