#include <memory>
#include <iostream>

#include "Placement_new.h"

using namespace std;

class MyClass
{
public:
	MyClass()
	{
		cout << "ctor\n";
	}
	~MyClass()
	{
		cout << "dtor\n";
	}
	MyClass(int value) : X_(value) {
		cout << "ctor with value\n";;
	}
	MyClass(const MyClass& other) : X_(other.X_) {
		cout << "copy ctor\n";
	}
private:
	int X_;
	MyClass& operator=(const MyClass& other) = delete;
};

void simple_int_placement_new()
{
	cout << "simple_int_placement_new" << endl;
	int a = 1;
	int* ptr = ::new(&a) int;
	(*ptr)++;
	cout << a << endl;
}

void simple_array_placement_new()
{
	cout << "simple_array_placement_new" << endl;
	int a[10]{1,2,3,4,5,6,7,8,9,10};
	int* ptr = ::new(a) int[10];
	for (int i = 0; i < 10; ++i) cout << ptr[i] << endl;
}

void simple_array_dynamic_placement_new()
{
	cout << "simple_array_dynamic_placement_new" << endl;
	int *a = new int[10]{ 1,2,3,4,5,6,7,8,9,10 };
	int* ptr = ::new (a) int[10];
	for (int i = 0; i < 10; ++i) cout << ptr[i] << endl;
	delete[] a;
}

// Examle of using arrays like memory place
void simple_stack_dynamic_placement_new()
{
	cout << "simple_stack_dynamic_placement_new" << endl;
	int arr_stack[5]{ 1,2,3,4,5 };
	int* arr_dynamic = new int[5]{ 1,2,3,4,5 };

	int* ptr1 = ::new(arr_stack) int[5];
	int* ptr2 = ::new(arr_dynamic) int[5];

	delete[] ptr2;
	// delete[] ptr1; -- error, can't free stack array
}

void simple_buffer_placement_new()
{
	cout << "simple_buffer_placement_new" << endl;
	const int N = 10;
	const int BUFF_SIZE = N * sizeof(double);

	char buffer[BUFF_SIZE];

	double* ptr = ::new (buffer) double[N];

	double value = 0.3;
	for (int i = 0; i < N; ++i)
	{
		ptr[i] = value;
		value = value + 0.3;
	}
	for (int i = 0; i < N; ++i) cout << ptr[i] << endl;
}

// Never use code like this
void simple_struct_problem_placement_new()
{
	cout << "simple_struct_problem_placement_new" << endl;
	{
		MyClass obj; //ctor
		::new(&obj) MyClass; //ctor
	}
	// only one destructor
	cout << endl;
}

// correct using whereas above
void simple_struct_placement_new()
{
	cout << "simple_struct_placement_new" << endl;
	{
		MyClass* ptr = (MyClass*)operator new(sizeof(MyClass));
		MyClass* ptr_new = new(ptr) MyClass;

		ptr->~MyClass();
		operator delete(ptr);
	}
	cout << endl;
}

void func_notation_placement_new()
{
	cout << "func_notation_placement_new" << endl;
	int a;
	::new(&a) int(10);
	cout << a << endl;

	int *ptr = (int*)operator new(sizeof(int));
	ptr = new(ptr) int(33);
	cout << *ptr << endl;
	operator delete(ptr);
}

// only for c++11 and older
void func_notation_array_placement_new()
{
	cout << "func_notation_array_placement_new" << endl;
	int a[10];
	::new(a) int[10]{ 1,2,3,4,5,6,7,8,9,10 };
	for (int i = 0; i < 10; ++i) cout << a[i] << endl;
}

void struct_ccopy_placement_new()
{
	cout << "struct_ccopy_placement_new" << endl;
	MyClass* ptr = (MyClass*)operator new(sizeof(MyClass));
	MyClass* obj = new (ptr) MyClass(7);
	obj->~MyClass();
	operator delete(ptr);
}

void unusual_placement_new()
{
	cout << "unusual_placement_new" << endl;
	{
		int a = 234;
		int* p = new int;
		int* ptr = new (p) int(a);
		(*p)++;

		cout << *p << '\n';
		cout << a << '\n';
	}
	{
		const int N = 5;
		int a[N] = { 1,2,3,4,5 };

		int* p = (int*)operator new(5 * sizeof(int));
		for (int i = 0; i < 5; i++) new(p + i) int(a[i]);

		for (int i = 0; i < 5; i++) cout << a[i] << '\t';
		operator delete(p);
	}
}

void tets_placement_new()
{
	simple_int_placement_new();
	simple_array_placement_new();
	simple_array_dynamic_placement_new();
	simple_stack_dynamic_placement_new();
	simple_buffer_placement_new();
	simple_struct_problem_placement_new();
	simple_struct_placement_new();
	func_notation_placement_new();
	func_notation_array_placement_new();
	struct_ccopy_placement_new();
	unusual_placement_new();
}
