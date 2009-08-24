// LarrysWordPerfectIndexer.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"
#include "LarrysWordPerfectIndexer.h"
#include <string>
#include <vector>
using namespace std;

class CLarrysWordPerfectIndexerModule : public CAtlDllModuleT< CLarrysWordPerfectIndexerModule >
{
public :
	DECLARE_LIBID(LIBID_LarrysWordPerfectIndexerLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_LARRYSWORDPERFECTINDEXER, "{6E87565C-AA6B-4D59-9955-314351AB70BE}")
};

CLarrysWordPerfectIndexerModule _AtlModule;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}


// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}

STDAPI DllRegisterServer(void)
{
	// registers object, typelib and all interfaces in typelib
    HRESULT hr = _AtlModule.DllRegisterServer();
	if (FAILED(hr)) { 
		return hr;
	}

	// Set the extensions that will be monitored
	vector<string> Extensions;
	Extensions.push_back("wpd");

	// Register the plugin with GDS
	LarGDSPlugin GDSPlugin(PluginClassID);
	bool RegisterResult = GDSPlugin.RegisterPluginWithExtensions("Larry's WordPerfect Indexer 1.00 by Larry Gadea", "Search all WordPerfect documents.", "", Extensions);

	if (!RegisterResult) {
		GDSPlugin.UnregisterPlugin();
		_AtlModule.DllUnregisterServer();

		return false;
	}

	return true;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();

	// Unregister plugin from GDS 
	LarGDSPlugin GDSPlugin(PluginClassID);
	GDSPlugin.UnregisterPlugin();

	return hr;
}
