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

#ifndef WP3ATTRIBUTEGROUP_H
#define WP3ATTRIBUTEGROUP_H

#include "WP3FixedLengthGroup.h"

class WP3AttributeGroup : public WP3FixedLengthGroup
{
 public:
	WP3AttributeGroup(WPXInputStream *input, uint8_t groupID);	
	virtual void parse(WP3HLListener *listener);
	
 protected:
	virtual void _readContents(WPXInputStream *input);

 private:
	uint8_t m_attribute;
	uint8_t m_state;
};

#endif /* WP3ATTRIBUTEGROUP_H */
