/* libwpd
 * Copyright (C) 2002 William Lachance (william.lachance@sympatico.ca)
 * Copyright (C) 2002 Marc Maurer (j.m.maurer@student.utwente.nl)
 *  
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
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

#ifndef WP5PAGEFORMATGROUP_H
#define WP5PAGEFORMATGROUP_H

#include "WP5VariableLengthGroup.h"

class WP5PageFormatGroup : public WP5VariableLengthGroup
{
 public:
	WP5PageFormatGroup(WPXInputStream *input);	
	virtual ~WP5PageFormatGroup();
	virtual void _readContents(WPXInputStream *input);
	virtual void parse(WP5HLListener *listener);

 private:
	// variables needed for subgroup 1 (Left/Right Margin Set)
	uint16_t m_leftMargin;
	uint16_t m_rightMargin;
	// variable needed for subgroup 2 (Spacing Set)
	float m_lineSpacing;
	// variables needed for subgroup 5 (Top/Bottom Margin Set)
	uint16_t m_topMargin;
	uint16_t m_bottomMargin;
	// variables needed for subgroup 6 (Justification)
	uint8_t m_justification;
	// variables needed for subgroup 11 (Form)
	uint16_t m_formLength;
	uint16_t m_formWidth;
	WPXFormOrientation m_formOrientation;

};

#endif /* WP5PAGEFORMATGROUP_H */
