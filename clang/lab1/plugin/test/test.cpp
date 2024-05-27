// RUN: %clang_cc1 -emit-llvm -o - %s | opt -load /plugin/build/AlwaysInlineNoConditionPass.so -passes=always-inline-no-condition -S | FileCheck %s


// CHECK:; Function Attrs: alwaysinline mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: readwrite) uwtable
// CHECK-NEXT:define dso_local void @_Z10inlineFuncRi(ptr nocapture noundef nonnull align 4 dereferenceable(4) %0) local_unnamed_addr #0 {

void inlineFunc(int &var) {
    ++var;
}


// CHECK:; Function Attrs: mustprogress nofree noinline norecurse nosync nounwind willreturn memory(none) uwtable
// CHECK-NEXT:define dso_local noundef i1 @_Z15notInlineFunc_1i(i32 noundef %0) local_unnamed_addr #1 {

bool notInlineFunc_1(int var) {
    if (var == 2 || var == 22) {
        return false;
    } else {
        return true;
    }
}


// CHECK:; Function Attrs: mustprogress nofree noinline norecurse nosync nounwind willreturn memory(none) uwtable
// CHECK-NEXT:define dso_local noundef i32 @_Z15notInlineFunc_2b(i1 noundef %0) local_unnamed_addr #1 {

int notInlineFunc_2(const bool var) {
    int a = var ? 3 : 0;
    return a;
}


// CHECK:; Function Attrs: mustprogress nofree noinline norecurse nosync nounwind willreturn memory(none) uwtable
// CHECK-NEXT:define dso_local noundef i32 @_Z15notInlineFunc_3i(i32 noundef %0) local_unnamed_addr #1 {

int notInlineFunc_3(int var) {
    switch (var) {
        case 1:
           return true;
        default:
            return false;
    }
}


// CHECK:; Function Attrs: alwaysinline mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
// CHECK-NEXT:define dso_local noundef i32 @main() local_unnamed_addr #2 {

int main() {
    int a = 4;
    inlineFunc(a);
    notInlineFunc_1(12);
    notInlineFunc_2(true);
    notInlineFunc_3(a);
}

// CHECK:attributes #0 = { alwaysinline mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: readwrite) uwtable "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fp-armv8,+neon,+outline-atomics,+v8a,-fmv" }
// CHECK-NEXT:attributes #1 = { mustprogress nofree noinline norecurse nosync nounwind willreturn memory(none) uwtable "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fp-armv8,+neon,+outline-atomics,+v8a,-fmv" }
// CHECK-NEXT:attributes #2 = { alwaysinline mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+fp-armv8,+neon,+outline-atomics,+v8a,-fmv" }
