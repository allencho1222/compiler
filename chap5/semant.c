#include "semant.h"

struct expty expTy (Tr_exp exp, Ty_ty ty) {
	struct expty e;
	e.exp = exp;
	e.ty = ty;
	
	return e;
}

Ty_ty actual_ty (Ty_ty ty) {
	if (ty.kind == Ty_name) {
		return actual_ty (ty->u.name.ty);
	} else {
		return ty;
	}
}



struct expty transVar (S_table venv, S_table tenv, A_var v) {
	switch (v->kind) {
		case A_simpleVar :
			E_enventry x = S_look (venv, v->u.simple);
			if (x && x->kind == E_varEntry)
				return expTy(NULL, actual_ty (x->u.var.ty));
			else {
				EM_error (v->pos, "undefined variable %s", S_name (v->u.simple));
				return expTy (NULL, Ty_Int ());
			}
			break;
		case A_fieldVar :
			struct expty e = transVar (venv, tenv, v->u.field.var);
			if (e.ty->kind != Ty_record) {
				EM_error (v->pos, "It's not a record type");
				return expTy (NULL, Ty_Record (NULL));
			} else {
				Ty_fieldList rec;
				for (rec = e.ty->u.record; rec!= NULL; rec = rec->tail) {
					Ty_field head = rec->head;	
					if (head->name == v->u.field.sym) {
						return expTy (NULL, actual_ty (head->ty));
					}
				}
				EM_error (v->pos, "no corresponding field in record : %s", S_name (v->u.field.sym));
			}
			return expTy (NULL, Ty_Record (NULL));
			break;
		case A_subscriptVar :
			struct expty e = transVar (venv, tenv, v->u.subscript.var);
			if (e.ty->kind != Ty_array) {
				EM_error (v->pos, "it's not an arreay type");
				return expTy (NULL, Ty_Array (NULL));
			} else {
				struct expty e = transExp (venv, tenv, v->u.subscript.exp);
				if (e.ty->kind != Ty_int) {
					EM_error (v->pos, "array index has to be an integer");
					return expTy (NULL, Ty_Array (NULL));
				} else {
					return expTy (NULL, actual_ty (e.ty->u.array));
				}
			}
			break;
		default :
			EM_error (v->pos, "unknown type error");
	}
}

struct expty transExp (S_table venv, S_table tenv, A_exp a) {
	swtich (a->kind) {
		case A_opExp :
			A_oper op = a->u.op.oper;
			struct expty left = transExp (venv, tenv, a->u.op.left);
			struct expty right = transExp (venv, tenv, a->u.op.right);

			if (op == A_plusOp || op == A_minusOp ||
				op == A_timesOp || op == A_divideOp ||
				op == A_ltOp || op == A_leOp || op == gtOp || op == geOp) {
				if (left.ty->kind != Ty_int)
					EM_error (a->u.op.left->pos, "integer required");
				if (right.ty->kind != Ty_int)
					EM_error (a->u.op.right->pos, "integer required");
				return expTy (NULL, Ty_Int ());
			} else {
				if (left.ty->kind == Ty_int && right.ty->kind == Ty_int)
					return expTy (NULL, Ty_Int ());
				else if (left.ty->kind == Ty_record && right.ty->kind == Ty_record)
					return expTy (NULL, Ty_Int ());
				else if (left.ty->kind == Ty_array && right.ty->kind == Ty_array)
					return expTy (NULL, Ty_Int ());
				else
					EM_error (a->u.op.left->pos, "two operands must be same type");
			}
			break;
		case A_letExp :
			struct expty exp;
			A_decList d;
			S_beginScope (venv);
			S_beginScope (tenv);

			for (d = a->u.let.decs; d != NULL; d = d->tail) {
				transDec (venv, tenv, d->head);
			exp = transExp (venv, tenv, a->u.let.body); 
			S_endScope (tenv);
			S_endScope (venv);
			
			return exp;
			break;
		case A_varExp :
			return transVar (venv, tenv, a);
			break;
		case A_nilExp :
			return expTy (NULL, Ty_Nil ());
		case A_intExp :
			return expTy (NULL, Ty_Int ())
		case A_stringExp :
			return expTy (NULL, Ty_String ());
		case A_callExp :
			E_enventry e = S_look (venv, a->u.call.func);
			if (e != NULL && e->kind == E_funEntry) {
				checkArgs (e->u.fun.formals, a->u.call.args);
				A_expList explist;
			} else {
				EM_error (a->pos, "function type does not match");
			}
			break;
		case A_recordExp :
			E_enventry e = S_look (tenv, a->u.record.typ);
			if (e != NULL && e->kind = E_varEntry) {
				if (e->u.var.ty->kind != Ty_record) {
					EM_error ("%s is not a record type", S_name (a->u.record.typ));
				} else {
					A_efieldList efieldlist = a->u.record.fields;
					Ty_fieldList fieldlist = e->u.var.ty->u.record;
					if (checkRecFields (efieldlist, fieldlist)) {
						return expTy (NULL, Ty_Record (fieldlist));
					
				
					
				
	}
}


void transDec (S_table venv, S_table tenv, A_dec d) {
	switch (d->kind) {
		case A_typeDec :
			A_nametyList nametyList;
			A_namety namety;
			for (nametyList = d->u.type; nametyList != NULL; nametyList = nametyList->tail) {
				A_namety namety = nametyList->head;
				S_enter (tenv, namety->name, transTy (namety->ty));
			}
			break;
		case A_functionDec :
			A_fundecList fundecList;
			A_fundec fundec;

			for (fundecList = d->u.function; fundecList != NULL; fundecList = fundecList->tail) {
				struct expty exp;
				fundec = fundecList->head;
				Ty_ty resultTy = (fundec->result != NULL) ? S_look (tenv, fundec->result) : Ty_void ();

				if (resultTy == NULL) {
					EM_error (fundec->pos, "result type is not declared before");
				}
				Ty_tyList formalTy = makeFormalTyList (tenv, fundec->params);

				S_enter (venv, f->name, E_FunEntry (formalTy, resultTy));
				S_beginScope (venv);
				{
					A_fieldList l;
					Ty_tyList t;
					for (l = fundec->params, t = formalTy; l != NULL; l = l->tail, t = t->tail)
						S_enter (venv, l->head->name, E_VarEntry (t->head));
				}
				exp = transExp (venv, tenv, d->u.function->body);

				if (!checkRetType (exp.ty, resultTy)) 
					EM_error (fundec->pos, "return type does not match");

				S_endScope (venv);
			}
			break;
		case A_varDec :
			struct expty e = transExp (venv, tenv, d->u.var.init);
			// varTy will be NULl if it the type is not declared before or not declared by programmer
			Ty_ty varTy = (d->u.var.typ != NULL) ? S_look (tenv, d->u.var.typ) : NULL;
			if (d->u.var.typ != NULL) {
				Ty_ty varTy = S_look (tenv, d->u.var.typ);
				if (varTy != NULL && varTy->kind == Ty_record) {
					S_enter (venv, d->u.var.var, E_varEntry (e.ty));
				} else {
					EM_error (d->pos, "NULL cannot be applied to non-record variable");
				}
			} else {
				EM_error (d->pos, "NULL cannot be applied to non-record variable");
			}
			break;
		default :
	}
}

Ty_ty transTy (S_table tenv, A_ty ty) {
	switch (ty->kind) {
		case  A_arrayTy :
			Ty_ty arrayTy = S_look (tenv, ty->u.array);
			if (arrayTy == NULL)
				EM_error (ty->pos, "%s is not defined", S_name (ty->u.array));

			return Ty_Array (arrayTy);
			break;
		case A_recordTy :
			Ty_fieldList fieldList = makeFieldTyList (tenv, ty);
			return Ty_Record (fieldList);
			break;
		case A_nameTy :
			Ty_ty nameTy = S_look (tenv, ty->u.name);
			if (nameTy == NULL)
				EM_error (ty->pos, "%s is not defined", S_name (ty->u.name));

			return Ty_Name (ty->u.name, nameTy);
			break;
		default :
			EM_error (ty->pos, "unknown declaration");
}



Ty_ty makeFieldTyList (S_table tenv, A_ty ty) {
	Ty_fieldList tyfieldList = NULL;
	A_fieldList fieldList = ty->u.record;
	A_field field;

	for (fieldList = ty->u.record; fieldList != NULL; fieldList = fieldList->tail) {
		A_field field = fieldList->head;
		Ty_ty typeTy = S_look (tenv, field->typ);
		if (typeTy == NULL) {
			EM_error (field->pos, "%s is not defined", S_name (field->typ));
		} else {
			Ty_field tyfield = Ty_field (field->name, typeTy);
			if (tyfieldList == NULL)
				tyfieldList = Ty_FieldList (tyfield, NULL);
			else {
				tyfieldList->tail = Ty_FieldList (tyfield, NULL);
				tyfieldList = tyfieldList->tail; 
			}
		}
	}
	return tyfieldList;
}


bool checkRetType (Ty_ty decTy, Ty_ty bodyTy) {
	if (decTy->kind != bodyTy->kind) {
		return false;
	} else {
		Ty_ty actual_decTy = actual_ty (decTy);
		Ty_ty actual_bodyTy = actual_ty (bodyTy);

		return actual_decTy == actual_bodyTy;
	}
}

bool checkArgs (Ty_tyList formals, A_expList explist) {
	Ty_tyList tylist;;
	A_expList explist;

	for (tylist = formals, exp = explist; tylist != NULL && explist != NULL;
		tylist = formals->tail, explist = explist->tail) {
		Ty_ty funcTy = actual_ty (tylist->head);
		A_exp exp = explist->head;
		struct expty exp_ty = transVar (exp);
		Ty_ty argsTy = actual_ty (exp_ty.ty);

		if (funcTy->kind != argsTy->kind) {
			EM_error (exp->pos, "function type does not match");
		} else {
			if (funcTy->kind == Ty_record) {
				Ty_field field = funcTy->u.record
}
				
				
				
				
			

