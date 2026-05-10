
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <windows.h >

using namespace std;

struct palabras_ocultas {

	string palabra;
	int fila_inicio;
	int col_inicio;
	int orientacion;// 0 para Horizontal, 1 para Vertical
	bool encontrada;// Para saber si el usuario ya la adivinó
};

// crear un banco de palabras
string banco_palabras[8]{ "LIMA" ,"PERU","SOPA", "JAVA","PYTHON", "PROGRAMA","CODIGO","GRUPO" };

void posision(int x, int y) {

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD cursor;
	cursor.X = x;
	cursor.Y = y;

	SetConsoleCursorPosition(hConsole, cursor);

}

void insertar_palabras_prdeterminadas(char** array, palabras_ocultas* lista) {

	int palabras_ocultas_cant = 4;

	for (int i = 0; i < palabras_ocultas_cant; i++) {

		string palabra_eleguida;
		bool repetida;

		// Evitar palabras repetidas
		do {

			repetida = false;

			int indice_palabra = rand() % 8;
			palabra_eleguida = banco_palabras[indice_palabra];

			for (int p = 0; p < i; p++) {

				if ((*(lista + p)).palabra == palabra_eleguida) {

					repetida = true;
					break;
				}
			}

		} while (repetida);

		int longuitud = palabra_eleguida.length();
		int orientacion = rand() % 3;

		
		// HORIZONTAL
	
		if (orientacion == 0) {

			int fila = rand() % 10;
			int columna = rand() % (10 - longuitud + 1);

			bool espacio_libre = true;

			// Verificar espacios
			for (int j = 0; j < longuitud; j++) {

				if (*(*(array + fila) + (columna + j)) != '-') {

					espacio_libre = false;
					break;
				}
			}

			// Si choca, repetir intento
			if (!espacio_libre) {

				i--;
				continue;
			}

			// Insertar palabra
			for (int j = 0; j < longuitud; j++) {

				*(*(array + fila) + (columna + j)) = palabra_eleguida[j];
			}

			// Guardar datos
			lista[i].palabra = palabra_eleguida;
			lista[i].fila_inicio = fila;
			lista[i].col_inicio = columna;
			lista[i].orientacion = 0;
			lista[i].encontrada = false;
		}


		// =========================
		// VERTICAL
		// =========================
		else if(orientacion == 1) {

			int fila_ini = rand() % (10 - longuitud + 1);
			int colum = rand() % 10;

			bool espacio_libre = true;

			// Verificar espacios
			for (int j = 0; j < longuitud; j++) {

				if (*(*(array + (fila_ini + j)) + colum) != '-') {

					espacio_libre = false;
					break;
				}
			}

			// Si choca, repetir intento
			if (!espacio_libre) {

				i--;
				continue;
			}

			// Insertar palabra
			for (int j = 0; j < longuitud; j++) {

				*(*(array + (fila_ini + j)) + colum) = palabra_eleguida[j];
			}

			// Guardar datos
			(*(lista + i)).palabra = palabra_eleguida;
			(*(lista + i)).fila_inicio = fila_ini;
			(*(lista + i)).col_inicio = colum;
			(*(lista + i)).orientacion = 1;
			(*(lista + i)).encontrada = false;
		}

		//DIAGONAL

		else {
			// DIAGONAL (Descendente: Derecha-Abajo)
			// Restringimos fila y columna para que la palabra no se salga del tablero 10x10
			int fila_ini = rand() % (10 - longuitud + 1);
			int colum_ini = rand() % (10 - longuitud + 1);

			bool espacio_libre = true;

			// 1. Verificar espacios en diagonal
			for (int j = 0; j < longuitud; j++) {
				// Acceso mediante aritmética de punteros: array[fila_ini + j][colum_ini + j]
				if (*(*(array + (fila_ini + j)) + (colum_ini + j)) != '-') {
					espacio_libre = false;
					break;
				}
			}

			// Si choca con otra palabra, retrocedemos el índice para reintentar
			if (!espacio_libre) {
				i--;
				continue;
			}

			// 2. Insertar palabra en diagonal
			for (int j = 0; j < longuitud; j++) {
				*(*(array + (fila_ini + j)) + (colum_ini + j)) = palabra_eleguida[j];
			}

			// 3. Guardar datos en la estructura
			(*(lista + i)).palabra = palabra_eleguida;
			(*(lista + i)).fila_inicio = fila_ini;
			(*(lista + i)).col_inicio = colum_ini;
			(*(lista + i)).orientacion = 2; // 2 representará Diagonal
			(*(lista + i)).encontrada = false;
		}

		
	}
}

/*
void insertar_palabras_prdeterminadas(char** array, palabras_ocultas* lista) {
	int palabras_ocultas_cant = 4;

	for (int i = 0; i < palabras_ocultas_cant; i++) {
		string palabra_eleguida;
		bool repetida;

		// nos aseguramos de que la palabra no se repitan al moemnto de generarlas
		do {
			repetida = false;
			int indice_palabra = rand() % 8;
			palabra_eleguida = banco_palabras[indice_palabra];

			// Revisamos si ya elegimos esta palabra en los turnos anteriores
			for (int p = 0; p < i; p++) {
				if ((*(lista + p)).palabra == palabra_eleguida) {
					repetida = true; //  Ya la habíamos usado
					break;
				}
			}
		} while (repetida); // Si está repetida, vuelve a sacar otra del banco

		int longuitud = palabra_eleguida.length();
		int orientacion = rand() % 2; // 0: Horizontal, 1: Vertical

		if (orientacion == 0) { // Horizontal
			int fila = rand() % 10;
			int columna = rand() % (10 - longuitud + 1);

			

			//for (int j = 0; j < longuitud; j++) {
				
			//	*(*(array + fila) + (columna + j)) = palabra_eleguida[j];
			//}

			// Guardamos en el struct
			//sin punteros
			lista[i].palabra = palabra_eleguida;
			lista[i].fila_inicio = fila;
			lista[i].col_inicio = columna;
			lista[i].orientacion = 0;
			lista[i].encontrada = false;
		}
		else { // Vertical
			int fila_ini = rand() % (10 - longuitud + 1);
			int colum = rand() % 10;

			for (int j = 0; j < longuitud; j++) {
				//array[fila_ini + j][colum] = palabra_eleguida[j];
				*(*(array + (fila_ini + j)) + colum) = palabra_eleguida[j];
			}

			// Guardamos en el struct
			//punteros
			(*(lista + i)).palabra = palabra_eleguida;
			(*(lista + i)).fila_inicio = fila_ini;
			(*(lista + i)).col_inicio = colum;
			(*(lista + i)).orientacion = 1; // o 1
			(*(lista + i)).encontrada = false;

			
		}
	}
}
*/
void mostrar_lista_palabras(palabras_ocultas* lista) {

	//mobemos el cursor a la fila 11
	posision(0, 13);

	cout << "PALABRAS OCULTAS ";
	cout << endl;
	for (int i = 0; i < 4; i++)
	{
		// Imprimimos la palabra guardada en el struct
		cout <<"-"<<(*(lista + i)).palabra << endl;
	}
	
}

void inicializar_sopa(char** array,int filas,int  columnas) {

    for (int i = 0; i < filas; i++) {

        for (int j = 0; j < columnas; j++) {

            *(*(array+i)+j) = '-'; // Espacio vacío
        }
    }
}

void rellenar_espacios(char** array, int filas, int  columnas) {

	for (int i = 0; i < filas; i++)
	{
		for (int j = 0; j < columnas; j++)
		{
			//condicion para yenar espacios
			if (*(*(array + i) + j) == '-') {

				*(*(array + i) + j) = 'A' + (rand() % 26);
			}
		}
	}
}

void mostrar_sopa_letras(char** array, int filas, int columnas) {

	cout << "============ SOPA DE LETRAS ============" << endl;
	//  numeros de columnas
	cout << "  ";
	
	for (int j = 0; j < columnas; j++)
	{
		cout << j << " ";
		
	}
	cout << endl;

	//  filas y  sopa
	for (int i = 0; i < filas; i++)
	{
		//  fila
		cout << i << " ";

		// Letras 
		for (int j = 0; j < columnas; j++)
		{
			cout << * (*(array + i) + j) << " ";
		}

		cout << endl;
	}
}

/*
void mostrar_sopa_letras(char** array, int filas, int  columnas) {

	
	for (int i = 0; i < filas; i++)
	{
		for (int j = 0; j < columnas; j++)
		{
			cout << *(*(array + i) + j) << " ";
		}
		cout << endl;
	}

}
*/

int validar_acierto(string palabra, int f, int c, palabras_ocultas lista[4]) {
	for (int i = 0; i < 4; i++) {
		// 1. Verificamos si la palabra coincide y si empieza en la coordenada correcta
		if (lista[i].palabra == palabra && lista[i].fila_inicio == f && lista[i].col_inicio == c) {

			// Si ya fue encontrada antes, no la validamos de nuevo
			if (lista[i].encontrada) return -1;

			// 2. Dependiendo de la orientación, comprobamos la dirección
			// HORIZONTAL
			if (lista[i].orientacion == 0) {
				return i; // Coincide
			}
			// VERTICAL
			else if (lista[i].orientacion == 1) {
				return i; // Coincide
			}
			// DIAGONAL (La nueva lógica)
			else if (lista[i].orientacion == 2) {
				// Como ya validamos que 'palabra', 'f' y 'c' son iguales a la lista,
				// y sabemos que es diagonal, confirmamos el acierto.
				return i;
			}
		}
	}
	return -1; // No se encontró coincidencia
}
/*
int validar_acierto(string palabra, int f, int c, palabras_ocultas* lista) {

	for (int i = 0; i < 4; i++) {//confirmamos si la palabra existe si coinside con i y j

		if ((*(lista + i)).palabra == palabra &&
			(*(lista + i)).fila_inicio == f &&
			(*(lista + i)).col_inicio == c &&
			(*(lista + i)).encontrada == false) {

			return i;//retornamos la posision de la palabra
		}
	}
	
	return -1;//se retorna si el usuario se equiboca
}
*/
//funcion para pintar en pantalla

void resaltar_palabra(char** array, int f_ini, int c_ini, int f_fin, int c_fin) {

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (f_ini == f_fin) { //pinta en horizontal
		for (int c = c_ini; c <= c_fin; c++)
		{
			//calculamos la posision en la pantalla
			int x_pantalla = 2 + c * 2;//sumo un dos para adecuarlo con los numeros
			int y_pantalla = 2 + f_ini;

			posision(x_pantalla, y_pantalla);// movemos el cursor

			SetConsoleTextAttribute(hConsole,( 9*16)+7);//color de fondo azul
			//SetConsoleTextAttribute(hConsole, 9);//color azul

			cout << *(*(array + f_ini) + c);// Sobrescribimos la letra original
		}
	}
	
	else if (c_ini == c_fin) {//pintar en vertical

		for (int f = f_ini; f <= f_fin; f++)
		{
			//calculamos la posision en la pantalla
			int x_pantalla = 2 + c_ini * 2;
			int y_pantalla = 2 + f;

			posision(x_pantalla, y_pantalla);

			SetConsoleTextAttribute(hConsole, 9);//color azul

			cout << *(*(array + f) + c_ini);
		}
	}
	else { // DIAGONAL (Nueva lógica)
		int pasos = f_fin - f_ini; // Cuántas letras avanzar
		SetConsoleTextAttribute(hConsole, 9);//color azul
		for (int i = 0; i <= pasos; i++) {
			// Aumentamos fila y columna simultáneamente
			int f_actual = f_ini + i;
			int c_actual = c_ini + i;

			posision(2 + c_actual * 2, 2 + f_actual);
			cout << *(*(array + f_actual) + c_actual);
		}
	}
	// Volvemos a dejar el color en blanco para lo que venga después
	SetConsoleTextAttribute(hConsole, 7);

}

void pintar_accierto(char** array, int f, int c, palabras_ocultas palabra_encontrada) {

	int longuitud = palabra_encontrada.palabra.length();
	int f_fin = f;
	int c_fin = c;

	if (palabra_encontrada.orientacion == 0) {//horizontal

		c_fin = c + longuitud - 1;
	}
	else if(palabra_encontrada.orientacion == 1) {
		f_fin = f + longuitud - 1;
	}
	else if (palabra_encontrada.orientacion == 2) { // DIAGONAL
		// En diagonal, tanto el final de la fila como de la columna avanzan
		f_fin = f + longuitud - 1;
		c_fin = c + longuitud - 1;
	}

	// Llamamos a tu función original para pintar de azul
	resaltar_palabra(array, f, c, f_fin, c_fin);
}

void pedir_coordenadas(char** array, palabras_ocultas palabras_en_juego[4]) {
	int intentos = 3;
	int puntos = 0;
	string palabra;
	int f, c;

	do {
		posision(0, 19);
		cout << "intentos restantes " << intentos << " Puntos: " << puntos << "                                           " << endl;

		cout << "cual es la palabra que encontraste: ";
		cin >> palabra;

		// pasamos lo que ingresa a mayusculas
		for (int i = 0; i < palabra.length(); i++) {
			palabra[i] = toupper(palabra[i]);
		}

		cout << "ingresa fila (i): ";
		cin >> f;
		cout << "ingresa columna (j): ";
		cin >> c;

		// llamamos a la funcion validar acierto
		int indice_encontrado = validar_acierto(palabra, f, c, palabras_en_juego);

		// --- CASO 1: EL USUARIO ACERTÓ ---
		if (indice_encontrado != -1) {
			palabras_en_juego[indice_encontrado].encontrada = true;
			puntos++;

			// Pintamos la palabra en la sopa
			pintar_accierto(array, f, c, palabras_en_juego[indice_encontrado]);

			// Mensaje de éxito
			posision(0, 24);
			cout << "felisidades encontraste la palabra '" << palabra << "'.                              " << endl;

			// Si ya completó las 4 palabras, gana y salimos de la función
			if (puntos == 4) {
				posision(0, 27);
				cout << "FELICIDADES has encontrado todas las palabras" << endl;
				return;
			}
		}
		// --- CASO 2: EL USUARIO SE EQUIVOCÓ ---
		else {
			intentos--;
			posision(0, 25);
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 12); // Rojo para el error
			cout << "Incorrecto no se encontro la palabra en esa posicion           " << endl;
			SetConsoleTextAttribute(hConsole, 7); // Volvemos al blanco normal
		}

	} while (intentos > 0);

	// Si se quedó sin intentos
	posision(0, 25);
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 12); // Rojo
	cout << "¡GAME OVER! Te has quedado sin intentos. Puntos finales: " << puntos << "  " << endl;
	SetConsoleTextAttribute(hConsole, 7); // Blanco
}





int main()
{
	srand(time(NULL));

	int filas = 10, columnas = 10;
	char** A = new char* [filas];

	for (int i = 0; i < filas; i++)
	{
		A[i] = new char[columnas];
	}

	// 2. Creamos el arreglo de structs para recordar las palabras ocultas
	palabras_ocultas palabras_en_juego[4];

	inicializar_sopa(A,filas,columnas);
	// Insertamos las palabras en la matriz física Y las guardamos en el struct
	insertar_palabras_prdeterminadas(A, palabras_en_juego);
	
	rellenar_espacios(A,filas,columnas);

	//mostramos en consola
	mostrar_sopa_letras(A,filas,columnas);

	mostrar_lista_palabras(palabras_en_juego);

	// Esta función se quedará ejecutándose hasta que el jugador gane o pierda
	pedir_coordenadas(A, palabras_en_juego);

	

	// Liberar memoria
	for (int i = 0; i < 10; i++) {
		delete[] A[i];
	}
	delete[] A;




	return 0;
}

