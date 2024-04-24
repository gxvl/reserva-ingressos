#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_INGRESSOS 10
#define NUM_PESSOAS 15
#define MAX_INGRESSOS_POR_TRANSACAO 4
#define MAX_TENTATIVAS_POR_PESSOA 3

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_compra = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_gera = PTHREAD_COND_INITIALIZER;
int ingressos_disponiveis = NUM_INGRESSOS;
int ingressos_vendidos = 0;
int geracao_iniciada = 0;

void *compra_ingresso(void *thread_id) {
  int tid = *((int *)thread_id);
  int num_ingressos_reservados =
      rand() % MAX_INGRESSOS_POR_TRANSACAO +
      1; // Número aleatório de ingressos para esta transação
  int tentativas = 0;

  while (1) {
    pthread_mutex_lock(&mutex);

    if (ingressos_disponiveis >= num_ingressos_reservados) {
      printf("Pessoa %d reservou %d ingressos.\n", tid,
             num_ingressos_reservados);
      ingressos_disponiveis -= num_ingressos_reservados;
      ingressos_vendidos += num_ingressos_reservados;

      if (!geracao_iniciada && ingressos_vendidos >= NUM_INGRESSOS) {
        geracao_iniciada = 1;
        printf("Ingressos iniciais esgotados. Iniciando geração de ingressos "
               "adicionais.\n");
        pthread_cond_signal(&cond_gera); // Sinaliza que a geração de ingressos
                                         // adicionais pode começar
      }

      pthread_mutex_unlock(&mutex);
      pthread_exit(NULL);
    } else {
      printf("Pessoa %d: Desculpe, não há ingressos suficientes. Tentando "
             "novamente...\n",
             tid);
      tentativas++;
      pthread_cond_wait(
          &cond_compra,
          &mutex); // Espera por novos ingressos disponíveis para compra
    }

    pthread_mutex_unlock(&mutex);
  }
}

void *gera_ingressos(void *args) {
  pthread_mutex_lock(&mutex);
  while (!geracao_iniciada) {
    pthread_cond_wait(
        &cond_gera,
        &mutex); // Espera até que os ingressos iniciais sejam esgotados
  }
  pthread_mutex_unlock(&mutex);

  printf("Iniciando geração de ingressos adicionais...\n");

  while (1) {
    pthread_mutex_lock(&mutex);
    if (ingressos_disponiveis < NUM_INGRESSOS) {
      // Simulando a geração de ingressos adicionais
      usleep(500000); // Espera 500ms para gerar ingressos
      int num_ingressos_gerados = rand() % 3 + 1; // Gera de 1 a 3 ingressos
      ingressos_disponiveis += num_ingressos_gerados;
      printf("%d ingressos adicionais gerados. Total: %d\n",
             num_ingressos_gerados, ingressos_disponiveis);
      pthread_cond_broadcast(
          &cond_compra); // Notifica todas as threads de compra sobre a
                         // disponibilidade de novos ingressos
    } else {
      pthread_mutex_unlock(&mutex);
      break;
    }
    pthread_mutex_unlock(&mutex);
  }

  pthread_exit(NULL);
}

int main() {
  srand(time(NULL));

  pthread_t threads_gera_ingressos, threads_compra_ingresso[NUM_PESSOAS];
  int thread_ids[NUM_PESSOAS];

  pthread_create(&threads_gera_ingressos, NULL, gera_ingressos,
                 NULL); // Thread para gerar ingressos

  for (int i = 0; i < NUM_PESSOAS; i++) {
    thread_ids[i] = i;
    pthread_create(&threads_compra_ingresso[i], NULL, compra_ingresso,
                   (void *)&thread_ids[i]);
  }

  for (int i = 0; i < NUM_PESSOAS; i++) {
    pthread_join(threads_compra_ingresso[i], NULL);
  }

  pthread_join(threads_gera_ingressos,
               NULL); // Espera a thread de geração de ingressos terminar

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond_compra);
  pthread_cond_destroy(&cond_gera);

  return 0;
}
