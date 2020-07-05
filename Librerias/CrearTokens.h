#ifndef CrearTokens_H
#define CrearTokens_H

#include <iostream>
#include <stdlib.h>
#include <regex>
#include <fstream>

using namespace std;

class CrearTokens
{
private:
    /* data */
public:
    ifstream archivo_entrada;
    ofstream archivoEscritura;
    string arreglo_tipo_datos[100];
    string arreglo_tipo_digitos[100];
    string arreglo_tipo_flotantess[100];
    string arreglo_aritmetico[100];
    void crearArchivo(string);  // Crea los txt que envía el main.
    void revisarRepetidos(string *, int);   // Se encarga de revisar si se repitieron los lexemas, ya que en el archivo de TablaTokens no se permite que se repitan.
    void analizarLexemas(); // Se encarga de llamar a todas las funciones que analizan los lexemas para que en el main sólo tengamos que llamar a esta función.
    void tablaTokens(string *, int, string);    // Función que inserta el lexema sin repetir junto con su respectivo token en el archivo TablaTokens.txt
    void lexemasDatos();
    void lexemasDigitos();
    void lexemasDigitosFlotantes();
    void lexemasAritmeticos();
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
    archivoEscritura.open("TablaTokens.txt", ios::app); // Se puede meter en una función a la que se le enviaría el arreglo, el tamaño del arreglo y el token
    if (archivoEscritura.fail()) {
        cout<<"Error al abrir el archivo\n";
        exit(1);
    }

    revisarRepetidos(arreglo, tamanio_arreglo);
    for (int i = 0; i < tamanio_arreglo; i++) {
        if(arreglo[i] != ""){
            archivoEscritura<<"Lexema: "<<arreglo[i]<<"\t\tToken: "<<token<<"\n";  // Una idea es poner el token determinado en el cout y quitar la variable
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
            cout<<"Match flotante: "<<palabra<<"\n";
        }
    }

    archivo_entrada.close();

    tablaTokens(arreglo_aritmetico, tamanio_arreglo, tipo_token);
}


#endif