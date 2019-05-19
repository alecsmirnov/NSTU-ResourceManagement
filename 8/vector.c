#include "vector.h"

#include <stdlib.h>

#define GROWN_COEFFICIENT 1.5       // Коэффициент роса вектора (соответствует STL)

// Инициализация вектора и его начальной вместимости
void vectorInit(Vector* V, uint8_t capacity) {
    V->size = 0;
    V->capacity = capacity;

    V->capacity ? (V->data = (Elem*)malloc(sizeof(Elem) * V->capacity)) :
                  (V->data = NULL);
}

// Добавить элемент в конец вектора
void vectorAdd(Vector* V, Elem E) {
    if (V->size == V->capacity) {
        V->capacity *= GROWN_COEFFICIENT;
        if (V->size == V->capacity)
            ++V->capacity;

        V->data = (Elem*)realloc(V->data, sizeof(Elem) * V->capacity);
    }

	vectorElemAssign(&V->data[V->size++], E);
}

// Удалить хранимые данные вектор
void vectorDelete(Vector* V) {
    free(V->data);
	vectorInit(V, 0);
}
