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

#include "scripting/abc.h"
#include "compat.h"
#include "exceptions.h"
#include "scripting/abcutils.h"
#include "scripting/class.h"
#include "scripting/toplevel/ASString.h"
#include "scripting/toplevel/RegExp.h"
#include "scripting/toplevel/Vector.h"
#include "parsing/streams.h"
#include <string>
#include <sstream>

using namespace std;
using namespace lightspark;

void ABCVm::abc_ifnlt_constant_constant(call_context* context)
{
	bool cond=!(asAtomHandler::isLess(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant) == TTRUE);
	LOG_CALL(_("ifNLT_cc (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifnlt_local_constant(call_context* context)
{
	bool cond=!(asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant) == TTRUE);
	LOG_CALL(_("ifNLT_lc (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifnlt_constant_local(call_context* context)
{
	bool cond=!(asAtomHandler::isLess(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2)) == TTRUE);
	LOG_CALL(_("ifNLT_cl (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifnlt_local_local(call_context* context)
{
	bool cond=!(asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2)) == TTRUE);
	LOG_CALL(_("ifNLT_ll (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifnge_constant_constant(call_context* context)
{
	bool cond=!(asAtomHandler::isLess(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant) == TFALSE);
	LOG_CALL("ifNGE_cc (" << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifnge_local_constant(call_context* context)
{
	bool cond=!(asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant) == TFALSE);
	LOG_CALL("ifNGE_lc (" << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifnge_constant_local(call_context* context)
{
	bool cond=!(asAtomHandler::isLess(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2)) == TFALSE);
	LOG_CALL("ifNGE_cl (" << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifnge_local_local(call_context* context)
{
	bool cond=!(asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2)) == TFALSE);
	LOG_CALL("ifNGE_ll (" << ((cond)?_("taken)"):_("not taken)"))<<" "<<asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1))<<" "<<asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2)));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_iftrue_constant(call_context* context)
{
	bool cond=asAtomHandler::Boolean_concrete(*context->exec_pos->arg1_constant);
	LOG_CALL(_("ifTrue_c (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_iftrue_local(call_context* context)
{
	bool cond=asAtomHandler::Boolean_concrete(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	LOG_CALL(_("ifTrue_l (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_iftrue_dup_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	bool cond=asAtomHandler::Boolean_concrete(*instrptr->arg1_constant);
	LOG_CALL(_("ifTrue_dup_c (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
	RUNTIME_STACK_PUSH(context,*instrptr->arg1_constant);
}
void ABCVm::abc_iftrue_dup_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	bool cond=asAtomHandler::Boolean_concrete(CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	LOG_CALL(_("ifTrue_dup_l (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
	ASATOM_INCREF(CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	RUNTIME_STACK_PUSH(context,CONTEXT_GETLOCAL(context,instrptr->local_pos1));
}
void ABCVm::abc_iffalse_constant(call_context* context)
{
	bool cond=!asAtomHandler::Boolean_concrete(*context->exec_pos->arg1_constant);
	LOG_CALL(_("ifFalse_c (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_iffalse_local(call_context* context)
{
	bool cond=!asAtomHandler::Boolean_concrete(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	LOG_CALL(_("ifFalse_l (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_iffalse_dup_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	bool cond=!asAtomHandler::Boolean_concrete(*instrptr->arg1_constant);
	LOG_CALL(_("ifFalse_dup_c (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
	RUNTIME_STACK_PUSH(context,*instrptr->arg1_constant);
}
void ABCVm::abc_iffalse_dup_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	bool cond=!asAtomHandler::Boolean_concrete(CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	LOG_CALL(_("ifFalse_dup_l (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
	ASATOM_INCREF(CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	RUNTIME_STACK_PUSH(context,CONTEXT_GETLOCAL(context,instrptr->local_pos1));
}
void ABCVm::abc_ifeq_constant_constant(call_context* context)
{
	bool cond=asAtomHandler::isEqual(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant);
	LOG_CALL(_("ifEq_cc (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifeq_local_constant(call_context* context)
{
	bool cond=asAtomHandler::isEqual(*context->exec_pos->arg2_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	LOG_CALL(_("ifEq_lc (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifeq_constant_local(call_context* context)
{
	bool cond=asAtomHandler::isEqual(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	LOG_CALL(_("ifEq_cl (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifeq_local_local(call_context* context)
{
	bool cond=asAtomHandler::isEqual(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	LOG_CALL(_("ifEq_ll (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifne_constant_constant(call_context* context)
{
	bool cond=!asAtomHandler::isEqual(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant) == TTRUE;
	LOG_CALL(_("ifNE_cc (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifne_local_constant(call_context* context)
{
	bool cond=!asAtomHandler::isEqual(*context->exec_pos->arg2_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	LOG_CALL(_("ifNE_lc (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifne_constant_local(call_context* context)
{
	bool cond=!asAtomHandler::isEqual(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	LOG_CALL(_("ifNE_cl (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifne_local_local(call_context* context)
{
	bool cond=!asAtomHandler::isEqual(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	LOG_CALL(_("ifNE_ll (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}

void ABCVm::abc_iflt_constant_constant(call_context* context)
{
	bool cond=asAtomHandler::isLess(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant) == TTRUE;
	LOG_CALL(_("ifLT_cc (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_iflt_local_constant(call_context* context)
{
	bool cond=asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant) == TTRUE;
	LOG_CALL(_("ifLT_lc (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_iflt_constant_local(call_context* context)
{
	bool cond=asAtomHandler::isLess(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2)) == TTRUE;
	LOG_CALL(_("ifLT_cl (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_iflt_local_local(call_context* context)
{
	bool cond=asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2)) == TTRUE;
	LOG_CALL(_("ifLT_ll (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifle_constant_constant(call_context* context)
{
	bool cond=asAtomHandler::isLess(*context->exec_pos->arg2_constant,context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant) == TFALSE;
	LOG_CALL(_("ifLE_cc (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifle_local_constant(call_context* context)
{
	bool cond=asAtomHandler::isLess(*context->exec_pos->arg2_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1)) == TFALSE;
	LOG_CALL(_("ifLE_lc (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifle_constant_local(call_context* context)
{
	bool cond=asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant) == TFALSE;
	LOG_CALL(_("ifLE_cl (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifle_local_local(call_context* context)
{
	bool cond=asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1)) == TFALSE;
	LOG_CALL(_("ifLE_ll (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifgt_constant_constant(call_context* context)
{
	bool cond=asAtomHandler::isLess(*context->exec_pos->arg2_constant,context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant) == TTRUE;
	LOG_CALL(_("ifGT_cc (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifgt_local_constant(call_context* context)
{
	bool cond=asAtomHandler::isLess(*context->exec_pos->arg2_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1)) == TTRUE;
	LOG_CALL(_("ifGT_lc (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifgt_constant_local(call_context* context)
{
	bool cond=asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant) == TTRUE;
	LOG_CALL(_("ifGT_cl (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifgt_local_local(call_context* context)
{
	bool cond=asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1)) == TTRUE;
	LOG_CALL(_("ifGT_ll (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifge_constant_constant(call_context* context)
{
	bool cond=asAtomHandler::isLess(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant) == TFALSE;
	LOG_CALL(_("ifGE_cc (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifge_local_constant(call_context* context)
{
	bool cond=asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant) == TFALSE;
	LOG_CALL(_("ifGE_lc (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifge_constant_local(call_context* context)
{
	bool cond=asAtomHandler::isLess(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2)) == TFALSE;
	LOG_CALL(_("ifGE_cl (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifge_local_local(call_context* context)
{
	bool cond=asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2)) == TFALSE;
	LOG_CALL(_("ifGE_ll (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifstricteq_constant_constant(call_context* context)
{
	bool cond=asAtomHandler::isEqualStrict(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant);
	LOG_CALL(_("ifstricteq_cc (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifstricteq_local_constant(call_context* context)
{
	bool cond=asAtomHandler::isEqualStrict(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant);
	LOG_CALL(_("ifstricteq_lc (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifstricteq_constant_local(call_context* context)
{
	bool cond=asAtomHandler::isEqualStrict(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	LOG_CALL(_("ifstricteq_cl (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifstricteq_local_local(call_context* context)
{
	LOG_CALL(_("ifstricteq_ll (") << asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1))<<" "<<asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2)));
	bool cond=asAtomHandler::isEqualStrict(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	LOG_CALL(_("ifstricteq_ll (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifstrictne_constant_constant(call_context* context)
{
	bool cond=!asAtomHandler::isEqualStrict(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant);
	LOG_CALL(_("ifstrictne_cc (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifstrictne_local_constant(call_context* context)
{
	bool cond=!asAtomHandler::isEqualStrict(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant);
	LOG_CALL(_("ifstrictne_lc (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifstrictne_constant_local(call_context* context)
{
	bool cond=!asAtomHandler::isEqualStrict(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	LOG_CALL(_("ifstrictne_cl (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_ifstrictne_local_local(call_context* context)
{
	bool cond=!asAtomHandler::isEqualStrict(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	LOG_CALL(_("ifstrictne_ll (") << ((cond)?_("taken)"):_("not taken)")));
	if(cond)
		context->exec_pos += context->exec_pos->arg3_int;
	else
		++(context->exec_pos);
}
void ABCVm::abc_pushcachedconstant(call_context* context)
{
	uint32_t t = context->exec_pos->arg3_uint;
	assert(t <= (uint32_t)context->mi->context->atomsCachedMaxID);
	asAtom a = context->mi->context->constantAtoms_cached[t];
	LOG_CALL("pushcachedconstant "<<t<<" "<<asAtomHandler::toDebugString(a));
	ASATOM_INCREF(a);
	RUNTIME_STACK_PUSH(context,a);
	++(context->exec_pos);
}
void ABCVm::abc_pushcachedslot(call_context* context)
{
	uint32_t t = context->exec_pos->arg3_uint;
	assert(t < context->mi->body->localconstantslots.size());
	asAtom a = *(context->localslots[context->mi->body->getReturnValuePos()+1+context->mi->body->localresultcount+t]);
	LOG_CALL("pushcachedslot "<<t<<" "<<asAtomHandler::toDebugString(a));
	ASATOM_INCREF(a);
	RUNTIME_STACK_PUSH(context,a);
	++(context->exec_pos);
}
void ABCVm::abc_getlexfromslot(call_context* context)
{
	uint32_t t = context->exec_pos->arg3_uint;

	ASObject* s = asAtomHandler::toObject(*context->locals,context->mi->context->root->getSystemState());
	asAtom a = s->getSlot(t);
	LOG_CALL("getlexfromslot "<<s->toDebugString()<<" "<<t);
	ASATOM_INCREF(a);
	RUNTIME_STACK_PUSH(context,a);
	++(context->exec_pos);
}
void ABCVm::abc_getlexfromslot_localresult(call_context* context)
{
	uint32_t t = context->exec_pos->arg3_uint;

	ASObject* s = asAtomHandler::toObject(*context->locals,context->mi->context->root->getSystemState());
	asAtom a = s->getSlot(t);
	LOG_CALL("getlexfromslot_l "<<s->toDebugString()<<" "<<t);
	ASATOM_INCREF(a);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),a);
	++(context->exec_pos);
}
void ABCVm::abc_pushScope_constant(call_context* context)
{
	//pushscope
	asAtom* t = context->exec_pos->arg1_constant;
	LOG_CALL( _("pushScope_c ") << asAtomHandler::toDebugString(*t) );
	assert_and_throw(context->curr_scope_stack < context->mi->body->max_scope_depth);
	ASATOM_INCREF_POINTER(t);
	context->scope_stack[context->curr_scope_stack] = *t;
	context->scope_stack_dynamic[context->curr_scope_stack] = false;
	context->curr_scope_stack++;
	++(context->exec_pos);
}
void ABCVm::abc_pushScope_local(call_context* context)
{
	//pushscope
	asAtom* t = &CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	LOG_CALL( _("pushScope_l ") << asAtomHandler::toDebugString(*t) );
	assert_and_throw(context->curr_scope_stack < context->mi->body->max_scope_depth);
	ASATOM_INCREF_POINTER(t);
	context->scope_stack[context->curr_scope_stack] = *t;
	context->scope_stack_dynamic[context->curr_scope_stack] = false;
	context->curr_scope_stack++;
	++(context->exec_pos);
}
void ABCVm::abc_li8_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	LOG_CALL( "li8_c");
	asAtom ret=asAtomHandler::invalidAtom;
	loadIntN<uint8_t>(context,ret,*instrptr->arg1_constant);
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_li8_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	LOG_CALL( "li8_l");
	asAtom ret=asAtomHandler::invalidAtom;
	loadIntN<uint8_t>(context,ret,CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_li8_constant_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	LOG_CALL( "li8_cl");
	asAtom ret=asAtomHandler::invalidAtom;
	loadIntN<uint8_t>(context,ret,*instrptr->arg1_constant);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),ret);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_li8_local_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	LOG_CALL( "li8_ll");
	asAtom ret=asAtomHandler::invalidAtom;
	loadIntN<uint8_t>(context,ret,CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),ret);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_li16_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	LOG_CALL( "li16_c");
	asAtom ret=asAtomHandler::invalidAtom;
	loadIntN<uint16_t>(context,ret,*instrptr->arg1_constant);
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_li16_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	LOG_CALL( "li16_l");
	asAtom ret=asAtomHandler::invalidAtom;
	loadIntN<uint16_t>(context,ret,CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_li16_constant_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	LOG_CALL( "li16_cl");
	asAtom ret=asAtomHandler::invalidAtom;
	loadIntN<uint16_t>(context,ret,*instrptr->arg1_constant);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),ret);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_li16_local_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	LOG_CALL( "li16_ll");
	asAtom ret=asAtomHandler::invalidAtom;
	loadIntN<uint16_t>(context,ret,CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),ret);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_li32_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	LOG_CALL( "li32_c");
	asAtom ret=asAtomHandler::invalidAtom;
	loadIntN<int32_t>(context,ret,*instrptr->arg1_constant);
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_li32_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	LOG_CALL( "li32_l");
	asAtom ret=asAtomHandler::invalidAtom;
	loadIntN<int32_t>(context,ret,CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_li32_constant_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	LOG_CALL( "li32_cl");
	asAtom ret=asAtomHandler::invalidAtom;
	loadIntN<int32_t>(context,ret,*instrptr->arg1_constant);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),ret);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_li32_local_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	LOG_CALL( "li32_ll");
	asAtom ret=asAtomHandler::invalidAtom;
	loadIntN<int32_t>(context,ret,CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),ret);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_lf32_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	LOG_CALL( "lf32_c");
	asAtom ret=asAtomHandler::invalidAtom;
	loadFloat(context,ret,*instrptr->arg1_constant);
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_lf32_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	LOG_CALL( "lf32_l");
	asAtom ret=asAtomHandler::invalidAtom;
	loadFloat(context,ret,CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_lf32_constant_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	LOG_CALL( "lf32_cl");
	asAtom ret=asAtomHandler::invalidAtom;
	loadFloat(context,ret,*instrptr->arg1_constant);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),ret);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_lf32_local_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	LOG_CALL( "lf32_ll");
	asAtom ret=asAtomHandler::invalidAtom;
	loadFloat(context,ret,CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),ret);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_lf64_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	LOG_CALL( "lf64_c");
	asAtom ret=asAtomHandler::invalidAtom;
	loadDouble(context,ret,*instrptr->arg1_constant);
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_lf64_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	LOG_CALL( "lf64_l");
	asAtom ret=asAtomHandler::invalidAtom;
	loadDouble(context,ret,CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_lf64_constant_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	LOG_CALL( "lf64_cl");
	asAtom ret=asAtomHandler::invalidAtom;
	loadDouble(context,ret,*instrptr->arg1_constant);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),ret);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_lf64_local_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	LOG_CALL( "lf64_ll");
	asAtom ret=asAtomHandler::invalidAtom;
	loadDouble(context,ret,CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),ret);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_si8_constant_constant(call_context* context)
{
	LOG_CALL( "si8_cc");
	preloadedcodedata* instrptr = context->exec_pos;
	storeIntN<uint8_t>(context,*instrptr->arg2_constant,*instrptr->arg1_constant);
	++(context->exec_pos);
}
void ABCVm::abc_si8_local_constant(call_context* context)
{
	LOG_CALL( "si8_lc");
	preloadedcodedata* instrptr = context->exec_pos;
	storeIntN<uint8_t>(context,*instrptr->arg2_constant,CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	++(context->exec_pos);
}
void ABCVm::abc_si8_constant_local(call_context* context)
{
	LOG_CALL( "si8_cl");
	preloadedcodedata* instrptr = context->exec_pos;
	storeIntN<uint8_t>(context,CONTEXT_GETLOCAL(context,instrptr->local_pos2),*instrptr->arg1_constant);
	++(context->exec_pos);
}
void ABCVm::abc_si8_local_local(call_context* context)
{
	LOG_CALL( "si8_ll");
	preloadedcodedata* instrptr = context->exec_pos;
	storeIntN<uint8_t>(context,CONTEXT_GETLOCAL(context,instrptr->local_pos2),CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	++(context->exec_pos);
}
void ABCVm::abc_si16_constant_constant(call_context* context)
{
	LOG_CALL( "si16_cc");
	preloadedcodedata* instrptr = context->exec_pos;
	storeIntN<uint16_t>(context,*instrptr->arg2_constant,*instrptr->arg1_constant);
	++(context->exec_pos);
}
void ABCVm::abc_si16_local_constant(call_context* context)
{
	LOG_CALL( "si16_lc");
	preloadedcodedata* instrptr = context->exec_pos;
	storeIntN<uint16_t>(context,*instrptr->arg2_constant,CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	++(context->exec_pos);
}
void ABCVm::abc_si16_constant_local(call_context* context)
{
	LOG_CALL( "si16_cl");
	preloadedcodedata* instrptr = context->exec_pos;
	storeIntN<uint16_t>(context,CONTEXT_GETLOCAL(context,instrptr->local_pos2),*instrptr->arg1_constant);
	++(context->exec_pos);
}
void ABCVm::abc_si16_local_local(call_context* context)
{
	LOG_CALL( "si16_ll");
	preloadedcodedata* instrptr = context->exec_pos;
	storeIntN<uint16_t>(context,CONTEXT_GETLOCAL(context,instrptr->local_pos2),CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	++(context->exec_pos);
}
void ABCVm::abc_si32_constant_constant(call_context* context)
{
	LOG_CALL( "si32_cc");
	preloadedcodedata* instrptr = context->exec_pos;
	storeIntN<uint32_t>(context,*instrptr->arg2_constant,*instrptr->arg1_constant);
	++(context->exec_pos);
}
void ABCVm::abc_si32_local_constant(call_context* context)
{
	LOG_CALL( "si32_lc");
	preloadedcodedata* instrptr = context->exec_pos;
	storeIntN<uint32_t>(context,*instrptr->arg2_constant,CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	++(context->exec_pos);
}
void ABCVm::abc_si32_constant_local(call_context* context)
{
	LOG_CALL( "si32_cl");
	preloadedcodedata* instrptr = context->exec_pos;
	storeIntN<uint32_t>(context,CONTEXT_GETLOCAL(context,instrptr->local_pos2),*instrptr->arg1_constant);
	++(context->exec_pos);
}
void ABCVm::abc_si32_local_local(call_context* context)
{
	LOG_CALL( "si32_ll");
	preloadedcodedata* instrptr = context->exec_pos;
	storeIntN<uint32_t>(context,CONTEXT_GETLOCAL(context,instrptr->local_pos2),CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	++(context->exec_pos);
}
void ABCVm::abc_sf32_constant_constant(call_context* context)
{
	LOG_CALL( "sf32_cc");
	preloadedcodedata* instrptr = context->exec_pos;
	storeFloat(context,*instrptr->arg2_constant,*instrptr->arg1_constant);
	++(context->exec_pos);
}
void ABCVm::abc_sf32_local_constant(call_context* context)
{
	LOG_CALL( "sf32_lc");
	preloadedcodedata* instrptr = context->exec_pos;
	storeFloat(context,*instrptr->arg2_constant,CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	++(context->exec_pos);
}
void ABCVm::abc_sf32_constant_local(call_context* context)
{
	LOG_CALL( "sf32_cl");
	preloadedcodedata* instrptr = context->exec_pos;
	storeFloat(context,CONTEXT_GETLOCAL(context,instrptr->local_pos2),*instrptr->arg1_constant);
	++(context->exec_pos);
}
void ABCVm::abc_sf32_local_local(call_context* context)
{
	LOG_CALL( "sf32_ll");
	preloadedcodedata* instrptr = context->exec_pos;
	storeFloat(context,CONTEXT_GETLOCAL(context,instrptr->local_pos2),CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	++(context->exec_pos);
}
void ABCVm::abc_sf64_constant_constant(call_context* context)
{
	LOG_CALL( "sf64_cc");
	preloadedcodedata* instrptr = context->exec_pos;
	storeDouble(context,*instrptr->arg2_constant,*instrptr->arg1_constant);
	++(context->exec_pos);
}
void ABCVm::abc_sf64_local_constant(call_context* context)
{
	LOG_CALL( "sf64_lc");
	preloadedcodedata* instrptr = context->exec_pos;
	storeDouble(context,*instrptr->arg2_constant,CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	++(context->exec_pos);
}
void ABCVm::abc_sf64_constant_local(call_context* context)
{
	LOG_CALL( "sf64_cl");
	preloadedcodedata* instrptr = context->exec_pos;
	storeDouble(context,CONTEXT_GETLOCAL(context,instrptr->local_pos2),*instrptr->arg1_constant);
	++(context->exec_pos);
}
void ABCVm::abc_sf64_local_local(call_context* context)
{
	LOG_CALL( "sf64_ll");
	preloadedcodedata* instrptr = context->exec_pos;
	storeDouble(context,CONTEXT_GETLOCAL(context,instrptr->local_pos2),CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	++(context->exec_pos);
}
void ABCVm::construct_noargs_intern(call_context* context,asAtom& ret,asAtom& obj)
{
	LOG_CALL(_("Constructing"));

	switch(asAtomHandler::getObjectType(obj))
	{
		case T_CLASS:
		{
			Class_base* o_class=asAtomHandler::as<Class_base>(obj);
			o_class->getInstance(ret,true,nullptr,0);
			break;
		}
		case T_FUNCTION:
		{
			constructFunction(ret,context, obj, nullptr,0);
			break;
		}

		default:
		{
			throwError<TypeError>(kConstructOfNonFunctionError);
		}
	}
	if (asAtomHandler::isObject(ret))
		asAtomHandler::getObject(ret)->setConstructorCallComplete();
	LOG_CALL("End of construct_noargs " << asAtomHandler::toDebugString(ret));
}
void ABCVm::abc_construct_constant(call_context* context)
{
	asAtom obj= *context->exec_pos->arg1_constant;
	LOG_CALL( "construct_noargs_c");
	asAtom ret=asAtomHandler::invalidAtom;
	construct_noargs_intern(context,ret,obj);
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_construct_local(call_context* context)
{
	asAtom obj= CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	LOG_CALL( "construct_noargs_l");
	asAtom ret=asAtomHandler::invalidAtom;
	construct_noargs_intern(context,ret,obj);
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_construct_constant_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj= *context->exec_pos->arg1_constant;
	LOG_CALL( "construct_noargs_c_lr");
	asAtom res=asAtomHandler::invalidAtom;
	construct_noargs_intern(context,res,obj);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),res);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_construct_local_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj= CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	LOG_CALL( "construct_noargs_l_lr ");
	asAtom res=asAtomHandler::invalidAtom;
	construct_noargs_intern(context,res,obj);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),res);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
FORCE_INLINE void callprop_intern(call_context* context,asAtom& ret,asAtom& obj,asAtom* args, uint32_t argsnum,multiname* name,preloadedcodedata* cacheptr,bool refcounted, bool needreturn, bool coercearguments)
{
	if ((cacheptr->local2.flags&ABC_OP_CACHED) == ABC_OP_CACHED)
	{
		if (asAtomHandler::isObject(obj) &&
				((asAtomHandler::is<Class_base>(obj) && asAtomHandler::getObjectNoCheck(obj) == cacheptr->cacheobj1)
				|| asAtomHandler::getObjectNoCheck(obj)->getClass() == cacheptr->cacheobj1))
		{
			asAtom o = asAtomHandler::fromObjectNoPrimitive(cacheptr->cacheobj3);
			LOG_CALL( "callProperty from cache:"<<*name<<" "<<asAtomHandler::toDebugString(obj)<<" "<<asAtomHandler::toDebugString(o)<<" "<<coercearguments);
			if(asAtomHandler::is<IFunction>(o))
				asAtomHandler::callFunction(o,ret,obj,args,argsnum,refcounted,needreturn && coercearguments,coercearguments);
			else if(asAtomHandler::is<Class_base>(o))
			{
				asAtomHandler::as<Class_base>(o)->generator(ret,args,argsnum);
				if (refcounted)
				{
					for(uint32_t i=0;i<argsnum;i++)
						ASATOM_DECREF(args[i]);
					ASATOM_DECREF(obj);
				}
			}
			else if(asAtomHandler::is<RegExp>(o))
				RegExp::exec(ret,context->mi->context->root->getSystemState(),o,args,argsnum);
			else
			{
				LOG(LOG_ERROR,"trying to call an object as a function:"<<asAtomHandler::toDebugString(o) <<" on "<<asAtomHandler::toDebugString(obj));
				throwError<TypeError>(kCallOfNonFunctionError, "Object");
			}
			if (needreturn && asAtomHandler::isInvalid(ret))
				ret = asAtomHandler::undefinedAtom;
			LOG_CALL("End of calling cached property "<<*name<<" "<<asAtomHandler::toDebugString(ret));
			return;
		}
		else
		{
			if (cacheptr->cacheobj3 && cacheptr->cacheobj3->is<Function>() && cacheptr->cacheobj3->as<IFunction>()->isCloned)
				cacheptr->cacheobj3->decRef();
			cacheptr->local2.flags |= ABC_OP_NOTCACHEABLE;
			cacheptr->local2.flags &= ~ABC_OP_CACHED;
		}
	}
	if(asAtomHandler::is<Null>(obj))
	{
		LOG(LOG_ERROR,"trying to call property on null:"<<*name);
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::is<Undefined>(obj))
	{
		LOG(LOG_ERROR,"trying to call property on undefined:"<<*name);
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}
	ASObject* pobj = asAtomHandler::getObject(obj);
	asAtom o=asAtomHandler::invalidAtom;
	bool canCache = false;
	if (!pobj)
	{
		// fast path for primitives to avoid creation of ASObjects
		asAtomHandler::getVariableByMultiname(obj,o,context->mi->context->root->getSystemState(),*name);
		canCache = asAtomHandler::isValid(o);
	}
	if(asAtomHandler::isInvalid(o))
	{
		pobj = asAtomHandler::toObject(obj,context->mi->context->root->getSystemState());
		//We should skip the special implementation of get
		canCache = pobj->getVariableByMultiname(o,*name, GET_VARIABLE_OPTION(SKIP_IMPL | NO_INCREF)) & GET_VARIABLE_RESULT::GETVAR_CACHEABLE;
	}
	name->resetNameIfObject();
	if(asAtomHandler::isInvalid(o) && asAtomHandler::is<Class_base>(obj))
	{
		// check super classes
		_NR<Class_base> tmpcls = asAtomHandler::as<Class_base>(obj)->super;
		while (tmpcls && !tmpcls.isNull())
		{
			tmpcls->getVariableByMultiname(o,*name, GET_VARIABLE_OPTION(SKIP_IMPL | NO_INCREF));
			if(asAtomHandler::isValid(o))
			{
				canCache = true;
				break;
			}
			tmpcls = tmpcls->super;
		}
	}
	if(asAtomHandler::isValid(o) && !asAtomHandler::is<Proxy>(obj))
	{
		if(asAtomHandler::is<IFunction>(o))
		{
			if (canCache
					&& (cacheptr->local2.flags & ABC_OP_NOTCACHEABLE)==0
					&& asAtomHandler::canCacheMethod(obj,name)
					&& asAtomHandler::getObject(o)
					&& ((asAtomHandler::is<Class_base>(obj) && asAtomHandler::as<IFunction>(o)->inClass == asAtomHandler::as<Class_base>(obj)) || (asAtomHandler::as<IFunction>(o)->inClass && asAtomHandler::getClass(obj,context->mi->context->root->getSystemState())->isSubClass(asAtomHandler::as<IFunction>(o)->inClass))))
			{
				// cache method if multiname is static and it is a method of a sealed class
				cacheptr->local2.flags |= ABC_OP_CACHED;
				if (argsnum==2 && asAtomHandler::is<SyntheticFunction>(o) && cacheptr->cacheobj1 && cacheptr->cacheobj3) // special case 2 parameters with known parameter types: check if coercion can be skipped
				{
					SyntheticFunction* f = asAtomHandler::as<SyntheticFunction>(o);
					if (!f->getMethodInfo()->returnType)
						f->checkParamTypes();
					if (f->getMethodInfo()->paramTypes.size()==2 &&
						f->canSkipCoercion(0,(Class_base*)cacheptr->cacheobj1) &&
						f->canSkipCoercion(1,(Class_base*)cacheptr->cacheobj3))
					{
						cacheptr->local2.flags |= ABC_OP_COERCED;
					}
				}
				cacheptr->cacheobj1 = asAtomHandler::getClass(obj,context->mi->context->root->getSystemState());
				cacheptr->cacheobj3 = asAtomHandler::getObject(o);
				LOG_CALL("caching callproperty:"<<*name<<" "<<cacheptr->cacheobj1->toDebugString()<<" "<<cacheptr->cacheobj3->toDebugString());
			}
			else
			{
				cacheptr->local2.flags |= ABC_OP_NOTCACHEABLE;
				cacheptr->local2.flags &= ~ABC_OP_CACHED;
			}
			obj = asAtomHandler::getClosureAtom(o,obj);
			asAtomHandler::callFunction(o,ret,obj,args,argsnum,refcounted,needreturn && coercearguments,coercearguments);
			if (!(cacheptr->local2.flags & ABC_OP_CACHED) && asAtomHandler::as<IFunction>(o)->isCloned)
				asAtomHandler::as<IFunction>(o)->decRef();
			if (needreturn && asAtomHandler::isInvalid(ret))
				ret = asAtomHandler::undefinedAtom;
		}
		else if(asAtomHandler::is<Class_base>(o))
		{
			asAtomHandler::as<Class_base>(o)->generator(ret,args,argsnum);
			if (refcounted)
			{
				for(uint32_t i=0;i<argsnum;i++)
					ASATOM_DECREF(args[i]);
				ASATOM_DECREF(obj);
			}
		}
		else if(asAtomHandler::is<RegExp>(o))
			RegExp::exec(ret,context->mi->context->root->getSystemState(),o,args,argsnum);
		else
		{
			LOG(LOG_ERROR,"trying to call an object as a function:"<<asAtomHandler::toDebugString(o) <<" on "<<asAtomHandler::toDebugString(obj));
			throwError<TypeError>(kCallOfNonFunctionError, "Object");
		}
	}
	else
	{
		//If the object is a Proxy subclass, try to invoke callProperty
		if(asAtomHandler::is<Proxy>(obj))
		{
			//Check if there is a custom caller defined, skipping implementation to avoid recursive calls
			multiname callPropertyName(NULL);
			callPropertyName.name_type=multiname::NAME_STRING;
			callPropertyName.name_s_id=context->mi->context->root->getSystemState()->getUniqueStringId("callProperty");
			callPropertyName.ns.emplace_back(context->mi->context->root->getSystemState(),flash_proxy,NAMESPACE);
			asAtom oproxy=asAtomHandler::invalidAtom;
			pobj->getVariableByMultiname(oproxy,callPropertyName,SKIP_IMPL);
			if(asAtomHandler::isValid(oproxy))
			{
				assert_and_throw(asAtomHandler::isFunction(oproxy));
				if(asAtomHandler::isValid(o))
				{
					if(asAtomHandler::is<IFunction>(o))
						asAtomHandler::callFunction(o,ret,obj,args,argsnum,refcounted,needreturn && coercearguments,coercearguments);
					else if(asAtomHandler::is<Class_base>(o))
					{
						asAtomHandler::as<Class_base>(o)->generator(ret,args,argsnum);
						if (refcounted)
						{
							for(uint32_t i=0;i<argsnum;i++)
								ASATOM_DECREF(args[i]);
							ASATOM_DECREF(obj);
						}
					}
					else if(asAtomHandler::is<RegExp>(o))
						RegExp::exec(ret,context->mi->context->root->getSystemState(),o,args,argsnum);
					else
					{
						LOG(LOG_ERROR,"trying to call an object as a function:"<<asAtomHandler::toDebugString(o) <<" on "<<asAtomHandler::toDebugString(obj));
						throwError<TypeError>(kCallOfNonFunctionError, "Object");
					}
				}
				else
				{
					//Create a new array
					asAtom* proxyArgs=g_newa(asAtom,argsnum+1);
					ASObject* namearg = abstract_s(context->mi->context->root->getSystemState(),name->normalizedName(context->mi->context->root->getSystemState()));
					namearg->setProxyProperty(*name);
					proxyArgs[0]=asAtomHandler::fromObject(namearg);
					for(uint32_t i=0;i<argsnum;i++)
						proxyArgs[i+1]=args[i];

					//We now suppress special handling
					LOG_CALL(_("Proxy::callProperty"));
					ASATOM_INCREF(oproxy);
					asAtomHandler::callFunction(oproxy,ret,obj,proxyArgs,argsnum+1,true,needreturn && coercearguments,coercearguments);
					ASATOM_DECREF(oproxy);
				}
				LOG_CALL(_("End of calling proxy custom caller ") << *name);
			}
			else if(asAtomHandler::isValid(o))
			{
				if(asAtomHandler::is<IFunction>(o))
					asAtomHandler::callFunction(o,ret,obj,args,argsnum,refcounted,needreturn && coercearguments,coercearguments);
				else if(asAtomHandler::is<Class_base>(o))
				{
					asAtomHandler::as<Class_base>(o)->generator(ret,args,argsnum);
					if (refcounted)
					{
						for(uint32_t i=0;i<argsnum;i++)
							ASATOM_DECREF(args[i]);
						ASATOM_DECREF(obj);
					}
				}
				else if(asAtomHandler::is<RegExp>(o))
					RegExp::exec(ret,context->mi->context->root->getSystemState(),o,args,argsnum);
				else
				{
					LOG(LOG_ERROR,"trying to call an object as a function:"<<asAtomHandler::toDebugString(o) <<" on "<<asAtomHandler::toDebugString(obj));
					throwError<TypeError>(kCallOfNonFunctionError, "Object");
				}
				LOG_CALL(_("End of calling proxy ") << *name);
			}
		}
		if (asAtomHandler::isInvalid(ret))
		{
			if (pobj->hasPropertyByMultiname(*name,true,true))
			{
				tiny_string clsname = pobj->getClass()->getQualifiedClassName();
				throwError<ReferenceError>(kWriteOnlyError, name->normalizedName(context->mi->context->root->getSystemState()), clsname);
			}
			if (pobj->getClass() && pobj->getClass()->isSealed)
			{
				tiny_string clsname = pobj->getClass()->getQualifiedClassName();
				throwError<ReferenceError>(kReadSealedError, name->normalizedName(context->mi->context->root->getSystemState()), clsname);
			}
			if (asAtomHandler::is<Class_base>(obj))
			{
				tiny_string clsname = asAtomHandler::as<Class_base>(obj)->class_name.getQualifiedName(context->mi->context->root->getSystemState());
				throwError<TypeError>(kCallOfNonFunctionError, name->qualifiedString(context->mi->context->root->getSystemState()), clsname);
			}
			else
			{
				tiny_string clsname = pobj->getClassName();
				throwError<TypeError>(kCallNotFoundError, name->qualifiedString(context->mi->context->root->getSystemState()), clsname);
			}
			asAtomHandler::setUndefined(ret);
		}
	}
	LOG_CALL(_("End of calling ") << *name<<" "<<asAtomHandler::toDebugString(ret));
}
void ABCVm::abc_callpropertyStaticName(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t argcount = context->exec_pos->local2.pos;
	multiname* name=(++context->exec_pos)->cachedmultiname2;
	LOG_CALL( "callProperty_staticname " << *name<<" "<<argcount);
	RUNTIME_STACK_POP_N_CREATE(context,argcount+1,args);
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,*args,args+1,argcount,name,instrptr,true,true,(instrptr->local2.flags&ABC_OP_COERCED)==0);
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_callpropertyStaticName_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));

	RUNTIME_STACK_POP_CREATE(context,args);
	multiname* name=instrptr->cachedmultiname2;

	LOG_CALL( "callProperty_l " << *name);
	RUNTIME_STACK_POP_CREATE(context,obj);
	asAtom res=asAtomHandler::invalidAtom;
	callprop_intern(context,res,*obj,args,1,name,context->exec_pos,true,true,(instrptr->local2.flags&ABC_OP_COERCED)==0);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),res);
	if (o)
		o->decRef();

	++(context->exec_pos);
}
void ABCVm::abc_callpropertyStaticNameCached(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t argcount = instrptr->local2.pos;
	asAtom* args = g_newa(asAtom,argcount);
	multiname* name=(context->exec_pos+1)->cachedmultiname3;
	LOG_CALL( "callProperty_staticnameCached " << *name<<" "<<argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	RUNTIME_STACK_POP_CREATE(context,obj);
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,*obj,args,argcount,name,instrptr,false,true,(instrptr->local2.flags&ABC_OP_COERCED)==0);
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_callpropertyStaticNameCached_localResult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t argcount = instrptr->local2.pos;
	asAtom* args = g_newa(asAtom,argcount);
	multiname* name=(context->exec_pos+1)->cachedmultiname3;
	LOG_CALL( "callProperty_staticnameCached_l " << *name<<" "<<argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	RUNTIME_STACK_POP_CREATE(context,obj);
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,*obj,args,argcount,name,instrptr,false,true,(instrptr->local2.flags&ABC_OP_COERCED)==0);
	// local result pos is stored in the context->exec_pos of the last argument
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_callpropertyStaticNameCached_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t argcount = instrptr->local2.pos;
	asAtom* args = g_newa(asAtom,argcount);
	assert(argcount>1);
	multiname* name=(context->exec_pos+1)->cachedmultiname3;
	LOG_CALL( "callProperty_staticnameCached_c " << *name<<" "<<argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	asAtom obj = *(++context->exec_pos)->arg2_constant;
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,obj,args,argcount,name,instrptr,false,true,(instrptr->local2.flags&ABC_OP_COERCED)==0);
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_callpropertyStaticNameCached_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t argcount = instrptr->local2.pos;
	asAtom* args = g_newa(asAtom,argcount);
	assert(argcount>1);
	multiname* name=(context->exec_pos+1)->cachedmultiname3;
	LOG_CALL( "callProperty_staticnameCached_l " << *name<<" "<<argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	asAtom obj = CONTEXT_GETLOCAL(context,(++context->exec_pos)->local_pos2);
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,obj,args,argcount,name,instrptr,false,true,(instrptr->local2.flags&ABC_OP_COERCED)==0);
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_callpropertyStaticNameCached_constant_localResult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t argcount = instrptr->local2.pos;
	asAtom* args = g_newa(asAtom,argcount);
	assert(argcount>1);
	multiname* name=(context->exec_pos+1)->cachedmultiname3;
	LOG_CALL( "callProperty_staticnameCached_cl " << *name<<" "<<argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	asAtom obj = *(++context->exec_pos)->arg2_constant;
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,obj,args,argcount,name,instrptr,false,true,(instrptr->local2.flags&ABC_OP_COERCED)==0);
	// local result pos is stored in the context->exec_pos of the last argument
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_callpropertyStaticNameCached_local_localResult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t argcount = instrptr->local2.pos;
	asAtom* args = g_newa(asAtom,argcount);
	assert(argcount>1);
	multiname* name=(context->exec_pos+1)->cachedmultiname3;
	LOG_CALL( "callProperty_staticnameCached_ll " << *name<<" "<<argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	asAtom obj = CONTEXT_GETLOCAL(context,(++context->exec_pos)->local_pos2);
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,obj,args,argcount,name,instrptr,false,true,(instrptr->local2.flags&ABC_OP_COERCED)==0);
	// local result pos is stored in the context->exec_pos of the last argument
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_callpropertyStaticName_constant_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	multiname* name=(++(context->exec_pos))->cachedmultiname2;
	bool coerce = ((++(context->exec_pos))->local2.flags&ABC_OP_COERCED)==0;

	asAtom args=*instrptr->arg2_constant;
	asAtom obj= *instrptr->arg1_constant;
	LOG_CALL( "callProperty_cc " << *name);
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,obj,&args,1,name,context->exec_pos,false,true,coerce);
	RUNTIME_STACK_PUSH(context,ret);

	++(context->exec_pos);
}
void ABCVm::abc_callpropertyStaticName_local_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	multiname* name=(++(context->exec_pos))->cachedmultiname2;
	bool coerce = ((++(context->exec_pos))->local2.flags&ABC_OP_COERCED)==0;

	asAtom args=*instrptr->arg2_constant;

	asAtom obj= CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	LOG_CALL( "callProperty_lc " << *name);
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,obj,&args,1,name,context->exec_pos,false,true,coerce);
	RUNTIME_STACK_PUSH(context,ret);

	++(context->exec_pos);
}
void ABCVm::abc_callpropertyStaticName_constant_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	multiname* name=(++(context->exec_pos))->cachedmultiname2;
	bool coerce = ((++(context->exec_pos))->local2.flags&ABC_OP_COERCED)==0;

	asAtom* args=&CONTEXT_GETLOCAL(context,instrptr->local_pos2);

	asAtom obj= *instrptr->arg1_constant;
	LOG_CALL( "callProperty_cl " << *name);
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,obj,args,1,name,context->exec_pos,false,true,coerce);
	RUNTIME_STACK_PUSH(context,ret);

	++(context->exec_pos);
}
void ABCVm::abc_callpropertyStaticName_local_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	multiname* name=(++(context->exec_pos))->cachedmultiname2;
	bool coerce = ((++(context->exec_pos))->local2.flags&ABC_OP_COERCED)==0;

	asAtom* args=&CONTEXT_GETLOCAL(context,instrptr->local_pos2);

	asAtom obj= CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	LOG_CALL( "callProperty_ll " << *name<<" "<<instrptr->local_pos1<<" "<<instrptr->local_pos2<<" "<<asAtomHandler::toDebugString(obj)<<" "<<asAtomHandler::toDebugString(*args));
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,obj,args,1,name,context->exec_pos,false,true,coerce);
	RUNTIME_STACK_PUSH(context,ret);

	++(context->exec_pos);
}
void ABCVm::abc_callpropertyStaticName_constant_constant_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	multiname* name=(++(context->exec_pos))->cachedmultiname2;
	bool coerce = ((++(context->exec_pos))->local2.flags&ABC_OP_COERCED)==0;

	asAtom args=*instrptr->arg2_constant;

	asAtom obj= *instrptr->arg1_constant;
	LOG_CALL( "callProperty_ccl " << *name);
	asAtom res=asAtomHandler::invalidAtom;
	callprop_intern(context,res,obj,&args,1,name,context->exec_pos,false,true,coerce);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),res);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_callpropertyStaticName_local_constant_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	multiname* name=(++(context->exec_pos))->cachedmultiname2;
	bool coerce = ((++(context->exec_pos))->local2.flags&ABC_OP_COERCED)==0;

	asAtom args=*instrptr->arg2_constant;

	asAtom obj= CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	LOG_CALL( "callProperty_lcl " << *name);
	asAtom res=asAtomHandler::invalidAtom;
	callprop_intern(context,res,obj,&args,1,name,context->exec_pos,false,true,coerce);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),res);
	if (o)
		o->decRef();

	++(context->exec_pos);
}
void ABCVm::abc_callpropertyStaticName_constant_local_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	multiname* name=(++(context->exec_pos))->cachedmultiname2;
	bool coerce = ((++(context->exec_pos))->local2.flags&ABC_OP_COERCED)==0;

	asAtom* args=&CONTEXT_GETLOCAL(context,instrptr->local_pos2);

	asAtom obj= *instrptr->arg1_constant;
	LOG_CALL( "callProperty_cll " << *name);
	asAtom res=asAtomHandler::invalidAtom;
	callprop_intern(context,res,obj,args,1,name,context->exec_pos,false,true,coerce);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),res);
	if (o)
		o->decRef();

	++(context->exec_pos);
}
void ABCVm::abc_callpropertyStaticName_local_local_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	multiname* name=(++(context->exec_pos))->cachedmultiname2;
	bool coerce = ((++(context->exec_pos))->local2.flags&ABC_OP_COERCED)==0;

	asAtom* args=&CONTEXT_GETLOCAL(context,instrptr->local_pos2);

	asAtom obj= CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	LOG_CALL( "callProperty_lll " << *name);
	asAtom res=asAtomHandler::invalidAtom;
	callprop_intern(context,res,obj,args,1,name,context->exec_pos,false,true,coerce);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),res);
	if (o)
		o->decRef();

	++(context->exec_pos);
}
void ABCVm::abc_callpropertyStaticName_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	multiname* name=context->exec_pos->cachedmultiname2;
	(++(context->exec_pos));


	asAtom obj= *instrptr->arg1_constant;
	LOG_CALL( "callProperty_noargs_c " << *name);
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,obj,nullptr,0,name,context->exec_pos,false,true,false);
	RUNTIME_STACK_PUSH(context,ret);

	++(context->exec_pos);
}
void ABCVm::abc_callpropertyStaticName_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	multiname* name=context->exec_pos->cachedmultiname2;
	(++(context->exec_pos));


	asAtom obj= CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	LOG_CALL( "callProperty_noargs_l " << *name);
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,obj,nullptr,0,name,context->exec_pos,false,true,false);
	RUNTIME_STACK_PUSH(context,ret);

	++(context->exec_pos);
}
void ABCVm::abc_callpropertyStaticName_constant_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	multiname* name=context->exec_pos->cachedmultiname2;
	(++(context->exec_pos));


	asAtom obj= *instrptr->arg1_constant;
	LOG_CALL( "callProperty_noargs_c_lr " << *name);
	asAtom res=asAtomHandler::invalidAtom;
	callprop_intern(context,res,obj,nullptr,0,name,context->exec_pos,false,true,false);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),res);
	if (o)
		o->decRef();

	++(context->exec_pos);
}
void ABCVm::abc_callpropertyStaticName_local_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	multiname* name=context->exec_pos->cachedmultiname2;
	(++(context->exec_pos));

	asAtom obj= CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	LOG_CALL( "callProperty_noargs_l_lr " << *name);
	asAtom res=asAtomHandler::invalidAtom;
	callprop_intern(context,res,obj,nullptr,0,name,context->exec_pos,false,true,false);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),res);
	if (o)
		o->decRef();

	++(context->exec_pos);
}
void ABCVm::abc_returnvalue_constant(call_context* context)
{
	asAtomHandler::set(context->locals[context->mi->body->getReturnValuePos()],*context->exec_pos->arg1_constant);
	LOG_CALL(_("returnValue_c ") << asAtomHandler::toDebugString(context->locals[context->mi->body->getReturnValuePos()]));
	++(context->exec_pos);
}
void ABCVm::abc_returnvalue_local(call_context* context)
{
	asAtomHandler::set(context->locals[context->mi->body->getReturnValuePos()],CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	LOG_CALL(_("returnValue_l ") << asAtomHandler::toDebugString(context->locals[context->mi->body->getReturnValuePos()]));
	ASATOM_INCREF(context->locals[context->mi->body->getReturnValuePos()]);
	++(context->exec_pos);
}
void ABCVm::abc_constructsuper_constant(call_context* context)
{
	LOG_CALL( _("constructSuper_c "));
	asAtom obj=*context->exec_pos->arg1_constant;
	context->inClass->super->handleConstruction(obj,nullptr, 0, false);
	LOG_CALL(_("End super construct ")<<asAtomHandler::toDebugString(obj));
	++(context->exec_pos);
}
void ABCVm::abc_constructsuper_local(call_context* context)
{
	LOG_CALL( _("constructSuper_l "));
	asAtom obj= CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	context->inClass->super->handleConstruction(obj,nullptr, 0, false);
	LOG_CALL(_("End super construct ")<<asAtomHandler::toDebugString(obj));
	++(context->exec_pos);
}
void ABCVm::constructpropnoargs_intern(call_context* context,asAtom& ret,asAtom& obj,multiname* name, ASObject* constructor)
{
	asAtom o=asAtomHandler::invalidAtom;
	if (constructor)
		o = asAtomHandler::fromObjectNoPrimitive(constructor);
	else
		asAtomHandler::toObject(obj,context->mi->context->root->getSystemState())->getVariableByMultiname(o,*name);

	if(asAtomHandler::isInvalid(o))
	{
		if (asAtomHandler::is<Undefined>(obj))
		{
			ASATOM_DECREF(obj);
			throwError<TypeError>(kConvertUndefinedToObjectError);
		}
		if (asAtomHandler::isPrimitive(obj))
		{
			ASATOM_DECREF(obj);
			throwError<TypeError>(kConstructOfNonFunctionError);
		}
		throwError<ReferenceError>(kUndefinedVarError, name->normalizedNameUnresolved(context->mi->context->root->getSystemState()));
	}
	try
	{
		if(asAtomHandler::isClass(o))
		{
			Class_base* o_class=asAtomHandler::as<Class_base>(o);
			o_class->getInstance(ret,true,nullptr,0);
		}
		else if(asAtomHandler::isFunction(o))
		{
			constructFunction(ret,context, o, nullptr, 0);
		}
		else if (asAtomHandler::isTemplate(o))
			throwError<TypeError>(kConstructOfNonFunctionError);
		else
			throwError<TypeError>(kNotConstructorError);
	}
	catch(ASObject* exc)
	{
		LOG_CALL(_("Exception during object construction. Returning Undefined"));
		//Handle eventual exceptions from the constructor, to fix the stack
		RUNTIME_STACK_PUSH(context,obj);
		throw;
	}
	if (asAtomHandler::isObject(ret))
		asAtomHandler::getObjectNoCheck(ret)->setConstructorCallComplete();
	LOG_CALL(_("End of constructing ") << asAtomHandler::toDebugString(ret));
}
void ABCVm::abc_constructpropStaticName_constant(call_context* context)
{
	multiname* name=context->exec_pos->cachedmultiname2;
	asAtom obj= *context->exec_pos->arg1_constant;
	LOG_CALL( "constructprop_noargs_c "<<*name);
	++(context->exec_pos);
	asAtom ret=asAtomHandler::invalidAtom;
	constructpropnoargs_intern(context,ret,obj,name,context->exec_pos->cacheobj2);
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_constructpropStaticName_local(call_context* context)
{
	multiname* name=context->exec_pos->cachedmultiname2;
	asAtom obj= CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	++(context->exec_pos);
	LOG_CALL( "constructprop_noargs_l " << *name);
	asAtom ret=asAtomHandler::invalidAtom;
	constructpropnoargs_intern(context,ret,obj,name,nullptr);
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_constructpropStaticName_constant_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	multiname* name=context->exec_pos->cachedmultiname2;
	asAtom obj= *context->exec_pos->arg1_constant;
	++(context->exec_pos);
	LOG_CALL( "constructprop_noargs_c_lr " << *name);
	asAtom res=asAtomHandler::invalidAtom;
	constructpropnoargs_intern(context,res,obj,name,context->exec_pos->cacheobj2);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),res);
	if (o && o != asAtomHandler::getObject(res))
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_constructpropStaticName_local_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	multiname* name=context->exec_pos->cachedmultiname2;
	asAtom obj= CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	++(context->exec_pos);
	LOG_CALL( "constructprop_noargs_l_lr " << *name);
	asAtom res=asAtomHandler::invalidAtom;
	constructpropnoargs_intern(context,res,obj,name,nullptr);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),res);
	if (o && o != asAtomHandler::getObject(res))
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_callpropvoidStaticName(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t argcount = instrptr->local2.pos;
	multiname* name=(++context->exec_pos)->cachedmultiname2;
	LOG_CALL( "callPropvoid_staticname " << *name<<" "<<argcount);
	RUNTIME_STACK_POP_N_CREATE(context,argcount+1,args);
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,*args,args+1,argcount,name,instrptr,true,false,(instrptr->local2.flags&ABC_OP_COERCED)==0);
	ASATOM_DECREF(ret);
	++(context->exec_pos);
}
void ABCVm::abc_callpropvoidStaticNameCached(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t argcount = instrptr->local2.pos;
	asAtom* args = g_newa(asAtom,argcount);
	multiname* name=(context->exec_pos+1)->cachedmultiname3;
	LOG_CALL( "callPropvoid_staticnameCached " << *name<<" "<<argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	RUNTIME_STACK_POP_CREATE(context,obj);
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,*obj,args,argcount,name,instrptr,false,false,(instrptr->local2.flags&ABC_OP_COERCED)==0);
	ASATOM_DECREF(ret);
	++(context->exec_pos);
}
void ABCVm::abc_callpropvoidStaticNameCached_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t argcount = instrptr->local2.pos;
	asAtom* args = g_newa(asAtom,argcount);
	multiname* name=(context->exec_pos+1)->cachedmultiname3;
	LOG_CALL( "callPropvoid_staticnameCached_c " << *name<<" "<<argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	asAtom obj = *(++context->exec_pos)->arg2_constant;
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,obj,args,argcount,name,instrptr,false,false,(instrptr->local2.flags&ABC_OP_COERCED)==0);
	ASATOM_DECREF(ret);
	++(context->exec_pos);
}
void ABCVm::abc_callpropvoidStaticNameCached_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t argcount = instrptr->local2.pos;
	asAtom* args = g_newa(asAtom,argcount);
	multiname* name=(context->exec_pos+1)->cachedmultiname3;
	LOG_CALL( "callPropvoid_staticnameCached_l " << *name<<" "<<argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	asAtom obj = CONTEXT_GETLOCAL(context,(++context->exec_pos)->local_pos2);
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,obj,args,argcount,name,instrptr,false,false,(instrptr->local2.flags&ABC_OP_COERCED)==0);
	ASATOM_DECREF(ret);
	++(context->exec_pos);
}
void ABCVm::abc_callpropvoidStaticName_constant_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));

	asAtom args=*instrptr->arg2_constant;
	multiname* name=context->exec_pos->cachedmultiname2;
	(++(context->exec_pos));

	asAtom obj= *instrptr->arg1_constant;
	LOG_CALL( "callPropvoid_cc " << *name);
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,obj,&args,1,name,context->exec_pos,false,false,(context->exec_pos->local2.flags&ABC_OP_COERCED)==0);
	ASATOM_DECREF(ret);

	++(context->exec_pos);
}
void ABCVm::abc_callpropvoidStaticName_local_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));

	asAtom args=*instrptr->arg2_constant;
	multiname* name=context->exec_pos->cachedmultiname2;
	(++(context->exec_pos));

	asAtom obj= CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	LOG_CALL( "callPropvoid_lc " << *name);
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,obj,&args,1,name,context->exec_pos,false,false,(context->exec_pos->local2.flags&ABC_OP_COERCED)==0);
	ASATOM_DECREF(ret);

	++(context->exec_pos);
}
void ABCVm::abc_callpropvoidStaticName_constant_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));

	asAtom* args=&CONTEXT_GETLOCAL(context,instrptr->local_pos2);
	multiname* name=context->exec_pos->cachedmultiname2;
	(++(context->exec_pos));

	asAtom obj= *instrptr->arg1_constant;
	LOG_CALL( "callPropvoid_cl " << *name);
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,obj,args,1,name,context->exec_pos,false,false,(context->exec_pos->local2.flags&ABC_OP_COERCED)==0);
	ASATOM_DECREF(ret);
	++(context->exec_pos);
}
void ABCVm::abc_callpropvoidStaticName_local_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));

	asAtom* args=&CONTEXT_GETLOCAL(context,instrptr->local_pos2);
	multiname* name=context->exec_pos->cachedmultiname2;
	(++(context->exec_pos));

	asAtom obj= CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	LOG_CALL( "callPropvoid_ll " << *name);
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,obj,args,1,name,context->exec_pos,false,false,(context->exec_pos->local2.flags&ABC_OP_COERCED)==0);
	ASATOM_DECREF(ret);

	++(context->exec_pos);
}
void ABCVm::abc_callpropvoidStaticName_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	multiname* name=context->exec_pos->cachedmultiname2;
	(++(context->exec_pos));

	asAtom obj= *instrptr->arg1_constant;
	LOG_CALL( "callPropvoid_noargs_c " << *name);
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,obj,nullptr,0,name,context->exec_pos,false,false,false);
	ASATOM_DECREF(ret);

	++(context->exec_pos);
}
void ABCVm::abc_callpropvoidStaticName_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	multiname* name=context->exec_pos->cachedmultiname2;
	(++(context->exec_pos));

	asAtom obj= CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	LOG_CALL( "callPropvoid_noargs_l " << *name);
	asAtom ret=asAtomHandler::invalidAtom;
	callprop_intern(context,ret,obj,nullptr,0,name,context->exec_pos,false,false,false);
	ASATOM_DECREF(ret);

	++(context->exec_pos);
}
void ABCVm::abc_getlex_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	assert(instrptr->local3.pos > 0);
	if ((instrptr->local3.flags&ABC_OP_CACHED) == ABC_OP_CACHED)
	{
		if(instrptr->cacheobj2)
			instrptr->cacheobj2->incRef();
		ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
		asAtomHandler::setFunction(CONTEXT_GETLOCAL(context,instrptr->local3.pos),instrptr->cacheobj1,nullptr);//,instrptr->cacheobj2);
		ASATOM_INCREF(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
		if (o)
			o->decRef();
		LOG_CALL( "getLex_l from cache: " <<  instrptr->cacheobj1->toDebugString());
	}
	else if (getLex_multiname(context,instrptr->cachedmultiname2,instrptr->local3.pos))
	{
		// put object in cache
		instrptr->local3.flags = ABC_OP_CACHED;
		instrptr->cacheobj1 = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
		instrptr->cacheobj2 = asAtomHandler::getClosure(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	}
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyStaticName(call_context* context)
{
	multiname* name=context->exec_pos->cachedmultiname2;
	RUNTIME_STACK_POP_CREATE(context,value);
	RUNTIME_STACK_POP_CREATE(context,obj);

	LOG_CALL(_("setProperty_s ") << *name << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));

	if(asAtomHandler::isNull(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << *name << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << *name << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}

	ASObject* o = asAtomHandler::toObject(*obj,context->mi->context->root->getSystemState());
	multiname* simplesettername = nullptr;
	if (context->exec_pos->local3.pos == 0x68)//initproperty
		simplesettername =o->setVariableByMultiname(*name,*value,ASObject::CONST_ALLOWED);
	else//Do not allow to set contant traits
		simplesettername =o->setVariableByMultiname(*name,*value,ASObject::CONST_NOT_ALLOWED);
	if (simplesettername)
		context->exec_pos->cachedmultiname2 = simplesettername;
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyStaticName_constant_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));
	multiname* name=context->exec_pos->cachedmultiname2;
	asAtom* obj = instrptr->arg1_constant;
	asAtom* value = instrptr->arg2_constant;

	LOG_CALL(_("setProperty_scc ") << *name << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));

	if(asAtomHandler::isNull(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << *name << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << *name << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}

	ASObject* o = asAtomHandler::toObject(*obj,context->mi->context->root->getSystemState());
	multiname* simplesettername = nullptr;
	if (context->exec_pos->local3.pos == 0x68)//initproperty
		simplesettername =o->setVariableByMultiname(*name,*value,ASObject::CONST_ALLOWED);
	else//Do not allow to set contant traits
		simplesettername =o->setVariableByMultiname(*name,*value,ASObject::CONST_NOT_ALLOWED);
	if (simplesettername)
		context->exec_pos->cachedmultiname2 = simplesettername;
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyStaticName_local_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));
	multiname* name=context->exec_pos->cachedmultiname2;
	asAtom* obj = &CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	asAtom* value = instrptr->arg2_constant;

	LOG_CALL(_("setProperty_slc ") << *name << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value)<<" "<<instrptr->local_pos1);

	if(asAtomHandler::isNull(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << *name << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << *name << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}
	ASObject* o = asAtomHandler::toObject(*obj,context->mi->context->root->getSystemState());
	multiname* simplesettername = nullptr;
	if (context->exec_pos->local3.pos == 0x68)//initproperty
		simplesettername =o->setVariableByMultiname(*name,*value,ASObject::CONST_ALLOWED);
	else//Do not allow to set contant traits
		simplesettername =o->setVariableByMultiname(*name,*value,ASObject::CONST_NOT_ALLOWED);
	if (simplesettername)
		context->exec_pos->cachedmultiname2 = simplesettername;
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyStaticName_constant_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));
	multiname* name=context->exec_pos->cachedmultiname2;
	asAtom* obj = instrptr->arg1_constant;
	asAtom* value = &CONTEXT_GETLOCAL(context,instrptr->local_pos2);

	LOG_CALL(_("setProperty_scl ") << *name << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));

	if(asAtomHandler::isNull(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << *name << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << *name << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}
	ASObject* o = asAtomHandler::toObject(*obj,context->mi->context->root->getSystemState());
	ASATOM_INCREF_POINTER(value);
	multiname* simplesettername = nullptr;
	if (context->exec_pos->local3.pos == 0x68)//initproperty
		simplesettername =o->setVariableByMultiname(*name,*value,ASObject::CONST_ALLOWED);
	else//Do not allow to set contant traits
		simplesettername =o->setVariableByMultiname(*name,*value,ASObject::CONST_NOT_ALLOWED);
	if (simplesettername)
		context->exec_pos->cachedmultiname2 = simplesettername;
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyStaticName_local_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));
	multiname* name=context->exec_pos->cachedmultiname2;
	asAtom* obj = &CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	asAtom* value = &CONTEXT_GETLOCAL(context,instrptr->local_pos2);

	LOG_CALL(_("setProperty_sll ") << *name << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));

	if(asAtomHandler::isNull(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << *name << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << *name << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}
	ASObject* o = asAtomHandler::toObject(*obj,context->mi->context->root->getSystemState());
	ASATOM_INCREF_POINTER(value);
	multiname* simplesettername = nullptr;
	if (context->exec_pos->local3.pos == 0x68)//initproperty
		simplesettername =o->setVariableByMultiname(*name,*value,ASObject::CONST_ALLOWED);
	else//Do not allow to set contant traits
		simplesettername =o->setVariableByMultiname(*name,*value,ASObject::CONST_NOT_ALLOWED);
	if (simplesettername)
		context->exec_pos->cachedmultiname2 = simplesettername;
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyInteger(call_context* context)
{
	RUNTIME_STACK_POP_CREATE(context,value);
	RUNTIME_STACK_POP_CREATE(context,idx);
	int index = asAtomHandler::getInt(*idx);
	RUNTIME_STACK_POP_CREATE(context,obj);

	LOG_CALL(_("setPropertyInteger ") << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));

	if(asAtomHandler::isNull(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}

	ASObject* o = asAtomHandler::getObject(*obj);
	if (!o)
		o = asAtomHandler::toObject(*obj,context->mi->context->root->getSystemState());
	if (context->exec_pos->local3.pos == 0x68)//initproperty
		o->setVariableByInteger(index,*value,ASObject::CONST_ALLOWED);
	else//Do not allow to set contant traits
		o->setVariableByInteger(index,*value,ASObject::CONST_NOT_ALLOWED);
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyInteger_constant_constant_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));
	asAtom* obj = instrptr->arg3_constant;
	int index = asAtomHandler::getInt(*instrptr->arg1_constant);
	asAtom* value = instrptr->arg2_constant;

	LOG_CALL(_("setProperty_i_ccc ") << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));

	if(asAtomHandler::isNull(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}

	ASObject* o = asAtomHandler::getObject(*obj);
	if (!o)
		o = asAtomHandler::toObject(*obj,context->mi->context->root->getSystemState());
	if (context->exec_pos->local3.pos == 0x68)//initproperty
		o->setVariableByInteger(index,*value,ASObject::CONST_ALLOWED);
	else//Do not allow to set contant traits
		o->setVariableByInteger(index,*value,ASObject::CONST_NOT_ALLOWED);
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyInteger_constant_local_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));
	asAtom* obj = instrptr->arg3_constant;
	int index = asAtomHandler::getInt(CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	asAtom* value = instrptr->arg2_constant;

	LOG_CALL(_("setProperty_i_clc ") << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));

	if(asAtomHandler::isNull(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}

	ASObject* o = asAtomHandler::getObject(*obj);
	if (!o)
		o = asAtomHandler::toObject(*obj,context->mi->context->root->getSystemState());
	if (context->exec_pos->local3.pos == 0x68)//initproperty
		o->setVariableByInteger(index,*value,ASObject::CONST_ALLOWED);
	else//Do not allow to set contant traits
		o->setVariableByInteger(index,*value,ASObject::CONST_NOT_ALLOWED);
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyInteger_constant_constant_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));
	asAtom* obj = instrptr->arg3_constant;
	int index = asAtomHandler::getInt(*instrptr->arg1_constant);
	asAtom* value = &CONTEXT_GETLOCAL(context,instrptr->local_pos2);

	LOG_CALL(_("setProperty_i_ccl ") << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));

	if(asAtomHandler::isNull(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}

	ASObject* o = asAtomHandler::getObject(*obj);
	if (!o)
		o = asAtomHandler::toObject(*obj,context->mi->context->root->getSystemState());
	if (context->exec_pos->local3.pos == 0x68)//initproperty
		o->setVariableByInteger(index,*value,ASObject::CONST_ALLOWED);
	else//Do not allow to set contant traits
		o->setVariableByInteger(index,*value,ASObject::CONST_NOT_ALLOWED);
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyInteger_constant_local_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));
	asAtom* obj = instrptr->arg3_constant;
	int index = asAtomHandler::getInt(CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	asAtom* value = &CONTEXT_GETLOCAL(context,instrptr->local_pos2);

	LOG_CALL(_("setProperty_i_cll ") << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));

	if(asAtomHandler::isNull(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}

	ASATOM_INCREF_POINTER(value);
	ASObject* o = asAtomHandler::getObject(*obj);
	if (!o)
		o = asAtomHandler::toObject(*obj,context->mi->context->root->getSystemState());
	if (context->exec_pos->local3.pos == 0x68)//initproperty
		o->setVariableByInteger(index,*value,ASObject::CONST_ALLOWED);
	else//Do not allow to set contant traits
		o->setVariableByInteger(index,*value,ASObject::CONST_NOT_ALLOWED);
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyInteger_local_constant_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local3.pos);
	int index = asAtomHandler::getInt(*instrptr->arg1_constant);
	asAtom* value = instrptr->arg2_constant;

	LOG_CALL(_("setProperty_i_lcc ") << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));

	if(asAtomHandler::isNull(obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}

	ASObject* o = asAtomHandler::getObject(obj);
	if (!o)
		o = asAtomHandler::toObject(obj,context->mi->context->root->getSystemState());
	if (context->exec_pos->local3.pos == 0x68)//initproperty
		o->setVariableByInteger(index,*value,ASObject::CONST_ALLOWED);
	else//Do not allow to set contant traits
		o->setVariableByInteger(index,*value,ASObject::CONST_NOT_ALLOWED);
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyInteger_local_local_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local3.pos);
	int index = asAtomHandler::getInt(CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	asAtom* value = instrptr->arg2_constant;

	LOG_CALL(_("setProperty_i_llc ") << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));

	if(asAtomHandler::isNull(obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}

	ASObject* o = asAtomHandler::getObject(obj);
	if (!o)
		o = asAtomHandler::toObject(obj,context->mi->context->root->getSystemState());
	if (context->exec_pos->local3.pos == 0x68)//initproperty
		o->setVariableByInteger(index,*value,ASObject::CONST_ALLOWED);
	else//Do not allow to set contant traits
		o->setVariableByInteger(index,*value,ASObject::CONST_NOT_ALLOWED);
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyInteger_local_constant_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local3.pos);
	int index = asAtomHandler::getInt(*instrptr->arg1_constant);
	asAtom* value = &CONTEXT_GETLOCAL(context,instrptr->local_pos2);

	LOG_CALL(_("setProperty_i_lcl ") << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));

	if(asAtomHandler::isNull(obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}

	ASATOM_INCREF_POINTER(value);
	ASObject* o = asAtomHandler::getObject(obj);
	if (!o)
		o = asAtomHandler::toObject(obj,context->mi->context->root->getSystemState());
	if (context->exec_pos->local3.pos == 0x68)//initproperty
		o->setVariableByInteger(index,*value,ASObject::CONST_ALLOWED);
	else//Do not allow to set contant traits
		o->setVariableByInteger(index,*value,ASObject::CONST_NOT_ALLOWED);
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyInteger_local_local_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local3.pos);
	int index = asAtomHandler::getInt(CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	asAtom* value = &CONTEXT_GETLOCAL(context,instrptr->local_pos2);

	LOG_CALL(_("setProperty_i_lll ") << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value)<<" "<<instrptr->local_pos1);

	if(asAtomHandler::isNull(obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}

	ASATOM_INCREF_POINTER(value);
	ASObject* o = asAtomHandler::getObject(obj);
	if (!o)
		o = asAtomHandler::toObject(obj,context->mi->context->root->getSystemState());
	if (context->exec_pos->local3.pos == 0x68)//initproperty
		o->setVariableByInteger(index,*value,ASObject::CONST_ALLOWED);
	else//Do not allow to set contant traits
		o->setVariableByInteger(index,*value,ASObject::CONST_NOT_ALLOWED);
	++(context->exec_pos);
}


void ABCVm::abc_setPropertyIntegerVector_constant_constant_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));
	asAtom* obj = instrptr->arg3_constant;
	int index = asAtomHandler::getInt(*instrptr->arg1_constant);
	asAtom* value = instrptr->arg2_constant;

	LOG_CALL(_("setProperty_iv_ccc ") << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));

	if(asAtomHandler::isNull(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}

	Vector* o = asAtomHandler::as<Vector>(*obj);
	o->setVariableByIntegerNoCoerce(index,*value);
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyIntegerVector_constant_local_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));
	asAtom* obj = instrptr->arg3_constant;
	int index = asAtomHandler::getInt(CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	asAtom* value = instrptr->arg2_constant;

	LOG_CALL(_("setProperty_iv_clc ") << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));

	if(asAtomHandler::isNull(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}

	Vector* o = asAtomHandler::as<Vector>(*obj);
	o->setVariableByIntegerNoCoerce(index,*value);
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyIntegerVector_constant_constant_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));
	asAtom* obj = instrptr->arg3_constant;
	int index = asAtomHandler::getInt(*instrptr->arg1_constant);
	asAtom* value = &CONTEXT_GETLOCAL(context,instrptr->local_pos2);

	LOG_CALL(_("setProperty_iv_ccl ") << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));

	if(asAtomHandler::isNull(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}

	Vector* o = asAtomHandler::as<Vector>(*obj);
	o->setVariableByIntegerNoCoerce(index,*value);
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyIntegerVector_constant_local_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));
	asAtom* obj = instrptr->arg3_constant;
	int index = asAtomHandler::getInt(CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	asAtom* value = &CONTEXT_GETLOCAL(context,instrptr->local_pos2);

	LOG_CALL(_("setProperty_iv_cll ") << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));

	if(asAtomHandler::isNull(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(*obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << index << ' ' << asAtomHandler::toDebugString(*obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}

	ASATOM_INCREF_POINTER(value);
	Vector* o = asAtomHandler::as<Vector>(*obj);
	o->setVariableByIntegerNoCoerce(index,*value);
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyIntegerVector_local_constant_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local3.pos);
	int index = asAtomHandler::getInt(*instrptr->arg1_constant);
	asAtom* value = instrptr->arg2_constant;

	LOG_CALL(_("setProperty_iv_lcc ") << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));

	if(asAtomHandler::isNull(obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}

	Vector* o = asAtomHandler::as<Vector>(obj);
	o->setVariableByIntegerNoCoerce(index,*value);
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyIntegerVector_local_local_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local3.pos);
	int index = asAtomHandler::getInt(CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	asAtom* value = instrptr->arg2_constant;

	LOG_CALL(_("setProperty_iv_llc ") << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));

	if(asAtomHandler::isNull(obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}

	Vector* o = asAtomHandler::as<Vector>(obj);
	o->setVariableByIntegerNoCoerce(index,*value);
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyIntegerVector_local_constant_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local3.pos);
	int index = asAtomHandler::getInt(*instrptr->arg1_constant);
	asAtom* value = &CONTEXT_GETLOCAL(context,instrptr->local_pos2);

	LOG_CALL(_("setProperty_iv_lcl ") << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));

	if(asAtomHandler::isNull(obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}

	ASATOM_INCREF_POINTER(value);
	Vector* o = asAtomHandler::as<Vector>(obj);
	o->setVariableByIntegerNoCoerce(index,*value);
	++(context->exec_pos);
}
void ABCVm::abc_setPropertyIntegerVector_local_local_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	(++(context->exec_pos));
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local3.pos);
	int index = asAtomHandler::getInt(CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	asAtom* value = &CONTEXT_GETLOCAL(context,instrptr->local_pos2);

	LOG_CALL(_("setProperty_iv_lll ") << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value)<<" "<<instrptr->local_pos1);

	if(asAtomHandler::isNull(obj))
	{
		LOG(LOG_ERROR,"calling setProperty on null:" << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertNullToObjectError);
	}
	if (asAtomHandler::isUndefined(obj))
	{
		LOG(LOG_ERROR,"calling setProperty on undefined:" << index << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
		throwError<TypeError>(kConvertUndefinedToObjectError);
	}

	ASATOM_INCREF_POINTER(value);
	Vector* o = asAtomHandler::as<Vector>(obj);
	o->setVariableByIntegerNoCoerce(index,*value);
	++(context->exec_pos);
}

void ABCVm::abc_setlocal_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t i = ((context->exec_pos)++)->arg3_uint;

	asAtom* obj = instrptr->arg1_constant;
	LOG_CALL( _("setLocal_c n ") << i << _(": ") << asAtomHandler::toDebugString(*obj) );
	assert(i <= uint32_t(context->mi->body->getReturnValuePos()+1+context->mi->body->localresultcount));
	if ((int)i != context->argarrayposition || asAtomHandler::isArray(*obj))
	{
		ASATOM_DECREF(context->locals[i]);
		context->locals[i]=*instrptr->arg1_constant;
	}
}
void ABCVm::abc_setlocal_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t i = ((context->exec_pos)++)->arg3_uint;

	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	LOG_CALL( _("setLocal_l n ") << i << _(": ") << asAtomHandler::toDebugString(obj) <<" "<<instrptr->local_pos1);
	assert(i <= uint32_t(context->mi->body->getReturnValuePos()+1+context->mi->body->localresultcount));
	if ((int)i != context->argarrayposition || asAtomHandler::isArray(obj))
	{
		ASATOM_INCREF(obj);
		ASATOM_DECREF(context->locals[i]);
		context->locals[i]=obj;
	}
}
void ABCVm::abc_getscopeobject_localresult(call_context* context)
{
	uint32_t t = context->exec_pos->arg2_uint;
	assert_and_throw(context->curr_scope_stack > (uint32_t)t);
	asAtom ret=context->scope_stack[t];
	if (ret.uintval != CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos).uintval)
	{
		ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
		asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
		ASATOM_INCREF(ret);
		if (o)
			o->decRef();
	}
	LOG_CALL("getScopeObject_l " << asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos))<<" "<<t);
	++(context->exec_pos);
}
void ABCVm::abc_getProperty_constant_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t t = (++(context->exec_pos))->arg3_uint;
	multiname* name=context->mi->context->getMultinameImpl(*instrptr->arg2_constant,nullptr,t,false);
	ASObject* obj= asAtomHandler::toObject(*instrptr->arg1_constant,context->mi->context->root->getSystemState());
	LOG_CALL( _("getProperty_cc ") << *name << ' ' << obj->toDebugString() << ' '<<obj->isInitialized());
	asAtom prop=asAtomHandler::invalidAtom;
	obj->getVariableByMultiname(prop,*name);
	if(asAtomHandler::isInvalid(prop))
		checkPropertyException(obj,name,prop);
	name->resetNameIfObject();
	RUNTIME_STACK_PUSH(context,prop);
	++(context->exec_pos);
}
void ABCVm::abc_getProperty_local_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t t = (++(context->exec_pos))->arg3_uint;
	multiname* name=context->mi->context->getMultinameImpl(*instrptr->arg2_constant,nullptr,t,false);
	ASObject* obj= asAtomHandler::toObject(CONTEXT_GETLOCAL(context,instrptr->local_pos1),context->mi->context->root->getSystemState());
	LOG_CALL( _("getProperty_lc ") << *name << ' ' << obj->toDebugString() << ' '<<obj->isInitialized());
	asAtom prop=asAtomHandler::invalidAtom;
	obj->getVariableByMultiname(prop,*name);
	if(asAtomHandler::isInvalid(prop))
		checkPropertyException(obj,name,prop);
	name->resetNameIfObject();
	RUNTIME_STACK_PUSH(context,prop);
	++(context->exec_pos);
}
void ABCVm::abc_getProperty_constant_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t t = (++(context->exec_pos))->arg3_uint;
	multiname* name=context->mi->context->getMultinameImpl(CONTEXT_GETLOCAL(context,instrptr->local_pos2),nullptr,t,false);
	ASObject* obj= asAtomHandler::toObject(*instrptr->arg1_constant,context->mi->context->root->getSystemState());
	LOG_CALL( _("getProperty_cl ") << *name << ' ' << obj->toDebugString() << ' '<<obj->isInitialized());
	asAtom prop=asAtomHandler::invalidAtom;
	obj->getVariableByMultiname(prop,*name);
	if(asAtomHandler::isInvalid(prop))
		checkPropertyException(obj,name,prop);
	name->resetNameIfObject();
	RUNTIME_STACK_PUSH(context,prop);
	++(context->exec_pos);
}
void ABCVm::abc_getProperty_local_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t t = (++(context->exec_pos))->arg3_uint;
	multiname* name=context->mi->context->getMultinameImpl(CONTEXT_GETLOCAL(context,instrptr->local_pos2),nullptr,t,false);
	ASObject* obj= asAtomHandler::toObject(CONTEXT_GETLOCAL(context,instrptr->local_pos1),context->mi->context->root->getSystemState());
	LOG_CALL( _("getProperty_ll ") << *name <<"("<<instrptr->local_pos2<<")"<< ' ' << obj->toDebugString() <<"("<<instrptr->local_pos1<<")"<< ' '<<obj->isInitialized());
	asAtom prop=asAtomHandler::invalidAtom;
	obj->getVariableByMultiname(prop,*name);
	if(asAtomHandler::isInvalid(prop))
		checkPropertyException(obj,name,prop);
	name->resetNameIfObject();
	RUNTIME_STACK_PUSH(context,prop);
	++(context->exec_pos);
}
void ABCVm::abc_getProperty_constant_constant_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t t = (++(context->exec_pos))->arg3_uint;
	multiname* name=context->mi->context->getMultinameImpl(*instrptr->arg2_constant,nullptr,t,false);
	ASObject* obj= asAtomHandler::toObject(*instrptr->arg1_constant,context->mi->context->root->getSystemState());
	LOG_CALL( _("getProperty_ccl ") << *name << ' ' << obj->toDebugString() << ' '<<obj->isInitialized());
	asAtom prop=asAtomHandler::invalidAtom;
	obj->getVariableByMultiname(prop,*name);
	if(asAtomHandler::isInvalid(prop))
		checkPropertyException(obj,name,prop);
	name->resetNameIfObject();
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),prop);
	ASATOM_INCREF(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_getProperty_local_constant_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t t = (++(context->exec_pos))->arg3_uint;
	asAtom prop=asAtomHandler::invalidAtom;
	if (asAtomHandler::isInteger(*instrptr->arg2_constant)
			&& asAtomHandler::is<Array>(CONTEXT_GETLOCAL(context,instrptr->local_pos1))
			&& asAtomHandler::getInt(*instrptr->arg2_constant) > 0
			&& (uint32_t)asAtomHandler::getInt(*instrptr->arg2_constant) < asAtomHandler::as<Array>(CONTEXT_GETLOCAL(context,instrptr->local_pos1))->currentsize)
	{
		LOG_CALL( _("getProperty_lcl ") << asAtomHandler::getInt(*instrptr->arg2_constant) << ' ' << asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,instrptr->local_pos1)));
		asAtomHandler::as<Array>(CONTEXT_GETLOCAL(context,instrptr->local_pos1))->at_nocheck(prop,asAtomHandler::getInt(*instrptr->arg2_constant));
	}
	else
	{
		multiname* name=context->mi->context->getMultinameImpl(*instrptr->arg2_constant,nullptr,t,false);
		ASObject* obj= asAtomHandler::toObject(CONTEXT_GETLOCAL(context,instrptr->local_pos1),context->mi->context->root->getSystemState());
		LOG_CALL( _("getProperty_lcl ") << *name << ' ' << obj->toDebugString() << ' '<<obj->isInitialized());
		obj->getVariableByMultiname(prop,*name,GET_VARIABLE_OPTION::NO_INCREF);
		if(asAtomHandler::isInvalid(prop))
			checkPropertyException(obj,name,prop);
		name->resetNameIfObject();
	}
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),prop);
	ASATOM_INCREF(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_getProperty_constant_local_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t t = (++(context->exec_pos))->arg3_uint;
	multiname* name=context->mi->context->getMultinameImpl(CONTEXT_GETLOCAL(context,instrptr->local_pos2),nullptr,t,false);
	ASObject* obj= asAtomHandler::toObject(*instrptr->arg1_constant,context->mi->context->root->getSystemState(),true);
	LOG_CALL( _("getProperty_cll ") << *name << ' ' << obj->toDebugString() << ' '<<obj->isInitialized());
	asAtom prop=asAtomHandler::invalidAtom;
	obj->getVariableByMultiname(prop,*name,GET_VARIABLE_OPTION::NO_INCREF);
	if(asAtomHandler::isInvalid(prop))
		checkPropertyException(obj,name,prop);
	name->resetNameIfObject();
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),prop);
	ASATOM_INCREF(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_getProperty_local_local_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t t = (++(context->exec_pos))->arg3_uint;
	asAtom prop=asAtomHandler::invalidAtom;
	if (asAtomHandler::isInteger(CONTEXT_GETLOCAL(context,instrptr->local_pos2))
			&& asAtomHandler::is<Array>(CONTEXT_GETLOCAL(context,instrptr->local_pos1))
			&& asAtomHandler::getInt(CONTEXT_GETLOCAL(context,instrptr->local_pos2)) > 0
			&& (uint32_t)asAtomHandler::getInt(CONTEXT_GETLOCAL(context,instrptr->local_pos2)) < asAtomHandler::as<Array>(CONTEXT_GETLOCAL(context,instrptr->local_pos1))->currentsize)
	{
		LOG_CALL( _("getProperty_lll int ") << asAtomHandler::getInt(CONTEXT_GETLOCAL(context,instrptr->local_pos2)) << ' ' << asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,instrptr->local_pos1)));
		asAtomHandler::as<Array>(CONTEXT_GETLOCAL(context,instrptr->local_pos1))->at_nocheck(prop,asAtomHandler::getInt(CONTEXT_GETLOCAL(context,instrptr->local_pos2)));
	}
	else
	{
		multiname* name=context->mi->context->getMultinameImpl(CONTEXT_GETLOCAL(context,instrptr->local_pos2),nullptr,t,false);
		ASObject* obj= asAtomHandler::toObject(CONTEXT_GETLOCAL(context,instrptr->local_pos1),context->mi->context->root->getSystemState());
		LOG_CALL( _("getProperty_lll ") << *name << ' ' << obj->toDebugString() << ' '<<obj->isInitialized());
		obj->getVariableByMultiname(prop,*name,GET_VARIABLE_OPTION::NO_INCREF);
		if(asAtomHandler::isInvalid(prop))
			checkPropertyException(obj,name,prop);
		name->resetNameIfObject();
	}
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),prop);
	ASATOM_INCREF(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_getPropertyInteger(call_context* context)
{
	RUNTIME_STACK_POP_CREATE(context,arg1);
	RUNTIME_STACK_POP_CREATE_ASOBJECT(context,obj, context->mi->context->root->getSystemState());
	int index=asAtomHandler::toInt(*arg1);
	LOG_CALL( _("getPropertyInteger ") << index << ' ' << obj->toDebugString() << ' '<<obj->isInitialized());
	asAtom prop=asAtomHandler::invalidAtom;
	if (obj->is<Vector>())
	{
		obj->as<Vector>()->getVariableByIntegerDirect(prop,index);
		ASATOM_INCREF(prop);
	}
	else
		obj->getVariableByInteger(prop,index);
	if(asAtomHandler::isInvalid(prop))
		checkPropertyExceptionInteger(obj,index,prop);
	RUNTIME_STACK_PUSH(context,prop);
	++(context->exec_pos);
}
void ABCVm::abc_getPropertyInteger_constant_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	int index=asAtomHandler::toInt(*instrptr->arg2_constant);
	ASObject* obj= asAtomHandler::getObject(*instrptr->arg1_constant);
	if (!obj)
		obj= asAtomHandler::toObject(*instrptr->arg1_constant,context->mi->context->root->getSystemState());
	LOG_CALL( _("getPropertyInteger_cc ") << index << ' ' << obj->toDebugString() << ' '<<obj->isInitialized());
	asAtom prop=asAtomHandler::invalidAtom;
	if (obj->is<Vector>())
	{
		obj->as<Vector>()->getVariableByIntegerDirect(prop,index);
		ASATOM_INCREF(prop);
	}
	else
		obj->getVariableByInteger(prop,index);
	if(asAtomHandler::isInvalid(prop))
		checkPropertyExceptionInteger(obj,index,prop);
	RUNTIME_STACK_PUSH(context,prop);
	++(context->exec_pos);
}
void ABCVm::abc_getPropertyInteger_local_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	int index=asAtomHandler::toInt(*instrptr->arg2_constant);
	ASObject* obj= asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	if (!obj)
		obj= asAtomHandler::toObject(CONTEXT_GETLOCAL(context,instrptr->local_pos1),context->mi->context->root->getSystemState());
	LOG_CALL( _("getPropertyInteger_lc ") << index << ' ' << obj->toDebugString() << ' '<<obj->isInitialized());
	asAtom prop=asAtomHandler::invalidAtom;
	if (obj->is<Vector>())
	{
		obj->as<Vector>()->getVariableByIntegerDirect(prop,index);
		ASATOM_INCREF(prop);
	}
	else
		obj->getVariableByInteger(prop,index);
	if(asAtomHandler::isInvalid(prop))
		checkPropertyExceptionInteger(obj,index,prop);
	RUNTIME_STACK_PUSH(context,prop);
	++(context->exec_pos);
}
void ABCVm::abc_getPropertyInteger_constant_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	int index=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,instrptr->local_pos2));
	ASObject* obj= asAtomHandler::getObject(*instrptr->arg1_constant);
	if (!obj)
		obj= asAtomHandler::toObject(*instrptr->arg1_constant,context->mi->context->root->getSystemState());
	LOG_CALL( _("getPropertyInteger_cl ") << index << ' ' << obj->toDebugString() << ' '<<obj->isInitialized());
	asAtom prop=asAtomHandler::invalidAtom;
	if (obj->is<Vector>())
	{
		obj->as<Vector>()->getVariableByIntegerDirect(prop,index);
		ASATOM_INCREF(prop);
	}
	else
		obj->getVariableByInteger(prop,index);
	if(asAtomHandler::isInvalid(prop))
		checkPropertyExceptionInteger(obj,index,prop);
	RUNTIME_STACK_PUSH(context,prop);
	++(context->exec_pos);
}
void ABCVm::abc_getPropertyInteger_local_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	int index=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,instrptr->local_pos2));
	ASObject* obj = nullptr;
	if (asAtomHandler::isObject(CONTEXT_GETLOCAL(context,instrptr->local_pos1)))
		obj = asAtomHandler::getObjectNoCheck(CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	else
		obj= asAtomHandler::toObject(CONTEXT_GETLOCAL(context,instrptr->local_pos1),context->mi->context->root->getSystemState());
	LOG_CALL( _("getPropertyInteger_ll ") << index <<"("<<instrptr->local_pos2<<")"<< ' ' << obj->toDebugString() <<"("<<instrptr->local_pos1<<")"<< ' '<<obj->isInitialized());
	asAtom prop=asAtomHandler::invalidAtom;
	if (obj->is<Vector>())
	{
		obj->as<Vector>()->getVariableByIntegerDirect(prop,index);
		ASATOM_INCREF(prop);
	}
	else
		obj->getVariableByInteger(prop,index);
	if(asAtomHandler::isInvalid(prop))
		checkPropertyExceptionInteger(obj,index,prop);
	RUNTIME_STACK_PUSH(context,prop);
	++(context->exec_pos);
}
void ABCVm::abc_getPropertyInteger_constant_constant_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	int index=asAtomHandler::toInt(*instrptr->arg2_constant);
	ASObject* obj= asAtomHandler::getObject(*instrptr->arg1_constant);
	if (!obj)
		obj= asAtomHandler::toObject(*instrptr->arg1_constant,context->mi->context->root->getSystemState());
	LOG_CALL( _("getPropertyInteger_ccl ") << index << ' ' << obj->toDebugString() << ' '<<obj->isInitialized());
	asAtom prop=asAtomHandler::invalidAtom;
	obj->getVariableByInteger(prop,index);
	if(asAtomHandler::isInvalid(prop))
		checkPropertyExceptionInteger(obj,index,prop);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),prop);
	ASATOM_INCREF(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_getPropertyInteger_local_constant_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	int index=asAtomHandler::toInt(*instrptr->arg2_constant);
	ASObject* obj= asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	if (!obj)
		obj= asAtomHandler::toObject(CONTEXT_GETLOCAL(context,instrptr->local_pos1),context->mi->context->root->getSystemState());
	LOG_CALL( _("getPropertyInteger_lcl ") << index << ' ' << obj->toDebugString() << ' '<<obj->isInitialized());
	asAtom prop=asAtomHandler::invalidAtom;
	if (obj->is<Vector>())
		obj->as<Vector>()->getVariableByIntegerDirect(prop,index);
	else
		obj->getVariableByInteger(prop,index,GET_VARIABLE_OPTION::NO_INCREF);
	if(asAtomHandler::isInvalid(prop))
		checkPropertyExceptionInteger(obj,index,prop);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),prop);
	ASATOM_INCREF(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_getPropertyInteger_constant_local_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	int index=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,instrptr->local_pos2));
	ASObject* obj= asAtomHandler::getObject(*instrptr->arg1_constant);
	if (!obj)
		obj= asAtomHandler::toObject(*instrptr->arg1_constant,context->mi->context->root->getSystemState(),true);
	LOG_CALL( _("getPropertyInteger_cll ") << index << ' ' << obj->toDebugString() << ' '<<obj->isInitialized());
	asAtom prop=asAtomHandler::invalidAtom;
	if (obj->is<Vector>())
		obj->as<Vector>()->getVariableByIntegerDirect(prop,index);
	else
		obj->getVariableByInteger(prop,index,GET_VARIABLE_OPTION::NO_INCREF);
	if(asAtomHandler::isInvalid(prop))
		checkPropertyExceptionInteger(obj,index,prop);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),prop);
	ASATOM_INCREF(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_getPropertyInteger_local_local_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	int index=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,instrptr->local_pos2));
	ASObject* obj= asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	if (!obj)
		obj= asAtomHandler::toObject(CONTEXT_GETLOCAL(context,instrptr->local_pos1),context->mi->context->root->getSystemState());
	LOG_CALL( _("getPropertyInteger_lll ") << index << ' ' << obj->toDebugString() << ' '<<obj->isInitialized());
	asAtom prop=asAtomHandler::invalidAtom;
	if (obj->is<Vector>())
		obj->as<Vector>()->getVariableByIntegerDirect(prop,index);
	else
		obj->getVariableByInteger(prop,index,GET_VARIABLE_OPTION::NO_INCREF);
	if(asAtomHandler::isInvalid(prop))
		checkPropertyExceptionInteger(obj,index,prop);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),prop);
	ASATOM_INCREF(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_getPropertyStaticName_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	multiname* name=instrptr->cachedmultiname2;

	ASObject* obj= asAtomHandler::toObject(*instrptr->arg1_constant,context->mi->context->root->getSystemState());
	LOG_CALL( _("getProperty_sc ") << *name << ' ' << obj->toDebugString() << ' '<<obj->isInitialized());
	asAtom prop=asAtomHandler::invalidAtom;
	if(asAtomHandler::isInvalid(prop))
	{
		bool isgetter = obj->getVariableByMultiname(prop,*name,GET_VARIABLE_OPTION::DONT_CALL_GETTER) & GET_VARIABLE_RESULT::GETVAR_ISGETTER;
		if (isgetter)
		{
			//Call the getter
			LOG_CALL("Calling the getter for " << *name << " on " << obj->toDebugString());
			assert(asAtomHandler::isFunction(prop));
			IFunction* f = asAtomHandler::as<IFunction>(prop);
			ASObject* closure = asAtomHandler::getClosure(prop);
			prop = asAtom();
			multiname* simplegetter = f->callGetter(prop,closure ? closure : obj);
			if (simplegetter)
			{
				LOG_CALL("is simple getter " << *simplegetter);
				instrptr->cachedmultiname2 = simplegetter;
			}
			LOG_CALL("End of getter"<< ' ' << f->toDebugString()<<" result:"<<asAtomHandler::toDebugString(prop));
		}
	}
	if(asAtomHandler::isInvalid(prop))
		checkPropertyException(obj,name,prop);
	RUNTIME_STACK_PUSH(context,prop);
	++(context->exec_pos);
}
void ABCVm::abc_getPropertyStaticName_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom prop=asAtomHandler::invalidAtom;
	multiname* name=instrptr->cachedmultiname2;

	if (name->name_type == multiname::NAME_INT
			&& asAtomHandler::is<Array>(CONTEXT_GETLOCAL(context,instrptr->local_pos1))
			&& name->name_i > 0
			&& (uint32_t)name->name_i < asAtomHandler::as<Array>(CONTEXT_GETLOCAL(context,instrptr->local_pos1))->currentsize)
	{
		LOG_CALL( _("getProperty_sl ") << name->name_i << ' ' << asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,instrptr->local_pos1)));
		asAtomHandler::as<Array>(CONTEXT_GETLOCAL(context,instrptr->local_pos1))->at_nocheck(prop,name->name_i);
	}
	else
	{
		ASObject* obj= asAtomHandler::toObject(CONTEXT_GETLOCAL(context,instrptr->local_pos1),context->mi->context->root->getSystemState());
		LOG_CALL( _("getProperty_sl ") << *name << ' ' << obj->toDebugString() << ' '<<obj->isInitialized());
		if(asAtomHandler::isInvalid(prop))
		{
			bool isgetter = obj->getVariableByMultiname(prop,*name,GET_VARIABLE_OPTION::DONT_CALL_GETTER) & GET_VARIABLE_RESULT::GETVAR_ISGETTER;
			if (isgetter)
			{
				//Call the getter
				LOG_CALL("Calling the getter for " << *name << " on " << obj->toDebugString());
				assert(asAtomHandler::isFunction(prop));
				IFunction* f = asAtomHandler::as<IFunction>(prop);
				ASObject* closure = asAtomHandler::getClosure(prop);
				prop = asAtom();
				multiname* simplegetter = f->callGetter(prop,closure ? closure : obj);
				if (simplegetter)
				{
					LOG_CALL("is simple getter " << *simplegetter);
					instrptr->cachedmultiname2 = simplegetter;
				}
				LOG_CALL("End of getter"<< ' ' << f->toDebugString()<<" result:"<<asAtomHandler::toDebugString(prop));
			}
		}
		if(asAtomHandler::isInvalid(prop))
			checkPropertyException(obj,name,prop);
	}
	RUNTIME_STACK_PUSH(context,prop);
	++(context->exec_pos);
}
void ABCVm::abc_getPropertyStaticName_constant_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	multiname* name=instrptr->cachedmultiname2;

	ASObject* obj= asAtomHandler::toObject(*instrptr->arg1_constant,context->mi->context->root->getSystemState(),true);
	LOG_CALL( _("getProperty_scl ") << *name << ' ' << obj->toDebugString() << ' '<<obj->isInitialized());
	asAtom prop=asAtomHandler::invalidAtom;
	if(asAtomHandler::isInvalid(prop))
	{
		bool isgetter = obj->getVariableByMultiname(prop,*name,(GET_VARIABLE_OPTION)(GET_VARIABLE_OPTION::NO_INCREF | GET_VARIABLE_OPTION::DONT_CALL_GETTER)) & GET_VARIABLE_RESULT::GETVAR_ISGETTER;
		if (isgetter)
		{
			//Call the getter
			LOG_CALL("Calling the getter for " << *name << " on " << obj->toDebugString());
			assert(asAtomHandler::isFunction(prop));
			IFunction* f = asAtomHandler::as<IFunction>(prop);
			ASObject* closure = asAtomHandler::getClosure(prop);
			prop = asAtom();
			multiname* simplegetter = f->callGetter(prop,closure ? closure : obj);
			if (simplegetter)
			{
				LOG_CALL("is simple getter " << *simplegetter);
				instrptr->cachedmultiname2 = simplegetter;
			}
			LOG_CALL("End of getter"<< ' ' << f->toDebugString()<<" result:"<<asAtomHandler::toDebugString(prop));
		}
		else
			ASATOM_INCREF(prop);
	}
	if(asAtomHandler::isInvalid(prop))
		checkPropertyException(obj,name,prop);
	name->resetNameIfObject();
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),prop);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_getPropertyStaticName_local_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	multiname* name=instrptr->cachedmultiname2;

	if (name->name_type == multiname::NAME_INT
			&& asAtomHandler::is<Array>(CONTEXT_GETLOCAL(context,instrptr->local_pos1))
			&& name->name_i > 0
			&& (uint32_t)name->name_i < asAtomHandler::as<Array>(CONTEXT_GETLOCAL(context,instrptr->local_pos1))->currentsize)
	{
		LOG_CALL( _("getProperty_slli ") << name->name_i << ' ' << asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,instrptr->local_pos1)));
		asAtomHandler::as<Array>(CONTEXT_GETLOCAL(context,instrptr->local_pos1))->at_nocheck(CONTEXT_GETLOCAL(context,instrptr->local3.pos),name->name_i);
	}
	else
	{
		ASObject* obj= asAtomHandler::toObject(CONTEXT_GETLOCAL(context,instrptr->local_pos1),context->mi->context->root->getSystemState());
		asAtom prop=asAtomHandler::invalidAtom;
// TODO caching doesn't work until we find a way to detect if obj is a reused object
// that has been destroyed since the last call
//		if (((context->exec_pos->data&ABC_OP_NOTCACHEABLE) == 0)
//				&& ((obj->is<Class_base>() && obj->as<Class_base>()->isSealed) ||
//					(obj->getClass()
//					 && (obj->getClass()->isSealed
//						 || (obj->getClass() == Class<Array>::getRef(obj->getSystemState()).getPtr())))))
//		{
//			variable* v = obj->findVariableByMultiname(*name,obj->getClass());
//			if (v)
//			{
//				context->exec_pos->data |= ABC_OP_CACHED;
//				context->exec_pos->cachedvar2=v;
//				context->exec_pos->cacheobj1=obj;
//				if (asAtomHandler::isValid(v->getter))
//				{
//					asAtomHandler::set(prop,v->getter);
//					//Call the getter
//					LOG_CALL("Calling the getter for " << *name << " on " << obj->toDebugString());
//					assert(asAtomHandler::isFunction(prop));
//					IFunction* f = asAtomHandler::as<IFunction>(prop);
//					ASObject* closure = asAtomHandler::getClosure(prop);
//					f->callGetter(prop,closure ? closure : obj);
//					LOG_CALL("End of getter"<< ' ' << f->toDebugString()<<" result:"<<asAtomHandler::toDebugString(prop));
//				}
//				else
//				{
//					asAtomHandler::set(prop,v->var);
//					ASATOM_INCREF(prop);
//				}
//			}
//		}
		if(asAtomHandler::isInvalid(prop))
		{
			bool isgetter = obj->getVariableByMultiname(prop,*name,(GET_VARIABLE_OPTION)(GET_VARIABLE_OPTION::NO_INCREF| GET_VARIABLE_OPTION::DONT_CALL_GETTER)) & GET_VARIABLE_RESULT::GETVAR_ISGETTER;
			if (isgetter)
			{
				//Call the getter
				LOG_CALL("Calling the getter for " << *name << " on " << obj->toDebugString());
				assert(asAtomHandler::isFunction(prop));
				IFunction* f = asAtomHandler::as<IFunction>(prop);
				ASObject* closure = asAtomHandler::getClosure(prop);
				prop = asAtom();
				multiname* simplegetter = f->callGetter(prop,closure ? closure : obj);
				if (simplegetter)
				{
					LOG_CALL("is simple getter " << *simplegetter);
					instrptr->cachedmultiname2 = simplegetter;
				}
				LOG_CALL("End of getter"<< ' ' << f->toDebugString()<<" result:"<<asAtomHandler::toDebugString(prop));
			}
			else
			{
				LOG_CALL("getProperty_sll " << *name << ' ' << obj->toDebugString()<<" "<<instrptr->local3.pos<<" "<<asAtomHandler::toDebugString(prop));
				ASATOM_INCREF(prop);
			}

		}
		if(asAtomHandler::isInvalid(prop))
			checkPropertyException(obj,name,prop);
		ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
		asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),prop);
		if (o)
			o->decRef();
	}
	++(context->exec_pos);
}
void ABCVm::abc_getPropertyStaticName_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	multiname* name=instrptr->cachedmultiname2;

	RUNTIME_STACK_POP_CREATE_ASOBJECT(context,obj,context->mi->context->root->getSystemState());
	LOG_CALL( _("getProperty_slr ") << *name << ' ' << obj->toDebugString() << ' '<<obj->isInitialized());
	asAtom prop=asAtomHandler::invalidAtom;
	if(asAtomHandler::isInvalid(prop))
	{
		bool isgetter = obj->getVariableByMultiname(prop,*name,(GET_VARIABLE_OPTION)(GET_VARIABLE_OPTION::NO_INCREF | GET_VARIABLE_OPTION::DONT_CALL_GETTER)) & GET_VARIABLE_RESULT::GETVAR_ISGETTER;
		if (isgetter)
		{
			//Call the getter
			LOG_CALL("Calling the getter for " << *name << " on " << obj->toDebugString());
			assert(asAtomHandler::isFunction(prop));
			IFunction* f = asAtomHandler::as<IFunction>(prop);
			ASObject* closure = asAtomHandler::getClosure(prop);
			prop = asAtom();
			multiname* simplegetter = f->callGetter(prop,closure ? closure : obj);
			if (simplegetter)
			{
				LOG_CALL("is simple getter " << *simplegetter);
				instrptr->cachedmultiname2 = simplegetter;
			}
			LOG_CALL("End of getter"<< ' ' << f->toDebugString()<<" result:"<<asAtomHandler::toDebugString(prop));
		}
		else
			ASATOM_INCREF(prop);
	}
	if(asAtomHandler::isInvalid(prop))
		checkPropertyException(obj,name,prop);
	name->resetNameIfObject();
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),prop);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionNoArgs_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = *instrptr->arg1_constant;
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj2);
	asAtom ret;
	LOG_CALL("callFunctionNoArgs_c " << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << " on " << asAtomHandler::toDebugString(obj));
	asAtomHandler::callFunction(func,ret,obj,nullptr,0,false,false);
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionNoArgs_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj2);
	asAtom ret;
	LOG_CALL("callFunctionNoArgs_l " << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << " on " << asAtomHandler::toDebugString(obj));
	asAtomHandler::callFunction(func,ret,obj,nullptr,0,false,false);
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionNoArgs_constant_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = *instrptr->arg1_constant;
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj2);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	LOG_CALL("callFunctionNoArgs_cl " << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << " on " << asAtomHandler::toDebugString(obj));
	asAtomHandler::callFunction(func,CONTEXT_GETLOCAL(context,instrptr->local3.pos),obj,nullptr,0,false,false);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionNoArgs_local_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj2);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	LOG_CALL("callFunctionNoArgs_ll " << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << " on " << asAtomHandler::toDebugString(obj));
	asAtomHandler::callFunction(func,CONTEXT_GETLOCAL(context,instrptr->local3.pos),obj,nullptr,0,false,false);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionNoArgsVoid_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = *instrptr->arg1_constant;
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj2);
	LOG_CALL("callFunctionNoArgsVoid_c " << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << " on " << asAtomHandler::toDebugString(obj));
	asAtom ret = asAtomHandler::invalidAtom;
	asAtomHandler::callFunction(func,ret,obj,nullptr,0,false,false,false);
	ASATOM_DECREF(ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionNoArgsVoid_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj2);
	LOG_CALL("callFunctionNoArgsVoid_l " << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << " on " << asAtomHandler::toDebugString(obj));
	asAtom ret = asAtomHandler::invalidAtom;
	asAtomHandler::callFunction(func,ret,obj,nullptr,0,false,false,false);
	ASATOM_DECREF(ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionSyntheticOneArgVoid_constant_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = *instrptr->arg1_constant;
	asAtom* value = instrptr->arg2_constant;
	asAtom func = asAtomHandler::fromObjectNoPrimitive((++context->exec_pos)->cacheobj3);
	LOG_CALL(_("callFunctionSyntheticOneArgVoid_cc ") << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
	asAtom ret = asAtomHandler::invalidAtom;
	asAtomHandler::getObjectNoCheck(func)->as<SyntheticFunction>()->call(ret,obj, value, 1,false,(instrptr->arg3_uint&ABC_OP_COERCED)==0);
	ASATOM_DECREF(ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionSyntheticOneArgVoid_local_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	asAtom* value = instrptr->arg2_constant;
	asAtom func = asAtomHandler::fromObjectNoPrimitive((++context->exec_pos)->cacheobj3);
	LOG_CALL(_("callFunctionSyntheticOneArgVoid_lc ") << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
	asAtom ret = asAtomHandler::invalidAtom;
	asAtomHandler::getObjectNoCheck(func)->as<SyntheticFunction>()->call(ret,obj, value, 1,false,(instrptr->arg3_uint&ABC_OP_COERCED)==0);
	ASATOM_DECREF(ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionSyntheticOneArgVoid_constant_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = *instrptr->arg1_constant;
	asAtom* value = &CONTEXT_GETLOCAL(context,instrptr->local_pos2);
	asAtom func = asAtomHandler::fromObjectNoPrimitive((++context->exec_pos)->cacheobj3);
	LOG_CALL(_("callFunctionOneSyntheticArgVoid_cl ") << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
	ASATOM_INCREF_POINTER(value);
	asAtom ret = asAtomHandler::invalidAtom;
	asAtomHandler::getObjectNoCheck(func)->as<SyntheticFunction>()->call(ret,obj, value, 1,false,(instrptr->arg3_uint&ABC_OP_COERCED)==0);
	ASATOM_DECREF(ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionSyntheticOneArgVoid_local_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	asAtom* value = &CONTEXT_GETLOCAL(context,instrptr->local_pos2);
	asAtom func = asAtomHandler::fromObjectNoPrimitive((++context->exec_pos)->cacheobj3);
	LOG_CALL(_("callFunctionOneSyntheticArgVoid_ll ") << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
	ASATOM_INCREF_POINTER(value);
	asAtom ret = asAtomHandler::invalidAtom;
	asAtomHandler::getObjectNoCheck(func)->as<SyntheticFunction>()->call(ret,obj, value, 1,false,(instrptr->arg3_uint&ABC_OP_COERCED)==0);
	ASATOM_DECREF(ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionBuiltinOneArgVoid_constant_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = *instrptr->arg1_constant;
	asAtom* value = instrptr->arg2_constant;
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj3);
	LOG_CALL(_("callFunctionOneBuiltinArgVoid_cc ") << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
	asAtom ret = asAtomHandler::invalidAtom;
	asAtomHandler::getObjectNoCheck(func)->as<Function>()->call(ret, obj, value, 1);
	ASATOM_DECREF(ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionBuiltinOneArgVoid_local_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	asAtom* value = instrptr->arg2_constant;
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj3);
	LOG_CALL(_("callFunctionOneBuiltinArgVoid_lc ") << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
	asAtom ret = asAtomHandler::invalidAtom;
	asAtomHandler::getObjectNoCheck(func)->as<Function>()->call(ret, obj, value, 1);
	ASATOM_DECREF(ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionBuiltinOneArgVoid_constant_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = *instrptr->arg1_constant;
	asAtom* value = &CONTEXT_GETLOCAL(context,instrptr->local_pos2);
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj3);
	LOG_CALL(_("callFunctionOneBuiltinArgVoid_cl ") << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
	asAtom ret = asAtomHandler::invalidAtom;
	asAtomHandler::getObjectNoCheck(func)->as<Function>()->call(ret, obj, value, 1);
	ASATOM_DECREF(ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionBuiltinOneArgVoid_local_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	asAtom* value = &CONTEXT_GETLOCAL(context,instrptr->local_pos2);
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj3);
	LOG_CALL(_("callFunctionBuiltinOneArgVoid_ll ") << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << ' ' << asAtomHandler::toDebugString(obj)<<" " <<asAtomHandler::toDebugString(*value));
	asAtom ret = asAtomHandler::invalidAtom;
	asAtomHandler::getObjectNoCheck(func)->as<Function>()->call(ret, obj, value, 1);
	ASATOM_DECREF(ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionSyntheticMultiArgsVoid(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t argcount = instrptr->local2.pos;
	SyntheticFunction* func = instrptr->cacheobj3->as<SyntheticFunction>();
	LOG_CALL("callFunctionSyntheticMultiArgsVoid " << func->getSystemState()->getStringFromUniqueId(func->functionname) << ' ' <<argcount);
	RUNTIME_STACK_POP_N_CREATE(context,argcount+1,args);
	asAtom ret;
	func->call(ret,*args, args+1, argcount,false,(instrptr->local2.flags&ABC_OP_COERCED)==0);
	ASATOM_DECREF(ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionSyntheticMultiArgsVoid_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = *instrptr->arg1_constant;
	uint32_t argcount = instrptr->local2.pos;
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj3);
	LOG_CALL(_("callFunctionSyntheticMultiArgsVoid_c ") << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << ' ' << asAtomHandler::toDebugString(obj)<<" "<<argcount);
	asAtom* args = g_newa(asAtom,argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
		{
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
			ASATOM_INCREF(args[i-1]);
		}
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	asAtom ret = asAtomHandler::invalidAtom;
	asAtomHandler::getObjectNoCheck(func)->as<SyntheticFunction>()->call(ret,obj, args, argcount,false,(instrptr->local2.flags&ABC_OP_COERCED)==0);
	ASATOM_DECREF(ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionSyntheticMultiArgsVoid_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	uint32_t argcount = instrptr->local2.pos;
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj3);
	LOG_CALL(_("callFunctionSyntheticMultiArgsVoid_l ") << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << ' ' << asAtomHandler::toDebugString(obj)<<" "<<argcount);
	asAtom* args = g_newa(asAtom,argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
		{
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
			ASATOM_INCREF(args[i-1]);
		}
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	asAtom ret = asAtomHandler::invalidAtom;
	asAtomHandler::getObjectNoCheck(func)->as<SyntheticFunction>()->call(ret,obj, args, argcount,false,(instrptr->local2.flags&ABC_OP_COERCED)==0);
	ASATOM_DECREF(ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionSyntheticMultiArgs(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	uint32_t argcount = instrptr->local2.pos;
	SyntheticFunction* func = instrptr->cacheobj3->as<SyntheticFunction>();
	LOG_CALL("callFunctionSyntheticMultiArgs " << func->getSystemState()->getStringFromUniqueId(func->functionname) << ' ' <<argcount);
	RUNTIME_STACK_POP_N_CREATE(context,argcount+1,args);
	asAtom ret;
	func->call(ret,*args, args+1, argcount,false,(instrptr->local2.flags&ABC_OP_COERCED)==0);
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionSyntheticMultiArgs_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = *instrptr->arg1_constant;
	uint32_t argcount = instrptr->local2.pos;
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj3);
	LOG_CALL(_("callFunctionSyntheticMultiArgs_c ") << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << ' ' << asAtomHandler::toDebugString(obj)<<" "<<argcount);
	asAtom* args = g_newa(asAtom,argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
		{
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
			ASATOM_INCREF(args[i-1]);
		}
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	asAtom ret;
	asAtomHandler::getObjectNoCheck(func)->as<SyntheticFunction>()->call(ret,obj, args, argcount,false,(instrptr->local2.flags&ABC_OP_COERCED)==0);
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionSyntheticMultiArgs_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	uint32_t argcount = instrptr->local2.pos;
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj3);
	LOG_CALL(_("callFunctionSyntheticMultiArgs_l ") << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << ' ' << asAtomHandler::toDebugString(obj)<<" "<<argcount);
	asAtom* args = g_newa(asAtom,argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
		{
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
			ASATOM_INCREF(args[i-1]);
		}
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	asAtom ret;
	asAtomHandler::getObjectNoCheck(func)->as<SyntheticFunction>()->call(ret,obj, args, argcount,false,(instrptr->local2.flags&ABC_OP_COERCED)==0);
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionSyntheticMultiArgs_constant_localResult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = *instrptr->arg1_constant;
	uint32_t argcount = instrptr->local2.pos;
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj3);
	LOG_CALL(_("callFunctionSyntheticMultiArgs_c_lr ") << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << ' ' << asAtomHandler::toDebugString(obj)<<" "<<argcount);
	asAtom* args = g_newa(asAtom,argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
		{
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
			ASATOM_INCREF(args[i-1]);
		}
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	asAtom ret;
	asAtomHandler::getObjectNoCheck(func)->as<SyntheticFunction>()->call(ret,obj, args, argcount,false,(instrptr->local2.flags&ABC_OP_COERCED)==0);
	// local result pos is stored in the context->exec_pos of the last argument
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionSyntheticMultiArgs_local_localResult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	uint32_t argcount = instrptr->local2.pos;
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj3);
	LOG_CALL(_("callFunctionSyntheticMultiArgs_l_lr ") << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << ' ' << asAtomHandler::toDebugString(obj)<<" "<<argcount);
	asAtom* args = g_newa(asAtom,argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
		{
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
			ASATOM_INCREF(args[i-1]);
		}
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	asAtom ret;
	asAtomHandler::getObjectNoCheck(func)->as<SyntheticFunction>()->call(ret,obj, args, argcount,false,(instrptr->local2.flags&ABC_OP_COERCED)==0);
	// local result pos is stored in the context->exec_pos of the last argument
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionBuiltinMultiArgsVoid_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = *instrptr->arg1_constant;
	uint32_t argcount = instrptr->local2.pos;
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj3);
	LOG_CALL(_("callFunctionBuiltinMultiArgsVoid_c ") << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << ' ' << asAtomHandler::toDebugString(obj)<<" "<<argcount);
	asAtom* args = g_newa(asAtom,argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	asAtom ret=asAtomHandler::invalidAtom;
	asAtomHandler::getObjectNoCheck(func)->as<Function>()->call(ret,obj, args, argcount);
	ASATOM_DECREF(ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionBuiltinMultiArgsVoid_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	uint32_t argcount = instrptr->local2.pos;
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj3);
	LOG_CALL(_("callFunctionBuiltinMultiArgsVoid_l ") << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << ' ' << asAtomHandler::toDebugString(obj)<<" "<<argcount);
	asAtom* args = g_newa(asAtom,argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	asAtom ret=asAtomHandler::invalidAtom;
	asAtomHandler::getObjectNoCheck(func)->as<Function>()->call(ret,obj, args, argcount);
	ASATOM_DECREF(ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionBuiltinMultiArgs_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = *instrptr->arg1_constant;
	uint32_t argcount = instrptr->local2.pos;
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj3);
	LOG_CALL(_("callFunctionBuiltinMultiArgs_c ") << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << ' ' << asAtomHandler::toDebugString(obj)<<" "<<argcount);
	asAtom* args = g_newa(asAtom,argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	asAtom ret;
	asAtomHandler::getObjectNoCheck(func)->as<Function>()->call(ret,obj, args, argcount);
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionBuiltinMultiArgs_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	uint32_t argcount = instrptr->local2.pos;
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj3);
	LOG_CALL(_("callFunctionBuiltinMultiArgs_l ") << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << ' ' << asAtomHandler::toDebugString(obj)<<" "<<argcount);
	asAtom* args = g_newa(asAtom,argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	asAtom ret;
	asAtomHandler::getObjectNoCheck(func)->as<Function>()->call(ret,obj, args, argcount);
	RUNTIME_STACK_PUSH(context,ret);
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionBuiltinMultiArgs_constant_localResult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = *instrptr->arg1_constant;
	uint32_t argcount = instrptr->local2.pos;
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj3);
	LOG_CALL(_("callFunctionBuiltinMultiArgs_c_lr ") << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << ' ' << asAtomHandler::toDebugString(obj)<<" "<<argcount);
	asAtom* args = g_newa(asAtom,argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	asAtom ret;
	asAtomHandler::getObjectNoCheck(func)->as<Function>()->call(ret,obj, args, argcount);
	// local result pos is stored in the context->exec_pos of the last argument
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_callFunctionBuiltinMultiArgs_local_localResult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos;
	asAtom obj = CONTEXT_GETLOCAL(context,instrptr->local_pos1);
	uint32_t argcount = instrptr->local2.pos;
	asAtom func = asAtomHandler::fromObjectNoPrimitive(instrptr->cacheobj3);
	LOG_CALL(_("callFunctionBuiltinMultiArgs_l_lr ") << asAtomHandler::as<IFunction>(func)->getSystemState()->getStringFromUniqueId(asAtomHandler::as<IFunction>(func)->functionname) << ' ' << asAtomHandler::toDebugString(obj)<<" "<<argcount);
	asAtom* args = g_newa(asAtom,argcount);
	for (uint32_t i = argcount; i > 0 ; i--)
	{
		(++(context->exec_pos));
		if (context->exec_pos->arg2_uint == OPERANDTYPES::OP_LOCAL || context->exec_pos->arg2_uint == OPERANDTYPES::OP_CACHED_SLOT)
			args[i-1] = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
		else
			args[i-1] = *context->exec_pos->arg1_constant;
	}
	asAtom ret;
	asAtomHandler::getObjectNoCheck(func)->as<Function>()->call(ret,obj, args, argcount);
	// local result pos is stored in the context->exec_pos of the last argument
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_getslot_constant(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos++;
	uint32_t t = instrptr->arg2_uint;
	asAtom* pval = instrptr->arg1_constant;
	asAtom ret=asAtomHandler::getObject(*pval)->getSlotNoCheck(t);
	LOG_CALL("getSlot_c " << t << " " << asAtomHandler::toDebugString(ret));
	ASATOM_INCREF(ret);
	RUNTIME_STACK_PUSH(context,ret);
}
void ABCVm::abc_getslot_local(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos++;
	uint32_t t = instrptr->arg2_uint;
	ASObject* obj = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	if (!obj)
		throwError<TypeError>(kConvertNullToObjectError);
	asAtom res = obj->getSlotNoCheck(t);
	LOG_CALL("getSlot_l " << t << " " << asAtomHandler::toDebugString(res));
	ASATOM_INCREF(res);
	RUNTIME_STACK_PUSH(context,res);
}
void ABCVm::abc_getslot_constant_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos++;
	uint32_t t = instrptr->arg2_uint;
	asAtom res = asAtomHandler::getObject(*instrptr->arg1_constant)->getSlotNoCheck(t);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),res);
	ASATOM_INCREF(CONTEXT_GETLOCAL(context,instrptr->local3.pos));
	if (o)
		o->decRef();
	LOG_CALL("getSlot_cl " << t << " " << asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,instrptr->local3.pos)));
}
void ABCVm::abc_getslot_local_localresult(call_context* context)
{
	preloadedcodedata* instrptr = context->exec_pos++;
	uint32_t t = instrptr->arg2_uint;
	if (!asAtomHandler::isObject(CONTEXT_GETLOCAL(context,instrptr->local_pos1)))
		throwError<TypeError>(kConvertNullToObjectError);
	ASObject* obj = asAtomHandler::getObjectNoCheck(CONTEXT_GETLOCAL(context,instrptr->local_pos1));
	asAtom res = obj->getSlotNoCheck(t);
	asAtom o = CONTEXT_GETLOCAL(context,instrptr->local3.pos);
	if (o.uintval != res.uintval)
	{
		asAtomHandler::set(CONTEXT_GETLOCAL(context,instrptr->local3.pos),res);
		ASATOM_INCREF(res);
		ASATOM_DECREF(o);
	}
	LOG_CALL("getSlot_ll " << t << " " <<instrptr->local_pos1<<":"<< asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,instrptr->local_pos1))<<" "<< asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,instrptr->local3.pos)));
}
void ABCVm::abc_setslot_constant_constant(call_context* context)
{
	asAtom v1 = *context->exec_pos->arg1_constant;
	asAtom v2 = *context->exec_pos->arg2_constant;
	uint32_t t = context->exec_pos->arg3_uint;
	LOG_CALL("setSlot_cc " << t << " "<< asAtomHandler::toDebugString(v2) << " "<< asAtomHandler::toDebugString(v1));
	if (asAtomHandler::getObject(v1)->setSlot(t,v2))
		ASATOM_INCREF(v2);
	++(context->exec_pos);
}
void ABCVm::abc_setslot_local_constant(call_context* context)
{
	asAtom v1 = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtom v2 = *context->exec_pos->arg2_constant;
	uint32_t t = context->exec_pos->arg3_uint;
	LOG_CALL("setSlot_lc " << t << " "<< asAtomHandler::toDebugString(v2) << " "<< asAtomHandler::toDebugString(v1));
	if (asAtomHandler::getObject(v1)->setSlot(t,v2))
		ASATOM_INCREF(v2);
	++(context->exec_pos);
}
void ABCVm::abc_setslot_constant_local(call_context* context)
{
	asAtom v1 = *context->exec_pos->arg1_constant;
	asAtom v2 = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2);
	uint32_t t = context->exec_pos->arg3_uint;
	LOG_CALL("setSlot_cl " << t << " "<< asAtomHandler::toDebugString(v2) << " "<< asAtomHandler::toDebugString(v1));
	if (asAtomHandler::getObject(v1)->setSlot(t,v2))
		ASATOM_INCREF(v2);
	++(context->exec_pos);
}
void ABCVm::abc_setslot_local_local(call_context* context)
{
	asAtom v1 = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtom v2 = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2);
	uint32_t t = context->exec_pos->arg3_uint;
	LOG_CALL("setSlot_ll " << t << " "<< asAtomHandler::toDebugString(v2) << " "<< asAtomHandler::toDebugString(v1));
	if (asAtomHandler::getObject(v1)->setSlot(t,v2))
		ASATOM_INCREF(v2);
	++(context->exec_pos);
}
void ABCVm::abc_setslotNoCoerce_constant_constant(call_context* context)
{
	asAtom v1 = *context->exec_pos->arg1_constant;
	asAtom v2 = *context->exec_pos->arg2_constant;
	uint32_t t = context->exec_pos->arg3_uint;
	LOG_CALL("setSlotNoCoerce_cc " << t << " "<< asAtomHandler::toDebugString(v2) << " "<< asAtomHandler::toDebugString(v1));
	if (asAtomHandler::getObjectNoCheck(v1)->setSlotNoCoerce(t,v2))
		ASATOM_INCREF(v2);
	++(context->exec_pos);
}
void ABCVm::abc_setslotNoCoerce_local_constant(call_context* context)
{
	asAtom v1 = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtom v2 = *context->exec_pos->arg2_constant;
	uint32_t t = context->exec_pos->arg3_uint;
	LOG_CALL("setSlotNoCoerce_lc " << t << " "<< asAtomHandler::toDebugString(v2) << " "<< asAtomHandler::toDebugString(v1));
	if (asAtomHandler::getObjectNoCheck(v1)->setSlotNoCoerce(t,v2))
		ASATOM_INCREF(v2);
	++(context->exec_pos);
}
void ABCVm::abc_setslotNoCoerce_constant_local(call_context* context)
{
	asAtom v1 = *context->exec_pos->arg1_constant;
	asAtom v2 = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2);
	uint32_t t = context->exec_pos->arg3_uint;
	LOG_CALL("setSlotNoCoerce_cl " << t << " "<< asAtomHandler::toDebugString(v2) << " "<< asAtomHandler::toDebugString(v1));
	if (asAtomHandler::getObjectNoCheck(v1)->setSlotNoCoerce(t,v2))
		ASATOM_INCREF(v2);
	++(context->exec_pos);
}
void ABCVm::abc_setslotNoCoerce_local_local(call_context* context)
{
	asAtom v1 = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtom v2 = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2);
	uint32_t t = context->exec_pos->arg3_uint;
	LOG_CALL("setSlotNoCoerce_ll " << t << " "<< asAtomHandler::toDebugString(v2) << " "<< asAtomHandler::toDebugString(v1));
	if (asAtomHandler::getObjectNoCheck(v1)->setSlotNoCoerce(t,v2))
		ASATOM_INCREF(v2);
	++(context->exec_pos);
}
void ABCVm::abc_convert_i_constant(call_context* context)
{
	LOG_CALL("convert_i_c");
	asAtom res = *context->exec_pos->arg1_constant;
	if(!asAtomHandler::isInteger(res))
	{
		int32_t v= asAtomHandler::toIntStrict(res);
		asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),v);
	}
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_convert_i_local(call_context* context)
{
	LOG_CALL("convert_i_l:"<<asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1)));
	asAtom res =CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	if(!asAtomHandler::isInteger(res))
	{
		int32_t v= asAtomHandler::toIntStrict(res);
		asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),v);
	}
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_convert_i_constant_localresult(call_context* context)
{
	LOG_CALL("convert_i_cl");
	asAtom res = *context->exec_pos->arg1_constant;
	if(!asAtomHandler::isInteger(res))
	{
		int32_t v= asAtomHandler::toIntStrict(res);
		asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),v);
	}
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_convert_i_local_localresult(call_context* context)
{
	LOG_CALL("convert_i_ll");
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	if(!asAtomHandler::isInteger(res))
	{
		int32_t v= asAtomHandler::toIntStrict(res);
		asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),v);
	}
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_convert_u_constant(call_context* context)
{
	LOG_CALL("convert_u_c");
	asAtom res = *context->exec_pos->arg1_constant;
	if(!asAtomHandler::isUInteger(res))
	{
		int32_t v= asAtomHandler::toUInt(res);
		asAtomHandler::setUInt(res,context->mi->context->root->getSystemState(),v);
	}
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_convert_u_local(call_context* context)
{
	LOG_CALL("convert_u_l:"<<asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1)));
	asAtom res =CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	if(!asAtomHandler::isUInteger(res))
	{
		int32_t v= asAtomHandler::toUInt(res);
		asAtomHandler::setUInt(res,context->mi->context->root->getSystemState(),v);
	}
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_convert_u_constant_localresult(call_context* context)
{
	LOG_CALL("convert_u_cl");
	asAtom res = *context->exec_pos->arg1_constant;
	if(!asAtomHandler::isUInteger(res))
	{
		int32_t v= asAtomHandler::toUInt(res);
		asAtomHandler::setUInt(res,context->mi->context->root->getSystemState(),v);
	}
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_convert_u_local_localresult(call_context* context)
{
	LOG_CALL("convert_u_ll");
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	if(!asAtomHandler::isUInteger(res))
	{
		int32_t v= asAtomHandler::toUInt(res);
		asAtomHandler::setUInt(res,context->mi->context->root->getSystemState(),v);
	}
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_convert_d_constant(call_context* context)
{
	LOG_CALL("convert_d_c");
	asAtom res = *context->exec_pos->arg1_constant;
	if(!asAtomHandler::isNumeric(res))
		asAtomHandler::setNumber(res,context->mi->context->root->getSystemState(),asAtomHandler::toNumber(res));
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_convert_d_local(call_context* context)
{
	LOG_CALL("convert_d_l:"<<asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1)));
	asAtom res =CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	if(!asAtomHandler::isNumeric(res))
		asAtomHandler::setNumber(res,context->mi->context->root->getSystemState(),asAtomHandler::toNumber(res));
	else
		ASATOM_INCREF(res);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_convert_d_constant_localresult(call_context* context)
{
	LOG_CALL("convert_d_cl");
	asAtom res = *context->exec_pos->arg1_constant;
	if(!asAtomHandler::isNumeric(res))
		asAtomHandler::setNumber(res,context->mi->context->root->getSystemState(),asAtomHandler::toNumber(res));
	else
		ASATOM_INCREF(res);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_convert_d_local_localresult(call_context* context)
{
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	LOG_CALL("convert_d_ll "<<asAtomHandler::toDebugString(res)<<" "<<asAtomHandler::getObject(res)<<" "<< context->exec_pos->local_pos1<<" "<<(context->exec_pos->local3.pos));
	if(!asAtomHandler::isNumeric(res))
		asAtomHandler::setNumber(res,context->mi->context->root->getSystemState(),asAtomHandler::toNumber(res));
	else
		ASATOM_INCREF(res);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_convert_b_constant(call_context* context)
{
	LOG_CALL("convert_b_c");
	asAtom res = *context->exec_pos->arg1_constant;
	if(!asAtomHandler::isBool(res))
		asAtomHandler::convert_b(res,false);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_convert_b_local(call_context* context)
{
	LOG_CALL("convert_b_l:"<<asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1)));
	asAtom res =CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	if(!asAtomHandler::isBool(res))
		asAtomHandler::convert_b(res,false);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_convert_b_constant_localresult(call_context* context)
{
	LOG_CALL("convert_b_cl");
	asAtom res = *context->exec_pos->arg1_constant;
	if(!asAtomHandler::isBool(res))
		asAtomHandler::convert_b(res,false);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_convert_b_local_localresult(call_context* context)
{
	LOG_CALL("convert_b_ll");
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	if(!asAtomHandler::isBool(res))
		asAtomHandler::convert_b(res,false);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_increment_local(call_context* context)
{
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	LOG_CALL("increment_l "<<context->exec_pos->local_pos1<<" "<<asAtomHandler::toDebugString(res));
	asAtomHandler::increment(res,context->mi->context->root->getSystemState());
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_increment_local_localresult(call_context* context)
{
	LOG_CALL("increment_ll "<<context->exec_pos->local_pos1<<" "<<context->exec_pos->local3.pos<<" "<<asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos)));
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	if (context->exec_pos->local3.pos != context->exec_pos->local_pos1)
		ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	asAtomHandler::increment(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState());
	++(context->exec_pos);
}
void ABCVm::abc_decrement_local(call_context* context)
{
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	LOG_CALL("decrement_l "<<context->exec_pos->local_pos1<<" "<<asAtomHandler::toDebugString(res));
	asAtomHandler::decrement(res,context->mi->context->root->getSystemState());
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_decrement_local_localresult(call_context* context)
{
	LOG_CALL("decrement_ll "<<context->exec_pos->local_pos1<<" "<<context->exec_pos->local3.pos<<" "<<asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos)));
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	if (context->exec_pos->local3.pos != context->exec_pos->local_pos1)
		ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	asAtomHandler::decrement(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState());
	++(context->exec_pos);
}
void ABCVm::abc_typeof_constant(call_context* context)
{
	LOG_CALL("typeof_c");
	asAtom res = asAtomHandler::typeOf(*context->exec_pos->arg1_constant);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_typeof_local(call_context* context)
{
	LOG_CALL("typeof_l "<<asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1)));
	asAtom res = asAtomHandler::typeOf(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_typeof_constant_localresult(call_context* context)
{
	LOG_CALL("typeof_cl");
	asAtom res = asAtomHandler::typeOf(*context->exec_pos->arg1_constant);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_typeof_local_localresult(call_context* context)
{
	LOG_CALL("typeof_ll");
	asAtom res = asAtomHandler::typeOf(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}void ABCVm::abc_not_constant(call_context* context)
{
	LOG_CALL("not_c");
	asAtom res = asAtomHandler::fromBool(!asAtomHandler::Boolean_concrete(*context->exec_pos->arg1_constant));
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_not_local(call_context* context)
{
	LOG_CALL("not_l "<<asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1)));
	asAtom res = asAtomHandler::fromBool(!asAtomHandler::Boolean_concrete(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1)));
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_not_constant_localresult(call_context* context)
{
	LOG_CALL("not_cl");
	bool res = !asAtomHandler::Boolean_concrete(*context->exec_pos->arg1_constant);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_not_local_localresult(call_context* context)
{
	LOG_CALL("not_ll");
	bool res = !asAtomHandler::Boolean_concrete(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_add_constant_constant(call_context* context)
{
	LOG_CALL("add_cc");
	asAtom res = *context->exec_pos->arg1_constant;
	asAtomHandler::add(res,*context->exec_pos->arg2_constant,context->mi->context->root->getSystemState(),context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_add_local_constant(call_context* context)
{
	LOG_CALL("add_lc");
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::add(res,*context->exec_pos->arg2_constant,context->mi->context->root->getSystemState(),context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_add_constant_local(call_context* context)
{
	LOG_CALL("add_cl");
	asAtom res = *context->exec_pos->arg1_constant;
	asAtomHandler::add(res,CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->mi->context->root->getSystemState(),context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_add_local_local(call_context* context)
{
	LOG_CALL("add_ll");
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::add(res,CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->mi->context->root->getSystemState(),context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_add_constant_constant_localresult(call_context* context)
{
	LOG_CALL("add_ccl");
	asAtomHandler::addreplace(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant,*context->exec_pos->arg2_constant,context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	++(context->exec_pos);
}
void ABCVm::abc_add_local_constant_localresult(call_context* context)
{
	LOG_CALL("add_lcl");
	asAtomHandler::addreplace(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),*context->exec_pos->arg2_constant,context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	++(context->exec_pos);
}
void ABCVm::abc_add_constant_local_localresult(call_context* context)
{
	LOG_CALL("add_cll");
	asAtomHandler::addreplace(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant,CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	++(context->exec_pos);
}
void ABCVm::abc_add_local_local_localresult(call_context* context)
{
	LOG_CALL("add_lll");
	asAtomHandler::addreplace(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	++(context->exec_pos);
}
void ABCVm::abc_subtract_constant_constant(call_context* context)
{
	LOG_CALL("subtract_cc");
	asAtom res = *context->exec_pos->arg1_constant;
	asAtomHandler::subtract(res,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant,context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_subtract_local_constant(call_context* context)
{
	LOG_CALL("subtract_lc");
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::subtract(res,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant,context->exec_pos->local3.flags & ABC_OP_FORCEINT);

	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_subtract_constant_local(call_context* context)
{
	LOG_CALL("subtract_cl");
	asAtom res = *context->exec_pos->arg1_constant;
	asAtomHandler::subtract(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_subtract_local_local(call_context* context)
{
	LOG_CALL("subtract_ll");
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::subtract(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_subtract_constant_constant_localresult(call_context* context)
{
	LOG_CALL("subtract_ccl");
	asAtomHandler::subtractreplace(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant,*context->exec_pos->arg2_constant,context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	++(context->exec_pos);
}
void ABCVm::abc_subtract_local_constant_localresult(call_context* context)
{
	LOG_CALL("subtract_lcl");
	asAtomHandler::subtractreplace(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),*context->exec_pos->arg2_constant,context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	++(context->exec_pos);
}
void ABCVm::abc_subtract_constant_local_localresult(call_context* context)
{
	LOG_CALL("subtract_cll");
	asAtomHandler::subtractreplace(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant,CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	++(context->exec_pos);
}
void ABCVm::abc_subtract_local_local_localresult(call_context* context)
{
	LOG_CALL("subtract_lll");
	asAtomHandler::subtractreplace(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	++(context->exec_pos);
}
void ABCVm::abc_multiply_constant_constant(call_context* context)
{
	LOG_CALL("multiply_cc");
	asAtom res = *context->exec_pos->arg1_constant;
	asAtomHandler::multiply(res,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant,context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_multiply_local_constant(call_context* context)
{
	LOG_CALL("multiply_lc");
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::multiply(res,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant,context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_multiply_constant_local(call_context* context)
{
	LOG_CALL("multiply_cl");
	asAtom res = *context->exec_pos->arg1_constant;
	asAtomHandler::multiply(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_multiply_local_local(call_context* context)
{
	LOG_CALL("multiply_ll");
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::multiply(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_multiply_constant_constant_localresult(call_context* context)
{
	LOG_CALL("multiply_ccl");
	asAtomHandler::multiplyreplace(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant,*context->exec_pos->arg2_constant,context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	++(context->exec_pos);
}
void ABCVm::abc_multiply_local_constant_localresult(call_context* context)
{
	LOG_CALL("multiply_lcl");
	asAtomHandler::multiplyreplace(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),*context->exec_pos->arg2_constant,context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	++(context->exec_pos);
}
void ABCVm::abc_multiply_constant_local_localresult(call_context* context)
{
	LOG_CALL("multiply_cll");
	asAtomHandler::multiplyreplace(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant,CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	++(context->exec_pos);
}
void ABCVm::abc_multiply_local_local_localresult(call_context* context)
{
	LOG_CALL("multiply_lll");
	asAtomHandler::multiplyreplace(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	++(context->exec_pos);
}
void ABCVm::abc_divide_constant_constant(call_context* context)
{
	LOG_CALL("divide_cc");
	asAtom res = *context->exec_pos->arg1_constant;
	asAtomHandler::divide(res,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant,context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_divide_local_constant(call_context* context)
{
	LOG_CALL("divide_lc");
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::divide(res,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant,context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_divide_constant_local(call_context* context)
{
	LOG_CALL("divide_cl");
	asAtom res = *context->exec_pos->arg1_constant;
	asAtomHandler::divide(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_divide_local_local(call_context* context)
{
	LOG_CALL("divide_ll");
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::divide(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_divide_constant_constant_localresult(call_context* context)
{
	LOG_CALL("divide_ccl");
	asAtomHandler::dividereplace(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant,*context->exec_pos->arg2_constant,context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	++(context->exec_pos);
}
void ABCVm::abc_divide_local_constant_localresult(call_context* context)
{
	LOG_CALL("divide_lcl");
	asAtomHandler::dividereplace(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),*context->exec_pos->arg2_constant,context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	++(context->exec_pos);
}
void ABCVm::abc_divide_constant_local_localresult(call_context* context)
{
	LOG_CALL("divide_cll");
	asAtomHandler::dividereplace(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant,CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	++(context->exec_pos);
}
void ABCVm::abc_divide_local_local_localresult(call_context* context)
{
	LOG_CALL("divide_lll");
	asAtomHandler::dividereplace(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->exec_pos->local3.flags & ABC_OP_FORCEINT);
	++(context->exec_pos);
}
void ABCVm::abc_modulo_constant_constant(call_context* context)
{
	//modulo
	asAtom res = *context->exec_pos->arg1_constant;
	asAtomHandler::modulo(res,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_modulo_local_constant(call_context* context)
{
	//modulo
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::modulo(res,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_modulo_constant_local(call_context* context)
{
	//modulo
	asAtom res = *context->exec_pos->arg1_constant;
	asAtomHandler::modulo(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_modulo_local_local(call_context* context)
{
	//modulo
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::modulo(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_modulo_constant_constant_localresult(call_context* context)
{
	LOG_CALL("modulo_ccl");
	asAtomHandler::moduloreplace(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant,*context->exec_pos->arg2_constant);
	++(context->exec_pos);
}
void ABCVm::abc_modulo_local_constant_localresult(call_context* context)
{
	LOG_CALL("modulo_lcl");
	asAtomHandler::moduloreplace(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),*context->exec_pos->arg2_constant);
	++(context->exec_pos);
}
void ABCVm::abc_modulo_constant_local_localresult(call_context* context)
{
	LOG_CALL("modulo_cll");
	asAtomHandler::moduloreplace(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant,CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	++(context->exec_pos);
}
void ABCVm::abc_modulo_local_local_localresult(call_context* context)
{
	LOG_CALL("modulo_lll");
	asAtomHandler::moduloreplace(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	++(context->exec_pos);
}
void ABCVm::abc_lshift_constant_constant(call_context* context)
{
	int32_t i2=context->exec_pos->arg1_int;
	uint32_t i1=(context->exec_pos->arg2_uint)&0x1f;
	LOG_CALL("lShift_cc "<<std::hex<<i2<<"<<"<<i1<<std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i2<<i1);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_lshift_local_constant(call_context* context)
{
	int32_t i2=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	uint32_t i1=(context->exec_pos->arg2_uint)&0x1f;
	LOG_CALL("lShift_lc "<<std::hex<<i2<<"<<"<<i1<<std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i2<<i1);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_lshift_constant_local(call_context* context)
{
	int32_t i2=context->exec_pos->arg1_int;
	uint32_t i1=(asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2)))&0x1f;
	LOG_CALL("lShift_cl "<<std::hex<<i2<<"<<"<<i1<<std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i2<<i1);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_lshift_local_local(call_context* context)
{
	int32_t i2=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	uint32_t i1=(asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2)))&0x1f;
	LOG_CALL("lShift_ll "<<std::hex<<i2<<"<<"<<i1<<std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i2<<i1);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_lshift_constant_constant_localresult(call_context* context)
{
	int32_t i2=context->exec_pos->arg1_int;
	uint32_t i1=(context->exec_pos->arg2_uint)&0x1f;
	LOG_CALL("lShift_ccl "<<std::hex<<i2<<"<<"<<i1<<std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i2<<i1);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_lshift_local_constant_localresult(call_context* context)
{
	int32_t i2=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	uint32_t i1=(context->exec_pos->arg2_uint)&0x1f;
	LOG_CALL("lShift_lcl "<<std::hex<<i2<<"<<"<<i1<<std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i2<<i1);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_lshift_constant_local_localresult(call_context* context)
{
	int32_t i2=context->exec_pos->arg1_int;
	uint32_t i1=(asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2)))&0x1f;
	LOG_CALL("lShift_cll "<<std::hex<<i2<<"<<"<<i1<<std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i2<<i1);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_lshift_local_local_localresult(call_context* context)
{
	int32_t i2=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	uint32_t i1=(asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2)))&0x1f;
	LOG_CALL("lShift_lll "<<std::hex<<i2<<"<<"<<i1<<std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i2<<i1);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_rshift_constant_constant(call_context* context)
{
	//rshift
	asAtom res = *context->exec_pos->arg1_constant;
	asAtomHandler::rshift(res,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_rshift_local_constant(call_context* context)
{
	//rshift
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::rshift(res,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_rshift_constant_local(call_context* context)
{
	//rshift
	asAtom res = *context->exec_pos->arg1_constant;
	asAtomHandler::rshift(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_rshift_local_local(call_context* context)
{
	//rshift
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::rshift(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_rshift_constant_constant_localresult(call_context* context)
{
	//rshift
	asAtom res = *context->exec_pos->arg1_constant;
	asAtomHandler::rshift(res,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_rshift_local_constant_localresult(call_context* context)
{
	//rshift
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::rshift(res,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_rshift_constant_local_localresult(call_context* context)
{
	//rshift
	asAtom res = *context->exec_pos->arg1_constant;
	asAtomHandler::rshift(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_rshift_local_local_localresult(call_context* context)
{
	//rshift
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::rshift(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_urshift_constant_constant(call_context* context)
{
	uint32_t i2=context->exec_pos->arg1_uint;
	uint32_t i1=(context->exec_pos->arg2_uint)&0x1f;
	LOG_CALL("urShift_cc "<<std::hex<<i2<<_(">>")<<i1<<std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i2>>i1);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_urshift_local_constant(call_context* context)
{
	uint32_t i2=asAtomHandler::toUInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	uint32_t i1=(context->exec_pos->arg2_uint)&0x1f;
	LOG_CALL("urShift_lc "<<std::hex<<i2<<_(">>")<<i1<<std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i2>>i1);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_urshift_constant_local(call_context* context)
{
	uint32_t i2=(context->exec_pos->arg1_uint);
	uint32_t i1=asAtomHandler::toUInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2))&0x1f;
	LOG_CALL("urShift_cl "<<std::hex<<i2<<_(">>")<<i1<<std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i2>>i1);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_urshift_local_local(call_context* context)
{
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::urshift(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_urshift_constant_constant_localresult(call_context* context)
{
	uint32_t i2=context->exec_pos->arg1_uint;
	uint32_t i1=(context->exec_pos->arg2_uint)&0x1f;
	LOG_CALL("urShift_ccl "<<std::hex<<i2<<_(">>")<<i1<<std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i2>>i1);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_urshift_local_constant_localresult(call_context* context)
{
	uint32_t i2=asAtomHandler::toUInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	uint32_t i1=(context->exec_pos->arg2_uint)&0x1f;
	LOG_CALL("urShift_lcl "<<std::hex<<i2<<_(">>")<<i1<<std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i2>>i1);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_urshift_constant_local_localresult(call_context* context)
{
	uint32_t i2=(context->exec_pos->arg1_uint);
	uint32_t i1=asAtomHandler::toUInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2))&0x1f;
	LOG_CALL("urShift_cll "<<std::hex<<i2<<_(">>")<<i1<<std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i2>>i1);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_urshift_local_local_localresult(call_context* context)
{
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::urshift(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_bitand_constant_constant(call_context* context)
{
	int32_t i1=context->exec_pos->arg2_int;
	int32_t i2=context->exec_pos->arg1_int;
	LOG_CALL("bitAnd_cc " << std::hex << i1 << '&' << i2 << std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i1&i2);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_bitand_local_constant(call_context* context)
{
	int32_t i1=context->exec_pos->arg2_int;
	int32_t i2=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	LOG_CALL("bitAnd_lc " << std::hex << i1 << '&' << i2 << std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i1&i2);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_bitand_constant_local(call_context* context)
{
	int32_t i1=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	int32_t i2=context->exec_pos->arg1_int;
	LOG_CALL("bitAnd_cl " << std::hex << i1 << '&' << i2 << std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i1&i2);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_bitand_local_local(call_context* context)
{
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::bit_and(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_bitand_constant_constant_localresult(call_context* context)
{
	int32_t i1=context->exec_pos->arg2_int;
	int32_t i2=context->exec_pos->arg1_int;
	LOG_CALL("bitAnd_ccl " << std::hex << i1 << '&' << i2 << std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i1&i2);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_bitand_local_constant_localresult(call_context* context)
{
	int32_t i1=context->exec_pos->arg2_int;
	int32_t i2=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	LOG_CALL("bitAnd_lcl " << std::hex << i1 << '&' << i2 << std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i1&i2);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_bitand_constant_local_localresult(call_context* context)
{
	int32_t i1=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	int32_t i2=context->exec_pos->arg1_int;
	LOG_CALL("bitAnd_cll " << std::hex << i1 << '&' << i2 << std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i1&i2);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_bitand_local_local_localresult(call_context* context)
{
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::bit_and(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_bitor_constant_constant(call_context* context)
{
	int32_t i1=context->exec_pos->arg2_int;
	int32_t i2=context->exec_pos->arg1_int;
	LOG_CALL(_("bitor_cc ") << std::hex << i1 << '|' << i2 << std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i1|i2);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_bitor_local_constant(call_context* context)
{
	int32_t i1=context->exec_pos->arg2_int;
	int32_t i2=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	LOG_CALL(_("bitor_lc ") << std::hex << i1 << '|' << i2 << std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i1|i2);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_bitor_constant_local(call_context* context)
{
	int32_t i1=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	int32_t i2=context->exec_pos->arg1_int;
	LOG_CALL(_("bitor_cl ") << std::hex << i1 << '|' << i2 << std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i1|i2);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_bitor_local_local(call_context* context)
{
	int32_t i1=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	int32_t i2=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	LOG_CALL(_("bitor_cl ") << std::hex << i1 << '|' << i2 << std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i1|i2);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_bitor_constant_constant_localresult(call_context* context)
{
	int32_t i1=context->exec_pos->arg2_int;
	int32_t i2=context->exec_pos->arg1_int;
	LOG_CALL(_("bitor_ccl ") << std::hex << i1 << '|' << i2 << std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i1|i2);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_bitor_local_constant_localresult(call_context* context)
{
	int32_t i1=context->exec_pos->arg2_int;
	int32_t i2=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	LOG_CALL(_("bitor_lcl ") << std::hex << i1 << '|' << i2 << std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i1|i2);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_bitor_constant_local_localresult(call_context* context)
{
	int32_t i1=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	int32_t i2=context->exec_pos->arg1_int;
	LOG_CALL(_("bitor_cll ") << std::hex << i1 << '|' << i2 << std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i1|i2);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_bitor_local_local_localresult(call_context* context)
{
	int32_t i1=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	int32_t i2=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	LOG_CALL(_("bitor_lll ") << std::hex << i1 << '|' << i2 << std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i1|i2);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_bitxor_constant_constant(call_context* context)
{
	int32_t i1=context->exec_pos->arg2_int;
	int32_t i2=context->exec_pos->arg1_int;
	LOG_CALL(_("bitXor_cc ") << std::hex << i1 << '^' << i2 << std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i1^i2);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_bitxor_local_constant(call_context* context)
{
	int32_t i1=context->exec_pos->arg2_int;
	int32_t i2=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	LOG_CALL(_("bitXor_lc ") << std::hex << i1 << '^' << i2 << std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i1^i2);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_bitxor_constant_local(call_context* context)
{
	int32_t i1=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	int32_t i2=context->exec_pos->arg1_int;
	LOG_CALL(_("bitXor_cl ") << std::hex << i1 << '^' << i2 << std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i1^i2);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_bitxor_local_local(call_context* context)
{
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::bit_xor(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_bitxor_constant_constant_localresult(call_context* context)
{
	int32_t i1=context->exec_pos->arg2_int;
	int32_t i2=context->exec_pos->arg1_int;
	LOG_CALL(_("bitXor_ccl ") << std::hex << i1 << '^' << i2 << std::dec);
	asAtom res;
	asAtomHandler::setInt(res,context->mi->context->root->getSystemState(),i1^i2);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_bitxor_local_constant_localresult(call_context* context)
{
	int32_t i1=context->exec_pos->arg2_int;
	int32_t i2=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1));
	LOG_CALL(_("bitXor_lcl ") << std::hex << i1 << '^' << i2 << std::dec);
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setInt(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),i1^i2);
	++(context->exec_pos);
}
void ABCVm::abc_bitxor_constant_local_localresult(call_context* context)
{
	int32_t i1=asAtomHandler::toInt(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	int32_t i2=context->exec_pos->arg1_int;
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setInt(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState(),i1^i2);
	++(context->exec_pos);
}
void ABCVm::abc_bitxor_local_local_localresult(call_context* context)
{
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::bit_xor(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	++(context->exec_pos);
}
void ABCVm::abc_equals_constant_constant(call_context* context)
{
	//equals

	bool ret=(asAtomHandler::isEqual(*context->exec_pos->arg2_constant,context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant));
	LOG_CALL(_("equals_cc ")<<ret);

	RUNTIME_STACK_PUSH(context,asAtomHandler::fromBool(ret));
	++(context->exec_pos);
}
void ABCVm::abc_equals_local_constant(call_context* context)
{
	//equals

	bool ret=(asAtomHandler::isEqual(*context->exec_pos->arg2_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1)));
	LOG_CALL(_("equals_lc ")<<ret);

	RUNTIME_STACK_PUSH(context,asAtomHandler::fromBool(ret));
	++(context->exec_pos);
}
void ABCVm::abc_equals_constant_local(call_context* context)
{
	//equals

	bool ret=(asAtomHandler::isEqual(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2)));
	LOG_CALL(_("equals_cl ")<<ret);

	RUNTIME_STACK_PUSH(context,asAtomHandler::fromBool(ret));
	++(context->exec_pos);
}
void ABCVm::abc_equals_local_local(call_context* context)
{
	//equals

	bool ret=(asAtomHandler::isEqual(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1)));
	LOG_CALL(_("equals_ll ")<<ret);

	RUNTIME_STACK_PUSH(context,asAtomHandler::fromBool(ret));
	++(context->exec_pos);
}
void ABCVm::abc_equals_constant_constant_localresult(call_context* context)
{
	//equals

	bool ret=(asAtomHandler::isEqual(*context->exec_pos->arg2_constant,context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant));
	LOG_CALL(_("equals_ccl ")<<ret);

	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	++(context->exec_pos);
}
void ABCVm::abc_equals_local_constant_localresult(call_context* context)
{
	//equals

	bool ret=(asAtomHandler::isEqual(*context->exec_pos->arg2_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1)));
	LOG_CALL(_("equals_lcl ")<<ret);

	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	++(context->exec_pos);
}
void ABCVm::abc_equals_constant_local_localresult(call_context* context)
{
	//equals

	bool ret=(asAtomHandler::isEqual(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2)));
	LOG_CALL(_("equals_cll ")<<ret);

	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	++(context->exec_pos);
}
void ABCVm::abc_equals_local_local_localresult(call_context* context)
{
	//equals

	bool ret=(asAtomHandler::isEqual(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1)));
	LOG_CALL(_("equals_lll ")<<ret);

	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	++(context->exec_pos);
}
void ABCVm::abc_lessthan_constant_constant(call_context* context)
{
	bool ret=(asAtomHandler::isLess(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant)==TTRUE);
	LOG_CALL(_("lessthan_cc ")<<ret);

	RUNTIME_STACK_PUSH(context,asAtomHandler::fromBool(ret));
	++(context->exec_pos);
}
void ABCVm::abc_lessthan_local_constant(call_context* context)
{
	bool ret=(asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant)==TTRUE);
	LOG_CALL(_("lessthan_lc ")<<ret);

	RUNTIME_STACK_PUSH(context,asAtomHandler::fromBool(ret));
	++(context->exec_pos);
}
void ABCVm::abc_lessthan_constant_local(call_context* context)
{
	bool ret=(asAtomHandler::isLess(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2))==TTRUE);
	LOG_CALL(_("lessthan_cl ")<<ret);

	RUNTIME_STACK_PUSH(context,asAtomHandler::fromBool(ret));
	++(context->exec_pos);
}
void ABCVm::abc_lessthan_local_local(call_context* context)
{
	bool ret=(asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2))==TTRUE);
	LOG_CALL(_("lessthan_ll ")<<ret);

	RUNTIME_STACK_PUSH(context,asAtomHandler::fromBool(ret));
	++(context->exec_pos);
}
void ABCVm::abc_lessthan_constant_constant_localresult(call_context* context)
{
	bool ret=(asAtomHandler::isLess(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant)==TTRUE);
	LOG_CALL(_("lessthan_ccl ")<<ret);

	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	++(context->exec_pos);
}
void ABCVm::abc_lessthan_local_constant_localresult(call_context* context)
{
	bool ret=(asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant)==TTRUE);
	LOG_CALL(_("lessthan_lcl ")<<ret);

	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	++(context->exec_pos);
}
void ABCVm::abc_lessthan_constant_local_localresult(call_context* context)
{
	bool ret=(asAtomHandler::isLess(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2))==TTRUE);
	LOG_CALL(_("lessthan_cll ")<<ret);

	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	++(context->exec_pos);
}
void ABCVm::abc_lessthan_local_local_localresult(call_context* context)
{
	bool ret=(asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2))==TTRUE);
	LOG_CALL(_("lessthan_lll ")<<ret);

	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	++(context->exec_pos);
}
void ABCVm::abc_lessequals_constant_constant(call_context* context)
{
	//lessequals

	bool ret=(asAtomHandler::isLess(*context->exec_pos->arg2_constant,context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant)==TFALSE);
	LOG_CALL(_("lessequals_cc ")<<ret);

	RUNTIME_STACK_PUSH(context,asAtomHandler::fromBool(ret));
	++(context->exec_pos);
}
void ABCVm::abc_lessequals_local_constant(call_context* context)
{
	//lessequals

	bool ret=(asAtomHandler::isLess(*context->exec_pos->arg2_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1))==TFALSE);
	LOG_CALL(_("lessequals_lc ")<<ret);

	RUNTIME_STACK_PUSH(context,asAtomHandler::fromBool(ret));
	++(context->exec_pos);
}
void ABCVm::abc_lessequals_constant_local(call_context* context)
{
	//lessequals

	bool ret=(asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant)==TFALSE);
	LOG_CALL(_("lessequals_cl ")<<ret);

	RUNTIME_STACK_PUSH(context,asAtomHandler::fromBool(ret));
	++(context->exec_pos);
}
void ABCVm::abc_lessequals_local_local(call_context* context)
{
	//lessequals

	bool ret=(asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1))==TFALSE);
	LOG_CALL(_("lessequals_ll ")<<ret);

	RUNTIME_STACK_PUSH(context,asAtomHandler::fromBool(ret));
	++(context->exec_pos);
}
void ABCVm::abc_lessequals_constant_constant_localresult(call_context* context)
{
	//lessequals

	bool ret=(asAtomHandler::isLess(*context->exec_pos->arg2_constant,context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant)==TFALSE);
	LOG_CALL(_("lessequals_ccl ")<<ret);

	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	++(context->exec_pos);
}
void ABCVm::abc_lessequals_local_constant_localresult(call_context* context)
{
	//lessequals

	bool ret=(asAtomHandler::isLess(*context->exec_pos->arg2_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1))==TFALSE);
	LOG_CALL(_("lessequals_lcl ")<<ret);

	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	++(context->exec_pos);
}
void ABCVm::abc_lessequals_constant_local_localresult(call_context* context)
{
	//lessequals

	bool ret=(asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant)==TFALSE);
	LOG_CALL(_("lessequals_cll ")<<ret);

	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	++(context->exec_pos);
}
void ABCVm::abc_lessequals_local_local_localresult(call_context* context)
{
	//lessequals

	bool ret=(asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1))==TFALSE);
	LOG_CALL(_("lessequals_lll ")<<ret);

	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	++(context->exec_pos);
}
void ABCVm::abc_greaterthan_constant_constant(call_context* context)
{
	//greaterthan

	bool ret=(asAtomHandler::isLess(*context->exec_pos->arg2_constant,context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant)==TTRUE);
	LOG_CALL(_("greaterThan_cc ")<<ret);

	RUNTIME_STACK_PUSH(context,asAtomHandler::fromBool(ret));
	++(context->exec_pos);
}
void ABCVm::abc_greaterthan_local_constant(call_context* context)
{
	//greaterthan

	bool ret=(asAtomHandler::isLess(*context->exec_pos->arg2_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1))==TTRUE);
	LOG_CALL(_("greaterThan_lc ")<<ret);

	RUNTIME_STACK_PUSH(context,asAtomHandler::fromBool(ret));
	++(context->exec_pos);
}
void ABCVm::abc_greaterthan_constant_local(call_context* context)
{
	//greaterthan

	bool ret=(asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant)==TTRUE);
	LOG_CALL(_("greaterThan_cl ")<<ret);

	RUNTIME_STACK_PUSH(context,asAtomHandler::fromBool(ret));
	++(context->exec_pos);
}
void ABCVm::abc_greaterthan_local_local(call_context* context)
{
	//greaterthan

	bool ret=(asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1))==TTRUE);
	LOG_CALL(_("greaterThan_ll ")<<ret);

	RUNTIME_STACK_PUSH(context,asAtomHandler::fromBool(ret));
	++(context->exec_pos);
}
void ABCVm::abc_greaterthan_constant_constant_localresult(call_context* context)
{
	//greaterthan

	bool ret=(asAtomHandler::isLess(*context->exec_pos->arg2_constant,context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant)==TTRUE);
	LOG_CALL(_("greaterThan_ccl ")<<ret);

	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	++(context->exec_pos);
}
void ABCVm::abc_greaterthan_local_constant_localresult(call_context* context)
{
	//greaterthan

	bool ret=(asAtomHandler::isLess(*context->exec_pos->arg2_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1))==TTRUE);
	LOG_CALL(_("greaterThan_lcl ")<<ret);

	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	++(context->exec_pos);
}
void ABCVm::abc_greaterthan_constant_local_localresult(call_context* context)
{
	//greaterthan

	bool ret=(asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->mi->context->root->getSystemState(),*context->exec_pos->arg1_constant)==TTRUE);
	LOG_CALL(_("greaterThan_cll ")<<ret);

	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	++(context->exec_pos);
}
void ABCVm::abc_greaterthan_local_local_localresult(call_context* context)
{
	//greaterthan

	bool ret=(asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1))==TTRUE);
	LOG_CALL(_("greaterThan_lll ")<<ret);

	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	++(context->exec_pos);
}
void ABCVm::abc_greaterequals_constant_constant(call_context* context)
{
	//greaterequals

	bool ret=(asAtomHandler::isLess(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant)==TFALSE);
	LOG_CALL(_("greaterequals_cc ")<<ret);

	RUNTIME_STACK_PUSH(context,asAtomHandler::fromBool(ret));
	++(context->exec_pos);
}
void ABCVm::abc_greaterequals_local_constant(call_context* context)
{
	//greaterequals

	bool ret=(asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant)==TFALSE);
	LOG_CALL(_("greaterequals_lc ")<<ret);

	RUNTIME_STACK_PUSH(context,asAtomHandler::fromBool(ret));
	++(context->exec_pos);
}
void ABCVm::abc_greaterequals_constant_local(call_context* context)
{
	//greaterequals

	bool ret=(asAtomHandler::isLess(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2))==TFALSE);
	LOG_CALL(_("greaterequals_cl ")<<ret);

	RUNTIME_STACK_PUSH(context,asAtomHandler::fromBool(ret));
	++(context->exec_pos);
}
void ABCVm::abc_greaterequals_local_local(call_context* context)
{
	//greaterequals

	bool ret=(asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2))==TFALSE);
	LOG_CALL(_("greaterequals_ll ")<<ret);

	RUNTIME_STACK_PUSH(context,asAtomHandler::fromBool(ret));
	++(context->exec_pos);
}
void ABCVm::abc_greaterequals_constant_constant_localresult(call_context* context)
{
	//greaterequals

	bool ret=(asAtomHandler::isLess(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant)==TFALSE);
	LOG_CALL(_("greaterequals_ccl ")<<ret);

	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	++(context->exec_pos);
}
void ABCVm::abc_greaterequals_local_constant_localresult(call_context* context)
{
	//greaterequals

	bool ret=(asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant)==TFALSE);
	LOG_CALL(_("greaterequals_lcl ")<<ret);

	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	++(context->exec_pos);
}
void ABCVm::abc_greaterequals_constant_local_localresult(call_context* context)
{
	//greaterequals

	bool ret=(asAtomHandler::isLess(*context->exec_pos->arg1_constant,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2))==TFALSE);
	LOG_CALL(_("greaterequals_cll ")<<ret);

	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	++(context->exec_pos);
}
void ABCVm::abc_greaterequals_local_local_localresult(call_context* context)
{
	//greaterequals

	bool ret=(asAtomHandler::isLess(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1),context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2))==TFALSE);
	LOG_CALL(_("greaterequals_lll ")<<ret<<" "<<asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1))<<" "<<asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2)));

	ASATOM_DECREF(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::setBool(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),ret);
	++(context->exec_pos);
}
void ABCVm::abc_increment_i_local(call_context* context)
{
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	LOG_CALL("increment_i_l "<<context->exec_pos->local_pos1<<" "<<asAtomHandler::toDebugString(res));
	asAtomHandler::increment_i(res,context->mi->context->root->getSystemState());
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_increment_i_local_localresult(call_context* context)
{
	LOG_CALL("increment_i_ll "<<context->exec_pos->local_pos1<<" "<<context->exec_pos->local3.pos<<" "<<asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1)));
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	asAtomHandler::increment_i(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState());
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_decrement_i_local(call_context* context)
{
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	LOG_CALL("decrement_i_l "<<context->exec_pos->local_pos1<<" "<<asAtomHandler::toDebugString(res));
	asAtomHandler::decrement_i(res,context->mi->context->root->getSystemState());
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_decrement_i_local_localresult(call_context* context)
{
	LOG_CALL("decrement_i_ll "<<context->exec_pos->local_pos1<<" "<<context->exec_pos->local3.pos<<" "<<asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos)));
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	asAtomHandler::decrement_i(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),context->mi->context->root->getSystemState());
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_add_i_constant_constant(call_context* context)
{
	LOG_CALL("add_i_cc");
	asAtom res = *context->exec_pos->arg1_constant;
	asAtomHandler::add_i(res,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_add_i_local_constant(call_context* context)
{
	LOG_CALL("add_i_lc");
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::add_i(res,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_add_i_constant_local(call_context* context)
{
	LOG_CALL("add_i_cl");
	asAtom res = *context->exec_pos->arg1_constant;
	asAtomHandler::add_i(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_add_i_local_local(call_context* context)
{
	LOG_CALL("add_i_ll");
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	asAtomHandler::add_i(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_add_i_constant_constant_localresult(call_context* context)
{
	LOG_CALL("add_i_ccl");
	asAtom res = *context->exec_pos->arg1_constant;
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::add_i(res,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant);
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_add_i_local_constant_localresult(call_context* context)
{
	LOG_CALL("add_i_lcl");
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::add_i(res,context->mi->context->root->getSystemState(),*context->exec_pos->arg2_constant);
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_add_i_constant_local_localresult(call_context* context)
{
	LOG_CALL("add_i_cll");
	asAtom res = *context->exec_pos->arg1_constant;
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::add_i(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_add_i_local_local_localresult(call_context* context)
{
	LOG_CALL("add_i_lll");
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::add_i(res,context->mi->context->root->getSystemState(),CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	if (o)
		o->decRef();
	++(context->exec_pos);
}

void ABCVm::abc_inclocal_i_optimized(call_context* context)
{
	int32_t t = context->exec_pos->arg1_uint;
	LOG_CALL( _("incLocal_i_o ") << t );
	asAtomHandler::increment_i(CONTEXT_GETLOCAL(context,t),context->mi->context->root->getSystemState());
	++context->exec_pos;
}
void ABCVm::abc_declocal_i_optimized(call_context* context)
{
	int32_t t = context->exec_pos->arg1_uint;
	LOG_CALL( _("decLocal_i_o ") << t );
	asAtomHandler::decrement_i(CONTEXT_GETLOCAL(context,t),context->mi->context->root->getSystemState());
	++context->exec_pos;
}
void ABCVm::abc_dup_local(call_context* context)
{
	LOG_CALL("dup_l "<<context->exec_pos->local_pos1<<" "<<(context->exec_pos->local3.pos));
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	ASATOM_INCREF(res);
	RUNTIME_STACK_PUSH(context,res);
	++(context->exec_pos);
}
void ABCVm::abc_dup_local_localresult(call_context* context)
{
	LOG_CALL("dup_ll "<<context->exec_pos->local_pos1<<" "<<(context->exec_pos->local3.pos));
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	ASATOM_INCREF(res);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_dup_increment_local_localresult(call_context* context)
{
	LOG_CALL("dup_increment_ll "<<context->exec_pos->local_pos1<<" "<<context->exec_pos->local_pos2<<" "<<context->exec_pos->local3.pos<<" "<<asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1)));
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),res);
	asAtomHandler::increment(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->mi->context->root->getSystemState());
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_dup_decrement_local_localresult(call_context* context)
{
	LOG_CALL("dup_decrement_ll "<<context->exec_pos->local_pos1<<" "<<context->exec_pos->local_pos2<<" "<<context->exec_pos->local3.pos<<" "<<asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1)));
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),res);
	asAtomHandler::decrement(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->mi->context->root->getSystemState());
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_dup_increment_i_local_localresult(call_context* context)
{
	LOG_CALL("dup_increment_i_ll "<<context->exec_pos->local_pos1<<" "<<context->exec_pos->local_pos2<<" "<<context->exec_pos->local3.pos<<" "<<asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1)));
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),res);
	asAtomHandler::increment_i(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->mi->context->root->getSystemState());
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
void ABCVm::abc_dup_decrement_i_local_localresult(call_context* context)
{
	LOG_CALL("dup_decrement_i_ll "<<context->exec_pos->local_pos1<<" "<<context->exec_pos->local_pos2<<" "<<context->exec_pos->local3.pos<<" "<<asAtomHandler::toDebugString(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1)));
	asAtom res = CONTEXT_GETLOCAL(context,context->exec_pos->local_pos1);
	ASObject* o = asAtomHandler::getObject(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2));
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),res);
	asAtomHandler::decrement_i(CONTEXT_GETLOCAL(context,context->exec_pos->local_pos2),context->mi->context->root->getSystemState());
	asAtomHandler::set(CONTEXT_GETLOCAL(context,context->exec_pos->local3.pos),res);
	if (o)
		o->decRef();
	++(context->exec_pos);
}
