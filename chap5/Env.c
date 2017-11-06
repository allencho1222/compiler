#include "Env.h"

S_table E_base_tenv (void) {
	S_table type_table = S_empty ();

	S_enter (type_table, S_Symbol ("int"), Ty_Int ());
	S_enter (type_table, S_Symbol ("string", Ty_String ());

	return type_table;
}

S_table E_base_venv (void) {
	S_table var_table = S_empty ();

	/* formal :string, return x */
	S_enter (var_table, S_Symbol ("print"), E_FunEntry (Ty_TyList (Ty_String (), NULL), Ty_Void ()));
	/* formal : x, return : x */
	S_enter (var_table, S_Symbol ("flush"), E_FunEntry (Ty_TyList (Ty_Void (), NULL), Ty_Void ()));
	/* formal : x, return : string */
	S_enter (var_table, S_Symbol ("getchar"), E_FunEntry (Ty_TyList (Ty_Void (), NULL), Ty_String ()));
	/* formal : string, return : int */
	S_enter (var_table, S_Symbol ("ord"), E_FunEntry (Ty_TyList (Ty_String (), NULL), Ty_Int ()));
	/* formal : int, return string */
	S_enter (var_table, S_Symbol ("chr"), E_FunEntry (Ty_TyList (Ty_Int (), NULL), Ty_String ()));
	/* formal : string, return : int */
	S_enter (var_table, S_Symbol ("size"), E_FunEntry (Ty_TyList (Ty_String (), NULL), Ty_Int ()));

	S_enter (var_table, S_Symbol ("substring"), E_FunEntry (Ty_TyList (
												Ty_String (), Ty_TyList (
												Ty_Int (), Ty_list (
												Ty_Int (), NULL))), Ty_String ()));
	S_enter (var_table, S_Symbol ("concat"), E_FunEntry (Ty_TyList (
											 Ty_String (), Ty_TyList (
											 Ty_String (), NULL)), Ty_String ()));
	S_enter (var_table, S_Symbol ("not"), E_FunEntry (Ty_List (
										  Ty_Int (), NULL), Ty_Int ());

	S_enter (var_table, S_Symbol ("exit"), E_FunEntry (Ty_List (Ty_Int (), NULL), Ty_Void ()));

	return var_table;
}

E_enventry E_VarEntry (Ty_ty ty) {
	E_enventry enventry = checked_malloc (sizeof (*enventry));
	
	enventry.kind = E_varEntry;
	enventry.u.var.ty = ty;
	
	return enventry;
}

E_enventry E_FunEntry (Ty_tyList formals, Ty_ty result) {
	E_enventry enventry = checked_malloc (sizeof (*enventry));

	enventry.kind = E_funEntry;
	enventry.u.fun.formals = formals;
	enventry.u.fun.result = result;

	return enventry;
}
