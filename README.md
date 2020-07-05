# Analizador léxico hecho en C++
El programa crea tres archivos, uno llamado Tokens.tx, otro llamado TablaTokens.txt y otro llamado TablaErrores.txt. De igual forma, el programa lee el archivo **Analizador.txt** para encontrar coincidencias, dichas coincidencias las escribe en el archivo TablaTokens.txt con su respectivo identificador.

Si desea modificar el archivo Analizador.txt, debe respetar el formato del original, al escribir un lexema, lo debe separar con espacios de los otros lexemas.

**Comandos para compilar el programa (Puede usar cualquiera de los dos comandos)**
```bash
g++ main.cpp
g++ main.cpp -o nombreArchivoCompilado
```
**Ejecuta el programa (Dependiendo del comando para compilar usado)**
```bash
./a.out
./nombreArchivoCompilado
```