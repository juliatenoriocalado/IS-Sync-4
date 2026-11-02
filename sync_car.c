/*
 * sync_car.c - sincronizacao entre o vagao e os passageiros da estacao.
 *
 * O estado da estacao fica em tres contadores protegidos pelo mesmo mutex:
 * quantos passageiros estao esperando, quantos assentos livres o vagao tem
 * e quantos passageiros ja foram liberados mas ainda nao chamaram
 * station_on_board. Com esses contadores as duas threads conseguem decidir
 * sozinhas quando devem dormir e quando devem acordar, sem busy-wait.
 */

#include <pthread.h>

#include "sync_car.h"

void station_init(struct station *station)
{
	station->waiting_passengers = 0;
	station->free_seats = 0;
	station->boarding_passengers = 0;

	pthread_mutex_init(&station->lock, NULL);
	pthread_cond_init(&station->car_arrived, NULL);
	pthread_cond_init(&station->passenger_boarded, NULL);
}

/*
 * Thread do vagao. count e o numero de assentos livres.
 *
 * Se nao ha ninguem esperando, o vagao nao tem o que fazer e volta na hora.
 * Caso contrario acorda todos os passageiros e espera ate que o vagao
 * esteja cheio (free_seats == 0) ou ate que nao sobre nenhum passageiro
 * esperando, e ainda ate que todos os que embarcaram tenham chamado
 * station_on_board.
 */
void station_load_car(struct station *station, int count)
{
	pthread_mutex_lock(&station->lock);

	station->free_seats = count;

	if (count > 0 && station->waiting_passengers > 0) {
		pthread_cond_broadcast(&station->car_arrived);

		while ((station->free_seats > 0 && station->waiting_passengers > 0) ||
		       station->boarding_passengers > 0)
			pthread_cond_wait(&station->passenger_boarded, &station->lock);
	}

	station->free_seats = 0;

	pthread_mutex_unlock(&station->lock);
}

/*
 * Thread do passageiro. Entra na contagem de espera e dorme enquanto nao
 * houver assento livre. Ao acordar, reserva um assento e passa a contar
 * como passageiro embarcado.
 */
void station_wait_for_car(struct station *station)
{
	pthread_mutex_lock(&station->lock);

	station->waiting_passengers++;

	while (station->free_seats == 0)
		pthread_cond_wait(&station->car_arrived, &station->lock);

	station->free_seats--;
	station->waiting_passengers--;
	station->boarding_passengers++;

	pthread_mutex_unlock(&station->lock);
}

/*
 * Avisa o vagao que o passageiro ja esta sentado. Quando o ultimo
 * passageiro avisa, o vagao pode finalmente partir.
 */
void station_on_board(struct station *station)
{
	pthread_mutex_lock(&station->lock);

	station->boarding_passengers--;

	if (station->boarding_passengers == 0)
		pthread_cond_signal(&station->passenger_boarded);

	pthread_mutex_unlock(&station->lock);
}
