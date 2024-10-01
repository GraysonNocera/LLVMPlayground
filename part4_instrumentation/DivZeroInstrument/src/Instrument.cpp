#include "Instrument.h"

using namespace llvm;

namespace instrument
{

  static const char *SanitizerFunctionName = "__sanitize__";
  static const char *CoverageFunctionName = "__coverage__";

  /*
   * Implement divide-by-zero sanitizer.
   */
  void instrumentSanitize(Module *M, Function &F, Instruction &I)
  {
    /* Add you code here */
    std::vector<Type *> Params(3, Type::getInt32Ty(M->getContext()));
    std::vector<Value *> Args(3);
    DebugLoc d = I.getDebugLoc();
    if (!d)
    {
      return;
    }

    IntegerType *i32 = IntegerType::getInt32Ty(M->getContext());
    ConstantInt *line = ConstantInt::get(i32, d.getLine());
    ConstantInt *col = ConstantInt::get(i32, d.getCol());

    Args[0] = I.getOperand(1);
    Args[1] = line;
    Args[2] = col;

    FunctionType *FT = FunctionType::get(Type::getVoidTy(M->getContext()), Params, false);
    FunctionCallee f = M->getOrInsertFunction(SanitizerFunctionName, FT);
    CallInst *Call = CallInst::Create(f, Args, "", &I);
    Call->setCallingConv(CallingConv::C);
    Call->setTailCall(true);
  }

  /*
   * Implement code coverage instrumentation.
   */
  void instrumentCoverage(Module *M, Function &F, Instruction &I)
  {
    /* Add you code here */
    std::vector<Type *> Params(2, Type::getInt32Ty(M->getContext()));
    std::vector<Value *> Args(2);
    DebugLoc d = I.getDebugLoc();
    if (!d)
    {
      return;
    }

    IntegerType *i32 = IntegerType::getInt32Ty(M->getContext());
    ConstantInt *line = ConstantInt::get(i32, d.getLine());
    ConstantInt *col = ConstantInt::get(i32, d.getCol());

    Args[0] = line;
    Args[1] = col;

    FunctionType *FT = FunctionType::get(Type::getVoidTy(M->getContext()), Params, false);
    FunctionCallee f = M->getOrInsertFunction(CoverageFunctionName, FT);
    CallInst *Call = CallInst::Create(f, Args, "", &I);
    Call->setCallingConv(CallingConv::C);
    Call->setTailCall(true);
  }

  bool Instrument::runOnFunction(Function &F)
  {
    /* Add you code here */
    Module *M = F.getParent();

    outs() << "Running DivZeroInstrumentation on " << F.getName() << "\n";
    for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I)
    {
      Instruction &Inst = *I;
      if (BinaryOperator *BO = dyn_cast<BinaryOperator>(&Inst))
      {
        switch (BO->getOpcode())
        {
        case Instruction::SDiv:
        case Instruction::UDiv:
          instrumentSanitize(M, F, Inst);
        }
      }
      instrumentCoverage(M, F, Inst);
    }

    return true;
  }

  char Instrument::ID = 1;
  static RegisterPass<Instrument>
      X("Instrument", "Instrumentations for Dynamic Analysis", false, false);

} // namespace instrument
