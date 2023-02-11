#include <stdio.h> //libreria principal
#include <stdlib.h> //libreria complementaria para usar el srand
#include <ctype.h> // para los caracteres de puntuacion
#include <time.h> // libreria para hacer numeros randoms

char board[3][3]; //se define el array que sera el tablero, se hace tipo char para que pueda tener letras como X , O
const char jugador1 = 'X'; // se define el jugador 1 como una x en el tablero
const char jugador2 = 'O'; // se define la maquina (jugador 2) como un o en el tablero

void reset();// funcion para resetear el tablero
void tablero();//funcion para dibujar el tablero 
int espacio();// funcion para ver si quedan espacios vacios en el tablero
void jugador();// funcion para colocar una X en el tablero segun la cordenada del jugador
void pc();// funcion para colocar un o en un lugar aleatorio del tablero
char ganador(); //
void mensajeganador(char); // funcion que dice el mensaje al final si se gana, se pierde, o 

int main()
{
   char winner;
   int respuesta;

   do  //dowhile comprueba ganador
   {
      winner = ' ';
      
      reset();

      while(winner == ' ' && espacio() != 0)
      {
         tablero();

         jugador();
         winner = ganador();
         if(winner != ' ' || espacio() == 0)
         {
            break;
         }
		 tablero();
         pc();
         winner = ganador();
         if(winner != ' ' || espacio() == 0)
         {
            break;
         }
      }

      tablero();
     mensajeganador(winner);

      printf("\nte gustaria jugar otra vez ? (si = 1/no = 0): ");
      scanf("%c");
      scanf("%i", &respuesta);
   } while (respuesta == 1);

   printf("gracias por jugar");

   return 0;
}

void reset() //void encargado de resetear el juego
{
   for(int i = 0; i < 3; i++)
   {
      for(int j = 0; j < 3; j++)
      {
         board[i][j] = ' ';
      }
   }
}
void tablero() //void encargado de crear el tablero
{
   printf("\n");
   printf(" %c | %c | %c ", board[0][0], board[0][1], board[0][2]);
   printf("\n---|---|---\n");
   printf(" %c | %c | %c ", board[1][0], board[1][1], board[1][2]);
   printf("\n---|---|---\n");
   printf(" %c | %c | %c ", board[2][0], board[2][1], board[2][2]);
   printf("\n");
}
int espacio()
{
   int casillas = 9;

   for(int i = 0; i < 3; i++)
   {
      for(int j = 0; j < 3; j++)
      {
         if(board[i][j] != ' ')
         {
            casillas--;
         }
      }
   }
   return casillas;
}
void jugador()
{
   int x,y;

   do
   {
      printf("\njugador 1 elija una fila #(1-3): ");
      scanf("%d", &x);
      x--;
      printf("\njugador 1 elija una columna #(1-3): ");
      scanf("%d", &y);
      y--;

      if(board[x][y] != ' ')
      {
         printf("\nmovimiento invalido");
      }
      else
      {
         board[x][y] = jugador1;
         break;
      }
   } while (board[x][y] != ' ');
   
}
void pc()
{
   int x,y;

   do
   {
      printf("\njugador 2 elija una fila #(1-3): ");
      scanf("%d", &x);
      x--;
      printf("\njugador 2 elija una columna #(1-3): ");
      scanf("%d", &y);
      y--;

      if(board[x][y] != ' ')
      {
         printf("\nmovimiento invalido");
      }
      else
      {
         board[x][y] = jugador2;
         break;
      }
   } while (board[x][y] != ' ');
}
char ganador()// chequea las condiciones para el ganador
{
   //se chequean las filas
   for(int i = 0; i < 3; i++)
   {
      if(board[i][0] == board[i][1] && board[i][0] == board[i][2])
      {
         return board[i][0];
      }
   }
   //se chequean las columnas
   for(int i = 0; i < 3; i++)
   {
      if(board[0][i] == board[1][i] && board[0][i] == board[2][i])
      {
         return board[0][i];
      }
   }
   //se chequean las diagonales
   if(board[0][0] == board[1][1] && board[0][0] == board[2][2])
   {
      return board[0][0];
   }
   if(board[0][2] == board[1][1] && board[0][2] == board[2][0])
   {
      return board[0][2];
   }

   return ' ';
}
void mensajeganador(char winner)  //chequea quien fue el ganador y asigna lavictoria correspondiente 
{
   if(winner == jugador1)
   {
      printf("HA GANADO EL JUGADOR 1");
   }
   else if(winner == jugador2)
   {
      printf("HA GANADO EL JUGADOR 2");
   }
   else{
      printf("EMPATE");
   }
}


