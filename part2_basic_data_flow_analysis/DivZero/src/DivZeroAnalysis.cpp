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
      // OpCode is the actual instruction after the = (i.e. "icmp", "sdiv", "zext")
      // variable(I) gives the whole command as a string
      // getName() returns the x in %x = ...
      // getOperand(0) returns the operand
      // %div = sdiv i32 0, %conv
      // getOperand(0) -> i32 0
      // getOperand(1) -> %conv = zext i1 %cmp to i32
      outs() << I->getName() << " " << I->getOpcodeName() << "  \n";
      if (BinaryOperator *BO = dyn_cast<BinaryOperator>(&*I))
      {
        // Binary Operations are on the same type
        outs() << "BINARY OPERATOR" << "\n";
        outs() << "Dumping type: ";
        BO->getType()->dump(); // line below does the same thing
        BO->getOperand(0)->getType()->dump();
        outs() << "Operand dump: ";
        BO->getOperand(0)->dump();

        outs() << "Dumping type: ";
        BO->getOperand(1)->getType()->dump();
        outs() << "Operand dump: ";
        BO->getOperand(1)->dump();
      }
      if (CmpInst *CI = dyn_cast<CmpInst>(&*I))
      {
        outs() << "COMPARE INSTRUCTION" << "\n";
        outs() << "Dumping operands" << "\n";
        CI->getOperand(0)->dump();
        CI->getOperand(1)->dump();
        // CI->getOperand(2)->dump(); // panics with an out of bounds error
        outs() << "OpcodeName: " << CI->getOpcodeName() << "\n";
        outs() << "Predicate: " << CI->getPredicate() << "\n"; // Predicate is an enum that is "eq", "ne"
        outs() << "Is integer: " << CI->getOperand(0)->getType()->isIntegerTy() << "\n";
        if (CI->getOperand(0)->getType()->isIntegerTy())
        {
          if (ConstantInt *i = dyn_cast<ConstantInt>(CI->getOperand(0)))
          {
            APInt actual = i->getValue();
            outs() << "dumping int: ";
            actual.dump();
            outs() << "is negative: " << actual.isNegative() << "\n";
            outs() << "is positive: " << actual.isStrictlyPositive() << "\n";
            uint64_t x = actual.getLimitedValue();
            outs() << "finally got the int? " << x << "\n";
          }
        }
      }
      if (BranchInst *BI = dyn_cast<BranchInst>(&*I))
      {
        outs() << "BRANCH INSTRUCTION\n";
        // BI->getSuccessor(0)->dump(); // basically gets the if in the case of unconditional loop

        // these two get the same thing for a conditional thing
        if (BI->isConditional())
        {
          outs() << "Condition: ";
          BI->getCondition()->dump();
        }
        BI->getOperand(0)->dump(); // gets the condition as a Value/Instruction if conditional, else gets first basic block

        // BI->getSuccessor(1)->dump(); // basically gets the else
      }
      if (CastInst *CI = dyn_cast<CastInst>(&*I)) {
        outs() << "CAST INSTRUCTION\n";
        outs() << CI->getOpcodeName() << "\n";
        CI->getOperand(0)->dump();
        CI->getDestTy()->dump();
      }
      if (CallInst *CI = dyn_cast<CallInst>(&*I)) {
        outs() << "CALL INSTRUCTION\n";
        outs() << CI->getTailCallKind() << "\n"; // idk what tail call is
        CI->getType()->dump(); // gets the return type of this call
        outs() << CI->canReturnTwice() << "\n";
      }
      WorkSet.insert(&(*I));
      outs() << "\n\n\n";
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
