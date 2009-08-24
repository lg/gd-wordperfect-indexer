// LarGDSPlugin 1.01 by Larry Gadea
// For the latest version of LarGDSPlugin, please visit: http://www.trivex.net

#include "LarGDSPlugin.h"

using namespace std;

LarGDSPlugin::LarGDSPlugin(CLSID ClassID) {
	ObjClassID = ClassID;
}
bool LarGDSPlugin::RegisterPlugin(string Title, string Description, string IconPath) {
	return RegisterPluginWithExtensions(Title, Description, IconPath, vector <string>());
}

bool LarGDSPlugin::RegisterPluginWithExtensions(string Title, string Description, string IconPath, vector<string> Extensions) {
	// Initialize COM (if required)
    HRESULT ComInitResult = CoInitialize(NULL);
	if (FAILED(ComInitResult) && ComInitResult != RPC_E_CHANGED_MODE) {
		return false;
	}

	// Open a new scope for COM actions. The idea behind this is so at the end,
	// all variables get freed and making CoUninitialize() safely close COM.
	HRESULT hr;

	{
		// Create connection with GDS
		CComPtr<IGoogleDesktopSearchComponentRegistration> spRegistration;
		CComPtr<IGoogleDesktopSearchComponentRegister> spRegister;
		
		hr = spRegister.CoCreateInstance(CLSID_GoogleDesktopSearchRegister);
  
		if (SUCCEEDED(hr)) {
			ATLASSERT(spRegister != NULL);
    
			// Component description is 6 strings
			CComSafeArray<VARIANT> arr_descr(6);

			arr_descr.SetAt(0, CComVariant(L"Title"));
			arr_descr.SetAt(1, CComVariant(Title.c_str()));
			arr_descr.SetAt(2, CComVariant(L"Description"));
			arr_descr.SetAt(3, CComVariant(Description.c_str()));
			arr_descr.SetAt(4, CComVariant(L"Icon"));
			arr_descr.SetAt(5, CComVariant(IconPath.c_str()));

			// our CLSID in string format
			CComBSTR clsid(ObjClassID);
	    
			// Wrap description array in variant
			CComVariant descr(arr_descr.m_psa);
			
			// and register
			hr = spRegister->RegisterComponent(clsid, descr, &spRegistration);
			if (SUCCEEDED(hr)) {
				ATLASSERT(FAILED(hr) || spRegistration);

				for (int CurExtension = 0; (SUCCEEDED(hr) && (CurExtension <= (int)(Extensions.size() - 1))); CurExtension++) {
					hr = spRegistration->RegisterExtension(CComBSTR(Extensions[CurExtension].c_str()));
				}

				// revoke our registration in case any of our extensions fail to register
				if (FAILED(hr)) {
					// ignore the error - we've already failed
					HRESULT ignore;
					ignore = spRegister->UnregisterComponent(CComBSTR(clsid));

					// Uninitialize COM (if required)
					if (SUCCEEDED(ComInitResult)) {
						CoUninitialize();
					}

					return false;
				}		
			}
		}
	}

	// Uninitialize COM (if required)
	if (SUCCEEDED(ComInitResult)) {
		CoUninitialize();
	}

	if (FAILED(hr)) {
		return false;
	}

	return true;
}

bool LarGDSPlugin::UnregisterPlugin() {
	// Initialize COM (if required)
	HRESULT ComInitResult = CoInitialize(NULL);
	if (FAILED(ComInitResult) && ComInitResult != RPC_E_CHANGED_MODE) {
		return false;
	}

	// Open a new scope so COM variables get freed before CoUninitialize().
	{
		// Connect to GDS
		CComPtr<IGoogleDesktopSearchComponentRegister> spRegister;
		HRESULT hr;
		hr = spRegister.CoCreateInstance(CLSID_GoogleDesktopSearchRegister);
  
		if (SUCCEEDED(hr)) { 
			// our CLSID in string format
			CComBSTR bstrClsid(ObjClassID);
  
			hr = spRegister->UnregisterComponent(bstrClsid);
		}
	}

    // Uninitialize COM (if required)
	if (SUCCEEDED(ComInitResult)) {
		CoUninitialize();	//Is causing problems for now...
	}

	return true;
}

bool LarGDSPlugin::SendIMEvent(string Content, string UserName, string BuddyName, string Title, unsigned long ConversationID) {
	SYSTEMTIME TimeNow;
	GetSystemTime(&TimeNow);
	
	return SendIMEvent(Content, UserName, BuddyName, Title, ConversationID, TimeNow, "");
}

bool LarGDSPlugin::SendIMEvent(string Content, string UserName, string BuddyName, string Title, unsigned long ConversationID, SYSTEMTIME MessageTime, string Format) {
	// Convert MessageTime
	double VarMessageTime;
	SystemTimeToVariantTime(&MessageTime, &VarMessageTime);
	
	// If no format was specified, assume it's text/html
	if (Format == "") {
		Format = "text/html";
	}

	// Init COM
	HRESULT hr_coinit = CoInitialize(NULL);
	if (FAILED(hr_coinit) && hr_coinit != RPC_E_CHANGED_MODE) {
		return false;
	}

	// Assemble event
	{
		CComPtr<IGoogleDesktopSearchEventFactory> spFactory;
		HRESULT hr = spFactory.CoCreateInstance(CLSID_GoogleDesktopSearch, NULL, CLSCTX_INPROC);
		if (FAILED(hr)) {
			return false;
		}

		CComPtr<IDispatch> spEventDisp;
		hr = spFactory->CreateEvent(CComBSTR(ObjClassID), CComBSTR(L"Google.Desktop.IM"), &spEventDisp);
		if (FAILED(hr)) {
			return false;
		}

		CComQIPtr<IGoogleDesktopSearchEvent> spEvent(spEventDisp);
		ATLASSERT(spEventDisp && spEvent);
		if (spEvent == NULL) {
			return false;
		}

		if (SUCCEEDED(hr)) {
			hr = spEvent->AddProperty(CComBSTR(L"message_time"), CComVariant(VarMessageTime, VT_DATE));
		}

		if (SUCCEEDED(hr)) {
			hr = spEvent->AddProperty(CComBSTR(L"format"), CComVariant(Format.c_str()));
		}

		if (SUCCEEDED(hr)) {
			hr = spEvent->AddProperty(CComBSTR(L"content"), CComVariant(Content.c_str()));
		}

		if (SUCCEEDED(hr)) {
			hr = spEvent->AddProperty(CComBSTR(L"buddy_name"), CComVariant(BuddyName.c_str()));
		}

		if (SUCCEEDED(hr)) {
			hr = spEvent->AddProperty(CComBSTR(L"title"), CComVariant(Title.c_str()));
		}

		if (SUCCEEDED(hr)) {
		    hr = spEvent->AddProperty(CComBSTR(L"conversation_id"), CComVariant(ConversationID));
		}

		if (SUCCEEDED(hr)) {
			hr = spEvent->AddProperty(CComBSTR(L"user_name"), CComVariant(UserName.c_str())); 
		}

		if (FAILED(hr)) {
			return false;
		}

		hr = spEvent->Send(EventFlagIndexable);
		if (FAILED(hr)) {
			return false;
		}
	}

	if (SUCCEEDED(hr_coinit)) {
		CoUninitialize();
	}

	return true;
}


bool LarGDSPlugin::SendTextFileEvent(string Content, string Path, SYSTEMTIME LastModified) {
	return SendTextFileEvent(Content, Path, LastModified, "text/plain");
}

bool LarGDSPlugin::SendTextFileEvent(string Content, string Path, SYSTEMTIME LastModified, string ContentType) {
	
	// Convert date to proper format
	double ModifiedTime;
	SystemTimeToVariantTime(&LastModified, &ModifiedTime);

	// Init COM
	HRESULT hr_coinit = CoInitialize(NULL);
	if (FAILED(hr_coinit) && hr_coinit != RPC_E_CHANGED_MODE) {
		return false;
	}

	// Assemble event
	{
		CComPtr<IGoogleDesktopSearchEventFactory> spFactory;
		HRESULT hr = spFactory.CoCreateInstance(CLSID_GoogleDesktopSearch, NULL, CLSCTX_INPROC);
		if (FAILED(hr)) {
			return false;
		}

		CComPtr<IDispatch> spEventDisp;
		hr = spFactory->CreateEvent(CComBSTR(ObjClassID), CComBSTR(L"Google.Desktop.File"), &spEventDisp);
		if (FAILED(hr)) {
			return false;
		}

		CComQIPtr<IGoogleDesktopSearchEvent> spEvent(spEventDisp);
		ATLASSERT(spEventDisp && spEvent);
		if (spEvent == NULL) {
			return false;
		}

		if (SUCCEEDED(hr)) {
			hr = spEvent->AddProperty(CComBSTR(L"format"), CComVariant(ContentType.c_str()));
		}

		if (SUCCEEDED(hr)) {
			hr = spEvent->AddProperty(CComBSTR(L"content"), CComVariant(Content.c_str()));
		}

		if (SUCCEEDED(hr)) {
			hr = spEvent->AddProperty(CComBSTR(L"uri"), CComVariant(Path.c_str()));
		}

		if (SUCCEEDED(hr)) {
			hr = spEvent->AddProperty(CComBSTR(L"last_modified_time"), CComVariant(ModifiedTime, VT_DATE));
		}

		if (FAILED(hr)) {
			return false;
		}

		hr = spEvent->Send(EventFlagIndexable);
		if (FAILED(hr)) {
			return false;
		}
	}

	// Uninit COM
	if (SUCCEEDED(hr_coinit)) {
		CoUninitialize();
	}

	return true;
}

bool LarGDSPlugin::SendEmailEvent(string Header, string Content, string ContentType) {
	// Init COM
	HRESULT hr_coinit = CoInitialize(NULL);
	if (FAILED(hr_coinit) && hr_coinit != RPC_E_CHANGED_MODE) {
		return false;
	}

	// Convert date
	SYSTEMTIME MessageDate;
	string DateString;
	DateString = GetFieldFromHeader(Header, "Date");
	if (DateString == "") {
		// If there was no date string in the header, assume today.
		GetSystemTime(&MessageDate);

		// Currently, if no date was found, __skip__ the message.
		if (SUCCEEDED(hr_coinit)) {
			CoUninitialize();
		}

		return true;
	} else {
		MessageDate = HTTPTimeToSystemTime(DateString);
		if (MessageDate.wMilliseconds == 1) {
			GetSystemTime(&MessageDate);
		}
	}
	
	double VarMessageTime;
	SystemTimeToVariantTime(&MessageDate, &VarMessageTime);

	// Assemble event
	HRESULT hr = S_OK;
	{
		CComPtr<IGoogleDesktopSearchEventFactory> spFactory;
		hr = spFactory.CoCreateInstance(CLSID_GoogleDesktopSearch, NULL, CLSCTX_INPROC);

		CComPtr<IDispatch> spEventDisp;

		if (SUCCEEDED(hr)) {
			hr = spFactory->CreateEvent(CComBSTR(ObjClassID), CComBSTR(L"Google.Desktop.Email"), &spEventDisp);
		}

		CComQIPtr<IGoogleDesktopSearchEvent> spEvent(spEventDisp);
				
		if ((spEvent != NULL) && SUCCEEDED(hr)) {
			if (SUCCEEDED(hr)) {
				hr = spEvent->AddProperty(CComBSTR(L"format"), CComVariant(ContentType.c_str()));
			}

			if (SUCCEEDED(hr)) {
				hr = spEvent->AddProperty(CComBSTR(L"content"), CComVariant(Content.c_str()));
			}

			if (SUCCEEDED(hr)) {
				hr = spEvent->AddProperty(CComBSTR(L"mail_header"), CComVariant(Header.c_str()));
			}

			if (SUCCEEDED(hr)) {
				hr = spEvent->AddProperty(CComBSTR(L"received"), CComVariant(VarMessageTime, VT_DATE));
			}
			
			if (SUCCEEDED(hr)) {
				hr = spEvent->Send(EventFlagIndexable);
			}
		}	
	}

	// Uninit COM
	if (SUCCEEDED(hr_coinit)) {
		CoUninitialize();
	}

	if (FAILED(hr)) {
		return false; 
	}

	return true;
}

unsigned long LarGDSPlugin::GetConversationID(string Identifier) {
	// Default to a timeout of 15 minutes.
	return GetConversationID(Identifier, 900); 
}
unsigned long LarGDSPlugin::GetConversationID(string Identifier, int Timeout) {
	bool CreateNewConversationID = false;

	// Check if there are any conversations with OtherPartyName in memory
	if (Conversations.find(Identifier) == Conversations.end()) {
		// If no, flag for new ConversationID creation

		CreateNewConversationID = true;
	}

	if (CreateNewConversationID == false) {
		// Check to see if nothing has been said within the last 15 minutes

		if (difftime(time(0), Conversations[Identifier].LastMessageTime) > Timeout) {
			// If it has been longer than 15 minutes, flag for new ConversationID creation

			CreateNewConversationID = true;
		}
	}

	if (CreateNewConversationID == true) {
		// If flagged to create a new ConversationID, randomize it.

        srand(static_cast<unsigned>(time(0)));
		ConversationStruct Conversation;
		Conversation.ConversationID = (unsigned long) rand();
		Conversations[Identifier] = Conversation;
	}

	Conversations[Identifier].LastMessageTime = time(0);
	
	return Conversations[Identifier].ConversationID;
}

string LarGDSPlugin::GetFieldFromHeader(string Header, string FieldName) {
	size_t FieldLoc, ValueStart, ValueEnd;

	// Find the field in the header
	FieldLoc = Header.find("\r\n" + FieldName + ": ");
	if (FieldLoc == Header.npos) {
		return "";
	}
	FieldLoc += 2;

	// Check that header actually contains text if it's at the end of the header
	ValueStart = FieldLoc + FieldName.length() + 2;
	if (ValueStart > Header.length() - 1) {
		return "";
	}

	// Find the end of the line
	ValueEnd = Header.find("\r\n", FieldLoc);
	if (ValueEnd == Header.npos) {
		ValueEnd = Header.length() - 1;
	}

	// Check if any data actually exists
	if (ValueStart >= ValueEnd) {
		return "";
	}
    
	// Return field value
	return Header.substr(ValueStart, ValueEnd - ValueStart);
}

SYSTEMTIME LarGDSPlugin::HTTPTimeToSystemTime(string FullDate) {
	vector<string> DateSegments;
	string SplitDate = FullDate;

	// Some SMTP servers are retarded and don't add a day of the week as theyre required to,
	// we therefore need to manually set a fake one.
	if (SplitDate.find(",") == SplitDate.npos) {
		SplitDate = "Sun, " + SplitDate;
	}

	// Some other SMTP servers also dont put a timezone, fortunetly it doesnt matter, but this function
	// still likes one.
	if (SplitDate.find("(") == SplitDate.npos) {
		SplitDate += " (PST)";
	}

	stringstream FullDateSS(SplitDate);

	// Split the main string by space
	string CurSegment;
	while (FullDateSS >> CurSegment) {
		DateSegments.push_back(CurSegment);
	}

	// Verify date
	if (DateSegments.size() < 6) {
		SYSTEMTIME FailedReturn;
		FailedReturn.wMilliseconds = 1;
		return FailedReturn;
	}

	// Split segments into seperate strings
	string DayStr, DayOfWeekStr, MonthStr, YearStr;
	string FullTimeStr, HourStr, MinuteStr, SecondStr;
	string TimezoneStr;

	DayOfWeekStr = DateSegments[0];
	DayStr = DateSegments[1];
	MonthStr = DateSegments[2];
	YearStr = DateSegments[3];
	FullTimeStr = DateSegments[4];
	TimezoneStr = DateSegments[5];

	// Verify time
	if (FullTimeStr.length() != 8) {
		SYSTEMTIME FailedReturn;
		FailedReturn.wMilliseconds = 1;
		return FailedReturn;
	}

	// Split the time up into segments
	HourStr = FullTimeStr.substr(0, 2);
	MinuteStr = FullTimeStr.substr(3, 2);
	SecondStr = FullTimeStr.substr(6, 2);

	// Convert all applicable strings into ints.
	WORD Day, DayOfWeek, Month, Year;
	WORD Hour, Minute, Second;

	stringstream ConvDay; ConvDay << DayStr; ConvDay >> Day;
	stringstream ConvYear; ConvYear << YearStr; ConvYear >> Year;
	stringstream ConvHour; ConvHour << HourStr; ConvHour >> Hour;
	stringstream ConvMinute; ConvMinute << MinuteStr; ConvMinute >> Minute;
	stringstream ConvSecond; ConvSecond << SecondStr; ConvSecond >> Second;

	// Convert weekday from string to integer
	if (DayOfWeekStr == "Sun,") { DayOfWeek = 0; }
	else if (DayOfWeekStr == "Mon,") { DayOfWeek = 1; }
	else if (DayOfWeekStr == "Tue,") { DayOfWeek = 2; }
	else if (DayOfWeekStr == "Wed,") { DayOfWeek = 3; }
	else if (DayOfWeekStr == "Thu,") { DayOfWeek = 4; }
	else if (DayOfWeekStr == "Fri,") { DayOfWeek = 5; }
	else { DayOfWeek = 6; }

	// Convert month from string to integer
	if (MonthStr == "Jan") { Month = 1; }
	else if (MonthStr == "Feb") { Month = 2; }
	else if (MonthStr == "Mar") { Month = 3; }
	else if (MonthStr == "Apr") { Month = 4; }
	else if (MonthStr == "May") { Month = 5; }
	else if (MonthStr == "Jun") { Month = 6; }
	else if (MonthStr == "Jul") { Month = 7; }
	else if (MonthStr == "Aug") { Month = 8; }
	else if (MonthStr == "Sep") { Month = 9; }
	else if (MonthStr == "Oct") { Month = 10; }
	else if (MonthStr == "Nov") { Month = 11; }
	else { Month = 12; }

	// Assign date to a SYSTEMTIME structure
	SYSTEMTIME SystemTime;
	SystemTime.wYear = Year;
	SystemTime.wMonth = Month;
	SystemTime.wDayOfWeek = DayOfWeek;
	SystemTime.wDay = Day;
	SystemTime.wHour = Hour;
	SystemTime.wMinute = Minute;
	SystemTime.wSecond = Second;
	SystemTime.wMilliseconds = 0;

	return SystemTime;
}
bool LarGDSPlugin::IsGDSRunning() {
	bool FoundGDS = false;

	// Create a handle to all processes
	HANDLE SnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	// Check that a valid handle was received
	if (SnapshotHandle == INVALID_HANDLE_VALUE) {
		return false;
	}

	// Prepare process structure
	PROCESSENTRY32 ProcessEntry;
	
	// Loop through all processes
	ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
	BOOL ProcessReturn = Process32First(SnapshotHandle, &ProcessEntry);
	while (ProcessReturn) {
		string ProcessName(ProcessEntry.szExeFile);

		if (ProcessName.find("GoogleDesktopIndex.exe") != ProcessName.npos) {
			FoundGDS = true;
		}

		ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
		ProcessReturn = Process32Next(SnapshotHandle, &ProcessEntry);
}
	

	// Close the process list handle
	CloseHandle(SnapshotHandle);

	return FoundGDS;
}

bool LarGDSPlugin::IsPluginRegistered() {
	// Init COM
	HRESULT hr_coinit = CoInitialize(NULL);
	if (FAILED(hr_coinit) && hr_coinit != RPC_E_CHANGED_MODE) {
		return false;
	}

	// Assemble event to test and see if GDS accepts it
	HRESULT hr = S_OK;
	{
		CComPtr<IGoogleDesktopSearchEventFactory> spFactory;
		hr = spFactory.CoCreateInstance(CLSID_GoogleDesktopSearch, NULL, CLSCTX_INPROC);

		CComPtr<IDispatch> spEventDisp;

		if (SUCCEEDED(hr)) {
			hr = spFactory->CreateEvent(CComBSTR(ObjClassID), CComBSTR(L"Google.Desktop.Email"), &spEventDisp);
		}
	}

	// Uninit COM
	if (SUCCEEDED(hr_coinit)) {
		CoUninitialize();
	}

	if (SUCCEEDED(hr)) {
		return true;
	}
	
	return false;
}