#include "PointerAnalysis.h"

#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instructions.h"

#include "DataflowAnalysis.h"

namespace dataflow {

//===----------------------------------------------------------------------===//
// Pointer Analysis Implementation
//===----------------------------------------------------------------------===//

/*
 * Implement your pointer analysis. (Lab 6)
 * 1. Define "PointerAnalysis" that stores your results in "PointsTo".
 * 2. Define "alias" that checks whether two pointers may alias each other.
 */

// PointsToInfo is a map of { instruction : {} }
// If we index a key that isn't in the map, it's an empty set that is returned
// Question: when do the sets get greater than 1 value
// a store instruction does not get added but only changes existing sets in the map

// Example: pointer0.opt.ll
/*
I: %a = alloca i32, align 4
PointsToInfo: { "%a = alloca i32, align 4": { "@(%a = alloca i32, align 4" )" } }

I: %c = alloca i32*, align 8
PointsToInfo:
{ 
"%a = alloca i32, align 4": { "@(%a = alloca i32, align 4" )" } 
"%c = alloca i32*, align 8": { "@(%c = alloca i32*, align 8)" }
}

I: %d = alloca i32*, align 8
PointsToInfo:
{
"%a = alloca i32, align 4": { "@(%a = alloca i32, align 4" )" } 
"%c = alloca i32*, align 8": { "@(%c = alloca i32*, align 8)" }
"%d = alloca i32*, align 8": { "@(%d = alloca i32*, align 8)" }
}

I: %x = alloca i32, align 4
PointsToInfo:
{
"%a = alloca i32, align 4": { "@(%a = alloca i32, align 4" )" } 
"%c = alloca i32*, align 8": { "@(%c = alloca i32*, align 8)" }
"%d = alloca i32*, align 8": { "@(%d = alloca i32*, align 8)" }
"%x = alloca i32, align 4": { "@(%x = alloca i32, align 4)" }
}

I: store i32* %a, i32** %c, align 8
PointsToInfo:
{
"%a = alloca i32, align 4": { "@(%a = alloca i32, align 4" )" } 
"%c = alloca i32*, align 8": { "@(%c = alloca i32*, align 8)" }
"%d = alloca i32*, align 8": { "@(%d = alloca i32*, align 8)" }
"%x = alloca i32, align 4": { "@(%x = alloca i32, align 4)" }
"@(%c = alloca i32*, align 8)": { "@(%a = alloca i32, align 4" )" } 
}
L = { "@(%c = alloca i32*, align 8)" }
R = { "@(%a = alloca i32, align 4" )" } 

I: store i32* %a, i32** %d, align 8
PointsToInfo:
{
"%a = alloca i32, align 4": { "@(%a = alloca i32, align 4" )" } 
"%c = alloca i32*, align 8": { "@(%c = alloca i32*, align 8)" }
"%d = alloca i32*, align 8": { "@(%d = alloca i32*, align 8)" }
"%x = alloca i32, align 4": { "@(%x = alloca i32, align 4)" }
"@(%c = alloca i32*, align 8)": { "@(%a = alloca i32, align 4" )" } 
"@(%d = alloca i32*, align 8)": { "@(%a = alloca i32, align 4" )" } 
}
L = { "@(%d = alloca i32*, align 8)" }
R = { "@(%a = alloca i32, align 4" )" } 

I: %0 = load i32*, i32** %c, align 8
PointsToInfo:
{
"%a = alloca i32, align 4": { "@(%a = alloca i32, align 4" )" } 
"%c = alloca i32*, align 8": { "@(%c = alloca i32*, align 8)" }
"%d = alloca i32*, align 8": { "@(%d = alloca i32*, align 8)" }
"%x = alloca i32, align 4": { "@(%x = alloca i32, align 4)" }
"@(%c = alloca i32*, align 8)": { "@(%a = alloca i32, align 4" )" } 
"@(%d = alloca i32*, align 8)": { "@(%a = alloca i32, align 4" )" } 
"%0 = load i32*, i32** %c, align 8": { "@(%a = alloca i32, align 4"  }
}
Variable = "%c = alloca i32*, align 8"
R = { "@(%c = alloca i32*, align 8)" }
Result = { "@(%a = alloca i32, align 4" }

I: %1 = load i32*, i32** %d, align 8
PointsToInfo:
{
"%a = alloca i32, align 4": { "@(%a = alloca i32, align 4" )" } 
"%c = alloca i32*, align 8": { "@(%c = alloca i32*, align 8)" }
"%d = alloca i32*, align 8": { "@(%d = alloca i32*, align 8)" }
"%x = alloca i32, align 4": { "@(%x = alloca i32, align 4)" }
"@(%c = alloca i32*, align 8)": { "@(%a = alloca i32, align 4" )" } 
"@(%d = alloca i32*, align 8)": { "@(%a = alloca i32, align 4" )" } 
"%0 = load i32*, i32** %c, align 8": { "@(%a = alloca i32, align 4"  }
"%1 = load i32*, i32** %d, align 8": { "@(%a = alloca i32, align 4" }
}
Variable = "%d = alloca i32*, align 8"
R = { "@(%d = alloca i32*, align 8)" }
Result = { "@(%a = alloca i32, align 4" )" } 

I: store i32* %x, i32** %c, align 8
PointsToInfo:
{
"%a = alloca i32, align 4": { "@(%a = alloca i32, align 4" )" } 
"%c = alloca i32*, align 8": { "@(%c = alloca i32*, align 8)" }
"%d = alloca i32*, align 8": { "@(%d = alloca i32*, align 8)" }
"%x = alloca i32, align 4": { "@(%x = alloca i32, align 4)" }
"@(%c = alloca i32*, align 8)": { "@(%a = alloca i32, align 4", "@(%x = alloca i32, align 4)" }
"@(%d = alloca i32*, align 8)": { "@(%a = alloca i32, align 4" )" } 
"%0 = load i32*, i32** %c, align 8": { "@(%a = alloca i32, align 4"  }
"%1 = load i32*, i32** %d, align 8": { "@(%a = alloca i32, align 4" }
}
Pointer = %c = alloca i32*, align 8
Value = %x = alloca i32, align 4
L = { "@(%c = alloca i32*, align 8)" }
R = { "@(%x = alloca i32, align 4)" }

I: %3 = load i32*, i32** %c, align 8
PointsToInfo:
{
"%a = alloca i32, align 4": { "@(%a = alloca i32, align 4" )" } 
"%c = alloca i32*, align 8": { "@(%c = alloca i32*, align 8)" }
"%d = alloca i32*, align 8": { "@(%d = alloca i32*, align 8)" }
"%x = alloca i32, align 4": { "@(%x = alloca i32, align 4)" }
"@(%c = alloca i32*, align 8)": { "@(%a = alloca i32, align 4", "@(%x = alloca i32, align 4)" }
"@(%d = alloca i32*, align 8)": { "@(%a = alloca i32, align 4" )" } 
"%0 = load i32*, i32** %c, align 8": { "@(%a = alloca i32, align 4"  }
"%1 = load i32*, i32** %d, align 8": { "@(%a = alloca i32, align 4" }
"%3 = load i32*, i32** %c, align 8": { "@(%a = alloca i32, align 4", "@(%x = alloca i32, align 4)" }
}
Variable = "%c = alloca i32*, align 8"
R = { "@(%c = alloca i32*, align 8)" }
Result = 
*/
void transfer(Instruction *I, PointsToInfo &PointsTo) {
  if (AllocaInst *AI = dyn_cast<AllocaInst>(I)) {
    // pointer is string version of instructions
    // ex: %a = alloca i32, align 4
    // %a is a pointer to an i32
    // PointsToInfo is { instruction: set(address1, address2) }
    std::string Pointer = variable(AI);
    PointsToSet &S = PointsTo[variable(AI)];
    S.insert(address(AI));
    
    // Result now is 
    // { "%a = alloca i32, align 4" : { "@(%a)" } }

  } else if (StoreInst *SI = dyn_cast<StoreInst>(I)) {
    // we ignore instructions that are not storing pointers
    // so ignore something like "store i32 1, i32* %a, align 4"
    if (!SI->getValueOperand()->getType()->isPointerTy())
      return;

    // example: store i32* %a, i32** %d, align 8
    // *Pointer = i32** %d
    Value *Pointer = SI->getPointerOperand();
    // *Value = i32* %a
    Value *Value = SI->getValueOperand();
    PointsToSet &L = PointsTo[variable(Pointer)];
    PointsToSet &R = PointsTo[variable(Value)];
    // Iterate through everything that the Pointer points to
    for (auto &I : L) {
      // Get everything that Pointer points to points to
      // Does this imply we are not handling double pointers

      // We are pretty much unioning every instruction that 
      // Pointer points to points to
      // Honestly, it's hard to figure this out unless I have 
      // an example run
      PointsToSet &S = PointsTo[I];
      PointsToSet Result;
      std::set_union(S.begin(), S.end(), R.begin(), R.end(),
                     std::inserter(Result, Result.begin()));
      PointsTo[I] = Result;
    }
  } else if (LoadInst *LI = dyn_cast<LoadInst>(I)) {
    // if not a pointer load, skip it
    if (!LI->getType()->isPointerTy())
      return;
    // there's only one operand, it's from where we are loading
    std::string Variable = variable(LI->getPointerOperand());
    PointsToSet &R = PointsTo[Variable];
    // this line not needed? the one right below this line
    // PointsToSet &L = PointsTo[variable(LI)];
    PointsToSet Result;
    // Union everything that Points to points to Variable
    for (auto &I : R) {
      PointsToSet &S = PointsTo[I];
      std::set_union(S.begin(), S.end(), Result.begin(), Result.end(),
                     std::inserter(Result, Result.begin()));
    }
    PointsTo[variable(LI)] = Result;
  }

  outs() << "After handling instruction: " << variable(I) << ": \n";
  for (auto &I : PointsTo) {
    errs() << "  " << I.first << ": { ";
    for (auto &J : I.second) {
      errs() << J << "; ";
    }
    errs() << "}\n";
  }
  errs() << "\n";
}

int countFacts(PointsToInfo &PointsTo) {
  int N = 0;
  for (auto &I : PointsTo) {
    N += I.second.size();
  }
  return N;
}

void print(std::map<std::string, PointsToSet> &PointsTo) {
  errs() << "Pointer Analysis Results:\n";
  for (auto &I : PointsTo) {
    errs() << "  " << I.first << ": { ";
    for (auto &J : I.second) {
      errs() << J << "; ";
    }
    errs() << "}\n";
  }
  errs() << "\n";
}

PointerAnalysis::PointerAnalysis(Function &F) {
  int NumOfOldFacts = 0;
  int NumOfNewFacts = 0;
  // this just keeps going until things converge it seems
  while (true) {
    for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I) {
      // already calls transfer function, we don't need to!!
      transfer(&*I, PointsTo);
    }
    NumOfNewFacts = countFacts(PointsTo);
    if (NumOfOldFacts < NumOfNewFacts)
      NumOfOldFacts = NumOfNewFacts;
    else
      break;
  }
  print(PointsTo);
}

bool PointerAnalysis::alias(std::string &Ptr1, std::string &Ptr2) const {
  if (PointsTo.find(Ptr1) == PointsTo.end() ||
      PointsTo.find(Ptr2) == PointsTo.end())
    return false;
  const PointsToSet &S1 = PointsTo.at(Ptr1);
  const PointsToSet &S2 = PointsTo.at(Ptr2);
  PointsToSet Inter;
  std::set_intersection(S1.begin(), S1.end(), S2.begin(), S2.end(),
                        std::inserter(Inter, Inter.begin()));
  return !Inter.empty();
}

}; // namespace dataflow
