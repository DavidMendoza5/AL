#ifndef ERRORES_H
#define ERRORES_H

#include <iostream>
#include <stdlib.h>
#include <regex>
#include <fstream>

using namespace std;

class Errores
{
private:
    ifstream archivo_entrada;
    ofstream archivoEscritura,archivoTokensError;
    string errores[100];
    int cont_errores=0;
public:
    void detectarErrores(); // Una idea es hacer otra función para ser llamada por el main y dicha función llame a este, pero pasando como parámetro el arreglo
    void revisarRepetidos(string *, int);
    void tablaTokens(string *, int, string, string);
};

void Errores::revisarRepetidos(string *arreglo, int tamanio_arreglo) {
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

void Errores::tablaTokens(string *arreglo, int tamanio_arreglo, string token, string descripcion_error) {
    int contador=1;
    cout<<"Entro\n";
    archivoEscritura.open("TablaTokens.txt", ios::app);
    archivoTokensError.open("TablaErrores.txt", ios::app);
    if (archivoEscritura.fail()) {
        cout<<"Error al abrir el archivo TablaTokens\n";
        exit(1);
    }
    if (archivoTokensError.fail()) {
        cout<<"Error al abrir el archivo Tokens\n";
        exit(1);
    }

    revisarRepetidos(arreglo, tamanio_arreglo);
    for (int i = 0; i < tamanio_arreglo; i++) {
        if(arreglo[i] != ""){
            archivoEscritura<<"Lexema: "<<arreglo[i]<<"\t\tToken: "<<token<<contador<<"\n";
            archivoTokensError<<"Lexema: "<<arreglo[i]<<"\t\tToken: "<<token<<contador<<"\t\tDescripción: "<<descripcion_error<<"\n";
            contador++;  
        }
    }
    archivoEscritura.close();
    archivoTokensError.close();
}

void Errores::detectarErrores() {   // Hace falta revisar si al poner un número junto con una letra, marca error
    regex rex_errores ("([-0-9][a-zA-Z_$0-9]+)");
    regex rex_td ("(int|string|void|long|char|double|float|short|boolean)");
    regex rex_id ("([a-zA-Z_$][a-zA-Z_$0-9]*)");
    regex rex_as ("([*/%+-]?=)");
    regex rex_sep ("([;,])"); 
    regex rex_del ("([\\(\\)])");   
    string texto="", datos[100], token = "", descripcion="", palabra=""; 
    int contador=0, palabra_borrar=0;
    bool bandera_td=false, bandera_id=false, bandera=true, entrar=true;

    archivo_entrada.open("Analizador.txt", ios::in);

    if (archivo_entrada.fail()) {
        cout<<"Error al abrir el archivo\n";
        exit(1);
    }
    while (!archivo_entrada.eof()) {
        getline(archivo_entrada, texto, ' ');

        palabra_borrar= texto.find_last_not_of("\n");  
        if(palabra_borrar != std::string::npos){
            texto.erase(palabra_borrar+1);
        }

        if(regex_match(texto, rex_errores)) {   // Hay un bug en el que si pones mal un TD y luego pones un id no válido (que entre a este if), sólo detecta el segundo error
            token = "ERLXID";
            descripcion = "Error en el identificador";
            errores[cont_errores] = texto;   
            cont_errores++;
            bandera = false;
        }

        if(regex_match(texto, rex_td)) {
            datos[contador] = "TD";
            palabra = texto;
            contador++;
            bandera_td = true;
            //bandera_id = true;
        } 
        if(bandera_td && (palabra != texto)) {
            //cout<<"Texto verificar: "<<texto<<"\n";
            if(regex_match(texto, rex_id) && entrar) {
                //cout<<"Texto verificar2: "<<texto<<"\n";
                bandera_id = true;
                entrar = false;
                //bandera_td = false;
            } else {
                if(entrar) {
                    token = "ERLXID";
                    descripcion = "Error en el identificador";
                    errores[cont_errores] = texto;   
                    cont_errores++;
                    bandera = false;
                    bandera_td = false;
                }
                if(bandera_id && bandera_td) {
                    if(regex_match(texto, rex_as) || regex_match(texto, rex_del) || regex_match(texto, rex_sep)) {
                        cout<<"Revisando: "<<texto<<"\n";
                        bandera_td = false;
                        bandera_id = false;
                        entrar = true;
                    } else {
                        token = "ERLX";
                        descripcion = "Error, un identificador debe ser seguido de una coma, paréntesis o una asignación";
                        errores[cont_errores] = texto;   
                        cont_errores++;
                        bandera = false;
                        bandera_td = false;
                        bandera_id = false;
                    }
                } 
                //cout<<"Texto verificar3: "<<texto<<"\n";
            }
            //bandera_id = false;
        } else{
            if(!bandera_td) {
                if(regex_match(texto, rex_id)) { 
                    cout<<"Texto entrante: "<<texto<<" "<<bandera_id<<"\n";
                    if(bandera_id) {
                        token = "ERLXTD";
                        descripcion = "Error en el tipo de dato, la palabra reservada no es válida";
                        errores[cont_errores] = palabra;
                        cout<<"Palabra error: "<<palabra<<" "<<bandera_id<<"\n";   
                        cont_errores++;
                        bandera = false;
                        bandera_id = false;
                    } else {
                        palabra = texto;
                        bandera_id = true;
                    }
                } else {
                    bandera_id = false;
                }
            }
        }
    }
    for (int i = 0; i < cont_errores; i++) {
        cout<<"Arreglo errores: "<<errores[i]<<"\n";   
    }
    if(!bandera)tablaTokens(errores, cont_errores, token, descripcion);
}


#endif