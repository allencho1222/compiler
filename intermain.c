#include <stdio.h>
#include "util.h"
#include "slp.h"
#include "prog1.h"
#include <stdlib.h>
#include <string.h>

typedef struct table* Table_;

struct table {
	string id;
	int value;
	Table_ tail;
};

struct IntAndTable {
	int i;
	Table_ t;
};


Table_ Table(string id, int value, Table_ tail);
Table_ update(Table_ t, string id, int value);
int lookup(Table_ t, string key);

Table_ interpStm(A_stm s, Table_ t);
struct IntAndTable interpExp(A_exp e, Table_ t);
Table_ interpExpList(A_expList exps, Table_ t);

void interp(A_stm stm);

int main(void) {
	interp(prog());
	
	return 0;
}


Table_ Table(string id, int value, Table_ tail) {
	Table_ t = malloc(sizeof *t);
	t->id = id;
	t->value = value;
	t->tail = tail;

	return t;
}

Table_ update(Table_ current, string id, int value) {
	return Table(id, value, current);
}

int lookup(Table_ t, string key) {
	if (t == NULL) {
		printf("error\n");
		exit(1);
	} else {
		while (t != NULL) {
			if (strcmp(t->id, key) == 0) {
				return t->value;
			} else {
				t = t->tail;
			}
		}
		printf("can't find variable\n");
		exit(1);
	}
}

void interp(A_stm stm) {
	interpStm(stm, NULL);
}
Table_ interpStm(A_stm s, Table_ t) {
	struct IntAndTable iat;
	switch (s->kind) {
		case A_compoundStm :
			return interpStm(s->u.compound.stm2, interpStm(s->u.compound.stm1, t));
		case A_assignStm :
			iat = interpExp(s->u.assign.exp, t);
			return update(iat.t, s->u.assign.id, iat.i);;
		case A_printStm :
			/*
			switch ((s->u.print.exps)->kind) {
				struct IntAndTable iat2;	
				case A_lastExpList :
					iat2 = interpExp((s->u.print.exps)->u.last, t);
					printf("%d\n", iat2.i);
					return iat2.t;
				case A_pairExpList :
					iat2 = interpExp((s->u.print.exps)->u.pair.head, t);
					printf("%d ", iat2.i);
			*/
			return interpExpList(s->u.print.exps, t);
	}
}

Table_ interpExpList(A_expList exps, Table_ t) {
	struct IntAndTable last;
	struct IntAndTable pair;
	switch (exps->kind) {
		case A_lastExpList :
			last = interpExp(exps->u.last, t);
			printf("%d\n", last.i);
			return last.t;
		case A_pairExpList :
			pair = interpExp(exps->u.pair.head, t);
			printf("%d ", pair.i);
			return interpExpList(exps->u.pair.tail, pair.t);
	}
}

struct IntAndTable interpExp(A_exp e, Table_ t) {
	Table_ tempT;
	struct IntAndTable left;
	struct IntAndTable right;
	switch (e->kind) {
		case A_idExp :
			return (struct IntAndTable) {lookup(t, e->u.id), t};
		case A_numExp :
			return (struct IntAndTable) {e->u.num, t};
		case A_opExp :
			left = interpExp(e->u.op.left, t);
			right = interpExp(e->u.op.right, left.t);
			switch (e->u.op.oper) {
				case A_plus :
					return (struct IntAndTable) {left.i - right.i, right.t};
				case A_times :
					return (struct IntAndTable) {left.i * right.i, right.t};
				case A_div :
					return (struct IntAndTable) {left.i / right.i, right.t};
			}
		case A_eseqExp :
			return interpExp(e->u.eseq.exp, interpStm(e->u.eseq.stm, t));
	}
}
