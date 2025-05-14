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
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "alumno.h"
#include "config.h"

/* === Macros definitions ========================================================================================== */
#ifndef MAX_ALUMNOS_INSTANCIAS
#endif
/* === Private data type declarations ============================================================================== */

struct alumno_s {
    char nombre[20];    //!< Nombre del alumno
    char apellido[20];  //!< Apellido del alumno
    uint32_t documento; //!< Documento del alumno
#ifndef USO_MEMORIA_DINAMICA
    bool ocupado;
#endif
};

/* === Private function declarations =============================================================================== */
/**
 * @brief Reserva espacio de memoria para los alumnos
 *
 * @return alumno_t Referencia al alumno creado
 */
#ifndef USO_MEMORIA_DINAMICA
static alumno_t CrearInstancia();
#endif

/**
 * @brief Serializa una cadena de caracteres
 *
 * @param campo Nombre del campo
 * @param valor Valor del campo
 * @param buffer Cadena de caracteres donde se almacenara el resultado
 * @param disponibles Cantidad de espacios disponibles en la cadena
 * @return int Devuelve la cantidad de espacios que se usaron, o -1 si hubo un error
 */
static int SerializarCadena(char campo[], char valor[], char buffer[], uint32_t disponible);

/**
 * @brief Serializa un número
 *
 * @param campo Nombre del campo
 * @param valor Valor del campo
 * @param buffer  Cadena de caracteres donde se almacena el resultado
 * @param disponible Cantidad de espacios disponibles en la cadena
 * @return int Devuelve la cantidad de espacios que se usaron, o -1 si hubo un error
 */
static int SerializarNumero(char campo[], uint32_t valor, char buffer[], uint32_t disponible);

/* === Private variable definitions ================================================================================ */
#ifndef USO_MEMORIA_DINAMICA
static struct alumno_s instancias[ALUMNOS_MAX_INSTANCIAS] = {0};
#endif

/* === Public variable definitions ================================================================================= */

/* === Private function definitions ================================================================================ */

#ifndef USO_MEMORIA_DINAMICA
static alumno_t CrearInstancia() {
    alumno_t self = NULL;
    int i;
    for (i = 0; i < ALUMNOS_MAX_INSTANCIAS; i++) {
        if (!instancias[i].ocupado) {
            self = &instancias[i];
            self->ocupado = true;
            break;
        }
    }
    return self;
}
#endif

static int SerializarCadena(char campo[], char valor[], char buffer[], uint32_t disponible) {
    int resultado = snprintf(buffer, disponible, "\"%s\":\"%s\",", campo, valor);
    if (resultado < 0 || (uint32_t)resultado >= disponible) {
        return -1;
    }
    return resultado;
}

static int SerializarNumero(char campo[], uint32_t valor, char buffer[], uint32_t disponible) {
    int resultado = snprintf(buffer, disponible, "\"%s\":\"%u\",", campo, valor);
    if (resultado < 0 || (uint32_t)resultado >= disponible) {
        return -1;
    }
    return resultado;
}

/* === Public function implementation ============================================================================== */

alumno_t AlumnoCrear(char nombre[], char apellido[], uint32_t dni) {
#ifdef USO_MEMORIA_DINAMICA
    alumno_t self = malloc(sizeof(struct alumno_s));
#else
    alumno_t self = CrearInstancia();
#endif
    if (self != NULL) {
        strncpy(self->nombre, nombre, sizeof(self->nombre) - 1);
        strncpy(self->apellido, apellido, sizeof(self->apellido) - 1);
        self->nombre[sizeof(self->nombre) - 1] = '\0';
        self->apellido[sizeof(self->apellido) - 1] = '\0';
        self->documento = dni;
    }
    return self;
}

int AlumnoSerializar(alumno_t self, char buffer[], uint32_t size) {
    int escritos; // Cantidad de caracteres que fueron escritos
    int resultado;

    buffer[0] = '{';
    buffer++;
    escritos = 1;

    resultado = SerializarCadena("nombre", self->nombre, buffer, size - escritos);
    if (resultado < 0) {
        return -1;
    }

    escritos += resultado;
    buffer += resultado;

    resultado = SerializarCadena("apellido", self->apellido, buffer, size - escritos);
    if (resultado < 0) {
        return -1;
    }

    escritos += resultado;
    buffer += resultado;

    resultado = SerializarNumero("documento", self->documento, buffer, size - escritos);
    if (resultado < 0) {
        return -1;
    }

    escritos += resultado;
    buffer += resultado - 1; 

    buffer[0] = '}';
    buffer++;
    buffer[0] = '\0';

    escritos++;

    return escritos;
}

/* === End of documentation ======================================================================================== */
