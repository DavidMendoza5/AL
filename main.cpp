#include "./Librerias/CrearTokens.h"

using namespace std;

int main() {
    CrearTokens tokens;
    string tabla_tokens="TablaTokens.txt", token_error="TablaErrores.txt", archivo_tokens="Tokens.txt";
//  Creación de los archivos necesarios para guardar los resultados
    tokens.crearArchivo(tabla_tokens);
    tokens.crearArchivo(token_error);
    tokens.crearArchivo(archivo_tokens);
// Función que llama a todas las otras funciones que analizan los lexemas y escriben los resultados en el archivo TablaTokens.txt
    tokens.analizarLexemas();


    return 0;
}