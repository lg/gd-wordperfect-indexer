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

#ifndef WP6CHARACTERGROUP_H
#define WP6CHARACTERGROUP_H

#include "WP6VariableLengthGroup.h"

class WP6CharacterGroup_SetAlignmentCharacterSubGroup: public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_SetAlignmentCharacterSubGroup(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	uint8_t m_character;
	uint8_t m_characterSet;
};

class WP6CharacterGroup_ColorSubGroup: public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_ColorSubGroup(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	uint8_t m_red;
	uint8_t m_green;
	uint8_t m_blue;
};

class WP6CharacterGroup_CharacterShadingChangeSubGroup: public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_CharacterShadingChangeSubGroup(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	uint8_t m_shading;
};

class WP6CharacterGroup_FontFaceChangeSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_FontFaceChangeSubGroup(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	uint16_t m_oldMatchedPointSize;
	uint16_t m_hash;
	uint16_t m_matchedFontIndex;
	uint16_t m_matchedFontPointSize;
};

class WP6CharacterGroup_SetDotLeaderCharactersSubGroup: public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_SetDotLeaderCharactersSubGroup(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	uint8_t m_character;
	uint8_t m_characterSet;
	uint8_t m_numberOfSpaces;
};

class WP6CharacterGroup_ParagraphNumberOnSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_ParagraphNumberOnSubGroup(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	uint16_t m_outlineHash;
	uint8_t m_level;
	uint8_t m_flag;
};

class WP6CharacterGroup_TableDefinitionOnSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_TableDefinitionOnSubGroup(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	uint8_t m_flags;
	uint8_t m_position;
	uint16_t m_leftOffset;
};

class WP6CharacterGroup_TableDefinitionOffSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_TableDefinitionOffSubGroup(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:

};

class WP6CharacterGroup_TableColumnSubGroup : public WP6VariableLengthGroup_SubGroup
{
public:
	WP6CharacterGroup_TableColumnSubGroup(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener, const uint8_t numPrefixIDs, uint16_t const *prefixIDs) const;

private:
	uint8_t m_flags;
	uint16_t m_width;
	uint16_t m_leftGutter;
	uint16_t m_rigthGutter;
	uint16_t m_attribWord1;
	uint16_t m_attribWord2;
	uint8_t m_alignment;
	uint16_t m_absPosFromRight;
	uint16_t m_numberType;
	uint8_t m_currencyIndex;
};

class WP6CharacterGroup : public WP6VariableLengthGroup
{
 public:
	WP6CharacterGroup(WPXInputStream *input);	
	virtual ~WP6CharacterGroup();
	virtual void _readContents(WPXInputStream *input);
	virtual void parse(WP6HLListener *listener);

 private:
	WP6VariableLengthGroup_SubGroup *m_subGroupData;

};

#endif /* WP6CHARACTERGROUP_H */
