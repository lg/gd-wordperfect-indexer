#include "TextListenerImpl.h"
#include "stdafx.h"

// use the BELL code to represent a TAB for now
#define UCS_TAB 0x0009 

TextListenerImpl::TextListenerImpl() {
	CurWPDText = "";
}

TextListenerImpl::~TextListenerImpl() {
	// Once the WPD parser is completed going through the WPD, it will end here.

	// Get the date modified of the file
	HANDLE FileHandle;
	FileHandle = CreateFile(FullWPDPath.c_str(), FILE_READ_ATTRIBUTES, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);

	FILETIME *ModifiedTime = (FILETIME *)malloc(sizeof FILETIME);
	BOOL FileTimeResult = GetFileTime(FileHandle, 0, 0, ModifiedTime);
	CloseHandle(FileHandle);
	if (!FileTimeResult) {
		#if FULLDEBUG
			MessageBox(0, "Unable to get date and time", "Full Debug", 0);
		#endif
        return; 
	}

	// Format modification date properly
	SYSTEMTIME SystemTime;
	FileTimeToSystemTime(ModifiedTime, &SystemTime);

	// A file event is sent to GDS with the data collected.
	LarGDSPlugin GDSPlugin(PluginClassID);
	GDSPlugin.SendTextFileEvent(CurWPDText, FullWPDPath, SystemTime);

	#if FULLDEBUG
		MessageBox(0, "Success", "Full Debug", 0);
	#endif
}

void TextListenerImpl::closeParagraph() {
	CurWPDText.append("\n");
}

void TextListenerImpl::insertTab() {
	CurWPDText.append("\t");
}

void TextListenerImpl::insertText(const WPXString &text) {
	CurWPDText.append(text.cstr());
}

void TextListenerImpl::insertLineBreak() {
	CurWPDText.append("\n");
}