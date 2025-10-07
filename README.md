[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/dBUisVyx)
[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-2972f46106e565e64193e422d61a12cf1da4916b45550586e14ef0a7c637dd04.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=20951062)
# Continuación del Problema - 2 - Vectores Dinámicos

## Problema: Sistema Integrado de Recepción y Despacho con Reordenamiento por Prioridad

### Definición del Problema a Resolver

La empresa "MegaEnvío" ha expandido su sistema logístico para gestionar no solo el despacho (salida), sino también la **recepción (entrada)** de paquetes. El sistema anterior (Cola FIFO de Despacho) sirve de base, pero ahora se requiere una **Pila (LIFO)** para rastrear los paquetes recibidos recientemente, y una lógica de **"Salto de Cola"** basada en la prioridad para manejar urgencias.

El sistema debe utilizar el mismo conjunto de **vectores paralelos dinámicos** para almacenar **todos** los paquetes (Recibidos, En Cola, Despachados), pero debe gestionar dos estructuras lógicas superpuestas usando estos datos:

  1.  **Pila de Recepción (LIFO):** Implementada con índices de los vectores paralelos, rastrea los últimos paquetes que han sido **recibidos**.
  2.  **Cola de Despacho (FIFO/Prioridad):** Utiliza un rango diferente de índices y gestiona el orden de salida.

El desafío es doble: **gestionar dos estructuras dinámicas (Pila y Cola) sobre el mismo bloque de memoria** y desarrollar una función de **reordenamiento por prioridad** que altere el orden lógico de la Cola sin mover físicamente los datos en la memoria.

### Estructuras Dinámicas Requeridas (Heredadas y Modificadas)

Se utilizarán los mismos **vectores paralelos dinámicos** de la base, con la adición de un nuevo estado: 'R' = Recibido (en la Pila).

-----

### Definición y Detalles del Proceso a Desarrollar

#### Estructuras Lógicas a Implementar

1.  **Vectores Paralelos (Base de Datos):** Mantienen la información de todos los paquetes. Su capacidad debe ser gestionada dinámicamente (`new[]`, `delete[]`, expansión manual).
2.  **Pila de Recepción (LIFO):**
      * Se utiliza un índice `top_pila` para el `PUSH` (Recepción) y `POP` (Transferencia a Cola).
      * Solo los paquetes con `Estado = 'R'` (Recibido) están en la Pila.
3.  **Cola de Despacho (FIFO con Salto):**
      * Se utilizan los índices `frente_cola` y `final_cola` para la gestión de `ENCOLAR` y `DESENCOLAR`.
      * Solo los paquetes con `Estado = 'E'` (En Cola) están en la Cola.

#### Proceso de Solución Detallado

1.  **RECEPCIÓN (PUSH a Pila):**
      * El usuario ingresa un nuevo paquete.
      * Se realiza el proceso de expansión dinámica de los vectores paralelos si es necesario.
      * El paquete se guarda en la primera posición disponible y su `Estado` se establece en **'R'**. El índice `top_pila` se actualiza.
2.  **PROCESAR RECEPCIÓN (POP de Pila y ENCOLAR en Cola):**
      * Se realiza un `POP` lógico de la Pila (se toma el paquete en `top_pila`).
      * Se cambia el `Estado` de ese paquete de **'R'** a **'E'** y se **ENCOLA** al final de la Cola de Despacho. El índice `top_pila` se decrementa y `final_cola` se incrementa.
3.  **DESPACHO con PRIORIDAD (DESENCOLAR y Reordenamiento):**
      * La función de despacho debe buscar el paquete en la Cola (`Estado = 'E'`) que cumpla con la regla de despacho:
          * **Prioridad 1 (Alta):** Siempre salta a la cabeza de la cola para ser el próximo despachado, *sin mover los datos*.
          * **Prioridad 2 y 3:** Se despachan en orden FIFO normal.
      * Para simular el "Salto de Cola", la función debe recorrer los índices entre `frente_cola` y `final_cola` y buscar el paquete con Prioridad 1. Si lo encuentra, su índice se convierte en el próximo a despachar. Después de despacharlo, el índice `frente_cola` se ajusta para continuar el FIFO normal.
4.  **Terminación Segura:** La liberación de memoria debe seguir siendo manual y total (`delete[]`) para todos los punteros asignados.

-----

### Requerimientos Funcionales y No Funcionales

#### Requisitos Funcionales

1.  **Gestión Dinámica de Capacidad:** Los vectores paralelos deben duplicar su capacidad cuando se llenen (implementación manual de "realloc").
2.  **Operación de Pila:** Implementar las operaciones **Recibir (PUSH)** y **Procesar (POP)**.
3.  **Operación de Cola:** Implementar las operaciones **Transferir (ENCOLAR)** y **Despachar (DESENCOLAR)**.
4.  **Lógica de Salto de Prioridad:** La función de despacho debe verificar si existe un paquete con **Prioridad 1** (`Alta`) en la cola (`Estado = 'E'`). Si existe, ese paquete debe ser el próximo en ser despachado, ignorando temporalmente la regla FIFO.
5.  **Reporte por Estado:** El sistema debe mostrar cuántos paquetes están en estado 'R' (Pila), 'E' (Cola) y 'D' (Despachado).

#### Requisitos No Funcionales

1.  **Exclusividad de Punteros:** Prohibido el uso de `std::vector`, `std::queue`, `std::stack`, o `std::string`.
2.  **Manejo Manual de Memoria:** Uso exclusivo de `new[]` y `delete[]`.
3.  **Eficiencia:** El reordenamiento por prioridad debe realizarse por búsqueda lógica de índices, **sin mover físicamente** los datos de los paquetes en la memoria.

-----

## 04\. Ejemplo de Entradas y Salidas del Problema

### Escenario de Prueba (Demostrando Salto de Prioridad)

1.  Paquete 101, Destino: NY, Prioridad: 2 (Media) -\> **ENCOLADO (FIFO)**
2.  Paquete 102, Destino: LA, Prioridad: 3 (Baja) -\> **ENCOLADO (FIFO)**
3.  Paquete 103, Destino: TX, Prioridad: 1 (Alta) -\> **ENCOLADO (SALTA)**

### Salida Esperada del Sistema

```
--- MegaEnvío: Sistema Integrado con Prioridad ---

Inicializando sistema con capacidad para 50 paquetes...

--- Menú Principal ---
1. Recibir Nuevo Paquete (PUSH)
2. Procesar Recepción (POP y ENCOLAR)
3. Despachar (DESENCOLAR con Prioridad)
4. Reporte de Estados
5. Salir (Liberar Memoria)

Opción seleccionada: 1
Ingrese Pkg ID: 201, Peso: 4.5, Destino: Boston, Prioridad: 2
Paquete 201 Recibido. Estado: 'R'. Pila.top_pila = 0.

Opción seleccionada: 2
POP de Pila: Pkg 201 transferido a Cola. Estado: 'E'. frente_cola=0, final_cola=0.

Opción seleccionada: 1
Ingrese Pkg ID: 202, Peso: 1.2, Destino: Chicago, Prioridad: 1
Paquete 202 Recibido. Estado: 'R'. Pila.top_pila = 1.

Opción seleccionada: 2
POP de Pila: Pkg 202 transferido a Cola. Estado: 'E'. frente_cola=0, final_cola=1.
(Cola: [201, 202])

Opción seleccionada: 3
Analizando Cola... Detectada ALTA PRIORIDAD.
DESPACHADO: Paquete 202 (Prioridad 1). Estado: 'D'.
frente_cola se mantiene en 0 (esperando turno FIFO original).
(El siguiente despacho debe ser 201)
```