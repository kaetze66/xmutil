#include "Function.h"
#include "../Symbol/ExpressionList.h"
#include "../XMUtil.h"

// model symbol - most variables including subscript ranges but not
// subscript elements which are just 
Function::Function(SymbolNameSpace *sns,const std::string &name,int nargs) 
   : Symbol(sns,name)
{
   assert(nargs < 32) ; // the way initial argument identification is implemented requires this
   iNumberArgs = nargs ;
}


Function::~Function(void)
{
}

MacroFunction::MacroFunction(SymbolNameSpace *sns, SymbolNameSpace* local, const std::string& name, ExpressionList *margs) : 
	mArgs(margs), pSymbolNameSpace(local), Function(sns, name, margs->Length())
{
}


bool Function::CheckComputed(ContextInfo *info,ExpressionList *arg) 
{ 
    return arg->CheckComputed(info,0xffffffff) ;
}
void Function::OutputComputable(ContextInfo *info,ExpressionList *arg) 
{ 
   *info << ComputableName() << "(" ;
    arg->OutputComputable(info,0xffffffff) ;
    *info << ")" ;
}

bool FunctionMemoryBase::CheckComputed(ContextInfo *info,ExpressionList *arg) 
{ 
   if(info->GetComputeType() == CF_initial) 
      return arg->CheckComputed(info,iInitArgMark) ;
   else
      return arg->CheckComputed(info,iActiveArgMark) ;
}
void FunctionMemoryBase::OutputComputable(ContextInfo *info,ExpressionList *arg) 
{ 
	if (info->GetComputeType() == CF_xmile_output) {
		const std::string& fname = this->GetName();
		if (fname == "INITIAL")
		{
			*info << "INIT(";
			arg->OutputComputable(info, iInitArgMark);
			*info << ")";
		}
		else if (fname == "INTEG")
			arg->OutputComputable(info, iInitArgMark);
		else
			arg->OutputComputable(info, iActiveArgMark);
		return;
	}
		
   if(info->GetComputeType() == CF_initial)  {
      *info << ComputableNameInit() << "(" ;
      arg->OutputComputable(info,iInitArgMark) ;
   } else {
      *info << ComputableName() << "(" ;
      arg->OutputComputable(info,iActiveArgMark) ;
   }
   *info << ")" ;
}
/* a simple utility function to flip the bits so we can use BOOST_BINARY
  in the intuitive right to left manner for initial value arguments */
unsigned  FunctionMemoryBase::BitFlip(unsigned bits)
{
   unsigned newbits = 0 ;
   int i ;
   for(i=0;i<iNumberArgs;i++) {
      if(bits&(1 << i))
         newbits |= (1 << (iNumberArgs-i-1)) ;
   }
   return newbits ;
}

void FunctionIfThenElse::OutputComputable(ContextInfo *info, ExpressionList *arg)
{
	if (arg->Length() == 3)
	{
		*info << "( IF ";
		const_cast<Expression*>((*arg)[0])->OutputComputable(info); // OutputComputable should really be const
		*info << " THEN ";
		const_cast<Expression*>((*arg)[1])->OutputComputable(info); // OutputComputable should really be const
		*info << " ELSE ";
		const_cast<Expression*>((*arg)[2])->OutputComputable(info); // OutputComputable should really be const
		*info << " )";
		return;
	}
	Function::OutputComputable(info, arg);
}

void FunctionLog::OutputComputable(ContextInfo *info, ExpressionList *arg)
{
	if (arg->Length() == 2)
	{
		*info << "(LN(";
		const_cast<Expression*>((*arg)[0])->OutputComputable(info); // OutputComputable should really be const
		*info << ") / LN(";
		const_cast<Expression*>((*arg)[1])->OutputComputable(info); // OutputComputable should really be const
		*info << "))";
		return;
	}
	Function::OutputComputable(info, arg);
}




#ifdef WANT_EVAL_STUFF
double FunctionMax::Eval(Expression *from,ExpressionList *arg,ContextInfo *info) 
{
   double a1 = arg->GetExp(0)->Eval(info) ;
   double a2 = arg->GetExp(1)->Eval(info) ;
   return a1 > a2?a1:a2 ;
}
double FunctionMin::Eval(Expression *from,ExpressionList *arg,ContextInfo *info) 
{
   double a1 = arg->GetExp(0)->Eval(info) ;
   double a2 = arg->GetExp(1)->Eval(info) ;
   return a1 < a2?a1:a2 ;
}
double FunctionInteg::Eval(Expression *from,ExpressionList *arg,ContextInfo *info) 
{
   if(info->GetComputeType() == CF_initial)
      return arg->GetExp(1)->Eval(info) ;// initialization
   return arg->GetExp(0)->Eval(info) ;
return 0 ;
}
double FunctionPulse::Eval(Expression *from,ExpressionList *arg,ContextInfo *info)
{
   double s = arg->GetExp(0)->Eval(info) ;
   double dt = info->GetDT() ;
   double w = arg->GetExp(1)->Eval(info) ;
   if(w < dt)
      w = dt ;
   double t = info->GetTime() ;
   if(t > s -dt/4.0  && t < s + w - dt/4.0)
      return 1.0 ;
   return 0 ;
}
#endif