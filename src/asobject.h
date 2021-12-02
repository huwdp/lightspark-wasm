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

#ifndef ASOBJECT_H
#define ASOBJECT_H 1

#include "swftypes.h"
#include "threading.h"
#include <unordered_map>
#include <limits>

#define ASFUNCTION_ATOM(name) \
	static void name(asAtom& ret,SystemState* sys, asAtom& , asAtom* args, const unsigned int argslen)

/* declare setter/getter and associated member variable */
#define ASPROPERTY_GETTER(type,name) \
	type name; \
	ASFUNCTION_ATOM( _getter_##name)

#define ASPROPERTY_GETTER_ATOM(name) \
	asAtom name=asAtomHandler::invalidAtom; \
	ASFUNCTION_ATOM( _getter_##name)

#define ASPROPERTY_SETTER(type,name) \
	type name; \
	ASFUNCTION_ATOM( _setter_##name)

#define ASPROPERTY_GETTER_SETTER(type, name) \
	type name; \
	ASFUNCTION_ATOM( _getter_##name); \
	ASFUNCTION_ATOM( _setter_##name)

#define ASPROPERTY_GETTER_SETTER_ATOM(name) \
	asAtom name=asAtomHandler::invalidAtom; \
	ASFUNCTION_ATOM( _getter_##name); \
	ASFUNCTION_ATOM( _setter_##name)

/* declare setter/getter for already existing member variable */
#define ASFUNCTION_GETTER(name) \
	ASFUNCTION_ATOM( _getter_##name)

#define ASFUNCTION_SETTER(name) \
	ASFUNCTION_ATOM( _setter_##name)

#define ASFUNCTION_GETTER_SETTER(name) \
	ASFUNCTION_ATOM( _getter_##name); \
	ASFUNCTION_ATOM( _setter_##name)

/* general purpose body for an AS function */
#define ASFUNCTIONBODY_ATOM(c,name) \
	void c::name(asAtom& ret, SystemState* sys, asAtom& obj, asAtom* args, const unsigned int argslen)

/* full body for a getter declared by ASPROPERTY_GETTER or ASFUNCTION_GETTER */
#define ASFUNCTIONBODY_GETTER(c,name) \
	void c::_getter_##name(asAtom& ret, SystemState* sys, asAtom& obj, asAtom* args, const unsigned int argslen) \
	{ \
		if(!asAtomHandler::is<c>(obj)) \
			throw Class<ArgumentError>::getInstanceS(sys,"Function applied to wrong object"); \
		c* th = asAtomHandler::as<c>(obj); \
		if(argslen != 0) \
			throw Class<ArgumentError>::getInstanceS(sys,"Arguments provided in getter"); \
		ArgumentConversionAtom<decltype(th->name)>::toAbstract(ret,sys,th->name); \
	}

#define ASFUNCTIONBODY_GETTER_STATIC(c,name) \
	void c::_getter_##name(asAtom& ret, SystemState* sys, asAtom& obj, asAtom* args, const unsigned int argslen) \
	{ \
		if(asAtomHandler::getObject(obj) != Class<c>::getRef(sys).getPtr()) \
			throw Class<ArgumentError>::getInstanceS(sys,"Function applied to wrong object"); \
		if(argslen != 0) \
			throw Class<ArgumentError>::getInstanceS(sys,"Arguments provided in getter"); \
		ArgumentConversionAtom<decltype(sys->static_##c##_##name)>::toAbstract(ret,sys,sys->static_##c##_##name); \
	}

#define ASFUNCTIONBODY_GETTER_STRINGID(c,name) \
	void c::_getter_##name(asAtom& ret, SystemState* sys, asAtom& obj, asAtom* args, const unsigned int argslen) \
	{ \
		if(!asAtomHandler::is<c>(obj)) \
			throw Class<ArgumentError>::getInstanceS(sys,"Function applied to wrong object"); \
		c* th = asAtomHandler::as<c>(obj); \
		if(argslen != 0) \
			throw Class<ArgumentError>::getInstanceS(sys,"Arguments provided in getter"); \
		ret = asAtomHandler::fromStringID(th->name); \
	}

#define ASFUNCTIONBODY_GETTER_NOT_IMPLEMENTED(c,name) \
	void c::_getter_##name(asAtom& ret, SystemState* sys, asAtom& obj, asAtom* args, const unsigned int argslen) \
	{ \
		if(!asAtomHandler::is<c>(obj)) \
			throw Class<ArgumentError>::getInstanceS(sys,"Function applied to wrong object"); \
		c* th = asAtomHandler::as<c>(obj); \
		if(argslen != 0) \
			throw Class<ArgumentError>::getInstanceS(sys,"Arguments provided in getter"); \
		LOG(LOG_NOT_IMPLEMENTED,asAtomHandler::getObject(obj)->getClassName() <<"."<< #name << " getter is not implemented"); \
		ArgumentConversionAtom<decltype(th->name)>::toAbstract(ret,sys,th->name); \
	}

/* full body for a getter declared by ASPROPERTY_SETTER or ASFUNCTION_SETTER */
#define ASFUNCTIONBODY_SETTER(c,name) \
	void c::_setter_##name(asAtom& ret,SystemState* sys, asAtom& obj, asAtom* args, const unsigned int argslen) \
	{ \
		if(!asAtomHandler::is<c>(obj)) \
			throw Class<ArgumentError>::getInstanceS(sys,"Function applied to wrong object"); \
		c* th = asAtomHandler::as<c>(obj); \
		if(argslen != 1) \
			throw Class<ArgumentError>::getInstanceS(sys,"Arguments provided in getter"); \
		th->name = ArgumentConversionAtom<decltype(th->name)>::toConcrete(sys,args[0],th->name); \
	}
#define ASFUNCTIONBODY_SETTER_STATIC(c,name) \
	void c::_setter_##name(asAtom& ret,SystemState* sys, asAtom& obj, asAtom* args, const unsigned int argslen) \
	{ \
		if(asAtomHandler::getObject(obj) != Class<c>::getRef(sys).getPtr()) \
			throw Class<ArgumentError>::getInstanceS(sys,"Function applied to wrong object"); \
		if(argslen != 1) \
			throw Class<ArgumentError>::getInstanceS(sys,"Arguments provided in getter"); \
		sys->static_##c##_##name = ArgumentConversionAtom<decltype(sys->static_##c##_##name)>::toConcrete(sys,args[0],sys->static_##c##_##name); \
	}

#define ASFUNCTIONBODY_SETTER_STRINGID(c,name) \
	void c::_setter_##name(asAtom& ret, SystemState* sys, asAtom& obj, asAtom* args, const unsigned int argslen) \
	{ \
		if(!asAtomHandler::is<c>(obj)) \
			throw Class<ArgumentError>::getInstanceS(sys,"Function applied to wrong object"); \
		c* th = asAtomHandler::as<c>(obj); \
		if(argslen != 1) \
			throw Class<ArgumentError>::getInstanceS(sys,"Arguments provided in getter"); \
		th->name = asAtomHandler::toStringId(args[0],sys); \
	}

#define ASFUNCTIONBODY_SETTER_NOT_IMPLEMENTED(c,name) \
	void c::_setter_##name(asAtom& ret, SystemState* sys, asAtom& obj, asAtom* args, const unsigned int argslen) \
	{ \
		if(!asAtomHandler::is<c>(obj)) \
			throw Class<ArgumentError>::getInstanceS(sys,"Function applied to wrong object"); \
		c* th = asAtomHandler::as<c>(obj); \
		if(argslen != 1) \
			throw Class<ArgumentError>::getInstanceS(sys,"Arguments provided in getter"); \
		LOG(LOG_NOT_IMPLEMENTED,asAtomHandler::getObject(obj)->getClassName() <<"."<< #name << " setter is not implemented"); \
		th->name = ArgumentConversionAtom<decltype(th->name)>::toConcrete(sys,args[0],th->name); \
	}

/* full body for a getter declared by ASPROPERTY_SETTER or ASFUNCTION_SETTER.
 * After the property has been updated, the callback member function is called with the old value
 * as parameter */
#define ASFUNCTIONBODY_SETTER_CB(c,name,callback) \
	void c::_setter_##name(asAtom& ret, SystemState* sys, asAtom& obj, asAtom* args, const unsigned int argslen) \
	{ \
		if(!asAtomHandler::is<c>(obj)) \
			throw Class<ArgumentError>::getInstanceS(sys,"Function applied to wrong object"); \
		c* th = asAtomHandler::as<c>(obj); \
		if(argslen != 1) \
			throw Class<ArgumentError>::getInstanceS(sys,"Arguments provided in getter"); \
		decltype(th->name) oldValue = th->name; \
		th->name = ArgumentConversionAtom<decltype(th->name)>::toConcrete(sys,args[0],th->name); \
		th->callback(oldValue); \
	}

#define ASFUNCTIONBODY_SETTER_STRINGID_CB(c,name,callback) \
	void c::_setter_##name(asAtom& ret, SystemState* sys, asAtom& obj, asAtom* args, const unsigned int argslen) \
	{ \
		if(!asAtomHandler::is<c>(obj)) \
			throw Class<ArgumentError>::getInstanceS(sys,"Function applied to wrong object"); \
		c* th = asAtomHandler::as<c>(obj); \
		if(argslen != 1) \
			throw Class<ArgumentError>::getInstanceS(sys,"Arguments provided in getter"); \
		decltype(th->name) oldValue = th->name; \
		th->name = asAtomHandler::toStringId(args[0],sys); \
		th->callback(oldValue); \
	}

/* full body for a getter declared by ASPROPERTY_GETTER_SETTER or ASFUNCTION_GETTER_SETTER */
#define ASFUNCTIONBODY_GETTER_SETTER(c,name) \
		ASFUNCTIONBODY_GETTER(c,name) \
		ASFUNCTIONBODY_SETTER(c,name)

#define ASFUNCTIONBODY_GETTER_SETTER_NOT_IMPLEMENTED(c,name) \
		ASFUNCTIONBODY_GETTER_NOT_IMPLEMENTED(c,name) \
		ASFUNCTIONBODY_SETTER_NOT_IMPLEMENTED(c,name)

#define ASFUNCTIONBODY_GETTER_SETTER_CB(c,name,callback) \
		ASFUNCTIONBODY_GETTER(c,name) \
		ASFUNCTIONBODY_SETTER_CB(c,name,callback)

#define ASFUNCTIONBODY_GETTER_SETTER_STATIC(c,name) \
		ASFUNCTIONBODY_GETTER_STATIC(c,name) \
		ASFUNCTIONBODY_SETTER_STATIC(c,name)

#define ASFUNCTIONBODY_GETTER_SETTER_STRINGID(c,name) \
		ASFUNCTIONBODY_GETTER_STRINGID(c,name) \
		ASFUNCTIONBODY_SETTER_STRINGID(c,name)

#define ASFUNCTIONBODY_GETTER_SETTER_STRINGID_CB(c,name,callback) \
		ASFUNCTIONBODY_GETTER_STRINGID(c,name) \
		ASFUNCTIONBODY_SETTER_STRINGID_CB(c,name,callback)

/* registers getter/setter with Class_base. To be used in ::sinit()-functions */
#define REGISTER_GETTER_RESULTTYPE(c,name,cls) \
	c->setDeclaredMethodByQName(#name,"",Class<IFunction>::getFunction(c->getSystemState(),_getter_##name,0,Class<cls>::getRef(c->getSystemState()).getPtr()),GETTER_METHOD,true)

#define REGISTER_GETTER_SETTER_RESULTTYPE(c,name,cls) \
		REGISTER_GETTER_RESULTTYPE(c,name,cls); \
		REGISTER_SETTER(c,name)

#define REGISTER_GETTER(c,name) \
	c->setDeclaredMethodByQName(#name,"",Class<IFunction>::getFunction(c->getSystemState(),_getter_##name),GETTER_METHOD,true)

#define REGISTER_SETTER(c,name) \
	c->setDeclaredMethodByQName(#name,"",Class<IFunction>::getFunction(c->getSystemState(),_setter_##name),SETTER_METHOD,true)

#define REGISTER_GETTER_SETTER(c,name) \
		REGISTER_GETTER(c,name); \
		REGISTER_SETTER(c,name)

#define REGISTER_GETTER_STATIC(c,name) \
	c->setDeclaredMethodByQName(#name,"",Class<IFunction>::getFunction(c->getSystemState(),_getter_##name),GETTER_METHOD,false)

#define REGISTER_SETTER_STATIC(c,name) \
	c->setDeclaredMethodByQName(#name,"",Class<IFunction>::getFunction(c->getSystemState(),_setter_##name),SETTER_METHOD,false)

#define REGISTER_GETTER_SETTER_STATIC(c,name) \
		REGISTER_GETTER_STATIC(c,name); \
		REGISTER_SETTER_STATIC(c,name)

#define CLASS_DYNAMIC_NOT_FINAL 0
#define CLASS_FINAL 1
#define CLASS_SEALED 2

// TODO: Every class should have a constructor
#define CLASS_SETUP_NO_CONSTRUCTOR(c, superClass, attributes) \
	c->setSuper(Class<superClass>::getRef(c->getSystemState())); \
	c->setConstructor(NULL); \
	c->isFinal = ((attributes) & CLASS_FINAL) != 0;	\
	c->isSealed = ((attributes) & CLASS_SEALED) != 0

#define CLASS_SETUP(c, superClass, constructor, attributes) \
	CLASS_SETUP_NO_CONSTRUCTOR(c, superClass, attributes); \
	c->setConstructor(Class<IFunction>::getFunction(c->getSystemState(),constructor));

#define CLASS_SETUP_CONSTRUCTOR_LENGTH(c, superClass, constructor, ctorlength, attributes) \
	CLASS_SETUP_NO_CONSTRUCTOR(c, superClass, attributes); \
	c->setConstructor(Class<IFunction>::getFunction(c->getSystemState(),(constructor), (ctorlength)));

using namespace std;
namespace lightspark
{

class ASObject;
class IFunction;
template<class T> class Class;
class Class_base;
class ByteArray;
class Loader;
class Type;
class ABCContext;
class SystemState;
class SyntheticFunction;
class SoundTransform;
class KeyboardEvent;
class EventDispatcher;
class MouseEvent;
class Event;

#define FREELIST_SIZE 16
struct asfreelist
{
	ASObject* freelist[FREELIST_SIZE];
	int freelistsize;
	asfreelist():freelistsize(0) {}
	~asfreelist();

	inline ASObject* getObjectFromFreeList();
	inline bool pushObjectToFreeList(ASObject *obj);
};

extern SystemState* getSys();
enum TRAIT_KIND { NO_CREATE_TRAIT=0, DECLARED_TRAIT=1, DYNAMIC_TRAIT=2, INSTANCE_TRAIT=5, CONSTANT_TRAIT=9 /* constants are also declared traits */ };
enum GET_VARIABLE_RESULT {GETVAR_NORMAL=0x00, GETVAR_CACHEABLE=0x01, GETVAR_ISGETTER=0x02, GETVAR_ISCONSTANT=0x04, GETVAR_ISNEWOBJECT=0x08};
enum GET_VARIABLE_OPTION {NONE=0x00, SKIP_IMPL=0x01, FROM_GETLEX=0x02, DONT_CALL_GETTER=0x04, NO_INCREF=0x08, DONT_CHECK_CLASS=0x10};

#ifdef LIGHTSPARK_64
union asAtom
{
	int64_t intval;
	uint64_t uintval;
};
#define LIGHTSPARK_ATOM_VALTYPE uint64_t
#else
union asAtom
{
	int32_t intval;
	uint32_t uintval;
};
#define LIGHTSPARK_ATOM_VALTYPE uint32_t
#endif
// atom is a 32bit value (64bit on 64bit architecture):
// malloc guarantees that the returned pointer is always a multiple of eight
// so we can use the lower 3 bits to indicate the type
// layout of the least significant byte:
// d: data
// p: indicates that data is pointer to ASObject
// t: defines type of atom
// dddd dptt
// 0000 0000: invalid
// 0100 0000: null
// 0010 0000: undefined
// d001 0000: bool (d indicates true/false)
// dddd d001: uint
// dddd d101: Number
// dddd d010: stringID
// dddd d110: ASString
// dddd d011: int
// dddd d111: (U)Integer
// dddd d100: ASObject
enum ATOM_TYPE 
{ 
	ATOM_INVALID_UNDEFINED_NULL_BOOL=0x0, 
	ATOM_UINTEGER=0x1, 
	ATOM_STRINGID=0x2,
	ATOM_INTEGER=0x3, 
	ATOM_OBJECTPTR=0x4, 
	ATOM_NUMBERPTR=0x5,
	ATOM_STRINGPTR=0x6, 
	ATOM_U_INTEGERPTR=0x7
};

class asAtomHandler
{
private:
#define ATOMTYPE_NULL_BIT 0x40
#define ATOMTYPE_UNDEFINED_BIT 0x20
#define ATOMTYPE_BOOL_BIT 0x10
#define ATOMTYPE_OBJECT_BIT 0x4
	static void decRef(asAtom& a);
	static void replaceBool(asAtom &a, ASObject* obj);
	static bool Boolean_concrete_string(asAtom &a);
	static TRISTATE isLessIntern(asAtom& a,SystemState *sys, asAtom& v2);
	static bool isEqualIntern(asAtom& a,SystemState *sys, asAtom& v2);
public:
	static FORCE_INLINE asAtom fromType(SWFOBJECT_TYPE _t)
	{
		asAtom a=asAtomHandler::invalidAtom;
		switch (_t)
		{
			case T_INVALID:
				a.uintval=ATOM_INVALID_UNDEFINED_NULL_BOOL;
				break;
			case T_UNDEFINED:
				a.uintval=ATOM_INVALID_UNDEFINED_NULL_BOOL | ATOMTYPE_UNDEFINED_BIT;
				break;
			case T_NULL:
				a.uintval=ATOM_INVALID_UNDEFINED_NULL_BOOL | ATOMTYPE_NULL_BIT;
				break;
			case T_BOOLEAN:
				a.uintval=ATOM_INVALID_UNDEFINED_NULL_BOOL | ATOMTYPE_BOOL_BIT;
				break;
			case T_NUMBER:
				a.uintval=ATOM_NUMBERPTR;
				break;
			case T_INTEGER:
				a.uintval=ATOM_INTEGER;
				break;
			case T_UINTEGER:
				a.uintval=ATOM_UINTEGER;
				break;
			case T_STRING:
				a.uintval=ATOM_STRINGID;
				break;
			default:
				a.uintval=ATOM_OBJECTPTR;
				break;
		}
		return a;
	}
	static FORCE_INLINE asAtom fromInt(int32_t val)
	{
		asAtom a=asAtomHandler::invalidAtom;
#ifdef LIGHTSPARK_64
		a.intval = ((((int64_t)val)<<3)|ATOM_INTEGER);
#else
		a.intval = ((val<<3)|ATOM_INTEGER);
		if (val <-(1<<28)  && val > (1<<28))
			a.uintval =((LIGHTSPARK_ATOM_VALTYPE)(abstract_d(getSys(),val))|ATOM_NUMBERPTR);
#endif
		return a;
	}
	static FORCE_INLINE asAtom fromUInt(uint32_t val)
	{
		asAtom a=asAtomHandler::invalidAtom;
#ifdef LIGHTSPARK_64
		a.uintval = ((((uint64_t)val)<<3)|ATOM_UINTEGER);
#else
		a.uintval =((val<<3)|ATOM_UINTEGER);
		if (val >= (1<<29))
			a.uintval =((LIGHTSPARK_ATOM_VALTYPE)(abstract_d(getSys(),val))|ATOM_NUMBERPTR);
#endif
		return a;
	}
	static FORCE_INLINE asAtom fromNumber(SystemState* sys, number_t val,bool constant)
	{
		asAtom a=asAtomHandler::invalidAtom;
		a.uintval =((LIGHTSPARK_ATOM_VALTYPE)(constant ? abstract_d_constant(sys,val) : abstract_d(sys,val))|ATOM_NUMBERPTR);
		return a;
	}
	
	static FORCE_INLINE asAtom fromBool(bool val)
	{
		asAtom a=asAtomHandler::invalidAtom;
		a.uintval =(ATOM_INVALID_UNDEFINED_NULL_BOOL | ATOMTYPE_BOOL_BIT | (val ? 0x80 : 0));
		return a;
	}
	static ASObject* toObject(asAtom& a, SystemState* sys,bool isconstant=false);
	// returns NULL if this atom is a primitive;
	static FORCE_INLINE ASObject* getObject(const asAtom& a);
	static FORCE_INLINE ASObject* getObjectNoCheck(const asAtom& a);
	static FORCE_INLINE void resetCached(const asAtom& a);
	static FORCE_INLINE asAtom fromObject(ASObject* obj)
	{
		asAtom a=asAtomHandler::invalidAtom;
		if (obj)
			replace(a,obj);
		return a;
	}
	static FORCE_INLINE asAtom fromObjectNoPrimitive(ASObject* obj);
	static FORCE_INLINE asAtom fromStringID(uint32_t sID)
	{
#ifndef LIGHTSPARK_64
		assert(sID <(1<<29));
#endif
		asAtom a=asAtomHandler::invalidAtom;
		a.uintval= (sID<<3)|ATOM_STRINGID;
		return a;
	}
	// only use this for strings that should get an internal stringID
	static asAtom fromString(SystemState *sys, const tiny_string& s);
	static ASObject* getClosure(asAtom& a);
	static asAtom getClosureAtom(asAtom& a, asAtom defaultAtom=asAtomHandler::nullAtom);
	static asAtom undefinedAtom;
	static asAtom nullAtom;
	static asAtom invalidAtom;
	static asAtom trueAtom;
	static asAtom falseAtom;
	/*
	 * Calls this function with the given object and args.
	 * if args_refcounted is true, one reference of obj and of each arg is consumed by this method.
	 * Return the asAtom the function returned.
	 * if coerceresult is false, the result of the function will not be coerced into the type provided by the method_info
	 */
	static void callFunction(asAtom& caller, asAtom& ret, asAtom &obj, asAtom *args, uint32_t num_args, bool args_refcounted, bool coerceresult=true, bool coercearguments=true);
	// returns invalidAtom for not-primitive values
	static void getVariableByMultiname(asAtom& a, asAtom &ret, SystemState *sys, const multiname& name);
	static Class_base* getClass(asAtom& a,SystemState *sys, bool followclass=true);
	static bool canCacheMethod(asAtom& a,const multiname* name);
	static void fillMultiname(asAtom& a,SystemState *sys, multiname& name);
	static void replace(asAtom& a,ASObject* obj);
	static FORCE_INLINE void set(asAtom& a,const asAtom& b)
	{
		a.uintval = b.uintval;
	}
	static bool stringcompare(asAtom& a, SystemState* sys, uint32_t stringID);
	static bool functioncompare(asAtom& a, SystemState* sys,asAtom& v2);
	static std::string toDebugString(asAtom& a);
	static FORCE_INLINE void applyProxyProperty(asAtom& a,SystemState *sys, multiname& name);
	static FORCE_INLINE TRISTATE isLess(asAtom& a,SystemState *sys, asAtom& v2);
	static FORCE_INLINE bool isEqual(asAtom& a,SystemState *sys, asAtom& v2);
	static FORCE_INLINE bool isEqualStrict(asAtom& a,SystemState *sys, asAtom& v2);
	static FORCE_INLINE bool isConstructed(const asAtom& a);
	static FORCE_INLINE bool isPrimitive(const asAtom& a);
	static FORCE_INLINE bool isNumeric(const asAtom& a); 
	static FORCE_INLINE bool isNumber(const asAtom& a); 
	static FORCE_INLINE bool isValid(const asAtom& a) { return a.uintval; }
	static FORCE_INLINE bool isInvalid(const asAtom& a) { return !a.uintval; }
	static FORCE_INLINE bool isNull(const asAtom& a) { return (a.uintval&0x7f) == ATOMTYPE_NULL_BIT; }
	static FORCE_INLINE bool isUndefined(const asAtom& a) { return (a.uintval&0x7f) == ATOMTYPE_UNDEFINED_BIT; }
	static FORCE_INLINE bool isBool(const asAtom& a) { return (a.uintval&0x7f) == ATOMTYPE_BOOL_BIT; }
	static FORCE_INLINE bool isInteger(const asAtom& a);
	static FORCE_INLINE bool isUInteger(const asAtom& a);
	static FORCE_INLINE bool isObject(const asAtom& a) { return a.uintval & ATOMTYPE_OBJECT_BIT; }
	static FORCE_INLINE bool isFunction(const asAtom& a);
	static FORCE_INLINE bool isString(const asAtom& a);
	static FORCE_INLINE bool isStringID(const asAtom& a) { return (a.uintval&0x7) == ATOM_STRINGID; }
	static FORCE_INLINE bool isQName(const asAtom& a);
	static FORCE_INLINE bool isNamespace(const asAtom& a);
	static FORCE_INLINE bool isArray(const asAtom& a);
	static FORCE_INLINE bool isClass(const asAtom& a);
	static FORCE_INLINE bool isTemplate(const asAtom& a);
	static FORCE_INLINE SWFOBJECT_TYPE getObjectType(const asAtom& a);
	static FORCE_INLINE bool checkArgumentConversion(const asAtom& a,const asAtom& obj);
	static asAtom asTypelate(asAtom& a, asAtom& b);
	static bool isTypelate(asAtom& a,ASObject* type);
	static bool isTypelate(asAtom& a,asAtom& t);
	static FORCE_INLINE number_t toNumber(const asAtom& a);
	static FORCE_INLINE number_t AVM1toNumber(asAtom& a, uint32_t swfversion);
	static FORCE_INLINE bool AVM1toBool(asAtom& a);
	static FORCE_INLINE int32_t toInt(const asAtom& a);
	static FORCE_INLINE int32_t toIntStrict(const asAtom& a);
	static FORCE_INLINE int64_t toInt64(const asAtom& a);
	static FORCE_INLINE uint32_t toUInt(asAtom& a);
	static tiny_string toString(const asAtom &a, SystemState *sys);
	static tiny_string toLocaleString(const asAtom &a);
	static uint32_t toStringId(asAtom &a, SystemState *sys);
	static FORCE_INLINE asAtom typeOf(asAtom& a);
	static bool Boolean_concrete(asAtom& a);
	static bool Boolean_concrete_object(asAtom& a);
	static void convert_b(asAtom& a, bool refcounted);
	static FORCE_INLINE int32_t getInt(const asAtom& a) { assert((a.uintval&0x3) == ATOM_INTEGER || (a.uintval&0x3) == ATOM_UINTEGER); return a.intval>>3; }
	static FORCE_INLINE uint32_t getUInt(const asAtom& a) { assert((a.uintval&0x3) == ATOM_UINTEGER || (a.uintval&0x3) == ATOM_INTEGER); return a.uintval>>3; }
	static FORCE_INLINE uint32_t getStringId(const asAtom& a) { assert((a.uintval&0x3) == ATOM_STRINGID); return a.uintval>>3; }
	static FORCE_INLINE void setInt(asAtom& a,SystemState* sys, int64_t val);
	static FORCE_INLINE void setUInt(asAtom& a,SystemState* sys, uint32_t val);
	static void setNumber(asAtom& a,SystemState* sys,number_t val);
	static bool replaceNumber(asAtom& a,SystemState* sys,number_t val);
	static FORCE_INLINE void setBool(asAtom& a,bool val);
	static FORCE_INLINE void setNull(asAtom& a);
	static FORCE_INLINE void setUndefined(asAtom& a);
	static void setFunction(asAtom& a,ASObject* obj, ASObject* closure);
	static FORCE_INLINE void increment(asAtom& a,SystemState* sys);
	static FORCE_INLINE void decrement(asAtom& a,SystemState* sys);
	static FORCE_INLINE void increment_i(asAtom& a,SystemState* sys);
	static FORCE_INLINE void decrement_i(asAtom& a,SystemState* sys);
	static bool add(asAtom& a,asAtom& v2, SystemState *sys,bool forceint);
	static void addreplace(asAtom& ret, SystemState* sys, asAtom &v1, asAtom &v2,bool forceint);
	static FORCE_INLINE void bitnot(asAtom& a,SystemState* sys);
	static FORCE_INLINE void subtract(asAtom& a,SystemState* sys,asAtom& v2,bool forceint);
	static FORCE_INLINE void subtractreplace(asAtom& ret, SystemState* sys,const asAtom& v1, const asAtom& v2,bool forceint);
	static FORCE_INLINE void multiply(asAtom& a,SystemState* sys,asAtom& v2,bool forceint);
	static FORCE_INLINE void multiplyreplace(asAtom& ret, SystemState* sys,const asAtom& v1, const asAtom &v2,bool forceint);
	static FORCE_INLINE void divide(asAtom& a,SystemState* sys,asAtom& v2,bool forceint);
	static FORCE_INLINE void dividereplace(asAtom& ret, SystemState* sys,const asAtom& v1, const asAtom& v2,bool forceint);
	static FORCE_INLINE void modulo(asAtom& a,SystemState* sys,asAtom& v2);
	static FORCE_INLINE void moduloreplace(asAtom& ret, SystemState* sys,const asAtom& v1, const asAtom &v2);
	static FORCE_INLINE void lshift(asAtom& a,SystemState* sys,asAtom& v1);
	static FORCE_INLINE void rshift(asAtom& a,SystemState* sys,asAtom& v1);
	static FORCE_INLINE void urshift(asAtom& a,SystemState* sys,asAtom& v1);
	static FORCE_INLINE void bit_and(asAtom& a,SystemState* sys,asAtom& v1);
	static FORCE_INLINE void bit_or(asAtom& a,SystemState* sys,asAtom& v1);
	static FORCE_INLINE void bit_xor(asAtom& a,SystemState* sys,asAtom& v1);
	static FORCE_INLINE void _not(asAtom& a);
	static FORCE_INLINE void negate_i(asAtom& a,SystemState* sys);
	static FORCE_INLINE void add_i(asAtom& a,SystemState* sys,asAtom& v2);
	static FORCE_INLINE void subtract_i(asAtom& a,SystemState* sys,asAtom& v2);
	static FORCE_INLINE void multiply_i(asAtom& a,SystemState* sys,asAtom& v2);
	template<class T> static bool is(asAtom& a);
	template<class T> static T* as(asAtom& a) 
	{ 
		assert(isObject(a));
		return static_cast<T*>((void*)(a.uintval& ~((LIGHTSPARK_ATOM_VALTYPE)0x7)));
	}
};
#define ASATOM_INCREF(a) if (asAtomHandler::isObject(a)) asAtomHandler::getObjectNoCheck(a)->incRef()
#define ASATOM_INCREF_POINTER(a) if (asAtomHandler::isObject(*a)) asAtomHandler::getObjectNoCheck(*a)->incRef()
#define ASATOM_DECREF(a) if (asAtomHandler::isObject(a)) { ASObject* obj_b = asAtomHandler::getObjectNoCheck(a); if (!obj_b->getConstant() && !obj_b->getInDestruction()) obj_b->decRef(); }
#define ASATOM_DECREF_POINTER(a) if (asAtomHandler::isObject(*a)) { ASObject* obj_b = asAtomHandler::getObject(*a); if (obj_b && !obj_b->getConstant() && !obj_b->getInDestruction()) obj_b->decRef(); }
struct variable
{
	asAtom var;
	union
	{
		multiname* traitTypemname;
		const Type* type;
		void* typeUnion;
	};
	asAtom setter;
	asAtom getter;
	nsNameAndKind ns;
	uint32_t slotid;
	TRAIT_KIND kind:4;
	bool isResolved:1;
	bool isenumerable:1;
	bool issealed:1;
	bool isrefcounted:1;
	variable(TRAIT_KIND _k,const nsNameAndKind& _ns)
		: var(asAtomHandler::invalidAtom),typeUnion(nullptr),setter(asAtomHandler::invalidAtom),getter(asAtomHandler::invalidAtom),ns(_ns),slotid(0),kind(_k),isResolved(false),isenumerable(true),issealed(false),isrefcounted(true) {}
	variable(TRAIT_KIND _k, asAtom _v, multiname* _t, const Type* type, const nsNameAndKind &_ns, bool _isenumerable);
	void setVar(asAtom v, ASObject* obj, bool _isrefcounted = true);
	/*
	 * To be used only if the value is guaranteed to be of the right type
	 */
	FORCE_INLINE void setVarNoCoerce(asAtom &v, ASObject *obj)
	{
		if(isrefcounted && asAtomHandler::isObject(var))
			preparereplacevar(obj);
		var=v;
		isrefcounted = asAtomHandler::isObject(v);
	}
	void preparereplacevar(ASObject *obj);
	
	void setResultType(const Type* t)
	{
		isResolved=true;
		type=t;
	}
};

struct varName
{
	uint32_t nameId;
	nsNameAndKind ns;
	varName():nameId(0){}
	varName(uint32_t name, const nsNameAndKind& _ns):nameId(name),ns(_ns){}
	inline bool operator<(const varName& r) const
	{
		//Sort by name first
		if(nameId==r.nameId)
		{
			//Then by namespace
			return ns<r.ns;
		}
		else
			return nameId<r.nameId;
	}
	inline bool operator==(const varName& r) const
	{
		return nameId==r.nameId && ns == r.ns;
	}
};

class variables_map
{
public:
	//Names are represented by strings in the string and namespace pools
	typedef std::unordered_multimap<uint32_t,variable> mapType;
	mapType Variables;
	typedef std::unordered_multimap<uint32_t,variable>::iterator var_iterator;
	typedef std::unordered_multimap<uint32_t,variable>::const_iterator const_var_iterator;
	std::vector<variable*> slots_vars;
	uint32_t slotcount;
	// indicates if this map was initialized with no variables with non-primitive values
	bool cloneable;
	variables_map(MemoryAccount* m);
	/**
	   Find a variable in the map

	   @param createKind If this is different from NO_CREATE_TRAIT and no variable is found
				a new one is created with the given kind
	   @param traitKinds Bitwise OR of accepted trait kinds
	*/
	variable* findObjVar(uint32_t nameId, const nsNameAndKind& ns, TRAIT_KIND createKind, uint32_t traitKinds);
	variable* findObjVar(SystemState* sys,const multiname& mname, TRAIT_KIND createKind, uint32_t traitKinds);
	// adds a dynamic variable without checking if a variable with this name already exists
	FORCE_INLINE void setDynamicVarNoCheck(uint32_t nameID,asAtom& v)
	{
		var_iterator inserted=Variables.insert(Variables.cbegin(),
				make_pair(nameID,variable(DYNAMIC_TRAIT,nsNameAndKind())));
		asAtomHandler::set(inserted->second.var,v);
	}

	/**
	 * Const version of findObjVar, useful when looking for getters
	 */
	FORCE_INLINE const variable* findObjVarConst(SystemState* sys,const multiname& mname, uint32_t traitKinds, uint32_t* nsRealId = nullptr) const
	{
		if (mname.isEmpty())
			return nullptr;
		uint32_t name=mname.name_type == multiname::NAME_STRING ? mname.name_s_id : mname.normalizedNameId(sys);
		bool noNS = mname.ns.empty(); // no Namespace in multiname means we don't care about the namespace and take the first match
		const_var_iterator ret=Variables.find(name);
		auto nsIt=mname.ns.cbegin();
		//Find the namespace
		while(ret!=Variables.cend() && ret->first==name)
		{
			//breaks when the namespace is not found
			const nsNameAndKind& ns=ret->second.ns;
			if(noNS || ns==*nsIt || (mname.hasEmptyNS && ns.hasEmptyName()) || (mname.hasBuiltinNS && ns.hasBuiltinName()))
			{
				if(ret->second.kind & traitKinds)
				{
					if (nsRealId)
						*nsRealId = ns.nsRealId;
					return &ret->second;
				}
				else
					return nullptr;
			}
			else
			{
				++nsIt;
				if(nsIt==mname.ns.cend())
				{
					nsIt=mname.ns.cbegin();
					++ret;
				}
			}
		}
	
		return nullptr;
	}

	//
	FORCE_INLINE variable* findObjVar(SystemState* sys,const multiname& mname, uint32_t traitKinds, uint32_t* nsRealId = nullptr)
	{
		if (mname.isEmpty())
			return nullptr;
		uint32_t name=mname.name_type == multiname::NAME_STRING ? mname.name_s_id : mname.normalizedNameId(sys);
		bool noNS = mname.ns.empty(); // no Namespace in multiname means we don't care about the namespace and take the first match

		var_iterator ret=Variables.find(name);
		auto nsIt=mname.ns.cbegin();
		//Find the namespace
		while(ret!=Variables.cend() && ret->first==name)
		{
			//breaks when the namespace is not found
			const nsNameAndKind& ns=ret->second.ns;
			if(noNS || ns==*nsIt || (mname.hasEmptyNS && ns.hasEmptyName()) || (mname.hasBuiltinNS && ns.hasBuiltinName()))
			{
				if(ret->second.kind & traitKinds)
				{
					if (nsRealId)
						*nsRealId = ns.nsRealId;
					return &ret->second;
				}
				else
					return nullptr;
			}
			else
			{
				++nsIt;
				if(nsIt==mname.ns.cend())
				{
					nsIt=mname.ns.cbegin();
					++ret;
				}
			}
		}

		return nullptr;
	}
	
	//Initialize a new variable specifying the type (TODO: add support for const)
	void initializeVar(multiname &mname, asAtom &obj, multiname *typemname, ABCContext* context, TRAIT_KIND traitKind, ASObject* mainObj, uint32_t slot_id, bool isenumerable);
	void killObjVar(SystemState* sys, const multiname& mname);
	FORCE_INLINE asAtom getSlot(unsigned int n)
	{
		assert_and_throw(n > 0 && n <= slotcount);
		return slots_vars[n-1]->var;
	}
	FORCE_INLINE variable* getSlotVar(unsigned int n)
	{
		return slots_vars[n-1];
	}
	FORCE_INLINE asAtom getSlotNoCheck(unsigned int n)
	{
		return slots_vars[n]->var;
	}
	FORCE_INLINE TRAIT_KIND getSlotKind(unsigned int n)
	{
		assert_and_throw(n > 0 && n <= slotcount);
		return slots_vars[n-1]->kind;
	}
	Class_base* getSlotType(unsigned int n);
	
	uint32_t findInstanceSlotByMultiname(multiname* name, SystemState *sys);
	FORCE_INLINE bool setSlot(unsigned int n, asAtom &o, ASObject* obj);
	/*
	 * This version of the call is guarantee to require no type conversion
	 * this is verified at optimization time
	 */
	FORCE_INLINE bool setSlotNoCoerce(unsigned int n, asAtom o, ASObject* obj)
	{
		assert_and_throw(n < slotcount);
		if (slots_vars[n]->var.uintval != o.uintval)
		{
			slots_vars[n]->setVarNoCoerce(o,obj);
			return asAtomHandler::isObject(o);
		}
		return false;
	}
	FORCE_INLINE void initSlot(unsigned int n, variable *v)
	{
		if (n>slots_vars.capacity())
			slots_vars.reserve(n+8);
		if(n>slotcount)
		{
			slots_vars.resize(n);
			slotcount= n;
		}
		v->slotid = n;
		slots_vars[n-1]=v;
	}
	FORCE_INLINE  unsigned int size() const
	{
		return Variables.size();
	}
	uint32_t getNameAt(unsigned int i) const;
	variable* getValueAt(unsigned int i);
	int getNextEnumerable(unsigned int i) const;
	~variables_map();
	void check() const;
	void serialize(ByteArray* out, std::map<tiny_string, uint32_t>& stringMap,
				std::map<const ASObject*, uint32_t>& objMap,
				std::map<const Class_base*, uint32_t>& traitsMap);
	void dumpVariables();
	void destroyContents();
	bool cloneInstance(variables_map& map);
	void removeAllDeclaredProperties();
};

enum METHOD_TYPE { NORMAL_METHOD=0, SETTER_METHOD=1, GETTER_METHOD=2 };
//for toPrimitive
enum TP_HINT { NO_HINT, NUMBER_HINT, STRING_HINT };

class ASObject: public memory_reporter, public RefCountable
{
friend class ABCVm;
friend class ABCContext;
friend class Class_base; //Needed for forced cleanup
friend class Class_inherit; 
friend void lookupAndLink(Class_base* c, const tiny_string& name, const tiny_string& interfaceNs);
friend class IFunction; //Needed for clone
friend struct asfreelist;
friend class SystemState;
friend struct variable;
friend class variables_map;
friend class RootMovieClip;
public:
	asfreelist* objfreelist;
private:
	variables_map Variables;
	Class_base* classdef;
	inline const variable* findGettable(const multiname& name, uint32_t* nsRealId = nullptr) const DLL_LOCAL
	{
		const variable* ret=Variables.findObjVarConst(getSystemState(),name,DECLARED_TRAIT|DYNAMIC_TRAIT,nsRealId);
		if(ret)
		{
			//It seems valid for a class to redefine only the setter, so if we can't find
			//something to get, it's ok
			if(!(asAtomHandler::isValid(ret->getter) || asAtomHandler::isValid(ret->var)))
				ret=NULL;
		}
		return ret;
	}
	
	variable* findSettable(const multiname& name, bool* has_getter=nullptr) DLL_LOCAL;
	multiname* proxyMultiName;
	SystemState* sys;
protected:
	ASObject(MemoryAccount* m):objfreelist(nullptr),Variables(m),classdef(nullptr),proxyMultiName(nullptr),sys(nullptr),
		stringId(UINT32_MAX),type(T_OBJECT),subtype(SUBTYPE_NOT_SET),traitsInitialized(false),constructIndicator(false),constructorCallComplete(false),implEnable(true)
	{
#ifndef NDEBUG
		//Stuff only used in debugging
		initialized=false;
#endif
	}
	
	ASObject(const ASObject& o);
	virtual ~ASObject()
	{
		destroy();
	}
	uint32_t stringId;
	SWFOBJECT_TYPE type;
	CLASS_SUBTYPE subtype;
	
	bool traitsInitialized:1;
	bool constructIndicator:1;
	bool constructorCallComplete:1; // indicates that the constructor including all super constructors has been called
	void serializeDynamicProperties(ByteArray* out, std::map<tiny_string, uint32_t>& stringMap,
				std::map<const ASObject*, uint32_t>& objMap,
				std::map<const Class_base*, uint32_t> traitsMap,bool usedynamicPropertyWriter=true);
	void setClass(Class_base* c);
	static variable* findSettableImpl(SystemState* sys,variables_map& map, const multiname& name, bool* has_getter);
	static FORCE_INLINE const variable* findGettableImplConst(SystemState* sys, const variables_map& map, const multiname& name, uint32_t* nsRealId = nullptr)
	{
		const variable* ret=map.findObjVarConst(sys,name,DECLARED_TRAIT|DYNAMIC_TRAIT,nsRealId);
		if(ret)
		{
			//It seems valid for a class to redefine only the setter, so if we can't find
			//something to get, it's ok
			if(!(asAtomHandler::isValid(ret->getter) || asAtomHandler::isValid(ret->var)))
				ret=nullptr;
		}
		return ret;
	}
	static FORCE_INLINE variable* findGettableImpl(SystemState* sys, variables_map& map, const multiname& name, uint32_t* nsRealId = nullptr)
	{
		variable* ret=map.findObjVar(sys,name,DECLARED_TRAIT|DYNAMIC_TRAIT,nsRealId);
		if(ret)
		{
			//It seems valid for a class to redefine only the setter, so if we can't find
			//something to get, it's ok
			if(!(asAtomHandler::isValid(ret->getter) || asAtomHandler::isValid(ret->var)))
				ret=nullptr;
		}
		return ret;
	}
	
	/*
	   overridden from RefCountable
	   The destruct function is called when the reference count reaches 0 and the object is added to the free list of the class.
	   It should be implemented in all derived classes.
	   It should decRef all referenced objects.
	   It has to reset all data to their default state.
	   It has to call ASObject::destruct() as last instruction
	   The destruct method must be callable multiple time with the same effects (no double frees).
	*/
	bool destruct() override;
	// called when object is really destroyed
	virtual void destroy(){}

	FORCE_INLINE bool destructIntern()
	{
		destroyContents();
		if (proxyMultiName)
		{
			delete proxyMultiName;
			proxyMultiName = nullptr;
		}
		stringId = UINT32_MAX;
		traitsInitialized =false;
		constructIndicator = false;
		constructorCallComplete =false;
		implEnable = true;
#ifndef NDEBUG
		//Stuff only used in debugging
		initialized=false;
#endif
		bool dodestruct = true;
		if (objfreelist)
		{
			if (!getCached())
				dodestruct = !objfreelist->pushObjectToFreeList(this);
			else
				dodestruct = false;
		}
		if (dodestruct)
		{
#ifndef NDEBUG
			if (classdef)
			{
				uint32_t x = objectcounter[classdef];
				x--;
				objectcounter[classdef] = x;
			}
#endif
			finalize();
		}
		return dodestruct;
	}
public:
	ASObject(Class_base* c,SWFOBJECT_TYPE t = T_OBJECT,CLASS_SUBTYPE subtype = SUBTYPE_NOT_SET);
	
#ifndef NDEBUG
	//Stuff only used in debugging
	bool initialized:1;
	static std::map<Class_base*,uint32_t> objectcounter;
	static void dumpObjectCounters(uint32_t threshhold);
#endif
	bool implEnable:1;

	inline Class_base* getClass() const { return classdef; }
	ASFUNCTION_ATOM(_constructor);
	// constructor for subclasses that can't be instantiated.
	// Throws ArgumentError.
	ASFUNCTION_ATOM(_constructorNotInstantiatable);
	ASFUNCTION_ATOM(_toString);
	ASFUNCTION_ATOM(_toLocaleString);
	ASFUNCTION_ATOM(hasOwnProperty);
	ASFUNCTION_ATOM(valueOf);
	ASFUNCTION_ATOM(isPrototypeOf);
	ASFUNCTION_ATOM(propertyIsEnumerable);
	ASFUNCTION_ATOM(setPropertyIsEnumerable);
	ASFUNCTION_ATOM(addProperty);
	ASFUNCTION_ATOM(registerClass);
	void check() const;
	static void s_incRef(ASObject* o)
	{
		o->incRef();
	}
	static void s_decRef(ASObject* o)
	{
		if(o)
			o->decRef();
	}
	static void s_decRef_safe(ASObject* o,ASObject* o2)
	{
		if(o && o!=o2)
			o->decRef();
	}
	/*
	   The finalize function is used only for classes that don't have the reusable flag set and on destruction at application exit
	   It should decRef all referenced objects.
	   It has to reset all data to their default state.
	   The finalize method must be callable multiple time with the same effects (no double frees).
	*/
	inline virtual void finalize() {}
	// use this to mark an ASObject as constant, instead of RefCountable->setConstant()
	// because otherwise it will not be properly deleted on application exit.
	void setRefConstant();

	virtual GET_VARIABLE_RESULT getVariableByMultiname(asAtom& ret, const multiname& name, GET_VARIABLE_OPTION opt=NONE)
	{
		return getVariableByMultinameIntern(ret,name,classdef,opt);
	}
	virtual GET_VARIABLE_RESULT getVariableByInteger(asAtom& ret, int index, GET_VARIABLE_OPTION opt=NONE)
	{
		return getVariableByIntegerIntern(ret,index,opt);
	}
	// AVM1 needs to check the "protoype" variable in addition to the normal behaviour
	GET_VARIABLE_RESULT AVM1getVariableByMultiname(asAtom& ret, const multiname& name, GET_VARIABLE_OPTION opt=NONE);
	/*
	 * Helper method using the get the raw variable struct instead of calling the getter.
	 * It is used by getVariableByMultiname and by early binding code
	 */
	variable *findVariableByMultiname(const multiname& name, Class_base* cls, uint32_t* nsRealID = nullptr, bool* isborrowed=nullptr, bool considerdynamic=false);
	/*
	 * Gets a variable of this object. It looks through all classes (beginning at cls),
	 * then the prototype chain, and then instance variables.
	 * If the property found is a getter, it is called and its return value returned.
	 */
	GET_VARIABLE_RESULT getVariableByMultinameIntern(asAtom& ret, const multiname& name, Class_base* cls, GET_VARIABLE_OPTION opt=NONE);
	GET_VARIABLE_RESULT getVariableByIntegerIntern(asAtom& ret, int index, GET_VARIABLE_OPTION opt=NONE)
	{
		multiname m(nullptr);
		m.name_type = multiname::NAME_INT;
		m.name_i = index;
		return getVariableByMultiname(ret,m,opt);
	}
	virtual int32_t getVariableByMultiname_i(const multiname& name);
	/* Simple getter interface for the common case */
	void getVariableByMultiname(asAtom& ret, const tiny_string& name, std::list<tiny_string> namespaces);
	/*
	 * Execute a AS method on this object. Returns the value
	 * returned by the function. One reference of each args[i] is
	 * consumed. The method must exist, otherwise a TypeError is
	 * thrown.
	 */
	void executeASMethod(asAtom &ret, const tiny_string& methodName, std::list<tiny_string> namespaces, asAtom *args, uint32_t num_args);
	virtual void setVariableByMultiname_i(multiname &name, int32_t value);
	enum CONST_ALLOWED_FLAG { CONST_ALLOWED=0, CONST_NOT_ALLOWED };
	/*
	 * If alreadyset is not null, it has to be initialized to false by the caller.
	 * It will be set to true if the old and new value are the same.
	 * In that case the old value will not be decReffed.
	 */
	virtual multiname* setVariableByMultiname(multiname& name, asAtom& o, CONST_ALLOWED_FLAG allowConst, bool* alreadyset=nullptr)
	{
		return setVariableByMultiname_intern(name,o,allowConst,classdef,alreadyset);
	}
	virtual void setVariableByInteger(int index, asAtom& o, CONST_ALLOWED_FLAG allowConst)
	{
		setVariableByInteger_intern(index,o,allowConst);
	}
	/*
	 * Sets  variable of this object. It looks through all classes (beginning at cls),
	 * then the prototype chain, and then instance variables.
	 * If the property found is a setter, it is called with the given 'o'.
	 * If no property is found, an instance variable is created.
	 * Setting CONSTANT_TRAIT is only allowed if allowConst is true
	 */
	multiname* setVariableByMultiname_intern(multiname& name, asAtom &o, CONST_ALLOWED_FLAG allowConst, Class_base* cls, bool *alreadyset);
	void setVariableByInteger_intern(int index, asAtom &o, CONST_ALLOWED_FLAG allowConst)
	{
		multiname m(nullptr);
		m.name_type = multiname::NAME_INT;
		m.name_i = index;
		setVariableByMultiname(m,o,allowConst,nullptr);
	}
	
	// sets dynamic variable without checking for existence
	// use it if it is guarranteed that the variable doesn't exist in this object
	FORCE_INLINE void setDynamicVariableNoCheck(uint32_t nameID, asAtom& o)
	{
		Variables.setDynamicVarNoCheck(nameID,o);
	}
	/*
	 * Called by ABCVm::buildTraits to create DECLARED_TRAIT or CONSTANT_TRAIT and set their type
	 */
	void initializeVariableByMultiname(multiname &name, asAtom& o, multiname* typemname,
			ABCContext* context, TRAIT_KIND traitKind, uint32_t slot_id, bool isenumerable);
	virtual bool deleteVariableByMultiname(const multiname& name);
	void setVariableByQName(const tiny_string& name, const tiny_string& ns, ASObject* o, TRAIT_KIND traitKind, bool isEnumerable = true);
	void setVariableByQName(const tiny_string& name, const nsNameAndKind& ns, ASObject* o, TRAIT_KIND traitKind, bool isEnumerable = true);
	variable *setVariableByQName(uint32_t nameId, const nsNameAndKind& ns, ASObject* o, TRAIT_KIND traitKind, bool isEnumerable = true);
	variable *setVariableAtomByQName(const tiny_string& name, const nsNameAndKind& ns, asAtom o, TRAIT_KIND traitKind, bool isEnumerable = true);
	variable *setVariableAtomByQName(uint32_t nameId, const nsNameAndKind& ns, asAtom o, TRAIT_KIND traitKind, bool isEnumerable = true, bool isRefcounted = true);
	//NOTE: the isBorrowed flag is used to distinguish methods/setters/getters that are inside a class but on behalf of the instances
	void setDeclaredMethodByQName(const tiny_string& name, const tiny_string& ns, IFunction* o, METHOD_TYPE type, bool isBorrowed, bool isEnumerable = true);
	void setDeclaredMethodByQName(const tiny_string& name, const nsNameAndKind& ns, IFunction* o, METHOD_TYPE type, bool isBorrowed, bool isEnumerable = true);
	void setDeclaredMethodByQName(uint32_t nameId, const nsNameAndKind& ns, IFunction* o, METHOD_TYPE type, bool isBorrowed, bool isEnumerable = true);
	void setDeclaredMethodAtomByQName(const tiny_string& name, const tiny_string& ns, asAtom o, METHOD_TYPE type, bool isBorrowed, bool isEnumerable = true);
	void setDeclaredMethodAtomByQName(const tiny_string& name, const nsNameAndKind& ns, asAtom o, METHOD_TYPE type, bool isBorrowed, bool isEnumerable = true);
	void setDeclaredMethodAtomByQName(uint32_t nameId, const nsNameAndKind& ns, asAtom f, METHOD_TYPE type, bool isBorrowed, bool isEnumerable = true);
	virtual bool hasPropertyByMultiname(const multiname& name, bool considerDynamic, bool considerPrototype);
	FORCE_INLINE asAtom getSlot(unsigned int n)
	{
		return Variables.getSlot(n);
	}
	FORCE_INLINE variable* getSlotVar(unsigned int n)
	{
		return Variables.getSlotVar(n);
	}
	FORCE_INLINE asAtom getSlotNoCheck(unsigned int n)
	{
		return Variables.getSlotNoCheck(n);
	}
	FORCE_INLINE TRAIT_KIND getSlotKind(unsigned int n)
	{
		return Variables.getSlotKind(n);
	}
	FORCE_INLINE bool setSlot(unsigned int n,asAtom o)
	{
		return Variables.setSlot(n,o,this);
	}
	FORCE_INLINE bool setSlotNoCoerce(unsigned int n,asAtom o)
	{
		return Variables.setSlotNoCoerce(n,o,this);
	}
	FORCE_INLINE Class_base* getSlotType(unsigned int n)
	{
		return Variables.getSlotType(n);
	}
	uint32_t findInstanceSlotByMultiname(multiname* name)
	{
		return Variables.findInstanceSlotByMultiname(name,getSystemState());
	}
	unsigned int numSlots() const
	{
		return Variables.slots_vars.size();
	}
	
	void initSlot(unsigned int n, variable *v);
	
	void initAdditionalSlots(std::vector<multiname *> &additionalslots);
	unsigned int numVariables() const;
	inline uint32_t getNameAt(int i) const
	{
		return Variables.getNameAt(i);
	}
	void getValueAt(asAtom &ret, int i);
	inline SWFOBJECT_TYPE getObjectType() const
	{
		return type;
	}
	inline SystemState* getSystemState() const
	{
		assert(sys);
		return sys;
	}
	inline void setSystemState(SystemState* s)
	{
		sys = s;
	}

	/* Implements ECMA's 9.8 ToString operation, but returns the concrete value */
	tiny_string toString();
	tiny_string toLocaleString();
	uint32_t toStringId();
	virtual int32_t toInt();
	virtual int32_t toIntStrict() { return toInt(); }
	virtual uint32_t toUInt();
	virtual int64_t toInt64();
	uint16_t toUInt16();
	/* Implements ECMA's 9.3 ToNumber operation, but returns the concrete value */
	virtual number_t toNumber();
	/* Implements ECMA's ToPrimitive (9.1) and [[DefaultValue]] (8.6.2.6) */
	void toPrimitive(asAtom& ret,TP_HINT hint = NO_HINT);
	bool isPrimitive() const;

	bool isInitialized() const {return traitsInitialized;}
	virtual bool isConstructed() const;
	
	/* helper functions for calling the "valueOf" and
	 * "toString" AS-functions which may be members of this
	 *  object */
	bool has_valueOf();
	void call_valueOf(asAtom &ret);
	bool has_toString();
	void call_toString(asAtom &ret);
	tiny_string call_toJSON(bool &ok, std::vector<ASObject *> &path, asAtom replacer, const tiny_string &spaces, const tiny_string &filter);

	/* Helper function for calling getClass()->getQualifiedClassName() */
	virtual tiny_string getClassName() const;

	ASFUNCTION_ATOM(generator);

	/* helpers for the dynamic property 'prototype' */
	bool hasprop_prototype();
	ASObject* getprop_prototype();
	void setprop_prototype(_NR<ASObject>& prototype, uint32_t nameID=BUILTIN_STRINGS::PROTOTYPE);

	//Comparison operators
	virtual bool isEqual(ASObject* r);
	virtual bool isEqualStrict(ASObject* r);
	virtual TRISTATE isLess(ASObject* r);
	virtual TRISTATE isLessAtom(asAtom& r);

	static void sinit(Class_base* c);
	static void buildTraits(ASObject* o);

	//Enumeration handling
	virtual uint32_t nextNameIndex(uint32_t cur_index);
	virtual void nextName(asAtom &ret, uint32_t index);
	virtual void nextValue(asAtom &ret, uint32_t index);

	//Called when the object construction is completed. Used by MovieClip implementation
	inline virtual void constructionComplete()
	{
	}
	//Called after the object Actionscript constructor was executed. Used by MovieClip implementation
	inline virtual void afterConstruction()
	{
	}

	/**
	  Serialization interface

	  The various maps are used to implement reference type of the AMF3 spec
	*/
	virtual void serialize(ByteArray* out, std::map<tiny_string, uint32_t>& stringMap,
				std::map<const ASObject*, uint32_t>& objMap,
				std::map<const Class_base*, uint32_t>& traitsMap);

	virtual ASObject *describeType() const;

	virtual tiny_string toJSON(std::vector<ASObject *> &path, asAtom replacer, const tiny_string &spaces,const tiny_string& filter);
	/* returns true if the current object is of type T */
	template<class T> bool is() const { 
		LOG(LOG_INFO,"dynamic cast:"<<this->getClassName());
		return dynamic_cast<const T*>(this); 
	}
	/* returns this object casted to the given type.
	 * You have to make sure that it actually is the type (see is<T>() above)
	 */
	template<class T> const T* as() const { return static_cast<const T*>(this); }
	template<class T> T* as() { return static_cast<T*>(this); }

	/* Returns a debug string identifying this object */
	virtual std::string toDebugString();
	
	/* stores proxy namespace settings for internal usage */
	void setProxyProperty(const multiname& name); 
	/* applies proxy namespace settings to name for internal usage */
	void applyProxyProperty(multiname &name); 
	
	void dumpVariables();
	
	inline void setConstructIndicator() { constructIndicator = true; }
	inline void setConstructorCallComplete() { constructorCallComplete = true; }
	inline void setIsInitialized(bool init=true) { traitsInitialized=init; }
	inline bool getConstructIndicator() const { return constructIndicator; }

	
	void setIsEnumerable(const multiname& name, bool isEnum);
	inline void destroyContents()
	{
		Variables.destroyContents();
	}
	CLASS_SUBTYPE getSubtype() const { return subtype;}
	// copies all variables into the target
	// returns false if cloning is not possible
	bool cloneInstance(ASObject* target);
	
	/*!
	 * \brief checks for circular dependencies within the function scope
	 * \param o the function to check
	 */
	void checkFunctionScope(ASObject *o);
	
	virtual asAtom getVariableBindingValue(const tiny_string &name);
	virtual void AVM1HandleEvent(EventDispatcher* dispatcher, Event* e) { }
	virtual bool AVM1HandleKeyboardEvent(KeyboardEvent* e);
	virtual bool AVM1HandleMouseEvent(EventDispatcher* dispatcher,MouseEvent* e);
	bool AVM1HandleMouseEventStandard(ASObject *dispobj, MouseEvent *e);
	
	
	// copies all dynamic values to the target
	void copyValues(ASObject* target);
};


FORCE_INLINE bool variables_map::setSlot(unsigned int n, asAtom &o, ASObject* obj)
{
	assert_and_throw(n < slotcount);
	if (slots_vars[n]->var.uintval != o.uintval)
	{
		slots_vars[n]->setVar(o,obj);
		if (asAtomHandler::is<SyntheticFunction>(slots_vars[n]->var))
			obj->checkFunctionScope(asAtomHandler::getObjectNoCheck(o));
		return slots_vars[n]->var.uintval == o.uintval; // setVar may coerce the object into a new instance, so we need to check if incRef is necessary
	}
	if (asAtomHandler::is<SyntheticFunction>(slots_vars[n]->var))
		obj->checkFunctionScope(asAtomHandler::getObjectNoCheck(o));
	return true;
}

class AVM1Function;
class Activation_object;
class ApplicationDomain;
class Array;
class ASMutex;
class ASQName;
class ASString;
class ASWorker;
class BevelFilter;
class Bitmap;
class BitmapData;
class BitmapFilter;
class Boolean;
class BlurFilter;
class ByteArray;
class Class_inherit;
class ColorTransform;
class ColorMatrixFilter;
class ConvolutionFilter;
class ContentElement;
class Context3D;
class ContextMenu;
class ContextMenuBuiltInItems;
class ContextMenuEvent;
class CubeTexture;
class Date;
class DisplacementFilter;
class DisplayObject;
class DisplayObjectContainer;
class DropShadowFilter;
class ElementFormat;
class Event;
class Function;
class Function_object;
class FontDescription;
class GameInputDevice;
class GameInputEvent;
class Global;
class GlowFilter;
class GradientGlowFilter;
class GradientBevelFilter;
class IFunction;
class Integer;
class InteractiveObject;
class IndexBuffer3D;
class KeyboardEvent;
class LoaderContext;
class LoaderInfo;
class Matrix;
class Matrix3D;
class MouseEvent;
class MovieClip;
class Namespace;
class NetStream;
class Null;
class Number;
class ObjectConstructor;
class Point;
class Program3D;
class ProgressEvent;
class Proxy;
class Rectangle;
class RectangleTexture;
class RegExp;
class RootMovieClip;
class SampleDataEvent;
class ShaderFilter;
class SharedObject;
class Sound;
class SoundChannel;
class Sprite;
class Stage;
class Stage3D;
class Template_base;
class TextBlock;
class TextElement;
class TextField;
class TextFormat;
class TextLine;
class Texture;
class TextureBase;
class ThrottleEvent;
class Type;
class UInteger;
class Undefined;
class Vector;
class Vector3D;
class VertexBuffer3D;
class Video;
class VideoTexture;
class WaitableEvent;
class WorkerDomain;
class XML;
class XMLList;


// this is used to avoid calls to dynamic_cast when testing for some classes
// keep in mind that when adding a class here you have to take care of the class inheritance and add the new SUBTYPE_ to all apropriate is<> methods 
template<> inline bool ASObject::is<AVM1Function>() const { return subtype==SUBTYPE_AVM1FUNCTION; }
template<> inline bool ASObject::is<Activation_object>() const { return subtype==SUBTYPE_ACTIVATIONOBJECT; }
template<> inline bool ASObject::is<ApplicationDomain>() const { return subtype==SUBTYPE_APPLICATIONDOMAIN; }
template<> inline bool ASObject::is<Array>() const { return type==T_ARRAY; }
template<> inline bool ASObject::is<ASMutex>() const { return subtype==SUBTYPE_MUTEX; }
template<> inline bool ASObject::is<ASObject>() const { return true; }
template<> inline bool ASObject::is<ASQName>() const { return type==T_QNAME; }
template<> inline bool ASObject::is<ASString>() const { return type==T_STRING; }
template<> inline bool ASObject::is<ASWorker>() const { return subtype==SUBTYPE_WORKER; }
template<> inline bool ASObject::is<BevelFilter>() const { return subtype==SUBTYPE_BEVELFILTER; }
template<> inline bool ASObject::is<Bitmap>() const { return subtype==SUBTYPE_BITMAP; }
template<> inline bool ASObject::is<BitmapData>() const { return subtype==SUBTYPE_BITMAPDATA; }
template<> inline bool ASObject::is<BitmapFilter>() const { return subtype==SUBTYPE_BITMAPFILTER || subtype==SUBTYPE_GLOWFILTER || subtype==SUBTYPE_DROPSHADOWFILTER || subtype==SUBTYPE_GRADIENTGLOWFILTER || subtype==SUBTYPE_BEVELFILTER || subtype==SUBTYPE_COLORMATRIXFILTER || subtype==SUBTYPE_BLURFILTER || subtype==SUBTYPE_CONVOLUTIONFILTER || subtype==SUBTYPE_DISPLACEMENTFILTER || subtype==SUBTYPE_GRADIENTBEVELFILTER || subtype==SUBTYPE_SHADERFILTER; }
template<> inline bool ASObject::is<Boolean>() const { return type==T_BOOLEAN; }
template<> inline bool ASObject::is<BlurFilter>() const { return subtype==SUBTYPE_BLURFILTER; }
template<> inline bool ASObject::is<ByteArray>() const { return subtype==SUBTYPE_BYTEARRAY; }
template<> inline bool ASObject::is<Class_base>() const { return type==T_CLASS; }
template<> inline bool ASObject::is<Class_inherit>() const { return subtype==SUBTYPE_INHERIT; }
template<> inline bool ASObject::is<ColorTransform>() const { return subtype==SUBTYPE_COLORTRANSFORM; }
template<> inline bool ASObject::is<ColorMatrixFilter>() const { return subtype==SUBTYPE_COLORMATRIXFILTER; }
template<> inline bool ASObject::is<ContentElement>() const { return subtype==SUBTYPE_CONTENTELEMENT || subtype == SUBTYPE_TEXTELEMENT; }
template<> inline bool ASObject::is<Context3D>() const { return subtype==SUBTYPE_CONTEXT3D; }
template<> inline bool ASObject::is<ContextMenu>() const { return subtype==SUBTYPE_CONTEXTMENU; }
template<> inline bool ASObject::is<ContextMenuBuiltInItems>() const { return subtype==SUBTYPE_CONTEXTMENUBUILTINITEMS; }
template<> inline bool ASObject::is<ContextMenuEvent>() const { return subtype==SUBTYPE_CONTEXTMENUEVENT; }
template<> inline bool ASObject::is<ConvolutionFilter>() const { return subtype==SUBTYPE_CONVOLUTIONFILTER; }
template<> inline bool ASObject::is<CubeTexture>() const { return subtype==SUBTYPE_CUBETEXTURE; }
template<> inline bool ASObject::is<Date>() const { return subtype==SUBTYPE_DATE; }
template<> inline bool ASObject::is<DisplacementFilter>() const { return subtype==SUBTYPE_DISPLACEMENTFILTER; }
template<> inline bool ASObject::is<DisplayObject>() const { return subtype==SUBTYPE_DISPLAYOBJECT || subtype==SUBTYPE_INTERACTIVE_OBJECT || subtype==SUBTYPE_TEXTFIELD || subtype==SUBTYPE_BITMAP || subtype==SUBTYPE_DISPLAYOBJECTCONTAINER || subtype==SUBTYPE_STAGE || subtype==SUBTYPE_ROOTMOVIECLIP || subtype==SUBTYPE_SPRITE || subtype == SUBTYPE_MOVIECLIP || subtype == SUBTYPE_TEXTLINE || subtype == SUBTYPE_VIDEO; }
template<> inline bool ASObject::is<DisplayObjectContainer>() const { return subtype==SUBTYPE_DISPLAYOBJECTCONTAINER || subtype==SUBTYPE_STAGE || subtype==SUBTYPE_ROOTMOVIECLIP || subtype==SUBTYPE_SPRITE || subtype == SUBTYPE_MOVIECLIP || subtype == SUBTYPE_TEXTLINE; }
template<> inline bool ASObject::is<DropShadowFilter>() const { return subtype==SUBTYPE_DROPSHADOWFILTER; }
template<> inline bool ASObject::is<ElementFormat>() const { return subtype==SUBTYPE_ELEMENTFORMAT; }
template<> inline bool ASObject::is<Event>() const { return subtype==SUBTYPE_EVENT || subtype==SUBTYPE_WAITABLE_EVENT || subtype==SUBTYPE_PROGRESSEVENT || subtype==SUBTYPE_KEYBOARD_EVENT || subtype==SUBTYPE_MOUSE_EVENT || subtype==SUBTYPE_SAMPLEDATA_EVENT || subtype == SUBTYPE_THROTTLE_EVENT || subtype == SUBTYPE_CONTEXTMENUEVENT || subtype == SUBTYPE_GAMEINPUTEVENT; }
template<> inline bool ASObject::is<FontDescription>() const { return subtype==SUBTYPE_FONTDESCRIPTION; }
template<> inline bool ASObject::is<Function_object>() const { return subtype==SUBTYPE_FUNCTIONOBJECT; }
template<> inline bool ASObject::is<Function>() const { return subtype==SUBTYPE_FUNCTION; }
template<> inline bool ASObject::is<GameInputDevice>() const { return subtype==SUBTYPE_GAMEINPUTDEVICE; }
template<> inline bool ASObject::is<GameInputEvent>() const { return subtype==SUBTYPE_GAMEINPUTEVENT; }
template<> inline bool ASObject::is<Global>() const { return subtype==SUBTYPE_GLOBAL; }
template<> inline bool ASObject::is<GlowFilter>() const { return subtype==SUBTYPE_GLOWFILTER; }
template<> inline bool ASObject::is<GradientGlowFilter>() const { return subtype==SUBTYPE_GRADIENTGLOWFILTER; }
template<> inline bool ASObject::is<GradientBevelFilter>() const { return subtype==SUBTYPE_GRADIENTBEVELFILTER; }
template<> inline bool ASObject::is<IFunction>() const { return type==T_FUNCTION; }
template<> inline bool ASObject::is<IndexBuffer3D>() const { return subtype==SUBTYPE_INDEXBUFFER3D; }
template<> inline bool ASObject::is<Integer>() const { return type==T_INTEGER; }
template<> inline bool ASObject::is<InteractiveObject>() const { return subtype==SUBTYPE_INTERACTIVE_OBJECT || subtype==SUBTYPE_TEXTFIELD || subtype==SUBTYPE_DISPLAYOBJECTCONTAINER || subtype==SUBTYPE_STAGE || subtype==SUBTYPE_ROOTMOVIECLIP || subtype==SUBTYPE_SPRITE || subtype == SUBTYPE_MOVIECLIP; }
template<> inline bool ASObject::is<KeyboardEvent>() const { return subtype==SUBTYPE_KEYBOARD_EVENT; }
template<> inline bool ASObject::is<LoaderContext>() const { return subtype==SUBTYPE_LOADERCONTEXT; }
template<> inline bool ASObject::is<LoaderInfo>() const { return subtype==SUBTYPE_LOADERINFO; }
template<> inline bool ASObject::is<Namespace>() const { return type==T_NAMESPACE; }
template<> inline bool ASObject::is<NetStream>() const { return subtype==SUBTYPE_NETSTREAM; }
template<> inline bool ASObject::is<Matrix>() const { return subtype==SUBTYPE_MATRIX; }
template<> inline bool ASObject::is<Matrix3D>() const { return subtype==SUBTYPE_MATRIX3D; }
template<> inline bool ASObject::is<MouseEvent>() const { return subtype==SUBTYPE_MOUSE_EVENT; }
template<> inline bool ASObject::is<MovieClip>() const { return subtype==SUBTYPE_ROOTMOVIECLIP || subtype == SUBTYPE_MOVIECLIP; }
template<> inline bool ASObject::is<Null>() const { return type==T_NULL; }
template<> inline bool ASObject::is<Number>() const { return type==T_NUMBER; }
template<> inline bool ASObject::is<ObjectConstructor>() const { return subtype==SUBTYPE_OBJECTCONSTRUCTOR; }
template<> inline bool ASObject::is<Point>() const { return subtype==SUBTYPE_POINT; }
template<> inline bool ASObject::is<Program3D>() const { return subtype==SUBTYPE_PROGRAM3D; }
template<> inline bool ASObject::is<ProgressEvent>() const { return subtype==SUBTYPE_PROGRESSEVENT; }
template<> inline bool ASObject::is<Proxy>() const { return subtype==SUBTYPE_PROXY; }
template<> inline bool ASObject::is<Rectangle>() const { return subtype==SUBTYPE_RECTANGLE; }
template<> inline bool ASObject::is<RectangleTexture>() const { return subtype==SUBTYPE_RECTANGLETEXTURE; }
template<> inline bool ASObject::is<RegExp>() const { return subtype==SUBTYPE_REGEXP; }
template<> inline bool ASObject::is<RootMovieClip>() const { return subtype==SUBTYPE_ROOTMOVIECLIP; }
template<> inline bool ASObject::is<SampleDataEvent>() const { return subtype==SUBTYPE_SAMPLEDATA_EVENT; }
template<> inline bool ASObject::is<ShaderFilter>() const { return subtype==SUBTYPE_SHADERFILTER; }
template<> inline bool ASObject::is<SharedObject>() const { return subtype==SUBTYPE_SHAREDOBJECT; }
template<> inline bool ASObject::is<Sound>() const { return subtype==SUBTYPE_SOUND; }
template<> inline bool ASObject::is<SoundChannel>() const { return subtype==SUBTYPE_SOUNDCHANNEL; }
template<> inline bool ASObject::is<SoundTransform>() const { return subtype==SUBTYPE_SOUNDTRANSFORM; }
template<> inline bool ASObject::is<Sprite>() const { return subtype==SUBTYPE_SPRITE || subtype==SUBTYPE_ROOTMOVIECLIP || subtype == SUBTYPE_MOVIECLIP; }
template<> inline bool ASObject::is<Stage>() const { return subtype==SUBTYPE_STAGE; }
template<> inline bool ASObject::is<Stage3D>() const { return subtype==SUBTYPE_STAGE3D; }
template<> inline bool ASObject::is<SyntheticFunction>() const { return subtype==SUBTYPE_SYNTHETICFUNCTION; }
template<> inline bool ASObject::is<Template_base>() const { return type==T_TEMPLATE; }
template<> inline bool ASObject::is<TextBlock>() const { return subtype==SUBTYPE_TEXTBLOCK; }
template<> inline bool ASObject::is<TextElement>() const { return subtype==SUBTYPE_TEXTELEMENT; }
template<> inline bool ASObject::is<TextField>() const { return subtype==SUBTYPE_TEXTFIELD; }
template<> inline bool ASObject::is<TextFormat>() const { return subtype==SUBTYPE_TEXTFORMAT; }
template<> inline bool ASObject::is<TextLine>() const { return subtype==SUBTYPE_TEXTLINE; }
template<> inline bool ASObject::is<Texture>() const { return subtype==SUBTYPE_TEXTURE; }
template<> inline bool ASObject::is<TextureBase>() const { return subtype==SUBTYPE_TEXTUREBASE || subtype==SUBTYPE_TEXTURE || subtype==SUBTYPE_CUBETEXTURE || subtype==SUBTYPE_RECTANGLETEXTURE || subtype==SUBTYPE_TEXTURE || subtype==SUBTYPE_VIDEOTEXTURE; }
template<> inline bool ASObject::is<ThrottleEvent>() const { return subtype==SUBTYPE_THROTTLE_EVENT; }
template<> inline bool ASObject::is<Type>() const { return type==T_CLASS; }
template<> inline bool ASObject::is<UInteger>() const { return type==T_UINTEGER; }
template<> inline bool ASObject::is<Undefined>() const { return type==T_UNDEFINED; }
template<> inline bool ASObject::is<Vector>() const { return subtype==SUBTYPE_VECTOR; }
template<> inline bool ASObject::is<Vector3D>() const { return subtype==SUBTYPE_VECTOR3D; }
template<> inline bool ASObject::is<VertexBuffer3D>() const { return subtype==SUBTYPE_VERTEXBUFFER3D; }
template<> inline bool ASObject::is<Video>() const { return subtype==SUBTYPE_VIDEO; }
template<> inline bool ASObject::is<VideoTexture>() const { return subtype==SUBTYPE_VIDEOTEXTURE; }
template<> inline bool ASObject::is<WaitableEvent>() const { return subtype==SUBTYPE_WAITABLE_EVENT; }
template<> inline bool ASObject::is<WorkerDomain>() const { return subtype==SUBTYPE_WORKERDOMAIN; }
template<> inline bool ASObject::is<XML>() const { return subtype==SUBTYPE_XML; }
template<> inline bool ASObject::is<XMLList>() const { return subtype==SUBTYPE_XMLLIST; }



template<class T> inline bool asAtomHandler::is(asAtom& a) {
	return isObject(a) ? getObjectNoCheck(a)->is<T>() : false;
}
template<> inline bool asAtomHandler::is<asAtom>(asAtom& a) { return true; }
template<> inline bool asAtomHandler::is<ASObject>(asAtom& a) { return true; }
template<> inline bool asAtomHandler::is<ASString>(asAtom& a) { return isStringID(a) || isString(a); }
template<> inline bool asAtomHandler::is<Boolean>(asAtom& a) { return isBool(a); }
template<> inline bool asAtomHandler::is<Integer>(asAtom& a) { return isInteger(a); }
template<> inline bool asAtomHandler::is<Null>(asAtom& a) { return isNull(a); }
template<> inline bool asAtomHandler::is<Number>(asAtom& a) { return isNumber(a); }
template<> inline bool asAtomHandler::is<UInteger>(asAtom& a) { return isUInteger(a); }
template<> inline bool asAtomHandler::is<Undefined>(asAtom& a) { return isUndefined(a); }


FORCE_INLINE int32_t asAtomHandler::toInt(const asAtom& a)
{
	if ((a.uintval&0x7)==ATOM_INTEGER)
        return a.intval>>3;
    else if ((a.uintval&0x7)==ATOM_UINTEGER)
        return a.uintval>>3;
    else if ((a.uintval&0x7)==ATOM_INVALID_UNDEFINED_NULL_BOOL)
        return (a.uintval&ATOMTYPE_BOOL_BIT) ? (a.uintval&0x80)>>7 : 0;
    else if ((a.uintval&0x7)==ATOM_STRINGID)
    {
        ASObject* s = abstract_s(getSys(),a.uintval>>3);
        int32_t ret = s->toInt();
        s->decRef();
        return ret;
    }
    assert(getObject(a));
    return getObjectNoCheck(a)->toInt();
}
FORCE_INLINE int32_t asAtomHandler::toIntStrict(const asAtom& a)
{
	switch(a.uintval&0x7)
	{
		case ATOM_INTEGER:
			return a.intval>>3;
		case ATOM_UINTEGER:
			return a.uintval>>3;
		case ATOM_INVALID_UNDEFINED_NULL_BOOL:
			return (a.uintval&ATOMTYPE_BOOL_BIT) ? (a.uintval&0x80)>>7 : 0;
		case ATOM_STRINGID:
		{
			ASObject* s = abstract_s(getSys(),a.uintval>>3);
			int32_t ret = s->toIntStrict();
			s->decRef();
			return ret;
		}
		default:
			assert(getObject(a));
			return getObjectNoCheck(a)->toIntStrict();
	}
}
FORCE_INLINE number_t asAtomHandler::toNumber(const asAtom& a)
{
	switch(a.uintval&0x7)
	{
		case ATOM_INTEGER:
			return a.intval>>3;
		case ATOM_UINTEGER:
			return a.uintval>>3;
		case ATOM_INVALID_UNDEFINED_NULL_BOOL:
			return (a.uintval&ATOMTYPE_BOOL_BIT) ? (a.uintval&0x80)>>7 : (a.uintval&ATOMTYPE_UNDEFINED_BIT) ? numeric_limits<double>::quiet_NaN() : 0;
		case ATOM_STRINGID:
		{
			ASObject* s = abstract_s(getSys(),a.uintval>>3);
			number_t ret = s->toNumber();
			s->decRef();
			return ret;
		}
		default:
			assert(getObject(a));
			return getObjectNoCheck(a)->toNumber();
	}
}
FORCE_INLINE number_t asAtomHandler::AVM1toNumber(asAtom& a, uint32_t swfversion)
{
	switch(a.uintval&0x7)
	{
		case ATOM_INTEGER:
			return a.intval>>3;
		case ATOM_UINTEGER:
			return a.uintval>>3;
		case ATOM_INVALID_UNDEFINED_NULL_BOOL:
			return (a.uintval&ATOMTYPE_BOOL_BIT) ? (a.uintval&0x80)>>7 : (a.uintval&ATOMTYPE_UNDEFINED_BIT) && swfversion > 6 ? numeric_limits<double>::quiet_NaN() : 0;
		case ATOM_STRINGID:
		{
			ASObject* s = abstract_s(getSys(),a.uintval>>3);
			number_t ret = s->toNumber();
			s->decRef();
			return ret;
		}
		default:
			assert(getObject(a));
			return getObjectNoCheck(a)->toNumber();
	}
}
FORCE_INLINE bool asAtomHandler::AVM1toBool(asAtom& a)
{
	switch(a.uintval&0x7)
	{
		case ATOM_INTEGER:
			return a.intval>>3;
		case ATOM_UINTEGER:
			return a.uintval>>3;
		case ATOM_NUMBERPTR:
			return toNumber(a);
		case ATOM_INVALID_UNDEFINED_NULL_BOOL:
			return (a.uintval&ATOMTYPE_BOOL_BIT) ? (a.uintval&0x80)>>7 : false;
		default:
			return true;
	}
}

FORCE_INLINE int64_t asAtomHandler::toInt64(const asAtom& a)
{
	switch(a.uintval&0x7)
	{
		case ATOM_INTEGER:
			return a.intval>>3;
		case ATOM_UINTEGER:
			return a.uintval>>3;
		case ATOM_INVALID_UNDEFINED_NULL_BOOL:
			return (a.uintval&ATOMTYPE_BOOL_BIT) ? (a.uintval&0x80)>>7 : 0;
		case ATOM_STRINGID:
		{
			ASObject* s = abstract_s(getSys(),a.uintval>>3);
			int64_t ret = s->toInt64();
			s->decRef();
			return ret;
		}
		default:
			assert(getObject(a));
			return getObjectNoCheck(a)->toInt64();
	}
}
FORCE_INLINE uint32_t asAtomHandler::toUInt(asAtom& a)
{
	switch(a.uintval&0x7)
	{
		case ATOM_INTEGER:
			return a.intval>>3;
		case ATOM_UINTEGER:
			return a.uintval>>3;
		case ATOM_INVALID_UNDEFINED_NULL_BOOL:
			return (a.uintval&ATOMTYPE_BOOL_BIT) ? (a.uintval&0x80)>>7 : 0;
		case ATOM_STRINGID:	
		{
			ASObject* s = abstract_s(getSys(),a.uintval>>3);
			uint32_t ret = s->toUInt();
			s->decRef();
			return ret;
		}
		default:
			assert(getObject(a));
			return getObjectNoCheck(a)->toUInt();
	}
}

FORCE_INLINE void asAtomHandler::applyProxyProperty(asAtom& a,SystemState* sys,multiname &name)
{
	switch(a.uintval&0x7)
	{
		case ATOM_INTEGER:
		case ATOM_UINTEGER:
		case ATOM_INVALID_UNDEFINED_NULL_BOOL:
		case ATOM_NUMBERPTR:
		case ATOM_U_INTEGERPTR:
			break;
		case ATOM_STRINGID:
			break; // no need to create string, as it won't have a proxyMultiName
		default:
			assert(getObject(a));
			getObjectNoCheck(a)->applyProxyProperty(name);
			break;
	}
}

FORCE_INLINE bool asAtomHandler::isEqualStrict(asAtom& a, SystemState *sys, asAtom &v2)
{
	if(getObjectType(a)!=getObjectType(v2))
	{
		//Type conversions are ok only for numeric types
		switch(a.uintval&0x7)
		{
			case ATOM_NUMBERPTR:
			case ATOM_INTEGER:
			case ATOM_UINTEGER:
			case ATOM_U_INTEGERPTR:
				break;
			case ATOM_INVALID_UNDEFINED_NULL_BOOL:
			{
				switch (v2.uintval&0x70)
				{
					case ATOMTYPE_NULL_BIT:
						if (!isConstructed(v2) && getObjectType(v2)!=T_CLASS)
							return true;
						return false;
					default:
						return false;
				}
				break;
			}
			case ATOM_OBJECTPTR:
				if (isNumber(a))
					break;
				return false;
			default:
				return false;
		}
		switch(v2.uintval&0x7)
		{
			case ATOM_NUMBERPTR:
			case ATOM_INTEGER:
			case ATOM_UINTEGER:
			case ATOM_U_INTEGERPTR:
				break;
			case ATOM_INVALID_UNDEFINED_NULL_BOOL:
			{
				switch (v2.uintval&0x70)
				{
					case ATOMTYPE_NULL_BIT:
						if (!isConstructed(a) && getObjectType(a)!=T_CLASS)
							return true;
						return false;
					default:
						return false;
				}
				break;
			}
			case ATOM_OBJECTPTR:
				if (isNumber(v2))
					break;
				return false;
			default:
				return false;
		}
	}
	return isEqual(a,sys,v2);
}

FORCE_INLINE bool asAtomHandler::isConstructed(const asAtom& a)
{
	switch(a.uintval&0x7)
	{
		case ATOM_INTEGER:
		case ATOM_UINTEGER:
		case ATOM_NUMBERPTR:
		case ATOM_INVALID_UNDEFINED_NULL_BOOL:
			return a.uintval;
		case ATOM_STRINGID:
			return true;
		default:
			return isObject(a) && getObjectNoCheck(a)->isConstructed();
	}
}

FORCE_INLINE bool asAtomHandler::isPrimitive(const asAtom& a)
{
	// ECMA 3, section 4.3.2, T_INTEGER and T_UINTEGER are added
	// because they are special cases of Number
	return isUndefined(a) || isNull(a) || isNumber(a) || isString(a) || isInteger(a)|| isUInteger(a) || isBool(a);
}
FORCE_INLINE bool asAtomHandler::checkArgumentConversion(const asAtom& a,const asAtom& obj)
{
	if ((a.uintval&0x7) == (obj.uintval&0x7))
	{
		if ((a.uintval&0x7) == ATOM_OBJECTPTR)
			return getObjectNoCheck(a)->getObjectType() == getObjectNoCheck(obj)->getObjectType();
		return true;
	}
	if (isNumeric(a) && isNumeric(obj))
		return true;
	return false;
}

FORCE_INLINE void asAtomHandler::setInt(asAtom& a,SystemState* sys, int64_t val)
{
#ifdef LIGHTSPARK_64
	a.intval = ((int64_t)val<<3)|ATOM_INTEGER;
#else
	if (val >=-(1<<28)  && val <=(1<<28))
		a.intval = (val<<3)|ATOM_INTEGER;
	else
		setNumber(a,sys,val);
#endif
}

FORCE_INLINE void asAtomHandler::setUInt(asAtom& a,SystemState* sys, uint32_t val)
{
#ifdef LIGHTSPARK_64
	a.uintval = ((uint64_t)val<<3)|ATOM_UINTEGER;
#else
	if (val <(1<<29))
		a.uintval = (val<<3)|ATOM_UINTEGER;
	else
		setNumber(a,sys,val);
#endif
}

FORCE_INLINE void asAtomHandler::setBool(asAtom& a,bool val)
{
	a.uintval = ATOM_INVALID_UNDEFINED_NULL_BOOL | ATOMTYPE_BOOL_BIT | (val ? 0x80 : 0);
}

FORCE_INLINE void asAtomHandler::setNull(asAtom& a)
{
	a.uintval = ATOM_INVALID_UNDEFINED_NULL_BOOL | ATOMTYPE_NULL_BIT;
}
FORCE_INLINE void asAtomHandler::setUndefined(asAtom& a)
{
	a.uintval = ATOM_INVALID_UNDEFINED_NULL_BOOL | ATOMTYPE_UNDEFINED_BIT;
}
FORCE_INLINE void asAtomHandler::increment(asAtom& a,SystemState* sys)
{
	switch(a.uintval&0x7)
	{
		case ATOM_INVALID_UNDEFINED_NULL_BOOL:
		{
			switch (a.uintval&0x70)
			{
				case ATOMTYPE_NULL_BIT:
					setInt(a,sys,1);
					break;
				case ATOMTYPE_UNDEFINED_BIT:
					setNumber(a,sys,numeric_limits<double>::quiet_NaN());
					break;
				case ATOMTYPE_BOOL_BIT:
					setInt(a,sys,(a.uintval & 0x80 ? 1 : 0)+1);
					break;
				default:
					return;
			}
			break;
		}
		case ATOM_INTEGER:
			setInt(a,sys,(a.intval>>3)+1);
			break;
		case ATOM_UINTEGER:
			setUInt(a,sys,(a.uintval>>3)+1);
			break;
		case ATOM_NUMBERPTR:
			setNumber(a,sys,toNumber(a)+1);
			break;
		case ATOM_STRINGID:
		{
			ASObject* s = abstract_s(sys,a.uintval>>3);
			number_t n = s->toNumber();
			setNumber(a,sys,n+1);
			s->decRef();
			break;
		}
		default:
		{
			number_t n=toNumber(a);
			setNumber(a,sys,n+1);
			break;
		}
	}
}

FORCE_INLINE void asAtomHandler::decrement(asAtom& a,SystemState* sys)
{
	switch(a.uintval&0x7)
	{
		case ATOM_INVALID_UNDEFINED_NULL_BOOL:
		{
			switch (a.uintval&0x70)
			{
				case ATOMTYPE_NULL_BIT:
					setInt(a,sys,-1);
					break;
				case ATOMTYPE_UNDEFINED_BIT:
					setNumber(a,sys,numeric_limits<double>::quiet_NaN());
					break;
				case ATOMTYPE_BOOL_BIT:
					setInt(a,sys,(a.uintval & 0x80 ? 1 : 0)-1);
					break;
				default:
					return;
			}
			break;
		}
		case ATOM_INTEGER:
			setInt(a,sys,(a.uintval>>3)-1);
			break;
		case ATOM_UINTEGER:
		{
			if (a.uintval>>3 > 0)
				setUInt(a,sys,(a.uintval>>3)-1);
			else
			{
				number_t val = a.uintval>>3;
				setNumber(a,sys,val-1);
			}
			break;
		}
		case ATOM_NUMBERPTR:
			setNumber(a,sys,toNumber(a)-1);
			break;
		case ATOM_STRINGID:
		{
			ASObject* s = abstract_s(sys,a.uintval>>3);
			number_t n = s->toNumber();
			setNumber(a,sys,n-1);
			s->decRef();
			break;
		}
		default:
		{
			number_t n=toNumber(a);
			setNumber(a,sys,n-1);
			break;
		}
	}

}

FORCE_INLINE void asAtomHandler::increment_i(asAtom& a,SystemState* sys)
{
	if ((a.uintval&0x7) == ATOM_INTEGER)
		setInt(a,sys,int32_t(a.intval>>3)+1);
	else
		setInt(a,sys,toInt(a)+1);
}
FORCE_INLINE void asAtomHandler::decrement_i(asAtom& a,SystemState* sys)
{
	if ((a.uintval&0x7) == ATOM_INTEGER)
		setInt(a,sys,int32_t(a.intval>>3)-1);
	else
		setInt(a,sys,toInt(a)-1);
}

FORCE_INLINE void asAtomHandler::bit_xor(asAtom& a,SystemState* sys,asAtom &v1)
{
	int32_t i1=toInt(v1);
	int32_t i2=toInt(a);
	LOG_CALL(_("bitXor ") << std::hex << i1 << '^' << i2 << std::dec);
	setInt(a,sys,i1^i2);
}

FORCE_INLINE void asAtomHandler::bitnot(asAtom& a,SystemState* sys)
{
	int32_t i1=toInt(a);
	LOG_CALL(_("bitNot ") << std::hex << i1 << std::dec);
	setInt(a,sys,~i1);
}

FORCE_INLINE void asAtomHandler::subtract(asAtom& a,SystemState* sys,asAtom &v2, bool forceint)
{
	if( ((a.uintval&0x7) == ATOM_INTEGER || (a.uintval&0x7) == ATOM_UINTEGER) &&
		(isInteger(v2) || (v2.uintval&0x7) ==ATOM_UINTEGER))
	{
		int64_t num1=toInt64(a);
		int64_t num2=toInt64(v2);
	
		LOG_CALL(_("subtractI ") << num1 << '-' << num2);
		int64_t res = num1-num2;
		if (forceint || (res > INT32_MIN>>3 && res < INT32_MAX>>3))
			setInt(a,sys,int32_t(res));
		else if (res >= 0 && res < UINT32_MAX>>3)
			setUInt(a,sys,res);
		else
			setNumber(a,sys,res);
	}
	else
	{
		number_t num2=toNumber(v2);
		number_t num1=toNumber(a);
	
		LOG_CALL(_("subtract ") << num1 << '-' << num2);
		if (forceint)
			setInt(a,sys,num1-num2);
		else
			setNumber(a,sys,num1-num2);
	}
}
FORCE_INLINE void asAtomHandler::subtractreplace(asAtom& ret,SystemState* sys,const asAtom &v1, const asAtom &v2, bool forceint)
{
	if( ((v1.uintval&0x7) == ATOM_INTEGER || (v1.uintval&0x7) == ATOM_UINTEGER) &&
		(isInteger(v2) || (v2.uintval&0x7) ==ATOM_UINTEGER))
	{
		int64_t num1=toInt64(v1);
		int64_t num2=toInt64(v2);
	
		LOG_CALL(_("subtractreplaceI ") << num1 << '-' << num2);
		ASATOM_DECREF(ret);
		int64_t res = num1-num2;
		if (forceint || (res > INT32_MIN>>3 && res < INT32_MAX>>3))
			setInt(ret,sys,int32_t(res));
		else if (res >= 0 && res < UINT32_MAX>>3)
			setUInt(ret,sys,res);
		else
			setNumber(ret,sys,res);
	}
	else
	{
		number_t num2=toNumber(v2);
		number_t num1=toNumber(v1);
	
		ASObject* o = getObject(ret);
		LOG_CALL(_("subtractreplace ")  << num1 << '-' << num2);
		if (forceint)
		{
			setInt(ret,sys,num1-num2);
			if (o)
				o->decRef();
		}
		else if (replaceNumber(ret,sys,num1-num2) && o)
			o->decRef();
	}
}

FORCE_INLINE void asAtomHandler::multiply(asAtom& a,SystemState* sys,asAtom &v2, bool forceint)
{
	if( ((a.uintval&0x7) == ATOM_INTEGER || (a.uintval&0x7) == ATOM_UINTEGER) &&
		(isInteger(v2) || (v2.uintval&0x7) ==ATOM_UINTEGER))
	{
		int64_t num1=toInt64(a);
		int64_t num2=toInt64(v2);
	
		LOG_CALL(_("multiplyI ") << num1 << '*' << num2);
		int64_t res = num1*num2;
		if (forceint || (res > INT32_MIN>>3 && res < INT32_MAX>>3))
			setInt(a,sys,res);
		else if (res >= 0 && res < UINT32_MAX>>3)
			setUInt(a,sys,res);
		else
			setNumber(a,sys,res);
	}
	else
	{
		double num1=toNumber(v2);
		double num2=toNumber(a);
		LOG_CALL(_("multiply ")  << num1 << '*' << num2);
		if (forceint)
			setInt(a,sys,num1*num2);
		else 
			setNumber(a,sys,num1*num2);
	}
}

FORCE_INLINE void asAtomHandler::multiplyreplace(asAtom& ret, SystemState* sys,const asAtom& v1, const asAtom &v2,bool forceint)
{
	if( ((v1.uintval&0x7) == ATOM_INTEGER || (v1.uintval&0x7) == ATOM_UINTEGER) &&
		(isInteger(v2) || (v2.uintval&0x7) ==ATOM_UINTEGER))
	{
		int64_t num1=toInt64(v1);
		int64_t num2=toInt64(v2);
	
		LOG_CALL(_("multiplyreplaceI ") << num1 << '*' << num2);
		ASATOM_DECREF(ret);
		int64_t res = num1*num2;
		
		if (forceint || (res > INT32_MIN>>3 && res < INT32_MAX>>3))
			setInt(ret,sys,res);
		else if (res >= 0 && res < UINT32_MAX>>3)
			setUInt(ret,sys,res);
		else
			setNumber(ret,sys,res);
	}
	else
	{
		double num1=toNumber(v2);
		double num2=toNumber(v1);
		ASObject* o = getObject(ret);
		LOG_CALL(_("multiplyreplace ")  << num1 << '*' << num2);
		if (forceint)
		{
			setInt(ret,sys,num1*num2);
			if (o)
				o->decRef();
		}
		else if (replaceNumber(ret,sys,num1*num2) && o)
			o->decRef();
	}
}

FORCE_INLINE void asAtomHandler::divide(asAtom& a,SystemState* sys,asAtom &v2, bool forceint)
{
	double num1=toNumber(a);
	double num2=toNumber(v2);

	LOG_CALL(_("divide ")  << num1 << '/' << num2);
	// handling of infinity according to ECMA-262, chapter 11.5.2
	if (std::isinf(num1))
	{
		if (std::isinf(num2) || std::isnan(num2))
			setNumber(a,sys,numeric_limits<double>::quiet_NaN());
		else
		{
			bool needssign = (std::signbit(num1) || std::signbit(num2)) && !(std::signbit(num1) && std::signbit(num2)); 
			setNumber(a,sys, needssign  ? -numeric_limits<double>::infinity() : numeric_limits<double>::infinity());
		}
	}
	else if (forceint)
		setInt(a,sys,num1/num2);
	else
		setNumber(a,sys,num1/num2);
}
FORCE_INLINE void asAtomHandler::dividereplace(asAtom& ret,SystemState* sys,const asAtom& v1, const asAtom &v2,bool forceint)
{
	double num1=toNumber(v1);
	double num2=toNumber(v2);

	number_t res=0;
	LOG_CALL(_("divide ")  << num1 << '/' << num2);
	// handling of infinity according to ECMA-262, chapter 11.5.2
	if (std::isinf(num1))
	{
		if (std::isinf(num2) || std::isnan(num2))
			res = numeric_limits<double>::quiet_NaN();
		else
		{
			bool needssign = (std::signbit(num1) || std::signbit(num2)) && !(std::signbit(num1) && std::signbit(num2)); 
			res = needssign  ? -numeric_limits<double>::infinity() : numeric_limits<double>::infinity();
		}
	}
	else
		res = num1/num2;
	
	ASObject* o = getObject(ret);
	LOG_CALL(_("dividereplace ")  << num1 << '/' << num2);
	if (forceint)
	{
		setInt(ret,sys,num1/num2);
		if (o)
			o->decRef();
	}
	else if (replaceNumber(ret,sys,res) && o)
		o->decRef();
	
}

FORCE_INLINE void asAtomHandler::modulo(asAtom& a,SystemState* sys,asAtom &v2)
{
	// if both values are Integers the result is also an int
	if( ((a.uintval&0x7) == ATOM_INTEGER || (a.uintval&0x7) == ATOM_UINTEGER) &&
		(isInteger(v2) || (v2.uintval&0x7) ==ATOM_UINTEGER))
	{
		int32_t num1=toInt(a);
		int32_t num2=toInt(v2);
		LOG_CALL(_("moduloI ")  << num1 << '%' << num2);
		if (num2 == 0)
			setNumber(a,sys,numeric_limits<double>::quiet_NaN());
		else
			setInt(a,sys,num1%num2);
	}
	else
	{
		number_t num1=toNumber(a);
		number_t num2=toNumber(v2);
		LOG_CALL(_("modulo ")  << num1 << '%' << num2);
		/* fmod returns NaN if num2 == 0 as the spec mandates */
		setNumber(a,sys,::fmod(num1,num2));
	}
}
FORCE_INLINE void asAtomHandler::moduloreplace(asAtom& ret, SystemState* sys,const asAtom& v1, const asAtom &v2)
{
	// if both values are Integers the result is also an int
	if( ((v1.uintval&0x7) == ATOM_INTEGER || (v1.uintval&0x7) == ATOM_UINTEGER) &&
		(isInteger(v2) || (v2.uintval&0x7) ==ATOM_UINTEGER))
	{
		int32_t num1=toInt(v1);
		int32_t num2=toInt(v2);
		ASATOM_DECREF(ret);
		LOG_CALL(_("moduloreplaceI ")  << num1 << '%' << num2);
		if (num2 == 0)
			setNumber(ret,sys,numeric_limits<double>::quiet_NaN());
		else
			setInt(ret,sys,num1%num2);
	}
	else
	{
		number_t num1=toNumber(v1);
		number_t num2=toNumber(v2);
		LOG_CALL(_("moduloreplace ")  << num1 << '%' << num2);
		/* fmod returns NaN if num2 == 0 as the spec mandates */
		ASObject* o = getObject(ret);
		if (replaceNumber(ret,sys,::fmod(num1,num2)) && o)
			o->decRef();
	}
}

FORCE_INLINE void asAtomHandler::lshift(asAtom& a,SystemState* sys,asAtom &v1)
{
	int32_t i2=toInt(a);
	uint32_t i1=toUInt(v1)&0x1f;
	LOG_CALL(_("lShift ")<<std::hex<<i2<<_("<<")<<i1<<std::dec);
	//Left shift are supposed to always work in 32bit
	setInt(a,sys,i2<<i1);
}

FORCE_INLINE void asAtomHandler::rshift(asAtom& a,SystemState* sys,asAtom &v1)
{
	int32_t i2=toInt(a);
	uint32_t i1=toUInt(v1)&0x1f;
	LOG_CALL(_("rShift ")<<std::hex<<i2<<_(">>")<<i1<<std::dec);
	setInt(a,sys,i2>>i1);
}

FORCE_INLINE void asAtomHandler::urshift(asAtom& a,SystemState* sys,asAtom &v1)
{
	uint32_t i2=toUInt(a);
	uint32_t i1=toUInt(v1)&0x1f;
	LOG_CALL(_("urShift ")<<std::hex<<i2<<_(">>")<<i1<<std::dec);
	setInt(a,sys,i2>>i1);
}
FORCE_INLINE void asAtomHandler::bit_and(asAtom& a,SystemState* sys,asAtom &v1)
{
	int32_t i1=toInt(v1);
	int32_t i2=toInt(a);
	LOG_CALL(_("bitAnd_oo ") << std::hex << i1 << '&' << i2 << std::dec);
	setInt(a,sys,i1&i2);
}
FORCE_INLINE void asAtomHandler::bit_or(asAtom& a,SystemState* sys,asAtom &v1)
{
	int32_t i1=toInt(v1);
	int32_t i2=toInt(a);
	LOG_CALL(_("bitOr ") << std::hex << i1 << '|' << i2 << std::dec);
	setInt(a,sys,i1|i2);
}
FORCE_INLINE void asAtomHandler::_not(asAtom& a)
{
	LOG_CALL( _("not:") <<toDebugString(a)<<" "<<!Boolean_concrete(a));
	
	bool ret=!Boolean_concrete(a);
	setBool(a,ret);
}

FORCE_INLINE void asAtomHandler::negate_i(asAtom& a,SystemState* sys)
{
	LOG_CALL(_("negate_i"));
	int n=toInt(a);
	setInt(a,sys,-n);
}

FORCE_INLINE void asAtomHandler::add_i(asAtom& a,SystemState* sys,asAtom &v2)
{
	int64_t num2=toInt(v2);
	int64_t num1=toInt(a);

	LOG_CALL(_("add_i ") << num1 << '+' << num2);
	int64_t res = num1+num2;
	if (res >= INT32_MAX || res <= INT32_MIN)
		setNumber(a,sys,res);
	else
		setInt(a,sys,res);
}

FORCE_INLINE void asAtomHandler::subtract_i(asAtom& a,SystemState* sys,asAtom &v2)
{
	int num2=toInt(v2);
	int num1=toInt(a);

	LOG_CALL(_("subtract_i ") << num1 << '-' << num2);
	setInt(a,sys,num1-num2);
}

FORCE_INLINE void asAtomHandler::multiply_i(asAtom& a,SystemState* sys,asAtom &v2)
{
	int num1=toInt(a);
	int num2=toInt(v2);
	LOG_CALL(_("multiply_i ")  << num1 << '*' << num2);
	int64_t res = num1*num2;
	setInt(a,sys,res);
}

FORCE_INLINE bool asAtomHandler::isNumeric(const asAtom& a)
{
	return (isInteger(a) || isUInteger(a) || isNumber(a));
}
FORCE_INLINE bool asAtomHandler::isNumber(const asAtom& a)
{
	return (a.uintval&0x7)==ATOM_NUMBERPTR;
}
FORCE_INLINE bool asAtomHandler::isInteger(const asAtom& a)
{ 
	return (a.uintval&0x3) == ATOM_INTEGER || ((a.uintval&0x7) == ATOM_U_INTEGERPTR && isObject(a) && getObjectNoCheck(a)->getObjectType() == T_INTEGER);
}
FORCE_INLINE bool asAtomHandler::isUInteger(const asAtom& a)
{ 
	return (a.uintval&0x7) == ATOM_UINTEGER || ((a.uintval&0x7) == ATOM_U_INTEGERPTR  && isObject(a) && getObjectNoCheck(a)->getObjectType() == T_UINTEGER);
}
FORCE_INLINE asAtom asAtomHandler::fromObjectNoPrimitive(ASObject* obj)
{
	assert(!obj->isPrimitive());
	asAtom a;
	a.uintval = ATOM_OBJECTPTR | (LIGHTSPARK_ATOM_VALTYPE)obj;
	return a;
}

FORCE_INLINE SWFOBJECT_TYPE asAtomHandler::getObjectType(const asAtom& a)
{
	switch (a.uintval&0x7)
	{
		case ATOM_INTEGER:
			return T_INTEGER;
		case ATOM_UINTEGER:
			return T_UINTEGER;
		case ATOM_INVALID_UNDEFINED_NULL_BOOL:
			switch (a.uintval&0xf0)
			{
				case ATOMTYPE_NULL_BIT: 
					return T_NULL;
				case ATOMTYPE_UNDEFINED_BIT: // UNDEFINED
					return T_UNDEFINED;
				default: // BOOL
					return T_BOOLEAN;
			}
		case ATOM_NUMBERPTR:
			return T_NUMBER;
		case ATOM_STRINGID:
		case ATOM_STRINGPTR:
			return T_STRING;
		case ATOM_U_INTEGERPTR:
		case ATOM_OBJECTPTR:
			return isObject(a) ? getObjectNoCheck(a)->getObjectType() : T_INVALID;
		default:
			return T_INVALID;
	}
}
FORCE_INLINE asAtom asAtomHandler::typeOf(asAtom& a)
{
	BUILTIN_STRINGS ret=BUILTIN_STRINGS::STRING_OBJECT;
	switch(a.uintval&0x7)
	{
		case ATOM_INVALID_UNDEFINED_NULL_BOOL:
		{
			switch (a.uintval&0x70)
			{
				case ATOMTYPE_NULL_BIT:
					ret= BUILTIN_STRINGS::STRING_OBJECT;
					break;
				case ATOMTYPE_UNDEFINED_BIT:
					ret=BUILTIN_STRINGS::STRING_UNDEFINED;
					break;
				case ATOMTYPE_BOOL_BIT:
					ret=BUILTIN_STRINGS::STRING_BOOLEAN;
					break;
				default:
					break;
			}
			break;
		}
		case ATOM_OBJECTPTR:
			if(getObjectNoCheck(a)->is<XML>() || getObjectNoCheck(a)->is<XMLList>())
				ret = BUILTIN_STRINGS::STRING_XML;
			if(getObjectNoCheck(a)->is<Number>() || getObjectNoCheck(a)->is<Integer>() || getObjectNoCheck(a)->is<UInteger>())
				ret = BUILTIN_STRINGS::STRING_NUMBER;
			if(getObjectNoCheck(a)->is<ASString>())
				ret = BUILTIN_STRINGS::STRING_STRING;
			if(getObjectNoCheck(a)->is<IFunction>())
				ret = BUILTIN_STRINGS::STRING_FUNCTION_LOWERCASE;
			if(getObjectNoCheck(a)->is<Undefined>())
				ret = BUILTIN_STRINGS::STRING_UNDEFINED;
			if(getObjectNoCheck(a)->is<Boolean>())
				ret = BUILTIN_STRINGS::STRING_BOOLEAN;
			break;
		case ATOM_NUMBERPTR:
		case ATOM_INTEGER:
		case ATOM_UINTEGER:
		case ATOM_U_INTEGERPTR:
			ret=BUILTIN_STRINGS::STRING_NUMBER;
			break;
		case ATOM_STRINGID:
		case ATOM_STRINGPTR:
			ret = BUILTIN_STRINGS::STRING_STRING;
			break;
		default:
			break;
	}
	return asAtomHandler::fromStringID(ret);
}
bool asAtomHandler::isEqual(asAtom& a, SystemState *sys, asAtom &v2)
{
	if ((((a.intval ^ ATOM_INTEGER) | (v2.intval ^ ATOM_INTEGER)) & 7) == 0)
		return (a.intval == v2.intval);
	if (a.uintval == v2.uintval && 
			((a.uintval&0x7) != ATOM_NUMBERPTR)) // number needs special handling for NaN
		return true;
	return isEqualIntern(a,sys,v2);
}
TRISTATE asAtomHandler::isLess(asAtom& a,SystemState *sys, asAtom &v2)
{
	if ((((a.intval ^ ATOM_INTEGER) | (v2.intval ^ ATOM_INTEGER)) & 7) == 0)
		return (a.intval < v2.intval)?TTRUE:TFALSE;
	if (a.uintval == v2.uintval && 
			((a.uintval&0x7) != ATOM_NUMBERPTR)) // number needs special handling for NaN
	{
		return a.uintval == ATOMTYPE_UNDEFINED_BIT ? TUNDEFINED : TFALSE;
	}
	return isLessIntern(a,sys,v2);
}
/* implements ecma3's ToBoolean() operation, see section 9.2, but returns the value instead of an Boolean object */
FORCE_INLINE bool asAtomHandler::Boolean_concrete(asAtom& a)
{
	switch(a.uintval&0x7)
	{
		case ATOM_INVALID_UNDEFINED_NULL_BOOL:
		{
			switch (a.uintval&0x70)
			{
				case ATOMTYPE_NULL_BIT:
				case ATOMTYPE_UNDEFINED_BIT:
					return false;
				case ATOMTYPE_BOOL_BIT:
					return (a.uintval&0x80)>>7;
				default:
					return false;
			}
			break;
		}
		case ATOM_NUMBERPTR:
			return toNumber(a) != 0.0 && !std::isnan(toNumber(a));
		case ATOM_INTEGER:
			return (a.intval>>3) != 0;
		case ATOM_UINTEGER:
			return (a.uintval>>3) != 0;
		case ATOM_STRINGID:
			return (a.uintval>>3) != BUILTIN_STRINGS::EMPTY;
		default:
			return Boolean_concrete_object(a);
	}
}

FORCE_INLINE bool asAtomHandler::isFunction(const asAtom& a) { return isObject(a) && getObjectNoCheck(a)->is<IFunction>(); }
FORCE_INLINE bool asAtomHandler::isString(const asAtom& a) { return isStringID(a) || (isObject(a) && getObjectNoCheck(a)->is<ASString>()); }
FORCE_INLINE bool asAtomHandler::isQName(const asAtom& a) { return getObject(a) && getObjectNoCheck(a)->is<ASQName>(); }
FORCE_INLINE bool asAtomHandler::isNamespace(const asAtom& a) { return isObject(a) && getObjectNoCheck(a)->is<Namespace>(); }
FORCE_INLINE bool asAtomHandler::isArray(const asAtom& a) { return isObject(a) && getObjectNoCheck(a)->is<Array>(); }
FORCE_INLINE bool asAtomHandler::isClass(const asAtom& a) { return isObject(a) && getObjectNoCheck(a)->is<Class_base>(); }
FORCE_INLINE bool asAtomHandler::isTemplate(const asAtom& a) { return isObject(a) && getObjectNoCheck(a)->getObjectType() == T_TEMPLATE; }

FORCE_INLINE ASObject* asAtomHandler::getObject(const asAtom& a)
{
	assert(!(a.uintval & ATOMTYPE_OBJECT_BIT) || !((ASObject*)(a.uintval& ~((LIGHTSPARK_ATOM_VALTYPE)0x7)))->getCached());
	return a.uintval & ATOMTYPE_OBJECT_BIT ? (ASObject*)(a.uintval& ~((LIGHTSPARK_ATOM_VALTYPE)0x7)) : nullptr;
}
FORCE_INLINE ASObject* asAtomHandler::getObjectNoCheck(const asAtom& a)
{
	assert(!(a.uintval & ATOMTYPE_OBJECT_BIT) || !((ASObject*)(a.uintval& ~((LIGHTSPARK_ATOM_VALTYPE)0x7)))->getCached());
	return (ASObject*)(a.uintval& ~((LIGHTSPARK_ATOM_VALTYPE)0x7));
}
FORCE_INLINE void asAtomHandler::resetCached(const asAtom& a)
{
	ASObject* o = a.uintval & ATOMTYPE_OBJECT_BIT ? (ASObject*)(a.uintval& ~((LIGHTSPARK_ATOM_VALTYPE)0x7)) : nullptr;
	if (o)
		o->resetCached();
}

inline ASObject* asfreelist::getObjectFromFreeList()
{
#ifndef NDEBUG
	// all ASObjects must be created in the VM thread
	//assert_and_throw(isVmThread());
#endif
	assert(freelistsize>=0);
	ASObject* o = freelistsize ? freelist[--freelistsize] :nullptr;
	LOG_CALL("getfromfreelist:"<<freelistsize<<" "<<o<<" "<<this);
	return o;
}
inline bool asfreelist::pushObjectToFreeList(ASObject *obj)
{
#ifndef NDEBUG
	// all ASObjects must be created in the VM thread
	//assert_and_throw(isVmThread());
#endif
	assert(obj->isLastRef());
	if (freelistsize < FREELIST_SIZE)
	{
		assert(freelistsize>=0);
		LOG_CALL("pushtofreelist:"<<freelistsize<<" "<<obj<<" "<<this);
		obj->setCached();
		freelist[freelistsize++]=obj;
		return true;
	}
	return false;
}

}
#endif /* ASOBJECT_H */
