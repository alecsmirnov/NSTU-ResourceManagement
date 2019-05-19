#include "CpuId.h"

// Перечень идентификаторов регистров
enum Register { EAX, EBX, ECX, EDX };

// Проверка процессора на поддержку инструкций CPUID
bool CpuId::cpuIdCheck() const {
	bool cpuid_support = false;

	__asm {
		pushfd
		pop  eax
		mov  ebx, eax
		xor  eax, 0x00200000
		push eax
		popfd
		pushfd
		pop  eax
		cmp  eax, ebx
		jne CPUID_SUP
		CPUID_SUP: mov cpuid_support, 1
	}

	return cpuid_support;
}

// Вызов инстуркции с переданным параметром (уровнем)
void CpuId::init(uint32_t level) {
	int i_eax, i_ebx, i_ecx, i_edx;

	__asm {
		mov eax,   level
		xor ecx,   ecx
		cpuid
		mov i_eax, eax
		mov i_ebx, ebx
		mov i_ecx, ecx
		mov i_edx, edx
	}
	
	setRegs(i_eax, i_ebx, i_ecx, i_edx);
}

// Вызов инструкции с уровнем и подуровнем (результатом)
void CpuId::init(uint32_t level, uint32_t sublevel) {
	int i_eax, i_ebx, i_ecx, i_edx;

	__asm {
		mov eax,   level
		mov ecx,   sublevel
		cpuid
		mov i_eax, eax
		mov i_ebx, ebx
		mov i_ecx, ecx
		mov i_edx, edx
	}

	setRegs(i_eax, i_ebx, i_ecx, i_edx);
}

// Получить значение регистра EAX 
const uint32_t& CpuId::EAX() const {
	return regs[Register::EAX];
}

// Получить значение регистра EBX 
const uint32_t& CpuId::EBX() const {
	return regs[Register::EBX];
}

// Получить значение регистра ECX 
const uint32_t& CpuId::ECX() const {
	return regs[Register::ECX];
}

// Получить значение регистра EDX 
const uint32_t& CpuId::EDX() const {
	return regs[Register::EDX];
}

// Установить значения всех ресистров
void CpuId::setRegs(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx) {
	regs[Register::EAX] = eax;
	regs[Register::EBX] = ebx;
	regs[Register::ECX] = ecx;
	regs[Register::EDX] = edx;
}