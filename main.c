/*
Tarea 4
Alejandra Nissan
A01024682
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

typedef struct{
    pid_t pidHijo;
    int promedio;
}proceso_hijo;

void imprimirHistograma(proceso_hijo*, int, int);

int main(int argc, char * const * argv){
    int nValue, c;
    int esEntero= 1;
    

    opterr = 0;

    while((c=getopt(argc, argv, "n:h")) != -1){
        switch(c){
            case 'n':
                nValue = atoi(optarg);
                //Revisando que el valor sea un número entero
                if(nValue<=0){
                    printf("Se requiere un numero y que sea mayor a 0\n");
                    exit(0);
                }
                break;
            case 'h':
                printf("Debes ingresar la cantidad de hijos que quieres crear en el siguiente formato ./a.out -n <numero>\n");
                return 1;
            case '?':
            if (optopt == 'n')
                fprintf (stderr, "Opción -%c requiere un argumento.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Opción desconocida '-%c'.\n", optopt);
            else
                fprintf (stderr,
                         "Opción desconocida '\\x%x'.\n",
                         optopt);
            return 1;
        default:
            abort ();                
        }
    }

    for (int index = optind; index < argc; index++){
        printf ("El argumento no es una opción válida %s\n", argv[index]);
        return 1;
    }

    //Aqui se va a almacenar la información de los procesos hijos
    proceso_hijo * procesos;
    procesos = (proceso_hijo *)malloc(sizeof(proceso_hijo) * nValue);

    proceso_hijo *aux = procesos;
    proceso_hijo *final= procesos + nValue;

    pid_t pid;
    int promedio, veces=0, max=0;

    while (aux<final) {
        aux->pidHijo = fork();
        
        if (aux->pidHijo == -1)
        {
            printf("Error al crear el proceso hijo \n");
            printf("Se crearon %d procesos hijos\n", veces);
            free(procesos);
            break;
        }
        else if (aux->pidHijo == 0)
        {
            // Estamos en el hijo
            promedio = (getpid()+getppid())/2;
            printf("Soy el proceso hijo con PID: %d con padre %d y mi promedio es %d\n", getpid(), getppid(), promedio);
            free(procesos);
            exit(promedio); //No se evnian correctamente los valors por este método
        }
        else {
            // Estamos en el padre
                if (waitpid(aux->pidHijo, &promedio, 0) != -1)
                {
                    if (WIFEXITED(promedio)) {
                        aux->promedio = WEXITSTATUS(promedio);
                        if(aux->promedio>max){
                            max = aux->promedio;
                        }
                    }
                }
        }
        aux++;
        veces++;
    }    

    imprimirHistograma(procesos, veces, max);

    free(procesos);

}

void imprimirHistograma(proceso_hijo * procesos, int veces, int max){
    int asteriscos=0;

    printf("Pid hijo  Promedio  Histograma\n");
    for(int i=0; i<veces; i++){
        asteriscos = (((procesos+i)->promedio)*10)/max;
        printf("%d       %d     ",(procesos+i)->pidHijo, (procesos+i)->promedio);
        for(int j=0; j<asteriscos; j++){
            printf("*");
        }
        printf("\n");
    }

}