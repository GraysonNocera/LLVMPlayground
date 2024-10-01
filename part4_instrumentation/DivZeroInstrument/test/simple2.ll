; ModuleID = 'simple2.c'
source_filename = "simple2.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @f() #0 !dbg !7 {
entry:
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %z = alloca i32, align 4
  call void @llvm.dbg.declare(metadata i32* %x, metadata !10, metadata !DIExpression()), !dbg !12
  store i32 0, i32* %x, align 4, !dbg !12
  call void @llvm.dbg.declare(metadata i32* %y, metadata !13, metadata !DIExpression()), !dbg !14
  store i32 2, i32* %y, align 4, !dbg !14
  call void @llvm.dbg.declare(metadata i32* %z, metadata !15, metadata !DIExpression()), !dbg !16
  %0 = load i32, i32* %x, align 4, !dbg !17
  %cmp = icmp slt i32 %0, 1, !dbg !19
  br i1 %cmp, label %if.then, label %if.else, !dbg !20

if.then:                                          ; preds = %entry
  %1 = load i32, i32* %y, align 4, !dbg !21
  %2 = load i32, i32* %x, align 4, !dbg !23
  %div = sdiv i32 %1, %2, !dbg !24
  store i32 %div, i32* %z, align 4, !dbg !25
  br label %if.end, !dbg !26

if.else:                                          ; preds = %entry
  %3 = load i32, i32* %z, align 4, !dbg !27
  %4 = load i32, i32* %x, align 4, !dbg !29
  %div1 = sdiv i32 %3, %4, !dbg !30
  store i32 %div1, i32* %z, align 4, !dbg !31
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  ret void, !dbg !32
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 !dbg !33 {
entry:
  call void @f(), !dbg !36
  ret i32 0, !dbg !37
}

attributes #0 = { noinline nounwind optnone uwtable "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5}
!llvm.ident = !{!6}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "Ubuntu clang version 12.0.1-19ubuntu3", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "simple2.c", directory: "/root/LLVMPlayground/part4_instrumentation/DivZeroInstrument/test")
!2 = !{}
!3 = !{i32 7, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{!"Ubuntu clang version 12.0.1-19ubuntu3"}
!7 = distinct !DISubprogram(name: "f", scope: !1, file: !1, line: 1, type: !8, scopeLine: 1, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!8 = !DISubroutineType(types: !9)
!9 = !{null}
!10 = !DILocalVariable(name: "x", scope: !7, file: !1, line: 2, type: !11)
!11 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!12 = !DILocation(line: 2, column: 7, scope: !7)
!13 = !DILocalVariable(name: "y", scope: !7, file: !1, line: 3, type: !11)
!14 = !DILocation(line: 3, column: 7, scope: !7)
!15 = !DILocalVariable(name: "z", scope: !7, file: !1, line: 4, type: !11)
!16 = !DILocation(line: 4, column: 7, scope: !7)
!17 = !DILocation(line: 5, column: 7, scope: !18)
!18 = distinct !DILexicalBlock(scope: !7, file: !1, line: 5, column: 7)
!19 = !DILocation(line: 5, column: 9, scope: !18)
!20 = !DILocation(line: 5, column: 7, scope: !7)
!21 = !DILocation(line: 6, column: 9, scope: !22)
!22 = distinct !DILexicalBlock(scope: !18, file: !1, line: 5, column: 14)
!23 = !DILocation(line: 6, column: 13, scope: !22)
!24 = !DILocation(line: 6, column: 11, scope: !22)
!25 = !DILocation(line: 6, column: 7, scope: !22)
!26 = !DILocation(line: 7, column: 3, scope: !22)
!27 = !DILocation(line: 8, column: 9, scope: !28)
!28 = distinct !DILexicalBlock(scope: !18, file: !1, line: 7, column: 10)
!29 = !DILocation(line: 8, column: 13, scope: !28)
!30 = !DILocation(line: 8, column: 11, scope: !28)
!31 = !DILocation(line: 8, column: 7, scope: !28)
!32 = !DILocation(line: 10, column: 1, scope: !7)
!33 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 12, type: !34, scopeLine: 12, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!34 = !DISubroutineType(types: !35)
!35 = !{!11}
!36 = !DILocation(line: 13, column: 3, scope: !33)
!37 = !DILocation(line: 14, column: 1, scope: !33)
