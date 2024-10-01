; ModuleID = 'branch4.c'
source_filename = "branch4.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local void @f() #0 {
entry:
  %in = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %out = alloca i32, align 4
  %call = call i32 @getchar()
  store i32 %call, i32* %in, align 4
  store i32 10, i32* %a, align 4
  store i32 2, i32* %b, align 4
  %0 = load i32, i32* %in, align 4
  %cmp = icmp sgt i32 %0, 0
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  store i32 0, i32* %b, align 4
  br label %if.end4

if.else:                                          ; preds = %entry
  %1 = load i32, i32* %in, align 4
  %cmp1 = icmp eq i32 %1, 0
  br i1 %cmp1, label %if.then2, label %if.else3

if.then2:                                         ; preds = %if.else
  %2 = load i32, i32* %b, align 4
  %sub = sub i32 2, %2
  store i32 %sub, i32* %b, align 4
  br label %if.end

if.else3:                                         ; preds = %if.else
  %3 = load i32, i32* %b, align 4
  %add = add i32 -2, %3
  store i32 %add, i32* %b, align 4
  br label %if.end

if.end:                                           ; preds = %if.else3, %if.then2
  br label %if.end4

if.end4:                                          ; preds = %if.end, %if.then
  %4 = load i32, i32* %a, align 4
  %5 = load i32, i32* %b, align 4
  %div = udiv i32 %4, %5
  store i32 %div, i32* %out, align 4
  ret void
}

declare dso_local i32 @getchar() #1

attributes #0 = { noinline nounwind uwtable "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"Ubuntu clang version 12.0.1-19ubuntu3"}
