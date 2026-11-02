#ifndef SYNC_CAR_H
#define SYNC_CAR_H

#include <pthread.h>

struct station {
	pthread_mutex_t lock;             /* unico lock da estacao */
	pthread_cond_t car_arrived;       /* passageiros esperam pelo vagao */
	pthread_cond_t passenger_boarded; /* vagao espera pelos passageiros */
	int waiting_passengers;           /* passageiros ainda sem assento */
	int free_seats;                   /* assentos livres do vagao atual */
	int boarding_passengers;          /* embarcaram e ainda nao avisaram */
};

void station_init(struct station *station);
void station_load_car(struct station *station, int count);
void station_wait_for_car(struct station *station);
void station_on_board(struct station *station);

#endif
