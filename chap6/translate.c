struct Tr_access {
	Tr_level level;
	F_access access;
};

Tr_accessList Tr_AccessList (Tr_access head, Tr_accessList tail) {
}

Tr_level Tr_outermost (void) {
}

Tr_level Tr_newLevel (Tr_level parent, Temp_label name, U_boolList formals) {
}

Tr_accessList Tr_formals (Tr_level level) {
}

Tr_access Tr_allocLocal (F_frame f, Tr_level level, bool escapee) {
}



