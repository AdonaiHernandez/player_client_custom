#include <stdio.h>
#include <stdlib.h>
#include <libplayerc/playerc.h>

int main(int argc, char *argv[])
{
    // Inicializar cliente y dispositivos
    playerc_client_t *client;
    playerc_position2d_t *position2d;

    // Crear cliente
    client = playerc_client_create(NULL, "localhost", 6665); // Cambia localhost y puerto si es necesario
    if (playerc_client_connect(client) != 0)
    {
        fprintf(stderr, "Error al conectar con el servidor Player.\n");
        return -1;
    }

    // Acceder a la interfaz position2d (necesaria para moverse)
    position2d = playerc_position2d_create(client, 0);
    if (playerc_position2d_subscribe(position2d, PLAYER_OPEN_MODE) != 0)
    {
        fprintf(stderr, "Error al suscribir position2d.\n");
        playerc_client_disconnect(client);
        playerc_client_destroy(client);
        return -1;
    }

    // Activar el control de posición
    playerc_position2d_enable(position2d, 1);

    // Configurar coordenadas globales de destino
    double x_dest = 5.0; // Coordenada X global
    double y_dest = 5.0; // Coordenada Y global
    double yaw_dest = 0.0; // Orientación deseada en radianes

    // Enviar el objetivo
    if (playerc_position2d_set_cmd_pose(position2d, x_dest, y_dest, yaw_dest, 1) != 0)
    {
        fprintf(stderr, "Error al enviar el objetivo.\n");
    }
    else
    {
        printf("Objetivo enviado: X = %.2f, Y = %.2f, Yaw = %.2f\n", x_dest, y_dest, yaw_dest);
    }

    // Esperar un poco antes de cerrar
    sleep(5);

    // Desconectar y limpiar
    playerc_position2d_unsubscribe(position2d);
    playerc_position2d_destroy(position2d);
    playerc_client_disconnect(client);
    playerc_client_destroy(client);

    return 0;
}
