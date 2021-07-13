#include <iostream>
#include <string>
#include <vector>
#include "mod_base/FunctionCommand.h"
#include "mod_base/CommandHandle.h"
#include "mod_dummy/DummyHandle.h"
using namespace std;

typedef vector<string> strv;
typedef vector<string>* strvp;
typedef string (*FP)(const strv);

string test1(const strv args);
string test2(const strv args);

void exec(FP fp, strv args);

int main(int argc, char **argv) {
    std::cout << "Hello, world!" << std::endl;
	FP fun;
	fun = &test1;
	
	strv data;
	data.push_back("Zero");
	data.push_back("One");
	data.push_back("Two");
	data.push_back("Three");
	
	(*fun)(data);
	fun = &test2;
	exec(fun, data);
	
	cout << "\nOk.. now for function command..." << endl;
	FunctionCommand fc1(fun);
	FunctionCommand fc2(&test2);
	
	fc1.execute(data);
	
	CommandHandle ch;
	string t("test"),
		t2("test zero one two three"),
		t3("draw 1,2");
	ch.add_command(t, fc1);
	ch.execute(t2);
	
	//Load module syntax
	DummyHandle dh(ch);
	
	cout << ch.execute(t3) << endl;
	
	return 0;
}

string test1(const strv args) {
	cout << args[1] <<endl;
	return "Woohoo!";
}

string test2(const strv args) {
	cout << args[2] << endl;
	return "Woohoo!2";
}

void exec(FP fp, strv args) {
	(*fp)(args);
}