/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 * For further information visit http://libwpd.sourceforge.net
 */

/* "This product is not manufactured, approved, or supported by 
 * Corel Corporation or Corel Corporation Limited."
 */

#ifndef WP6PARSER_H
#define WP6PARSER_H

#include "WPXParser.h"
#include "WP6Header.h"

class WP6PrefixData;
class WP6HLListener;
class WPXHLListenerImpl;

class WP6Parser : public WPXParser
{
public:
	WP6Parser(WPXInputStream *input, WPXHeader *header);
	~WP6Parser();

	virtual void parse(WPXHLListenerImpl *listenerImpl);

	static void parseDocument(WPXInputStream *stream, WP6HLListener *listener);

private:
	WP6PrefixData * getPrefixData(WPXInputStream *input);

	void parse(WPXInputStream *stream, WP6HLListener *listener);

	void parsePacket(WP6PrefixData *prefixData, int type, WP6HLListener *listener);
	void parsePackets(WP6PrefixData *prefixData, int type, WP6HLListener *listener);
};

#endif /* WP6PARSER_H */
