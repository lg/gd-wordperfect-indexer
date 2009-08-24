#ifndef TEXTLISTENERIMPL_H
#define TEXTLISTENERIMPL_H

#include "libwpd/WPXHLListenerImpl.h"
#include "libwpd/WPXHLListener.h"

#include <string>
#include <stdio.h>

using namespace std;

class TextListenerImpl : public WPXHLListenerImpl
{
public:
	TextListenerImpl();
	virtual ~TextListenerImpl();

 	virtual void setDocumentMetaData(const WPXPropertyList &propList) {}

	virtual void startDocument() {}
	virtual void endDocument() {}

	virtual void openPageSpan(const WPXPropertyList &propList) {}
	virtual void closePageSpan() {}
	virtual void openHeader(const WPXPropertyList &propList) {}
	virtual void closeHeader() {}
	virtual void openFooter(const WPXPropertyList &propList) {}
	virtual void closeFooter() {}

	virtual void openSection(const WPXPropertyList &propList, const WPXPropertyListVector &columns) {}
	virtual void closeSection() {}
	virtual void openParagraph(const WPXPropertyList &propList, const WPXPropertyListVector &tabStops) {}
	virtual void closeParagraph();
	virtual void openSpan(const WPXPropertyList &propList) {}
	virtual void closeSpan() {}

	virtual void insertTab();
	virtual void insertText(const WPXString &text);
	virtual void insertLineBreak();

	virtual void defineOrderedListLevel(const WPXPropertyList &propList) {}
	virtual void defineUnorderedListLevel(const WPXPropertyList &propList) {}
	virtual void openOrderedListLevel(const WPXPropertyList &propList) {}
	virtual void openUnorderedListLevel(const WPXPropertyList &propList) {}
	virtual void closeOrderedListLevel() {}
	virtual void closeUnorderedListLevel() {}
	virtual void openListElement(const WPXPropertyList &propList, const WPXPropertyListVector &tabStops) {}
	virtual void closeListElement() {}

	virtual void openFootnote(const WPXPropertyList &propList) {}
	virtual void closeFootnote() {}
	virtual void openEndnote(const WPXPropertyList &propList) {}
	virtual void closeEndnote() {}

	virtual void openTable(const WPXPropertyList &propList, const WPXPropertyListVector &columns) {}
	virtual void openTableRow(const WPXPropertyList &propList) {}
	virtual void closeTableRow() {}
	virtual void openTableCell(const WPXPropertyList &propList) {}
	virtual void closeTableCell() {}
	virtual void insertCoveredTableCell(const WPXPropertyList &propList) {}
	virtual void closeTable() {}

	string FullWPDPath;

private:
	unsigned int m_currentListLevel;
	string CurWPDText;
};

#endif /* TEXTLISTENERIMPL_H */