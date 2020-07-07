#ifndef CrearTokens_H
#define CrearTokens_H

#include <iostream>
#include <stdlib.h>
#include <regex>
#include <fstream>
#include "Errores.h"

using namespace std;

class CrearTokens
{
private:
    /* data */
public:
    Errores error;
    ifstream archivo_entrada;
    ofstream archivoEscritura;
    string arreglo_tipo_datos[100];
    string arreglo_tipo_digitos[100];
    string arreglo_tipo_flotantess[100];
    string arreglo_aritmetico[100];
    string arreglo_asignacion[100];
    string arreglo_identificador[100];
    string arreglo_operador_relacional[100];
    string arreglo_delimitadores[100];
    string arreglo_miscelaneos[100];
    int tam_datos=0;
    void crearArchivo(string);  // Crea los txt que envía el main.
    void revisarRepetidos(string *, int);   // Se encarga de revisar si se repitieron los lexemas, ya que en el archivo de TablaTokens no se permite que se repitan.
    void analizarLexemas(); // Se encarga de llamar a todas las funciones que analizan los lexemas para que en el main sólo tengamos que llamar a esta función.
    void tablaTokens(string *, int, string);    // Función que inserta el lexema sin repetir junto con su respectivo token en el archivo TablaTokens.txt
    void lexemasDatos();
    void lexemasDigitos();
    void lexemasDigitosFlotantes();
    void lexemasAritmeticos();
    void lexemasAsignacion();
    void lexemasIdentificador();
    void lexemasOperadoresRel();
    void lexemasDelimitadores();
    void lexemasMiscelaneos();
};

void CrearTokens::crearArchivo(string nombre_archivo) {
    ofstream archivo(nombre_archivo.c_str());
    if (archivo.fail()) {
        cout<<"Error al crear el archivo";
        exit(1);
    }
}

void CrearTokens::analizarLexemas() {
    lexemasDatos();
    lexemasDigitos();
    lexemasDigitosFlotantes();
    lexemasAritmeticos();
    lexemasAsignacion();
    lexemasIdentificador();
    lexemasOperadoresRel();
    lexemasDelimitadores();
    lexemasMiscelaneos();
}

void CrearTokens::revisarRepetidos(string *arreglo, int tamanio_arreglo) {
    string analizar="";
    for(int k = 0; k < tamanio_arreglo; k++){
        analizar = arreglo[k];
        for (int j = k+1; j <= tamanio_arreglo; j++) {
            if(analizar == arreglo[j]) {
                arreglo[j]="";
            }
        }
    }
}

void CrearTokens::tablaTokens(string *arreglo, int tamanio_arreglo, string token) {
    int contador=1;
    archivoEscritura.open("TablaTokens.txt", ios::app); 
    if (archivoEscritura.fail()) {
        cout<<"Error al abrir el archivo\n";
        exit(1);
    }

    revisarRepetidos(arreglo, tamanio_arreglo);
    for (int i = 0; i < tamanio_arreglo; i++) {
        if(arreglo[i] != ""){
            archivoEscritura<<"Lexema: "<<arreglo[i]<<"\t\tToken: "<<token<<contador<<"\n";
            contador++;  
        }
    }
    archivoEscritura.close();
}

void CrearTokens::lexemasDatos() {
    //smatch matches;
    regex rex ("(int|string|void|long|char|double|float|short|boolean)");
    string texto="", palabra="", tipo_token="TD";    //analizar="",
    int tamanio_arreglo=0;

    archivo_entrada.open("Analizador.txt", ios::in);

    if (archivo_entrada.fail()) {
        cout<<"Error al abrir el archivo\n";
        exit(1);
    }
    while (!archivo_entrada.eof()) {
        getline(archivo_entrada, texto, ' '); // '\n'
        if(regex_match(texto, rex)) {
            palabra = texto;
            arreglo_tipo_datos[tamanio_arreglo] = palabra;
            tamanio_arreglo++;
            cout<<"Match tipo de dato: "<<palabra<<"\n";
        }
        //analizar+=texto+"\n";
    }

    archivo_entrada.close();
    tam_datos = tamanio_arreglo;
    //cout<<"Analizar: "<<analizar<<"\n";

    tablaTokens(arreglo_tipo_datos, tamanio_arreglo, tipo_token);
}

void CrearTokens::lexemasDigitos() {
    regex rex ("(-?[0-9]+)");
    string texto="", palabra="", tipo_token="CNE"; 
    int tamanio_arreglo=0;

    archivo_entrada.open("Analizador.txt", ios::in);

    if (archivo_entrada.fail()) {
        cout<<"Error al abrir el archivo\n";
        exit(1);
    }
    while (!archivo_entrada.eof()) {
        getline(archivo_entrada, texto, ' ');
        if(regex_match(texto, rex)) {
            palabra = texto;
            arreglo_tipo_digitos[tamanio_arreglo] = palabra;
            tamanio_arreglo++;
            cout<<"Match digito: "<<palabra<<"\n";
        }
    }

    archivo_entrada.close();
    tablaTokens(arreglo_tipo_digitos, tamanio_arreglo, tipo_token);
}

void CrearTokens::lexemasDigitosFlotantes() {
    regex rex ("([0-9]+\\.[0-9]+)");
    string texto="", palabra="", tipo_token="CNPF"; 
    int tamanio_arreglo=0;

    archivo_entrada.open("Analizador.txt", ios::in);

    if (archivo_entrada.fail()) {
        cout<<"Error al abrir el archivo\n";
        exit(1);
    }
    while (!archivo_entrada.eof()) {
        getline(archivo_entrada, texto, ' ');
        if(regex_match(texto, rex)) {
            palabra = texto;
            arreglo_tipo_flotantess[tamanio_arreglo] = palabra;
            tamanio_arreglo++;
            cout<<"Match flotante: "<<palabra<<"\n";
        }
    }

    archivo_entrada.close();

    tablaTokens(arreglo_tipo_flotantess, tamanio_arreglo, tipo_token);
}

void CrearTokens::lexemasAritmeticos() {
    regex rex ("([*/%+-])");
    string texto="", palabra="", tipo_token="OA"; 
    int tamanio_arreglo=0;

    archivo_entrada.open("Analizador.txt", ios::in);

    if (archivo_entrada.fail()) {
        cout<<"Error al abrir el archivo\n";
        exit(1);
    }
    while (!archivo_entrada.eof()) {
        getline(archivo_entrada, texto, ' ');
        if(regex_match(texto, rex)) {
            palabra = texto;
            arreglo_aritmetico[tamanio_arreglo] = palabra;
            tamanio_arreglo++;
            cout<<"Match aritmético: "<<palabra<<"\n";
        }
    }

    archivo_entrada.close();

    tablaTokens(arreglo_aritmetico, tamanio_arreglo, tipo_token);
}

void CrearTokens::lexemasAsignacion() {
    regex rex ("([*/%+-]?=)");
    string texto="", palabra="", tipo_token="AS"; 
    int tamanio_arreglo=0;

    archivo_entrada.open("Analizador.txt", ios::in);

    if (archivo_entrada.fail()) {
        cout<<"Error al abrir el archivo\n";
        exit(1);
    }
    while (!archivo_entrada.eof()) {
        getline(archivo_entrada, texto, ' ');
        if(regex_match(texto, rex)) {
            palabra = texto;
            arreglo_asignacion[tamanio_arreglo] = palabra;
            tamanio_arreglo++;
            cout<<"Match asignación: "<<palabra<<"\n";
        }
    }

    archivo_entrada.close();

    tablaTokens(arreglo_asignacion, tamanio_arreglo, tipo_token);
}

void CrearTokens::lexemasIdentificador() {
    regex rex ("([a-zA-Z_$][a-zA-Z_$0-9]*)");
    string texto="", palabra="", tipo_token="ID"; 
    int tamanio_arreglo=0;
    bool bandera=true;

    archivo_entrada.open("Analizador.txt", ios::in);

    if (archivo_entrada.fail()) {
        cout<<"Error al abrir el archivo\n";
        exit(1);
    }
    while (!archivo_entrada.eof()) {
        getline(archivo_entrada, texto, ' ');
        if(regex_match(texto, rex)) {   // Se debe agregar un ciclo for por cada arrreglo de palabras reservadas que no debe tomar como identificador, igual se puede hacer un método
            for (int i = 0; i < tam_datos; i++) {
                if(texto == arreglo_tipo_datos[i]) {
                    bandera=false;
                }
            }
            for (int i = 0; i < error.cont_errores+1; i++) {    // Bug: No recibe los datos guardados en el arreglo de errores, al igual que no recibe el contador.
               // cout<<"Error: "<<error.errores[i]<<"\n";
                if(texto == error.errores[i]) {
                    bandera=false;
                }
            }
            if(bandera) {
                palabra = texto;
                arreglo_identificador[tamanio_arreglo] = palabra;
                tamanio_arreglo++;
                cout<<"Match identificador: "<<palabra<<"\n";
            }
            bandera=true;
        }
    }

    archivo_entrada.close();

    tablaTokens(arreglo_identificador, tamanio_arreglo, tipo_token);
}

void CrearTokens::lexemasOperadoresRel() {
    regex rex ("([<|>|!][=]?)");
    string texto="", palabra="", tipo_token="OR"; 
    int tamanio_arreglo=0;

    archivo_entrada.open("Analizador.txt", ios::in);

    if (archivo_entrada.fail()) {
        cout<<"Error al abrir el archivo\n";
        exit(1);
    }
    while (!archivo_entrada.eof()) {
        getline(archivo_entrada, texto, ' ');
        if(regex_match(texto, rex)) {
            palabra = texto;
            arreglo_operador_relacional[tamanio_arreglo] = palabra;
            tamanio_arreglo++;
            cout<<"Match operador rel: "<<palabra<<"\n";
        }
    }

    archivo_entrada.close();

    tablaTokens(arreglo_operador_relacional, tamanio_arreglo, tipo_token);
}

void CrearTokens::lexemasDelimitadores() {
    regex rex ("([\\[\\{\\}\\]\\)\\(])");   
    string texto="", palabra="", tipo_token="DEL"; 
    int tamanio_arreglo=0, palabra_borrar=0;

    archivo_entrada.open("Analizador.txt", ios::in);

    if (archivo_entrada.fail()) {
        cout<<"Error al abrir el archivo\n";
        exit(1);
    }
    while (!archivo_entrada.eof()) {
        getline(archivo_entrada, texto, ' ');
        palabra_borrar= texto.find_last_not_of("\n");   // En esta parte eliminamos los saltos de línea para poder revisar correctamente el lexema.
        //cout<<"Texto antes: "<<texto<<"\n";
        if(palabra_borrar != std::string::npos){
            texto.erase(palabra_borrar+1);
        }
        //cout<<"Texto después: "<<texto<<"\n";
        if(regex_match(texto, rex)) {
            palabra = texto;
            arreglo_delimitadores[tamanio_arreglo] = palabra;
            tamanio_arreglo++;
            cout<<"Match delimitador: "<<palabra<<"\n";
        }
    }

    archivo_entrada.close();

    tablaTokens(arreglo_delimitadores, tamanio_arreglo, tipo_token);
}

void CrearTokens::lexemasMiscelaneos() {
    regex rex ("([,;:])");   
    string texto="", palabra="", tipo_token="SEP"; 
    int tamanio_arreglo=0, palabra_borrar=0;

    archivo_entrada.open("Analizador.txt", ios::in);

    if (archivo_entrada.fail()) {
        cout<<"Error al abrir el archivo\n";
        exit(1);
    }
    while (!archivo_entrada.eof()) {
        getline(archivo_entrada, texto, ' ');
        palabra_borrar= texto.find_last_not_of("\n");   // En esta parte eliminamos los saltos de línea para poder revisar correctamente el lexema.
        //cout<<"Texto antes: "<<texto<<"\n";
        if(palabra_borrar != std::string::npos){
            texto.erase(palabra_borrar+1);
        }
        //cout<<"Texto después: "<<texto<<"\n";
        if(regex_match(texto, rex)) {
            palabra = texto;
            arreglo_miscelaneos[tamanio_arreglo] = palabra;
            tamanio_arreglo++;
            cout<<"Match separador: "<<palabra<<"\n";
        }
    }

    archivo_entrada.close();

    tablaTokens(arreglo_miscelaneos, tamanio_arreglo, tipo_token);
}


#endif