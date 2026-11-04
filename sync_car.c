#include <pthread.h> 
#include "sync_car.h"

void station_init(struct station *station){
    station->passageiros_esperando = 0;
    station->quantidade_de_assentos_livres = 0;
    station->passageiros_a_bordo = 0;

    pthread_mutex_init(&station->lock, NULL);
    pthread_cond_init(&station->carro_chegou, NULL);
    pthread_cond_init(&station->passageiro_embarcou, NULL);
}

void station_load_car(struct station *station, int count)
{
	pthread_mutex_lock(&station->lock);
	station->quantidade_de_assentos_livres = count;
	if (count > 0 && station->passageiros_esperando > 0) {
		pthread_cond_broadcast(&station->carro_chegou);
		while ((station->quantidade_de_assentos_livres > 0 &&
		        station->passageiros_esperando > 0) ||
		       station->passageiros_a_bordo > 0) {
			pthread_cond_wait(&station->passageiro_embarcou, &station->lock);
		}
	}
	station->quantidade_de_assentos_livres = 0;
	pthread_mutex_unlock(&station->lock);
}

void station_wait_for_car(struct station *station){
    pthread_mutex_lock(&station->lock);
    station->passageiros_esperando++;

    while(station->quantidade_de_assentos_livres == 0){
        pthread_cond_wait(&station->carro_chegou, &station->lock);
    }

    station->quantidade_de_assentos_livres--;
    station->passageiros_esperando--;
    station->passageiros_a_bordo++;

    pthread_mutex_unlock(&station->lock);
}

void station_on_board(struct station *station){
    pthread_mutex_lock(&station->lock);
    station->passageiros_a_bordo--;

    if(station->passageiros_a_bordo == 0){
        pthread_cond_signal(&station->passageiro_embarcou);
    }

    pthread_mutex_unlock(&station->lock);

}
