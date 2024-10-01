; ModuleID = 'simple9.c'
source_filename = "simple9.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 !dbg !7 {
entry:
  %retval = alloca i32, align 4
  %i = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata i32* %i, metadata !11, metadata !DIExpression()), !dbg !12
  call void @llvm.dbg.declare(metadata i32* %a, metadata !13, metadata !DIExpression()), !dbg !14
  store i32 0, i32* %a, align 4, !dbg !14
  call void @llvm.dbg.declare(metadata i32* %b, metadata !15, metadata !DIExpression()), !dbg !16
  %call = call i32 @getchar(), !dbg !17
  store i32 %call, i32* %b, align 4, !dbg !16
  store i32 10000, i32* %i, align 4, !dbg !18
  br label %for.cond, !dbg !20

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, i32* %i, align 4, !dbg !21
  %cmp = icmp sgt i32 %0, 0, !dbg !23
  br i1 %cmp, label %for.body, label %for.end, !dbg !24

for.body:                                         ; preds = %for.cond
  %1 = load i32, i32* %b, align 4, !dbg !25
  %2 = load i32, i32* %b, align 4, !dbg !26
  %mul = mul nsw i32 %1, %2, !dbg !27
  %3 = load i32, i32* %i, align 4, !dbg !28
  %4 = load i32, i32* %b, align 4, !dbg !29
  %sub = sub nsw i32 %3, %4, !dbg !30
  %div = sdiv i32 %mul, %sub, !dbg !31
  %call1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %div), !dbg !32
  br label %for.inc, !dbg !32

for.inc:                                          ; preds = %for.body
  %5 = load i32, i32* %i, align 4, !dbg !33
  %dec = add nsw i32 %5, -1, !dbg !33
  store i32 %dec, i32* %i, align 4, !dbg !33
  br label %for.cond, !dbg !34, !llvm.loop !35

for.end:                                          ; preds = %for.cond
  ret i32 0, !dbg !38
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare dso_local i32 @getchar() #2

declare dso_local i32 @printf(i8*, ...) #2

attributes #0 = { noinline nounwind optnone uwtable "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #2 = { "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5}
!llvm.ident = !{!6}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "Ubuntu clang version 12.0.1-19ubuntu3", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "simple9.c", directory: "/root/LLVMPlayground/part4_instrumentation/DivZeroInstrument/test")
!2 = !{}
!3 = !{i32 7, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{!"Ubuntu clang version 12.0.1-19ubuntu3"}
!7 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 3, type: !8, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!8 = !DISubroutineType(types: !9)
!9 = !{!10}
!10 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!11 = !DILocalVariable(name: "i", scope: !7, file: !1, line: 4, type: !10)
!12 = !DILocation(line: 4, column: 7, scope: !7)
!13 = !DILocalVariable(name: "a", scope: !7, file: !1, line: 4, type: !10)
!14 = !DILocation(line: 4, column: 10, scope: !7)
!15 = !DILocalVariable(name: "b", scope: !7, file: !1, line: 4, type: !10)
!16 = !DILocation(line: 4, column: 17, scope: !7)
!17 = !DILocation(line: 4, column: 21, scope: !7)
!18 = !DILocation(line: 5, column: 10, scope: !19)
!19 = distinct !DILexicalBlock(scope: !7, file: !1, line: 5, column: 3)
!20 = !DILocation(line: 5, column: 8, scope: !19)
!21 = !DILocation(line: 5, column: 19, scope: !22)
!22 = distinct !DILexicalBlock(scope: !19, file: !1, line: 5, column: 3)
!23 = !DILocation(line: 5, column: 21, scope: !22)
!24 = !DILocation(line: 5, column: 3, scope: !19)
!25 = !DILocation(line: 6, column: 20, scope: !22)
!26 = !DILocation(line: 6, column: 24, scope: !22)
!27 = !DILocation(line: 6, column: 22, scope: !22)
!28 = !DILocation(line: 6, column: 29, scope: !22)
!29 = !DILocation(line: 6, column: 33, scope: !22)
!30 = !DILocation(line: 6, column: 31, scope: !22)
!31 = !DILocation(line: 6, column: 26, scope: !22)
!32 = !DILocation(line: 6, column: 5, scope: !22)
!33 = !DILocation(line: 5, column: 27, scope: !22)
!34 = !DILocation(line: 5, column: 3, scope: !22)
!35 = distinct !{!35, !24, !36, !37}
!36 = !DILocation(line: 6, column: 35, scope: !19)
!37 = !{!"llvm.loop.mustprogress"}
!38 = !DILocation(line: 7, column: 3, scope: !7)
