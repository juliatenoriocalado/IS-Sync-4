# IS-Sync-4

Atividade 4 de Infraestrutura de Software: sincronização entre os vagões e os
passageiros de uma estação de metrô usando threads POSIX e variáveis de
condição.

## Arquivos

- `sync_car.h`: declaração da `struct station` e os protótipos das quatro
  funções usadas pelo programa de teste.
- `sync_car.c`: implementação de `station_init`, `station_load_car`,
  `station_wait_for_car` e `station_on_board`.
- `sync_car-runner.c`: programa de teste fornecido pelo professor. Ele cria
  100 threads de passageiros e vários vagões com números aleatórios de
  assentos, verificando se o vagão espera o tempo correto e se todos os
  passageiros embarcam.
- `Makefile`: compila os dois arquivos `.c` e gera o executável `sync_car`.
  Também tem os alvos `test` e `clean`.
- `relatorio.pdf`: relatório da atividade.

## Como compilar

É necessário ter o `gcc` e o `make` instalados. Na raiz do projeto, execute:

```
make
```

O comando gera o executável `sync_car`.

## Como executar

```
./sync_car
```

O programa imprime a sequência de vagões que chegam à estação e termina com a
mensagem `Looks good!` quando todos os 100 passageiros embarcam. Também é
possível usar `make test`, que compila (se necessário) e executa o teste.

## Como limpar

```
make clean
```

Remove o executável e os arquivos objeto.

## Sistema operacional

O código foi implementado e testado em Ubuntu 24.04 LTS dentro do WSL2, com
`gcc` 13.3.0 e GNU Make 4.3. Por usar apenas pthreads, também compila em
Linux, Unix e macOS com um compilador C que suporte POSIX threads.

## Observações sobre a implementação

- A `struct station` tem um único `pthread_mutex_t`.
- Não são usados semáforos nem outras primitivas de sincronização.
- Não há espera ocupada: todas as esperas são feitas com
  `pthread_cond_wait`.
- O vagão acorda todos os passageiros com `pthread_cond_broadcast`, então
  vários passageiros podem sair de `station_wait_for_car` antes de qualquer
  um chamar `station_on_board`.
