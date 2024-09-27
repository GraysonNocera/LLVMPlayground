#include "DataflowAnalysis.h"

using namespace llvm;

namespace dataflow
{

  //===----------------------------------------------------------------------===//
  // Memory Access
  //===----------------------------------------------------------------------===//

  const char *WhiteSpaces = " \t\n\r";

  // instructions are also values, so you can pass variable(I1) to get the Value computed from instruction
  std::string variable(Value *V)
  {
    // encodes an llvm Value into string representation

    std::string Code;
    raw_string_ostream SS(Code);
    V->print(SS);
    Code.erase(0, Code.find_first_not_of(WhiteSpaces));
    return Code;
  }

  //===----------------------------------------------------------------------===//
  // Dataflow Analysis Implementation
  //===----------------------------------------------------------------------===//

  DataflowAnalysis::DataflowAnalysis(char ID) : FunctionPass(ID) {}

  void DataflowAnalysis::collectErrorInsts(Function &F)
  {
    for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I)
    {
      if (check(&*I))
        ErrorInsts.insert(&*I);
    }
  }

  bool DataflowAnalysis::runOnFunction(Function &F)
  {
    outs() << "Running " << getAnalysisName() << " on " << F.getName() << "\n";

    // Iterate through all instructions in a given function F to initialize InMap and OutMap
    for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I)
    {
      InMap[&(*I)] = new Memory;
      OutMap[&(*I)] = new Memory;
    }

    // Analyze the function
    // This will fill the InMaps and OutMaps
    doAnalysis(F);

    // Find devide by 0 entries
    collectErrorInsts(F);
    outs() << "Potential Instructions by " << getAnalysisName() << ": \n";
    for (auto I : ErrorInsts)
    {
      outs() << *I << "\n";
    }

    // clean up value map
    for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I)
    {
      delete InMap[&(*I)];
      delete OutMap[&(*I)];
    }
    return false;
  }

  Domain *DataflowAnalysis::evalPhiNode(PHINode *PHI, Memory *Mem)
  {
    Value *cv = PHI->hasConstantValue();
    if (cv)
    {
      Domain *d = new Domain(Domain::Uninit);
      if (Mem->find(variable(cv)) == Mem->end())
      {
        if (ConstantInt *i = dyn_cast<ConstantInt>(cv))
        {
          d->Value = i->isZero() ? Domain::Zero : Domain::NonZero;
        }
      }
      else
      {
        d = Mem->at(variable(cv));
      }
      Mem->insert({variable(PHI), d});
      return d;
    }

    unsigned int n = PHI->getNumIncomingValues();
    Domain *joined = nullptr;
    for (unsigned int i = 0; i < n; i++)
    {
      Value *V = PHI->getIncomingValue(i);
      Domain *d = new Domain(Domain::Uninit);
      if (Mem->find(variable(V)) == Mem->end())
      {
        if (ConstantInt *i = dyn_cast<ConstantInt>(V))
        {
          d->Value = i->isZero() ? Domain::Zero : Domain::NonZero;
        }
      }
      else
      {
        d = Mem->at(variable(V));
      }

      if (!joined)
      {
        joined = d;
      }
      joined = Domain::join(joined, d);
    }
    Mem->insert({variable(PHI), joined});
    return joined;
  }
} // namespace dataflow
