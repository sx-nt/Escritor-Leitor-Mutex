# ISADORA E SANT




### 1. Instruções simples de compilação e execução

Para compilar e executar o código no terminal Linux (ou ambiente com suporte POSIX/GCC), certifique-se de estar no mesmo diretório do arquivo fonte e utilize os seguintes comandos:

**Compilação:**
`gcc leitores_escritores.c -o programa -pthread`

**Execução:**
`./programa`

---

### 2. Breve descrição da opção escolhida e dos mecanismos de sincronização

**Opção Escolhida:** Opção 3 - Escritor e múltiplos leitores. 
Esta implementação resolve o problema clássico de concorrência permitindo que vários leitores acessem simultaneamente uma região compartilhada, enquanto garante que a thread escritora tenha acesso estritamente exclusivo[cite: 1].

**Mecanismos de Sincronização Utilizados:**
Para evitar espera ocupada e proteger corretamente as regiões críticas[cite: 1], a solução utiliza a biblioteca `pthread` com a seguinte lógica de sincronização:

*   **Contador de leitores (`contador_leitores`):** Variável que rastreia exatamente quantos leitores estão acessando a área de leitura no momento[cite: 1].
*   **Mutex do Contador (`mutex_contador`):** Um mutex dedicado exclusivamente para proteger a variável `contador_leitores`[cite: 1]. Ele impede que duas threads leitoras tentem alterar o contador no mesmo instante, o que causaria uma condição de corrida.
*   **Mutex do Recurso (`mutex_recurso`):** Protege o acesso ao recurso compartilhado contra escritores[cite: 1]. 
    *   O **primeiro leitor** a chegar bloqueia este mutex, impedindo a entrada de qualquer escritor[cite: 1].
    *   Os leitores subsequentes entram livremente (pois a exclusão mútua de leitura não é ativada para eles).
    *   O **último leitor** a sair libera este mutex, sinalizando que a área está vazia e permitindo que o escritor possa, enfim, bloquear o recurso e realizar a sua escrita com acesso exclusivo[cite: 1].