
#include <windows.h>
#include <iostream>


using namespace std;

// reduce la nesesida de escribir SetConsoleTextAttribute y lo remplasa por color
#define color  SetConsoleTextAttribute

//esto muebe el cursor a una possision dada 
void posision(int x, int y) {

    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //GetStdHandle pide permiso para controlar la consola
    //STD_OUTPUT_HANDLE es lo que controla la consola y nos permite pintar
    //STD_ERROR_HANDLE: La "llave" para los mensajes de error.
    //HANDLE Es el tipo de dato de la variable (Manejador)
    // hConsole nombre de la bariable

    COORD cursor;
    cursor.X = x;
    cursor.Y = y;

    SetConsoleCursorPosition(hConsole, cursor);
}
void pintar_las_palabras() {
    //cambiamos el color de la letra en terminal
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    color(hConsole, 1);

    for (int i = 1; i < 16; i++)
    {
        color(hConsole, i);
        cout << "pintando ajajaj" << endl;
    }
    
    
}

void pintar_fondo(int color_fondo,int color_tex) {

   
    //color de fondo X16)+color de texto

 
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    int color_final = (color_fondo * 16) + color_tex;
    color(hConsole,color_final);
}



int main()
{
 
    pintar_las_palabras();

    //pasamos los colores a usar en la funcion
    pintar_fondo(9,15);
   
    posision(5, 5);
    cout << "este texto tiene un fondo azul" << endl;

    //restablesemos al color origuinal
    pintar_fondo(0, 7);
 
    return 0;
}

