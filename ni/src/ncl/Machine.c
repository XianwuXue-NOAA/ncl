
/*
 *      $Id: Machine.c,v 1.42 1995-06-17 01:21:33 ethan Exp $
 */
/************************************************************************
*									*
*			     Copyright (C)  1993			*
*	     University Corporation for Atmospheric Research		*
*			     All Rights Reserved			*
*									*
************************************************************************/
/*
 *	File:		Machine.c
 *
 *	Author:		Ethan Alpert
 *			National Center for Atmospheric Research
 *			PO 3000, Boulder, Colorado
 *
 *	Date:		Wed Jul 7 09:08:34 MDT 1993
 *
 *	Description:	Contains functions for manipulating stacks, frames
 *			and the instruction list.
 */
#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <ncarg/hlu/hlu.h>
#include <ncarg/hlu/NresDB.h>
#include <errno.h>
#include <math.h>
#include "defs.h"
#include "Symbol.h"
#include "NclVar.h"
#include "NclMultiDValData.h"
#include "NclAtt.h"
#include "Machine.h"
#include "OpsList.h"
#include "VarSupport.h"
#include "AttSupport.h"
#include "DataSupport.h"

/*
* This is done so stack size and machine size can be configured at 
* compile time
*/
#ifndef NCL_LEVEL_1_SIZE
#define NCL_LEVEL_1_SIZE 512
#endif

#ifndef NCL_STACK_SIZE
#define NCL_STACK_SIZE 1024
#endif

#ifndef NCL_MACHINE_SIZE
#define NCL_MACHINE_SIZE 16384
#endif

/*
* Making this smaller because functions will generally be smaller than 
* whole programs.
*/
#ifndef NCL_FUNC_MACHINE_SIZE
#define NCL_FUNC_MACHINE_SIZE 2048
#endif

NclStackEntry *thestack;
int cur_stacksize = NCL_STACK_SIZE;

char *ops_strings[NUM_OPERATORS];

_NclMachineStack *mstk;

NclStackEntry  *level_1_vars;
int	current_level_1_size;

/*
NclFrame *fp;
NclStackEntry *sb;
*/
unsigned int fp;
unsigned int sb;
int sb_off;
unsigned int current_scope_level = 1;

static void SetUpOpsStrings() {
	ops_strings[0] = "NOOP";
	ops_strings[NOOP] = "NOOP";
	ops_strings[STOPSEQ] = "STOPSEQ";
	ops_strings[ENDSTMNT_OP] = "ENDSTMNT_OP";
	ops_strings[RETURN_OP] = "RETURN_OP";
	ops_strings[FPDEF] = "FPDEF";
	ops_strings[NEW_FRAME_OP] = "NEW_FRAME_OP";
	ops_strings[BPROC_CALL_OP] = "BPROC_CALL_OP";
	ops_strings[FUNC_CALL_OP] = "FUNC_CALL_OP";
	ops_strings[INTRINSIC_FUNC_CALL] = "INTRINSIC_FUNC_CALL";
	ops_strings[INTRINSIC_PROC_CALL] = "INTRINSIC_PROC_CALL";
	ops_strings[PROC_CALL_OP] = "PROC_CALL_OP";
	ops_strings[JMP] = "JMP";
	ops_strings[JMPFALSE] = "JMPFALSE";
	ops_strings[DO_FROM_TO_OP] = "DO_FROM_TO_OP";
	ops_strings[DO_FROM_TO_STRIDE_OP] = "DO_FROM_TO_STRIDE_OP";
	ops_strings[DO_WHILE_OP] = "DO_WHILE_OP";
	ops_strings[BREAK_OP] = "BREAK_OP";
	ops_strings[CONTINUE_OP] = "CONTINUE_OP";
	ops_strings[CREATE_OBJ_OP] = "CREATE_OBJ_OP";
	ops_strings[SET_OBJ_OP] = "SET_OBJ_OP";
	ops_strings[GET_OBJ_OP] = "GET_OBJ_OP";
	ops_strings[CONVERT_TO_LOCAL] = "CONVERT_TO_LOCAL";
	ops_strings[PUSH_STRING_LIT_OP] = "PUSH_STRING_LIT_OP";
	ops_strings[PUSH_REAL_LIT_OP] = "PUSH_REAL_LIT_OP";
	ops_strings[PUSH_INT_LIT_OP] = "PUSH_INT_LIT_OP";
	ops_strings[ARRAY_LIT_OP] = "ARRAY_LIT_OP";
	ops_strings[NAMED_INT_SUBSCRIPT_OP] = "NAMED_INT_SUBSCRIPT_OP";
	ops_strings[INT_SUBSCRIPT_OP] = "INT_SUBSCRIPT_OP";
	ops_strings[NAMED_COORD_SUBSCRIPT_OP] = "NAMED_COORD_SUBSCRIPT_OP";
	ops_strings[COORD_SUBSCRIPT_OP] = "COORD_SUBSCRIPT_OP";
	ops_strings[SINGLE_INDEX_OP] = "SINGLE_INDEX_OP";
	ops_strings[DEFAULT_RANGE_OP] = "DEFAULT_RANGE_OP";
	ops_strings[BFUNC_CALL_OP] = "BFUNC_CALL_OP";
	ops_strings[RANGE_INDEX_OP] = "RANGE_INDEX_OP";
	ops_strings[NEG_OP] = "NEG_OP";
	ops_strings[NOT_OP] = "NOT_OP";
	ops_strings[MOD_OP] = "MOD_OP";
	ops_strings[OR_OP] = "OR_OP";
	ops_strings[AND_OP] = "AND_OP";
	ops_strings[XOR_OP] = "XOR_OP";
	ops_strings[LTSEL_OP] = "LTSEL_OP";
	ops_strings[GTSEL_OP] = "GTSEL_OP";
	ops_strings[PLUS_OP] = "PLUS_OP";
	ops_strings[MINUS_OP] = "MINUS_OP";
	ops_strings[MUL_OP] = "MUL_OP";
	ops_strings[MAT_OP] = "MAT_OP";
	ops_strings[DIV_OP] = "DIV_OP";
	ops_strings[EXP_OP] = "EXP_OP";
	ops_strings[LE_OP] = "LE_OP";
	ops_strings[GE_OP] = "GE_OP";
	ops_strings[GT_OP] = "GT_OP";
	ops_strings[LT_OP] = "LT_OP";
	ops_strings[EQ_OP] = "EQ_OP";
	ops_strings[NE_OP] = "NE_OP";
	ops_strings[FILE_VAR_OP] = "FILE_VAR_OP";
	ops_strings[ASSIGN_FILE_VAR_OP] = "ASSIGN_FILE_VAR_OP";
	ops_strings[PARAM_FILE_VAR_OP] = "PARAM_FILE_VAR_OP";
	ops_strings[FILEVAR_DIM_OP] = "FILEVAR_DIM_OP";
	ops_strings[ASSIGN_FILEVAR_DIM_OP] = "ASSIGN_FILEVAR_DIM_OP";
	ops_strings[PARAM_FILEVAR_DIM_OP] = "PARAM_FILEVAR_DIM_OP";
	ops_strings[FILEVAR_COORD_OP] = "FILEVAR_COORD_OP";
	ops_strings[ASSIGN_FILEVAR_COORD_OP] = "ASSIGN_FILEVAR_COORD_OP";
	ops_strings[PARAM_FILEVAR_COORD_OP] = "PARAM_FILEVAR_COORD_OP";
	ops_strings[FILEVARATT_OP] = "FILEVARATT_OP";
	ops_strings[ASSIGN_FILEVARATT_OP] = "ASSIGN_FILEVARATT_OP";
	ops_strings[PARAM_FILEVARATT_OP] = "PARAM_FILEVARATT_OP";
	ops_strings[VAR_READ_OP] = "VAR_READ_OP";
	ops_strings[ASSIGN_VAR_OP] = "ASSIGN_VAR_OP";
	ops_strings[PARAM_VAR_OP] = "PARAM_VAR_OP";
	ops_strings[VARATT_OP] = "VARATT_OP";
	ops_strings[ASSIGN_VARATT_OP] = "ASSIGN_VARATT_OP";
	ops_strings[PARAM_VARATT_OP] = "PARAM_VARATT_OP";
	ops_strings[VAR_COORD_OP] = "VAR_COORD_OP";
	ops_strings[VAR_READ_COORD_OP] = "VAR_READ_COORD_OP";
	ops_strings[ASSIGN_VAR_COORD_OP] = "ASSIGN_VAR_COORD_OP";
	ops_strings[PARAM_VAR_COORD_OP] = "PARAM_VAR_COORD_OP";
	ops_strings[VAR_DIM_OP]= "VAR_DIM_OP";
	ops_strings[ASSIGN_VAR_DIM_OP]= "ASSIGN_VAR_DIM_OP";
	ops_strings[PARAM_VAR_DIM_OP]= "PARAM_VAR_DIM_OP";
	ops_strings[ASSIGN_VAR_VAR_OP]= "ASSIGN_VAR_VAR_OP";
	ops_strings[DUP_TOFS]= "DUP_TOFS";
	ops_strings[PUSH_LOG_LIT_OP]= "PUSH_LOG_LIT_OP";
	ops_strings[JMP_SCALAR_TRUE_OP] = "JMP_SCALAR_TRUE_OP";
	ops_strings[JMP_SCALAR_FALSE_OP] = "JMP_SCALAR_FALSE_OP";
	ops_strings[ISDEFINED_OP] = "ISDEFINED_OP";

}

static NhlErrorTypes IncreaseStackSize
#if	NhlNeedProto
(void)
#else
()
#endif
{
	NclStackEntry* tmpstack;
	int i;

	tmpstack = thestack;

	thestack = NclRealloc(thestack,cur_stacksize*2*sizeof(NclStackEntry));
	if(thestack == NULL ) {
		thestack = tmpstack;
		return(NhlFATAL);
	} else {
		tmpstack = thestack;
		for(i = cur_stacksize; i < cur_stacksize * 2; i++ ) {
			tmpstack[i].kind = NclStk_NOVAL;
			tmpstack[i].u.offset = 0;
		}
		cur_stacksize *= 2;
		return(NhlNOERROR);
	}
}

NclValue *_NclGetCurrentMachine
#if	NhlNeedProto
(void)
#else
()
#endif
{
	return(mstk->the_rec->themachine);
}

NclStackEntry *_NclPeek
#if	NhlNeedProto
(int offset)
#else
(offset)
	int offset;
#endif
{
	return((NclStackEntry*)(thestack + ( sb + sb_off - (offset + 1))));
}

NhlErrorTypes _NclPutArg
#if	NhlNeedProto
(NclStackEntry data, int arg_num,int total_args)
#else
(data,arg_num,total_args)
NclStackEntry data;
int arg_num;
int total_args;
#endif
{
	NclStackEntry *ptr;

	if((sb+sb_off-total_args + arg_num) >= cur_stacksize) {
		if(IncreaseStackSize() == NhlFATAL) {
			NhlPError(NhlFATAL,NhlEUNKNOWN,"Can not increase stack size, memory error can't continue");
			return(NhlFATAL);
		}
	}
	ptr = ((NclStackEntry*)(thestack + ( sb + sb_off - total_args))) + arg_num;
	*ptr = data;
 
	return(NhlNOERROR);
}

NclStackEntry _NclGetArg
#if	NhlNeedProto
(int arg_num,int total_args,int access_type)
#else
(arg_num,total_args,access_type)
int arg_num;
int total_args;
int access_type;
#endif
{
	NclStackEntry *ptr;
	NclStackEntry tmp ;
	NclFrame *fpt;
	struct _NclParamRecList *the_list;

	fpt = (NclFrame*)(thestack + fp);
	the_list = (fpt->parameter_map.u.the_list);

	if(!(the_list->the_elements[arg_num].is_modified)&&(access_type)) {
		the_list->the_elements[arg_num].is_modified = access_type;
	} else if(access_type == WRITE_IT) {
		the_list->the_elements[arg_num].is_modified = access_type;
	}

	if( sb + sb_off - total_args + arg_num  >= cur_stacksize ) {
		tmp.kind = NclStk_NOVAL;
		tmp.u.offset = 0;
		return(tmp);
	}
	ptr = ((NclStackEntry*)(thestack + ( sb + sb_off - total_args))) + arg_num;

	return(*ptr);
}

int *_NclGetCurrentLineRec
#if	NhlNeedProto
(void)
#else
()
#endif
{
	return(mstk->the_rec->thelines);
}

char **_NclGetCurrentFileNameRec
#if	NhlNeedProto
(void)
#else
()
#endif
{
	return(mstk->the_rec->thefiles);
}

void _NclNewMachine
#if	NhlNeedProto
(void)
#else
()
#endif
{
	_NclMachineRec* tmp;
	tmp = (_NclMachineRec*)NclMalloc((unsigned)sizeof(_NclMachineRec));
	tmp->themachine = (NclValue*)NclCalloc(NCL_FUNC_MACHINE_SIZE,sizeof(NclValue));
	tmp->thefiles = (char**)NclCalloc(NCL_FUNC_MACHINE_SIZE,sizeof(char*));
	tmp->thelines = (int*)NclCalloc(NCL_FUNC_MACHINE_SIZE,sizeof(int));
	if(tmp->themachine == NULL ){
		NhlPError(NhlFATAL,errno,"_NhlNewMachine: Can't allocate space for new machine");
		return;
	}
	tmp->pc = tmp->themachine;
	tmp->lc = tmp->thelines;
	tmp->fn = tmp->thefiles;
	tmp->pcoffset = 0;
	tmp->current_machine_size = NCL_FUNC_MACHINE_SIZE;
	_NclPushMachine((void*)tmp);
}

void *_NclPopMachine
#if	NhlNeedProto
(void)
#else
()
#endif
{
	_NclMachineStack* tmp;
	_NclMachineRec* the_rec;

	tmp = mstk;
	mstk = mstk->next;
	the_rec = tmp->the_rec;
	NclFree(tmp);
	return((void*)the_rec);
}
void _NclPushMachine
#if	NhlNeedProto
(void * the_mach_rec)
#else
(the_mach_rec)
	void * the_mach_rec;
#endif
{
	_NclMachineStack* tmp;

	tmp = (_NclMachineStack*)NclMalloc((unsigned)sizeof(_NclMachineStack));

	tmp->next = mstk;
	tmp->the_rec  = (_NclMachineRec*)the_mach_rec;
	mstk = tmp;
	return;
}

void _NclResetMachine
#if	NhlNeedProto
(void)
#else
()
#endif
{
	fp = 0;
	if((NclStackEntry*)(thestack + (sb + sb_off)) != thestack) {
		NhlPError(NhlWARNING,NhlEUNKNOWN,"ResetMachine: reseting non-empty stack, memory may leak!");
	}
/*
	sb = thestack;
*/
	sb = 0;
	sb_off = 0;
	mstk->the_rec->pcoffset = 0;
	mstk->the_rec->pc = mstk->the_rec->themachine;
	mstk->the_rec->lc = mstk->the_rec->thelines;
	mstk->the_rec->fn = mstk->the_rec->thefiles;
	return;
}


static NhlErrorTypes IncreaseMachineSize
#if	NhlNeedProto
(void)
#else 
()
#endif
{
	mstk->the_rec->themachine = (NclValue*)NclRealloc(mstk->the_rec->themachine,mstk->the_rec->current_machine_size*2*sizeof(NclValue));
	mstk->the_rec->thefiles = (char**)NclRealloc(mstk->the_rec->thefiles,mstk->the_rec->current_machine_size*2*sizeof(char*));
	mstk->the_rec->thelines = (int*)NclRealloc(mstk->the_rec->thelines,mstk->the_rec->current_machine_size*2*sizeof(int));
	mstk->the_rec->current_machine_size *=2;
	if(mstk->the_rec->themachine == NULL) {
		NhlPError(NhlFATAL,errno,"IncreaseMachineSize: Unable to increase the size of the machine");
		return(NhlFATAL);

	}
/*
* Since a new pointer is possible here a new value of pc needs to be computed
* from the current pcoffset value
*/
	mstk->the_rec->pc = &(mstk->the_rec->themachine[mstk->the_rec->pcoffset]);
	mstk->the_rec->fn = &(mstk->the_rec->thefiles[mstk->the_rec->pcoffset]);
	mstk->the_rec->lc = &(mstk->the_rec->thelines[mstk->the_rec->pcoffset]);
	return(NhlNOERROR);
}
	

NhlErrorTypes _NclInitMachine
#if	NhlNeedProto
(void)
#else
()
#endif
{
	int i;

	thestack = (NclStackEntry*)NclMalloc((unsigned)sizeof(NclStackEntry)*NCL_STACK_SIZE);
	for(i = 0; i< NCL_STACK_SIZE; i++) {
		thestack[i].kind = NclStk_NOVAL;
		thestack[i].u.offset = 0;
	}
/*
	fp = (NclFrame*)thestack;
	sb = thestack;
*/
	fp = 0;
	sb = 0;
	sb_off = 0;
	mstk = (_NclMachineStack*)NclMalloc((unsigned)sizeof(_NclMachineStack));
	mstk->the_rec = (_NclMachineRec*)NclMalloc((unsigned)sizeof(_NclMachineRec));
	mstk->the_rec->themachine = (NclValue*)NclCalloc(NCL_MACHINE_SIZE,sizeof(NclValue));
	mstk->the_rec->thefiles = (char**)NclCalloc(NCL_MACHINE_SIZE,sizeof(char*));
	mstk->the_rec->thelines = (int*)NclCalloc(NCL_MACHINE_SIZE,sizeof(int));
	if(mstk->the_rec->themachine == NULL ){
		NhlPError(NhlFATAL,errno,"_NhlInitMachine: Can't allocate space for machine");
		return(NhlFATAL);
	}
	mstk->the_rec->pc = mstk->the_rec->themachine;
	mstk->the_rec->lc = mstk->the_rec->thelines;
	mstk->the_rec->fn = mstk->the_rec->thefiles;
	mstk->the_rec->pcoffset = 0;
	mstk->the_rec->current_machine_size = NCL_MACHINE_SIZE;
	mstk->next = NULL;
	SetUpOpsStrings();
/*
* Now set up level 1 variable storage locations
*/
	
	level_1_vars = (NclStackEntry*)NclCalloc(NCL_LEVEL_1_SIZE,
		sizeof(NclStackEntry));
	current_level_1_size = NCL_LEVEL_1_SIZE;
	if(level_1_vars == NULL) {
		NhlPError(NhlFATAL,errno,"_NhlInitMachine: Can't allocate space for machine");
		return(NhlFATAL);
	}
	return(NhlNOERROR);
}

NhlErrorTypes _NclPutLevel1Var
#if	NhlNeedProto
(int offset,NclStackEntry *therec) 
#else
(offset,therec)
	int offset;
	NclStackEntry *therec;
#endif
{ 	
	if((offset >= current_level_1_size)||(offset < 0)){
		return(NhlWARNING);
	} else {
		
		level_1_vars[offset] = *therec;
		return(NhlNOERROR);
	}
}
NclStackEntry *_NclGetLevel1Var
#if	NhlNeedProto
(int offset) 
#else
(offset)
	int offset;
#endif
{ 	
	if((offset >= current_level_1_size)||(offset < 0)){
		return(NULL);
	} else {
		return(&(level_1_vars[offset]));
	}
}

NhlErrorTypes _NclPutRec
#if NhlNeedProto
(NclSymbol *thesym, NclStackEntry* therec)
#else
(thesym, therec)
NclSymbol *thesym;
NclStackEntry* therec;
#endif
{
	int i;
	NclFrame *previous;


	i = current_scope_level;
	

	if(thesym->level == 1) {
		return(_NclPutLevel1Var(thesym->offset,therec));
	} else if(thesym->level != 0){
		previous = (NclFrame*)(thestack + fp);
		while(i != thesym->level) {
			i--;
			previous = (NclFrame*)((NclStackEntry*)thestack + previous->static_link.u.offset);
		}

/*
* increment over stack frame stuff to base of actual scope
*/
		previous++;
		*(NclStackEntry*)((NclStackEntry*)previous + thesym->offset) = *therec;
		return(NhlNOERROR);
	} else {
		NhlPError(NhlFATAL,NhlEUNKNOWN,"Attempt to reference keyword (%s) as variable",thesym->name);
		return(NhlFATAL);
	}
}

int _NclIsParam
#if NhlNeedProto
(NclSymbol* thesym)
#else
(thesym)
NclSymbol* thesym;
#endif
{
	int i;
	NclFrame *previous;
	NclParamRecList *the_list;
	if((thesym->level == 1)||(thesym->level == 0)) {
		return(0);
	} else {
		i = current_scope_level;
		previous = (NclFrame*)(thestack + fp);
		while(i != thesym->level) {
			i--;
			previous = (NclFrame*)((NclStackEntry*)thestack + previous->static_link.u.offset);
		}
		if(previous->parameter_map.kind == NclStk_PARAMLIST) {
			the_list = previous->parameter_map.u.the_list;
			if(thesym->offset < the_list->n_elements) {
				return(1);
			} else {
				return(0);
			}
		} else {
			return(0);
		}
	}
}
NclStackEntry *_NclRetrieveRec
#if	NhlNeedProto
(NclSymbol* the_sym,int access_type)
#else
(the_sym,access_type)
NclSymbol* the_sym;
int access_type;
#endif
{
	int i;
	NclFrame *previous;
	NclParamRecList *the_list;


	i = current_scope_level;
	

	if(the_sym->level == 1) {
		return(_NclGetLevel1Var(the_sym->offset));
	} else if(the_sym->level != 0){
		previous = (NclFrame*)(thestack + fp);
		while(i != the_sym->level) {
			i--;
			previous = (NclFrame*)((NclStackEntry*)thestack + previous->static_link.u.offset);
		}

/*
* Mark it if its read
*/
		if(previous->parameter_map.kind == NclStk_PARAMLIST) {
			the_list = previous->parameter_map.u.the_list;
		}
		if((access_type == WRITE_IT)
			&&(the_list != NULL)&&(the_sym->offset < the_list->n_elements)) {

			the_list->the_elements[the_sym->offset].is_modified = 1;
		}
/*
* increment over stack frame stuff to base of actual scope
*/
		previous++;
		return((NclStackEntry*)((NclStackEntry*)previous + the_sym->offset));
	} else {
		NhlPError(NhlFATAL,NhlEUNKNOWN,"Attempt to reference keyword (%s) as variable",the_sym->name);
		return(NULL);
	}
}

static struct _NclFrameList flist ;

static void SaveFramePtrNLevel
#if	NhlNeedProto
(unsigned int tmp_fp,int level,unsigned int tmp_sb)
#else
(tmp_fp,level,tmp_sb)
	unsigned int tmp_fp;
	int level;
	unsigned int tmp_sb;
#endif
{
	static inited = 0;

	struct _NclFrameList *tmp;

	if(!inited) {
		flist.next = NULL;
		flist.fp = 0;
		flist.sb = 0;
		flist.level = -1;
		inited = 1;
	}
	tmp = flist.next;

	flist.next = (NclFrameList*)NclMalloc((unsigned)sizeof(NclFrameList));
	flist.next->fp = tmp_fp;
	flist.next->sb = tmp_sb;
	flist.next->level = level;
	flist.next->next = tmp;
}

void _NclAbortFrame
#if	NhlNeedProto
(void)
#else
()
#endif
{
	struct _NclFrameList *tmp;
	struct _NclFrame *tmp_fp;

	if(flist.next != NULL) {
		while(flist.next != NULL) {
			tmp = flist.next;
			tmp_fp = (NclFrame*)(thestack + tmp->fp);
			flist.next = flist.next->next;
			NclFree(tmp);
		}
/*
* May need to reset sb_off
*/
		_NclCleanUpStack( (int)( (NclStackEntry*) (thestack + (sb + sb_off))  - (NclStackEntry*) (tmp_fp)));
		sb_off = 0;
		sb = (unsigned int)((NclStackEntry*)tmp_fp - thestack);
	}
}

void _NclClearToStackBase
#if	NhlNeedProto
(void)
#else
()
#endif
{
	_NclCleanUpStack(sb_off);
}


static int SetNextFramePtrNLevel
#if	NhlNeedProto
(void)
#else
()
#endif
{
	struct _NclFrameList *tmp;
	int tmp_level;

	tmp_level = current_scope_level;

	tmp = flist.next;
	if(tmp != NULL) {
		flist.next = flist.next->next;
		fp = tmp->fp;
		sb = tmp->sb;
		sb_off = 0;
		current_scope_level = tmp->level;
		NclFree(tmp);
	}
	return(tmp_level);
	
}

void _NclPopFrame
#if	NhlNeedProto
(int popping_from)
#else 
(popping_from)
	int popping_from;
#endif
{
	int i,j;
	NclStackEntry data;

	switch(popping_from) {
	case FUNC_CALL_OP:	
	case INTRINSIC_FUNC_CALL:
	case BFUNC_CALL_OP:
		for(i = 0;i<(sizeof(NclFrame)/sizeof(NclStackEntry))-1; i++) {
			data = _NclPop();
			if(data.kind == NclStk_PARAMLIST) {
				if(data.u.the_list != NULL) {
					if(data.u.the_list->the_elements != NULL) {	
						for( j = 0; j < data.u.the_list->n_elements; j++) {
							if(data.u.the_list->the_elements[j].rec != NULL) {
								NclFree(data.u.the_list->the_elements[j].rec );
							}
						}
						NclFree(data.u.the_list->the_elements);
					}
					NclFree(data.u.the_list);
				}
			}
		}
	break;
	default:
		for(i = 0 ; i < (sizeof(NclFrame)/sizeof(NclStackEntry)); i++) {
			data = _NclPop();
			if(data.kind == NclStk_PARAMLIST) {
				if(data.u.the_list != NULL) {
					if(data.u.the_list->the_elements != NULL) {	
						for( j = 0; j < data.u.the_list->n_elements; j++) {
							if(data.u.the_list->the_elements[j].rec != NULL) {
								NclFree(data.u.the_list->the_elements[j].rec );
							}
						}
						NclFree(data.u.the_list->the_elements);
					}
					NclFree(data.u.the_list);
				}
			}
        	}
        break;
	}
}
NhlErrorTypes _NclPushFrame
#if	NhlNeedProto
(struct _NclSymbol *the_sym,unsigned long next_instr_offset)
#else
(the_sym,next_instr_offset)
	struct _NclSymbol *the_sym;
	unsigned long next_instr_offset;
#endif
{
	NclFrame *tmp,*tmp_fp; 
	NclFrame *previous; 
	int i;
	int nargs =  the_sym->u.procfunc->nargs; 
	int new_scope_level = 2;
	int new_scope_cur_off= 0;


	if(the_sym->u.procfunc->thescope != NULL) {
		new_scope_level = the_sym->u.procfunc->thescope->level; 
		new_scope_cur_off = the_sym->u.procfunc->thescope->cur_offset;
	} else {
		new_scope_level = current_scope_level + 1;
		new_scope_cur_off = the_sym->u.procfunc->nargs;
	}



	if(sb + sb_off + sizeof(NclFrame)/sizeof(NclStackEntry)>= cur_stacksize) {
		if(IncreaseStackSize() == NhlFATAL) {
			NhlPError(NhlFATAL,NhlEUNKNOWN,"Increasing stack size failed, memory allocation problem, can't continue");
			return(NhlFATAL);
		}
	}
	tmp = (NclFrame*)(thestack + (sb + sb_off));
	tmp->func_ret_value.kind = NclStk_RETURNVAL;
	tmp->func_ret_value.u.data_obj= (NclMultiDValData)the_sym;
	if(new_scope_level == current_scope_level+1) {
		tmp->static_link.u.offset  = (unsigned long)fp;
		tmp->static_link.kind = NclStk_STATIC_LINK;
	} else if(new_scope_level == current_scope_level) {
		tmp->static_link.u.offset = ((NclFrame*)(thestack + fp))->static_link.u.offset;
		tmp->static_link.kind = NclStk_STATIC_LINK;
	} else  {	
		previous = (NclFrame*)(thestack + fp);
		i = current_scope_level - new_scope_level;
		while(i-- >= 0) {
			previous = (NclFrame*)((NclStackEntry*)thestack + previous->static_link.u.offset);
		}
		tmp->static_link.u.offset = (unsigned long)((NclStackEntry*)previous - (NclStackEntry*)thestack);
		tmp->static_link.kind = NclStk_STATIC_LINK;
	}
	tmp->dynamic_link.u.offset  = fp;
	tmp->dynamic_link.kind = NclStk_DYNAMIC_LINK;

/*
* Maybe should be pcoffset + 1???
*/
	tmp->return_pcoffset.u.offset = next_instr_offset ;
	tmp->return_pcoffset.kind = NclStk_RET_OFFSET;
	if(nargs > 0) {
		tmp->parameter_map.kind = NclStk_PARAMLIST;
		tmp->parameter_map.u.the_list = (NclParamRecList*)NclMalloc((unsigned) sizeof(NclParamRecList));
		tmp->parameter_map.u.the_list->fpsym = the_sym;
		tmp->parameter_map.u.the_list->n_elements = nargs;
		tmp->parameter_map.u.the_list->the_elements = (NclParamRec*)
					NclMalloc((unsigned)sizeof(NclParamRec)*
					nargs);
			
		for(i = 0 ; i< nargs; i++) {
			tmp->parameter_map.u.the_list->the_elements[i].p_type = NONE_P;
			tmp->parameter_map.u.the_list->the_elements[i].var_sym = NULL;
			tmp->parameter_map.u.the_list->the_elements[i].rec = NULL;
		}
	} else {
		tmp->parameter_map.kind = NclStk_PARAMLIST;
		tmp->parameter_map.u.the_list = NULL;
		tmp->parameter_map.u.the_list = NULL;
	}
	tmp_fp = tmp;
	
	tmp++;
/*
* The stack frame has to be temporarily set to this 
* since the frame pointer can't be set until the
* arguments are executed the CONVERT_TO_LOCAL operator
* can't use the normal lookup to get the correct stack
* location to place a local variable. This means that
* for the _NclRetrieveRec and _NclPush to be used to
* implement CONVERT_TO_LOCAL the stack base must 
* be temporarily save with the new frame pointer information
*/
	sb = (unsigned int)((NclStackEntry*)tmp - thestack);
	sb_off = 0;
	

/* 
* the frame pointer needs to be returned so it can be set after all of the
* arguments have been pushed on to the stack. If this isn't done then the
*  the _NclRetrieveRec function won't be able to find the correct parameters
* referenced by the instruction sequence. Same goes for the current scope
* level.
*/
	SaveFramePtrNLevel((unsigned int)((NclStackEntry*)tmp_fp - thestack),new_scope_level,(sb + (sb_off + new_scope_cur_off)));
	return(NhlNOERROR);
}

int _NclFinishFrame
#if	NhlNeedProto
(void)
#else
()
#endif
{
	return(SetNextFramePtrNLevel());
}

void *_NclLeaveFrame
#if	NhlNeedProto
(int caller_level)
#else
(caller_level)
	int caller_level;
#endif
{
	NclFrame * prev;
/*
	sb = &(fp->func_ret_value);
*/
	prev = (NclFrame*)(thestack + fp);
	fp = ((NclFrame*)(thestack + fp))->dynamic_link.u.offset;
	current_scope_level = caller_level;
	return((void*)prev);
}

NhlErrorTypes _NclPush
#if	NhlNeedProto
(NclStackEntry data)
#else
(data)
	NclStackEntry data;
#endif
{
	*(NclStackEntry*)(thestack + (sb+sb_off)) = data;
	sb_off++;
	if((sb+sb_off) >= cur_stacksize ) {
/*
		NhlPError(NhlFATAL,NhlEUNKNOWN,"Push: Stack overflow");
*/
		if(IncreaseStackSize() == NhlFATAL) {
			NhlPError(NhlFATAL,NhlEUNKNOWN,"Push: Stack overflow, could not increase stack size, memory allocation error");
			return(NhlFATAL);
		}
	}
	return(NhlNOERROR);
}

NclStackEntry _NclPop
#if	NhlNeedProto
(void)
#else
()
#endif
{
	NclStackEntry tmp;
	if(thestack + (sb +sb_off) <= thestack) {
		NhlPError(NhlFATAL,NhlEUNKNOWN,"Pop: Stack underflow");
		tmp.kind = NclStk_NOVAL;
		tmp.u.offset = 0;
		return(tmp);
	} else {
		sb_off--;
		tmp = (*(NclStackEntry*)(thestack +(sb + sb_off)));
		((NclStackEntry*)(thestack + (sb+sb_off)))->kind = NclStk_NOVAL;
		((NclStackEntry*)(thestack + (sb+sb_off)))->u.offset = 0;
		return(tmp);
	}
}
int _NclPutIntInstr
#if	NhlNeedProto
(int val,int line,char* file)
#else
(val,line,file)
        int val;
        int line;
        char *file;
#endif
{
/* 
* Sometimes the parser needs to know the offset of the instruction it put
* into the instruction list (i.e. function return addresses, loops and 
* conditionals. Therefore it is necessary to return the offset of the instruct
* being placed in the list.
*/
	int old_offset = (int)(mstk->the_rec->pc - mstk->the_rec->themachine);

/*
* Check for overflow
*/
	if(mstk->the_rec->pc >= &(mstk->the_rec->themachine[mstk->the_rec->current_machine_size -1])) {
/*
* Will take care of updating mstk->the_rec->pc
*/
		IncreaseMachineSize();
	}
	*((int*)mstk->the_rec->pc++) = val;
	*(mstk->the_rec->lc++) = line;
	*(mstk->the_rec->fn++) = file;
	mstk->the_rec->pcoffset = (int)(mstk->the_rec->pc - mstk->the_rec->themachine);

	return(old_offset);
}


int _NclPutInstr
#if	NhlNeedProto
(NclValue val,int line, char* file)
#else
(val,line,file)
	NclValue val;
	int line;
	char *file;
#endif
{
/* 
* Sometimes the parser needs to know the offset of the instruction it put
* into the instruction list (i.e. function return addresses, loops and 
* conditionals. Therefore it is necessary to return the offset of the instruct
* being placed in the list.
*/
	int old_offset = (int)(mstk->the_rec->pc - mstk->the_rec->themachine);

/*
* Check for overflow
*/
	if(mstk->the_rec->pc >= &(mstk->the_rec->themachine[mstk->the_rec->current_machine_size -1])) {
/*
* Will take care of updating mstk->the_rec->pc
*/
		IncreaseMachineSize();
	}
	*(mstk->the_rec->pc++) = val;
	*(mstk->the_rec->lc++) = line;
	*(mstk->the_rec->fn++) = file;
	mstk->the_rec->pcoffset = (int)(mstk->the_rec->pc - mstk->the_rec->themachine);

	return(old_offset);
}

int _NclGetCurrentOffset
#if	NhlNeedProto
(void)
#else
()
#endif
{
	return(mstk->the_rec->pcoffset);
}

int _NclPutInstrAt
#if	NhlNeedProto
(int offset,NclValue val,int line, char* file)
#else 
(offset, val, line, file)
	int offset;
	NclValue val;
	int line;
	char *file;
#endif
{
	NclValue *ptr;
	int *lptr;
	char **fptr;

	ptr = (NclValue*)(mstk->the_rec->themachine + offset);
	lptr = (int*)(mstk->the_rec->thelines + offset);
	fptr = (char**)(mstk->the_rec->thefiles+ offset);

	*ptr = val;
	*lptr = line;
	*fptr = file;

	return(offset);
}



void _NclPrintMachine
#if	NhlNeedProto
(int from, int to,FILE* fp)
#else
(from,to,fp)
	int from;
	int to;
	FILE *fp;
#endif
{
	NclValue *ptr;
	NclValue *eptr;
	int	*lptr;
	char **fptr;


	if(fp == NULL)
		return;
	if(from == -1){
		from = 0;
	}
	if(to == -1) {
		to = mstk->the_rec->pcoffset;
	}

	ptr = (NclValue*)(mstk->the_rec->themachine + from);
	eptr = (NclValue*)(mstk->the_rec->themachine + to);
	lptr = (int*)(mstk->the_rec->thelines+from);		
	fptr = (char**)(mstk->the_rec->thefiles+from);
	
	while(ptr != eptr) {
		if(*fptr != NULL) {
			fprintf(fp,"(%d,%d,%s)\t",(int)(ptr-mstk->the_rec->themachine),*lptr,*fptr);
		} else {
			fprintf(fp,"(%d,%d)\t",(int)(ptr-mstk->the_rec->themachine),*lptr);
		}
		switch(*ptr) {
			case NOOP :
			case STOPSEQ:
			case RETURN_OP :
			case NAMED_COORD_SUBSCRIPT_OP :
			case INT_SUBSCRIPT_OP :
			case NAMED_INT_SUBSCRIPT_OP :
			case COORD_SUBSCRIPT_OP :
			case SINGLE_INDEX_OP :
			case DEFAULT_RANGE_OP :
			case RANGE_INDEX_OP :
			case NEG_OP :
			case NOT_OP :
			case MOD_OP :
			case OR_OP :
			case AND_OP :
			case XOR_OP :
			case LTSEL_OP :
			case GTSEL_OP :
			case PLUS_OP :
			case MINUS_OP :
			case MUL_OP :
			case MAT_OP :
			case DIV_OP :
			case EXP_OP :
			case LE_OP :
			case GE_OP :
			case GT_OP :
			case LT_OP :
			case EQ_OP :
			case NE_OP :
			case BREAK_OP:
			case CONTINUE_OP:
			case ENDSTMNT_OP:
			case DUP_TOFS:
			case GET_OBJ_OP :
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				break;
			case JMP :
			case JMPFALSE :
			case SET_OBJ_OP :
			case PUSH_INT_LIT_OP :
			case PUSH_LOGICAL_LIT_OP :
			case PUSH_LOG_LIT_OP :
			case ARRAY_LIT_OP :
			case JMP_SCALAR_TRUE_OP:
			case JMP_SCALAR_FALSE_OP:
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%d\n",(int)*ptr);
				break;
			case PUSH_STRING_LIT_OP :
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%s\n",NrmQuarkToString(*ptr));
				break;
			case NEW_FRAME_OP:
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t");
				_NclPrintSymbol((NclSymbol*)*ptr,fp);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%d\n",(int)*ptr);
				break;
			case PROC_CALL_OP:
			case BPROC_CALL_OP:
			case FUNC_CALL_OP:
			case BFUNC_CALL_OP:
			case ISDEFINED_OP:
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t");
				_NclPrintSymbol((NclSymbol*)*ptr,fp);
				break;
			case INTRINSIC_FUNC_CALL:
			case INTRINSIC_PROC_CALL:
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t");
				_NclPrintSymbol((NclSymbol*)*ptr,fp);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%d\n",(int)*ptr);
				break;
			case DO_FROM_TO_STRIDE_OP :
			case DO_FROM_TO_OP :
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%d\n",*ptr);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t");
				_NclPrintSymbol((NclSymbol*)*ptr,fp);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t");
				_NclPrintSymbol((NclSymbol*)*ptr,fp);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t");
				_NclPrintSymbol((NclSymbol*)*ptr,fp);
				break;
			case PUSH_REAL_LIT_OP :
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%g\n",*(float*)ptr);
				break;
			case DO_WHILE_OP :
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%d\n",(int)*ptr);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%d\n",(int)*ptr);
				break;
			case FILEVAR_COORD_OP:
			case ASSIGN_FILEVAR_COORD_OP:
			case PARAM_FILEVAR_COORD_OP:
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t");
				_NclPrintSymbol((NclSymbol*)*ptr,fp);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%s\n",NrmQuarkToString(*ptr));
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%s\n",NrmQuarkToString(*ptr));
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%d\n",(int)*ptr);
				break;
			case VAR_COORD_OP:
			case VAR_READ_COORD_OP:
			case ASSIGN_VAR_COORD_OP:
			case PARAM_VAR_COORD_OP:
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t");
				_NclPrintSymbol((NclSymbol*)*ptr,fp);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%s\n",NrmQuarkToString(*ptr));
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%d\n",(int)*ptr);
				break;
			case FILEVARATT_OP:
			case ASSIGN_FILEVARATT_OP:
			case PARAM_FILEVARATT_OP:
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t");
				_NclPrintSymbol((NclSymbol*)*ptr,fp);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%s\n",NrmQuarkToString(*ptr));
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%s\n",NrmQuarkToString(*ptr));
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%d\n",(int)*ptr);
				break;
			case VARATT_OP:
			case ASSIGN_VARATT_OP:
			case PARAM_VARATT_OP:
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t");
				_NclPrintSymbol((NclSymbol*)*ptr,fp);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%s\n",NrmQuarkToString(*ptr));
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%d\n",(int)*ptr);
				break;
			case FILE_VAR_OP :
			case ASSIGN_FILE_VAR_OP :
			case PARAM_FILE_VAR_OP :
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t");
				_NclPrintSymbol((NclSymbol*)*ptr,fp);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t");
				fprintf(fp,"%s\n",NrmQuarkToString(*ptr),fp);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%d\n",*ptr);
				break;
			case VAR_READ_OP :
			case ASSIGN_VAR_OP :
			case PARAM_VAR_OP :
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t");
				_NclPrintSymbol((NclSymbol*)*ptr,fp);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%d\n",*ptr);
				break;
			case ASSIGN_VAR_VAR_OP :
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t");
				_NclPrintSymbol((NclSymbol*)*ptr,fp);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%d\n",*ptr);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t");
				_NclPrintSymbol((NclSymbol*)*ptr,fp);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t%d\n",*ptr);
				break;
			case FPDEF:
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t");
				_NclPrintSymbol((NclSymbol*)*ptr,fp);
				_NclPushMachine(((NclSymbol*)*ptr)->u.procfunc->mach_rec_ptr);
				_NclPrintMachine(-1,-1,fp);
				(void)_NclPopMachine();
				break;
			case CONVERT_TO_LOCAL:
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\t");
				_NclPrintSymbol((NclSymbol*)*ptr,fp);
				ptr++;lptr++;fptr++;
				fprintf(fp,"\tArg #:");
				fprintf(fp,"%d\n",(int)*ptr,fp);
				break;
			case VAR_DIM_OP:
			case ASSIGN_VAR_DIM_OP:
			case PARAM_VAR_DIM_OP:
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				ptr++;lptr++,fptr++;
				fprintf(fp,"\t");
				_NclPrintSymbol((NclSymbol*)*ptr,fp);
				break;
			case FILEVAR_DIM_OP :
			case ASSIGN_FILEVAR_DIM_OP :
			case PARAM_FILEVAR_DIM_OP :
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				ptr++;lptr++,fptr++;
				fprintf(fp,"\t");
				_NclPrintSymbol((NclSymbol*)*ptr,fp);
				ptr++;lptr++,fptr++;
				fprintf(fp,"\t");
				fprintf(fp,"%s\n",NrmQuarkToString(*ptr));
				break;
			case CREATE_OBJ_WP_OP:
			case CREATE_OBJ_OP:
				fprintf(fp,"%s\n",ops_strings[*ptr]);
				ptr++;lptr++,fptr++;
				fprintf(fp,"\tnres:%d",(*ptr));
				ptr++;lptr++,fptr++;
				fprintf(fp,"\t");
				_NclPrintSymbol((NclSymbol*)*ptr,fp);
				break;	
			default:
				break;
		}
		ptr++;lptr++;fptr++;
	}
	return;
}


extern void _NclAddObjToParamList
#if	NhlNeedProto
(struct _NclObjRec *obj, int arg_num)
#else
(obj,arg_num)
	struct _NclObjRec *obj;
	int arg_num;
#endif 
{
	NclMultiDValData tmp_md;
	NclFrame *tmp_fp;
/*
* guarenteed to have next frame pointer in the flist stack.
*/
	tmp_fp = (NclFrame*)(thestack + flist.next->fp);
	if(obj->obj.obj_type_mask & NCL_VAR_TYPE_MASK) {
		tmp_fp->parameter_map.u.the_list->the_elements[arg_num].p_type = VAR_P;
		tmp_fp->parameter_map.u.the_list->the_elements[arg_num].var_sym = ((NclVar)obj)->var.thesym;
		tmp_fp->parameter_map.u.the_list->the_elements[arg_num].var_ptr = (NclVar)obj;
		if((((NclVar)obj)->var.var_type == VARSUBSEL)
			||(((NclVar)obj)->var.var_type == COORDSUBSEL)
			||(((NclVar)obj)->var.var_type == FILEVARSUBSEL)) {
		
			tmp_md = (NclMultiDValData)_NclGetObj(((NclVar)obj)->var.thevalue_id);
			if(tmp_md->multidval.sel_rec != NULL) {
				tmp_fp->parameter_map.u.the_list->the_elements[arg_num].rec = (NclSelectionRecord*)NclMalloc((unsigned)sizeof(NclSelectionRecord));
            			memcpy((char*)tmp_fp->parameter_map.u.the_list->the_elements[arg_num].rec,(char*)tmp_md->multidval.sel_rec,sizeof(NclSelectionRecord));

			} else {
				tmp_fp->parameter_map.u.the_list->the_elements[arg_num].rec = NULL;
			}
		}
		tmp_fp->parameter_map.u.the_list->the_elements[arg_num].is_modified = 0;
	} else if(obj->obj.obj_type_mask & NCL_MD_MASK) {
		tmp_fp->parameter_map.u.the_list->the_elements[arg_num].p_type = VALUE_P;
		tmp_fp->parameter_map.u.the_list->the_elements[arg_num].var_sym = NULL;
		tmp_fp->parameter_map.u.the_list->the_elements[arg_num].var_ptr = NULL;
		tmp_fp->parameter_map.u.the_list->the_elements[arg_num].rec = NULL;
		tmp_fp->parameter_map.u.the_list->the_elements[arg_num].is_modified = 0;
	} else {
		tmp_fp->parameter_map.u.the_list->the_elements[arg_num].p_type = NONE_P;
		tmp_fp->parameter_map.u.the_list->the_elements[arg_num].var_sym = NULL;
		tmp_fp->parameter_map.u.the_list->the_elements[arg_num].var_ptr = NULL;
		tmp_fp->parameter_map.u.the_list->the_elements[arg_num].rec = NULL;
		tmp_fp->parameter_map.u.the_list->the_elements[arg_num].is_modified = 0;
/*
* handle files 
*/
	}
	
}

/*ARGSUSED*/
void _NclRemapIntrParameters
#if	NhlNeedProto
(int nargs,void *previous_fp,int from)
#else
(nargs,previous_fp,from)
	int nargs;
	void *previous_fp;
	int from;
#endif
{
	NclParamRecList *the_list = NULL;
	NclStackEntry data,*data_ptr = NULL;
	NclObjTypes param_rep_type,var_rep_type;
	NclVar anst_var = NULL, tmp_var = NULL,tmp_var1 = NULL;
	int i = 0,j = 0,k= 0,contains_vec = 0,m = 0;
	NclFrame* tmp_fp = (NclFrame*)previous_fp;
	int check_ret_status = 0;
	int value_ref_count = 0;
	int coord_ids[NCL_MAX_DIMENSIONS];
	NclAtt tmp_att = NULL;
	NclVar tmp_coord_var = NULL;
	NclMultiDValData tmp_md,tmp2_md;
	int tmp_elem = 0;
	char *step = NULL;
	NclSelectionRecord tmp_selection;
	void *tmp_ptr;
/*
* Some kind of check is need to assure top of stack and arguments are 
* aligned before the following loop starts up
*/
	switch(from) {
	case FUNC_CALL_OP:
	case INTRINSIC_FUNC_CALL:
	case BFUNC_CALL_OP:
		check_ret_status = 1;
		break;
	default:
		check_ret_status = 0;
		break;
	}
	the_list = tmp_fp->parameter_map.u.the_list;
if(the_list != NULL) {
	for (i = nargs -1 ; i > -1; i--) {
		data = _NclPop();
		if(the_list->the_elements[i].p_type == VAR_P){
			if(data.u.data_var != NULL) {
				if((the_list->the_elements[i].var_sym != NULL)&&(the_list->the_elements[i].rec != NULL)) {

/* 
* Key thing to think about here: if there is a subscript record or no variable 
* symbol then the value associated with a parameter variable is an array subsection
* or the result of an expression which can be converted to a temporary value if it has 
* been placed in the return value spot of the frame. Otherwise, if there is a sym and
* no selection record then the variable must be copied into a temporary variable.
*/ 



/*
* Variables regardless of subsectioning or type will share att id with parameter
* variable. So only coordinate variables, type and actual data array values 
* must bew remapped.
*/
					for(j = 0; j< the_list->the_elements[i].rec->n_entries; j ++) {
						if(the_list->the_elements[i].rec->selection[j].sel_type == Ncl_VECSUBSCR) {
							contains_vec = 1;
						}
					}
					if(the_list->the_elements[i].is_modified) {
						data_ptr = _NclRetrieveRec(the_list->the_elements[i].var_sym,WRITE_IT);
					} else {
						data_ptr = _NclRetrieveRec(the_list->the_elements[i].var_sym,DONT_CARE);
					}
					anst_var = data_ptr->u.data_var;
					if(data.u.data_var != NULL ) {
						var_rep_type = _NclGetVarRepValue(data.u.data_var);
						param_rep_type = _NclGetVarRepValue(anst_var);
						if((var_rep_type != param_rep_type)&&(!contains_vec)) {
	/*
	* Try reverse coercion which in many cases will
	* just fail.
	*/
	/*
							tmp_var = _NclCoerceVar(data.u.data_var,param_rep_type,NULL);
	*/
							tmp_var = NULL;
							if(tmp_var == NULL) { 
								NhlPError(NhlWARNING,NhlEUNKNOWN,"_NclRemapParameters: Argument (%d) to function or procedure was coerced before calling and can not be coerced back, arguments value remains unchanged",i);
							} else {
								_NclDestroyObj((NclObj)data.u.data_var);
								data.u.data_var = tmp_var;
							}
						} else {
							if((!contains_vec) &&(the_list->the_elements[i].is_modified)) {
								_NclAssignVarToVar(anst_var,the_list->the_elements[i].rec,data.u.data_var,NULL);
	
	/* 
	* When the selection record for the initial parameter has been subselected dimension sizes 
	* may be different between the parameter and the internal parameter
	*/
								if(the_list->the_elements[i].rec == NULL) {
									for(j = 0; j < anst_var->var.n_dims; j++) {
										anst_var->var.dim_info[j] = data.u.data_var->var.dim_info[j];
										if((anst_var->var.coord_vars[j] >0) &&
											(data.u.data_var->var.coord_vars[j]>0)) { 
											tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->var.coord_vars[j]);
											tmp_md = _NclVarValueRead(
												tmp_var1,
												NULL,
												NULL);
											if(tmp_md != NULL) {
												_NclWriteCoordVar(anst_var,tmp_md,NrmQuarkToString(anst_var->var.dim_info[j].dim_quark),NULL);
											}
										} else if(data.u.data_var->var.coord_vars[j]>0){
											if(data.u.data_var->obj.id != anst_var->obj.id) {
												tmp_var = (NclVar)_NclGetObj(data.u.data_var->var.coord_vars[j]);
												if(tmp_var != NULL) {
													tmp_md = _NclVarValueRead(
														tmp_var,
														NULL,
														NULL);
												}
												if((tmp_md != NULL)&&(data.u.data_var->var.dim_info[j].dim_quark!= -1)) {
													_NclWriteCoordVar(anst_var,tmp_md,NrmQuarkToString(data.u.data_var->var.dim_info[j].dim_quark),NULL);
												}
											} 
										}
									}
								} else {
									k = 0;
/*
* Should not be any vector subscripts here see contains_vec variable. Therefore
* 
*/
									for(j = 0; j < the_list->the_elements[i].rec->n_entries; j++) {
										tmp_elem = (int)(((float) (the_list->the_elements[i].rec->selection[j].u.sub.start - the_list->the_elements[i].rec->selection[j].u.sub.finish)) /(float)fabs(((float)the_list->the_elements[i].rec->selection[j].u.sub.stride))) + 1;
										if((tmp_elem != 1)&&(data.u.data_var->var.coord_vars[k]>0)){
											if((anst_var->var.coord_vars[j] >0)||(tmp_elem == anst_var->var.dim_info[j].dim_size)) {
												anst_var->var.dim_info[j].dim_quark = data.u.data_var->var.dim_info[k].dim_quark;
												tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->var.coord_vars[k]);
												tmp_md = _NclVarValueRead(tmp_var1, NULL, NULL);
												if(tmp_md != NULL) {
													tmp_selection.n_entries = 1;
													tmp_selection.selection[0] = the_list->the_elements[i].rec->selection[j];
													_NclWriteCoordVar(anst_var,tmp_md,NrmQuarkToString(anst_var->var.dim_info[j].dim_quark),&tmp_selection);
												}
											} else {
/*
* Case where missing values must be filled in
*/
												anst_var->var.dim_info[j].dim_quark = data.u.data_var->var.dim_info[k].dim_quark;
												tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->var.coord_vars[k]);
												tmp_md = _NclVarValueRead(tmp_var1, NULL, NULL);
												tmp_ptr =  (void*)NclMalloc((unsigned)tmp_md->multidval.type->type_class.size*anst_var->var.dim_info[j].dim_size);
												step = (char*)tmp_ptr;
												for(m = 0; m < tmp_elem; m++) {
													memcpy((void*)step,(void*)&tmp_md->multidval.type->type_class.default_mis,tmp_md->multidval.type->type_class.size);
													step = step + tmp_md->multidval.type->type_class.size;
												}
												tmp2_md = _NclCreateMultiDVal(
													NULL,
													NULL,
													Ncl_MultiDValData,
													0,
													tmp_ptr,
													&tmp_md->multidval.type->type_class.default_mis,
													1,
													&anst_var->var.dim_info[j].dim_size,
													TEMPORARY,
													NULL,
													tmp_md->multidval.type);
												tmp_selection.selection[0] = the_list->the_elements[i].rec->selection[j];
												_NclWriteSubSection((NclData)tmp2_md,&tmp_selection,(NclData)tmp_md);
											 	_NclWriteCoordVar(anst_var,tmp2_md,NrmQuarkToString(anst_var->var.dim_info[j].dim_quark),NULL);

										}
											k++;
										}
										}
								}
							}
						}
/* 
* 5/17/94 Is this really a problem to deal with here? shouldn't AssignVarToVar take care of copying these?
* ------->
* Need to deal with mapping coordinates, attributes and dim_info. THis is
* not trivial since remapping can cause coord arrays to expand to the
* size of target dimension and be partially filled in with missing values 
* <-------
*/
/*	
					if((the_list->the_elements[i].var_ptr != NULL)&&(anst_var->obj.id != the_list->the_elements[i].var_ptr->obj.id)) {
						_NclDestroyObj((NclObj)the_list->the_elements[i].var_ptr);
					}
*/

						value_ref_count = _NclGetObjRefCount(data.u.data_var->var.thevalue_id);

						if((check_ret_status)&&(tmp_fp->func_ret_value.kind == NclStk_VAR)&&(tmp_fp->func_ret_value.u.data_var->obj.id == data.u.data_var->obj.id)&&(value_ref_count == 1)) {
/* data better be a variable otherwise something is majorly hosed */
/* Here the object is available to be forced into a temporary variable */

								tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->obj.id);
								tmp_var = _NclVarNclCreate(
									NULL,
									tmp_var1->obj.class_ptr,
									tmp_var1->obj.obj_type,
									tmp_var1->obj.obj_type_mask,
									NULL,
									(NclMultiDValData)_NclGetObj(tmp_var1->var.thevalue_id),
									tmp_var1->var.dim_info,
									tmp_var1->var.att_id,
									tmp_var1->var.coord_vars,
									RETURNVAR,
									NULL,
									TEMPORARY
									);
								_NclDestroyObj((NclObj)tmp_var1);
								tmp_fp->func_ret_value.u.data_var = tmp_var;
								check_ret_status = 0;
						} else if((check_ret_status)&&(tmp_fp->func_ret_value.u.data_var->obj.id == data.u.data_var->obj.id)){
							tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->obj.id);
							if(tmp_var1->var.att_id != -1) 
								tmp_att = _NclCopyAtt((NclAtt)_NclGetObj(tmp_var1->var.att_id),NULL);
							for(j = 0; j< tmp_var1->var.n_dims; j++) {
								if(tmp_var1->var.coord_vars[j] != -1) {
									tmp_coord_var = _NclCopyVar((NclVar)_NclGetObj(tmp_var1->var.coord_vars[j]),NULL,NULL);
									coord_ids[j] = tmp_coord_var->obj.id;
								} else {
									coord_ids[j] = -1;
								}
							}
							tmp_var = _NclVarNclCreate(
								NULL,
								tmp_var1->obj.class_ptr,
								tmp_var1->obj.obj_type,
								tmp_var1->obj.obj_type_mask,
								NULL,
								_NclCopyVal((NclMultiDValData)_NclGetObj(tmp_var1->var.thevalue_id),NULL),
								tmp_var1->var.dim_info,
								(tmp_att == NULL ? -1:tmp_att->obj.id),
								coord_ids,
								RETURNVAR,
								NULL,
								TEMPORARY
								);
							_NclDestroyObj((NclObj)tmp_var1);
							tmp_fp->func_ret_value.u.data_var = tmp_var;
							check_ret_status = 0;
						} else {
							_NclDestroyObj((NclObj)data.u.data_var);
						}
					} else {
						if((the_list->the_elements[i].var_ptr != NULL)&&(anst_var->obj.id != the_list->the_elements[i].var_ptr->obj.id)) {
							_NclDestroyObj((NclObj)the_list->the_elements[i].var_ptr);
						}
					}
				} else if(the_list->the_elements[i].var_sym != NULL){
/*
* -----------> Not really sure about DONT_CARE <------------
*/
					if(the_list->the_elements[i].is_modified) {
						data_ptr = _NclRetrieveRec(the_list->the_elements[i].var_sym,WRITE_IT);
					} else {
						data_ptr = _NclRetrieveRec(the_list->the_elements[i].var_sym,DONT_CARE);
					}
					anst_var = data_ptr->u.data_var;
					if(the_list->the_elements[i].is_modified) {
						if((anst_var->var.att_id == -1)&&(data.u.data_var->var.att_id != -1)) {
							anst_var->var.att_id = data.u.data_var->var.att_id;
							_NclAddParent(_NclGetObj(anst_var->var.att_id),(NclObj)anst_var);
						} else if((anst_var->var.att_id != -1)&&(data.u.data_var->var.att_id == -1)){
	
							_NclDelParent(_NclGetObj(anst_var->var.att_id),(NclObj)anst_var);
							anst_var->var.att_id = -1;
						} else if(anst_var->var.att_id != data.u.data_var->var.att_id) {
							_NclDelParent(_NclGetObj(anst_var->var.att_id),(NclObj)anst_var);
							anst_var->var.att_id = data.u.data_var->var.att_id;
							_NclAddParent(_NclGetObj(anst_var->var.att_id),(NclObj)anst_var);
						}
						for(j = 0 ; j < data.u.data_var->var.n_dims; j++) {
							if((anst_var->var.coord_vars[j] != -1)&&(data.u.data_var->var.coord_vars[j] == -1)) {
								_NclDelParent(_NclGetObj(anst_var->var.coord_vars[j]),(NclObj)anst_var);
								anst_var->var.coord_vars[j] = -1;
							
						
							} else if((anst_var->var.coord_vars[j] == -1)&&(data.u.data_var->var.coord_vars[j] == -1)) {
								anst_var->var.coord_vars[j] = data.u.data_var->var.coord_vars[j];
								_NclAddParent(_NclGetObj(anst_var->var.coord_vars[j]),(NclObj)anst_var);
					
							} else if(anst_var->var.coord_vars[j] != data.u.data_var->var.coord_vars[j]) {
								_NclDelParent(_NclGetObj(anst_var->var.coord_vars[j]),(NclObj)anst_var);
								anst_var->var.coord_vars[j] = data.u.data_var->var.coord_vars[j];
								_NclAddParent(_NclGetObj(anst_var->var.coord_vars[j]),(NclObj)anst_var);
							} 
							anst_var->var.dim_info[j] = data.u.data_var->var.dim_info[j];
						}
	
/*
* AssignVarVar only assigns values not atts, coords or dimensions
*/
						_NclAssignVarToVar(anst_var,NULL,data.u.data_var,NULL);
					}
					value_ref_count = _NclGetObjRefCount(data.u.data_var->var.thevalue_id);

					if((check_ret_status)&&(tmp_fp->func_ret_value.kind == NclStk_VAR)&&(tmp_fp->func_ret_value.u.data_var->obj.id == data.u.data_var->obj.id)&&(value_ref_count == 1)) {
	/* data better be a variable otherwise something is majorly hosed */
	/* Here the object is available to be forced into a temporary variable */
							tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->obj.id);
							tmp_var = _NclVarNclCreate(
								NULL,
								tmp_var1->obj.class_ptr,
								tmp_var1->obj.obj_type,
								tmp_var1->obj.obj_type_mask,
								NULL,
								(NclMultiDValData)_NclGetObj(tmp_var1->var.thevalue_id),
								tmp_var1->var.dim_info,
								tmp_var1->var.att_id,
								tmp_var1->var.coord_vars,
								RETURNVAR,
								NULL,
								TEMPORARY
								);
							_NclDestroyObj((NclObj)tmp_var1);
							tmp_fp->func_ret_value.u.data_var = tmp_var;
							check_ret_status = 0;
							
					} else if((check_ret_status)&&(tmp_fp->func_ret_value.u.data_var->obj.id == data.u.data_var->obj.id)){
							tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->obj.id);
							if(tmp_var1->var.att_id != -1) 
								tmp_att = _NclCopyAtt((NclAtt)_NclGetObj(tmp_var1->var.att_id),NULL);
							for(j = 0; j< tmp_var1->var.n_dims; j++) {
								if(tmp_var1->var.coord_vars[j] != -1) {
									tmp_coord_var = _NclCopyVar((NclVar)_NclGetObj(tmp_var1->var.coord_vars[i]),NULL,NULL);
									coord_ids[j] = tmp_coord_var->obj.id;
								} else {
									coord_ids[j] = -1;
								}
							}
							tmp_var = _NclVarNclCreate(
								NULL,
								tmp_var1->obj.class_ptr,
								tmp_var1->obj.obj_type,
								tmp_var1->obj.obj_type_mask,
								NULL,
								_NclCopyVal((NclMultiDValData)_NclGetObj(tmp_var1->var.thevalue_id),NULL),
								tmp_var1->var.dim_info,
								(tmp_att == NULL ? -1:tmp_att->obj.id),
								coord_ids,
								RETURNVAR,
								NULL,
								TEMPORARY
								);
							_NclDestroyObj((NclObj)tmp_var1);
							tmp_fp->func_ret_value.u.data_var = tmp_var;
							check_ret_status = 0;
					} else {
/*
* Since convert to local doesn't create a new symbol a status
* check is required here
*/
						if(data.u.data_var->obj.status != PERMANENT) {
							_NclDestroyObj((NclObj)data.u.data_var);
						}
					}
				} else {
/*
* In this situation the variable is a return value which isn't attached to a symbol
*/
					value_ref_count = _NclGetObjRefCount(data.u.data_var->var.thevalue_id);

					if((check_ret_status)&&(tmp_fp->func_ret_value.kind == NclStk_VAR)&&(tmp_fp->func_ret_value.u.data_var->obj.id == data.u.data_var->obj.id)&&(value_ref_count == 1)) {
/* data better be a variable otherwise something is majorly hosed */
/* Here the object is available to be forced into a temporary variable */
							tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->obj.id);
							tmp_var = _NclVarNclCreate(
								NULL,
								tmp_var1->obj.class_ptr,
								tmp_var1->obj.obj_type,
								tmp_var1->obj.obj_type_mask,
								NULL,
								(NclMultiDValData)_NclGetObj(tmp_var1->var.thevalue_id),
								tmp_var1->var.dim_info,
								tmp_var1->var.att_id,
								tmp_var1->var.coord_vars,
								RETURNVAR,
								NULL,
								TEMPORARY
								);
							_NclDestroyObj((NclObj)tmp_var1);
							tmp_fp->func_ret_value.u.data_var = tmp_var;
							check_ret_status = 0;
							
					} else if((check_ret_status)&&(tmp_fp->func_ret_value.u.data_var->obj.id == data.u.data_var->obj.id)){
							tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->obj.id);
							if(tmp_var1->var.att_id != -1) 
								tmp_att = _NclCopyAtt((NclAtt)_NclGetObj(tmp_var1->var.att_id),NULL);
							for(j = 0; j< tmp_var1->var.n_dims; j++) {
								if(tmp_var1->var.coord_vars[j] != -1) {
									tmp_coord_var = _NclCopyVar((NclVar)_NclGetObj(tmp_var1->var.coord_vars[j]),NULL,NULL);
									coord_ids[j] = tmp_coord_var->obj.id;
								} else {
									coord_ids[j] = -1;
								}
							}
							tmp_var = _NclVarNclCreate(
								NULL,
								tmp_var1->obj.class_ptr,
								tmp_var1->obj.obj_type,
								tmp_var1->obj.obj_type_mask,
								NULL,
								_NclCopyVal((NclMultiDValData)_NclGetObj(tmp_var1->var.thevalue_id),NULL),
								tmp_var1->var.dim_info,
								(tmp_att == NULL ? -1:tmp_att->obj.id),
								coord_ids,
								RETURNVAR,
								NULL,
								TEMPORARY
								);
							_NclDestroyObj((NclObj)tmp_var1);
							tmp_fp->func_ret_value.u.data_var = tmp_var;
							check_ret_status = 0;
					} else {
						if(data.u.data_var->obj.status != PERMANENT) {
							_NclDestroyObj((NclObj)data.u.data_var);
						}
					}
				}
			} 
		} else if((the_list->the_elements[i].p_type == VALUE_P)&&(data.u.data_obj != NULL)) {
			if(data.kind == NclStk_VAR) { 
/*
* Need to turn data part into a temporary variable and still destroy variable
*   >Does not currently do this for now it does the same thing as if it were
*    a variable parameter<
*/
				value_ref_count = _NclGetObjRefCount(data.u.data_var->var.thevalue_id);

				if((check_ret_status)&&(tmp_fp->func_ret_value.kind == NclStk_VAR)&&(tmp_fp->func_ret_value.u.data_var->obj.id == data.u.data_var->obj.id)&&(value_ref_count == 1)) {
/* data better be a variable otherwise something is majorly hosed */
/* Here the object is available to be forced into a temporary variable */
						tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->obj.id);
						tmp_var = _NclVarNclCreate(
							NULL,
							tmp_var1->obj.class_ptr,
							tmp_var1->obj.obj_type,
							tmp_var1->obj.obj_type_mask,
							NULL,
							(NclMultiDValData)_NclGetObj(tmp_var1->var.thevalue_id),
							tmp_var1->var.dim_info,
							tmp_var1->var.att_id,
							tmp_var1->var.coord_vars,
							RETURNVAR,
							NULL,
							TEMPORARY
							);
						_NclDestroyObj((NclObj)tmp_var1);
						tmp_fp->func_ret_value.u.data_var = tmp_var;
						check_ret_status = 0;
						
				} else if((check_ret_status)&&(tmp_fp->func_ret_value.u.data_var->obj.id == data.u.data_var->obj.id)){
						tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->obj.id);
						if(tmp_var1->var.att_id != -1) 
							tmp_att = _NclCopyAtt((NclAtt)_NclGetObj(tmp_var1->var.att_id),NULL);
						for(j = 0; i< tmp_var1->var.n_dims; i++) {
							if(tmp_var1->var.coord_vars[j] != -1) {
								tmp_coord_var = _NclCopyVar((NclVar)_NclGetObj(tmp_var1->var.coord_vars[i]),NULL,NULL);
								coord_ids[j] = tmp_coord_var->obj.id;
							} else {
								coord_ids[j] = -1;
							}
						}
						tmp_var = _NclVarNclCreate(
							NULL,
							tmp_var1->obj.class_ptr,
							tmp_var1->obj.obj_type,
							tmp_var1->obj.obj_type_mask,
							NULL,
							_NclCopyVal((NclMultiDValData)_NclGetObj(tmp_var1->var.thevalue_id),NULL),
							tmp_var1->var.dim_info,
							(tmp_att == NULL ? -1:tmp_att->obj.id),
							coord_ids,
							RETURNVAR,
							NULL,
							TEMPORARY
							);
						_NclDestroyObj((NclObj)tmp_var1);
						tmp_fp->func_ret_value.u.data_var = tmp_var;
						check_ret_status = 0;
				} else {
					_NclDestroyObj((NclObj)data.u.data_var);
				}
			} else {
				if(data.u.data_obj->obj.status != PERMANENT) {
					_NclDestroyObj((NclObj)data.u.data_obj);
				}
			}
		}
	}
} 
}
void _NclRemapParameters
#if	NhlNeedProto
(int nargs,int cur_off,void *previous_fp,int from)
#else
(nargs,cur_off,previous_fp,from)
	int nargs;
	int cur_off;
	void *previous_fp;
	int from;
#endif
{
	NclParamRecList *the_list = NULL;
	NclStackEntry data,*data_ptr = NULL;
	NclObjTypes param_rep_type,var_rep_type;
	NclVar anst_var = NULL, tmp_var = NULL,tmp_var1 = NULL;
	int i = 0,j = 0,k= 0,contains_vec = 0,m = 0;
	NclFrame* tmp_fp = (NclFrame*)previous_fp;
	int check_ret_status = 0;
	int value_ref_count = 0;
	int coord_ids[NCL_MAX_DIMENSIONS];
	NclAtt tmp_att = NULL;
	NclVar tmp_coord_var = NULL;
	NclMultiDValData tmp_md,tmp2_md;
	int tmp_elem = 0;
	char *step = NULL;
	NclSelectionRecord tmp_selection;
	void *tmp_ptr;
/*
* Some kind of check is need to assure top of stack and arguments are 
* aligned before the following loop starts up
*/
	switch(from) {
	case FUNC_CALL_OP:
	case INTRINSIC_FUNC_CALL:
	case BFUNC_CALL_OP:
		check_ret_status = 1;
		break;
	default:
		check_ret_status = 0;
		break;
	}
/*
* Have to remove any local variable space, should
* be no problem to just destroy them
*/
	if(check_ret_status) {
		for(i = 0; i< cur_off-nargs; i++) {
			data = _NclPop();
			switch(data.kind) {
			case NclStk_VAL:
/*
* Not sure this can ever happen but it can't hurt
*/
				if(!((data.kind == NclStk_VAL)&&(tmp_fp->func_ret_value.kind == NclStk_VAL)&&
					(tmp_fp->func_ret_value.u.data_obj->obj.id == data.u.data_obj->obj.id))) {
					_NclDestroyObj((NclObj)data.u.data_obj);
				}
				break;
			case NclStk_VAR:
				if((data.kind == NclStk_VAR)&&(tmp_fp->func_ret_value.kind == NclStk_VAR)&&
					(tmp_fp->func_ret_value.u.data_var->obj.id == data.u.data_var->obj.id)) {
					tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->obj.id);
					tmp_var = _NclVarNclCreate(
						NULL,
						tmp_var1->obj.class_ptr,
						tmp_var1->obj.obj_type,
						tmp_var1->obj.obj_type_mask,
						NULL,
						(NclMultiDValData)_NclGetObj(tmp_var1->var.thevalue_id),
						tmp_var1->var.dim_info,
						tmp_var1->var.att_id,
						tmp_var1->var.coord_vars,
						RETURNVAR,
						NULL,
						TEMPORARY
						);
					_NclDestroyObj((NclObj)tmp_var1);
					tmp_fp->func_ret_value.u.data_var = tmp_var;
					check_ret_status = 0;
					
				} else {
					_NclDestroyObj((NclObj)data.u.data_var);
				}
			default:
				break;
			}
		}
	} else {
		for(i = 0; i< cur_off-nargs; i++) {
			data = _NclPop();
			switch(data.kind) {
			case NclStk_VAL:
				_NclDestroyObj((NclObj)data.u.data_obj);
				break;
			case NclStk_VAR:
				_NclDestroyObj((NclObj)data.u.data_var);
			default:
				break;
			}
		}
	}

	the_list = tmp_fp->parameter_map.u.the_list;
if(the_list != NULL) {
	for (i = nargs -1 ; i > -1; i--) {
		data = _NclPop();
		if(the_list->the_elements[i].p_type == VAR_P) {
			if((the_list->the_elements[i].var_sym != NULL)&&(the_list->the_elements[i].rec != NULL)) {

/* 
* Key thing to think about here: if there is a subscript record or no variable 
* symbol then the value associated with a parameter variable is an array subsection
* or the result of an expression which can be converted to a temporary value if it has 
* been placed in the return value spot of the frame. Otherwise, if there is a sym and
* no selection record then the variable must be copied into a temporary variable.
*/ 



/*
* Variables regardless of subsectioning or type will share att id with parameter
* variable. So only coordinate variables, type and actual data array values 
* must bew remapped.
*/
				for(j = 0; j< the_list->the_elements[i].rec->n_entries; j ++) {
					if(the_list->the_elements[i].rec->selection[j].sel_type == Ncl_VECSUBSCR) {
						contains_vec = 1;
					}
				}
				if(the_list->the_elements[i].is_modified) {
					data_ptr = _NclRetrieveRec(the_list->the_elements[i].var_sym,WRITE_IT);
				} else {
					data_ptr = _NclRetrieveRec(the_list->the_elements[i].var_sym,DONT_CARE);
				}
				anst_var = data_ptr->u.data_var;
				if(data.u.data_var != NULL ) {
					var_rep_type = _NclGetVarRepValue(data.u.data_var);
					param_rep_type = _NclGetVarRepValue(anst_var);
					if((var_rep_type != param_rep_type)&&(!contains_vec)) {
/*
* Try reverse coercion which in many cases will
* just fail.
*/
/*
						tmp_var = _NclCoerceVar(data.u.data_var,param_rep_type,NULL);
*/
						tmp_var = NULL;
						if(tmp_var == NULL) { 
							NhlPError(NhlWARNING,NhlEUNKNOWN,"_NclRemapParameters: Argument (%d) to function or procedure was coerced before calling and can not be coerced back, arguments value remains unchanged",i);
						} else {
							_NclDestroyObj((NclObj)data.u.data_var);
							data.u.data_var = tmp_var;
						}
					} else {
						if((!contains_vec) &&(the_list->the_elements[i].is_modified)) {
							_NclAssignVarToVar(anst_var,the_list->the_elements[i].rec,data.u.data_var,NULL);

/* 
* When the selection record for the initial parameter has been subselected dimension sizes 
* may be different between the parameter and the internal parameter
*/
							if(the_list->the_elements[i].rec == NULL) {
								for(j = 0; j < anst_var->var.n_dims; j++) {
									anst_var->var.dim_info[j] = data.u.data_var->var.dim_info[j];
									if((anst_var->var.coord_vars[j] >0) &&
										(data.u.data_var->var.coord_vars[j]>0)) { 
										tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->var.coord_vars[j]);
										tmp_md = _NclVarValueRead(
											tmp_var1,
											NULL,
											NULL);
										if(tmp_md != NULL) {
											_NclWriteCoordVar(anst_var,tmp_md,NrmQuarkToString(anst_var->var.dim_info[j].dim_quark),NULL);
										}
									} else if(data.u.data_var->var.coord_vars[j]>0){
										if(data.u.data_var->obj.id != anst_var->obj.id) {
											tmp_var = (NclVar)_NclGetObj(data.u.data_var->var.coord_vars[j]);
											if(tmp_var != NULL) {
												tmp_md = _NclVarValueRead(
													tmp_var,
													NULL,
													NULL);
											}
											if((tmp_md != NULL)&&(data.u.data_var->var.dim_info[j].dim_quark!= -1)) {
												_NclWriteCoordVar(anst_var,tmp_md,NrmQuarkToString(data.u.data_var->var.dim_info[j].dim_quark),NULL);
											}
										} 
									}
								}
							} else {
								k = 0;
/*
* Should not be any vector subscripts here see contains_vec variable. Therefore
* 
*/
								for(j = 0; j < the_list->the_elements[i].rec->n_entries; j++) {
									tmp_elem = (int)(((float) (the_list->the_elements[i].rec->selection[j].u.sub.start - the_list->the_elements[i].rec->selection[j].u.sub.finish)) /(float)fabs(((float)the_list->the_elements[i].rec->selection[j].u.sub.stride))) + 1;
									if((tmp_elem != 1)&&(data.u.data_var->var.coord_vars[k]>0)){
										if((anst_var->var.coord_vars[j] >0)||(tmp_elem == anst_var->var.dim_info[j].dim_size)) {
											anst_var->var.dim_info[j].dim_quark = data.u.data_var->var.dim_info[k].dim_quark;
											tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->var.coord_vars[k]);
											tmp_md = _NclVarValueRead(tmp_var1, NULL, NULL);
											if(tmp_md != NULL) {
												tmp_selection.n_entries = 1;
												tmp_selection.selection[0] = the_list->the_elements[i].rec->selection[j];
												_NclWriteCoordVar(anst_var,tmp_md,NrmQuarkToString(anst_var->var.dim_info[j].dim_quark),&tmp_selection);
											}
										} else {
/*
* Case where missing values must be filled in
*/
											anst_var->var.dim_info[j].dim_quark = data.u.data_var->var.dim_info[k].dim_quark;
											tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->var.coord_vars[k]);
											tmp_md = _NclVarValueRead(tmp_var1, NULL, NULL);
											tmp_ptr =  (void*)NclMalloc((unsigned)tmp_md->multidval.type->type_class.size*anst_var->var.dim_info[j].dim_size);
											step = (char*)tmp_ptr;
											for(m = 0; m < anst_var->var.dim_info[j].dim_size; m++) {
												memcpy((void*)step,(void*)&tmp_md->multidval.type->type_class.default_mis,tmp_md->multidval.type->type_class.size);
												step = step + tmp_md->multidval.type->type_class.size;
											}
											tmp2_md = _NclCreateMultiDVal(
												NULL,
												NULL,
												Ncl_MultiDValData,
												0,
												tmp_ptr,
												&tmp_md->multidval.type->type_class.default_mis,
												1,
												&anst_var->var.dim_info[j].dim_size,
												TEMPORARY,
												NULL,
												tmp_md->multidval.type);
											tmp_selection.selection[0] = the_list->the_elements[i].rec->selection[j];
											_NclWriteSubSection((NclData)tmp2_md,&tmp_selection,(NclData)tmp_md);
											 _NclWriteCoordVar(anst_var,tmp2_md,NrmQuarkToString(anst_var->var.dim_info[j].dim_quark),NULL);

										}
										k++;
									}
								}
							}
						}
					}
/* 
* 5/17/94 Is this really a problem to deal with here? shouldn't AssignVarToVar take care of copying these?
* ------->
* Need to deal with mapping coordinates, attributes and dim_info. THis is
* not trivial since remapping can cause coord arrays to expand to the
* size of target dimension and be partially filled in with missing values 
* <-------
*/
				
					if((the_list->the_elements[i].var_ptr != NULL)&&(anst_var->obj.id != the_list->the_elements[i].var_ptr->obj.id)) {
						_NclDestroyObj((NclObj)the_list->the_elements[i].var_ptr);
					}

					value_ref_count = _NclGetObjRefCount(data.u.data_var->var.thevalue_id);

					if((check_ret_status)&&(tmp_fp->func_ret_value.kind == NclStk_VAR)&&(tmp_fp->func_ret_value.u.data_var->obj.id == data.u.data_var->obj.id)&&(value_ref_count == 1)) {
/* data better be a variable otherwise something is majorly hosed */
/* Here the object is available to be forced into a temporary variable */

							tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->obj.id);
							tmp_var = _NclVarNclCreate(
								NULL,
								tmp_var1->obj.class_ptr,
								tmp_var1->obj.obj_type,
								tmp_var1->obj.obj_type_mask,
								NULL,
								(NclMultiDValData)_NclGetObj(tmp_var1->var.thevalue_id),
								tmp_var1->var.dim_info,
								tmp_var1->var.att_id,
								tmp_var1->var.coord_vars,
								RETURNVAR,
								NULL,
								TEMPORARY
								);
							_NclDestroyObj((NclObj)tmp_var1);
							tmp_fp->func_ret_value.u.data_var = tmp_var;
							check_ret_status = 0;
					} else if((check_ret_status)&&(tmp_fp->func_ret_value.u.data_var->obj.id == data.u.data_var->obj.id)){
						tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->obj.id);
						if(tmp_var1->var.att_id != -1) 
							tmp_att = _NclCopyAtt((NclAtt)_NclGetObj(tmp_var1->var.att_id),NULL);
						for(j = 0; j< tmp_var1->var.n_dims; j++) {
							if(tmp_var1->var.coord_vars[j] != -1) {
								tmp_coord_var = _NclCopyVar((NclVar)_NclGetObj(tmp_var1->var.coord_vars[j]),NULL,NULL);
								coord_ids[j] = tmp_coord_var->obj.id;
							} else {
								coord_ids[j] = -1;
							}
						}
						tmp_var = _NclVarNclCreate(
							NULL,
							tmp_var1->obj.class_ptr,
							tmp_var1->obj.obj_type,
							tmp_var1->obj.obj_type_mask,
							NULL,
							_NclCopyVal((NclMultiDValData)_NclGetObj(tmp_var1->var.thevalue_id),NULL),
							tmp_var1->var.dim_info,
							(tmp_att == NULL ? -1:tmp_att->obj.id),
							coord_ids,
							RETURNVAR,
							NULL,
							TEMPORARY
							);
						_NclDestroyObj((NclObj)tmp_var1);
						tmp_fp->func_ret_value.u.data_var = tmp_var;
						check_ret_status = 0;
					} else {
						_NclDestroyObj((NclObj)data.u.data_var);
					}
				} else {
					if((the_list->the_elements[i].var_ptr != NULL)&&(anst_var->obj.id != the_list->the_elements[i].var_ptr->obj.id)) {
						_NclDestroyObj((NclObj)the_list->the_elements[i].var_ptr);
					}
				}
			} else if(the_list->the_elements[i].var_sym != NULL){
/*
* -----------> Not really sure about DONT_CARE <------------
*/
				if(the_list->the_elements[i].is_modified) {
					data_ptr = _NclRetrieveRec(the_list->the_elements[i].var_sym,WRITE_IT);
				} else {
					data_ptr = _NclRetrieveRec(the_list->the_elements[i].var_sym,DONT_CARE);
				}
				anst_var = data_ptr->u.data_var;
				if(the_list->the_elements[i].is_modified) {
					if((anst_var->var.att_id == -1)&&(data.u.data_var->var.att_id != -1)) {
						anst_var->var.att_id = data.u.data_var->var.att_id;
						_NclAddParent(_NclGetObj(anst_var->var.att_id),(NclObj)anst_var);
					} else if((anst_var->var.att_id != -1)&&(data.u.data_var->var.att_id == -1)){

						_NclDelParent(_NclGetObj(anst_var->var.att_id),(NclObj)anst_var);
						anst_var->var.att_id = -1;
					} else if(anst_var->var.att_id != data.u.data_var->var.att_id) {
						_NclDelParent(_NclGetObj(anst_var->var.att_id),(NclObj)anst_var);
						anst_var->var.att_id = data.u.data_var->var.att_id;
						_NclAddParent(_NclGetObj(anst_var->var.att_id),(NclObj)anst_var);
					}
					for(j = 0 ; j < data.u.data_var->var.n_dims; j++) {
						if((anst_var->var.coord_vars[j] != -1)&&(data.u.data_var->var.coord_vars[j] == -1)) {
							_NclDelParent(_NclGetObj(anst_var->var.coord_vars[j]),(NclObj)anst_var);
							anst_var->var.coord_vars[j] = -1;
						
						
						} else if((anst_var->var.coord_vars[j] == -1)&&(data.u.data_var->var.coord_vars[j] == -1)) {
							anst_var->var.coord_vars[j] = data.u.data_var->var.coord_vars[j];
							_NclAddParent(_NclGetObj(anst_var->var.coord_vars[j]),(NclObj)anst_var);
				
						} else if(anst_var->var.coord_vars[j] != data.u.data_var->var.coord_vars[j]) {
							_NclDelParent(_NclGetObj(anst_var->var.coord_vars[j]),(NclObj)anst_var);
							anst_var->var.coord_vars[j] = data.u.data_var->var.coord_vars[j];
							_NclAddParent(_NclGetObj(anst_var->var.coord_vars[j]),(NclObj)anst_var);
						} 
						anst_var->var.dim_info[j] = data.u.data_var->var.dim_info[j];
					}

/*
* AssignVarVar only assigns values not atts, coords or dimensions
*/
					_NclAssignVarToVar(anst_var,NULL,data.u.data_var,NULL);
				}
				value_ref_count = _NclGetObjRefCount(data.u.data_var->var.thevalue_id);

				if((check_ret_status)&&(tmp_fp->func_ret_value.kind == NclStk_VAR)&&(tmp_fp->func_ret_value.u.data_var->obj.id == data.u.data_var->obj.id)&&(value_ref_count == 1)) {
/* data better be a variable otherwise something is majorly hosed */
/* Here the object is available to be forced into a temporary variable */
						tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->obj.id);
						tmp_var = _NclVarNclCreate(
							NULL,
							tmp_var1->obj.class_ptr,
							tmp_var1->obj.obj_type,
							tmp_var1->obj.obj_type_mask,
							NULL,
							(NclMultiDValData)_NclGetObj(tmp_var1->var.thevalue_id),
							tmp_var1->var.dim_info,
							tmp_var1->var.att_id,
							tmp_var1->var.coord_vars,
							RETURNVAR,
							NULL,
							TEMPORARY
							);
						_NclDestroyObj((NclObj)tmp_var1);
						tmp_fp->func_ret_value.u.data_var = tmp_var;
						check_ret_status = 0;
						
				} else if((check_ret_status)&&(tmp_fp->func_ret_value.u.data_var->obj.id == data.u.data_var->obj.id)){
						tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->obj.id);
						if(tmp_var1->var.att_id != -1) 
							tmp_att = _NclCopyAtt((NclAtt)_NclGetObj(tmp_var1->var.att_id),NULL);
						for(j = 0; j< tmp_var1->var.n_dims; j++) {
							if(tmp_var1->var.coord_vars[j] != -1) {
								tmp_coord_var = _NclCopyVar((NclVar)_NclGetObj(tmp_var1->var.coord_vars[i]),NULL,NULL);
								coord_ids[j] = tmp_coord_var->obj.id;
							} else {
								coord_ids[j] = -1;
							}
						}
						tmp_var = _NclVarNclCreate(
							NULL,
							tmp_var1->obj.class_ptr,
							tmp_var1->obj.obj_type,
							tmp_var1->obj.obj_type_mask,
							NULL,
							_NclCopyVal((NclMultiDValData)_NclGetObj(tmp_var1->var.thevalue_id),NULL),
							tmp_var1->var.dim_info,
							(tmp_att == NULL ? -1:tmp_att->obj.id),
							coord_ids,
							RETURNVAR,
							NULL,
							TEMPORARY
							);
						_NclDestroyObj((NclObj)tmp_var1);
						tmp_fp->func_ret_value.u.data_var = tmp_var;
						check_ret_status = 0;
				} else {
					_NclDestroyObj((NclObj)data.u.data_var);
				}
			} else {
/*
* In this situation the variable is a return value which isn't attached to a symbol
*/
				value_ref_count = _NclGetObjRefCount(data.u.data_var->var.thevalue_id);

				if((check_ret_status)&&(tmp_fp->func_ret_value.kind == NclStk_VAR)&&(tmp_fp->func_ret_value.u.data_var->obj.id == data.u.data_var->obj.id)&&(value_ref_count == 1)) {
/* data better be a variable otherwise something is majorly hosed */
/* Here the object is available to be forced into a temporary variable */
						tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->obj.id);
						tmp_var = _NclVarNclCreate(
							NULL,
							tmp_var1->obj.class_ptr,
							tmp_var1->obj.obj_type,
							tmp_var1->obj.obj_type_mask,
							NULL,
							(NclMultiDValData)_NclGetObj(tmp_var1->var.thevalue_id),
							tmp_var1->var.dim_info,
							tmp_var1->var.att_id,
							tmp_var1->var.coord_vars,
							RETURNVAR,
							NULL,
							TEMPORARY
							);
						_NclDestroyObj((NclObj)tmp_var1);
						tmp_fp->func_ret_value.u.data_var = tmp_var;
						check_ret_status = 0;
						
				} else if((check_ret_status)&&(tmp_fp->func_ret_value.u.data_var->obj.id == data.u.data_var->obj.id)){
						tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->obj.id);
						if(tmp_var1->var.att_id != -1) 
							tmp_att = _NclCopyAtt((NclAtt)_NclGetObj(tmp_var1->var.att_id),NULL);
						for(j = 0; j< tmp_var1->var.n_dims; j++) {
							if(tmp_var1->var.coord_vars[j] != -1) {
								tmp_coord_var = _NclCopyVar((NclVar)_NclGetObj(tmp_var1->var.coord_vars[j]),NULL,NULL);
								coord_ids[j] = tmp_coord_var->obj.id;
							} else {
								coord_ids[j] = -1;
							}
						}
						tmp_var = _NclVarNclCreate(
							NULL,
							tmp_var1->obj.class_ptr,
							tmp_var1->obj.obj_type,
							tmp_var1->obj.obj_type_mask,
							NULL,
							_NclCopyVal((NclMultiDValData)_NclGetObj(tmp_var1->var.thevalue_id),NULL),
							tmp_var1->var.dim_info,
							(tmp_att == NULL ? -1:tmp_att->obj.id),
							coord_ids,
							RETURNVAR,
							NULL,
							TEMPORARY
							);
						_NclDestroyObj((NclObj)tmp_var1);
						tmp_fp->func_ret_value.u.data_var = tmp_var;
						check_ret_status = 0;
				} else {
					_NclDestroyObj((NclObj)data.u.data_var);
				}
			}
		} else if(the_list->the_elements[i].p_type == VALUE_P) {
			if(data.kind == NclStk_VAR) { 
/*
* Need to turn data part into a temporary variable and still destroy variable
*   >Does not currently do this for now it does the same thing as if it were
*    a variable parameter<
*/
				value_ref_count = _NclGetObjRefCount(data.u.data_var->var.thevalue_id);

				if((check_ret_status)&&(tmp_fp->func_ret_value.kind == NclStk_VAR)&&(tmp_fp->func_ret_value.u.data_var->obj.id == data.u.data_var->obj.id)&&(value_ref_count == 1)) {
/* data better be a variable otherwise something is majorly hosed */
/* Here the object is available to be forced into a temporary variable */
						tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->obj.id);
						tmp_var = _NclVarNclCreate(
							NULL,
							tmp_var1->obj.class_ptr,
							tmp_var1->obj.obj_type,
							tmp_var1->obj.obj_type_mask,
							NULL,
							(NclMultiDValData)_NclGetObj(tmp_var1->var.thevalue_id),
							tmp_var1->var.dim_info,
							tmp_var1->var.att_id,
							tmp_var1->var.coord_vars,
							RETURNVAR,
							NULL,
							TEMPORARY
							);
						_NclDestroyObj((NclObj)tmp_var1);
						tmp_fp->func_ret_value.u.data_var = tmp_var;
						check_ret_status = 0;
						
				} else if((check_ret_status)&&(tmp_fp->func_ret_value.u.data_var->obj.id == data.u.data_var->obj.id)){
						tmp_var1 = (NclVar)_NclGetObj(data.u.data_var->obj.id);
						if(tmp_var1->var.att_id != -1) 
							tmp_att = _NclCopyAtt((NclAtt)_NclGetObj(tmp_var1->var.att_id),NULL);
						for(j = 0; i< tmp_var1->var.n_dims; i++) {
							if(tmp_var1->var.coord_vars[j] != -1) {
								tmp_coord_var = _NclCopyVar((NclVar)_NclGetObj(tmp_var1->var.coord_vars[i]),NULL,NULL);
								coord_ids[j] = tmp_coord_var->obj.id;
							} else {
								coord_ids[j] = -1;
							}
						}
						tmp_var = _NclVarNclCreate(
							NULL,
							tmp_var1->obj.class_ptr,
							tmp_var1->obj.obj_type,
							tmp_var1->obj.obj_type_mask,
							NULL,
							_NclCopyVal((NclMultiDValData)_NclGetObj(tmp_var1->var.thevalue_id),NULL),
							tmp_var1->var.dim_info,
							(tmp_att == NULL ? -1:tmp_att->obj.id),
							coord_ids,
							RETURNVAR,
							NULL,
							TEMPORARY
							);
						_NclDestroyObj((NclObj)tmp_var1);
						tmp_fp->func_ret_value.u.data_var = tmp_var;
						check_ret_status = 0;
				} else {
					_NclDestroyObj((NclObj)data.u.data_var);
				}
			} else {
				_NclDestroyObj((NclObj)data.u.data_var);
			}
		}
	}
}
}

void _NclDumpStack
#if	NhlNeedProto
(FILE *fp,int off)
#else 
(fp,off) 
	FILE *fp;
	int off;
#endif
{
	NclStackEntry *tmp_ptr = (NclStackEntry*)(thestack + (sb + sb_off - off));
	int i;

	if(fp == NULL) 
		fp = stdout;
	fprintf(fp,"\n");
	tmp_ptr = tmp_ptr - 1;
	while(tmp_ptr >= thestack) {
		fprintf(fp,"%d)\t",(int)(tmp_ptr - thestack));
		switch(tmp_ptr->kind) {
		case	NclStk_NOVAL:
			fprintf(fp,"NclStk_NOVAL\n");
			break;
		case	NclStk_OFFSET:
			fprintf(fp,"NclStk_OFFSET\t",tmp_ptr->u.offset);
			break;
		case 	NclStk_VAL:
			fprintf(fp,"NclStk_VAL\t");
			if(tmp_ptr->u.data_obj->obj.obj_type_mask & (Ncl_MultiDValHLUObjData | Ncl_MultiDValnclfileData))
				fprintf(fp,"%s\t",NrmQuarkToString(_NclObjTypeToName(tmp_ptr->u.data_obj->obj.obj_type)));
			else 
				fprintf(fp,"%s\t",NrmQuarkToString(_NclObjTypeToName(tmp_ptr->u.data_obj->multidval.type->type_class.type)));
				
			for(i = 0; i< tmp_ptr->u.data_obj->multidval.n_dims; i++) {
				fprintf(fp,"[%d]",tmp_ptr->u.data_obj->multidval.dim_sizes[i]);
				if(i !=  tmp_ptr->u.data_obj->multidval.n_dims - 1) {
					fprintf(fp," x ");
				}
        		}
			fprintf(fp,"\n");
			break;
		case 	NclStk_VAR:
			fprintf(fp,"NclStk_VAR\t");
			fprintf(fp,"%s ",NrmQuarkToString(tmp_ptr->u.data_var->var.var_quark));
			switch(tmp_ptr->u.data_var->var.var_type) {
			case VARSUBSEL:
				fprintf(fp,"(subsection)\t");
				break;
			case COORD:
				fprintf(fp,"(coordinate)\t");
				break;
			case COORDSUBSEL:
				fprintf(fp,"(coordinate subsection)\t");
				break;
			case PARAM:
				fprintf(fp,"(parameter)\t");
				break;
			case RETURNVAR:
				fprintf(fp,"(return)\t");
				break;
			case HLUOBJ:
			case NORMAL:
			default:
				fprintf(fp,"\t");
				break;
			}
			for(i = 0; i< tmp_ptr->u.data_var->var.n_dims; i++) {
				fprintf(fp,"[");
				if((tmp_ptr->u.data_var->var.dim_info[i].dim_quark != -1)) {
					fprintf(fp,"%s | ",NrmQuarkToString(tmp_ptr->u.data_var->var.dim_info[i].dim_quark));
				}
				fprintf(fp,"%d]",tmp_ptr->u.data_var->var.dim_info[i].dim_size);
				if(i !=  tmp_ptr->u.data_var->var.n_dims - 1) {
					fprintf(fp," x ");
				}
			}

			fprintf(fp,"\n");
			break;
		case 	NclStk_SUBREC:
			fprintf(fp,"NclStk_SUBREC\n");
			break;
		case	NclStk_PARAMLIST:
			fprintf(fp,"NclStk_PARAMLIST\t");
			fprintf(fp,"\n");
			break;
		case	NclStk_RANGEREC:
			fprintf(fp,"NclStk_RANGEREC\n");
			break;
		case	NclStk_VECREC:
			fprintf(fp,"NclStk_VECREC\n");
			break;
		case	NclStk_FILE:	
			fprintf(fp,"NclStk_FILE\n");
			break;
		case 	NclStk_GRAPHIC:
			fprintf(fp,"NclStk_GRAPHIC\n");
			break;
        	case	NclStk_STATIC_LINK:
			fprintf(fp,"NclStk_STATIC_LINK\t%d\n",(int)(tmp_ptr->u.offset ));
			break;
        	case	NclStk_DYNAMIC_LINK:
			fprintf(fp,"NclStk_DYNAMIC_LINK\t%d\n",(int)(tmp_ptr->u.offset));
			break;
        	case	NclStk_RET_OFFSET:
			fprintf(fp,"NclStk_RET_OFFSET\t%d\n",(int)(tmp_ptr->u.offset));
			break;
		case 	NclStk_RETURNVAL:
			fprintf(fp,"------------FRAME(%s)------------\n",((NclSymbol*)tmp_ptr->u.data_obj)->name);
			break;
		default:
			fprintf(fp,"\n");
			break;
		}
		tmp_ptr--;
	}
	fprintf(fp,"\n");
	
}

NhlErrorTypes _NclPlaceReturn
#if	NhlNeedProto
(struct _NclStackEntry data)
#else
(data)
	struct _NclStackEntry data;
#endif
{
	((NclFrame*)(thestack + fp))->func_ret_value = data;
	return(NhlNOERROR);
}

void _NclCleanUpStack
#if	NhlNeedProto
(int n)
#else
(n)
	int n;
#endif
{
	int i;
	NclStackEntry data;
	for( i = 0 ; i < n; i++) {
		data = _NclPop();
		switch(data.kind) {
		case NclStk_VAL:
			if((data.u.data_obj != NULL)&&(data.u.data_obj->obj.status != PERMANENT)) {
				_NclDestroyObj((NclObj)data.u.data_obj);
			}
			break;
		case NclStk_VAR:
			if((data.u.data_var != NULL) &&( data.u.data_var->obj.status != PERMANENT)) {
				_NclDestroyObj((NclObj)data.u.data_var);
			} 
			break;
		case NclStk_SUBREC:
				_NclFreeSubRec(data.u.sub_rec);
			break;
		default:
			break;
		}
	}
}

int _NclPutRealInstr
#if	NhlNeedProto
(float val, int line, char* file)
#else
(val,line,file)
float val;
int line;
char *file;
#endif
{
/* 
* Sometimes the parser needs to know the offset of the instruction it put
* into the instruction list (i.e. function return addresses, loops and 
* conditionals. Therefore it is necessary to return the offset of the instruct
* being placed in the list.
*/
	int old_offset = (int)(mstk->the_rec->pc - mstk->the_rec->themachine);

/*
* Check for overflow
*/
	if(mstk->the_rec->pc >= &(mstk->the_rec->themachine[mstk->the_rec->current_machine_size -1])) {
/*
* Will take care of updating mstk->the_rec->pc
*/
		IncreaseMachineSize();
	}
	*((float*)mstk->the_rec->pc++) = val;
	*(mstk->the_rec->lc++) = line;
	*(mstk->the_rec->fn++) = file;
	mstk->the_rec->pcoffset = (int)(mstk->the_rec->pc - mstk->the_rec->themachine);

	return(old_offset);
}
#ifdef __cplusplus
}
#endif
