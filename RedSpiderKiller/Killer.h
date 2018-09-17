#pragma once

#include <memory>
#include <vector>
#include <string>

#include <Windows.h>

class Killer
	: public std::enable_shared_from_this<Killer>
{
public:
	static std::shared_ptr<Killer> Create();

private:
	Killer() = default;

public:
	~Killer();

public:
	// 初始化
	bool Init();
	// 窗口化
	void Window();
	// 隐藏（当做强制结束，毕竟结束掉教师端会提示）
	void Hide();
	// 隐藏之后重新显示
	void Show();

	// 设置包含窗口，不需要 Init
	// @param window：要设置的窗口句柄
	void SetTopWindow(HWND window);
	// 设置红蜘蛛窗口
	// @param window：要尝试设置的窗口句柄
	// @return：当 window 是红蜘蛛的窗口时，设置成功返回 true
	bool SetSpiderWindow(HWND window);

	// 手动设置红蜘蛛路径，不需要 Init
	// @param filePath：可执行文件的完整路径
	void SetFilePath(const std::wstring& filePath);

	bool IsHide();

private:
	// 获取红蜘蛛根目录及文件名
	bool GetPath();

private:
	struct RegisterPath
	{
		// 注册表根目录
		HKEY rootPath;
		// 注册表子路径
		std::wstring subPath;
		// 注册表键名
		std::wstring key;
	};
	// 红蜘蛛的注册表路径。虽然分了 x86 和 x64，但是第一个路径在 x64 下也可用
	const std::vector<RegisterPath> PathsInRegister
	{
		// 7.2 x86
		{ HKEY_LOCAL_MACHINE, L"SOFTWARE\\3000soft\\Red Spider", L"AgentCommand" },
		// 7.2 x64
		{ HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\3000soft\\Red Spider", L"AgentCommand" },
	};

private:
	// 红蜘蛛根目录
	std::wstring m_rootPath;
	// 红蜘蛛文件名
	std::wstring m_fileName;

	// 用于包含红蜘蛛窗口的窗口
	HWND m_topWindow = nullptr;
	// 红蜘蛛的窗口
	HWND m_spiderWindow = nullptr;

	bool m_inited = false;
	bool m_hided = true;
};