## Tabla de Contenido

1. [[#Comentarios|Comentarios]]
2. [[#Constantes y Enums|Constantes y Enums]]
3. [[#Includes|Includes]]
4. [[#Prototipos de Funciones|Prototipos de Funciones]]
5. [[#Función main|Función main]]
6. [[#Tipos de Datos|Tipos de Datos]]
7. [[#Arrays|Arrays]]
8. [[#Strings|Strings]]
9. [[#Operadores|Operadores]]
10. [[#Estructuras de Control|Estructuras de Control]]
11. [[#Typecasting|Typecasting]]
12. [[#Punteros|Punteros]]
13. [[#Funciones|Funciones]]
14. [[#Tipos Definidos por el Usuario y Structs|Tipos Definidos por el Usuario y Structs]]
15. [[#Punteros a Funciones|Punteros a Funciones]]
16. [[#Imprimiendo con printf()|Imprimiendo con printf()]]
17. [[#Orden de Evaluación|Orden de Evaluación]]
18. [[#Header Files|Header Files]]

---

## Comentarios

```c
// Los comentarios de una línea comienzan con // — solo disponibles en C99 en adelante.

/*
Los comentarios multilínea se ven así. Funcionan también en C89.
*/

/*
Los comentarios multilínea NO se anidan /* Ten cuidado */  // el comentario termina aquí...
*/ // ...no aquí!
```

---

## Constantes y Enums

```c
// Constantes: #define <palabra_clave>
// Por convención se escriben en MAYÚSCULAS, pero no es obligatorio.
#define DAYS_IN_YEAR 365

// Las enumeraciones también son una forma de declarar constantes.
// Toda sentencia debe terminar con punto y coma.
enum days {SUN, MON, TUE, WED, THU, FRI, SAT};
// SUN vale 0, MON vale 1, TUE vale 2, etc.

// También se pueden especificar los valores:
enum days {SUN = 1, MON, TUE, WED = 99, THU, FRI, SAT};
// MON es 2 automáticamente, TUE es 3, etc.
// WED es 99, THU es 100, FRI es 101, etc.
```

---

## Includes

```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Los nombres entre <angle brackets> le dicen al compilador que busque
// en las librerías del sistema.
// Para tus propios headers, usa comillas dobles y proporciona la ruta:
#include "my_header.h"               // archivo local
#include "../my_lib/my_lib_header.h" // ruta relativa
```

---

## Prototipos de Funciones

```c
// Declara las firmas de funciones por adelantado en un archivo .h
// o al inicio de tu archivo .c
void function_1();
int function_2(void);

// Como mínimo debes declarar un 'prototipo de función' antes de usarla.
// Normalmente los prototipos se colocan al inicio del archivo, antes de cualquier definición.
int add_two_ints(int x1, int x2); // prototipo de función
// Aunque `int add_two_ints(int, int);` también es válido (sin nombrar los args),
// se recomienda nombrar los argumentos en el prototipo para mayor claridad.

// Los prototipos no son necesarios si la definición de la función aparece
// antes de cualquier función que la llame. Sin embargo, la práctica estándar es
// siempre agregar el prototipo a un header (*.h) e incluirlo al inicio.
// Esto evita problemas cuando una función es llamada antes de que el compilador
// conozca su existencia, y además le da al desarrollador un header limpio para
// compartir con el resto del proyecto.
```

---

## Función main

```c
// El punto de entrada de tu programa es una función llamada "main".
// El tipo de retorno puede ser cualquiera, pero la mayoría de sistemas operativos
// esperan un `int` para el procesamiento de códigos de error.
int main(void) {
  // tu programa
}

// Los argumentos de línea de comandos también se pasan a main:
// argc = número de argumentos (el nombre del programa cuenta como 1)
// argv = array de arrays de chars con los argumentos en sí
// argv[0] = nombre de tu programa, argv[1] = primer argumento, etc.
int main(int argc, char** argv)
{
  // imprimir salida con printf, de "print formatted"
  // %d es un entero, \n es nueva línea
  printf("%d\n", 0); // => Imprime 0

  // leer entrada con scanf
  // '&' se usa para definir la ubicación
  // donde queremos guardar el valor ingresado
  int input;
  scanf("%d", &input);
```

---

## Tipos de Datos

```c
  // Los compiladores que NO cumplen C99 requieren que las variables se declaren
  // al inicio del bloque de scope actual.
  // Los compiladores que SÍ cumplen C99 permiten declaraciones cerca del punto de uso.
  // En este tutorial las variables se declaran dinámicamente bajo estándar C99.

  // los int suelen ser 4 bytes (usa el operador `sizeof` para verificar)
  int x_int = 0;

  // los short suelen ser 2 bytes
  short x_short = 0;

  // los char se definen como la unidad mínima direccionable del procesador.
  // Usualmente es 1 byte, pero en algunos sistemas puede ser más
  // (ej: TMS320 de TI usa 2 bytes).
  char x_char = 0;
  char y_char = 'y'; // Los literales char van entre comillas simples ''

  // los long suelen ser de 4 a 8 bytes; los long long garantizan al menos 8 bytes
  long x_long = 0;
  long long x_long_long = 0;

  // los float suelen ser números de punto flotante de 32 bits
  float x_float = 0.0f; // el sufijo 'f' denota literal de punto flotante

  // los double suelen ser números de punto flotante de 64 bits
  double x_double = 0.0; // los números reales sin sufijo son doubles

  // los tipos enteros pueden ser unsigned (mayor o igual a cero)
  unsigned short ux_short;
  unsigned int ux_int;
  unsigned long long ux_long_long;

  // los chars entre comillas simples son enteros en el set de caracteres de la máquina.
  '0'; // => 48 en el set de caracteres ASCII.
  'A'; // => 65 en el set de caracteres ASCII.

  // sizeof(T) da el tamaño en bytes de una variable de tipo T
  // sizeof(obj) da el tamaño de la expresión (variable, literal, etc.).
  printf("%zu\n", sizeof(int)); // => 4 (en la mayoría de máquinas con palabras de 4 bytes)

  // Si el argumento de `sizeof` es una expresión, dicha expresión NO se evalúa
  // (excepto en VLAs, ver más adelante).
  // El valor que retorna es una constante en tiempo de compilación.
  int a = 1;
  // size_t es un tipo entero sin signo de al menos 2 bytes, usado para
  // representar el tamaño de un objeto.
  size_t size = sizeof(a++); // a++ NO se evalúa
  printf("sizeof(a++) = %zu where a = %d\n", size, a);
  // imprime "sizeof(a++) = 4 where a = 1" (en arquitectura de 32 bits)
```

---

## Arrays

```c
  // Los arrays deben inicializarse con un tamaño concreto.
  char my_char_array[20]; // Este array ocupa 1 * 20 = 20 bytes
  int my_int_array[20];   // Este array ocupa 4 * 20 = 80 bytes
                          // (asumiendo palabras de 4 bytes)

  // Puedes inicializar un array de veinte ints en cero así:
  int my_array[20] = {0};
  // donde "{0}" se llama "inicializador de array".
  // Todos los elementos más allá de los especificados se inicializan en 0:
  int my_array[5] = {1, 2};
  // my_array tiene cinco elementos, los primeros dos son 1 y 2, el resto 0:
  // [1, 2, 0, 0, 0]

  // NOTA: puedes omitir el tamaño si inicializas el array en la misma línea:
  int my_array[] = {0};
  // El tamaño del array será el número de elementos en el inicializador.
  // Con "{0}", my_array tiene tamaño uno: [0]

  // Para evaluar el tamaño del array en runtime, divide su tamaño en bytes
  // entre el tamaño en bytes de su tipo de elemento:
  size_t my_array_size = sizeof(my_array) / sizeof(my_array[0]);
  // ADVERTENCIA: evalúa el tamaño *antes* de pasar el array a funciones,
  // porque los arrays se "degradan" a punteros crudos cuando se pasan a funciones
  // (la sentencia anterior produciría un resultado incorrecto dentro de la función).

  // Indexar un array es igual que en otros lenguajes —
  // o mejor dicho, los otros lenguajes son como C.
  my_array[0]; // => 0

  // Los arrays son mutables; ¡es solo memoria!
  my_array[1] = 2;
  printf("%d\n", my_array[1]); // => 2

  // En C99 (y como característica opcional en C11), también se pueden declarar
  // arrays de longitud variable (VLA). El tamaño no necesita ser una constante
  // en tiempo de compilación:
  printf("Enter the array size: "); // pedirle al usuario el tamaño
  int array_size;
  fscanf(stdin, "%d", &array_size);
  int var_length_array[array_size]; // declarar el VLA
  printf("sizeof array = %zu\n", sizeof var_length_array);

  // Ejemplo:
  // > Enter the array size: 10
  // > sizeof array = 40
```

### Arrays multidimensionales

```c
  int multi_array[2][5] = {
    {1, 2, 3, 4, 5},
    {6, 7, 8, 9, 0}
  };
  // acceder a elementos:
  int array_int = multi_array[0][2]; // => 3
```

---

## Strings

```c
  // Las strings son solo arrays de chars terminados por un byte NULL (0x00),
  // representado en strings como el carácter especial '\0'.
  // (No necesitamos incluir el byte NULL en literales de string; el compilador
  //  lo inserta al final del array por nosotros.)
  char a_string[20] = "This is a string";
  printf("%s\n", a_string); // %s formatea una string

  printf("%d\n", a_string[16]); // => 0
  // es decir, el byte #17 es 0 (igual que el 18, 19 y 20)

  // Si tenemos caracteres entre comillas simples, es un literal de carácter.
  // Su tipo es `int`, NO `char` (por razones históricas).
  int cha = 'a';  // válido
  char chb = 'a'; // también válido (conversión implícita de int a char)

  // Es buena práctica usar `const char *` para referirse a literales de string,
  // ya que los literales de string no deben modificarse (ej: "foo"[0] = 'a' es ILEGAL).
  const char *my_str = "This is my very own string literal";
  printf("%c\n", *my_str); // => 'T'

  // Esto no aplica si la string es un array
  // (potencialmente inicializado con un literal) que reside en memoria escribible:
  char foo[] = "foo";
  foo[0] = 'a'; // esto es legal, foo ahora contiene "aoo"
```

---

## Operadores

### Múltiples declaraciones

```c
  int i1 = 1, i2 = 2;
  float f1 = 1.0, f2 = 2.0;

  int b, c;
  b = c = 0;
```

### Aritméticos

```c
  i1 + i2; // => 3
  i2 - i1; // => 1
  i2 * i1; // => 2
  i1 / i2; // => 0 (0.5, pero truncado hacia 0)

  // Necesitas castear al menos un entero a float para obtener resultado de punto flotante
  (float)i1 / i2;  // => 0.5f
  i1 / (double)i2; // => 0.5 — igual con double
  f1 / f2;         // => 0.5, más o menos epsilon

  // Los números de punto flotante se definen por IEEE 754 y no pueden almacenar
  // valores exactos. Por ejemplo, lo siguiente no produce resultados esperados
  // porque 0.1 podría ser 0.099999999999 internamente, y 0.3 podría ser 0.300000000001.
  (0.1 + 0.1 + 0.1) != 0.3; // => 1 (true)
  // Y NO es asociativo por las razones mencionadas:
  1 + (1e123 - 1e123) != (1 + 1e123) - 1e123; // => 1 (true)
  // Esta notación es científica: 1e123 = 1*10^123

  // Es importante notar que casi todos los sistemas usan IEEE 754 para representar
  // flotantes. Incluso Python, usado en computación científica, eventualmente llama
  // a C que usa IEEE 754. Se menciona no como crítica sino como advertencia:
  // al comparar flotantes, hay que considerar un pequeño margen de error (epsilon).
```

### Módulo

```c
  // El módulo también existe, pero ten cuidado con argumentos negativos
  11 % 3;    // => 2  (11 = 2 + 3*3)
  (-11) % 3; // => -2, como se esperaría
  11 % (-3); // => 2  y no -2, lo cual es bastante contraintuitivo
```

### Comparación

```c
  // No hay tipo booleano en C. Usamos ints.
  // (C99 introdujo el tipo _Bool disponible en stdbool.h)
  // 0 es false, cualquier otra cosa es true.
  // Los operadores de comparación siempre retornan 0 o 1.
  3 == 2; // => 0 (false)
  3 != 2; // => 1 (true)
  3 > 2;  // => 1
  3 < 2;  // => 0
  2 <= 2; // => 1
  2 >= 2; // => 1

  // C NO encadena comparaciones como Python.
  // Advertencia: la línea siguiente compila, pero significa `(0 < a) < 2`.
  // Esta expresión siempre es true porque (0 < a) puede ser 1 o 0.
  // En este caso es 1, porque (0 < 1).
  int between_0_and_2 = 0 < a < 2;   // INCORRECTO
  int between_0_and_2 = 0 < a && a < 2; // CORRECTO
```

### Lógicos

```c
  // La lógica opera sobre ints
  !3;     // => 0 (NOT lógico)
  !0;     // => 1
  1 && 1; // => 1 (AND lógico)
  0 && 1; // => 0
  0 || 1; // => 1 (OR lógico)
  0 || 0; // => 0
```

### Ternario

```c
  int e = 5;
  int f = 10;
  int z;
  z = (e > f) ? e : f; // => 10  "si e > f retorna e, si no retorna f"
```

### Incremento y Decremento

```c
  int j = 0;
  int s = j++; // Retorna j LUEGO incrementa j. (s = 0, j = 1)
  s = ++j;     // Incrementa j LUEGO retorna j. (s = 2, j = 2)
  // igual con j-- y --j
```

### Bitwise

```c
  ~0x0F;       // => 0xFFFFFFF0 (negación bit a bit, "complemento a 1", resultado en int de 32 bits)
  0x0F & 0xF0; // => 0x00       (AND bit a bit)
  0x0F | 0xF0; // => 0xFF       (OR bit a bit)
  0x04 ^ 0x0F; // => 0x0B       (XOR bit a bit)
  0x01 << 1;   // => 0x02       (shift izquierda por 1)
  0x02 >> 1;   // => 0x01       (shift derecha por 1)

  // Ten cuidado al hacer shift en enteros con signo — lo siguiente es comportamiento indefinido:
  // - shift hacia el bit de signo de un entero con signo (int a = 1 << 31)
  // - shift izquierda de un número negativo (int a = -1 << 2)
  // - shift con un offset >= el ancho en bits del tipo del lado izquierdo:
  //   int a = 1 << 32; // UB si int tiene 32 bits de ancho
```

---

## Estructuras de Control

### if / else if / else

```c
  if (0) {
    printf("Nunca me ejecuto\n");
  } else if (0) {
    printf("Yo tampoco me ejecuto\n");
  } else {
    printf("Yo sí imprimo\n");
  }
```

### while

```c
  int ii = 0;
  while (ii < 10) { // CUALQUIER valor menor a diez es true.
    printf("%d, ", ii++); // ii++ incrementa ii DESPUÉS de usar su valor actual.
  } // => imprime "0, 1, 2, 3, 4, 5, 6, 7, 8, 9, "

  printf("\n");
```

### do-while

```c
  int kk = 0;
  do {
    printf("%d, ", kk);
  } while (++kk < 10); // ++kk incrementa kk ANTES de usar su valor.
  // => imprime "0, 1, 2, 3, 4, 5, 6, 7, 8, 9, "

  printf("\n");
```

### for

```c
  int jj;
  for (jj = 0; jj < 10; jj++) {
    printf("%d, ", jj);
  } // => imprime "0, 1, 2, 3, 4, 5, 6, 7, 8, 9, "

  printf("\n");

  // NOTAS: Los loops y funciones DEBEN tener un cuerpo. Si no necesitas cuerpo:
  int i;
  for (i = 0; i <= 5; i++) {
    ; // usa punto y coma como cuerpo (sentencia nula)
  }
  // O simplemente:
  for (i = 0; i <= 5; i++);
```

### switch

```c
  // ramificación con múltiples opciones: switch()
  switch (a) {
  case 0: // las etiquetas deben ser expresiones constantes integrales (como enums)
    printf("Hey, 'a' equals 0!\n");
    break; // si no pones break, el control cae hacia las etiquetas siguientes
  case 1:
    printf("Huh, 'a' equals 1!\n");
    break;
    // Cuidado — sin "break", la ejecución continúa hasta el próximo "break".
  case 3:
  case 4:
    printf("Look at that.. 'a' is either 3, or 4\n");
    break;
  default:
    // si la expresión no coincidió con ninguna etiqueta
    fputs("Error!\n", stderr);
    exit(-1);
    break;
  }
```

### goto

```c
  int i, j;
  for (i = 0; i < 100; ++i)
  for (j = 0; j < 100; ++j)
  {
    if ((i + j) >= 150) {
        goto error;  // salir de ambos for loops inmediatamente
    }
  }
  printf("No error found. Completed normally.\n");
  goto end;

  error: // esta es una etiqueta a la que puedes "saltar" con "goto error;"
  printf("Error occurred at i = %d & j = %d.\n", i, j);
  end:
    return 0;

  // https://ideone.com/z7nzKJ
  // imprimirá "Error occurred at i = 51 & j = 99."

  // Generalmente se considera mala práctica usar goto, excepto cuando
  // realmente sabes lo que estás haciendo. Ver:
  // https://en.wikipedia.org/wiki/Spaghetti_code#Meaning
```

---

## Typecasting

```c
  // Todo valor en C tiene un tipo, pero puedes castear un valor a otro tipo
  // si lo deseas (con algunas restricciones).

  int x_hex = 0x01; // puedes asignar variables con literales hexadecimales
                    // el binario no está en el estándar, pero algunos compiladores
                    // lo permiten (x_bin = 0b0010010110)

  // El cast entre tipos intentará preservar sus valores numéricos
  printf("%d\n", x_hex);         // => Imprime 1
  printf("%d\n", (short) x_hex); // => Imprime 1
  printf("%d\n", (char) x_hex);  // => Imprime 1

  // Si asignas un valor mayor al máximo del tipo, hará rollover sin advertencia.
  printf("%d\n", (unsigned char) 257); // => 1 (Máximo de char = 255 si char es de 8 bits)

  // Para determinar el valor máximo de `char`, `signed char` y `unsigned char`,
  // usa las macros CHAR_MAX, SCHAR_MAX y UCHAR_MAX de <limits.h>

  // Los tipos integrales pueden castearse a tipos de punto flotante y viceversa.
  printf("%f\n", (double) 100); // %f siempre formatea un double...
  printf("%f\n", (float)  100); // ...incluso con un float.
  printf("%d\n", (char)100.0);
```

---

## Punteros

```c
  // Un puntero es una variable declarada para almacenar una dirección de memoria.
  // Su declaración también indica el tipo de dato al que apunta.
  // Puedes obtener la dirección de memoria de tus variables y manipularlas.

  int x = 0;
  printf("%p\n", (void *)&x); // Usa & para obtener la dirección de una variable
  // (%p formatea un puntero a objeto de tipo void *)
  // => Imprime alguna dirección de memoria

  // Los punteros comienzan con * en su declaración
  int *px, not_a_pointer; // px es un puntero a int
  px = &x; // almacena la dirección de x en px
  printf("%p\n", (void *)px); // => Imprime alguna dirección de memoria
  printf("%zu, %zu\n", sizeof(px), sizeof(not_a_pointer));
  // => Imprime "8, 4" en un sistema típico de 64 bits

  // Para obtener el valor en la dirección a la que apunta un puntero,
  // pon * adelante para desreferenciarlo.
  // Nota: puede ser confuso que '*' se use tanto para declarar un puntero
  // como para desreferenciarlo.
  printf("%d\n", *px); // => Imprime 0, el valor de x

  // También puedes cambiar el valor al que apunta el puntero.
  // Debemos envolver la desreferencia en paréntesis porque
  // ++ tiene mayor precedencia que *.
  (*px)++; // Incrementa en 1 el valor al que px apunta
  printf("%d\n", *px); // => Imprime 1
  printf("%d\n", x);   // => Imprime 1
```

### Arrays y Punteros

```c
  // Los arrays son una buena forma de asignar un bloque contiguo de memoria
  int x_array[20]; // declara array de tamaño 20 (no se puede cambiar)
  int xx;
  for (xx = 0; xx < 20; xx++) {
    x_array[xx] = 20 - xx;
  } // Inicializa x_array como 20, 19, 18,... 2, 1

  // Declara un puntero de tipo int e inicialízalo apuntando a x_array
  int* x_ptr = x_array;
  // x_ptr ahora apunta al primer elemento del array (el entero 20).
  // Esto funciona porque los arrays suelen decaer a punteros a su primer elemento.
  // Por ejemplo, cuando un array se pasa a una función o se asigna a un puntero,
  // decae (se convierte implícitamente) en un puntero.
  // Excepciones: cuando el array es argumento del operador `&` (dirección de):
  int arr[10];
  int (*ptr_to_arr)[10] = &arr; // &arr NO es de tipo `int *`!
  // Es de tipo "puntero a array" (de diez `int`s).
  // O cuando el array es un literal de string usado para inicializar un array de char:
  char otherarr[] = "foobarbazquirk";
  // O cuando es argumento de los operadores `sizeof` o `alignof`:
  int arraythethird[10];
  int *ptr = arraythethird; // equivalente a int *ptr = &arr[0];
  printf("%zu, %zu\n", sizeof(arraythethird), sizeof(ptr));
  // probablemente imprime "40, 4" o "40, 8"

  // Los punteros se incrementan y decrementan según su tipo
  // (esto se llama aritmética de punteros)
  printf("%d\n", *(x_ptr + 1)); // => Imprime 19
  printf("%d\n", x_array[1]);   // => Imprime 19
```

### Memoria Dinámica

```c
  // También puedes asignar dinámicamente bloques contiguos de memoria con la
  // función estándar malloc, que toma un argumento de tipo size_t representando
  // el número de bytes a asignar (usualmente del heap, aunque esto puede no ser
  // verdad en sistemas embebidos — el estándar C no dice nada al respecto).
  int *my_ptr = malloc(sizeof(*my_ptr) * 20);
  for (xx = 0; xx < 20; xx++) {
    *(my_ptr + xx) = 20 - xx; // my_ptr[xx] = 20-xx
  } // Inicializa memoria como 20, 19, 18, 17... 2, 1 (como ints)

  // Ten cuidado al pasar valores proporcionados por el usuario a malloc.
  // Si quieres ser seguro, usa calloc (que, a diferencia de malloc, también
  // pone todo en cero).
  int* my_other_ptr = calloc(20, sizeof(int));

  // No existe una forma estándar de obtener la longitud de un array asignado
  // dinámicamente en C. Por eso, si tus arrays se van a pasar por el programa,
  // necesitas otra variable para rastrear el número de elementos (tamaño).
  size_t size = 10;
  int *my_arr = calloc(size, sizeof(int));
  // Agregar un elemento al array
  size++;
  my_arr = realloc(my_arr, sizeof(int) * size);
  if (my_arr == NULL) {
    // ¡Recuerda verificar fallos de realloc!
    return;
  }
  my_arr[10] = 5;

  // Desreferenciar memoria que no has asignado produce "resultados impredecibles"
  // — se dice que el programa invoca "comportamiento indefinido"
  printf("%d\n", *(my_ptr + 21)); // => Imprime quién sabe qué. Podría incluso crashear.

  // Cuando termines con un bloque de memoria asignado con malloc, debes liberarlo.
  // De lo contrario nadie más puede usarlo hasta que el programa termine
  // (esto se llama "memory leak"):
  free(my_ptr);
```

---

## Funciones

```c
// Sintaxis de declaración de función:
// <tipo de retorno> <nombre de función>(<args>)

int add_two_ints(int x1, int x2)
{
  return x1 + x2; // usa return para retornar un valor
}

// Las funciones son "call by value". Cuando se llama una función, los argumentos
// que se le pasan son COPIAS de los argumentos originales (excepto arrays).
// Lo que hagas con los argumentos dentro de la función NO cambia el valor del
// argumento original donde fue llamada la función.
//
// Usa punteros si necesitas editar los valores de los argumentos originales
// (los arrays siempre se pasan como punteros).
```

### Invertir string in-place

```c
// Una función void no retorna ningún valor
void str_reverse(char *str_in)
{
  char tmp;
  size_t ii = 0;
  size_t len = strlen(str_in); // `strlen()` es parte de la librería estándar de C
                               // NOTA: la longitud retornada por `strlen` NO incluye
                               //       el byte NULL terminador ('\0')
  // En C99 y versiones más nuevas, puedes declarar la variable de control del loop
  // directamente en los paréntesis: `for (size_t ii = 0; ...`
  for (ii = 0; ii < len / 2; ii++) {
    tmp = str_in[ii];
    str_in[ii] = str_in[len - ii - 1]; // carácter ii desde el final
    str_in[len - ii - 1] = tmp;
  }
}
// NOTA: se necesita incluir el header string.h para usar strlen()

/*
char c[] = "This is a test.";
str_reverse(c);
printf("%s\n", c); // => ".tset a si sihT"
*/
```

### Swap (paso por referencia)

```c
// Como solo podemos retornar una variable, para cambiar valores de más de
// una variable usamos call by reference (paso por referencia):
void swapTwoNumbers(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/*
int first = 10;
int second = 20;
printf("first: %d\nsecond: %d\n", first, second);
swapTwoNumbers(&first, &second);
printf("first: %d\nsecond: %d\n", first, second);
// los valores quedarán intercambiados
*/
```

### Retornar múltiples valores

```c
// C no permite retornar múltiples valores con return.
// Si quieres retornar múltiples valores, el llamador debe pasar las variables
// donde quiere que vayan los valores retornados, como punteros para que la
// función pueda modificarlos.
int return_multiple(int *array_of_3, int *ret1, int *ret2, int *ret3)
{
    if (array_of_3 == NULL)
        return 0; // retorna código de error (false)

    // desreferencia el puntero para modificar su valor
    *ret1 = array_of_3[0];
    *ret2 = array_of_3[1];
    *ret3 = array_of_3[2];

    return 1; // retorna código de éxito (true)
}
```

### Arrays como parámetros

```c
// Con respecto a los arrays, siempre se pasan a funciones como punteros.
// Incluso si asignas estáticamente un array como `arr[10]`, igual se pasa
// como puntero al primer elemento en cualquier llamado a función.
// No existe forma estándar de obtener el tamaño de un array asignado dinámicamente.

// ¡Se debe pasar el tamaño!
// De lo contrario, la función no tiene forma de saber qué tan grande es el array.
void printIntArray(int *arr, size_t size) {
    int i;
    for (i = 0; i < size; i++) {
        printf("arr[%d] is: %d\n", i, arr[i]);
    }
}

/*
int my_arr[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
int size = 10;
printIntArray(my_arr, size);
// imprimirá "arr[0] is: 1", etc
*/
```

### extern y static

```c
// Si refieres a variables externas fuera de la función, debes usar extern:
int i = 0;
void testFunc() {
  extern int i; // i aquí usa la variable externa i
}

// Haz privadas las variables externas al archivo fuente con static:
static int j = 0; // otros archivos que usen testFunc2() no pueden acceder a j
void testFunc2() {
  extern int j;
}

// La palabra clave static hace que una variable sea inaccesible para código fuera
// de la unidad de compilación. (En casi todos los sistemas, una "unidad de compilación"
// es un archivo .c). static aplica tanto a variables globales de la unidad de compilación,
// funciones, y variables locales a funciones. Al usar static con variables locales,
// la variable es efectivamente global y retiene su valor entre llamados a la función,
// pero solo es accesible dentro de la función donde fue declarada.
// Además, las variables static se inicializan en 0 si no se declaran con otro valor inicial.
// **También puedes declarar funciones como static para hacerlas privadas**
```

---

## Tipos Definidos por el Usuario y Structs

```c
// Los typedef pueden usarse para crear aliases de tipos
typedef int my_type;
my_type my_type_var = 0;

// Los structs son simplemente colecciones de datos, los miembros se asignan
// secuencialmente en el orden en que se escriben:
struct rectangle {
  int width;
  int height;
};

// No es generalmente verdad que
// sizeof(struct rectangle) == sizeof(int) + sizeof(int)
// debido al posible padding entre los miembros (por razones de alineación). [1]

void function_1()
{
  struct rectangle my_rec = { 1, 2 }; // Los campos pueden inicializarse inmediatamente

  // Acceder a miembros del struct con .
  my_rec.width = 10;
  my_rec.height = 20;

  // Puedes declarar punteros a structs
  struct rectangle *my_rec_ptr = &my_rec;

  // Usa desreferencia para establecer miembros del struct por puntero...
  (*my_rec_ptr).width = 30;

  // ...o mejor aún: prefiere el shorthand -> por legibilidad
  my_rec_ptr->height = 10; // Igual que (*my_rec_ptr).height = 10;
}

// Puedes aplicar un typedef a un struct por conveniencia
typedef struct rectangle rect;

int area(rect r)
{
  return r.width * r.height;
}

// Los typedef también pueden definirse justo durante la definición del struct:
typedef struct {
  int width;
  int height;
} rect;
// Como antes, esto permite escribir simplemente:
rect r;
// en lugar de tener que escribir:
struct rectangle r;

// Si tienes structs grandes, puedes pasarlos "por puntero" para evitar copiar
// todo el struct:
int areaptr(const rect *r)
{
  return r->width * r->height;
}
```

---

## Punteros a Funciones

```c
// En runtime, las funciones se ubican en direcciones de memoria conocidas.
// Los punteros a funciones son como cualquier otro puntero (almacenan una
// dirección de memoria), pero pueden usarse para invocar funciones directamente
// y para pasar handlers (o funciones callback). Sin embargo, la sintaxis de
// definición puede ser confusa al principio.

// Ejemplo: usar str_reverse desde un puntero
void str_reverse_through_pointer(char *str_in) {
  // Define una variable puntero a función, llamada f.
  void (*f)(char *); // La firma debe coincidir exactamente con la función objetivo.
  f = &str_reverse;  // Asigna la dirección de la función real (determinada en runtime)
  // f = str_reverse; también funciona — las funciones decaen a punteros, como los arrays
  (*f)(str_in); // Llamar a la función a través del puntero
  // f(str_in); // Esta es una sintaxis alternativa igualmente válida para llamarla.
}

// Mientras las firmas de función coincidan, puedes asignar cualquier función
// al mismo puntero.
// Los punteros a funciones suelen hacerse typedef por simplicidad y legibilidad:

typedef void (*my_fnp_type)(char *);

// Luego se usa al declarar la variable puntero real:
// ...
// my_fnp_type f;
```

---

## Imprimiendo con printf()

### Caracteres especiales

```c
'\a'; // carácter de alerta (campana)
'\n'; // carácter de nueva línea
'\t'; // carácter de tabulación (justifica texto a la izquierda)
'\v'; // tabulación vertical
'\f'; // nueva página (form feed)
'\r'; // retorno de carro
'\b'; // carácter de retroceso
'\0'; // carácter NULL. Usualmente al final de strings en C.
      //   hello\n\0. \0 se usa por convención para marcar el fin de string.
'\\'; // barra invertida
'\?'; // signo de pregunta
'\''; // comilla simple
'\"'; // comilla doble
'\xhh'; // número hexadecimal. Ejemplo: '\xb' = carácter de tabulación vertical
'\0oo'; // número octal. Ejemplo: '\013' = carácter de tabulación vertical
```

### Especificadores de formato

```c
"%d";    // entero
"%3d";   // entero con mínimo 3 dígitos de longitud (justifica texto a la derecha)
"%s";    // string
"%f";    // float
"%ld";   // long
"%3.2f"; // float con mínimo 3 dígitos a la izquierda y 2 a la derecha del decimal
"%7.4s"; // (también funciona con strings)
"%c";    // char
"%p";    // puntero. NOTA: hay que castear el puntero a (void *) antes de pasarlo a printf.
"%x";    // hexadecimal
"%o";    // octal
"%%";    // imprime %
```

---

## Orden de Evaluación

_(De mayor a menor precedencia)_

| Operadores                           | Asociatividad       |
| ------------------------------------ | ------------------- |
| `() [] -> .`                         | izquierda → derecha |
| `! ~ ++ -- + - *(type) sizeof`       | derecha → izquierda |
| `* / %`                              | izquierda → derecha |
| `+ -`                                | izquierda → derecha |
| `<< >>`                              | izquierda → derecha |
| `< <= > >=`                          | izquierda → derecha |
| `== !=`                              | izquierda → derecha |
| `&`                                  | izquierda → derecha |
| `^`                                  | izquierda → derecha |
| `\|`                                 | izquierda → derecha |
| `&&`                                 | izquierda → derecha |
| `\|`                                 | izquierda → derecha |
| `?:`                                 | derecha → izquierda |
| `= += -= *= /= %= &= ^= \|= <<= >>=` | derecha → izquierda |
| `,`                                  | izquierda → derecha |

---

## Header Files

Los headers son una parte importante de C ya que permiten conectar archivos fuente y simplificar el código separando definiciones en archivos distintos.

Son sintácticamente similares a los archivos `.c` pero residen en archivos `.h`. Se incluyen con la directiva preprocesador `#include "example.h"`, dado que `example.h` existe en el mismo directorio.

```c
// Salvaguarda para prevenir que el header se defina múltiples veces.
// Esto ocurre en el caso de dependencia circular — el contenido del header
// ya estaría definido.

// Se usa estos headers para asegurar de que no se entre en esta importacion
// mas de una vez pero tambien se puede usar: #pragma once

#ifndef EXAMPLE_H /* si EXAMPLE_H aún no está definido */
#define EXAMPLE_H /* Define el macro EXAMPLE_H */

// Otros headers pueden incluirse en headers y por lo tanto incluirse
// transitivamente en archivos que incluyan este header.
#include <string.h>

// Al igual que en archivos .c, los macros pueden definirse en headers
// y usarse en archivos que incluyan este header.
#define EXAMPLE_NAME "Dennis Ritchie"

// Los macros de función también pueden definirse.
#define ADD(a, b) ((a) + (b))

// Nota los paréntesis que rodean los argumentos — esto es importante para
// garantizar que a y b no se expandan de forma inesperada (ej: considera
// MUL(x, y) (x * y); MUL(1 + 2, 3) se expandiría como (1 + 2 * 3), dando
// un resultado incorrecto).

// Los structs y typedef pueden usarse para consistencia entre archivos.
typedef struct Node
{
    int val;
    struct Node *next;
} Node;

// Lo mismo con enumeraciones.
enum traffic_light_state {GREEN, YELLOW, RED};

// Los prototipos de función también pueden definirse aquí para uso en múltiples
// archivos, pero es mala práctica definir la función en el header.
// Las definiciones deben ir en un archivo .c.
Node createLinkedList(int *vals, int len);

// Más allá de los elementos anteriores, otras definiciones deben dejarse en un archivo .c. Los includes o definiciones excesivas tampoco deben estar en un header, sino en headers separados o en un archivo .c.

#endif /* Fin de la directiva preprocesador if */
```
