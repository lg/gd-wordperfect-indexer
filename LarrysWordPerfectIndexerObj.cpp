// LarrysWordPerfectIndexerObj.cpp : Implementation of CLarrysWordPerfectIndexerObj

#include <string>
#include <fstream>
#include "stdafx.h"
#include "LarrysWordPerfectIndexerObj.h"
#include ".\larryswordperfectindexerobj.h"
#include "libwpd/libwpd.h"
#include "libwpd/WPXMemoryStream.h"
#include "TextListenerImpl.h"

using namespace std;


// CLarrysWordPerfectIndexerObj

STDMETHODIMP CLarrysWordPerfectIndexerObj::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ILarrysWordPerfectIndexerObj
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CLarrysWordPerfectIndexerObj::HandleFile(BSTR RawFullPath, IDispatch* EventFactory)
{
	string FullPath = CW2A(RawFullPath);
	#if FULLDEBUG
		MessageBox(0, FullPath.c_str(), "Full Debug", 0);
	#endif

	// Open file for filesize
    HANDLE FileHandle = 0;
	FileHandle = CreateFile(FullPath.c_str(), FILE_READ_ATTRIBUTES, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (FileHandle == INVALID_HANDLE_VALUE) {
		return S_FALSE;
	}

	// Get the file size
	DWORD FileSize;
	FileSize = GetFileSize(FileHandle, NULL);
	if (FileSize == 0xFFFFFFFF) {
		#if FULLDEBUG
			MessageBox(0, "Unable to get filesize", "Full Debug", 0);
		#endif
		return S_FALSE;
	}
	
	// Close file after getting file size
	CloseHandle(FileHandle);
    
	// Reopen file for binary read
	ifstream WPDFile(FullPath.c_str(), ios_base::binary);
	if (!WPDFile) {
		#if FULLDEBUG
			MessageBox(0, "Unable to open for binary read", "Full Debug", 0);
		#endif
		return S_FALSE;
	}

	// Allocate memory for entire file
	char *CharData;
	CharData = (char *)malloc(FileSize);
	if (CharData == 0) {
		#if FULLDEBUG
			MessageBox(0, "Could not allocate memory", "Full Debug", 0);
		#endif
		return S_FALSE;
	}

	// Read entire file.
	if (!WPDFile.read(CharData, FileSize)) {
		#if FULLDEBUG
			MessageBox(0, "Could not read file", "Full Debug", 0);
		#endif
		return S_FALSE;
	}
	
	// Close file
	WPDFile.close();

	// Open the file and verify that it's a valid WordPerfect file
	WPXMemoryInputStream InputStream((uint8_t *)CharData, FileSize);
	WPDConfidence confidence = WPDocument::isFileFormatSupported(&InputStream, false);
	if (confidence == WPD_CONFIDENCE_NONE || confidence == WPD_CONFIDENCE_POOR) {
		#if FULLDEBUG
			MessageBox(0, "No confidence!", "Full Debug", 0);
		#endif
		return 0;
	}

	// Create the object that will receive events based on the structure of the WPD
	TextListenerImpl TextListener;
	TextListener.FullWPDPath = FullPath;

	// Parse the file, sending events to the TextListener above
 	WPDResult WPDError = WPDocument::parse(&InputStream, static_cast<WPXHLListenerImpl *>(&TextListener));

	// Free the memory buffer (check if works earlier on)
	free(CharData);

	// Check to see if any errors occured
	if (WPDError == WPD_FILE_ACCESS_ERROR) {
		#if FULLDEBUG
			MessageBox(0, "File access error", "Full Debug", 0);
		#endif
		return S_FALSE;
	} else if (WPDError == WPD_PARSE_ERROR) {
		#if FULLDEBUG
			MessageBox(0, "Parse error", "Full Debug", 0);
		#endif
        return S_FALSE;
	} else if (WPDError == WPD_UNSUPPORTED_ENCRYPTION_ERROR) {
		#if FULLDEBUG
			MessageBox(0, "Unsupported encryption", "Full Debug", 0);
		#endif
		return S_FALSE;
	} else if (WPDError == WPD_OLE_ERROR) {
		#if FULLDEBUG
			MessageBox(0, "OLE error", "Full Debug", 0);
		#endif
		return S_FALSE;
	} else if (WPDError != WPD_OK) {
		#if FULLDEBUG
			MessageBox(0, "Unknown error", "Full Debug", 0);
		#endif
		return S_FALSE;
	}

	return S_OK;
}
