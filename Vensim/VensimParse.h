#ifndef _XMUTIL_VENSIM_VENSIMPARSE_H
#define _XMUTIL_VENSIM_VENSIMPARSE_H
#include <string>
#include <boost/iostreams/device/mapped_file.hpp>
#include <iostream>
#include "../Symbol/Parse.h"
#include "../Symbol/Symbol.h"
#include "../Symbol/Units.h"
#include "../Function/Function.h"
#include "../Symbol/Equation.h"
#include "VensimLex.h"

class VensimParseSyntaxError
{
public :
   std::string str ;
} ;

class VensimParse
{
public:
   VensimParse(SymbolNameSpace *sns);
   ~VensimParse(void);
   void ReadyFunctions();
   bool ProcessFile(const std::string &filename) ;
   inline int yylex(void) { return mVensimLex.yylex() ; }
   int yyerror(const char *str) ;
   Equation *AddEq(LeftHandSide *lhs,Expression *ex,ExpressionList *exl,int tok) ;
   Equation *AddTable(LeftHandSide* lhs, Expression *ex, ExpressionTable* table);
   inline SymbolNameSpace *GetSymbolNameSpace(void) { return pSymbolNameSpace ; }
   Variable *InsertVariable(const std::string &name) ;
   Units *InsertUnits(const std::string &name) ;
   UnitExpression *InsertUnitExpression(Units *u) ;
   void AddFullEq(Equation *eq,UnitExpression *un) ;
   LeftHandSide *AddExceptInterp(ExpressionVariable *var,SymbolListList *except,int interpmode) ;
   SymbolList *SymList(SymbolList *in,Variable *add,bool bang,Variable *end) ;
   SymbolList *MapSymList(SymbolList* in, Variable* range,SymbolList *list);
   UnitExpression *UnitsDiv(UnitExpression *num, UnitExpression *denom);
   UnitExpression *UnitsMult(UnitExpression *f,UnitExpression *s) ;
   UnitExpression *UnitsRange(UnitExpression *e,double minval,double maxval,double increment) ;
   SymbolListList *ChainSublist(SymbolListList *sll,SymbolList *nsl) ;
   ExpressionList *ChainExpressionList(ExpressionList *el,Expression *e) ;
   Expression *NumExpression(double num) ;
   ExpressionVariable *VarExpression(Variable *var,SymbolList *subs) ;
   ExpressionSymbolList *SymlistExpression(SymbolList *sym,SymbolList *map) ;
   Expression *OperatorExpression(int oper,Expression *exp1,Expression *exp2) ;
   Expression *FunctionExpression(Function *func,ExpressionList *eargs) ;
   Expression *LookupExpression(ExpressionVariable *var,Expression *exp) ;
   ExpressionTable *TablePairs(ExpressionTable *table,double x,double y) ;
   ExpressionTable *TableRange(ExpressionTable *table,double x1,double y1,double x2,double y2) ;
   void MacroStart();
   void MacroExpression(Variable *macro, ExpressionList *margs);
   void MacroEnd();

private :
   std::string sFilename ;
   bool FindNextEq(void) ;
   boost::iostreams::mapped_file_source mfSource ;
   VensimLex mVensimLex ;
   VensimParseSyntaxError mSyntaxError ;
   SymbolNameSpace *pSymbolNameSpace ;
   SymbolNameSpace* pMainSymbolNameSpace;
   Variable *pActiveVar ;
   bool mInMacro;
   std::vector<MacroFunction*> mMacroFunctions;
};

extern VensimParse *VPObject ;

#endif