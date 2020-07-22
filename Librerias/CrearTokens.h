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
    ifstream archivo_entrada;
    ofstream archivoEscritura, archivoTokensError;
    string arreglo_tipo_datos[100], arreglo_tipo_digitos[100], arreglo_tipo_flotantess[100], arreglo_aritmetico[100], arreglo_asignacion[100], 
        arreglo_identificador[100], arreglo_operador_relacional[100], arreglo_delimitadores[100], arreglo_miscelaneos[100], errores[100];
    string lexemas="";
    int tam_datos=0, cont_errores=0; 
public:
    void crearArchivo(string);  // Crea los txt que envía el main.
    void revisarRepetidos(string *, int);   // Se encarga de revisar si se repitieron los lexemas, ya que en el archivo de TablaTokens no se permite que se repitan.
    void analizarLexemas(); // Se encarga de llamar a todas las funciones que analizan los lexemas para que en el main sólo tengamos que llamar a esta función.
    void tablaTokens(string *, int, string);    // Función que inserta el lexema sin repetir junto con su respectivo token en el archivo TablaTokens.txt
    void detectarErrores();
    void tablaTokensError(string *, int, string, string);
    void crearArchivoTokens();
    void lexemasDatos();
    void lexemasDigitos();
    void lexemasDigitosFlotantes();
    void lexemasAritmeticos();
    void lexemasAsignacion();
    void lexemasIdentificador();
    void lexemasOperadoresRel();
    void lexemasDelimitadores();
    void lexemasMiscelaneos();
    void reemplazarLexemas(regex, string);
};

void CrearTokens::crearArchivo(string nombre_archivo) {
    ofstream archivo(nombre_archivo.c_str());
    if (archivo.fail()) {
        cout<<"Error al crear el archivo";
        exit(1);
    }
}

void CrearTokens::analizarLexemas() {
    detectarErrores();
    crearArchivoTokens();
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

void CrearTokens::tablaTokensError(string *arreglo, int tamanio_arreglo, string token, string descripcion_error) {
    int contador=1;
    //cout<<"Entro\n";
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

void CrearTokens::detectarErrores() {   // Hace falta revisar si al poner un número junto con una letra, marca error
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
        cout<<"Error al abrir el archivo Analizador\n";
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
    archivo_entrada.close();
    for (int i = 0; i < cont_errores; i++) {
        cout<<"Arreglo errores: "<<errores[i]<<"\n";   
    }
    if(!bandera)tablaTokensError(errores, cont_errores, token, descripcion);
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
        if(regex_match(texto, rex)) {   // Se puede hacer un método
            for (int i = 0; i < tam_datos; i++) {
                if(texto == arreglo_tipo_datos[i]) {
                    bandera=false;
                }
            }
            for (int i = 0; i < cont_errores; i++) {   
                if(texto == errores[i]) {
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

void CrearTokens::reemplazarLexemas(regex rex, string token) {  // El problema con este método es que no realiza bien su trabajo con ID y con CNE
    smatch matches;
    string revisar_palabra="";
    int contador_tokens=0, cont_inicio=0, cont_final=0;
    while(regex_search(lexemas,matches,rex)) {
        // Se puede hacer un ciclo que revise el arreglo de errores para que no se contabilice como un identificador
        if(revisar_palabra!= matches.str()) {
            revisar_palabra=matches.str();
            contador_tokens++;
        }
        cont_inicio=lexemas.find(matches.str());
        cont_final=matches.str().length();
        lexemas.replace(cont_inicio,cont_final,token+to_string(contador_tokens));
    }
}

void CrearTokens::crearArchivoTokens() {
    regex rex_identificador ("([a-zA-Z_$][a-zA-Z_$0-9]*)");
    regex rex_td ("(int|string|void|long|char|double|float|short|boolean)");
    regex rex_miscelaneos ("([,;:])");
    regex rex_del ("([\\[\\{\\}\\]\\)\\(])"); 
    regex rex_operadores ("([<|>|!][=]?)");
    regex rex_asignacion ("([*/%+-]?=)");
    regex rex_aritmeticos ("([*/%+-])");
    regex rex_flotantes ("([0-9]+\\.[0-9]+)");
    regex rex_digitos ("(-?[0-9]+)");
    string texto="", token="", palabras=""; // lexemas="",
    int cont_inicio=0, cont_final=0, contador_tokens=0, palabra_borrar=0, salto_linea=0;
    bool bandera_salto_linea=true, bandera_error=false;

    archivo_entrada.open("Analizador.txt", ios::in);

    if (archivo_entrada.fail()) {
        cout<<"Error al abrir el archivo\n";
        exit(1);
    }

    while (!archivo_entrada.eof()) {
        getline(archivo_entrada, texto, ' ');
        if(regex_match(texto, rex_td)) {    // Este método de los if es muy repetitivo pero se pueden manejar errores a diferencia del otro método,
                                            // lo malo es que no tiene contador de tokens.
            lexemas+="TD";
            bandera_salto_linea=false;
        } else {
            if(regex_match(texto, rex_identificador)){
                for (int i = 0; i < cont_errores; i++) {
                    if(texto == errores[i]) {
                        bandera_error=true;
                    }
                }
                if(bandera_error) {
                    lexemas+="ERLX";
                    bandera_salto_linea=false;
                } else {
                    lexemas+="ID";
                    bandera_salto_linea=false;
                }
                bandera_error=false;
            }
        }
        if(regex_match(texto, rex_miscelaneos)) {
            lexemas+="SEP";
            bandera_salto_linea=false;
        }
        if(regex_match(texto, rex_del)) {
            lexemas+="DEL";
            bandera_salto_linea=false;
        }
        if(regex_match(texto, rex_operadores)) {
            lexemas+="OR";
            bandera_salto_linea=false;
        }
        if(regex_match(texto, rex_asignacion)) {
            lexemas+="AS";
            bandera_salto_linea=false;
        }
        if(regex_match(texto, rex_aritmeticos)) {
            lexemas+="OA";
            bandera_salto_linea=false;
        }
        if(regex_match(texto, rex_digitos)) {
            lexemas+="CNE";
            bandera_salto_linea=false;
        }
        if(regex_match(texto, rex_flotantes)) {
            lexemas+="CNPF";
            bandera_salto_linea=false;
        }
        if(bandera_salto_linea) {
            palabra_borrar= texto.find_last_not_of("\n"); 
            if(palabra_borrar != std::string::npos){
                texto.erase(palabra_borrar+1);
            }
            if(regex_match(texto, rex_miscelaneos)) {
                lexemas+="SEP\n";
            } else {
                if(regex_match(texto, rex_del)) {
                lexemas+="DEL\n";
                } else {
                    for (int i = 0; i < cont_errores; i++) {
                        if(texto == errores[i]) {
                            lexemas+="ERLX";
                        }
                    }
                    
                }
            }
            //lexemas += texto;
        }
        bandera_salto_linea=true;
    }
    archivo_entrada.close();
    //reemplazarLexemas(rex_identificador, "ID");
    //reemplazarLexemas(rex_digitos, "CNE");
    //reemplazarLexemas(rex_td, "TD");
    /*reemplazarLexemas(rex_miscelaneos, "SEP");
    reemplazarLexemas(rex_del, "DEL");
    reemplazarLexemas(rex_operadores, "OR");
    reemplazarLexemas(rex_asignacion, "AS");
    reemplazarLexemas(rex_aritmeticos, "OA");
    reemplazarLexemas(rex_flotantes, "CNPF");*/
    //cout<<"Lexemas: "<<lexemas<<"\n";
    archivoEscritura.open("Tokens.txt", ios::app); 
    if (archivoEscritura.fail()) {
        cout<<"Error al abrir el archivo\n";
        exit(1);
    }
    archivoEscritura<<lexemas;

    archivoEscritura.close();
}


#endif