#include "util.h"
#include "slp.h"
#include "prog1.h"
#include <stdio.h>


int maxargs(A_stm stm);
int deepExpList(A_expList exps);
int deepExp(A_exp exp);
int deepStm(A_stm stm);

int main(void) {
	printf("%d \n", maxargs(prog()));

	return 0;
}

int maxargs(A_stm stm) {
	return deepStm(stm);
}

int deepExpList(A_expList exps) {
	switch(exps->kind) {
		case A_lastExpList :	//one argument
			deepExp(exps->u.last);
			return 1;
		case A_pairExpList :	//two arguments
			deepExp(exps->u.pair.head);
			return deepExpList(exps->u.pair.tail) + 1;
	}
}

int deepExp(A_exp exp) {
	int a, b, aa, bb;
	switch(exp->kind) {
		case A_idExp :
		case A_numExp :
			return 0;
		case A_opExp :
			printf("%d\n", exp->u.op.oper);
			a = deepExp(exp->u.op.left);
			b = deepExp(exp->u.op.right);
			return (a > b) ? a : b;
		case A_eseqExp :
			aa = deepStm(exp->u.eseq.stm);
			bb = deepExp(exp->u.eseq.exp);
			return (aa > bb) ? aa: bb;
	}
}

int deepStm(A_stm stm) {
	int a, b;
	switch(stm->kind) {
		case A_compoundStm :
			a = deepStm(stm->u.compound.stm1);
			b = deepStm(stm->u.compound.stm2);
			return (a > b) ? a : b;
		case A_assignStm :
			return deepExp(stm->u.assign.exp);
		case A_printStm :
			return deepExpList(stm->u.print.exps);
	}
}
