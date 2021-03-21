/**************************************************************************
    Lightspark, a free flash player implementation

    Copyright (C) 2009-2013  Alessandro Pignotti (a.pignotti@sssup.it)

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

#include "scripting/flash/globalization/datetimeformatter.h"
#include "backends/locale.h"
#include "scripting/class.h"
#include "scripting/argconv.h"
#include "scripting/toplevel/Date.h"
#include <iomanip>

using namespace lightspark;

DateTimeFormatter::DateTimeFormatter(Class_base* c):
	ASObject(c)
{
}

void DateTimeFormatter::sinit(Class_base* c)
{
	CLASS_SETUP(c, ASObject, _constructor, CLASS_SEALED|CLASS_FINAL);
	REGISTER_GETTER(c, actualLocaleIDName);
	REGISTER_GETTER(c, lastOperationStatus);
	REGISTER_GETTER(c, requestedLocaleIDName);
	c->setDeclaredMethodByQName("setDateTimePattern","",Class<IFunction>::getFunction(c->getSystemState(),setDateTimePattern),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("format","",Class<IFunction>::getFunction(c->getSystemState(),format),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("formatUTC","",Class<IFunction>::getFunction(c->getSystemState(),formatUTC),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getAvailableLocaleIDNames","",Class<IFunction>::getFunction(c->getSystemState(),formatUTC),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getDateStyle","",Class<IFunction>::getFunction(c->getSystemState(),getDateStyle),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getDateTimePattern","",Class<IFunction>::getFunction(c->getSystemState(),getDateTimePattern),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getFirstWeekday","",Class<IFunction>::getFunction(c->getSystemState(),getFirstWeekday),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getMonthNames","",Class<IFunction>::getFunction(c->getSystemState(),getMonthNames),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getTimeStyle","",Class<IFunction>::getFunction(c->getSystemState(),getTimeStyle),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("getWeekdayNames","",Class<IFunction>::getFunction(c->getSystemState(),getWeekdayNames),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("setDateTimeStyles","",Class<IFunction>::getFunction(c->getSystemState(),setDateTimeStyles),NORMAL_METHOD,true);
}

ASFUNCTIONBODY_ATOM(DateTimeFormatter,_constructor)
{
	DateTimeFormatter* th =asAtomHandler::as<DateTimeFormatter>(obj);
	tiny_string dateStyle, timeStyle;
	ARG_UNPACK_ATOM(th->requestedLocaleIDName)(dateStyle,"long")(timeStyle,"long");

	if (dateStyle == "long" || dateStyle == "medium" ||
		dateStyle == "short" || dateStyle == "none")
	{
		th->dateStyle = dateStyle;
	}
	else
	{
		throw Class<ArgumentError>::getInstanceS(sys,"dateStyle value is not valid", kInvalidArgumentError);
	}
	if (timeStyle == "long" || timeStyle == "medium" ||
		timeStyle == "short" || timeStyle == "none")
	{
		th->timeStyle = timeStyle;
	}
	else
	{
		throw Class<ArgumentError>::getInstanceS(sys,"timeStyle value is not valid", kInvalidArgumentError);
	}

	if (sys->localeManager->isLocaleAvailableOnSystem(th->requestedLocaleIDName))
	{
		std::string localeName = sys->localeManager->getSystemLocaleName(th->requestedLocaleIDName);
		th->currlocale = std::locale(localeName);
		th->actualLocaleIDName = th->requestedLocaleIDName;
		th->lastOperationStatus="noError";
	}
	else
	{
		LOG(LOG_INFO,"unknown locale:"<<th->requestedLocaleIDName);
		th->lastOperationStatus="usingDefaultWarning";
	}
}

ASFUNCTIONBODY_GETTER(DateTimeFormatter, actualLocaleIDName);
ASFUNCTIONBODY_GETTER(DateTimeFormatter, lastOperationStatus);
ASFUNCTIONBODY_GETTER(DateTimeFormatter, requestedLocaleIDName);

ASFUNCTIONBODY_ATOM(DateTimeFormatter,setDateTimePattern)
{
	DateTimeFormatter* th =asAtomHandler::as<DateTimeFormatter>(obj);
	ARG_UNPACK_ATOM(th->pattern);
	th->dateStyle = "custom";
	th->timeStyle = "custom";
	th->lastOperationStatus="noError";
}

ASFUNCTIONBODY_ATOM(DateTimeFormatter,format)
{
	DateTimeFormatter* th =asAtomHandler::as<DateTimeFormatter>(obj);
	_NR<Date> dt;
	ARG_UNPACK_ATOM(dt);
	tiny_string res;
	if (!dt.isNull())
	{
		LOG(LOG_NOT_IMPLEMENTED,"DateTimeFormatter.format is not really tested for all formats");
		std::locale l =  std::locale::global(th->currlocale);
		std::string pattern = th->pattern;
		if (th->dateStyle != "custom")
		{
			pattern = buildDateTimePattern(th->dateStyle, th->timeStyle);
		}
		tiny_string internalPattern = pattern = buildInternalFormat(pattern);
		tiny_string value = dt->toFormat(true, internalPattern);
		if (value.startsWith(" "))
		{
			value = value.substr(1,value.numBytes());
		}
		ret = asAtomHandler::fromString(sys,value);
		std::locale::global(l);
		th->lastOperationStatus = "noError";
	}
	else
	{
		ret = asAtomHandler::fromString(sys,"");
	}
}

ASFUNCTIONBODY_ATOM(DateTimeFormatter,formatUTC)
{
	DateTimeFormatter* th =asAtomHandler::as<DateTimeFormatter>(obj);
	_NR<Date> dt;
	ARG_UNPACK_ATOM(dt);
	tiny_string res;
	if (!dt.isNull())
	{
		LOG(LOG_NOT_IMPLEMENTED,"DateTimeFormatter.format is not really tested for all formats");
		std::locale l =  std::locale::global(th->currlocale);
		std::string pattern = th->pattern;

		if (th->dateStyle != "custom")
		{
			pattern = buildDateTimePattern(th->dateStyle, th->timeStyle);
		}
		tiny_string internalPattern = pattern = buildInternalFormat(pattern);
		tiny_string value = dt->toFormat(true, internalPattern);	
		if (value.startsWith(" "))
		{
			value = value.substr(1,value.numBytes());
		}
		ret = asAtomHandler::fromString(sys,value);
		std::locale::global(l);
		th->lastOperationStatus = "noError";
	}
	else
	{
		ret = asAtomHandler::fromString(sys,"");
	}
}

ASFUNCTIONBODY_ATOM(DateTimeFormatter,getAvailableLocaleIDNames)
{
	DateTimeFormatter* th =asAtomHandler::as<DateTimeFormatter>(obj);
	Array* res=Class<Array>::getInstanceSNoArgs(sys);
	std::vector<std::string> localeIds = sys->localeManager->getAvailableLocaleIDNames();
	for (std::vector<std::string>::iterator it = localeIds.begin(); it != localeIds.end(); ++it)
	{
		tiny_string value = (*it);
		res->push(asAtomHandler::fromObject(abstract_s(sys, value)));
	}
	th->lastOperationStatus="noError";
	ret = asAtomHandler::fromObject(res);
}

ASFUNCTIONBODY_ATOM(DateTimeFormatter,getDateStyle)
{
	DateTimeFormatter* th =asAtomHandler::as<DateTimeFormatter>(obj);
	ret = asAtomHandler::fromString(sys,th->dateStyle);
}

ASFUNCTIONBODY_ATOM(DateTimeFormatter,getDateTimePattern)
{
	DateTimeFormatter* th =asAtomHandler::as<DateTimeFormatter>(obj);
	ret = asAtomHandler::fromString(sys,th->pattern);
}

ASFUNCTIONBODY_ATOM(DateTimeFormatter,getFirstWeekday)
{
	LOG(LOG_NOT_IMPLEMENTED,"DateTimeFormatter.getFirstWeekday is not implemented and always returns 0");
}

ASFUNCTIONBODY_ATOM(DateTimeFormatter,getMonthNames)
{
	LOG(LOG_NOT_IMPLEMENTED,"DateTimeFormatter.getMonthNames is not implemented");
}

ASFUNCTIONBODY_ATOM(DateTimeFormatter,getTimeStyle)
{
	DateTimeFormatter* th =asAtomHandler::as<DateTimeFormatter>(obj);
	ret = asAtomHandler::fromString(sys,th->timeStyle);
}

ASFUNCTIONBODY_ATOM(DateTimeFormatter,getWeekdayNames)
{
	LOG(LOG_NOT_IMPLEMENTED,"DateTimeFormatter.getWeekdayNames is not implemented");
}

ASFUNCTIONBODY_ATOM(DateTimeFormatter,setDateTimeStyles)
{
	DateTimeFormatter* th =asAtomHandler::as<DateTimeFormatter>(obj);
	tiny_string dateStyle, timeStyle;
	ARG_UNPACK_ATOM(dateStyle)(timeStyle);

	if (dateStyle == "long" || dateStyle == "medium" ||
		dateStyle == "short" || dateStyle == "none")
	{
		th->dateStyle = dateStyle;
	}
	else
	{
		throw Class<ArgumentError>::getInstanceS(sys,"dateStyle value is not valid", kInvalidArgumentError);
	}
	if (timeStyle == "long" || timeStyle == "medium" ||
		timeStyle == "short" || timeStyle == "none")
	{
		th->timeStyle = timeStyle;
	}
	else
	{
		throw Class<ArgumentError>::getInstanceS(sys,"timeStyle value is not valid", kInvalidArgumentError);
	}
}

tiny_string DateTimeFormatter::buildDateTimePattern(tiny_string dateStyle, tiny_string timeStyle)
{
	tiny_string pattern;
	if (timeStyle == "long" || timeStyle == "medium")
	{
		pattern = pattern + "YYYY-mm-dd p";
	}
	else if (dateStyle == "short")
	{
		pattern = pattern + "YYYY-mm-dd p";
	}

	if (timeStyle == "long" || timeStyle == "medium")
	{
		pattern = pattern + " hh:MM:ss";
	}
	else if (timeStyle == "short")
	{
		pattern = pattern + " hh:MM";
	}
	return pattern;
}

std::string DateTimeFormatter::buildInternalFormat(std::string pattern)
{
	std::vector<std::string> list;
	std::string word = "";

	for (std::string::iterator it = pattern.begin(); it != pattern.end(); ++it)
	{
		if (word == "")
		{
			word.push_back((*it));
		}
		else
		{
			if (word[0] == (*it))
			{
				word.push_back((*it));
			}
			else
			{
				list.push_back(word);
				word = (*it);
			}
		}
	}
	list.push_back(word);

	bool quatation = false;

	for (std::vector<std::string>::iterator it = list.begin(); it != list.end(); ++it)
	{
		std::string item = (*it);

		if (item == "'")
		{
			(*it) = "";
			quatation = !quatation;
		}
		else if (quatation)
		{
			// Do nothing here as we don't convert literal strings
		}
		else if (item == "%")
		{
			(*it) = "%%";
		}
		else if (item == "yyyy" || item == "yyyyy" || item == "YYYY" || item == "YYYYY" || item == "y" || item == "yy")
		{
			(*it) = "%y";	// Not correct but will do, returns something like "98"
		}
		else if (item == "M")
		{
			(*it) = "%m";	// Not correct but will do, value is padded with 0
		}
		else if (item == "MM")
		{
			(*it) = "%m";
		}
		else if (item == "MMM")
		{
			(*it) = "%b";
		}
		else if (item == "MMMM" || item == "MMMMM")
		{
			(*it) = "%B";
		}
		else if (item == "d")
		{
			(*it) = "%e";
		}
		else if (item == "dd")
		{
			(*it) = "%d";
		}
		else if (item == "a" || item == "p")
		{
			(*it) = "%p";
		}
		else if (item == "H" || item == "HH"|| item == "HHH")
		{
			(*it) = "%H";	// Not correct but will do, value is padded with 0
		}
		else if (item == "m" || item == "mm")
		{
			(*it) = "%M";	// Not correct but will do, value is padded with 0
		}
		else if (item == "s")
		{
			(*it) = "%S";
		}
		else if (item == "ss")
		{
			(*it) = "%S";
		}
		else if (item == "E" || item == "EE" || item == "EEE")
		{
			(*it) = "%a";
		}
		else if (item == "EEEE" || item == "EEEEE")
		{
			(*it) = "%A";
		}
		else if (item == "h")
		{
			(*it) = "%l"; // Not correct but will do, value is padded with 0
		}
		else if (item == "hh" || item == "hhh")
		{
			(*it) = "%I";
		}
		else if (item == "K")
		{
			(*it) = "%l";
		}
		else if (item == "KK")
		{
			(*it) = "%I";
		}
		else if (item == "k")
		{
			(*it) = "%k";
		}
		else if (item == "kk")
		{
			(*it) = "%H";
		}
		else if (item == "G" ||item == "GG" || item == "GGG" || item == "GGGG" || item == "GGGGG")
		{
			// Do nothing. Not supported.
		}
		else
		{
			// Specifiers below are empty in Flash even though
			// AS3 documentation says the opposite
			std::vector<std::string> unusedTokens;
			unusedTokens.push_back("S");
			unusedTokens.push_back("SS");
			unusedTokens.push_back("SSS");
			unusedTokens.push_back("SSSS");
			unusedTokens.push_back("SSSSS");
			unusedTokens.push_back("Z");
			unusedTokens.push_back("ZZ");
			unusedTokens.push_back("ZZZ");
			unusedTokens.push_back("ZZZZ");
			unusedTokens.push_back("z");
			unusedTokens.push_back("zz");
			unusedTokens.push_back("zzz");
			unusedTokens.push_back("zzzz");
			unusedTokens.push_back("F");
			unusedTokens.push_back("Q");
			unusedTokens.push_back("QQ");
			unusedTokens.push_back("QQQ");
			unusedTokens.push_back("vvvv");
			unusedTokens.push_back("v");
			unusedTokens.push_back("Q");
			unusedTokens.push_back("QQ");
			unusedTokens.push_back("QQQ");
			unusedTokens.push_back("QQQQ");
			unusedTokens.push_back("w");
			unusedTokens.push_back("ww");
			unusedTokens.push_back("W");
			unusedTokens.push_back("WW");
			unusedTokens.push_back("D");
			unusedTokens.push_back("DD");
			unusedTokens.push_back("DDD");

			for (std::vector<std::string>::iterator t = unusedTokens.begin(); t != unusedTokens.end(); ++t)
			{
				if (item == (*t))
				{
					(*it) = "";
				}
			}
		}
	}

	std::string output = "";
	for (std::vector<std::string>::iterator it = list.begin(); it != list.end(); ++it)
	{
		output += (*it);
	}
	return output;
}
