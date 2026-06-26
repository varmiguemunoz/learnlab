#include <stdio.h>
#include <stdbool.h>

/* 
Ejercicio: Sistema de asientos de un cine

Debes representar una sala de cine de 5 filas y 8 columnas utilizando una matriz.

Al iniciar el programa, todos los asientos deben estar libres.

El usuario podrá:

1.Mostrar la sala.
2.Reservar un asiento.
3.Cancelar una reserva.
4.Consultar cuántos asientos quedan disponibles.
5.Salir.
*/

void Movies(void)
{
    const int F = 5;
    const int C = 8;
  
    int user, indice, fila, columna, seet;
    int seets[F][C];

    bool continuar = true;

    for (int f = 0; f < 5; f++)
    {
        for (int c = 0; c < 8; c++)
        {
            seets[f][c] = 0;
        }
    }

  while(continuar == true)  {
    printf("Que deseas hacer hoy?\n");
    printf("\nMENU PRINCIPAL\n"
           "\n1. Mostrar la sala\n"
           "2. Reservar un asiento\n"
           "3. Cancelar una reserva\n"
           "4. Consultar cuantos asientos quedan disponibles\n"
           "5. Salir\n");

    printf("Digita el numero de la accion (1,2,3,4,5):\n");
    scanf("%i", &user);
  
 
    switch (user)
    {
        case 1:
            for (int f = 0; f < 5; f++)
            {
                printf("Fila %i", f);

                for (int c = 0; c < 8; c++)
                {
                    printf("%-4d", seets[f][c]);
                }

                printf("\n");
            }
            break;

        case 2:
        {

            printf("Digita el asiento que quieres reservar (1-40):\n");
            scanf("%i", &seet);
 
            if (seet < 1 && seet > 40)
              {
                printf("El numero debe ser entre (1 y 40)\n");
              }

            indice = seet - 1;
            fila = indice / C;
            columna = indice % C;

            if (seets[fila][columna] == 1)
            {
                printf("La posicion no esta disponible\n");
            }
            else if (seets[fila][columna] == 0)
            {
                seets[fila][columna] = 1;
                printf("Posicion asignada correctamente\n");
            }

            break;
        }

        case 3:
          printf("Digita el asiento que quieres cancelar (1-40):\n");
          scanf("%i", &seet);

        {
            if (seets[fila][columna] == 1)
            {
                seets[fila][columna] = 0;
                printf("Asiento cancelado con exito\n");
            }
            else if (seets[fila][columna] == 0)
            {
                printf("La posicion ya se encuentra vacia\n");
            }

            break;
        }

        case 4:
        {
            int seets_enable = 0;

            for (int f = 0; f < 5; f++)
            {
                for (int c = 0; c < 8; c++)
                {
                    if (seets[f][c] == 0)
                    {
                        seets_enable++;
                    }
                }
            }

            printf("Asientos disponibles: %i\n", seets_enable);

            break;
        }

        case 5:
            printf("Saliendo del programa.\n");
            continuar = false;
            break;

        default:
            printf("Inserta una de las opciones del menu\n");
            break;
    }
  }
}
