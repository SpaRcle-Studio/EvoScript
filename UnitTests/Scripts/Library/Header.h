//
// Created by Evo Script code generator on Mon Jul 19 17:57:27 2021 | Author - Monika
//

#ifndef EVOSCRIPTLIB_HEADER_H
#define EVOSCRIPTLIB_HEADER_H

#include "Standard/Addresses.h"
#include "string"

class A {
public:
	int a;
	std::string lol;
	std::string lol2;
public:
	void PrintA() {
		void* voidPtr = g_methodPointers[0];
		typedef void (A::*ClassPtr)();
		auto origPtr = *reinterpret_cast<ClassPtr*>(&voidPtr);
		return (*this.*origPtr)();
	}
	virtual void OverPrintA() {
		void* voidPtr = g_methodPointers[1];
		typedef void (A::*ClassPtr)();
		auto origPtr = *reinterpret_cast<ClassPtr*>(&voidPtr);
		return (*this.*origPtr)();
	}
	virtual void VirtualA() {
		void* voidPtr = g_methodPointers[2];
		typedef void (A::*ClassPtr)();
		auto origPtr = *reinterpret_cast<ClassPtr*>(&voidPtr);
		return (*this.*origPtr)();
	}
};

class B {
public:
	int b;
	std::string str;
public:
	void PrintB() {
		void* voidPtr = g_methodPointers[3];
		typedef void (B::*ClassPtr)();
		auto origPtr = *reinterpret_cast<ClassPtr*>(&voidPtr);
		return (*this.*origPtr)();
	}
	virtual void OverPrintB() {
		void* voidPtr = g_methodPointers[4];
		typedef void (B::*ClassPtr)();
		auto origPtr = *reinterpret_cast<ClassPtr*>(&voidPtr);
		return (*this.*origPtr)();
	}
};

class C : public A, public B {
public:
	int c;
public:
	void PrintC() {
		void* voidPtr = g_methodPointers[5];
		typedef void (A::*ClassPtr)();
		auto origPtr = *reinterpret_cast<ClassPtr*>(&voidPtr);
		return (*this.*origPtr)();
	}
	void OverPrintA() override {
		void* voidPtr = g_methodPointers[6];
		typedef void (A::*ClassPtr)();
		auto origPtr = *reinterpret_cast<ClassPtr*>(&voidPtr);
		return (*this.*origPtr)();
	}
	void OverPrintB() override {
		void* voidPtr = g_methodPointers[7];
		typedef void (B::*ClassPtr)();
		auto origPtr = *reinterpret_cast<ClassPtr*>(&voidPtr);
		return (*this.*origPtr)();
	}
};

#endif