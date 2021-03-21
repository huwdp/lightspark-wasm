/**************************************************************************
    Lightspark, a free flash player implementation

    Copyright (C) 2012-2013  Alessandro Pignotti (a.pignotti@sssup.it)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

#include "backends/xml_support.h"
#include "logger.h"
#include "scripting/abc.h"
#include "scripting/toplevel/Error.h"
#include "scripting/class.h"

using namespace lightspark;
using namespace std;

const pugi::xml_node XMLBase::buildFromString(const tiny_string& str,
										unsigned int xmlparsemode,
										const tiny_string& default_ns)
{
	tiny_string buf = quirkEncodeNull(removeWhitespace(str));
	if (buf.numBytes() > 0 && buf.charAt(0) == '<')
	{
		pugi::xml_parse_result res = xmldoc.load_buffer((void*)buf.raw_buf(),buf.numBytes(),xmlparsemode);
		switch (res.status)
		{
			case pugi::status_ok:
				break;
			case pugi::status_end_element_mismatch:
				throwError<TypeError>(kXMLUnterminatedElementTag);
				break;
			case pugi::status_unrecognized_tag:
				throwError<TypeError>(kXMLMalformedElement);
				break;
			case pugi::status_bad_pi:
				throwError<TypeError>(kXMLUnterminatedXMLDecl);
				break;
			case pugi::status_bad_attribute:
				throwError<TypeError>(kXMLUnterminatedAttribute);
				break;
			case pugi::status_bad_cdata:
				throwError<TypeError>(kXMLUnterminatedCData);
				break;
			case pugi::status_bad_doctype:
				throwError<TypeError>(kXMLUnterminatedDocTypeDecl);
				break;
			case pugi::status_bad_comment:
				throwError<TypeError>(kXMLUnterminatedComment);
				break;
			default:
				LOG(LOG_ERROR,"xml parser error:"<<buf<<" "<<res.status<<" "<<res.description());
				break;
		}
	}
	else
	{
		pugi::xml_node n = xmldoc.append_child(pugi::node_pcdata);
		n.set_value(str.raw_buf());
	}
	return xmldoc.root();
}
const tiny_string XMLBase::encodeToXML(const tiny_string value, bool bIsAttribute)
{

	tiny_string res;
	auto it = value.begin();
	while (it != value.end())
	{
		switch (*it)
		{
			case '<':
				res += "&lt;";
				break;
			case '>':
				res += bIsAttribute ? ">" : "&gt;";
				break;
			case '&':
				res += "&amp;";
				break;
			case '\"':
				res += bIsAttribute ? "&quot;" : "\"";
				break;
			case '\r':
				res += bIsAttribute ? "&#xD;" : "\r";
				break;
			case '\n':
				res += bIsAttribute ? "&#xA;" : "\n";
				break;
			case '\t':
				res += bIsAttribute ? "&#x9;" : "\t";
				break;
			default:
				res += *it;
				break;
		}
		it++;
	}
	return res;
}

// Adobe player's XML parser accepts many strings which are not valid
// XML according to the specs. This function attempts to massage
// invalid-but-accepted-by-Adobe strings into valid XML so that
// libxml++ parser doesn't throw an error.
string XMLBase::parserQuirks(const string& str)
{
	string buf = quirkEncodeNull(str);
	buf = quirkXMLDeclarationInMiddle(buf);
	return buf;
}

string XMLBase::quirkXMLDeclarationInMiddle(const string& str) {
	string buf(str);

	// Adobe player ignores XML declarations in the middle of a
	// string.
	while (true)
	{
		size_t start = buf.find("<?xml ", 1);
		if (start == buf.npos)
			break;
		
		size_t end = buf.find("?>", start+5);
		if (end == buf.npos)
			break;
		end += 2;
		
		buf.erase(start, end-start);
	}

	return buf;
}
string XMLBase::quirkEncodeNull(const string value)
{
	string res;
	auto it = value.cbegin();
	while (it != value.cend())
	{
		switch (*it)
		{
			case '\0':
				res += "&#x0;";
				break;
			default:
				res += *it;
				break;
		}
		it++;
	}
	return res;
}

tiny_string XMLBase::removeWhitespace(tiny_string val)
{
	bool bwhite = true;
	uint32_t start = 0;
	CharIterator it = val.begin();
	CharIterator itend = val.begin();
	while (it != val.end())
	{
		if (!g_unichar_isspace(*it))
		{
			itend=it;
			itend++;
			bwhite = false;
		}
		else if (bwhite)
			start++;
		it++;
	}
	if (bwhite)
		return "";
	return val.substr(start,itend);
}
