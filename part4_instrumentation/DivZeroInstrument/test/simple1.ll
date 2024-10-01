; ModuleID = 'simple1.c'
source_filename = "simple1.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 %argc, i8** %argv) #0 !dbg !7 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %z = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  store i32 %argc, i32* %argc.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %argc.addr, metadata !14, metadata !DIExpression()), !dbg !15
  store i8** %argv, i8*** %argv.addr, align 8
  call void @llvm.dbg.declare(metadata i8*** %argv.addr, metadata !16, metadata !DIExpression()), !dbg !17
  %0 = load i32, i32* %argc.addr, align 4, !dbg !18
  %cmp = icmp sgt i32 %0, 2, !dbg !20
  br i1 %cmp, label %if.then, label %if.end, !dbg !21

if.then:                                          ; preds = %entry
  call void @llvm.dbg.declare(metadata i32* %x, metadata !22, metadata !DIExpression()), !dbg !24
  store i32 0, i32* %x, align 4, !dbg !24
  call void @llvm.dbg.declare(metadata i32* %y, metadata !25, metadata !DIExpression()), !dbg !26
  %1 = load i32, i32* %x, align 4, !dbg !27
  store i32 %1, i32* %y, align 4, !dbg !26
  call void @llvm.dbg.declare(metadata i32* %z, metadata !28, metadata !DIExpression()), !dbg !29
  %2 = load i32, i32* %y, align 4, !dbg !30
  %3 = load i32, i32* %x, align 4, !dbg !31
  %div = sdiv i32 %2, %3, !dbg !32
  store i32 %div, i32* %z, align 4, !dbg !29
  br label %if.end, !dbg !33

if.end:                                           ; preds = %if.then, %entry
  ret i32 0, !dbg !34
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

attributes #0 = { noinline nounwind optnone uwtable "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5}
!llvm.ident = !{!6}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "Ubuntu clang version 12.0.1-19ubuntu3", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "simple1.c", directory: "/root/LLVMPlayground/part4_instrumentation/DivZeroInstrument/test")
!2 = !{}
!3 = !{i32 7, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{!"Ubuntu clang version 12.0.1-19ubuntu3"}
!7 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 1, type: !8, scopeLine: 1, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!8 = !DISubroutineType(types: !9)
!9 = !{!10, !10, !11}
!10 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!11 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !12, size: 64)
!12 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !13, size: 64)
!13 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!14 = !DILocalVariable(name: "argc", arg: 1, scope: !7, file: !1, line: 1, type: !10)
!15 = !DILocation(line: 1, column: 14, scope: !7)
!16 = !DILocalVariable(name: "argv", arg: 2, scope: !7, file: !1, line: 1, type: !11)
!17 = !DILocation(line: 1, column: 27, scope: !7)
!18 = !DILocation(line: 2, column: 7, scope: !19)
!19 = distinct !DILexicalBlock(scope: !7, file: !1, line: 2, column: 7)
!20 = !DILocation(line: 2, column: 12, scope: !19)
!21 = !DILocation(line: 2, column: 7, scope: !7)
!22 = !DILocalVariable(name: "x", scope: !23, file: !1, line: 3, type: !10)
!23 = distinct !DILexicalBlock(scope: !19, file: !1, line: 2, column: 17)
!24 = !DILocation(line: 3, column: 9, scope: !23)
!25 = !DILocalVariable(name: "y", scope: !23, file: !1, line: 4, type: !10)
!26 = !DILocation(line: 4, column: 9, scope: !23)
!27 = !DILocation(line: 4, column: 13, scope: !23)
!28 = !DILocalVariable(name: "z", scope: !23, file: !1, line: 5, type: !10)
!29 = !DILocation(line: 5, column: 9, scope: !23)
!30 = !DILocation(line: 5, column: 13, scope: !23)
!31 = !DILocation(line: 5, column: 17, scope: !23)
!32 = !DILocation(line: 5, column: 15, scope: !23)
!33 = !DILocation(line: 6, column: 3, scope: !23)
!34 = !DILocation(line: 7, column: 3, scope: !7)
