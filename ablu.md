Respostas aos "Pontos a explicar" (Guia para a sua apresentação)
Você precisará explicar esses pontos para garantir a nota da apresentação. Use este guia como base:  

Por que o contador de leitores precisa ser protegido?
O contador (contador_leitores) é uma variável global manipulada por múltiplas threads leitoras simultaneamente. Se ele não fosse protegido pelo mutex_contador, sofreria condição de corrida (duas threads tentando ler e incrementar o valor no mesmo milissegundo, resultando em uma contagem errada).  

Por que o primeiro leitor bloqueia o escritor?
O escritor não pode alterar os dados enquanto há alguém lendo. O primeiro leitor que chega pega a "chave" do mutex_recurso. Isso impede o escritor de entrar na sua região crítica.  
+1

Por que o último leitor libera o escritor?
Se houver leitores ativos, o escritor tem que continuar esperando. Apenas quando o contador zera (significando que não sobrou absolutamente ninguém lendo), o último leitor que está saindo devolve a chave do mutex_recurso, permitindo que o escritor finalmente possa acessar e alterar o dado.  

Por que os leitores podem executar simultaneamente?
Porque a única coisa que os leitores travam exclusivamente para eles mesmos (usando mutex_contador) é o ato de atualizar o número de pessoas na sala (o contador). A sala em si (a região de leitura) fica destravada para que outros leitores entrem, já que apenas o primeiro leitor pegou o lock do recurso principal.  
+1

A solução implementada pode causar inanição (starvation) de escritores?
Sim, pode. Esta é a clássica solução com "Prioridade para Leitores". Se novos leitores continuarem chegando constantemente antes que os leitores atuais terminem, o contador_leitores nunca chegará a zero. Consequentemente, o último leitor nunca vai executar o unlock no recurso, e o escritor ficará bloqueado ("faminto") para sempre.