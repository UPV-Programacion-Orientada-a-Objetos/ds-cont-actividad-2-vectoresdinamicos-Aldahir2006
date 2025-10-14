#include <iostream>
#include <cstring>
#include <limits>

struct SistemaMegaEnvio {
    int* pkgID;
    float* peso;
    char** destino;
    int* prioridad;
    char* estado; // 'R' recibidos, 'E' en cola, 'D' despachados
    
    int capacidad;
    int tope_pila;    // índice top pila recepción (-1 vacío)
    int frente_cola;  // índice frente cola despacho
    int final_cola;   // índice final cola despacho
    
    SistemaMegaEnvio(int cap) {
        capacidad = cap;
        pkgID = new int[capacidad];
        peso = new float[capacidad];
        destino = new char*[capacidad];
        for (int i = 0; i < capacidad; ++i)
            destino[i] = new char[50]; 
        prioridad = new int[capacidad];
        estado = new char[capacidad];
        tope_pila = -1;
        frente_cola = 0;
        final_cola = -1;
    }
    
    ~SistemaMegaEnvio() {
        for (int i=0; i<capacidad; ++i)
            delete[] destino[i];
        delete[] destino;
        delete[] pkgID;
        delete[] peso;
        delete[] prioridad;
        delete[] estado;
    }
    
    void expandir() {
        int nueva_cap = capacidad * 2;
        int* new_pkgID = new int[nueva_cap];
        float* new_peso = new float[nueva_cap];
        char** new_destino = new char*[nueva_cap];
        for (int i=0; i<nueva_cap; ++i)
            new_destino[i] = new char[50];
        int* new_prioridad = new int[nueva_cap];
        char* new_estado = new char[nueva_cap];
        
        // Copiar datos existentes
        for (int i=0; i<capacidad; ++i) {
            new_pkgID[i] = pkgID[i];
            new_peso[i] = peso[i];
            std::strcpy(new_destino[i], destino[i]);
            new_prioridad[i] = prioridad[i];
            new_estado[i] = estado[i];
        }
        for (int i=capacidad; i<nueva_cap; ++i) {         // Inicializar nuevas posiciones (opcional)
            new_estado[i] = 'D'; // marcar como despachado para liberar espacio lógico
        }
        
        for (int i=0; i<capacidad; ++i)         // Liberar memoria vieja
            delete[] destino[i];
        delete[] destino;
        delete[] pkgID;
        delete[] peso;
        delete[] prioridad;
        delete[] estado;
        
        pkgID = new_pkgID;         // Actualizar punteros y capacidad
        peso = new_peso;
        destino = new_destino;
        prioridad = new_prioridad;
        estado = new_estado;
        capacidad = nueva_cap;
    }
    
    int contarLibres() {
        int cont = 0;
        for (int i = 0; i < capacidad; i++) {
            if (estado[i] == 'D') cont++;
        }
        return cont;
    }
    
    int posicionLibre() {
        for (int i = 0; i < capacidad; i++) {
            if (estado[i] == 'D') return i;
        }
        return -1;
    }
    
    void recibirPaquete() {
        if (contarLibres() == 0)
            expandir();
        
        int pos = posicionLibre();
        if (pos == -1) {
            std::cout << "Error: no space available despite resize.\n";
            return;
        }
        
        std::cout << "Ingrese Pkg ID: ";
        int id; std::cin >> id;
        std::cout << "Ingrese Peso: ";
        float w; std::cin >> w;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Ingrese Destino: ";
        char dest[50];
        std::cin.getline(dest, 50);
        std::cout << "Ingrese Prioridad (1-Alta, 2 Media, 3 Baja): ";
        int pri; std::cin >> pri;
        
        pkgID[pos] = id;
        peso[pos] = w;
        std::strcpy(destino[pos], dest);
        prioridad[pos] = pri;
        estado[pos] = 'R';
        
        tope_pila++;
        
        std::cout << "Paquete " << id << " Recibido. Estado: 'R'. Pila.tope_pila = " << tope_pila << ".\n";
    }
    
    void procesarRecepcion() {
        if (tope_pila < 0) {
            std::cout << "Pila de recepción vacía.\n";
            return;
        }
        int pos = -1;
        // Buscar el índice del paquete en estado 'R' y que esté en tope_pila lógico (usando la cantidad de paquetes recibidos)
        // Lo que hay que hacer es encontrar el enésimo paquete con estado R, donde n=tope_pila
        // Pero para simplificar, tope_pila es el contador de paquetes recibidos -1
        
        int countR = -1;        // para buscar el paquete en estado 'R' que sea el último recibido
        for (int i = 0; i < capacidad; i++) {
            if (estado[i] == 'R') countR++;
            if (countR == tope_pila) {
                pos = i;
                break;
            }
        }
        if (pos == -1) {
            std::cout << "Error: paquete tope no encontrado.\n";
            return;
        }
        
        estado[pos] = 'E';        // Cambiar estado a 'E' y colocarlo en cola
        final_cola++;         // mover final_cola adelante

        
        if (final_cola >= capacidad)         // Si cola vacía (final_cola==frente_cola), varios casos:
            final_cola = capacidad - 1; // protección (puede pasar si hay error)
        if (frente_cola > final_cola)
            frente_cola = final_cola;
        
        tope_pila--;
        
        std::cout << "POP de Pila: Pkg " << pkgID[pos] << " transferido a Cola. Estado: 'E'. frente_cola=" << frente_cola << ", final_cola=" << final_cola << ".\n";
    }
    
    void despachar() {
        if (frente_cola > final_cola) {
            std::cout << "Cola vacía.\n";
            return;
        }
        
        int pos_prioridad1 = -1;         // Buscar paquete prioridad 1 entre frente_cola y final_cola
        for (int i = frente_cola; i <= final_cola; i++) {
            if (estado[i] == 'E' && prioridad[i] == 1) {
                pos_prioridad1 = i;
                break;
            }
        }
        
        int pos_despacho = -1;
        if (pos_prioridad1 != -1) {
            pos_despacho = pos_prioridad1;
            std::cout << "Analizando Cola... Detectada ALTA PRIORIDAD.\n";
        } else {
            for (int i = frente_cola; i <= final_cola; i++) {            // Normal FIFO
                if (estado[i] == 'E') {
                    pos_despacho = i;
                    break;
                }
            }
        }
        
        if (pos_despacho == -1) {
            std::cout << "No hay paquetes para despachar.\n";
            return;
        }
        
        estado[pos_despacho] = 'D';         // Marcar como despachado
        std::cout << "DESPACHADO: Paquete " << pkgID[pos_despacho] << " (Prioridad " << prioridad[pos_despacho] << "). Estado: 'D'.\n";
        
        if (pos_despacho == frente_cola) {             // Avanzar frente_cola para mantener FIFO
            do {
                frente_cola++;
            } while (frente_cola <= final_cola && estado[frente_cola] != 'E');
        } else {
            // No mover datos si solo ignorar índice pos_despacho (salto lógico)
            // frente y cola no cambia
        }
        
        // Si quedo solo vacia reset indices
        if (frente_cola > final_cola) {
            frente_cola = 0;
            final_cola = -1;
        }
    }
    
    void reporteEstados() {
        int count_r = 0, count_e = 0, count_d = 0;
        for (int i = 0; i < capacidad; i++) {
            switch (estado[i]) {
                case 'R': count_r++; break;
                case 'E': count_e++; break;
                case 'D': count_d++; break;
            }
        }
        std::cout << "Reporte de estados:\n";
        std::cout << "Paquetes Recibidos (Pila): " << count_r << "\n";
        std::cout << "Paquetes en Cola (Despacho): " << count_e << "\n";
        std::cout << "Paquetes Despachados: " << count_d << "\n";
    }
};

int menu() {
    int opcion;
    std::cout << "\n--- Menú Principal ---\n";
    std::cout << "1. Recibir Nuevo Paquete (PUSH)\n";
    std::cout << "2. Procesar Recepción (POP y ENCOLAR)\n";
    std::cout << "3. Despachar (DESENCOLAR con Prioridad)\n";
    std::cout << "4. Reporte de Estados\n";
    std::cout << "5. Salir (Liberar Memoria)\n";
    std::cout << "Opción seleccionada: ";
    std::cin >> opcion;
    // crear bufer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return opcion;
}

int main() {
    std::cout << "--- MegaEnvío: Sistema Integrado con Prioridad ---\n";
    int capacidad_inicial = 50;
    std::cout << "Inicializando sistema con capacidad para " << capacidad_inicial << " paquetes...\n";
    
    SistemaMegaEnvio sistema(capacidad_inicial);
    
    bool salir = false;
    while (!salir) {
        int op = menu();
        switch (op) {
            case 1:
                sistema.recibirPaquete();
                break;
            case 2:
                sistema.procesarRecepcion();
                break;
            case 3:
                sistema.despachar();
                break;
            case 4:
                sistema.reporteEstados();
                break;
            case 5:
                salir = true;
                break;
            default:
                std::cout << "Opción inválida.\n";
        }
    }
    
    std::cout << "Saliendo y liberando memoria...\n";     // Destructor de mundos automático liberará la memoria
    return 0;
}

