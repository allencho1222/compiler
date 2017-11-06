#include "semant.h"

struct expty expTy (Tr_exp exp, Ty_ty ty) {
	struct expty e;
	e.exp = exp;
	e.ty = ty;
	
	return e;
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
			return expTy (NULL, TY_Record (NULL));
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
			
	}
}


void transDec (S_table venv, S_table tenv, A_dec d) {
	switch (d->kind) {
		case A_typeDec :
			S_enter (tenv, d->u.type->head.name, transTy (d->u.type->head->ty));
			break;
		case A_functionDec :
			A_fundec function = d->u.function->head;
			Ty_ty resultTy = S_look (tenv, function->result);
			Ty_tyList formalTy = makeFormalTyList (tenv, function->params);

			S_enter (venv, f->name, E_FunEntry (formalTy, resultTy));
			S_beginScope (venv);
			{
				A_fieldList l;
				Ty_tyList t;
				for (l = function->params, t = formalTy; l; l = l->tail, t = t->tail)
					S_enter (venv, l->head->name, E_VarEntry (t->head));
			}
			transExp (venv, tenv, d->u.function->body);
			break;
		case A_varDec :
			struct expty e = transExp (venv, tenv, d->u.var.init);
			S_enter (venv, d->u.var.var, E_VarEntry (e.ty));
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

			return nameTy;
			break;
		default :
			EM_error (ty->pos, "unknown declaration");
}



Ty_ty makeFieldTyList (S_table tenv, A_ty ty) {
	struct Ty_fieldList tyfieldList = NULL;
	struct A_fieldList fieldList = ty->u.record;
	struct A_field field;

	for (field = fieldList->head; field != NULL; i field = fieldList->head->tail) {
		Ty_ty typeTy = S_look (tenv, field->typ);
		if (typeTy == NULL) {
			EM_error (field->pos, "%s is not defined", S_name (field->typ));
		} else {
			struct Ty_field tyfield = Ty_field (field->name, typeTy);
			tyfieldList = Ty_FieldList (tyfield, NULL);
			tyfieldList = tyfieldList->tail;
		}
	}
	return tyfieldList;
}
				
