#ifndef _XMUTIL_MODEL_H
#define _XMUTIL_MODEL_H
#include "Symbol/Variable.h"
#include "Symbol/Expression.h"
#include <vector>
class View
{
public:
	virtual bool UpgradeGhost(Variable* var) = 0;
	virtual bool AddFlowDefinition(Variable* var, Variable* in, Variable* out) = 0;
	virtual bool AddVarDefinition(Variable* var, int x, int y) = 0;
	virtual void CheckLinksIn() = 0;
	// just a placeholder to derive from
};
class Model
{
public:
   Model(void);
   ~Model(void);
   bool UnitsCheck(void) { return false ; }
   bool AnalyzeEquations(void) ;
   bool Simulate(void) ;
   SymbolNameSpace *GetNameSpace(void) { return &mSymbolNameSpace ; }
   Equation *AddUnnamedVariable(ExpressionFunctionMemory *e) ;
   bool RenameVariable(Variable *v,const std::string &newname) ;
   void GenerateCanonicalNames(void) ;
   void GenerateShortNames(void) ;
   bool OutputComputable(bool wantshort) ;
   bool MarkVariableTypes();
   void AttachStragglers(); // try to get diagramatic stuff right
   bool WriteToXMILE(const std::string& filePath, std::vector<std::string>& errs);

   double GetConstanValue(const char *var, double defval);
   void SetUnwanted(const char *var, const char *nametouse);
   std::vector<Variable*> GetVariables();
   void AddView(View* view) { vViews.push_back(view); }
   std::vector<View*>& Views() { return vViews; }

private :
   bool OrderEquations(ContextInfo *info,bool tonly) ;
   bool SetupVariableStates(int pass) ;
   bool ValidatePlaceholderVars(void) ;
   bool OrganizeSubscripts(void) ;
   void ClearCompEquations(void) ;


   SymbolNameSpace mSymbolNameSpace ;
   std::vector<View*> vViews;
   std::vector<Variable *>vUnamedVars;
   //std::vector<Equation *>vConstantComps ; // actually just assignment
   std::vector<Equation *>vInitialTimeComps ;
   std::vector<Equation *>vInitialComps ;
   std::vector<Equation *>vUnchangingComps ;
   std::vector<Equation *>vActiveComps ;
   std::vector<Equation *>vRateComps ; 
   /* the last could be part of active but it is helpful to split 
      out when creating equations for a computer language */
   int iNLevel ;
   int iNAux ;
   double *dLevel ;
   double *dRate ;
   double *dAux ;
};

#endif
