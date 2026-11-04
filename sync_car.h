#ifndef SYNC_CAR_H
#define SYNC_CAR_H

#include <pthread.h>

struct station { 
	pthread_mutex_t lock;
	pthread_cond_t carro_chegou;
	pthread_cond_t passageiro_embarcou;
	int passageiros_esperando;
	int quantidade_de_assentos_livres;
	int passageiros_a_bordo;
};

void station_init(struct station *station); //inicialização do objeto
void station_load_car(struct station *station, int count); //quando um carro chega à estação e tem as portas aberta
void station_wait_for_car(struct station *station); //quando um passageiro chega à estação
void station_on_board(struct station *station); //passageiro embarcou

#endif