#include "DivZeroAnalysis.h"

namespace dataflow
{

  //===----------------------------------------------------------------------===//
  // Dataflow Analysis Implementation
  //===----------------------------------------------------------------------===//

  /**
   * Implement your data-flow analysis.
   * 1. Define "flowIn" that joins the memory set of all incoming flows
   * 2. Define "transfer" that computes the semantics of each instruction.
   * 3. Define "flowOut" that flows the memory set to all outgoing flows
   * 4. Define "doAnalysis" that stores your results in "InMap" and "OutMap".
   * 5. Define "check" that checks if a given instruction is erroneous or not.
   */

  void printMemory(Memory *m1)
  {
    outs() << "Printing Memory:\n";
    for (auto pair = m1->begin(); pair != m1->end(); ++pair)
    {
      char str[1024];
      sprintf(str, "\t%20s -> %d\n", pair->first.c_str(), pair->second->Value);
      outs() << str;
    }
    outs() << "\n";
  }

  Memory *join(Memory *M1, Memory *M2)
  {
    Memory *Result = new Memory();

    for (auto pair = M1->begin(); pair != M1->end(); ++pair)
    {
      Result->insert({pair->first, pair->second});
    }

    for (auto pair = M2->begin(); pair != M2->end(); ++pair)
    {
      if (Result->find(pair->first) == Result->end())
      {
        // cannot find M2 key in M1, so we call it Uninit in M1
        // Domain *d = Domain::join(new Domain(Domain::Uninit), pair->second);
        Result->insert({pair->first, pair->second});
      }
      else
      {
        // it is in both hashmaps
        Domain *m1d = Result->at(pair->first);
        if (m1d->Value == Domain::Uninit)
        {
          Result->insert({pair->first, pair->second});
        }
        else
        {
          Domain *d = Domain::join(Result->at(pair->first), pair->second);
          Result->insert({pair->first, d});
        }
      }
    }

    return Result;
  }

  bool equal(Memory *M1, Memory *M2)
  {
    /* Add your code here */
    /* Return true if the two memories M1 and M2 are equal */
    // merge m2 into m1, then check if m1 equals the previous m1

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
    // joining all OUT sets of incoming flows and saving the result in the In set
    std::vector<Instruction *> predecessors = getPredecessors(I);
    Memory *joined = NULL;
    for (Instruction *predecessor : predecessors)
    {
      // populate In, which is a hash table {string : Domain *}
      Memory *outN = OutMap[predecessor];
      // printMemory(outN);
      if (!joined)
      {
        joined = outN;
      }
      joined = join(joined, outN);
    }
    if (!joined)
    {
      return;
    }

    for (auto pair = joined->begin(); pair != joined->end(); ++pair)
    {
      In->insert({pair->first, pair->second});
    }
  }

  void DivZeroAnalysis::transfer(Instruction *I, const Memory *In, Memory *NOut,
                                 PointerAnalysis *PA,
                                 SetVector<Value *> PointerSet)
  {
    for (auto in = In->begin(); in != In->end(); in++)
    {
      NOut->insert({in->first, in->second});
    }

    if (BinaryOperator *BO = dyn_cast<BinaryOperator>(I))
    {
      outs() << "Binary operator\n";
      Value *op1 = BO->getOperand(0);
      Value *op2 = BO->getOperand(1);
      Domain *d1 = new Domain(Domain::Uninit);
      Domain *d2 = new Domain(Domain::Uninit);

      // rational: if we find the variable in incoming hashmap, use it,
      // otherwise, if we see a constant, set that domain explicitly
      ConstantInt *int1 = nullptr;
      ConstantInt *int2 = nullptr;
      if (In->find(variable(op1)) == In->end())
      {
        if (ConstantInt *i = dyn_cast<ConstantInt>(op1))
        {
          d1->Value = i->isZero() ? Domain::Zero : Domain::NonZero;
          int1 = i;
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
          int2 = i;
        }
      }
      else
      {
        d2 = In->at(variable(op2));
      }

      if (int1 && int2)
      {
        APInt a1 = int1->getValue();
        APInt a2 = int2->getValue();
        if ((a1.isStrictlyPositive() && a2.isStrictlyPositive()) || (a1.isNegative() && a2.isNegative()))
        {
          // this is to induce a NonZero output
          d1->Value = Domain::Zero;
          d2->Value = Domain::NonZero;
        }
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
    }
    else if (CastInst *CastI = dyn_cast<CastInst>(I))
    {
      outs() << "Cast int\n";
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
      outs() << "compare inst\n";
      Value *op1 = CmpI->getOperand(0);
      Value *op2 = CmpI->getOperand(1);
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
      outs() << "got d2: " << d2->Value << " and d3: " << d3->Value << "\n";

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
      outs() << "branch inst\n";
      // nothing to do here? we don't get any new variables or anything
    }
    else if (isInput(I))
    {
      outs() << "getchar()\n";
      // getchar() returns int -> maybeZero
      Domain *d = new Domain(Domain::MaybeZero);
      outs() << "d->Value" << d->Value << "\n";
      NOut->insert({variable(I), d});
    }
    else if (CallInst *CI = dyn_cast<CallInst>(I))
    {
      if (!CI->getFunctionType()->getReturnType()->isIntegerTy())
      {
        return;
      }
      Domain *d = new Domain(Domain::MaybeZero);
      // because we don't know what happened in this function, we just assign
      // the return type to be MaybeZero because it could be
      NOut->insert({variable(I), d});
    }
    else if (AllocaInst *AI = dyn_cast<AllocaInst>(I))
    {
      // TODO: do we need this?
    }
    else if (LoadInst *LI = dyn_cast<LoadInst>(I))
    {
      Value *FromPtr = LI->getPointerOperand();
      Domain *d = new Domain(Domain::Uninit);
      outs() << "load type ";
      LI->getType()->dump();
      if (LI->getType()->isIntegerTy())
      {
        Value *LoadedInt = FromPtr;
        if (In->find(variable(LoadedInt)) == In->end())
        {
          // IDK if this actually possible, can loads have a constant value
          if (ConstantInt *i = dyn_cast<ConstantInt>(LoadedInt))
          {
            d->Value = i->isZero() ? Domain::Zero : Domain::NonZero;
          }
        }
        else
        {
          d = In->at(variable(LoadedInt));
        }
      }
      else if (FromPtr->getType()->isPointerTy())
      {
        outs() << "a load with a pointer!\n";
        d = nullptr;
        for (Value *V : PointerSet)
        {
          std::string a = address(V);
          std::string b = address(FromPtr);
          if (PA->alias(a, b))
          {
            // update abstract values
            if (!d)
            {
              outs() << "is it here?\n";
              d = In->at(variable(V));
            }
            d = Domain::join(d, In->at(variable(V)));
          }
        }
        if (!d) {
          d = new Domain(Domain::Uninit);
        }
        outs() << "value of d: " << d << "\n";
      }
      NOut->insert({variable(I), d});
    }
    else if (StoreInst *SI = dyn_cast<StoreInst>(I))
    {
      // for a store, setup:

      Value *Ptr = SI->getPointerOperand();
      Value *Val = SI->getValueOperand();
      Domain *domainVal = new Domain(Domain::Uninit);
      if (Val->getType()->isIntegerTy())
      {
        // if integery type:
        // get the abstract domain value of the value operand
        // update the Ptr operand's out to be domain value of value operand
        if (In->find(variable(Val)) == In->end())
        {
          if (ConstantInt *i = dyn_cast<ConstantInt>(Val))
          {
            domainVal->Value = i->isZero() ? Domain::Zero : Domain::NonZero;
          }
        }
        else
        {
          domainVal = In->at(variable(Val));
        }
        NOut->insert({variable(Ptr), domainVal});
      }
      else if (Val->getType()->isPointerTy())
      {
        outs() << "oh boy it's here\n";
        Domain *domainVal = In->at(variable(Val));
        SetVector<Value *> aliases;
        for (Value *V : PointerSet)
        {
          std::string a = address(V);
          std::string b = address(Ptr);
          if (PA->alias(a, b))
          {
            // join the current domainValue with the domainValue at each alias of pointer
            domainVal = Domain::join(domainVal, In->at(variable(V)));
            aliases.insert(V);
          }
        }
        NOut->insert({variable(Ptr), domainVal});
        for (Value *V : aliases)
        {
          NOut->insert({variable(V), domainVal});
        }
      }
    }
    printMemory(NOut);
  }

  void DivZeroAnalysis::flowOut(Instruction *I, Memory *Pre, Memory *Post, SetVector<Instruction *> &WorkSet)
  {
    // printMemory(Pre);
    // printMemory(Post);
    bool isEqual = equal(Pre, Post);
    if (!isEqual)
    {
      // add it back to worker set for processing
      WorkSet.insert(I);
    }

    // I'm confused by this
    for (auto pair = Post->begin(); pair != Post->end(); ++pair)
    {
      OutMap[I]->insert({pair->first, pair->second});
    }
  }

  void DivZeroAnalysis::doAnalysis(Function &F, PointerAnalysis *PA)
  {
    /* Add your code here */
    SetVector<Instruction *> WorkSet;
    SetVector<Value *> PointerSet;
    for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I)
    {
      WorkSet.insert(&(*I));
      PointerSet.insert(&(*I));
    }

    // handle function arguments here, we won't handle alloca instructions in flowIn, transfer, or flowOut,
    // so we instantiate them here
    for (Function::arg_iterator A = F.arg_begin(), E = F.arg_end(); A != E; ++A)
    {
      Argument &Arg = *A;
      if (!Arg.getType()->isIntegerTy())
      {
        // skip non-integer types
        continue;
      }
      outs() << "argument: " << Arg.getName() << "\n";

      // initialize each argument to MaybeZero because we know nothing about it
      Memory *M = new Memory();
      Domain *d1 = new Domain(Domain::MaybeZero);
      M->insert({variable(&Arg), d1});

      InMap[cast<Instruction>(&Arg)] = M;
    }

    /* Add your code here */
    /* Basic Workflow-
         Visit instruction in WorkSet
         For each visited instruction I, construct its In memory by joining all memory sets of incoming flows (predecessors of I)
         Based on the type of instruction I and the In memory, populate the NOut memory. Take the pointer analysis into consideration for this step
         Based on the previous Out memory and the current Out memory, check if there is a difference between the two and
           flow the memory set appropriately to all successors of I and update WorkSet accordingly
    */
    while (WorkSet.size() > 0)
    {
      Instruction *I = WorkSet.front();
      WorkSet.remove(I);
      Memory *in = new Memory();
      Memory *out = new Memory();

      // DEBUGGING
      outs() << "PROCESSING I: \n";
      I->dump();
      outs() << "\n";
      outs() << "first flow in \n";
      // DEBUGGING

      flowIn(I, in);
      InMap[I] = in;

      // DEBUGGING
      outs() << "after flowIn: ";
      printMemory(InMap[I]);
      outs() << "transfer\n";
      // DEBUGGING

      Memory *pre = OutMap[I];
      transfer(I, in, out, PA, PointerSet);

      // DEBUGGING
      outs() << "after transfer: ";
      printMemory(in);
      printMemory(out);
      outs() << "flowOut\n\n\n";
      // DEBUGGING

      flowOut(I, pre, out, WorkSet);

      // DEBUGGING
      // outs() << "flow out: ";
      // printMemory(OutMap[I]);
    }
  }

  bool DivZeroAnalysis::check(Instruction *I)
  {
    Memory *mem = InMap[I];
    if (BinaryOperator *BO = dyn_cast<BinaryOperator>(I))
    {
      switch (BO->getOpcode())
      {
      case Instruction::SDiv:
      case Instruction::UDiv:
        outs() << "checking for errors \n";
        Value *op1 = BO->getOperand(0);
        Value *op2 = BO->getOperand(1);
        Domain *d1 = new Domain(Domain::Uninit);
        Domain *d2 = new Domain(Domain::Uninit);
        if (mem->find(variable(op1)) == mem->end())
        {
          if (ConstantInt *i = dyn_cast<ConstantInt>(op1))
          {
            d1->Value = i->isZero() ? Domain::Zero : Domain::NonZero;
          }
        }
        else
        {
          d1 = mem->at(variable(op1));
        }
        if (mem->find(variable(op2)) == mem->end())
        {
          if (ConstantInt *i = dyn_cast<ConstantInt>(op2))
          {
            d2->Value = i->isZero() ? Domain::Zero : Domain::NonZero;
          }
        }
        else
        {
          d2 = mem->at(variable(op2));
        }

        // actual check for division by 0 occurs here
        if (d2->Value == Domain::Zero || d2->Value == Domain::MaybeZero)
        {
          outs() << "DIVISION BY 0\n";
          return true;
        }
      }
    }
    return false;
  }

  char DivZeroAnalysis::ID = 1;
  static RegisterPass<DivZeroAnalysis> X("DivZero", "Divide-by-zero Analysis",
                                         false, false);
} // namespace dataflow
