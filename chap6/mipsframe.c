#include "frame.h"

struct F_access_ {
	enum {inFrame, inReg} kind;
	union {
		int offset;
		Temp_temp reg;
	} u;
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

F_access F_allocLocal (F_frame f, bool escape)
