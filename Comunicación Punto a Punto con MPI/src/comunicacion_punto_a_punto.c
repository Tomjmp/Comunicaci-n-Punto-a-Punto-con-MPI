#include <stdio.h>   /* Biblioteca estandar de entrada/salida (printf) */
#include <mpi.h>     /* Biblioteca MPI: define funciones, tipos y constantes MPI */

int main(int argc, char *argv[])
{
    /* --- Variables generales --- */
    int rank;        /* Identificador unico de este proceso dentro del comunicador */
    int size;        /* Numero total de procesos lanzados por mpirun */

    /* --- Variables de comunicacion --- */
    int dato;        /* Valor entero que el proceso 0 enviara al proceso 1 */
    int origen;      /* Almacena el rank del proceso emisor (leido desde status) */
    int tag_recibido;/* Almacena el tag del mensaje (leido desde status) */
    MPI_Status status; /* Estructura que guarda metadatos del mensaje recibido:
                          - status.MPI_SOURCE : rank del proceso que envio el mensaje
                          - status.MPI_TAG    : tag asociado al mensaje
                          - status.MPI_ERROR  : codigo de error de la operacion */

    /* -----------------------------------------------------------------------
     * MPI_Init(&argc, &argv)
     * Inicializa el entorno de ejecucion MPI. Debe ser la primera llamada MPI.
     *   argc    : puntero al numero de argumentos de la linea de comandos
     *   argv    : puntero al vector de argumentos de la linea de comandos
     * MPI puede modificar argc/argv para eliminar sus propios argumentos internos.
     * ----------------------------------------------------------------------- */
    MPI_Init(&argc, &argv);

    /* -----------------------------------------------------------------------
     * MPI_Comm_rank(communicator, &rank)
     * Obtiene el identificador (rank) de este proceso dentro del comunicador.
     *   MPI_COMM_WORLD : comunicador global que incluye TODOS los procesos
     *   &rank          : puntero donde se escribe el rank de este proceso (0..size-1)
     * ----------------------------------------------------------------------- */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* -----------------------------------------------------------------------
     * MPI_Comm_size(communicator, &size)
     * Obtiene el numero total de procesos en el comunicador indicado.
     *   MPI_COMM_WORLD : comunicador global
     *   &size          : puntero donde se escribe el total de procesos lanzados
     * ----------------------------------------------------------------------- */
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* Validacion: el programa necesita exactamente 2 procesos para funcionar */
    if (size < 2) {
        if (rank == 0) {
            /* Solo el proceso 0 imprime el error para evitar mensajes duplicados */
            printf("ERROR: Este programa requiere al menos 2 procesos.\n");
            printf("       Ejecute con: mpirun -np 2 ./comunicacion\n");
        }
        /* Finalizar MPI correctamente antes de salir, aunque haya error */
        MPI_Finalize();
        return 1; /* Codigo de retorno != 0 indica error al sistema operativo */
    }

    /* =========================================================
     * PROCESO EMISOR: rank 0
     * ========================================================= */
    if (rank == 0) {

        dato = 100; /* Valor entero que se enviara al proceso 1 */

        printf("[Proceso %d] Enviando el valor %d al proceso 1...\n", rank, dato);

        /* -------------------------------------------------------------------
         * MPI_Send(buf, count, datatype, dest, tag, comm)
         * Envia un mensaje de forma bloqueante: no retorna hasta que el buffer
         * puede reutilizarse con seguridad (el mensaje fue enviado o copiado).
         *
         *   &dato          : direccion del buffer que contiene los datos a enviar
         *   1              : numero de elementos a enviar
         *   MPI_INT        : tipo de dato MPI equivalente a 'int' en C
         *   1              : rank del proceso DESTINO (proceso 1)
         *   0              : tag del mensaje (etiqueta para identificar el mensaje;
         *                    el receptor debe usar el mismo tag para recibirlo)
         *   MPI_COMM_WORLD : comunicador dentro del cual se realiza el envio
         * ------------------------------------------------------------------- */
        MPI_Send(&dato, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        printf("[Proceso %d] Mensaje enviado correctamente.\n", rank);
    }

    /* =========================================================
     * PROCESO RECEPTOR: rank 1
     * ========================================================= */
    else if (rank == 1) {

        /* -------------------------------------------------------------------
         * MPI_Recv(buf, count, datatype, source, tag, comm, &status)
         * Recibe un mensaje de forma bloqueante: no retorna hasta que el mensaje
         * ha llegado completamente y los datos estan disponibles en el buffer.
         *
         *   &dato          : direccion del buffer donde se almacenaran los datos
         *   1              : numero maximo de elementos a recibir
         *   MPI_INT        : tipo de dato esperado (debe coincidir con el emisor)
         *   0              : rank del proceso FUENTE del que se espera el mensaje
         *                    (MPI_ANY_SOURCE para aceptar de cualquier proceso)
         *   0              : tag esperado (debe coincidir con el tag del emisor;
         *                    MPI_ANY_TAG para aceptar cualquier tag)
         *   MPI_COMM_WORLD : comunicador dentro del cual se realiza la recepcion
         *   &status        : estructura donde MPI escribe informacion del mensaje
         *                    recibido (emisor real, tag real, posibles errores)
         * ------------------------------------------------------------------- */
        MPI_Recv(&dato, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        /* Extraer metadatos del mensaje desde la estructura status */
        origen       = status.MPI_SOURCE; 
        tag_recibido = status.MPI_TAG;    je  */

        printf("[Proceso %d] Mensaje recibido!\n", rank);
        printf("[Proceso %d] Valor recibido  : %d\n", rank, dato);
        printf("[Proceso %d] Proceso emisor  : %d\n", rank, origen);
        printf("[Proceso %d] Tag del mensaje : %d\n", rank, tag_recibido);
    }

    MPI_Finalize();

    return 0; /* El programa termino sin errores */
}
