/*********************************************************************************************************************
Copyright (c) 2025, María Ayelén Vega Caro <ayelenvegacaro@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*********************************************************************************************************************/

/** @file alumno.c
 ** @brief Código fuente del módulo para gestión de alumnos
 **/

/* === Headers files inclusions ==================================================================================== */
#include <stdio.h>
#include "alumno.h"

/* === Macros definitions ========================================================================================== */

/* === Private data type declarations ============================================================================== */

/* === Private function declarations =============================================================================== */
/**
 * @brief Serializa una cadena de caracteres
 *
 * @param campo Nombre del campo
 * @param valor Valor del campo
 * @param buffer Cadena de caracteres donde se almacenara el resultado
 * @param disponibles Cantidad de espacios disponibles en la cadena
 * @return int Devuelve la cantidad de espacios que se usaron, o -1 si hubo un error
 */
static int SerializarCadena(const char campo[], const char valor[], char buffer[], uint32_t disponible);

/**
 * @brief Serializa un número
 *
 * @param campo Nombre del campo
 * @param valor Valor del campo
 * @param buffer  Cadena de caracteres donde se almacena el resultado
 * @param disponible Cantidad de espacios disponibles en la cadena
 * @return int Devuelve la cantidad de espacios que se usaron, o -1 si hubo un error
 */
static int SerializarNumero(const char campo[], uint32_t valor, char buffer[], uint32_t disponible);

/* === Private variable definitions ================================================================================ */

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */
static int SerializarCadena(const char campo[], const char valor[], char buffer[], uint32_t disponible) {
    int resultado = snprintf(buffer, disponible, "\"%s\":\"%s\",", campo, valor);
    if (resultado < 0 || resultado >= disponible) {
        return -1;
    }
    return resultado;
}

static int SerializarNumero(const char campo[], uint32_t valor, char buffer[], uint32_t disponible) {
    int resultado = snprintf(buffer, disponible, "\"%s\":\"%u\",", campo, valor);
    if (resultado < 0 || resultado >= disponible) {
        return -1;
    }
    return resultado;
}

/* === Public function implementation ============================================================================== */

int Serializar(alumno_t alumno, char buffer[], uint32_t size) {
    int escritos; // Cantidad de caracteres que fueron escritos
    int resultado;

    buffer[0] = '{';
    buffer++;
    escritos = 1;

    resultado = SerializarCadena("nombre", alumno->nombre, buffer, size - escritos);
    if (resultado < 0) {
        return -1;
    }

    escritos += resultado;
    buffer += resultado;

    resultado = SerializarCadena("apellido", alumno->apellido, buffer, size - escritos);
    if (resultado < 0) {
        return -1;
    }

    escritos += resultado;
    buffer +=  resultado;

    resultado = SerializarNumero("documento", alumno->documento, buffer, size - escritos);
    if (resultado < 0) {
        return -1;
    }

    escritos += resultado;
    buffer += resultado - 1; // Porque quiero pisar la ultima coma

    buffer[0] = '}';
    buffer++;
    buffer[0] = '\0';

    escritos++;

    return escritos;
}

/* === End of documentation ======================================================================================== */
