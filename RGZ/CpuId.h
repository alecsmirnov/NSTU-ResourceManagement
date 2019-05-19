#ifndef CPUID_H
#define CPUID_H

#include <cstdint>
#include <intrin.h>

// Класс для работы с инструкцией CPUID
class CpuId {
public:
	CpuId() = default;

	// Проверка процессора на поддержку инструкций CPUID
	bool cpuIdCheck() const;

	// Вызов инстуркции с переданным параметром (уровнем)
	void init(uint32_t level);
	// Вызов инструкции с уровнем и подуровнем (результатом)
	void init(uint32_t level, uint32_t sublevel);

	// Получить значения регистров 
	const uint32_t& EAX() const;
	const uint32_t& EBX() const;
	const uint32_t& ECX() const;
	const uint32_t& EDX() const;

private:
	// Установить значения всех ресистров
	void setRegs(uint32_t eax, uint32_t ebx, uint32_t ecx, uint32_t edx);

public:
	// Количество регистров для работы с инструкцией
	static const uint8_t regs_count = 4;

private:
	// Регистры для работы с инструкцией
	uint32_t regs[regs_count] = { 0 };
};

#endif