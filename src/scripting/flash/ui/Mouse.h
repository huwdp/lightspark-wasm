/**************************************************************************
    Lightspark, a free flash player implementation

    Copyright (C) 2013  Antti Ajanki (antti.ajanki@iki.fi)

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

#ifndef SCRIPTING_FLASH_UI_MOUSE_H
#define SCRIPTING_FLASH_UI_MOUSE_H 1

#include "asobject.h"

namespace lightspark
{
class Vector;
class Point;

class Mouse : public ASObject
{
public:
	Mouse(Class_base* c):ASObject(c){}
	static void sinit(Class_base* c);
	ASFUNCTION_ATOM(hide);
	ASFUNCTION_ATOM(show);
	ASFUNCTION_ATOM(getCursor);
	ASFUNCTION_ATOM(setCursor);
	ASFUNCTION_ATOM(getSupportsCursor);
	ASFUNCTION_ATOM(getSupportsNativeCursor);
	ASFUNCTION_ATOM(registerCursor);
};

class MouseCursor : public ASObject
{
public:
	MouseCursor(Class_base* c):ASObject(c){}
	static void sinit(Class_base* c);
};

class MouseCursorData : public ASObject
{
public:
	MouseCursorData(Class_base* c);
	static void sinit(Class_base* c);
	ASFUNCTION_ATOM(_constructor);
	ASPROPERTY_GETTER_SETTER(_NR<Vector>,data);
	ASPROPERTY_GETTER_SETTER(number_t,frameRate);
	ASPROPERTY_GETTER_SETTER(_NR<Point>,hotSpot);
};

};

#endif /* SCRIPTING_FLASH_UI_MOUSE_H */
