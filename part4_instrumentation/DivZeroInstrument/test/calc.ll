; ModuleID = 'calc.c'
source_filename = "calc.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [33 x i8] c"Enter an operator (+, -, *, /): \00", align 1
@.str.1 = private unnamed_addr constant [4 x i8] c" %c\00", align 1
@.str.2 = private unnamed_addr constant [21 x i8] c"Enter two operands: \00", align 1
@.str.3 = private unnamed_addr constant [6 x i8] c"%d %d\00", align 1
@.str.4 = private unnamed_addr constant [14 x i8] c"%d + %d = %d\0A\00", align 1
@.str.5 = private unnamed_addr constant [14 x i8] c"%d - %d = %d\0A\00", align 1
@.str.6 = private unnamed_addr constant [14 x i8] c"%d * %d = %d\0A\00", align 1
@.str.7 = private unnamed_addr constant [14 x i8] c"%d / %d = %d\0A\00", align 1
@.str.8 = private unnamed_addr constant [31 x i8] c"Error! operator is not correct\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 !dbg !7 {
entry:
  %retval = alloca i32, align 4
  %operator = alloca i8, align 1
  %firstNumber = alloca i32, align 4
  %secondNumber = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata i8* %operator, metadata !11, metadata !DIExpression()), !dbg !13
  call void @llvm.dbg.declare(metadata i32* %firstNumber, metadata !14, metadata !DIExpression()), !dbg !15
  call void @llvm.dbg.declare(metadata i32* %secondNumber, metadata !16, metadata !DIExpression()), !dbg !17
  br label %do.body, !dbg !18

do.body:                                          ; preds = %do.cond, %entry
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @.str, i64 0, i64 0)), !dbg !19
  %call1 = call i32 (i8*, ...) @__isoc99_scanf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.1, i64 0, i64 0), i8* %operator), !dbg !21
  %call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.2, i64 0, i64 0)), !dbg !22
  %call3 = call i32 (i8*, ...) @__isoc99_scanf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.3, i64 0, i64 0), i32* %firstNumber, i32* %secondNumber), !dbg !23
  %0 = load i8, i8* %operator, align 1, !dbg !24
  %conv = sext i8 %0 to i32, !dbg !24
  switch i32 %conv, label %sw.default [
    i32 43, label %sw.bb
    i32 45, label %sw.bb5
    i32 42, label %sw.bb7
    i32 47, label %sw.bb9
  ], !dbg !25

sw.bb:                                            ; preds = %do.body
  %1 = load i32, i32* %firstNumber, align 4, !dbg !26
  %2 = load i32, i32* %secondNumber, align 4, !dbg !28
  %3 = load i32, i32* %firstNumber, align 4, !dbg !29
  %4 = load i32, i32* %secondNumber, align 4, !dbg !30
  %add = add nsw i32 %3, %4, !dbg !31
  %call4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.4, i64 0, i64 0), i32 %1, i32 %2, i32 %add), !dbg !32
  br label %sw.epilog, !dbg !33

sw.bb5:                                           ; preds = %do.body
  %5 = load i32, i32* %firstNumber, align 4, !dbg !34
  %6 = load i32, i32* %secondNumber, align 4, !dbg !35
  %7 = load i32, i32* %firstNumber, align 4, !dbg !36
  %8 = load i32, i32* %secondNumber, align 4, !dbg !37
  %sub = sub nsw i32 %7, %8, !dbg !38
  %call6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.5, i64 0, i64 0), i32 %5, i32 %6, i32 %sub), !dbg !39
  br label %sw.epilog, !dbg !40

sw.bb7:                                           ; preds = %do.body
  %9 = load i32, i32* %firstNumber, align 4, !dbg !41
  %10 = load i32, i32* %secondNumber, align 4, !dbg !42
  %11 = load i32, i32* %firstNumber, align 4, !dbg !43
  %12 = load i32, i32* %secondNumber, align 4, !dbg !44
  %mul = mul nsw i32 %11, %12, !dbg !45
  %call8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.6, i64 0, i64 0), i32 %9, i32 %10, i32 %mul), !dbg !46
  br label %sw.epilog, !dbg !47

sw.bb9:                                           ; preds = %do.body
  %13 = load i32, i32* %firstNumber, align 4, !dbg !48
  %14 = load i32, i32* %secondNumber, align 4, !dbg !49
  %15 = load i32, i32* %firstNumber, align 4, !dbg !50
  %16 = load i32, i32* %secondNumber, align 4, !dbg !51
  %div = sdiv i32 %15, %16, !dbg !52
  %call10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.7, i64 0, i64 0), i32 %13, i32 %14, i32 %div), !dbg !53
  br label %sw.epilog, !dbg !54

sw.default:                                       ; preds = %do.body
  %call11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str.8, i64 0, i64 0)), !dbg !55
  br label %sw.epilog, !dbg !56

sw.epilog:                                        ; preds = %sw.default, %sw.bb9, %sw.bb7, %sw.bb5, %sw.bb
  br label %do.cond, !dbg !57

do.cond:                                          ; preds = %sw.epilog
  br i1 true, label %do.body, label %do.end, !dbg !57, !llvm.loop !58

do.end:                                           ; preds = %do.cond
  ret i32 0, !dbg !60
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare dso_local i32 @printf(i8*, ...) #2

declare dso_local i32 @__isoc99_scanf(i8*, ...) #2

attributes #0 = { noinline nounwind optnone uwtable "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #2 = { "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5}
!llvm.ident = !{!6}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "Ubuntu clang version 12.0.1-19ubuntu3", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "calc.c", directory: "/root/LLVMPlayground/part4_instrumentation/DivZeroInstrument/test")
!2 = !{}
!3 = !{i32 7, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{!"Ubuntu clang version 12.0.1-19ubuntu3"}
!7 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 2, type: !8, scopeLine: 2, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!8 = !DISubroutineType(types: !9)
!9 = !{!10}
!10 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!11 = !DILocalVariable(name: "operator", scope: !7, file: !1, line: 3, type: !12)
!12 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!13 = !DILocation(line: 3, column: 8, scope: !7)
!14 = !DILocalVariable(name: "firstNumber", scope: !7, file: !1, line: 4, type: !10)
!15 = !DILocation(line: 4, column: 7, scope: !7)
!16 = !DILocalVariable(name: "secondNumber", scope: !7, file: !1, line: 4, type: !10)
!17 = !DILocation(line: 4, column: 19, scope: !7)
!18 = !DILocation(line: 6, column: 3, scope: !7)
!19 = !DILocation(line: 7, column: 5, scope: !20)
!20 = distinct !DILexicalBlock(scope: !7, file: !1, line: 6, column: 6)
!21 = !DILocation(line: 8, column: 5, scope: !20)
!22 = !DILocation(line: 9, column: 5, scope: !20)
!23 = !DILocation(line: 10, column: 5, scope: !20)
!24 = !DILocation(line: 11, column: 13, scope: !20)
!25 = !DILocation(line: 11, column: 5, scope: !20)
!26 = !DILocation(line: 13, column: 33, scope: !27)
!27 = distinct !DILexicalBlock(scope: !20, file: !1, line: 11, column: 23)
!28 = !DILocation(line: 13, column: 46, scope: !27)
!29 = !DILocation(line: 13, column: 60, scope: !27)
!30 = !DILocation(line: 13, column: 74, scope: !27)
!31 = !DILocation(line: 13, column: 72, scope: !27)
!32 = !DILocation(line: 13, column: 9, scope: !27)
!33 = !DILocation(line: 14, column: 9, scope: !27)
!34 = !DILocation(line: 16, column: 33, scope: !27)
!35 = !DILocation(line: 16, column: 46, scope: !27)
!36 = !DILocation(line: 16, column: 60, scope: !27)
!37 = !DILocation(line: 16, column: 74, scope: !27)
!38 = !DILocation(line: 16, column: 72, scope: !27)
!39 = !DILocation(line: 16, column: 9, scope: !27)
!40 = !DILocation(line: 17, column: 9, scope: !27)
!41 = !DILocation(line: 19, column: 33, scope: !27)
!42 = !DILocation(line: 19, column: 46, scope: !27)
!43 = !DILocation(line: 19, column: 60, scope: !27)
!44 = !DILocation(line: 19, column: 74, scope: !27)
!45 = !DILocation(line: 19, column: 72, scope: !27)
!46 = !DILocation(line: 19, column: 9, scope: !27)
!47 = !DILocation(line: 20, column: 9, scope: !27)
!48 = !DILocation(line: 22, column: 33, scope: !27)
!49 = !DILocation(line: 22, column: 46, scope: !27)
!50 = !DILocation(line: 22, column: 60, scope: !27)
!51 = !DILocation(line: 22, column: 74, scope: !27)
!52 = !DILocation(line: 22, column: 72, scope: !27)
!53 = !DILocation(line: 22, column: 9, scope: !27)
!54 = !DILocation(line: 23, column: 9, scope: !27)
!55 = !DILocation(line: 25, column: 9, scope: !27)
!56 = !DILocation(line: 26, column: 5, scope: !27)
!57 = !DILocation(line: 27, column: 3, scope: !20)
!58 = distinct !{!58, !18, !59}
!59 = !DILocation(line: 27, column: 12, scope: !7)
!60 = !DILocation(line: 28, column: 3, scope: !7)
