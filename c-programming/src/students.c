#include <stdio.h>
#include <stdbool.h>

int students(){
  int usuarios;

  int user_note;
  char user_continue;

  bool shouldContinue = true;
  bool isTheLetterInvalid = false;

  while (shouldContinue==true) {
  if (isTheLetterInvalid == false) {
  printf("Cuantos usuarios quieres registrar?");
  scanf(" %i", &usuarios);

  if(usuarios > 10){
    printf("El numero maximo de estudiantes permitidos es 10. \n");
  } else if (usuarios < 1) {
    printf("Debe registrar por lo menos a un estudiante. \n");
  } else if (usuarios >= 1 && usuarios <= 10) {

    float nota[usuarios]; //comportamiento indefinido debe inicializarse con el total del arreglo siempre
    
    int nota_alta;
    int nota_baja;
    int estudantes_aprobados;
    int promedio;

    for(int i = 0; i < usuarios; i++){
      printf("Digite la nota de cada estudiante: ");
      scanf(" %i", &user_note);
      nota[i] = user_note;
      printf("Nota agregada con exito!");
     }

    for(int j =0; j<usuarios; j++){
          printf("Estudiante %d: %.1f\n", j + 1, nota[j]);
    }

    //calculos
  }

      printf("Desea volver a registrar estudiantes? Y o N ");
      scanf(" %c", &user_continue);

      if (user_continue == 'Y') {
        shouldContinue=true;
      } else if (user_continue == 'N') {
        shouldContinue=false;
        break;
      } else {
        printf("Por favor digite un una letra valida Y o N \n");
        isTheLetterInvalid=true;
      }

} else {
      printf("Desea volver a registrar estudiantes? Y o N ");
      scanf(" %c", &user_continue);

      if (user_continue == 'Y') {
        shouldContinue=true;
        isTheLetterInvalid=false;
      } else if (user_continue == 'N') {
        shouldContinue=false;
        isTheLetterInvalid=false;
        break;
      } else {
        printf("Por favor digite un una letra valida Y o N \n");
        isTheLetterInvalid=true;
      }
    }
}

  return 0;
}
