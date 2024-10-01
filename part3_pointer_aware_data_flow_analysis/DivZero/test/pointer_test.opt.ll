; ModuleID = 'pointer_test.c'
source_filename = "pointer_test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @f(i32 %arg) #0 {
entry:
  %arg.addr = alloca i32, align 4
  %z = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %e = alloca i32, align 4
  %f = alloca i32, align 4
  %c = alloca i32*, align 8
  %d = alloca i32*, align 8
  %y = alloca i32, align 4
  %x = alloca i32, align 4
  %dub = alloca i32**, align 8
  store i32 %arg, i32* %arg.addr, align 4
  %0 = load i32, i32* %arg.addr, align 4
  store i32 %0, i32* %z, align 4
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  store i32 3, i32* %e, align 4
  store i32 4, i32* %f, align 4
  store i32* %a, i32** %c, align 8
  store i32* %a, i32** %d, align 8
  %1 = load i32*, i32** %c, align 8
  store i32 0, i32* %1, align 4
  %2 = load i32*, i32** %d, align 8
  store i32 4, i32* %2, align 4
  store i32* %b, i32** %d, align 8
  store i32* %b, i32** %c, align 8
  store i32* %e, i32** %c, align 8
  store i32* %f, i32** %c, align 8
  %3 = load i32*, i32** %c, align 8
  %4 = load i32, i32* %3, align 4
  %div = sdiv i32 1, %4
  store i32 %div, i32* %y, align 4
  %5 = load i32*, i32** %d, align 8
  %6 = load i32, i32* %5, align 4
  %div1 = sdiv i32 1, %6
  store i32 %div1, i32* %x, align 4
  %7 = load i32*, i32** %c, align 8
  %8 = load i32**, i32*** %dub, align 8
  store i32* %7, i32** %8, align 8
  %9 = load i32**, i32*** %dub, align 8
  %10 = load i32*, i32** %9, align 8
  store i32 0, i32* %10, align 4
  %11 = load i32, i32* %x, align 4
  ret i32 %11
}

attributes #0 = { noinline nounwind uwtable "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"Ubuntu clang version 12.0.1-19ubuntu3"}
