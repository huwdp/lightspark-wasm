/**************************************************************************
    Lightspark, a free flash player implementation

    Copyright (C) 2011-2013  Alessandro Pignotti (a.pignotti@sssup.it)

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

#include "scripting/toplevel/Vector.h"
#include "scripting/abc.h"
#include "scripting/class.h"
#include "parsing/amf3_generator.h"
#include "scripting/argconv.h"
#include "scripting/toplevel/XML.h"
#include <3rdparty/pugixml/src/pugixml.hpp>
#include <algorithm>

using namespace std;
using namespace lightspark;

void Vector::sinit(Class_base* c)
{
	CLASS_SETUP(c, ASObject, _constructor, CLASS_FINAL);
	c->isReusable = true;
	c->setDeclaredMethodByQName("length",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),getLength,0,Class<UInteger>::getRef(c->getSystemState()).getPtr()),GETTER_METHOD,true);
	c->setDeclaredMethodByQName("length",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),setLength,0,Class<UInteger>::getRef(c->getSystemState()).getPtr()),SETTER_METHOD,true);
	c->setDeclaredMethodByQName("toString",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),_toString),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("toString",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),_toString),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("fixed",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),getFixed,0,Class<Boolean>::getRef(c->getSystemState()).getPtr()),GETTER_METHOD,true);
	c->setDeclaredMethodByQName("fixed",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),setFixed,0,Class<Boolean>::getRef(c->getSystemState()).getPtr()),SETTER_METHOD,true);
	c->setDeclaredMethodByQName("concat",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),_concat),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("concat",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),_concat),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("every",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),every),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("every",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),every),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("filter",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),filter),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("filter",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),filter),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("forEach",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),forEach),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("forEach",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),forEach),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("indexOf",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),indexOf),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("indexOf",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),indexOf),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("lastIndexOf",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),lastIndexOf),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("lastIndexOf",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),lastIndexOf),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("join",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),join),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("join",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),join),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("map",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),_map),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("map",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),_map),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("pop",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),_pop),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("pop",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),_pop),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("push",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),push),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("push",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),push),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("reverse",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),_reverse),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("reverse",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),_reverse),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("shift",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),shift),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("shift",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),shift),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("slice",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),slice),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("slice",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),slice),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("some",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),some),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("some",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),some),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("sort",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),_sort),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("sort",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),_sort),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("splice",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),splice),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("splice",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),splice),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("toLocaleString",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),_toString),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("toLocaleString",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),_toString),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("unshift",nsNameAndKind(),Class<IFunction>::getFunction(c->getSystemState(),unshift),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("unshift",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),unshift),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("insertAt",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),insertAt,2),NORMAL_METHOD,true);
	c->setDeclaredMethodByQName("removeAt",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),removeAt,1),NORMAL_METHOD,true);
	

	c->prototype->setVariableByQName(BUILTIN_STRINGS::STRING_TOSTRING,nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),_toString),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("concat",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),_concat),CONSTANT_TRAIT);
	c->prototype->setVariableByQName("every",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),every),CONSTANT_TRAIT);
	c->prototype->setVariableByQName("filter",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),filter),CONSTANT_TRAIT);
	c->prototype->setVariableByQName("forEach",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),forEach),CONSTANT_TRAIT);
	c->prototype->setVariableByQName("indexOf",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),indexOf),CONSTANT_TRAIT);
	c->prototype->setVariableByQName("lastIndexOf",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),lastIndexOf),CONSTANT_TRAIT);
	c->prototype->setVariableByQName("join",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),join),CONSTANT_TRAIT);
	c->prototype->setVariableByQName("map",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),_map),CONSTANT_TRAIT);
	c->prototype->setVariableByQName("pop",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),_pop),CONSTANT_TRAIT);
	c->prototype->setVariableByQName("push",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),push),CONSTANT_TRAIT);
	c->prototype->setVariableByQName("reverse",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),_reverse),CONSTANT_TRAIT);
	c->prototype->setVariableByQName("shift",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),shift),CONSTANT_TRAIT);
	c->prototype->setVariableByQName("slice",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),slice),CONSTANT_TRAIT);
	c->prototype->setVariableByQName("some",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),some),CONSTANT_TRAIT);
	c->prototype->setVariableByQName("sort",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),_sort),CONSTANT_TRAIT);
	c->prototype->setVariableByQName("splice",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),splice),CONSTANT_TRAIT);
	c->prototype->setVariableByQName("toLocaleString",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),_toString),DYNAMIC_TRAIT);
	c->prototype->setVariableByQName("unshift",nsNameAndKind(c->getSystemState(),BUILTIN_STRINGS::STRING_AS3NS,NAMESPACE),Class<IFunction>::getFunction(c->getSystemState(),unshift),CONSTANT_TRAIT);
}

Vector::Vector(Class_base* c, const Type *vtype):ASObject(c,T_OBJECT,SUBTYPE_VECTOR),vec_type(vtype),fixed(false),vec(reporter_allocator<asAtom>(c->memoryAccount))
{
}

Vector::~Vector()
{
}

bool Vector::destruct()
{
	for(unsigned int i=0;i<size();i++)
	{
		ASATOM_DECREF(vec[i]);
	}
	vec.clear();
	vec_type=nullptr;
	return destructIntern();
}

void Vector::setTypes(const std::vector<const Type *> &types)
{
	assert(vec_type == NULL);
	if(types.size() == 1)
		vec_type = types[0];
}
bool Vector::sameType(const Class_base *cls) const
{
	if (this->getClass()->class_name == cls->class_name)
		return true;
	tiny_string clsname = this->getClass()->getQualifiedClassName();
	return (clsname.startsWith(cls->class_name.getQualifiedName(getSystemState()).raw_buf()));
}

void Vector::generator(asAtom& ret,SystemState *sys, asAtom &o_class, asAtom* args, const unsigned int argslen)
{
	assert_and_throw(argslen == 1);
	assert_and_throw(asAtomHandler::toObject(args[0],sys)->getClass());
	assert_and_throw(asAtomHandler::as<TemplatedClass<Vector>>(o_class)->getTypes().size() == 1);

	const Type* type = asAtomHandler::as<TemplatedClass<Vector>>(o_class)->getTypes()[0];

	if(asAtomHandler::is<Array>(args[0]))
	{
		//create object without calling _constructor
		asAtomHandler::as<TemplatedClass<Vector>>(o_class)->getInstance(ret,false,nullptr,0);
		Vector* res = asAtomHandler::as<Vector>(ret);

		Array* a = asAtomHandler::as<Array>(args[0]);
		for(unsigned int i=0;i<a->size();++i)
		{
			asAtom obj = a->at(i);
			//Convert the elements of the array to the type of this vector
			if (!type->coerce(sys,obj))
				ASATOM_INCREF(obj);
			res->vec.push_back(obj);
		}
		res->setIsInitialized(true);
	}
	else if(asAtomHandler::getObject(args[0])->getClass()->getTemplate() == Template<Vector>::getTemplate(sys))
	{
		Vector* arg = asAtomHandler::as<Vector>(args[0]);

		Vector* res = nullptr;
		if (arg->vec_type == type)
		{
			// according to specs, the argument is returned if it is a vector of the same type as the provided class
			res = arg;
			res->incRef();
			ret = asAtomHandler::fromObject(res);
		}
		else
		{
			//create object without calling _constructor
			asAtomHandler::as<TemplatedClass<Vector>>(o_class)->getInstance(ret,false,nullptr,0);
			res = asAtomHandler::as<Vector>(ret);
			for(auto i = arg->vec.begin(); i != arg->vec.end(); ++i)
			{
				asAtom v = *i;
				if (!type->coerce(sys,v))
					ASATOM_INCREF(v);
				res->vec.push_back(v);
			}
		}
	}
	else
	{
		throwError<ArgumentError>(kCheckTypeFailedError, asAtomHandler::toObject(args[0],sys)->getClassName(), "Vector");
	}
}

ASFUNCTIONBODY_ATOM(Vector,_constructor)
{
	uint32_t len;
	bool fixed;
	ARG_UNPACK_ATOM (len, 0) (fixed, false);
	assert_and_throw(argslen <= 2);

	Vector* th=asAtomHandler::as<Vector>(obj);
	assert(th->vec_type);
	th->fixed = fixed;
	th->vec.resize(len, th->getDefaultValue());
}

ASFUNCTIONBODY_ATOM(Vector,_concat)
{
	Vector* th=asAtomHandler::as<Vector>(obj);
	th->getClass()->getInstance(ret,true,nullptr,0);
	Vector* res = asAtomHandler::as<Vector>(ret);
	// copy values into new Vector
	res->vec.resize(th->size(), th->getDefaultValue());
	auto it=th->vec.begin();
	uint32_t index = 0;
	for(;it != th->vec.end();++it)
	{
		res->vec[index]=*it;
		ASATOM_INCREF(res->vec[index]);
		index++;
	}
	//Insert the arguments in the vector
	int pos = sys->getSwfVersion() < 11 ? argslen-1 : 0;
	for(unsigned int i=0;i<argslen;i++)
	{
		if (asAtomHandler::is<Vector>(args[pos]))
		{
			Vector* arg=asAtomHandler::as<Vector>(args[pos]);
			res->vec.resize(index+arg->size(), th->getDefaultValue());
			auto it=arg->vec.begin();
			for(;it != arg->vec.end();++it)
			{
				if (asAtomHandler::isValid(*it))
				{
					res->vec[index]= *it;
					th->vec_type->coerceForTemplate(sys,res->vec[index]);
					ASATOM_INCREF(res->vec[index]);
				}
				index++;
			}
		}
		else
		{
			asAtom v = args[pos];
			if (!th->vec_type->coerce(sys,v))
				ASATOM_INCREF(v);
			res->vec[index] = v;
			index++;
		}
		pos += (sys->getSwfVersion() < 11 ?-1 : 1);
	}	
}

ASFUNCTIONBODY_ATOM(Vector,filter)
{
	if (argslen < 1 || argslen > 2)
		throwError<ArgumentError>(kWrongArgumentCountError, "Vector.filter", "1", Integer::toString(argslen));
	if (!asAtomHandler::is<IFunction>(args[0]))
		throwError<TypeError>(kCheckTypeFailedError, asAtomHandler::toObject(args[0],sys)->getClassName(), "Function");
	Vector* th=asAtomHandler::as<Vector>(obj);
	  
	asAtom f = args[0];
	asAtom params[3];
	th->getClass()->getInstance(ret,true,NULL,0);
	Vector* res= asAtomHandler::as<Vector>(ret);
	asAtom funcRet=asAtomHandler::invalidAtom;
	asAtom closure = asAtomHandler::getClosure(f) ? asAtomHandler::fromObject(asAtomHandler::getClosure(f)) : asAtomHandler::nullAtom;

	for(unsigned int i=0;i<th->size();i++)
	{
		params[0] = th->vec[i];
		params[1] = asAtomHandler::fromUInt(i);
		params[2] = asAtomHandler::fromObject(th);

		if(argslen==1)
		{
			asAtomHandler::callFunction(f,funcRet,closure, params, 3,false);
		}
		else
		{
			asAtomHandler::callFunction(f,funcRet,args[1], params, 3,false);
		}
		if(asAtomHandler::isValid(funcRet))
		{
			if(asAtomHandler::Boolean_concrete(funcRet))
			{
				ASATOM_INCREF(th->vec[i]);
				res->vec.push_back(th->vec[i]);
			}
			ASATOM_DECREF(funcRet);
		}
	}
}

ASFUNCTIONBODY_ATOM(Vector, some)
{
	if (argslen < 1)
		throwError<ArgumentError>(kWrongArgumentCountError, "Vector.some", "1", Integer::toString(argslen));
	if (!asAtomHandler::is<IFunction>(args[0]))
		throwError<TypeError>(kCheckTypeFailedError, asAtomHandler::toObject(args[0],sys)->getClassName(), "Function");
	Vector* th=static_cast<Vector*>(asAtomHandler::getObject(obj));
	asAtom f = args[0];
	asAtom params[3];
	asAtom closure = asAtomHandler::getClosure(f) ? asAtomHandler::fromObject(asAtomHandler::getClosure(f)) : asAtomHandler::nullAtom;

	for(unsigned int i=0; i < th->size(); i++)
	{
		params[0] = th->vec[i];
		params[1] = asAtomHandler::fromUInt(i);
		params[2] = asAtomHandler::fromObject(th);

		if(argslen==1)
		{
			asAtomHandler::callFunction(f,ret,closure, params, 3,false);
		}
		else
		{
			asAtomHandler::callFunction(f,ret,args[1], params, 3,false);
		}
		if(asAtomHandler::isValid(ret))
		{
			if(asAtomHandler::Boolean_concrete(ret))
			{
				return;
			}
			ASATOM_DECREF(ret);
		}
	}
	asAtomHandler::setBool(ret,false);
}

ASFUNCTIONBODY_ATOM(Vector, every)
{
	Vector* th=static_cast<Vector*>(asAtomHandler::getObject(obj));
	if (argslen < 1)
		throwError<ArgumentError>(kWrongArgumentCountError, "Vector.every", "1", Integer::toString(argslen));
	if (!asAtomHandler::is<IFunction>(args[0]))
		throwError<TypeError>(kCheckTypeFailedError, asAtomHandler::toObject(args[0],th->getSystemState())->getClassName(), "Function");
	asAtom f = args[0];
	asAtom params[3];
	asAtom closure = asAtomHandler::getClosure(f) ? asAtomHandler::fromObject(asAtomHandler::getClosure(f)) : asAtomHandler::nullAtom;

	for(unsigned int i=0; i < th->size(); i++)
	{
		if (asAtomHandler::isValid(th->vec[i]))
			params[0] = th->vec[i];
		else
			params[0] = asAtomHandler::nullAtom;
		params[1] = asAtomHandler::fromUInt(i);
		params[2] = asAtomHandler::fromObject(th);

		if(argslen==1)
		{
			asAtomHandler::callFunction(f,ret,closure, params, 3,false);
		}
		else
		{
			asAtomHandler::callFunction(f,ret,args[1], params, 3,false);
		}
		if(asAtomHandler::isValid(ret))
		{
			if (asAtomHandler::isUndefined(ret) || asAtomHandler::isNull(ret))
				throwError<TypeError>(kCallOfNonFunctionError, asAtomHandler::toString(ret,sys));
			if(!asAtomHandler::Boolean_concrete(ret))
			{
				return;
			}
			ASATOM_DECREF(ret);
		}
	}
	asAtomHandler::setBool(ret,true);
}

void Vector::append(asAtom &o)
{
	if (fixed)
	{
		ASATOM_DECREF(o);
		throwError<RangeError>(kVectorFixedError);
	}
	asAtom v = o;
	if (vec_type->coerce(getSystemState(),v))
		ASATOM_DECREF(v);
	vec.push_back(o);
}

void Vector::remove(ASObject *o)
{
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		if (asAtomHandler::getObject(*it) == o)
		{
			vec.erase(it);
			break;
		}
	}
}

ASObject *Vector::describeType() const
{
	pugi::xml_document p;
	pugi::xml_node root = p.append_child("type");

	// type attributes
	Class_base* prot=getClass();
	if(prot)
	{
		root.append_attribute("name").set_value(prot->getQualifiedClassName(true).raw_buf());
		root.append_attribute("base").set_value("__AS3__.vec::Vector.<*>");
	}
	bool isDynamic = getClass() && !getClass()->isSealed;
	root.append_attribute("isDynamic").set_value(isDynamic);
	root.append_attribute("isFinal").set_value(false);
	root.append_attribute("isStatic").set_value(false);
	pugi::xml_node node=root.append_child("extendsClass");
	node.append_attribute("type").set_value("__AS3__.vec::Vector.<*>");

	if(prot)
		prot->describeInstance(root,true,true);

	//LOG(LOG_INFO,"describeType:"<< Class<XML>::getInstanceS(getSystemState(),root)->toXMLString_internal());

	return XML::createFromNode(root);
	
}

ASFUNCTIONBODY_ATOM(Vector,push)
{
	Vector* th=static_cast<Vector*>(asAtomHandler::getObject(obj));
	if (th->fixed)
		throwError<RangeError>(kVectorFixedError);
	for(size_t i = 0; i < argslen; ++i)
	{
		//The proprietary player violates the specification and allows elements of any type to be pushed;
		//they are converted to the vec_type
		asAtom v = args[i];
		if (!th->vec_type->coerce(sys,v))
			ASATOM_INCREF(v);
		th->vec.push_back(v);
	}
	asAtomHandler::setUInt(ret,sys,(uint32_t)th->vec.size());
}

ASFUNCTIONBODY_ATOM(Vector,_pop)
{
	Vector* th=asAtomHandler::as<Vector>(obj);
	if (th->fixed)
		throwError<RangeError>(kVectorFixedError);
	uint32_t size =th->size();
	if (size == 0)
	{
		asAtomHandler::setNull(ret);
		th->vec_type->coerce(th->getSystemState(),ret);
		return;
	}
	ret = th->vec[size-1];
	th->vec.pop_back();
}

ASFUNCTIONBODY_ATOM(Vector,getLength)
{
	asAtomHandler::setUInt(ret,sys,(uint32_t)asAtomHandler::as<Vector>(obj)->vec.size());
}

ASFUNCTIONBODY_ATOM(Vector,setLength)
{
	Vector* th = asAtomHandler::as<Vector>(obj);
	if (th->fixed)
		throwError<RangeError>(kVectorFixedError);
	uint32_t len;
	ARG_UNPACK_ATOM (len);
	if(len <= th->vec.size())
	{
		for(size_t i=len; i< th->vec.size(); ++i)
			ASATOM_DECREF(th->vec[i]);
	}
	th->vec.resize(len, th->getDefaultValue());
}

ASFUNCTIONBODY_ATOM(Vector,getFixed)
{
	asAtomHandler::setBool(ret,asAtomHandler::as<Vector>(obj)->fixed);
}

ASFUNCTIONBODY_ATOM(Vector,setFixed)
{
	Vector* th = asAtomHandler::as<Vector>(obj);
	bool fixed;
	ARG_UNPACK_ATOM (fixed);
	th->fixed = fixed;
}

ASFUNCTIONBODY_ATOM(Vector,forEach)
{
	Vector* th=asAtomHandler::as<Vector>(obj);
	if (argslen < 1)
		throwError<ArgumentError>(kWrongArgumentCountError, "Vector.forEach", "1", Integer::toString(argslen));
	if (!asAtomHandler::is<IFunction>(args[0]))
		throwError<TypeError>(kCheckTypeFailedError, asAtomHandler::toObject(args[0],th->getSystemState())->getClassName(), "Function");
	asAtom f = args[0];
	asAtom params[3];
	asAtom closure = asAtomHandler::getClosure(f) ? asAtomHandler::fromObject(asAtomHandler::getClosure(f)) : asAtomHandler::nullAtom;

	for(unsigned int i=0; i < th->size(); i++)
	{
		params[0] = th->vec[i];
		params[1] = asAtomHandler::fromUInt(i);
		params[2] = asAtomHandler::fromObject(th);

		asAtom funcret=asAtomHandler::invalidAtom;
		if( argslen == 1 )
		{
			asAtomHandler::callFunction(f,funcret,closure, params, 3,false);
		}
		else
		{
			asAtomHandler::callFunction(f,funcret,args[1], params, 3,false);
		}
		ASATOM_DECREF(funcret);
	}
}

ASFUNCTIONBODY_ATOM(Vector, _reverse)
{
	Vector* th = asAtomHandler::as<Vector>(obj);

	std::vector<asAtom> tmp = std::vector<asAtom>(th->vec.begin(),th->vec.end());
	uint32_t size = th->size();
	th->vec.clear();
	th->vec.resize(size, th->getDefaultValue());
	std::vector<asAtom>::iterator it=tmp.begin();
	uint32_t index = size-1;
	for(;it != tmp.end();++it)
 	{
		th->vec[index]=*it;
		index--;
	}
	th->incRef();
	ret = asAtomHandler::fromObject(th);
}

ASFUNCTIONBODY_ATOM(Vector,lastIndexOf)
{
	Vector* th=asAtomHandler::as<Vector>(obj);
	assert_and_throw(argslen==1 || argslen==2);
	int32_t res=-1;
	asAtom arg0=args[0];

	if(th->vec.size() == 0)
	{
		asAtomHandler::setInt(ret,sys,(int32_t)-1);
		return;
	}

	size_t i = th->size()-1;

	if(argslen == 2 && std::isnan(asAtomHandler::toNumber(args[1])))
	{
		asAtomHandler::setInt(ret,sys,0);
		return;
	}

	if(argslen == 2 && !asAtomHandler::isUndefined(args[1]) && !std::isnan(asAtomHandler::toNumber(args[1])))
	{
		int j = asAtomHandler::toInt(args[1]); //Preserve sign
		if(j < 0) //Negative offset, use it as offset from the end of the array
		{
			if((size_t)-j > th->size())
				i = 0;
			else
				i = th->size()+j;
		}
		else //Positive offset, use it directly
		{
			if((size_t)j >= th->size()) //If the passed offset is bigger than the array, cap the offset
				i = th->size()-1;
			else
				i = j;
		}
	}
	do
	{
		if (asAtomHandler::isEqualStrict(th->vec[i],th->getSystemState(),arg0))
		{
			res=i;
			break;
		}
	}
	while(i--);

	asAtomHandler::setInt(ret,sys,res);
}

ASFUNCTIONBODY_ATOM(Vector,shift)
{
	Vector* th=asAtomHandler::as<Vector>(obj);
	if (th->fixed)
		throwError<RangeError>(kVectorFixedError);
	if(!th->size())
	{
		asAtomHandler::setNull(ret);
		th->vec_type->coerce(th->getSystemState(),ret);
		return;
	}
	if(asAtomHandler::isValid(th->vec[0]))
		ret=th->vec[0];
	else
	{
		asAtomHandler::setNull(ret);
		th->vec_type->coerce(th->getSystemState(),ret);
	}
	for(uint32_t i= 1;i< th->size();i++)
	{
		th->vec[i-1]=th->vec[i];
	}
	th->vec.resize(th->size()-1, th->getDefaultValue());
}

int Vector::capIndex(int i) const
{
	int totalSize=size();

	if(totalSize <= 0)
		return 0;
	else if(i < -totalSize)
		return 0;
	else if(i > totalSize)
		return totalSize;
	else if(i>=0)     // 0 <= i < totalSize
		return i;
	else              // -totalSize <= i < 0
	{
		//A negative i is relative to the end
		return i+totalSize;
	}
}

asAtom Vector::getDefaultValue()
{
	if (vec_type == Class<Integer>::getClass(getSystemState()))
		return asAtomHandler::fromInt(0);
	else if (vec_type == Class<UInteger>::getClass(getSystemState()))
		return asAtomHandler::fromUInt(0);
	else if (vec_type == Class<Number>::getClass(getSystemState()))
		return asAtomHandler::fromInt(0);
	else
		return asAtomHandler::nullAtom;
}

ASFUNCTIONBODY_ATOM(Vector,slice)
{
	Vector* th=asAtomHandler::as<Vector>(obj);

	int startIndex=0;
	int endIndex=16777215;
	if(argslen>0)
		startIndex=asAtomHandler::toInt(args[0]);
	if(argslen>1)
		endIndex=asAtomHandler::toInt(args[1]);

	startIndex=th->capIndex(startIndex);
	endIndex=th->capIndex(endIndex);
	th->getClass()->getInstance(ret,true,NULL,0);
	Vector* res= asAtomHandler::as<Vector>(ret);
	res->vec.resize(endIndex-startIndex, th->getDefaultValue());
	int j = 0;
	for(int i=startIndex; i<endIndex; i++) 
	{
		if (asAtomHandler::isValid(th->vec[i]))
		{
			res->vec[j] =th->vec[i];
			if (!th->vec_type->coerce(th->getSystemState(),th->vec[j]))
				ASATOM_INCREF(th->vec[j]);
		}
		j++;
	}
}

ASFUNCTIONBODY_ATOM(Vector,splice)
{
	Vector* th=asAtomHandler::as<Vector>(obj);
	if (th->fixed)
		throwError<RangeError>(kVectorFixedError);
	int startIndex=asAtomHandler::toInt(args[0]);
	//By default, delete all the element up to the end
	//Use the array len, it will be capped below
	int deleteCount=th->size();
	if(argslen > 1)
		deleteCount=asAtomHandler::toUInt(args[1]);
	int totalSize=th->size();
	th->getClass()->getInstance(ret,true,NULL,0);
	Vector* res= asAtomHandler::as<Vector>(ret);

	startIndex=th->capIndex(startIndex);

	if((startIndex+deleteCount)>totalSize)
		deleteCount=totalSize-startIndex;

	res->vec.resize(deleteCount, th->getDefaultValue());
	if(deleteCount)
	{
		// write deleted items to return array
		for(int i=0;i<deleteCount;i++)
		{
			res->vec[i] = th->vec[startIndex+i];
		}
		// delete items from current array
		for (int i = 0; i < deleteCount; i++)
		{
			th->vec[startIndex+i] = th->getDefaultValue();
		}
	}
	// remember items in current array that have to be moved to new position
	vector<asAtom> tmp = vector<asAtom>(totalSize- (startIndex+deleteCount));
	tmp.resize(totalSize- (startIndex+deleteCount), th->getDefaultValue());
	for (int i = startIndex+deleteCount; i < totalSize ; i++)
	{
		if (asAtomHandler::isValid(th->vec[i]))
		{
			tmp[i-(startIndex+deleteCount)] = th->vec[i];
			th->vec[i] = th->getDefaultValue();
		}
	}
	th->vec.resize(startIndex, th->getDefaultValue());

	
	//Insert requested values starting at startIndex
	for(unsigned int i=2;i<argslen;i++)
	{
		ASATOM_INCREF(args[i]);
		th->vec.push_back(args[i]);
	}
	// move remembered items to new position
	th->vec.resize((totalSize-deleteCount)+(argslen > 2 ? argslen-2 : 0), th->getDefaultValue());
	for(int i=0;i<totalSize- (startIndex+deleteCount);i++)
	{
		th->vec[startIndex+i+(argslen > 2 ? argslen-2 : 0)] = tmp[i];
	}
}

ASFUNCTIONBODY_ATOM(Vector,join)
{
	Vector* th=asAtomHandler::as<Vector>(obj);
	
	tiny_string del = ",";
	if (argslen == 1)
		  del=asAtomHandler::toString(args[0],sys);
	string res;
	for(uint32_t i=0;i<th->size();i++)
	{
		if (asAtomHandler::isValid(th->vec[i]))
			res+=asAtomHandler::toString(th->vec[i],sys).raw_buf();
		if(i!=th->size()-1)
			res+=del.raw_buf();
	}
	ret = asAtomHandler::fromObject(abstract_s(th->getSystemState(),res));
}

ASFUNCTIONBODY_ATOM(Vector,indexOf)
{
	Vector* th=asAtomHandler::as<Vector>(obj);
	assert_and_throw(argslen==1 || argslen==2);
	int32_t res=-1;
	asAtom arg0=args[0];

	int unsigned i = 0;
	if(argslen == 2)
	{
		i = asAtomHandler::toInt(args[1]);
	}

	for(;i<th->size();i++)
	{
		if(asAtomHandler::isEqualStrict(th->vec[i],th->getSystemState(),arg0))
		{
			res=i;
			break;
		}
	}
	asAtomHandler::setInt(ret,sys,res);
}
bool Vector::sortComparatorDefault::operator()(const asAtom& d1, const asAtom& d2)
{
	asAtom o1 = d1;
	asAtom o2 = d2;
	if(isNumeric)
	{
		number_t a=asAtomHandler::toNumber(o1);

		number_t b=asAtomHandler::toNumber(o2);

		if(std::isnan(a) || std::isnan(b))
			throw RunTimeException("Cannot sort non number with Array.NUMERIC option");
		if(isDescending)
			return b>a;
		else
			return a<b;
	}
	else
	{
		//Comparison is always in lexicographic order
		tiny_string s1 = asAtomHandler::toString(o1,getSys());
		tiny_string s2 = asAtomHandler::toString(o2,getSys());

		if(isDescending)
		{
			//TODO: unicode support
			if(isCaseInsensitive)
				return s1.strcasecmp(s2)>0;
			else
				return s1>s2;
		}
		else
		{
			//TODO: unicode support
			if(isCaseInsensitive)
				return s1.strcasecmp(s2)<0;
			else
				return s1<s2;
		}
	}
}
number_t Vector::sortComparatorWrapper::compare(const asAtom& d1, const asAtom& d2)
{
	asAtom objs[2];
	if (asAtomHandler::isValid(d1))
		objs[0] = d1;
	else
		objs[0] = asAtomHandler::nullAtom;
	if (asAtomHandler::isValid(d2))
		objs[1] = d2;
	else
		objs[1] = asAtomHandler::nullAtom;
	asAtom ret=asAtomHandler::invalidAtom;
	asAtom obj = asAtomHandler::getClosureAtom(comparator);
	// don't coerce the result, as it may be an int that would loose it's sign through coercion
	asAtomHandler::callFunction(comparator,ret,obj, objs, 2,false,false);
	assert_and_throw(asAtomHandler::isValid(ret));
	return asAtomHandler::toNumber(ret);
}

// std::sort expects strict weak ordering for the comparison function
// this is not guarranteed by user defined comparison functions, so we need our own sorting method.
// TODO this is just a simple quicksort implementation, not optimized for performance
void simplequicksortVector(std::vector<asAtom>& v, Vector::sortComparatorWrapper& comp, int32_t lo, int32_t hi)
{
	if (v.size() == 2)
	{
		if (comp.compare(v[0],v[1]) > 0)
			std::swap(v[0],v[1]);
		return;
	}
	if (lo < hi)
	{
		asAtom pivot = v[lo];
		int32_t i = lo - 1;
		int32_t j = hi + 1;
		while (true)
		{
			do
			{
				i++;
			}
			while (i < hi && (comp.compare(v[i],pivot) < 0));
			
			do
			{
				j--;
			}
			while (j >= 0 && (comp.compare(v[j],pivot) > 0));
	
			if (i >= j)
				break;
	
			std::swap(v[i],v[j]);
		}
		if (j >= lo)
		{
			simplequicksortVector(v, comp, lo, j);
			simplequicksortVector(v, comp, j + 1, hi);
		}
	}
}

ASFUNCTIONBODY_ATOM(Vector,_sort)
{
	if (argslen != 1)
		throwError<ArgumentError>(kWrongArgumentCountError, "Vector.sort", "1", Integer::toString(argslen));
	Vector* th=static_cast<Vector*>(asAtomHandler::getObject(obj));
	
	asAtom comp=asAtomHandler::invalidAtom;
	bool isNumeric=false;
	bool isCaseInsensitive=false;
	bool isDescending=false;
	if(asAtomHandler::isFunction(args[0])) //Comparison func
	{
		assert_and_throw(asAtomHandler::isInvalid(comp));
		comp=args[0];
	}
	else
	{
		uint32_t options=asAtomHandler::toInt(args[0]);
		if(options&Array::NUMERIC)
			isNumeric=true;
		if(options&Array::CASEINSENSITIVE)
			isCaseInsensitive=true;
		if(options&Array::DESCENDING)
			isDescending=true;
		if(options&(~(Array::NUMERIC|Array::CASEINSENSITIVE|Array::DESCENDING)))
			throw UnsupportedException("Vector::sort not completely implemented");
	}
	std::vector<asAtom> tmp = vector<asAtom>(th->vec.size());
	int i = 0;
	for(auto it=th->vec.begin();it != th->vec.end();++it)
	{
		tmp[i++]= *it;
	}
	
	if(asAtomHandler::isValid(comp))
	{
		sortComparatorWrapper c(comp);
		simplequicksortVector(tmp,c,0,tmp.size()-1);
	}
	else
		sort(tmp.begin(),tmp.end(),sortComparatorDefault(isNumeric,isCaseInsensitive,isDescending));

	th->vec.clear();
	for(auto ittmp=tmp.begin();ittmp != tmp.end();++ittmp)
	{
		th->vec.push_back(*ittmp);
	}
	ASATOM_INCREF(obj);
	ret = obj;
}

ASFUNCTIONBODY_ATOM(Vector,unshift)
{
	Vector* th=asAtomHandler::as<Vector>(obj);
	if (th->fixed)
		throwError<RangeError>(kVectorFixedError);
	if (argslen > 0)
	{
		uint32_t s = th->size();
		th->vec.resize(th->size()+argslen, th->getDefaultValue());
		for(uint32_t i=s;i> 0;i--)
		{
			th->vec[(i-1)+argslen]=th->vec[i-1];
			th->vec[i-1] = th->getDefaultValue();
		}
		
		for(uint32_t i=0;i<argslen;i++)
		{
			th->vec[i] = args[i];
			if (!th->vec_type->coerce(th->getSystemState(),th->vec[i]))
				ASATOM_INCREF(th->vec[i]);
		}
	}
	asAtomHandler::setInt(ret,sys,(int32_t)th->size());
}

ASFUNCTIONBODY_ATOM(Vector,_map)
{
	Vector* th=asAtomHandler::as<Vector>(obj);
	asAtom thisObject=asAtomHandler::invalidAtom;
	
	if (argslen >= 1 && !asAtomHandler::is<IFunction>(args[0]))
		throwError<TypeError>(kCheckTypeFailedError, asAtomHandler::toObject(args[0],th->getSystemState())->getClassName(), "Function");
	asAtom func=asAtomHandler::invalidAtom;

	ARG_UNPACK_ATOM(func)(thisObject,asAtomHandler::nullAtom);
	th->getClass()->getInstance(ret,true,NULL,0);
	Vector* res= asAtomHandler::as<Vector>(ret);

	for(uint32_t i=0;i<th->size();i++)
	{
		asAtom funcArgs[3];
		funcArgs[0]=th->vec[i];
		funcArgs[1]=asAtomHandler::fromUInt(i);
		funcArgs[2]=asAtomHandler::fromObject(th);
		asAtom funcRet=asAtomHandler::invalidAtom;
		asAtomHandler::callFunction(func,funcRet,thisObject, funcArgs, 3,false);
		assert_and_throw(asAtomHandler::isValid(funcRet));
		ASATOM_INCREF(funcRet);
		res->vec.push_back(funcRet);
	}

	ret = asAtomHandler::fromObject(res);
}

ASFUNCTIONBODY_ATOM(Vector,_toString)
{
	tiny_string res;
	Vector* th = asAtomHandler::as<Vector>(obj);
	for(size_t i=0; i < th->vec.size(); ++i)
	{
		if (asAtomHandler::isValid(th->vec[i]))
			res += asAtomHandler::toString(th->vec[i],sys);
		else
		{
			// use the type's default value
			asAtom natom = asAtomHandler::nullAtom;
			th->vec_type->coerce(th->getSystemState(), natom);
			res += asAtomHandler::toString(natom,sys);
		}

		if(i!=th->vec.size()-1)
			res += ',';
	}
	ret = asAtomHandler::fromObject(abstract_s(th->getSystemState(),res));
}

ASFUNCTIONBODY_ATOM(Vector,insertAt)
{
	Vector* th=asAtomHandler::as<Vector>(obj);
	if (th->fixed)
		throwError<RangeError>(kOutOfRangeError);
	int32_t index;
	asAtom o=asAtomHandler::invalidAtom;
	ARG_UNPACK_ATOM(index)(o);

	if (index < 0 && th->vec.size() >= (uint32_t)(-index))
		index = th->vec.size()+(index);
	if (index < 0)
		index = 0;
	if ((uint32_t)index >= th->vec.size())
	{
		ASATOM_INCREF(o);
		th->vec.push_back(o);
	}
	else
	{
		ASATOM_INCREF(o);
		th->vec.insert(th->vec.begin()+index,o);
	}
}

ASFUNCTIONBODY_ATOM(Vector,removeAt)
{
	Vector* th=asAtomHandler::as<Vector>(obj);
	if (th->fixed)
		throwError<RangeError>(kOutOfRangeError);
	int32_t index;
	ARG_UNPACK_ATOM(index);
	if (index < 0)
		index = th->vec.size()+index;
	if (index < 0)
		index = 0;
	if ((uint32_t)index < th->vec.size())
	{
		ret = th->vec[index];
		th->vec.erase(th->vec.begin()+index);
	}
	else
		throwError<RangeError>(kOutOfRangeError);
}

bool Vector::hasPropertyByMultiname(const multiname& name, bool considerDynamic, bool considerPrototype)
{
	if(!considerDynamic)
		return ASObject::hasPropertyByMultiname(name, considerDynamic, considerPrototype);

	if(!name.hasEmptyNS)
		return ASObject::hasPropertyByMultiname(name, considerDynamic, considerPrototype);

	unsigned int index=0;
	if(!Vector::isValidMultiname(getSystemState(),name,index))
		return ASObject::hasPropertyByMultiname(name, considerDynamic, considerPrototype);

	if(index < vec.size())
		return true;
	else
		return false;
}

/* this handles the [] operator, because vec[12] becomes vec.12 in bytecode */
GET_VARIABLE_RESULT Vector::getVariableByMultiname(asAtom& ret, const multiname& name, GET_VARIABLE_OPTION opt)
{
	if((opt & SKIP_IMPL)!=0 || !implEnable)
	{
		return getVariableByMultinameIntern(ret,name,this->getClass(),opt);
	}

	if(!name.hasEmptyNS)
	{
		return getVariableByMultinameIntern(ret,name,this->getClass(),opt);
	}

	unsigned int index=0;
	bool isNumber =false;
	if(!Vector::isValidMultiname(getSystemState(),name,index,&isNumber) || index > vec.size())
	{
		switch(name.name_type) 
		{
			case multiname::NAME_NUMBER:
				if (getSystemState()->getSwfVersion() >= 11 
						|| (uint32_t(name.name_d) == name.name_d && name.name_d < UINT32_MAX))
					throwError<RangeError>(kOutOfRangeError,name.normalizedName(getSystemState()),Integer::toString(vec.size()));
				else
					throwError<ReferenceError>(kReadSealedError, name.normalizedName(getSystemState()), this->getClass()->getQualifiedClassName());
				break;
			case multiname::NAME_INT:
				if (getSystemState()->getSwfVersion() >= 11
						|| name.name_i >= (int32_t)vec.size())
					throwError<RangeError>(kOutOfRangeError,name.normalizedName(getSystemState()),Integer::toString(vec.size()));
				else
					throwError<ReferenceError>(kReadSealedError, name.normalizedName(getSystemState()), this->getClass()->getQualifiedClassName());
				break;
			case multiname::NAME_UINT:
				throwError<RangeError>(kOutOfRangeError,name.normalizedName(getSystemState()),Integer::toString(vec.size()));
				break;
			case multiname::NAME_STRING:
				if (isNumber)
				{
					if (getSystemState()->getSwfVersion() >= 11 )
						throwError<RangeError>(kOutOfRangeError,name.normalizedName(getSystemState()),Integer::toString(vec.size()));
					else
						throwError<ReferenceError>(kReadSealedError, name.normalizedName(getSystemState()), this->getClass()->getQualifiedClassName());
				}
				break;
			default:
				break;
		}

		GET_VARIABLE_RESULT res = getVariableByMultinameIntern(ret,name,this->getClass(),opt);
		if (asAtomHandler::isInvalid(ret))
			throwError<ReferenceError>(kReadSealedError, name.normalizedName(getSystemState()), this->getClass()->getQualifiedClassName());
		return res;
	}
	if(index < vec.size())
	{
		ret = vec[index];
		if (!(opt & NO_INCREF))
			ASATOM_INCREF(ret);
	}
	else
	{
		throwError<RangeError>(kOutOfRangeError,
				       Integer::toString(index),
				       Integer::toString(vec.size()));
	}
	return GET_VARIABLE_RESULT::GETVAR_NORMAL;
}
GET_VARIABLE_RESULT Vector::getVariableByInteger(asAtom &ret, int index, GET_VARIABLE_OPTION opt)
{
	if (index >=0 && uint32_t(index) < size())
	{
		ret = vec[index];
		if (!(opt & NO_INCREF))
			ASATOM_INCREF(ret);
		return GET_VARIABLE_RESULT::GETVAR_NORMAL;
	}
	else
		return getVariableByIntegerIntern(ret,index,opt);
}

multiname *Vector::setVariableByMultiname(multiname& name, asAtom& o, CONST_ALLOWED_FLAG allowConst,bool* alreadyset)
{
	if(!name.hasEmptyNS)
		return ASObject::setVariableByMultiname(name, o, allowConst,alreadyset);

	unsigned int index=0;
	if(!Vector::isValidMultiname(getSystemState(),name,index))
	{
		switch(name.name_type) 
		{
			case multiname::NAME_NUMBER:
				if (getSystemState()->getSwfVersion() >= 11 
						|| (this->fixed && ((int32_t(name.name_d) != name.name_d) || name.name_d >= (int32_t)vec.size() || name.name_d < 0)))
					throwError<RangeError>(kOutOfRangeError,name.normalizedName(getSystemState()),Integer::toString(vec.size()));
				else
					throwError<ReferenceError>(kWriteSealedError, name.normalizedName(getSystemState()), this->getClass()->getQualifiedClassName());
				break;
			case multiname::NAME_INT:
				if (getSystemState()->getSwfVersion() >= 11
						|| (this->fixed && (name.name_i >= (int32_t)vec.size() || name.name_i < 0)))
					throwError<RangeError>(kOutOfRangeError,name.normalizedName(getSystemState()),Integer::toString(vec.size()));
				else
					throwError<ReferenceError>(kWriteSealedError, name.normalizedName(getSystemState()), this->getClass()->getQualifiedClassName());
				break;
			case multiname::NAME_UINT:
				throwError<RangeError>(kOutOfRangeError,name.normalizedName(getSystemState()),Integer::toString(vec.size()));
				break;
			default:
				break;
		}
		
		if (!ASObject::hasPropertyByMultiname(name,false,true))
			throwError<ReferenceError>(kWriteSealedError, name.normalizedName(getSystemState()), this->getClass()->getQualifiedClassName());
		return ASObject::setVariableByMultiname(name, o, allowConst,alreadyset);
	}
	asAtom v = o;
	if (this->vec_type->coerce(getSystemState(), o))
		ASATOM_DECREF(v);
	if(index < vec.size())
	{
		if (vec[index].uintval == o.uintval)
		{
			if (alreadyset)
				*alreadyset = true;
		}
		else
		{
			ASATOM_DECREF(vec[index]);
			vec[index] = o;
		}
	}
	else if(!fixed && index == vec.size())
	{
		vec.push_back( o );
	}
	else
	{
		/* Spec says: one may not set a value with an index more than
		 * one beyond the current final index. */
		throwError<RangeError>(kOutOfRangeError,
				       Integer::toString(index),
				       Integer::toString(vec.size()));
	}
	return nullptr;
}

void Vector::setVariableByInteger(int index, asAtom &o, ASObject::CONST_ALLOWED_FLAG allowConst)
{
	if (index < 0)
	{
		setVariableByInteger_intern(index,o,allowConst);
		return;
	}
	asAtom v = o;
	if (this->vec_type->coerce(getSystemState(), o))
		ASATOM_DECREF(v);
	if(size_t(index) < vec.size())
	{
		if (vec[index].uintval != o.uintval)
		{
			ASATOM_DECREF(vec[index]);
			vec[index] = o;
		}
	}
	else if(!fixed && size_t(index) == vec.size())
	{
		vec.push_back( o );
	}
	else
	{
		/* Spec says: one may not set a value with an index more than
		 * one beyond the current final index. */
		throwError<RangeError>(kOutOfRangeError,
				       Integer::toString(index),
				       Integer::toString(vec.size()));
	}
}

void Vector::throwRangeError(int index)
{
	/* Spec says: one may not set a value with an index more than
	 * one beyond the current final index. */
	throwError<RangeError>(kOutOfRangeError,
				   Integer::toString(index),
				   Integer::toString(vec.size()));
}

tiny_string Vector::toString()
{
	//TODO: test
	tiny_string t;
	for(size_t i = 0; i < vec.size(); ++i)
	{
		if( i )
			t += ",";
		t += asAtomHandler::toString(vec[i],getSystemState());
	}
	return t;
}

uint32_t Vector::nextNameIndex(uint32_t cur_index)
{
	if(cur_index < vec.size())
		return cur_index+1;
	else
		return 0;
}

void Vector::nextName(asAtom& ret,uint32_t index)
{
	if(index<=vec.size())
		asAtomHandler::setUInt(ret,this->getSystemState(),index-1);
	else
		throw RunTimeException("Vector::nextName out of bounds");
}

void Vector::nextValue(asAtom& ret,uint32_t index)
{
	if(index<=vec.size())
	{
		ASATOM_INCREF(vec[index-1]);
		ret = vec[index-1];
	}
	else
		throw RunTimeException("Vector::nextValue out of bounds");
}

bool Vector::isValidMultiname(SystemState* sys,const multiname& name, uint32_t& index, bool* isNumber)
{
	//First of all the multiname has to contain the null namespace
	//As the namespace vector is sorted, we check only the first one
	if(!name.hasEmptyNS)
		return false;

	bool validIndex=name.toUInt(sys,index, true,isNumber) && (index != UINT32_MAX);

	return validIndex;
}

tiny_string Vector::toJSON(std::vector<ASObject *> &path, asAtom replacer, const tiny_string &spaces, const tiny_string &filter)
{
	bool ok;
	tiny_string res = call_toJSON(ok,path,replacer,spaces,filter);
	if (ok)
		return res;
	// check for cylic reference
	if (std::find(path.begin(),path.end(), this) != path.end())
		throwError<TypeError>(kJSONCyclicStructure);

	path.push_back(this);
	res += "[";
	bool bfirst = true;
	tiny_string newline = (spaces.empty() ? "" : "\n");
	asAtom closure = asAtomHandler::isValid(replacer) && asAtomHandler::getClosure(replacer) ? asAtomHandler::fromObject(asAtomHandler::getClosure(replacer)) : asAtomHandler::nullAtom;
	for (unsigned int i =0;  i < vec.size(); i++)
	{
		tiny_string subres;
		asAtom o = vec[i];
		if (asAtomHandler::isValid(replacer))
		{
			asAtom params[2];
			
			params[0] = asAtomHandler::fromUInt(i);
			params[1] = o;
			asAtom funcret=asAtomHandler::invalidAtom;
			asAtomHandler::callFunction(replacer,funcret,closure, params, 2,false);
			if (asAtomHandler::isValid(funcret))
				subres = asAtomHandler::toObject(funcret,getSystemState())->toJSON(path,asAtomHandler::invalidAtom,spaces,filter);
		}
		else
		{
			subres = asAtomHandler::toObject(o,getSystemState())->toJSON(path,replacer,spaces,filter);
		}
		if (!subres.empty())
		{
			if (!bfirst)
				res += ",";
			res += newline+spaces;
			
			bfirst = false;
			res += subres;
		}
	}
	if (!bfirst)
		res += newline+spaces.substr_bytes(0,spaces.numBytes()/2);
	res += "]";
	path.pop_back();
	return res;
}

asAtom Vector::at(unsigned int index, asAtom defaultValue) const
{
	if (index < vec.size())
		return vec.at(index);
	else
		return defaultValue;
}

void Vector::serialize(ByteArray* out, std::map<tiny_string, uint32_t>& stringMap,
				std::map<const ASObject*, uint32_t>& objMap,
				std::map<const Class_base*, uint32_t>& traitsMap)
{
	if (out->getObjectEncoding() == ObjectEncoding::AMF0)
	{
		LOG(LOG_NOT_IMPLEMENTED,"serializing Vector in AMF0 not implemented");
		return;
	}
	uint8_t marker = 0;
	if (vec_type == Class<Integer>::getClass(getSystemState()))
		marker = vector_int_marker;
	else if (vec_type == Class<UInteger>::getClass(getSystemState()))
		marker = vector_uint_marker;
	else if (vec_type == Class<Number>::getClass(getSystemState()))
		marker = vector_double_marker;
	else
		marker = vector_object_marker;
	out->writeByte(marker);
	//Check if the vector has been already serialized
	auto it=objMap.find(this);
	if(it!=objMap.end())
	{
		//The least significant bit is 0 to signal a reference
		out->writeU29(it->second << 1);
	}
	else
	{
		//Add the Vector to the map
		objMap.insert(make_pair(this, objMap.size()));

		uint32_t count = size();
		assert_and_throw(count<0x20000000);
		uint32_t value = (count << 1) | 1;
		out->writeU29(value);
		out->writeByte(fixed ? 0x01 : 0x00);
		if (marker == vector_object_marker)
		{
			out->writeStringVR(stringMap,vec_type->getName());
		}
		for(uint32_t i=0;i<count;i++)
		{
			if (asAtomHandler::isInvalid(vec[i]))
			{
				//TODO should we write a null_marker here?
				LOG(LOG_NOT_IMPLEMENTED,"serialize unset vector objects");
				continue;
			}
			switch (marker)
			{
				case vector_int_marker:
					out->writeUnsignedInt(out->endianIn((uint32_t)asAtomHandler::toInt(vec[i])));
					break;
				case vector_uint_marker:
					out->writeUnsignedInt(out->endianIn(asAtomHandler::toUInt(vec[i])));
					break;
				case vector_double_marker:
					out->serializeDouble(asAtomHandler::toNumber(vec[i]));
					break;
				case vector_object_marker:
					asAtomHandler::toObject(vec[i],getSystemState())->serialize(out, stringMap, objMap, traitsMap);
					break;
			}
		}
	}
}
