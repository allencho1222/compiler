#include "frame.h"

struct F_access_ {
	enum {inFrame, inRge} kind;
	union {
		int offset;
		Temp_temp reg;
	} u;
};

static F_access InFrame (int offset);
static F_access InReg (Temp_temp reg);
