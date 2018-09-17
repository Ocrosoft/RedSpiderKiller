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
	// ��ʼ��
	bool Init();
	// ���ڻ�
	void Window();
	// ���أ�����ǿ�ƽ������Ͼ���������ʦ�˻���ʾ��
	void Hide();
	// ����֮��������ʾ
	void Show();

	// ���ð������ڣ�����Ҫ Init
	// @param window��Ҫ���õĴ��ھ��
	void SetTopWindow(HWND window);
	// ���ú�֩�봰��
	// @param window��Ҫ�������õĴ��ھ��
	// @return���� window �Ǻ�֩��Ĵ���ʱ�����óɹ����� true
	bool SetSpiderWindow(HWND window);

	// �ֶ����ú�֩��·��������Ҫ Init
	// @param filePath����ִ���ļ�������·��
	void SetFilePath(const std::wstring& filePath);

	bool IsHide();

private:
	// ��ȡ��֩���Ŀ¼���ļ���
	bool GetPath();

private:
	struct RegisterPath
	{
		// ע����Ŀ¼
		HKEY rootPath;
		// ע�����·��
		std::wstring subPath;
		// ע������
		std::wstring key;
	};
	// ��֩���ע���·������Ȼ���� x86 �� x64�����ǵ�һ��·���� x64 ��Ҳ����
	const std::vector<RegisterPath> PathsInRegister
	{
		// 7.2 x86
		{ HKEY_LOCAL_MACHINE, L"SOFTWARE\\3000soft\\Red Spider", L"AgentCommand" },
		// 7.2 x64
		{ HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\3000soft\\Red Spider", L"AgentCommand" },
	};

private:
	// ��֩���Ŀ¼
	std::wstring m_rootPath;
	// ��֩���ļ���
	std::wstring m_fileName;

	// ���ڰ�����֩�봰�ڵĴ���
	HWND m_topWindow = nullptr;
	// ��֩��Ĵ���
	HWND m_spiderWindow = nullptr;

	bool m_inited = false;
	bool m_hided = true;
};