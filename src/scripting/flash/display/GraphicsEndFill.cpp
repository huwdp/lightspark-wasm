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

#include "scripting/flash/display/GraphicsEndFill.h"
#include "scripting/class.h"

using namespace lightspark;

GraphicsEndFill::GraphicsEndFill(Class_base* c):
	ASObject(c)
{
}

void GraphicsEndFill::sinit(Class_base* c)
{
	CLASS_SETUP(c, ASObject, _constructor, CLASS_SEALED | CLASS_FINAL);
	c->addImplementedInterface(InterfaceClass<IGraphicsFill>::getClass(c->getSystemState()));
	IGraphicsFill::linkTraits(c);
	c->addImplementedInterface(InterfaceClass<IGraphicsData>::getClass(c->getSystemState()));
	IGraphicsData::linkTraits(c);
}

FILLSTYLE GraphicsEndFill::toFillStyle()
{
	return FILLSTYLE(0xff);
}

void GraphicsEndFill::appendToTokens(std::vector<_NR<GeomToken>, reporter_allocator<_NR<GeomToken>> > &tokens)
{
	tokens.emplace_back(_MR(new GeomToken(CLEAR_FILL)));
}
