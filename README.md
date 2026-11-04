# Sincronização do Metrô (sync_car)

Implementação em C das funções de sincronização de embarque de passageiros
em um vagão de metrô, usando threads POSIX, um único mutex e variáveis de condição.

## Arquivos

- `sync_car.h`: declara `struct station` e as funções públicas.
- `sync_car.c`: implementa `station_init`, `station_load_car`,
  `station_wait_for_car` e `station_on_board`.
- `sync_car-runner.c`: programa de teste fornecido pelo professor (não alterado).
- `Makefile`: compila e limpa o projeto.
- `evidencias.log`: registro das sessões de teste (gravado com `script -a`).

## Sistema e ferramentas

Desenvolvido e testado em WSL (Ubuntu 24.04) no Windows, com
gcc 13.3.0 e GNU Make 4.3.

## Como compilar

```bash
make clean
make
```

Gera o executável `sync_car`. O aviso `unused parameter 'foo'` vem do
`sync_car-runner.c` do professor, não do `sync_car.c`.

## Como executar e testar

```bash
./sync_car
```

Imprime os carros entrando e saindo da estação e termina com `Looks good!`
se tudo passou (código de saída 0). Para repetir o teste 100 vezes:

```bash
ok=0; fail=0; for i in $(seq 1 100); do if timeout 20 ./sync_car > /dev/null 2>&1; then ok=$((ok+1)); else fail=$((fail+1)); fi; done; echo "sucessos=$ok falhas=$fail"
```

Limpar os arquivos compilados:

```bash
make clean
```