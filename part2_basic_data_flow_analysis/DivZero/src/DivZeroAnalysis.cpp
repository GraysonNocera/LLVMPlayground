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

    for (auto pair = M1->begin(); pair != M1->end(); ++pair)
    {
      Result->insert({pair->first, pair->second});
    }
    for (auto pair = M2->begin(); pair != M2->end(); ++pair)
    {
      if (Result->find(pair->first) == Result->end())
      {
        Domain *d = Domain::join(Result->at(pair->first), pair->second);
        Result->insert({pair->first, d});
      }
      else
      {
        Result->insert({pair->first, pair->second});
      }
    }

    /* Result will be the union of memories M1 and M2 */

    return Result;
  }

  bool equal(Memory *M1, Memory *M2)
  {
    /* Add your code here */
    /* Return true if the two memories M1 and M2 are equal */
    // merge m2 into m1, then check if m1 equals the previous m1

    // for (auto m1 = M1->begin(), m2 = M2->begin();
    //      m1 != M1->end() || m2 != M2->end(); ++m1, ++m2)
    // {
    // }
    if (M1->size() != M2->size())
    {
      return false;
    }
    for (auto m1 = M1->begin(); m1 != M1->end(); ++m1)
    {
      if (M2->find(m1->first) == M2->end())
      {
        // couldn't find M1 key in M2, thus return false
        return false;
      }
      if (M2->at(m1->first)->Value != M1->at(m1->first)->Value)
      {
        // if the values here are not equal, return false
        return false;
      }
    }

    return true;
  }

  void DivZeroAnalysis::flowIn(Instruction *I, Memory *In)
  {
    /* Add your code here */
    // joining all OUT sets of incoming flows and saving the result in the In set
    std::vector<Instruction *> predecessors = getPredecessors(I);
    Domain d = Domain(Domain::Uninit);
    Memory *joined = NULL;
    for (Instruction *predecessor : predecessors)
    {
      // populate In, which is a hash table {string : Domain *}
      Memory *outN = OutMap[predecessor];
      if (!joined)
      {
        joined = outN;
      }
      joined = join(joined, outN);
      // // Domain *temp = outN->at(variable(predecessor));
      // if (d.Value == Domain::Uninit)
      // {
      //   d = *temp;
      // }
      // else
      // {
      //   d = *Domain::join(&d, temp);
      // }
    }
    for (auto pair = joined->begin(); pair != joined->end(); ++pair)
    {
      In->insert({pair->first, pair->second});
    }
    // union the IN with the OUT
    // In->insert({variable(I), &d});
  }

  void DivZeroAnalysis::transfer(Instruction *I, const Memory *In, Memory *NOut)
  {
    /* Add your code here */
    for (auto in = In->begin(); in != In->end(); in++)
    {
      NOut->insert({in->first, in->second});
    }

    if (BinaryOperator *BO = dyn_cast<BinaryOperator>(I))
    {
      outs() << "Binary operator \n";
      Value *op1 = BO->getOperand(0);
      Value *op2 = BO->getOperand(1);
      Domain *d1 = new Domain(Domain::Uninit);
      Domain *d2 = new Domain(Domain::Uninit);

      // rational: if we find the variable in incoming hashmap, use it,
      // otherwise, if we see a constant, set that domain explicitly
      if (In->find(variable(op1)) == In->end())
      {
        if (ConstantInt *i = dyn_cast<ConstantInt>(op1))
        {
          d1->Value = i->isZero() ? Domain::Zero : Domain::NonZero;
        }
      }
      else
      {
        d1 = In->at(variable(op1));
      }

      if (In->find(variable(op2)) == In->end())
      {
        if (ConstantInt *i = dyn_cast<ConstantInt>(op2))
        {
          d2->Value = i->isZero() ? Domain::Zero : Domain::NonZero;
        }
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
      // not worrying about overflow and underflow here
      Value *op1 = CastI->getOperand(0);
      Domain *d = new Domain(Domain::Uninit);
      if (In->find(variable(op1)) == In->end())
      {
        if (ConstantInt *i = dyn_cast<ConstantInt>(op1))
        {
          d->Value = i->isZero() ? Domain::Zero : Domain::NonZero;
        }
      }
      else
      {
        d = In->at(variable(op1));
      }
      NOut->insert({variable(I), d});
    }
    else if (CmpInst *CmpI = dyn_cast<CmpInst>(I))
    {
      Value *op1 = BO->getOperand(0);
      Value *op2 = BO->getOperand(1);
      Domain *d1 = new Domain(Domain::Uninit);
      Domain *d2 = new Domain(Domain::Uninit);
      Domain *d3 = new Domain(Domain::Uninit);

      // rational: if we find the variable in incoming hashmap, use it,
      // otherwise, if we see a constant, set that domain explicitly
      if (In->find(variable(op1)) == In->end())
      {
        if (ConstantInt *i = dyn_cast<ConstantInt>(op1))
        {
          d1->Value = i->isZero() ? Domain::Zero : Domain::NonZero;
        }
      }
      else
      {
        d1 = In->at(variable(op1));
      }

      if (In->find(variable(op2)) == In->end())
      {
        if (ConstantInt *i = dyn_cast<ConstantInt>(op2))
        {
          d2->Value = i->isZero() ? Domain::Zero : Domain::NonZero;
        }
      }
      else
      {
        d2 = In->at(variable(op2));
      }

      if (d1->Value == Domain::MaybeZero || d2->Value == Domain::MaybeZero)
      {
        d3->Value = Domain::MaybeZero;
      }
      else if (d1->Value == Domain::Uninit || d2->Value == Domain::Uninit)
      {
        d3->Value = Domain::Uninit;
      }
      else
      {
        // at this point, we know that neither are MaybeZero or Uninit
        switch (CmpI->getPredicate())
        {
        case CmpInst::ICMP_EQ:
          if (d1->Value == Domain::Zero && d2->Value == Domain::Zero)
          {
            d3->Value = Domain::NonZero;
          }
          else if (d1->Value == Domain::NonZero && d2->Value == Domain::NonZero)
          {
            d3->Value = Domain::MaybeZero;
          }
          else
          {
            d3->Value = Domain::Zero;
          }
          break;
        case CmpInst::ICMP_NE:
          if (d1->Value == Domain::Zero && d2->Value == Domain::Zero)
          {
            d3->Value = Domain::Zero;
          }
          else if (d1->Value == Domain::NonZero && d2->Value == Domain::NonZero)
          {
            d3->Value = Domain::MaybeZero;
          }
          else
          {
            d3->Value = Domain::NonZero;
          }
        case CmpInst::ICMP_SGE:
        case CmpInst::ICMP_UGE:
        case CmpInst::ICMP_SLE:
        case CmpInst::ICMP_ULE:
          if (d1->Value == Domain::Zero && d2->Value == Domain::Zero)
          {
            d3->Value = Domain::NonZero;
          }
          else
          {
            d3->Value = Domain::MaybeZero;
          }
          break;
        case CmpInst::ICMP_SGT:
        case CmpInst::ICMP_UGT:
        case CmpInst::ICMP_SLT:
        case CmpInst::ICMP_ULT:
          if (d1->Value == Domain::Zero && d2->Value == Domain::Zero)
          {
            d3->Value = Domain::Zero;
          }
          else
          {
            d3->Value = Domain::MaybeZero;
          }
          break;
        }
      }
      NOut->insert({variable(I), d3});
    }
    else if (BranchInst *BI = dyn_cast<BranchInst>(I))
    {
      // nothing to do here? we don't get any new variables or anything
    }
    else if (isInput(I))
    {
      // getchar() returns int -> maybeZero
      Domain *d = new Domain(Domain::MaybeZero);
      NOut->insert({variable(I), d});
    }
    else if (PHINode *P = dyn_cast<PHINode>(I))
    {
      // evalPhiNode manipulated NOut
      Domain *d = evalPhiNode(P, NOut);
    }
  }

  void DivZeroAnalysis::flowOut(Instruction *I, Memory *Pre, Memory *Post, SetVector<Instruction *> &WorkSet)
  {
    /* Add your code here */
    bool isEqual = equal(Pre, Post);
    if (!isEqual)
    {
      // add it back to worker set for processing
      WorkSet.insert(I);
    }

    Memory *mem = OutMap[I];
    Memory *joined = join(mem, Post);
    OutMap[I] = joined;
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
    while (WorkSet.size() > 0)
    {
      Instruction *I = WorkSet.back();
      WorkSet.pop_back();

      flowIn(I, InMap[I]);

      transfer(I, InMap[I], OutMap[I]);

      flowOut(I, InMap[I], OutMap[I], WorkSet);
    }
  }

  bool DivZeroAnalysis::check(Instruction *I)
  {
    /* Add your code here */
    Memory *mem = InMap[I];
    Domain *d = mem->at(variable(I));
    if (BinaryOperator *BO = dyn_cast<BinaryOperator>(I))
    {
      switch (BO->getOpcode())
      {
      case Instruction::SDiv:
      case Instruction::UDiv:
        Value *op1 = BO->getOperand(0);
        Value *op2 = BO->getOperand(1);
        // int *i = cast<int>(op2);
        // Domain *res = Domain::abstract(*i);
        // if (res->Value == Domain::Zero)
        // {
        //   return true;
        // }
        Domain *d = new Domain(Domain::Uninit);
      }

      return false;
    }
    return false;
  }

  char DivZeroAnalysis::ID = 1;
  static RegisterPass<DivZeroAnalysis> X("DivZero", "Divide-by-zero Analysis",
                                         false, false);

} // namespace dataflow
