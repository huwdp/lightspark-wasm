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

#include "scripting/flash/system/ime.h"
#include "scripting/class.h"
#include "scripting/argconv.h"
#include <iomanip>

using namespace lightspark;

void IME::sinit(Class_base* c)
{
	CLASS_SETUP_NO_CONSTRUCTOR(c, ASObject, CLASS_SEALED | CLASS_FINAL);

	c->setDeclaredMethodByQName("conversionMode","",Class<IFunction>::getFunction(c->getSystemState(),_getConversionMode),GETTER_METHOD,false);
	c->setDeclaredMethodByQName("conversionMode","",Class<IFunction>::getFunction(c->getSystemState(),_setConversionMode),SETTER_METHOD,false);

	c->setDeclaredMethodByQName("enabled","",Class<IFunction>::getFunction(c->getSystemState(),_getEnabled),GETTER_METHOD,false);
	c->setDeclaredMethodByQName("enabled","",Class<IFunction>::getFunction(c->getSystemState(),_setEnabled),SETTER_METHOD,false);

	c->setDeclaredMethodByQName("isSupported","",Class<IFunction>::getFunction(c->getSystemState(),_getIsSupported),GETTER_METHOD,false);

	c->setDeclaredMethodByQName("compositionAbandoned","",Class<IFunction>::getFunction(c->getSystemState(),compositionAbandoned),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("compositionSelectionChanged","",Class<IFunction>::getFunction(c->getSystemState(),compositionSelectionChanged),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("doConversion","",Class<IFunction>::getFunction(c->getSystemState(),doConversion),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("setCompositionString","",Class<IFunction>::getFunction(c->getSystemState(),setCompositionString),NORMAL_METHOD,true);
}

ASFUNCTIONBODY_ATOM(IME,compositionAbandoned)
{
	LOG(LOG_NOT_IMPLEMENTED,"IME.compositionAbandoned is not implemented");
}

ASFUNCTIONBODY_ATOM(IME,compositionSelectionChanged)
{
	LOG(LOG_NOT_IMPLEMENTED,"IME.compositionAbandoned is not implemented");
}

ASFUNCTIONBODY_ATOM(IME,doConversion)
{
	LOG(LOG_NOT_IMPLEMENTED,"IME.compositionAbandoned is not implemented");
}

ASFUNCTIONBODY_ATOM(IME,setCompositionString)
{
	LOG(LOG_NOT_IMPLEMENTED,"IME.compositionAbandoned is not implemented");
}

ASFUNCTIONBODY_ATOM(IME,_getConversionMode)
{
	LOG(LOG_NOT_IMPLEMENTED,"IME.compositionAbandoned is not implemented");
}
ASFUNCTIONBODY_ATOM(IME,_setConversionMode)
{
	LOG(LOG_NOT_IMPLEMENTED,"IME.compositionAbandoned is not implemented");
}

ASFUNCTIONBODY_ATOM(IME,_getEnabled)
{
	LOG(LOG_NOT_IMPLEMENTED,"IME.compositionAbandoned is not implemented");
	asAtomHandler::setBool(ret,false);
}
ASFUNCTIONBODY_ATOM(IME,_setEnabled)
{
	LOG(LOG_NOT_IMPLEMENTED,"IME.compositionAbandoned is not implemented");
}

ASFUNCTIONBODY_ATOM(IME,_getIsSupported)
{
	LOG(LOG_NOT_IMPLEMENTED,"IME.compositionAbandoned is not implemented");
	asAtomHandler::setBool(ret,false);
}
