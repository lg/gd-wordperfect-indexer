#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <ctime>
#include <windows.h>
#include <tlhelp32.h>
#include <atlstr.h>
#include <atlsafe.h>
#include "GoogleDesktopSearchAPI.h"

using namespace std;

class LarGDSPlugin {

private:
	struct ConversationStruct {
		time_t LastMessageTime;
		ULONG ConversationID;
	};
 
	CLSID ObjClassID;
	map <string, ConversationStruct> Conversations;

public:
	LarGDSPlugin(CLSID ClassID);
	bool RegisterPlugin(string Title, string Description, string IconPath);
	bool RegisterPluginWithExtensions(string Title, string Description, string IconPath, vector<string> Extensions);
	bool UnregisterPlugin();
	bool SendIMEvent(string Content, string UserName, string BuddyName, string Title, unsigned long ConversationID);
	bool SendIMEvent(string Content, string UserName, string BuddyName, string Title, unsigned long ConversationID, SYSTEMTIME MessageTime, string Format);
	bool SendTextFileEvent(string Content, string Path, SYSTEMTIME LastModified);
	bool SendTextFileEvent(string Content, string Path, SYSTEMTIME LastModified, string ContentType);
	bool SendEmailEvent(string Header, string Content, string ContentType);
	unsigned long GetConversationID(string Identifier);
	unsigned long GetConversationID(string Identifier, int Timeout);
	bool IsGDSRunning();
	bool IsPluginRegistered();

	string GetFieldFromHeader(string Header, string FieldName);
	SYSTEMTIME HTTPTimeToSystemTime(string FullDate);

};