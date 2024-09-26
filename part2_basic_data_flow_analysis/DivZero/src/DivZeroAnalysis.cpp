#include "DivZeroAnalysis.h"

namespace dataflow
{

  //===----------------------------------------------------------------------===//
  // DivZero Analysis Implementation
  //===----------------------------------------------------------------------===//

  /**
   * Implement your data-flow analysis.
   * 1. Define "flowIn" that joins the memory set of all incoming flows
   * 2. Define "transfer" that computes the semantics of each instruction.
   * 3. Define "flowOut" that flows the memory set to all outgoing flows
   * 4. Define "doAnalysis" that stores your results in "InMap" and "OutMap".
   * 5. Define "check" that checks if a given instruction is erroneous or not.
   */

  // define the following functions if needed (not compulsory to do so)
  Memory *join(Memory *M1, Memory *M2)
  {
    Memory *Result = new Memory();
    /* Add your code here */

    /* Result will be the union of memories M1 and M2 */

    return Result;
  }

  bool equal(Memory *M1, Memory *M2)
  {
    /* Add your code here */
    /* Return true if the two memories M1 and M2 are equal */
  }

  void DivZeroAnalysis::flowIn(Instruction *I, Memory *In)
  {
    /* Add your code here */
  }

  void DivZeroAnalysis::transfer(Instruction *I, const Memory *In, Memory *NOut)
  {
    /* Add your code here */
    if (BinaryOperator *BO = dyn_cast<BinaryOperator>(I))
    {
      outs() << "Binary operator: " << *BO << "\n";
      Value *op1 = BO->getOperand(0);
      Value *op2 = BO->getOperand(2);
      Domain *d1;
      Domain *d2;

      if (In->find(variable(op1)) == In->end())
      {
        d1 = &Domain();
        d1->Value = Domain::Uninit;
      }
      else
      {
        d1 = In->at(variable(op1));
      }

      if (In->find(variable(op2)) == In->end())
      {
        d2 = &Domain();
        d2->Value = Domain::Uninit;
      }
      else
      {
        d2 = In->at(variable(op2));
      }
      switch (BO->getOpcode())
      {
      case Instruction::Add:
        NOut->insert({variable(I), Domain::add(d1, d2)});
        break;
      case Instruction::Sub:
        NOut->insert({variable(I), Domain::sub(d1, d2)});
        break;
      case Instruction::Mul:
        NOut->insert({variable(I), Domain::mul(d1, d2)});
        break;
      case Instruction::SDiv:
      case Instruction::UDiv:
        NOut->insert({variable(I), Domain::div(d1, d2)});
        break;
      }
      outs() << "Nout for add " << NOut->at(variable(I)) << "\n";
    }
    else if (CastInst *CastI = dyn_cast<CastInst>(I))
    {
      // identity
      Domain *d = In->at(variable(I));
      NOut->insert({variable(I), d});
    }
    else if (CmpInst *CmpI = dyn_cast<CmpInst>(I))
    {
      Value *result = I;
      if (int *intResult = dyn_cast<int>(result->getType()))
      {
        Domain *d = Domain::abstract(*intResult);
        NOut->insert({variable(I), d});
      }
    }
    else if (BranchInst *BI = dyn_cast<BranchInst>(I))
    {
      Domain *d = In->at(variable(I));
      NOut->insert({variable(I), d});
    }
    else if (isInput(I))
    {
      int *c = cast<int>(I);
      Domain *d = Domain::abstract(*c);
      NOut->insert({variable(I), d});
    }
    else if (PHINode *P = dyn_cast<PHINode>(I))
    {
      Domain *d = evalPhiNode(P, NOut);
      NOut->insert({variable(I), d});
    }
  }

  void DivZeroAnalysis::flowOut(Instruction *I, Memory *Pre, Memory *Post, SetVector<Instruction *> &WorkSet)
  {
    /* Add your code here */
  }

  void DivZeroAnalysis::doAnalysis(Function &F)
  {
    SetVector<Instruction *> WorkSet;
    for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I)
    {
      I->dump();
      outs() << I->getType() << I->getName() << "\n";
      WorkSet.insert(&(*I));
      // if (BinaryOperator *BO = dyn_cast<BinaryOperator *>(I)) {
      //   outs() << "Binary operator: " << *BO << "\n";
      // }
    }

    /* Add your code here */
    /* Basic Workflow-
         Visit instruction in WorkSet
         For each visited instruction I, construct its In memory by joining all memory sets of incoming flows (predecessors of I)
         Based on the type of instruction I and the In memory, populate the NOut memory
         Based on the previous Out memory and the current Out memory, check if there is a difference between the two and
           flow the memory set appropriately to all successors of I and update WorkSet accordingly
    */
  }

  bool DivZeroAnalysis::check(Instruction *I)
  {
    /* Add your code here */
    
    return false;
  }

  char DivZeroAnalysis::ID = 1;
  static RegisterPass<DivZeroAnalysis> X("DivZero", "Divide-by-zero Analysis",
                                         false, false);
} // namespace dataflow
