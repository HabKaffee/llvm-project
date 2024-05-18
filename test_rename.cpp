// RUN: %clang++ -cc1 -load ./../../lab1/Babushkina_Anna/RenameIdentifiers/RenameIdentifiers.so -plugin rename-id -plugin-arg-rename-id oldVar -plugin-arg-rename-id oldVar test_rename.cpp %s 2>&1 | FileCheck %s
// RUN: %clang++ -cc1 -load ./../../lab1/Babushkina_Anna/RenameIdentifiers/RenameIdentifiers.so -plugin rename-id -plugin-arg-rename-id oldClass -plugin-arg-rename-id oldClass test_rename.cpp %s 2>&1 | FileCheck %s
// RUN: %clang++ -cc1 -load ./../../lab1/Babushkina_Anna/RenameIdentifiers/RenameIdentifiers.so -plugin rename-id -plugin-arg-rename-id func -plugin-arg-rename-id newfunc test_rename.cpp %s 2>&1 | FileCheck %s
// RUN: %clang++ -cc1 -load ./../../lab1/Babushkina_Anna/RenameIdentifiers/RenameIdentifiers.so -plugin rename-id -plugin-arg-rename-id l -plugin-arg-rename-id m_l test_rename.cpp %s 2>&1 | FileCheck %s

int func(int x) {
// CHECK: newfunc
	return x*x;
}

class oldClass {
// CHECK: oldClass
	int m_l = 0;
// CHECK: m_l
public:
	oldClass(int oldVar) {
// CHECK: oldClass()
// CHECK-NEXT: oldVar
		l = func(oldVar);
// CHECK: newfunc
// CHECK-NEXT: oldVar
	}
};

oldClass classfunc() {
	oldClass obj = oldClass(3);
	return obj;
}

int main(){
	int oldVar = 2;
// CHECK: oldVar
	int composedVar = oldVar * oldVar + 1 * func(oldVar);
	oldClass P(composedVar);
	int j = func(oldVar);
// CHECK: newfunc
// CHECK-NEXT: oldVar
}
