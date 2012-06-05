#include "StdAfx.h"
#include "GlobalCenter.h"

CGlobalCenter Global2;			// 实体
IGlobal* Global = &Global2;		// App全局访问接口

CGlobalCenter::CGlobalCenter(void)
{
}


CGlobalCenter::~CGlobalCenter(void)
{
}

HRESULT CGlobalCenter::Initialize( IModule* UpperFrame/*=NULL*/ )
{
	_InitializeLog();		// 所有地方都依赖log，所以当最先初始化
	_InitializeConfig();
	_InitializeNetCenter();
	return S_OK;
}

HRESULT CGlobalCenter::UnInitialize()
{
	_UnInitializeNetCenter();
	_UnInitializeConfig();
	_UnInitializeLog();
	return S_OK;
}

HRESULT CGlobalCenter::Run()
{
	return S_OK;
}

HRESULT CGlobalCenter::Terminate()
{
	return S_OK;
}

HRESULT CGlobalCenter::GetIConfig( IConfig** pConfig )
{
	*pConfig = dynamic_cast<IConfig*>(&m_config);
	return S_OK;
}

void CGlobalCenter::_InitializeConfig()
{
	m_config.Initialize(NULL);
	GetCurrentDirectory(MAX_PATH,m_configFilePath.GetBuffer(MAX_PATH));
	m_configFilePath.ReleaseBuffer();
	m_configFilePath += PathSplit;
	m_configFilePath += CONFIGXMLFILE;
	m_config.LoadConfigFromXML(m_configFilePath);
}

void CGlobalCenter::_UnInitializeConfig()
{
	m_config.SaveConfigToXML();
	m_config.UnInitialize();
}

HRESULT CGlobalCenter::CheckLastError(LPCTSTR helpStr)
{
	DWORD dw = GetLastError();
	CString str;
	str.Format(_T("检查LastError : %d"),dw);
	if (helpStr)
	{
		str.AppendFormat(_T("(%s)"),helpStr);
	}
	Log(LogFile_General,str);
	return dw;
}

HRESULT CGlobalCenter::PtrAssert( void* p )
{
	ATLASSERT(p && "指针为空");
	if (!p)
	{
		MessageBox(0,_T("发生了严重错误，程序将退出执行。"),_T("警告"),0);
		ExitProcess(8);
	}
	return S_OK;
}

void CGlobalCenter::_InitializeNetCenter()
{
	m_netCenter.Initialize(NULL);
	m_netCenter.Run();
	HRESULT hr = m_netCenter.ConnectServer(_T("127.0.0.1"),2508);
	if (FAILED(hr))
	{
		MessageBox(0,_T("无法连接服务器"),0,0);
	}
}

void CGlobalCenter::_UnInitializeNetCenter()
{
	m_netCenter.Terminate();
	m_netCenter.UnInitialize();
}

HRESULT CGlobalCenter::GetINetCenter( INetCenter** pNetCenter )
{
	*pNetCenter = dynamic_cast<INetCenter*>(&m_netCenter);
	return S_OK;
}

