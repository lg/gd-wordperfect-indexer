// LarrysWordPerfectIndexerObj.h : Declaration of the CLarrysWordPerfectIndexerObj

#pragma once
#include "resource.h"       // main symbols

#include "LarrysWordPerfectIndexer.h"


// CLarrysWordPerfectIndexerObj

class ATL_NO_VTABLE CLarrysWordPerfectIndexerObj : 
	public CComObjectRootEx<CComMultiThreadModel>,
	public CComCoClass<CLarrysWordPerfectIndexerObj, &CLSID_LarrysWordPerfectIndexerObj>,
	public ISupportErrorInfo,
	public IDispatchImpl<ILarrysWordPerfectIndexerObj, &IID_ILarrysWordPerfectIndexerObj, &LIBID_LarrysWordPerfectIndexerLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CLarrysWordPerfectIndexerObj()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LARRYSWORDPERFECTINDEXEROBJ)


BEGIN_COM_MAP(CLarrysWordPerfectIndexerObj)
	COM_INTERFACE_ENTRY(ILarrysWordPerfectIndexerObj)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:

	STDMETHOD(HandleFile)(BSTR RawFilename, IDispatch* EventFactory);
};

OBJECT_ENTRY_AUTO(__uuidof(LarrysWordPerfectIndexerObj), CLarrysWordPerfectIndexerObj)
