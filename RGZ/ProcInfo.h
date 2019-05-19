#ifndef PROCINFO_H
#define PROCINFO_H

#include <string>

#include "CpuId.h"
#include "Cache.h"

// Статический класс для получения информации о процессоре
class ProcInfo {
private:
	static const uint8_t L3_CACHE = 3;						// Идентификатор кэша 3-го уровня

	// Сдвиги информации о кэше:
	static const uint8_t CACHE_LEVEL 			   = 5;		// Уровень кэша
	static const uint8_t CACHE_STRING_SEGMENTATION = 12;	// Сегментирование строк	
	static const uint8_t CACHE_ASSOCIATIVITY 	   = 22;	// Пути ассоциативности

	static const uint32_t KB = 1024;						// Килобайт в байтах
	static const uint32_t MB = 1024 * KB;					// Мегабайт в килобайтах

	// Стандартный уровни инструкции процессора (модель процессора, кэша и т.д.) 
	enum class StandardLevel : uint32_t {
		LVL_0, LVL_1, LVL_2, LVL_3, LVL_4	
	};

	// Расширенные уровни инструкции процессора	(имя процессора и т.д.)
	enum class ExtendedLevel : uint32_t {			
		LVL_0 = 0x80000000,									
		LVL_1 = 0x80000001,
		LVL_2 = 0x80000002,
		LVL_3 = 0x80000003,
		LVL_4 = 0x80000004
	};

private:
	// Получить содержимое регистров в строковом представлении 
	static inline std::string fromUint32ToString(const CpuId& cpu_id) {
		std::string str;
		constexpr uint8_t reg_size = 4;

		str += std::string(reinterpret_cast<const char*>(&cpu_id.EAX()), reg_size);
		str += std::string(reinterpret_cast<const char*>(&cpu_id.EBX()), reg_size);
		str += std::string(reinterpret_cast<const char*>(&cpu_id.ECX()), reg_size);
		str += std::string(reinterpret_cast<const char*>(&cpu_id.EDX()), reg_size);

		return str;
	}

public:
	// Получить кэш 3-го уровня
	static Cache getL3Cache() {
		Cache cache;
		
		// Проверка процессора на поддержку инструкций CPUID
		if (CpuId cpu_id; cpu_id.cpuIdCheck()) {
			cpu_id.init(static_cast<uint32_t>(StandardLevel::LVL_0));

			// Проверка доступа ко второму уровню инструкции и выше
			if (auto aviable_lvl = cpu_id.EAX(); static_cast<uint32_t>(StandardLevel::LVL_2) <= aviable_lvl) {
				cpu_id.init(static_cast<uint32_t>(StandardLevel::LVL_2));

				const uint32_t* cache_descriptor = &cpu_id.EAX();
				constexpr uint8_t cache_table_size = 0xFF;

				// Поиск информации о кэше 3-го уровня в стандартной таблице описателей кэш-памяти
				for (uint8_t i = 0; i != cache_table_size && !cache.empty(); ++i)
					switch (cache_descriptor[i]) {
						case 0x22: cache = { 512  * KB, 64 }; break;
						case 0x23: cache = { 1    * MB, 64 }; break;
						case 0x25: cache = { 2    * MB, 64 }; break;
						case 0x29: cache = { 4    * MB, 64 }; break;
						case 0x46: cache = { 4    * MB, 64 }; break;
						case 0x47: cache = { 8    * MB, 64 }; break;
						case 0x49: cache = { 4    * MB, 64 }; break;
						case 0x4A: cache = { 3    * MB, 64 }; break;
						case 0x4B: cache = { 8	  * MB, 64 }; break;
						case 0x4C: cache = { 12   * MB, 64 }; break;
						case 0x4D: cache = { 16   * MB, 64 }; break;
						case 0xD0: cache = { 512  * KB, 64 }; break;
						case 0xD1: cache = { 1	  * MB, 64 }; break;
						case 0xD2: cache = { 2	  * MB, 64 }; break;
						case 0xD6: cache = { 1	  * MB, 64 }; break;
						case 0xD7: cache = { 2	  * MB, 64 }; break;
						case 0xD8: cache = { 4	  * MB, 64 }; break;
						case 0xDC: cache = { 1536 * KB, 64 }; break;
						case 0xDD: cache = { 3	  * MB, 64 }; break;
						case 0xDE: cache = { 6	  * MB, 64 }; break;
						case 0xE2: cache = { 2	  * MB, 64 }; break;
						case 0xE3: cache = { 4	  * MB, 64 }; break;
						case 0xE4: cache = { 8	  * MB, 64 }; break;
						case 0xEA: cache = { 12	  * MB, 64 }; break;
						case 0xEB: cache = { 18   * MB, 64 }; break;
						case 0xEC: cache = { 24   * MB, 64 }; break;
					}

				// Если в таблице описателей не найдена информация - проверяем досту к 4-у уровню инструкции
				if (cache.empty() && static_cast<uint32_t>(StandardLevel::LVL_4) <= aviable_lvl) {
					auto info_search = true;

					// Поиск по листу CPUID (для новых процессоров)
					for (uint8_t sublevel = 0; sublevel != static_cast<uint32_t>(StandardLevel::LVL_4) && info_search; ++sublevel) {
						cpu_id.init(static_cast<uint32_t>(StandardLevel::LVL_4), sublevel);

						// Если найденный кэш является кэшем 3-го уровня
						if (((cpu_id.EAX() >> CACHE_LEVEL) & 0x7) == L3_CACHE) {
							// Длина строки кэша
							auto line_size  = (cpu_id.EBX() & 0xFFF) + 1;
							// Размер физического сегментирования строки
							auto partitions = ((cpu_id.EBX() >> CACHE_STRING_SEGMENTATION) & 0x3FF) + 1;
							// Кол-во путей ассоциативности
							auto assoc_ways = ((cpu_id.EBX() >> CACHE_ASSOCIATIVITY) & 0x3FF) + 1;
							// Кол-во наборов
							auto sets_count = cpu_id.ECX() + 1;

							cache = { line_size * partitions * assoc_ways * sets_count, line_size };

							info_search = false;
						}
					}
				}
			}
		}

		return cache;
	}

	// Получить имя процессора 
	static std::string getProcName() {
		std::string proc_name;

		// Проверка процессора на поддержку инструкций CPUID
		if (CpuId cpu_id; cpu_id.cpuIdCheck()) {
			cpu_id.init(static_cast<uint32_t>(ExtendedLevel::LVL_2));
			proc_name += fromUint32ToString(cpu_id);

			cpu_id.init(static_cast<uint32_t>(ExtendedLevel::LVL_3));
			proc_name += fromUint32ToString(cpu_id);
		
			cpu_id.init(static_cast<uint32_t>(ExtendedLevel::LVL_4));
			proc_name += fromUint32ToString(cpu_id);
		}

		return proc_name;
	}
};

#endif