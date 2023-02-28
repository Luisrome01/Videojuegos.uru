#include <vector>
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <conio.h>
#include <string>

// Simbolos o Caracteres
#define CARACTER_FANTASMA 2

// Movimiento con las teclas
#define TECLA_ARRIBA       'w'
#define TECLA_ABAJO        's'
#define TECLA_IZQUIERDA    'a'
#define TECLA_DERECHA      'd'

// Dirección de pacman
#define DIRECCION_IZQUIERDA 0
#define DIRECCION_DERECHA   1
#define DIRECCION_ARRIBA    2
#define DIRECCION_ABAJO     3
#define DIRECCION_INVALIDA  -1


// Define los colores
#define COLOR_PARED            0x009 // AZUL CLARO
#define COLOR_COMIDA           0x00F // BLANCO BRILLANTE
#define COLOR_PACMAN           0x00E // AMARILLO
#define COLOR_FANTASMA1        0x00C // ROJO CLARO
#define COLOR_FANTASMA2        0x00D  // PÚRPURA CLARO
#define COLOR_FANTASMA3        0x00B // AGUAMARINA CLARO
#define COLOR_FANTASMA4        0x00A // VERDE CLARO
#define COLOR_MARCADOR         0x00F   // BLANCO_BRILLANTE
#define COLOR_FANTASMA_ZOMBIE  0x004 // AZUL

#define BLANCO_BRILLANTE 0x00F

#define NADA 0

// Medidas del MAPA
#define FILAS_MAPA         25
#define COLUMNAS_MAPA      29
#define LIMITE_IZQUIERDO   1
#define LIMITE_SUPERIOR    0

#define LIMITE_DERECHO (LIMITE_IZQUIERDO + COLUMNAS_MAPA - 1)
#define LIMITE_INFERIOR (LIMITE_SUPERIOR + FILAS_MAPA - 1)

// Estados finales de pacman
#define PACMAN_VIVO        10    // Se encuentra jugando
#define PACMAN_MUERTO      20    // Ha muerto
#define PACMAN_GANADOR     30    // Ya ganó

#define VIDAS_PACMAN 3
#define CARACTER_PACMAN 2

// Modos de estado de los fantasmas
#define MODO_CAZADOR 1000     // Este es el modo por defecto de los fantasmas
#define MODO_PRESA   1001     // Este modo es cuando pacman ha comida la píldora mágica
#define MODO_INVISIBLE 1002   // Este modo es cuando el fantasma ha sido comido por pacman y está regresando a su casa

#define TIEMPO_ZOMBIE 50

// Función para el manejo de los colores 
int backcolor = 0; 
void setCColor(int color)
{
   static HANDLE hConsole;
   
   hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   
   SetConsoleTextAttribute(hConsole, color | (backcolor * 0x10 + 0x100));
}



char mapa[FILAS_MAPA][COLUMNAS_MAPA + 1] = {
   "ahhhhhhhhhhhhhnhhhhhhhhhhhhhb",
   "v@············v············@v",
   "v·ahhb·ahhhhb·v·ahhhhb·ahhb·v",
   "v·v  v·v    v·v·v    v·v  v·v",
   "v·chhd·chhhhd·v·chhhhd·chhd·v",
   "v···························v",
   "v·hhhh·v·hhhhhnhhhhh·v·hhhh·v",
   "v······v······v······v······v",
   "chhhhb·phhhhh·v·hhhhho·ahhhhd",
   "     v·v······i······v·v     ",
   "     v·v·ahhhh2hhhhb·v·v     ", // 2 = para que vaya hacia arriba
   "hhhhhd·v·v    2    v·v·chhhhh",
   "·········v    2    v·········",
   "hhhhhb·v·v    2    v·v·ahhhhh",
   "     v·v·chhhhhhhhhd·v·v     ",
   "     v·v·············v·v     ",
   "ahhhhd·v·hhhhhnhhhhh·v·chhhhb",
   "v·············v·············v",
   "v·hhhb·hhhhhh·v·hhhhhh·ahhh·v",
   "v····v·················v····v",
   "phhh·v·v·hhhhhnhhhhh·v·v·hhho",
   "v······v······v······v······v",
   "v·hhhhhmhhhhh·v·hhhhhmhhhhh·v",
   "v@·························@v",
   "chhhhhhhhhhhhhhhhhhhhhhhhhhhd",
};
/* v : vertical
   h: horizontal
   m: medio y hacia arriba   _l_
   n: medio y hacia abajo    -j- 
   o: medio y hacia la izquierda -|
   p: medio y hacia la derecha   |-
   a:  esquina superior izquierda 
   b:  esquina superior derecha
   c:  esquina inferior izquierda
   d:  esquina inferior derecha
   s: salida => se transformara a hosizontal
*/

int cantidad_comida = 290;    // Guarda la cantidad de comida que hay en el mapa al principio del juego


// Esta matriz va a guardar todos los moviemientos hacia donde se va pacman. Si es que
// pacman no pasó por algún lugar, simplemente se le asigna el caracter a que significa
// aleatorio
short matriz_movimientos[FILAS_MAPA][COLUMNAS_MAPA];


using namespace std;


/******************************* DECLARACIÓN DE CLASES **************************/
class PERSONAJE;
class PACMAN;
class FANTASMA;

class PERSONAJE {
   private:
      int x, y;   // Coordenadas absolutas
      int simbolo;
      string nombre;
      int direccion;
      /* La direccion de pacman tiene  posibilidades:
         0 IZQUIERDA      : DIRECCION_IZQUIERDA
         1 DERECHA        : DIRECCION_DERECHA
         2 ARRIBA         : DIRECCION_ARRIBA
         3 ABAJO          : DIRECCION_ABAJO
         -1 NO SE MUEVE   : DIRECCION_INVALIDA
            
      */
      
      int color;
      
      int anteriorX, anteriorY;   // Coordenadas anteriores de x , y
      
   public:
      PERSONAJE(int, int = 1, int = 1, int = 1);
      void setX(int);
      void setY(int);
      void setSimbolo(int);
      void setDireccion(int);
      int getX();
      int getY();
      string getNombre();
      void setNombre(string);
      
      int getColor();
      void setColor(int);
      
      int getAnteriorX();
      int getAnteriorY();
      
      int getXRelativoAlMapa();
      int getYRelativoAlMapa();
      
      int getAnteriorXRelativoAlMapa();   // Obtiene las coordenadas anteriores relativas al mapa
      int getAnteriorYRelativoAlMapa();
      
      int getSimbolo();
      int getDireccion();
      virtual void mover(int);
      virtual void pintar();
      void borrar();
      
      int direccionVerdadera(int);
      
      bool esPosibleIrA(int posible_direccion);
      
}; // Fin de la declaración de la clase PERSONAJE



class PACMAN : public PERSONAJE {
   private:
      int puntaje;
      int vidas;     // Generalmente tiene tres vidas
      bool sigue_jugando;  // Cuando pierde todas sus vidas, ya no sigue jugando
      
   public:
      PACMAN(int color = COLOR_PACMAN, int x = LIMITE_IZQUIERDO + 14, int y = LIMITE_SUPERIOR + 15, int simbolo = CARACTER_PACMAN);
      void setPuntaje(int puntaje);
      int getPuntaje();
      void actualizar_puntaje();
      void setVidas(int vidas);
      int getVidas();
      void setSigueJugando(bool);
      bool getSigueJugando();
      bool choque(vector<FANTASMA> *,int &modo, FANTASMA **fantasma);
      int estadoFinalPacman(); // Retorna PACMAN_VIVO, PACMAN_MUERTO, PACMAN_GANADOR
      // No redefinimos el método pintar() así que se llamará al de la superclase
      void imprimirVidas();
      void reiniciaCoordenadas();
      bool comioPildora();
}; // Fin de la declaración de la clase PACMAN


class FANTASMA : public PERSONAJE {
   private:
      int modo;   // CAZADOR, PRESA
      int temporizador;
      bool temporizador_activado;
   public:
      // FANTASMA(int = (LIMITE_IZQUIERDO + LIMITE_DERECHO) / 2, int = (LIMITE_SUPERIOR + LIMITE_INFERIOR) / 2, int = 1);
      FANTASMA(int color, int = LIMITE_IZQUIERDO + 10, int = LIMITE_SUPERIOR + 12, int = CARACTER_FANTASMA);
      void mover();
      void setModo(int modo);
      int getModo();
      virtual void reiniciar_coordenadas();
      void setColorPredeterminado();
      int getTemporizador();
      void setTemporizador(int temporizador);
}; // Fin de la declaración de la clase FANTASMA


class FANTASMA_ROJO : public FANTASMA {
   private:
      
   public:
      FANTASMA_ROJO();
      void reiniciar_coordenadas();
};


class FANTASMA_ROSADO : public FANTASMA {
   private:
      
   public:
      FANTASMA_ROSADO();
      void reiniciar_coordenadas();
};


class FANTASMA_CELESTE : public FANTASMA {
   private:
      
   public:
      FANTASMA_CELESTE();
      void reiniciar_coordenadas();
};


class FANTASMA_VERDE : public FANTASMA {
   private:
      
   public:
      FANTASMA_VERDE();
      void reiniciar_coordenadas();
};
/*******************FIN DE LAS DECLARACIONES DE LAS CLASES*********************/


// Funciones
void pintar_mapa();        // Para pintar el mapa de juego con la comida
void gotoxy2(int, int);    // Similar a gotoxy pero con la API de windows
bool esPared(char);        // Determina si su argumento es un caracter correspondiente a la pared
void setCColor(int color); // Para establecer el color con lo que pintaremos

// Después hacer que esto sea una función miembro de pacman
void puntaje(PACMAN pacman, char mapa[][COLUMNAS_MAPA + 1]);   // Modifica el puntaje de pacman

void inicializaMatrizMovimientos();    // Inicializa el vector que guarda los movimientos de pacman
void imprimir_informacion(class PACMAN, int tecla_presionada);   // Imprime información del juego en tiempo real
void captura_tecla(int &tecla);  // Captura la tecla presionada y la modifica en caso se presione
void pintar_mapa(char mapa[][COLUMNAS_MAPA + 1], int filas, int columnas);
void empezar_juego();      // Función llamada por main para empezar el juego
void reinicia_personajes(PACMAN &pacman, vector<FANTASMA> &fantasmas);   // Regresa a pacman y a los fantasmas a sus coordenadas iniciales
void cuenta_regresiva(int x, int y, int limite_inferior, int limite_superior); // Hace una cuenta regresiva en las coordenadas
void PINTA_MATRIZ_MOVIMIENTOS(); // BORRAR ESTO ES DE PRUEBA

int main() 
{
   empezar_juego();
   return 0;   
}


void empezar_juego() 
{
   system("color");
   // Inicializo la matriz que guarda los movimientos de pacman con -1 (es decir, aleatorio)
   inicializaMatrizMovimientos();
   
   // Pintamos el mapa donde estará pacman y toda la comida actual
   pintar_mapa(mapa, FILAS_MAPA, COLUMNAS_MAPA);
   
   // Creamos al pacman
   PACMAN pacman;
   
   // Creando los objetos FANTASMA en tiempo de compilación
   FANTASMA_ROJO    f1;
   FANTASMA_ROSADO  f2;
   FANTASMA_CELESTE f3;
   FANTASMA_VERDE   f4;

   // Guardamos los fantasmas en un vector de fantasmas
   vector <FANTASMA> fantasmas;
   fantasmas.push_back(f1);
   fantasmas.push_back(f2);
   fantasmas.push_back(f3);
   fantasmas.push_back(f4);
   
   // Pintamos a pacman por primera vez
   pacman.pintar();
   
   // Imprimimos las vida de Pacman
   pacman.imprimirVidas();
   
   // Comienza la cuenta regresiva
   cuenta_regresiva(LIMITE_IZQUIERDO + 14, LIMITE_SUPERIOR + 13, 1, 3);
   
   int tecla = DIRECCION_INVALIDA; ///////////////////// ver si esta línea es necesaria ¿?
   
   int tecla_salir;
   bool repintar = false;
   
   int modo_fantasma = MODO_CAZADOR;   // Guarda el modo en el que estaba el fantasma con el que choqué
   FANTASMA *fantasma = NULL;          // Apunta al fantasma chocado. Si estaba en modo cazador no sirve este dato
   
   reinicia_personajes(pacman, fantasmas);
   do {
      // Esto es antes de actualizar el puntaje porque al actualizar el puntaje se borra las comidas
      if (pacman.comioPildora()) { // Verificamos si pacman comió la píldora mágica)
         // Cambiamos el modo a todos los fantasmas
         for (int i = 0; i < fantasmas.size(); ++i) {
            fantasmas[i].setModo(MODO_PRESA);   // Este método se encarga de cambiar el color
         }
      }
      
      pacman.actualizar_puntaje(); // Actualizamos el puntaje de pacman

      
      captura_tecla(tecla);   // Capturamos la tecla presionada. Si no presiono nada, conserva su valor antes de entrar a la función
      pacman.mover(tecla);    // Movemos a pacman (Lo hago así para que pacman siempre esté pintado detrás de los fantasmas al ser comido)
      imprimir_informacion(pacman, tecla); // Imprimir la información actual: coordenadas, comida, puntaje, etc
      
      // Movemos a los fantasmas
      for (int i = 0; i < fantasmas.size(); ++i) {
         fantasmas[i].mover();
      }

      // Verificamos que Pacman haya chocado o no con algún fantasma
      if (pacman.choque(&fantasmas, modo_fantasma, &fantasma)) {  // Si pacman choca con algún fantasma
      
         cout << "\a"; Sleep(1000); // Se hace una pausita luego del choque
         
         // Veamos si es que pacman muere o se come al fantasma o simplemente lo ignora (cuando el fantasma está muerto)
         
         if (modo_fantasma == MODO_CAZADOR) {  // Si chocaron cuando los fantasmas estaban en modo cazador, pacman pierde
         
            fantasma = NULL;     // No nos sirve la dirección del fantasma chocado
            
            pacman.setVidas(pacman.getVidas() - 1);  // Disminuimos la vida de pacman
            
            if (pacman.getVidas() > 0) {  // Sigue jugando pero disminuye una vida
            
               // Regresamos a pacman y a los fantasmas a su posición inicial
               reinicia_personajes(pacman, fantasmas);
               
               // Pintamos al mapa
               pintar_mapa(mapa, FILAS_MAPA, COLUMNAS_MAPA);
               
               // Pintamos a los personajes
               pacman.pintar();
               for (int i = 0; i < fantasmas.size(); ++i) {
                  fantasmas[i].pintar();
               }
               
               // Imprimimos las vida de Pacman
               pacman.imprimirVidas();
               
               // Empieza la cuenta regresiva
               cuenta_regresiva(LIMITE_IZQUIERDO + 14, LIMITE_SUPERIOR + 13, 1, 3); // Volvemos a empezar

            } else { // Significa que ya murió porque tiene cero vidas
            
               gotoxy2(LIMITE_IZQUIERDO + 10, LIMITE_SUPERIOR + 10);
               cout << "Has perdido xD!!: [s] para salir";
               
               bool sal = false;
               do {
                  char tecla = getch();
                  if (tecla == 's') 
                     sal = true;
               } while (!sal);
               // while ((getch() == 's') ? 0 : 1);
               return;  // Salimos del juego
            
            } // fin de if
            
         } else if (modo_fantasma == MODO_PRESA) { // Cuando pacman choca a un fantasma en modo presa, pacman se lo come
            
            // Cambiar al fantasma a modo MODO_INVISIBLE
            // Entonces dicho fantasma debe regresar a su casa
            
            // Una vez llegado a su casa, debe de volver su estado a modo CAZADOR y empezar con sus coordenadas iniciales
            
            
            
            ///////////////////////////
            // Por ahora simplemente haré que cuando un fantasma sea comido, aparezca inmediatamente en su casa
            // Luego investigaré sobre como hacer para que el fantasma regrese a su casa
            /*********************************************************************/
            
            
            
            fantasma->reiniciar_coordenadas();  // Regresamos al fantasma a su casa (posición inicial)
            string nombre_fantasma = fantasma->getNombre();
            fantasma->setModo(MODO_CAZADOR); // Regresamos al modo cazador
            
            /*********************************************************************/
            
            
            // Falta implementar todo esto, cuando los fantasmas regresan a su casa
            // Usar el puntero "fantasma" con el que podemos modificar al fantasma chocado
            
         } else if (modo_fantasma == MODO_INVISIBLE) { // Cuando pacman choca a un fantasma estando en modo invisible, no pasa nada
            
         }
         
      } else {
         if (pacman.estadoFinalPacman() == PACMAN_GANADOR) {
            gotoxy2(LIMITE_IZQUIERDO + 10, LIMITE_SUPERIOR + 10);
            cout << "Has GANADO :D!!: [s] para salir";
            bool sal = false; 
            do {
               char tecla = getch();
               if (tecla == 's') 
                  sal = true;
            } while (!sal);
            return;
         }
      }
      
      
      Sleep(100); // Hace una pausa muy pequeña en milisegundos
      
      /*
      if (tecla == 's') {
         tecla = pacman.getDireccion();
         gotoxy2(LIMITE_IZQUIERDO + 10, LIMITE_SUPERIOR + 10);
         cout << "Seguro que desea salir del juego (y/n)?: ";
         tecla_salir = getche();
         
         if (tecla_salir == 'y') {
            return;
         } else {
            repintar = true;  // para pintar el mapa solo cuando se haya paralizado el juego
         }
      }
      */
      
   } while (tecla_salir != 'y');
}

// Hace una cuenta regresiva en las coordenadas (x, y) y desde limite_inferior hasta limite_superior
void cuenta_regresiva(int x, int y, int limite_inferior, int limite_superior)
{
   setCColor(BLANCO_BRILLANTE);
   for (int i = limite_superior; i >= limite_inferior; --i) {
      gotoxy2(x, y); cout << i << "\a";
      Sleep(750);
   }
   gotoxy2(x - 4, y); cout << "Empieza!!";
   Sleep(750);
   pintar_mapa(mapa, FILAS_MAPA, COLUMNAS_MAPA);
}

void reinicia_personajes(PACMAN &pacman, vector<FANTASMA> &fantasmas) 
{
   pacman.reiniciaCoordenadas();
   pacman.setDireccion(DIRECCION_IZQUIERDA);
   
   for (int i = 0; i < fantasmas.size(); ++i) {
      fantasmas[i].reiniciar_coordenadas();  // Establece sus coordenadas iniciales
      fantasmas[i].setModo(MODO_CAZADOR); // Establece su modo inicial de todos
   }
}

// Función que determina si el caracter c es alguno de los siguientes caracteres
/* v : vertical
   h: horizontal
   m: medio y hacia arriba   _l_
   n: medio y hacia abajo    -j- 
   o: medio y hacia la izquierda -|
   p: medio y hacia la derecha   |-
   a:  esquina superior izquierda 
   b:  esquina superior derecha
   c:  esquina inferior izquierda
   d:  esquina inferior derecha
*/
bool esPared(char c) 
{
   
   if (c == 'v' || 
       c == 'h' ||
       c == 'm' ||
       c == 'n' ||
       c == 'o' ||
       c == 'p' ||
       c == 'a' ||
       c == 'b' ||
       c == 'c' ||
       c == 'd') {
      return true;
      
   } else {
      return false;
   }
}








// Pinta el mapa
void pintar_mapa(char mapa[][COLUMNAS_MAPA + 1], int filas, int columnas) {
   /* 
   
   v : vertical
   h: horizontal
   m: medio y hacia arriba   _l_
   n: medio y hacia abajo    -j- 
   o: medio y hacia la izquierda -|
   p: medio y hacia la derecha   |-
   a:  esquina superior izquierda
   b:  esquina superior derecha
   c:  esquina inferior izquierda
   d:  esquina inferior derecha
   @: Es la píldora mágica
   */
   
   /* Pinta todo el cuerpo */

   for (int i = 0; i < filas; ++i) {
      for (int j = 0; j < columnas; ++j) {
         
         setCColor(COLOR_PARED);
         gotoxy2(LIMITE_IZQUIERDO + j, LIMITE_SUPERIOR + i);
         
         
         switch (mapa[i][j]) {
            
            case 'v':
               cout << char(186);
               break;
            
            case 'h':
               cout << char(205);
               break;
            
            case 'm':
               cout << char(202);
               break;
            
            case 'n':
               cout << char(203);
               break;
               
            case 'o':
               cout << char(185);
               break;
            
            case 'p':
               cout << char(204);
               break;
               
            case 'a':
               cout << char(201);
               break;
               
            case 'b':
               cout << char(187);
               break;
               
            case 'c':
               cout << char(200);
               break;
               
            case 'd':
               cout << char(188);
               break;
            /*case 's': case '0':  // En estos casos no se imprime nada en el mapa. Solo son caracteres que sirven para que pacman se vaya hacia arriba
               cout << " ";*/
               
            case '·':   // La comida de pacman
               setCColor(COLOR_COMIDA);
               cout << char(250);
               break;
            
            case '@':   // la píldora
               setCColor(COLOR_COMIDA);
               cout << 'o';
               break;
               
            default:
               cout << " ";
         }
         
      }
   }
}


void gotoxy2(int x, int y)
{
   HANDLE hCon;
   hCon = GetStdHandle(STD_OUTPUT_HANDLE);
   COORD dwPos;
   dwPos.X = x;
   dwPos.Y = y;
   
   SetConsoleCursorPosition(hCon, dwPos);
}


void imprimir_informacion(class PACMAN p, int tecla_presionada)
{
   static int j = 0;
   
   gotoxy2(LIMITE_DERECHO + 10, 2); cout << "Tecla presionada: " << tecla_presionada 
      << " \'" << (char)tecla_presionada << "\'" << endl;
   gotoxy2(LIMITE_DERECHO + 10, 3); cout << (j++) << endl;  // Tiempo relativo
   gotoxy2(LIMITE_DERECHO + 10, 4); cout << "Direccion: ";
   
   switch (p.getDireccion()) {
      case DIRECCION_IZQUIERDA:
         cout << "Izquierda";
         break;
      
      case DIRECCION_DERECHA:
         cout << "Derecha";
         break;
         
      case DIRECCION_ARRIBA:
         cout << "Arriba";
         break;
         
      case DIRECCION_ABAJO:
         cout << "Abajo";
         break;
      
      default:
         cout << "Desconocido";
   }

   gotoxy2(LIMITE_DERECHO + 10, 6); cout << "Posicion absoluta:   (" << p.getX() << ", " << p.getY() << ")";
   gotoxy2(LIMITE_DERECHO + 10, 7); cout << "Posicion relativa:   (" << p.getXRelativoAlMapa() << ", " << p.getYRelativoAlMapa() << ")";
   gotoxy2(LIMITE_DERECHO + 10, 8); cout << "Puntaje: " << p.getPuntaje() << endl;
   gotoxy2(LIMITE_DERECHO + 10, 9); cout << "(a_x, a_y) == : " << p.getAnteriorX() << ", " << p.getAnteriorY() << ")" << endl;
   gotoxy2(LIMITE_DERECHO + 10, 11); cout << "Cantidad comida == : " << cantidad_comida << endl;
   
}

// Captura la tecla presionada y la modifica en caso se presione
void captura_tecla(int &tecla) 
{
   if (kbhit()) {
      tecla = getch();
      
      if (tecla == 's') {
         
      } else if (tecla != TECLA_IZQUIERDA && tecla != TECLA_DERECHA && tecla != TECLA_ARRIBA && tecla != TECLA_ABAJO) {
      // Si presiono una tecla invalida, es como si hubiesemos presionado nada
      tecla = -1;
      }
      
   } 
   // Si no presiono nada, conserva la tecla presionada para que pacman se vaya por esa dirección ni bien pueda,
   // esto hace que su movimiento sea mejor
}

// BORRRAR ESTA FUNCIÓN, ES SOLO DE PRUEBA
void PINTA_MATRIZ_MOVIMIENTOS() 
{
   for (int i = 0; i < FILAS_MAPA; ++i) {
      for (int j = 0; j < COLUMNAS_MAPA; ++j) {
         gotoxy2(j + LIMITE_DERECHO + 5, i + LIMITE_SUPERIOR);
         
         switch (matriz_movimientos[i][j]) {
            case -1:
               setCColor(0x007);
               cout << "a";
               setCColor(0x00E);
               break;
               
            case DIRECCION_ARRIBA:
               cout << "^";
               break;
            
            case DIRECCION_ABAJO:
               cout << "v";
               break;
               
            case DIRECCION_IZQUIERDA:
               cout << "<";
               break;
            
            case DIRECCION_DERECHA:
               cout << ">";
               break;
         }
      }
   }
}

void inicializaMatrizMovimientos() 
{
   for (int i = 0; i < FILAS_MAPA; ++i) {
      for (int j = 0; j < COLUMNAS_MAPA; ++j) {
         matriz_movimientos[i][j] = DIRECCION_INVALIDA; // Lo inicializo con cualquier valor distinto de las direcciones para que no todo esté aleatorio
      }
   }
}



/******************************* DEFINICIÓN DE CLASES **************************/
/* 
void PERSONAJE::PERSONAJE(int x, int y, char simbolo) {
   this->setX(x);
   this->setY(y);
   this->setSimbolo(simbolo);
}
*/

PERSONAJE::PERSONAJE(int color, int _x, int _y, int _simbolo) : x(_x), y(_y), simbolo(_simbolo) {
   this->setColor(color);
   this->pintar();
   this->setDireccion(DIRECCION_IZQUIERDA); // Direccion por defecto

   this->anteriorX = 0;
   this->anteriorY = 0;
}

void PERSONAJE::setX(int x) {
   this->x = (x >= 0 ? x : 0);
   // x = (x > 0 ? x : 1); // Cuidado con este error lógico, allí no estaría modificando el atributo
}

void PERSONAJE::setY(int y) {
   this->y = (y >= 0 ? y : 0);
}

void PERSONAJE::setSimbolo(int simbolo) {
   this->simbolo = simbolo;
}

void PERSONAJE::setNombre(string nombre) {
   this->nombre = nombre;
}

string PERSONAJE::getNombre() {
   return this->nombre;
}
/* Los valores que puede tener la variable direccion son 4:
   - DIRECCION_IZQUIERDA
   - DIRECCION_DERECHA
   _ DIRECCION_ARRIBA
   _ DIRECCION_ABAJO
   - DIRECCION_INVALIDA : El personaje tendrá este valor de dirección cuando no se va a mover
   
   Estas direcciones permitirán saber para donde se va a mover, o en el caso de
   la DIRECCION_INVALIDA que no se va a mover
*/
void PERSONAJE::setDireccion(int teclaDireccional) {
   int d;

   if ((d = this->direccionVerdadera(teclaDireccional)) != -2) {  // Solo le asigno una dirección valida
      this->direccion = d;
   } else this->direccion = DIRECCION_INVALIDA;
   /* En caso que se presione una tecla incorrecta, No se modifica el valor actual de la dirección */
}

int PERSONAJE::getX() {
   return this->x;
}

int PERSONAJE::getY() {
   return this->y;
}

int PERSONAJE::getAnteriorX() {
   return this->anteriorX;
}

int PERSONAJE::getAnteriorY(){
   return this->anteriorY;
}

int PERSONAJE::getXRelativoAlMapa() {
   return this->x - LIMITE_IZQUIERDO;
}

int PERSONAJE::getYRelativoAlMapa() {
   return this->y - LIMITE_SUPERIOR;
}

int PERSONAJE::getAnteriorXRelativoAlMapa() {
   return this->anteriorX - LIMITE_IZQUIERDO;
}

int PERSONAJE::getAnteriorYRelativoAlMapa() {
   return this->anteriorY - LIMITE_SUPERIOR;
}

int PERSONAJE::getSimbolo() {
   return this->simbolo;
}


int PERSONAJE::getDireccion() {
   return this->direccion;
}

/* TENGO QUE PASARLE LA TECLA DIRECCIONAL PRESIONADA: TECLA_IZQUIERDA, TECLA_DERECHA, TECLA ARRIBA, TECLA_ABAJO */
void PERSONAJE::mover(int tecla_direccional) {
   
   // Vamos a ver que el usuario haya presionado una tecla, osea, que tecla sea distinto de -1
   if (tecla_direccional != -1) {
      
      // Si no es posible cambiar de dirección
      if (!this->esPosibleIrA(tecla_direccional)) {
         
         // Y si no se puede seguir llendo en la dirección actual
         if (!this->esPosibleIrA(this->getDireccion())) {
            
            // NO SE DEBE MOVER: Establecemos que tenga una direccion invalida
            this->setDireccion(DIRECCION_INVALIDA);
         }
      } else { // Sí es que es posible cambiar de direccion
         this->setDireccion(tecla_direccional);
      }
      
   } else { // Si no se presionó una tecla. Solo va a seguir moviéndose si es que se puede ir en su dirección actual
   
      // Si no puede seguir yendo en su dirección actual, se detiene
      if (!this->esPosibleIrA(this->getDireccion())) {
         
         this->setDireccion(DIRECCION_INVALIDA);   // Para que deje de moverse
      }
   }
   
   this->borrar(); // Borramos al personaje de su posición actual
   
   
   // Ya tengo la dirección hacia donde me moverá
   
   // Si es PACMAN el que se mueve:
   if (this->getNombre() == "pacman") { // guardo los movimientos de pacman en la matriz
      matriz_movimientos[this->getY()][this->getX()] = this->getDireccion();
   }
   
   
   // Antes de alterar las coordenadas de un personaje, hay que guardar las anteriores
   this->anteriorX = this->getX();
   this->anteriorY = this->getY();
   
   switch (this->getDireccion()) {
      
      case DIRECCION_ARRIBA:
         this->setY(this->y - 1);
         break;
         
      case DIRECCION_ABAJO:
         this->setY(this->y + 1);
         break;
      
      case DIRECCION_DERECHA:
         this->setX(this->x + 1);
         
         if (this->getX() > LIMITE_DERECHO) {   // Si pacman logró salirse de los bordes del mapa, significa que allí no había pared y que es el atajo derecho
            this->setX(LIMITE_IZQUIERDO);   // Se establece la nueva coordenada
         }
         break;
      
      case DIRECCION_IZQUIERDA:
         this->setX(this->x - 1);
         
         if (this->getX() < LIMITE_IZQUIERDO) {   // Si pacman logró salirse de los bordes del mapa, significa que allí no había pared y que es el atajo izquierdo
            this->setX(LIMITE_DERECHO);   // Se establece la nueva coordenada
         }
         
         break;
   }
   
   // Pintamos la comida si es que esta no fue comida por pacman. Ya que los fantasmas al moverse, lo pueden borrar
   int a_mx = this->getAnteriorXRelativoAlMapa();  // Obtenemos la posición anterior del personaje
   int a_my = this->getAnteriorYRelativoAlMapa();
   
   // Evitamos borrar la comida
   if (mapa[a_my][a_mx] == '·') {
   // if (mapa[a_my][a_mx] != ' ' && !esPared(mapa[a_my][a_mx]) && mapa[a_my][a_mx] != 's' && mapa[a_my][a_mx] != '0') {
      setCColor(COLOR_COMIDA); gotoxy2(anteriorX, anteriorY); cout << char(250);               // Evita que se borre la comida cuando pasa un fantasma 
   }
   
   
   // Pintamos al personaje en su nueva posición
   this->pintar();
}

void PERSONAJE::setColor(int color) {
   this->color = color;
}

int PERSONAJE::getColor() {
   return this->color;
}

void PERSONAJE::pintar() {
   setCColor(this->getColor());
   gotoxy2(x, y);
   cout << (char)getSimbolo();
}

void PERSONAJE::borrar() {
   gotoxy2(x, y);
   cout << " ";
}



/*
A esta función le puedo pasar la dirección directamente: 
   DIRECCION_ARRIBA, DIRECCION_ABAJO, DIRECCION_IZQUIERDA, DIRECCION_DERECHA
O sino le puedo pasar la tecla presionada y si es la correcta:
   TECLA_ARRIBA, TECLA_ABAJO, TECLA_IZQUIERDA, TECLA_DERECHA
   
esta función retorna la dirección correcta correspondiente:
   DIRECCION_ARRIBA, DIRECCION_ABAJO, DIRECCION_IZQUIERDA, DIRECCION_DERECHA
Si se le pasa una tecla no válida, retorna -2, que significa que no debe de asignarse esta direccion
*/
int PERSONAJE::direccionVerdadera(int teclaDireccional) {
   if (teclaDireccional == DIRECCION_ARRIBA ||
       teclaDireccional == DIRECCION_ABAJO ||
       teclaDireccional == DIRECCION_IZQUIERDA ||
       teclaDireccional == DIRECCION_DERECHA ||
       teclaDireccional == DIRECCION_INVALIDA) {   // Para que no se mueva
       return teclaDireccional;
    
    } else {
    
      if (teclaDireccional == TECLA_IZQUIERDA)
         return DIRECCION_IZQUIERDA;
         
      else if (teclaDireccional == TECLA_DERECHA)
         return DIRECCION_DERECHA;
         
      else if (teclaDireccional == TECLA_ARRIBA)
         return DIRECCION_ARRIBA;
         
      else if (teclaDireccional == TECLA_ABAJO)
         return DIRECCION_ABAJO;
         
      else
         return -2;
   }
}

bool PERSONAJE::esPosibleIrA(int posible_direccion) {
   bool puede_ir = true;
   
   // Obtenemos las coordenadas de PACMAN respecto del mapa
   int mx = this->getXRelativoAlMapa();
   int my = this->getYRelativoAlMapa();
   
   // Calculamos la posible dirección donde nos queremos mover
   switch (posible_direccion) {
      
      case TECLA_ARRIBA: case DIRECCION_ARRIBA:
         my -= 1;
         break;
         
      case TECLA_ABAJO: case DIRECCION_ABAJO:
         my += 1;
         break;
         
      case TECLA_IZQUIERDA: case DIRECCION_IZQUIERDA:
         mx -= 1;
         break;
         
      case TECLA_DERECHA: case DIRECCION_DERECHA:
         mx += 1;
         break;
   }
   
   if (esPared(mapa[my][mx])) {
      puede_ir = false;
   }
   
   return puede_ir;
}




PACMAN::PACMAN(int color, int x, int y, int simbolo) : PERSONAJE(color, x, y, simbolo) {
   this->setDireccion(DIRECCION_IZQUIERDA);
   this->setNombre("pacman");
   this->setPuntaje(0);
   this->setVidas(VIDAS_PACMAN);
   this->setSigueJugando(true);
}

void PACMAN::setPuntaje(int puntaje) {
   this->puntaje = (puntaje > 0) ? puntaje : 0;
}

int PACMAN::getPuntaje() {
   return puntaje;
}

void PACMAN::actualizar_puntaje() {
   int mx = this->getXRelativoAlMapa();
   int my = this->getYRelativoAlMapa();
   
   if (mapa[my][mx] == '·' ||mapa[my][mx] == '@') {
      this->setPuntaje(this->getPuntaje() + 1);
      mapa[my][mx] = ' ';
   }
}

int PACMAN::getVidas() {
   return this->vidas;
}

void PACMAN::setVidas(int vidas) {
   this->vidas = (vidas > 0) ? vidas : 0;
}

bool PACMAN::getSigueJugando() {
   return this->sigue_jugando;
}

void PACMAN::setSigueJugando(bool sigue_jugando) {
   this->sigue_jugando = sigue_jugando;
}

int PACMAN::estadoFinalPacman() {
   if (this->getPuntaje() == cantidad_comida) {
      return PACMAN_GANADOR;
   }
}

bool PACMAN::choque(vector <FANTASMA> *fantasmas, int &modo_fantasma, FANTASMA **fantasma) {
   // Retorna el modo del fantasma con el que chocó
   // Retorna un puntero al fantasma chocado para poder cambiar de él su modo, su color y demás cosas
   
   // Coordenadas absolutas de Pacman
   int x = this->getX();
   int y = this->getY();
   int anterior_px = this->getAnteriorX();
   int anterior_py = this->getAnteriorY();
   
   // Coordenadas absolutas de cada fantasma
   int fx, fy, anterior_fx, anterior_fy;
   
   for (int i = 0; i < fantasmas->size(); ++i) {
      // Obtenemos las coordenadas del fantasma actual
      fx = (*fantasmas)[i].getX();
      fy = (*fantasmas)[i].getY();
      anterior_fx = (*fantasmas)[i].getAnteriorX();
      anterior_fy = (*fantasmas)[i].getAnteriorY();
      
      // El choque se produce cuando coinciden sus coordenadas o sino, en caso que
      // se crucen al mismo tiempo sin coincidir sus coordenadas, el choque se produce
      // si conciden sus coordenadas anteriores de uno con sus coordenadas actuales del otro
      if ((x == fx && y == fy) || (x == anterior_fx && y == anterior_fy)) {
         modo_fantasma = (*fantasmas)[i].getModo();   // Guardamos el modo en que estaba el fantasma
         (*fantasma) = &((*fantasmas)[i]);     // Guardamos la dirección del fantasma con el que choqué para aplicarle los respectivos cambios de símbolo

         gotoxy2(fx, fy); cout << " "; // borramos al fantasma para que desparezca
         return true;
      }
   }
   
   return false;
}

void PACMAN::imprimirVidas() {
   setCColor(COLOR_PACMAN);
   gotoxy2(LIMITE_IZQUIERDO, LIMITE_INFERIOR + 1);
   cout << "       ";
   
   int vidas = this->getVidas();
   
   for (int i = 0; i < vidas; ++i) {
      gotoxy2(LIMITE_IZQUIERDO + i + 1, LIMITE_INFERIOR + 1);
      printf("%c", CARACTER_PACMAN);
   }
}

void PACMAN::reiniciaCoordenadas() {
   this->setX(LIMITE_IZQUIERDO + 14);
   this->setY(LIMITE_SUPERIOR + 15);
}

bool PACMAN::comioPildora() {
   
   int mx = this->getXRelativoAlMapa();
   int my = this->getYRelativoAlMapa();
   
   if (mapa[my][mx] == '@') { // Significa que comió la píldora
      return true;
   } else {
      return false;
   }
}


FANTASMA::FANTASMA(int color, int x, int y, int simbolo) : PERSONAJE(color, x, y, simbolo) {
   this->setDireccion(DIRECCION_DERECHA);
   this->setNombre("fantasma");
   
   // Modo por defecto es cazador
   this->setModo(MODO_CAZADOR);
   this->setTemporizador(0);
   this->temporizador_activado = false;
}

void FANTASMA::mover() {
   // DUDA:
   /*
      En la superclase PERSONAJE, se tiene la función virtual mover(int) con un argumento.
      En la subclase FANTASMA, se está redefiniendo dicha función mediante este nuevo 
      método mover declarado otra vez en FANTASMA pero con otra funcionalidad.
      Lo malo es que aquí no se necesita el argumento int.
      
      Si en caso, declaro la función mover() sin argumentos en la subclase
      FANTASMA, estaría redefiniendo la función virtual mover(int) de la superclase?
      Lo pregunto, porque yo pensé que no lo redefinía, entonces, en ese caso, en
      la subclase contaría con dos métodos: 
            mover() sin argumento, heredado del padre OBJETO
            mover(int) con un argumento, declarado en el hijo FANTASMA
      Y si mi suposición fuera correcta, al llamar a cualquiera de estas funciones,
      mi programa no tendría error y se ejecutaría la función correspondiente.
      Pero cuando intento llamar a la función mover(int) con argumento que
      supuestamente he heredado del padre, me genera un error
      error
   */
   /*
   if (fantasma salio de su casa) {
      salio = true;
   }*/
   
   int direccion = this->getDireccion(); // Dirección hacia donde tienen intención de moverse los fantasmas

   // Si de la matriz de movimientos de pacman, estamos en un lugar donde pacman no ha pasado
   if (matriz_movimientos[this->getY()][this->getX()] == DIRECCION_INVALIDA) {
      
      /* Truco para que se salga de la casita donde están ////////////////////////////*/
      int mx = this->getXRelativoAlMapa();
      int my = this->getYRelativoAlMapa();
      
      if (mapa[my][mx] == '2') {
         direccion = DIRECCION_ARRIBA;
         
      } else if (mapa[my][mx] == 'i') {
         direccion = rand() % 2; // Se mueve a la izquierda o a la derecha aleatoriamente
         
      } else {
         direccion = rand() % 4; // Se mueve aleatoriamente hacia cualquier dirección
      }
      /********************************************************************************/
      
   } else if (matriz_movimientos[this->getY()][this->getX()] == DIRECCION_IZQUIERDA ||
              matriz_movimientos[this->getY()][this->getX()] == DIRECCION_DERECHA ||
              matriz_movimientos[this->getY()][this->getX()] == DIRECCION_ARRIBA ||
              matriz_movimientos[this->getY()][this->getX()] == DIRECCION_ABAJO) { // Significa que debe seguir a pacman
              
         direccion = matriz_movimientos[this->getY()][this->getX()];
   }
   
   /* ESTE CODIGO ERPETIDO ME PERMITE QUE LOS FANTASMAS NO ENTREN A SU CASA O.o , analizar mejor*/
   int mx = this->getXRelativoAlMapa();
   int my = this->getYRelativoAlMapa();
   
   if (mapa[my][mx] == '2') {
      direccion = DIRECCION_ARRIBA;
      
   } else if (mapa[my][mx] == 'i') {
      direccion = rand() % 2;
   }
   /*********************************************************************************************/
   
   PERSONAJE::mover(direccion); // Llamada al método mover de la clase padre PERSONAJE
   /*
   gotoxy(getX(), getY());
   this->pintar(); // Llama al método heredado pintar
   */
   
   if (temporizador_activado) {
      this->setTemporizador(this->getTemporizador() + 1);
      if (this->getTemporizador() >= TIEMPO_ZOMBIE) {
         this->setModo(MODO_CAZADOR);
      }
   }
}

void FANTASMA::setModo(int modo) {
   this->modo = (modo == MODO_CAZADOR || modo == MODO_PRESA || modo == MODO_INVISIBLE) ? modo : MODO_CAZADOR;
   
   if (modo == MODO_CAZADOR) {
      this->temporizador_activado = false;
      this->setTemporizador(0); // No se cuenta el tiempo
      this->setColorPredeterminado();  // Regresa a su color original
      
   } else if (modo == MODO_PRESA) {
      this->temporizador_activado = true;
      this->setTemporizador(0);
      this->setColor(COLOR_FANTASMA_ZOMBIE); // Se convierte a color zombie
   }
}

int FANTASMA::getModo() {
   return this->modo;
}

void FANTASMA::reiniciar_coordenadas() {
   string nombre_fantasma = this->getNombre();
   
   if (nombre_fantasma == "Fantasma Rojo") {
      this->setX(LIMITE_IZQUIERDO + 14);
      this->setY(LIMITE_SUPERIOR + 9);
      
   } else if (nombre_fantasma == "Fantasma Rosado") {
      this->setX(LIMITE_IZQUIERDO + 13);
      this->setY(LIMITE_SUPERIOR + 11);
      
   } else if (nombre_fantasma == "Fantasma Celeste") {
      this->setX(LIMITE_IZQUIERDO + 14);
      this->setY(LIMITE_SUPERIOR + 11);
      
   } else if (nombre_fantasma == "Fantasma Verde") {
      this->setX(LIMITE_IZQUIERDO + 15);
      this->setY(LIMITE_SUPERIOR + 11);
   }
}

void FANTASMA::setColorPredeterminado() {
   string nombre_fantasma = this->getNombre();
   int color;
   
   if (nombre_fantasma == "Fantasma Rojo") {
      color = COLOR_FANTASMA1;
      
   } else if (nombre_fantasma == "Fantasma Rosado") {
      color = COLOR_FANTASMA2;
      
   } else if (nombre_fantasma == "Fantasma Celeste") {
      color = COLOR_FANTASMA3;
      
   } else if (nombre_fantasma == "Fantasma Verde") {
      color = COLOR_FANTASMA4;
   }
   
   this->setColor(color);
}

int FANTASMA::getTemporizador() {
   return temporizador;
}

void FANTASMA::setTemporizador(int temporizador) {
   this->temporizador = temporizador;
}

// Fantasma Rojo
FANTASMA_ROJO::FANTASMA_ROJO() 
   : FANTASMA(COLOR_FANTASMA1, LIMITE_IZQUIERDO + 14, LIMITE_SUPERIOR + 9, CARACTER_FANTASMA) {
   this->setNombre("Fantasma Rojo");
}

void FANTASMA_ROJO::reiniciar_coordenadas() {
   this->setX(LIMITE_IZQUIERDO + 14);
   this->setY(LIMITE_SUPERIOR + 9);
}

// Fantasma Rosado
FANTASMA_ROSADO::FANTASMA_ROSADO()
   : FANTASMA(COLOR_FANTASMA2, LIMITE_IZQUIERDO + 13, LIMITE_SUPERIOR + 11, CARACTER_FANTASMA) {
   this->setNombre("Fantasma Rosado");
}

void FANTASMA_ROSADO::reiniciar_coordenadas() {
   this->setX(LIMITE_IZQUIERDO + 13);
   this->setY(LIMITE_SUPERIOR + 11);
}

// Fantasma Celeste
FANTASMA_CELESTE::FANTASMA_CELESTE()
   : FANTASMA(COLOR_FANTASMA3, LIMITE_IZQUIERDO + 14, LIMITE_SUPERIOR + 11, CARACTER_FANTASMA) {
   this->setNombre("Fantasma Celeste");
}

void FANTASMA_CELESTE::reiniciar_coordenadas() {
   this->setX(LIMITE_IZQUIERDO + 14);
   this->setY(LIMITE_SUPERIOR + 11);
}
   
// Fantasma Verde
FANTASMA_VERDE::FANTASMA_VERDE()
   : FANTASMA(COLOR_FANTASMA4, LIMITE_IZQUIERDO + 15, LIMITE_SUPERIOR + 11, CARACTER_FANTASMA) {
   this->setNombre("Fantasma Verde");
}

void FANTASMA_VERDE::reiniciar_coordenadas() {
   this->setX(LIMITE_IZQUIERDO + 15);
   this->setY(LIMITE_SUPERIOR + 11);
}
