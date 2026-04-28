#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Variável compartilhada que será lida e escrita
int dado_compartilhado = 0;

// Contador de leitores ativos
int contador_leitores = 0;

// Mutexes para sincronização
pthread_mutex_t mutex_contador; // Protege o contador_leitores
pthread_mutex_t mutex_recurso;  // Protege a região de escrita/leitura contra concorrência leitor-escritor e escritor-escritor

// Função que simula o comportamento dos leitores
void* leitor(void* arg) {
    int id = *((int*)arg);
    
    // Executa a leitura duas vezes para que o programa termine sozinho
    for (int i = 0; i < 2; i++) {
        // --- PROTEÇÃO DO CONTADOR ---
        pthread_mutex_lock(&mutex_contador);
        contador_leitores++;
        
        // Se for o primeiro leitor, bloqueia o escritor
        if (contador_leitores == 1) {
            printf("[Leitor %d] Primeiro leitor ativo. Bloqueando o escritor.\n", id);
            pthread_mutex_lock(&mutex_recurso);
        }
        
        printf("[Leitor %d] Entrou na região de leitura. Leitores ativos: %d\n", id, contador_leitores);
        pthread_mutex_unlock(&mutex_contador);

        // --- REGIÃO CRÍTICA DE LEITURA ---
        // Vários leitores podem estar aqui ao mesmo tempo
        printf("[Leitor %d] Lendo o dado compartilhado: %d\n", id, dado_compartilhado);
        sleep(1); // Simula o tempo de leitura

        // --- ATUALIZAÇÃO DO CONTADOR E LIBERAÇÃO ---
        pthread_mutex_lock(&mutex_contador);
        contador_leitores--;
        
        // Se for o último leitor, libera a região para o escritor
        if (contador_leitores == 0) {
            printf("[Leitor %d] Último leitor saindo. Liberando o escritor.\n", id);
            pthread_mutex_unlock(&mutex_recurso);
        }
        pthread_mutex_unlock(&mutex_contador);

        sleep(1); // Dá um tempo antes da próxima tentativa
    }
    return NULL;
}

// Função que simula o comportamento do escritor
void* escritor(void* arg) {
    int id = *((int*)arg);
    
    for (int i = 0; i < 2; i++) {
        // O escritor tenta acessar o recurso. Ficará bloqueado se houver leitores.
        pthread_mutex_lock(&mutex_recurso);

        // --- REGIÃO CRÍTICA DE ESCRITA ---
        printf("[Escritor %d] Entrou na região crítica.\n", id);
        dado_compartilhado += 10;
        printf("[Escritor %d] Modificou o dado para: %d\n", id, dado_compartilhado);
        sleep(2); // Simula o tempo de escrita
        
        printf("[Escritor %d] Saiu da região crítica.\n", id);
        
        // Libera o recurso para leitores ou outros escritores
        pthread_mutex_unlock(&mutex_recurso);

        sleep(2); // Dá um tempo antes da próxima tentativa
    }
    return NULL;
}

int main() {
    // Inicialização dos mutexes
    pthread_mutex_init(&mutex_contador, NULL);
    pthread_mutex_init(&mutex_recurso, NULL);

    // Identificadores de thread
    pthread_t th_escritor;
    pthread_t th_leitores[3];
    int id_escritor = 1;
    int id_leitores[3] = {1, 2, 3};

    printf("--- INICIANDO SIMULAÇÃO ---\n");

    // Criação de pelo menos uma thread escritora e três threads leitoras
    pthread_create(&th_escritor, NULL, escritor, &id_escritor);
    for (int i = 0; i < 3; i++) {
        pthread_create(&th_leitores[i], NULL, leitor, &id_leitores[i]);
    }

    // Aguarda o término de todas as threads (o programa termina corretamente)
    pthread_join(th_escritor, NULL);
    for (int i = 0; i < 3; i++) {
        pthread_join(th_leitores[i], NULL);
    }

    // Destruição dos mutexes
    pthread_mutex_destroy(&mutex_contador);
    pthread_mutex_destroy(&mutex_recurso);

    printf("--- SIMULAÇÃO ENCERRADA ---\n");
    return 0;
}