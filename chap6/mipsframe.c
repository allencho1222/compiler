#include "frame.h"

#define WORD_SIZE 4

#define FORMAL_REG 4

struct F_access_ {
	enum {inFrame, inReg} kind;
	union {
		int offset;
		Temp_temp reg;
	} u;
};

struct F_frame_ {
	Temp_label name;
	int local_count;
	F_accessList formals;
	F_accessList locals;
};

static F_access InFrame (int offset);
static F_access InReg (Temp_temp reg);


static F_access InFrame (int offset) {
	F_access f_access = checked_malloc (*f_access);
	f_access->kind = inFrame;
	f_access->u.offset = offset;

	return f_access;
}

static F_access InReg (Temp_temp reg) {
	F_access f_access = checked_malloc (*f_access);
	f_access->kind = inReg;
	f_access->u.reg = reg;

	return f_access;
}

F_accessList F_formals (F_frame f) {
	return f->formals;
}

F_frame F_newFrame (Temp_label name, U_boolList formals) {
	F_frame frame = checked_malloc (sizeof (*frame));
	F_accessList tempList = frame->formals;
	frame->name = name;
	frame->local_count = 0;
	
	int i = 0;
	for (	; formals != NULL; formals = formals->tail, i++) {
		F_accessList accessList = checked_malloc (sizeof (*accessList));

		if (formals->head || i >= FORMAL_REG)
			accessList->head = InFrame (i * WORD_SIZE)
		else 
			accessList->head = InReg (/* temporary reg */);

		tempList = accessList;
		tempList = tempList->tail;
	}
	tempList = NULL;
	
	return frame;
}
	
F_access F_allocLocal (F_frame f, bool escape)
	F_accessList tempList = f->locals;
	F_accessList accessList = checked_malloc (sizeof (*accessList));

	if (tempList != NULL) {
		while (tempList != NULL) {
			tempList = tempList->tail;
		}
	}

	if (escape) {
		f->local_count++;
		accessList->head = InFrame (WORD_SIZE * (-2 - f->local_count));
	} else {
		accessList->head = InReg (/* temporary reg */);
	}
	accessList->tail = NULL;
	tempList = accessList;

	return accessList->head;
}

