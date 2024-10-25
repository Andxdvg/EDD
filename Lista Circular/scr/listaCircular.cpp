#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Usuario {
public:
    int uid;
    string nombreCompleto;
    string apellido;
    string email;

    Usuario() : uid(0), nombreCompleto(""), apellido(""), email("") {}
    Usuario(int id, string nom, string ape, string mail) 
        : uid(id), nombreCompleto(nom), apellido(ape), email(mail) {}

    bool operator<(const Usuario& otro) const {
        return uid < otro.uid;  // Comparar por ID
    }

    bool operator==(const Usuario& otro) const {
        return uid == otro.uid;  // Comparar por ID
    }

    friend ostream& operator<<(ostream& os, const Usuario& u) {
        os << u.uid << "," << u.nombreCompleto << "," << u.apellido << "," << u.email;
        return os;
    }
};

// Nodo para lista circular
template <typename T>
class NodoCircular {
public:
    T info;
    NodoCircular* siguiente;

    NodoCircular(T valor) : info(valor), siguiente(nullptr) {}
};

// Nodo para lista doble
template <typename T>
class NodoDoble {
public:
    T info;
    NodoDoble* siguiente;
    NodoDoble* anterior;

    NodoDoble(T valor) : info(valor), siguiente(nullptr), anterior(nullptr) {}
};

// Implementación de lista circular
template <typename T>
class ListaCircular {
    NodoCircular<T>* fin;
public:
    ListaCircular() : fin(nullptr) {}

    bool vacia() const {
        return fin == nullptr;
    }

    void agregar(T valor) {
        NodoCircular<T>* nuevo = new NodoCircular<T>(valor);
        if (vacia()) {
            fin = nuevo;
            fin->siguiente = nuevo;
        } else {
            nuevo->siguiente = fin->siguiente;
            fin->siguiente = nuevo;
            fin = nuevo;
        }
    }

    NodoCircular<T>* obtenerFin() const {
        return fin;
    }

    void buscar(int id) const {
        if (vacia()) {
            cout << "Lista vacía." << endl;
            return;
        }

        NodoCircular<T>* actual = fin->siguiente;
        do {
            if (actual->info.uid == id) {
                cout << "Encontrado: " << actual->info << endl;
                return;
            }
            actual = actual->siguiente;
        } while (actual != fin->siguiente);

        cout << "No se encontró." << endl;
    }

    void eliminar(int id) {
        if (vacia()) return;

        NodoCircular<T>* actual = fin->siguiente;
        NodoCircular<T>* previo = fin;

        do {
            if (actual->info.uid == id) {
                if (actual == fin->siguiente && actual == fin) {
                    delete actual;
                    fin = nullptr;
                } else {
                    previo->siguiente = actual->siguiente;
                    if (actual == fin) {
                        fin = previo;
                    }
                    delete actual;
                }
                return;
            }
            previo = actual;
            actual = actual->siguiente;
        } while (actual != fin->siguiente);
    }

    void guardarCSV(const string& nombreArchivo) const {
        ofstream archivo(nombreArchivo);
        if (archivo.is_open()) {
            NodoCircular<T>* actual = fin->siguiente;
            do {
                archivo << actual->info << endl;
                actual = actual->siguiente;
            } while (actual != fin->siguiente);
            archivo.close();
        }
    }

    void cargarCSV(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo);
        string linea, nombre, apellido, email;
        int id;

        while (getline(archivo, linea)) {
            size_t pos1 = linea.find(",");
            size_t pos2 = linea.find(",", pos1 + 1);
            size_t pos3 = linea.find(",", pos2 + 1);
            id = stoi(linea.substr(0, pos1));
            nombre = linea.substr(pos1 + 1, pos2 - pos1 - 1);
            apellido = linea.substr(pos2 + 1, pos3 - pos2 - 1);
            email = linea.substr(pos3 + 1);
            agregar(Usuario(id, nombre, apellido, email));
        }

        archivo.close();
    }
};

// Implementación de lista doblemente enlazada
template <typename T>
class ListaDoble {
    NodoDoble<T>* cabeza;
public:
    ListaDoble() : cabeza(nullptr) {}

    bool vacia() const {
        return cabeza == nullptr;
    }

    void agregarOrdenado(T valor) {
        NodoDoble<T>* nuevo = new NodoDoble<T>(valor);
        if (vacia()) {
            cabeza = nuevo;
        } else {
            NodoDoble<T>* actual = cabeza;
            NodoDoble<T>* anterior = nullptr;
            while (actual != nullptr && actual->info < valor) {
                anterior = actual;
                actual = actual->siguiente;
            }
            if (anterior == nullptr) {
                nuevo->siguiente = cabeza;
                cabeza->anterior = nuevo;
                cabeza = nuevo;
            } else {
                nuevo->siguiente = anterior->siguiente;
                nuevo->anterior = anterior;
                if (anterior->siguiente != nullptr) {
                    anterior->siguiente->anterior = nuevo;
                }
                anterior->siguiente = nuevo;
            }
        }
    }

    NodoDoble<T>* obtenerCabeza() const {
        return cabeza;
    }

    void buscar(int id) const {
        NodoDoble<T>* actual = cabeza;
        while (actual != nullptr) {
            if (actual->info.uid == id) {
                cout << "Encontrado: " << actual->info << endl;
                return;
            }
            actual = actual->siguiente;
        }
        cout << "No se encontró." << endl;
    }

    void eliminar(int id) {
        NodoDoble<T>* actual = cabeza;
        while (actual != nullptr) {
            if (actual->info.uid == id) {
                if (actual->anterior != nullptr) {
                    actual->anterior->siguiente = actual->siguiente;
                } else {
                    cabeza = actual->siguiente;
                }
                if (actual->siguiente != nullptr) {
                    actual->siguiente->anterior = actual->anterior;
                }
                delete actual;
                return;
            }
            actual = actual->siguiente;
        }
    }

    void guardarCSV(const string& nombreArchivo) const {
        ofstream archivo(nombreArchivo);
        if (archivo.is_open()) {
            NodoDoble<T>* actual = cabeza;
            while (actual != nullptr) {
                archivo << actual->info << endl;
                actual = actual->siguiente;
            }
            archivo.close();
        }
    }

    void cargarCSV(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo);
        string linea, nombre, apellido, email;
        int id;

        while (getline(archivo, linea)) {
            size_t pos1 = linea.find(",");
            size_t pos2 = linea.find(",", pos1 + 1);
            size_t pos3 = linea.find(",", pos2 + 1);
            id = stoi(linea.substr(0, pos1));
            nombre = linea.substr(pos1 + 1, pos2 - pos1 - 1);
            apellido = linea.substr(pos2 + 1, pos3 - pos2 - 1);
            email = linea.substr(pos3 + 1);
            agregarOrdenado(Usuario(id, nombre, apellido, email));
        }

        archivo.close();
    }
};

void mostrarMenu(ListaCircular<Usuario>& listaCir, ListaDoble<Usuario>& listaDob) {
    int opcion;
    string archivoCSV = "personas.csv";

    // Cargar datos desde el archivo CSV
    listaCir.cargarCSV(archivoCSV);
    listaDob.cargarCSV(archivoCSV);

    do {
        cout << "1. Agregar usuario" << endl;
        cout << "2. Buscar usuario" << endl;
        cout << "3. Eliminar usuario" << endl;
        cout << "4. Guardar en CSV" << endl;
        cout << "0. Salir" << endl;
        cin >> opcion;

        switch (opcion) {
            case 1: {
                string nombre, apellido, email;
                int nuevoId = listaCir.obtenerFin() ? listaCir.obtenerFin()->info.uid + 1 : 1;

                cout << "Ingrese nombre: ";
                cin >> nombre;
                cout << "Ingrese apellido: ";
                cin >> apellido;
                cout << "Ingrese correo: ";
                cin >> email;

                listaCir.agregar(Usuario(nuevoId, nombre, apellido, email));
                listaDob.agregarOrdenado(Usuario(nuevoId, nombre, apellido, email));
                break;
            }
            case 2: {
                int id;
                cout << "Ingrese ID a buscar: ";
                cin >> id;
                listaCir.buscar(id);
                listaDob.buscar(id);
                break;
            }
            case 3: {
                int id;
                cout << "Ingrese ID a eliminar: ";
                cin >> id;
                listaCir.eliminar(id);
                listaDob.eliminar(id);
                // Guardar después de eliminar
                listaCir.guardarCSV(archivoCSV);
                listaDob.guardarCSV(archivoCSV);
                break;
            }
            case 4: {
                listaCir.guardarCSV(archivoCSV);
                listaDob.guardarCSV(archivoCSV);
                cout << "Datos guardados." << endl;
                break;
            }
            case 0:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opción no válida." << endl;
                break;
        }
    } while (opcion != 0);
}

int main() {
    ListaCircular<Usuario> listaUsuariosCirculares;
    ListaDoble<Usuario> listaUsuariosDobles;
    mostrarMenu(listaUsuariosCirculares, listaUsuariosDobles);
    return 0;
}
