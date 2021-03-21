/**************************************************************************
    Lightspark, a free flash player implementation

    Copyright (C) 2010-2013  Alessandro Pignotti (a.pignotti@sssup.it)

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

#include "scripting/flash/external/ExternalInterface.h"
#include "scripting/class.h"
#include "backends/extscriptobject.h"
#include "scripting/toplevel/ASString.h"

using namespace lightspark;

void ExternalInterface::sinit(Class_base* c)
{
	CLASS_SETUP_NO_CONSTRUCTOR(c, ASObject, CLASS_SEALED | CLASS_FINAL);
	c->setDeclaredMethodByQName("available","",Class<IFunction>::getFunction(c->getSystemState(),_getAvailable),GETTER_METHOD,false);
	c->setDeclaredMethodByQName("objectID","",Class<IFunction>::getFunction(c->getSystemState(),_getObjectID),GETTER_METHOD,false);
	c->setDeclaredMethodByQName("marshallExceptions","",Class<IFunction>::getFunction(c->getSystemState(),_getMarshallExceptions),GETTER_METHOD,false);
	c->setDeclaredMethodByQName("marshallExceptions","",Class<IFunction>::getFunction(c->getSystemState(),_setMarshallExceptions),SETTER_METHOD,false);
	c->setDeclaredMethodByQName("addCallback","",Class<IFunction>::getFunction(c->getSystemState(),addCallback),NORMAL_METHOD,false);
	c->setDeclaredMethodByQName("call","",Class<IFunction>::getFunction(c->getSystemState(),call),NORMAL_METHOD,false);
}

ASFUNCTIONBODY_ATOM(ExternalInterface,_getAvailable)
{
	asAtomHandler::setBool(ret,sys->extScriptObject != NULL);
}

ASFUNCTIONBODY_ATOM(ExternalInterface,_getObjectID)
{
	if(sys->extScriptObject == NULL)
	{
		ret = asAtomHandler::fromString(sys,"");
		return;
	}

	ExtScriptObject* so=sys->extScriptObject;
	if(so->hasProperty("name")==false)
	{
		ret = asAtomHandler::fromString(sys,"");
		return;
	}

	const ExtVariant& object = so->getProperty("name");
	std::string result = object.getString();
	ret = asAtomHandler::fromObject(abstract_s(sys,result));
}

ASFUNCTIONBODY_ATOM(ExternalInterface, _getMarshallExceptions)
{
	if(sys->extScriptObject == NULL)
		asAtomHandler::setBool(ret,false);
	else
		asAtomHandler::setBool(ret,sys->extScriptObject->getMarshallExceptions());
}

ASFUNCTIONBODY_ATOM(ExternalInterface, _setMarshallExceptions)
{
	if(sys->extScriptObject != NULL)
		sys->extScriptObject->setMarshallExceptions(asAtomHandler::Boolean_concrete(args[0]));
}


ASFUNCTIONBODY_ATOM(ExternalInterface,addCallback)
{
	if(sys->extScriptObject == NULL)
	{
		asAtomHandler::setBool(ret,false);
		return;
	}
//		throw Class<ASError>::getInstanceS("Container doesn't support callbacks");

	assert_and_throw(argslen == 2);

	if(asAtomHandler::isNull(args[1]))
		sys->extScriptObject->removeMethod(asAtomHandler::toString(args[0],sys).raw_buf());
	else
	{
		sys->extScriptObject->setMethod(asAtomHandler::toString(args[0],sys).raw_buf(), new ExtASCallback(args[1]));
	}
	asAtomHandler::setBool(ret,true);
}

ASFUNCTIONBODY_ATOM(ExternalInterface,call)
{
	if(sys->extScriptObject == NULL)
	{
		asAtomHandler::setNull(ret);
		return;
	}
//		throw Class<ASError>::getInstanceS("Container doesn't support callbacks");

	assert_and_throw(argslen >= 1);
	const tiny_string& arg0=asAtomHandler::toString(args[0],sys);

	// TODO: Check security constraints & throw SecurityException

	// Convert given arguments to ExtVariants
	const ExtVariant** callArgs = g_newa(const ExtVariant*,argslen-1);
	std::map<const ASObject*, std::unique_ptr<ExtObject>> objectsMap;
	for(uint32_t i = 0; i < argslen-1; i++)
	{
		ASATOM_INCREF(args[i+1]);
		callArgs[i] = new ExtVariant(objectsMap,_MR(asAtomHandler::toObject(args[i+1],sys)));
	}

	ASObject* asobjResult = NULL;
	// Let the external script object call the external method
	bool callSuccess = sys->extScriptObject->callExternal(arg0.raw_buf(), callArgs, argslen-1, &asobjResult);

	// Delete converted arguments
	for(uint32_t i = 0; i < argslen-1; i++)
		delete callArgs[i];

	if(!callSuccess)
	{
		assert(asobjResult==NULL);
		LOG(LOG_INFO, "External function failed, returning null: " << arg0);
		// If the call fails, return null
		asAtomHandler::setNull(ret);
		return;
	}
	ret = asAtomHandler::fromObject(asobjResult);
}
