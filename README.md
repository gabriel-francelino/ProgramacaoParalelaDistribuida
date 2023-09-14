# Programação Paralela e Distribuida utilizadando a biblioteca MPI

## O que é MPI?

O MPI(Message Passing Interface) uma biblioteca amplamente utilizada para programação paralela em sistemas distribuídos. O MPI permite a comunicação e coordenação entre processos paralelos. Aqui estão alguns conceitos-chave:

### Processos MPI:

Um processo MPI é uma instância separada de um programa em execução que pode executar de forma independente. Os processos MPI podem ser executados em um único nó ou em vários nós de um cluster.

```
// Inicializa o ambiente MPI
MPI_Init(NULL, NULL);
int world_rank;
// Obtém o rank do processo dentro do comunicador MPI_COMM_WORLD
MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
// Imprime o rank do processo
printf("Processo com rank %d\n", world_rank);
// Finaliza o ambiente MPI
MPI_Finalize();
```

### Comunicadores:

Os comunicadores MPI são grupos lógicos de processos que podem se comunicar entre si. O MPI_COMM_WORLD é o comunicador padrão que contém todos os processos em uma aplicação MPI, mas você pode criar comunicadores personalizados para grupos específicos de processos.

```
MPI_Comm custom_comm;
// Duplica o comunicador MPI_COMM_WORLD para criar um novo comunicador customizado
MPI_Comm_dup(MPI_COMM_WORLD, &custom_comm);
```

### Rank:

Cada processo em um comunicador MPI é identificado por um número inteiro exclusivo chamado "rank". O rank é usado para distinguir um processo dos outros dentro do comunicador.

```
int world_rank;
// Obtém o rank do processo dentro do comunicador MPI_COMM_WORLD
MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
// Imprime o rank do processo
printf("Meu rank é %d\n", world_rank);
```

### Size:

O tamanho de um comunicador MPI é o número total de processos nesse comunicador. É frequentemente usado para determinar quantos processos estão envolvidos em uma operação paralela.

```
int world_size;
// Obtém o tamanho do comunicador MPI_COMM_WORLD (número total de processos)
MPI_Comm_size(MPI_COMM_WORLD, &world_size);
// Imprime o tamanho do comunicador
printf("Tamanho do comunicador é %d\n", world_size);
```

### Envio e Recebimento de Mensagens:

A comunicação em MPI é baseada em troca de mensagens. Os processos podem enviar dados para outros processos usando operações de envio (e.g., MPI_Send) e receber dados usando operações de recebimento (e.g., MPI_Recv).

#### MPI_Send()

A função MPI_Send é usada para enviar dados de um processo para outro processo. Ela tem a seguinte assinatura:

```
int MPI_Send(void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);
```

- **buf**: Um ponteiro para os dados que você deseja enviar.
- **count**: O número de elementos no buffer buf que você deseja enviar.
- **datatype**: O tipo de dados dos elementos em buf. Isso deve corresponder ao tipo de dados real dos elementos em buf.
- **dest**: O rank do destino, ou seja, o rank do processo que receberá a mensagem.
- **tag**: Uma etiqueta (um número inteiro) que você pode associar à mensagem. É usado para distinguir mensagens diferentes entre os mesmos processos.
- **comm**: O comunicador MPI que identifica o grupo de processos envolvidos na comunicação.

### MPI_Recv()

A função MPI_Recv é usada para receber dados de outro processo. Ela tem a seguinte assinatura:

```
int MPI_Recv(void* buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status* status);
```

- **buf**: Um ponteiro para um buffer onde os dados recebidos serão armazenados.
- **count**: O número de elementos esperados para receber.
- **datatype**: O tipo de dados esperado nos elementos que serão recebidos. Deve corresponder ao tipo de dados real dos elementos recebidos.
- **source**: O rank da origem, ou seja, o rank do processo que está enviando a mensagem.
- **tag**: A etiqueta associada à mensagem que você deseja receber. Isso permite que você filtre mensagens com base em etiquetas específicas.
- **comm**: O comunicador MPI que identifica o grupo de processos envolvidos na comunicação.
- **status**: Uma estrutura que armazena informações sobre a operação de recebimento, como a origem da mensagem e a etiqueta. Pode ser usado para verificar o status da operação.

### Exemplos

```
int data = 42;
if (world_rank == 0) {
    // Processo 0 envia um valor inteiro (42) para o processo 1
    MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
} else if (world_rank == 1) {
    // Processo 1 recebe um valor inteiro do processo 0
    MPI_Recv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // Imprime o valor recebido
    printf("Processo 1 recebeu: %d\n", data);
}
```

### Blocking vs. Non-blocking:

Operações de envio e recebimento podem ser bloqueadoras ou não bloqueadoras. Operações bloqueadoras esperam até que a comunicação seja concluída, enquanto operações não bloqueadoras permitem que um processo continue a executar outras tarefas enquanto aguarda a conclusão da comunicação.

```
int data = 42;
MPI_Request request;
if (world_rank == 0) {
    // Processo 0 envia de forma não bloqueadora
    MPI_Isend(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
} else if (world_rank == 1) {
    // Processo 1 recebe de forma não bloqueadora
    MPI_Irecv(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
    // Realiza outras operações enquanto aguarda
    MPI_Wait(&request, MPI_STATUS_IGNORE);
    // Imprime o valor recebido
    printf("Processo 1 recebeu: %d\n", data);
}
```

### Sincronização:

MPI fornece mecanismos para sincronização entre processos, como MPI_Barrier, que faz com que os processos aguardem até que todos tenham alcançado um determinado ponto no código antes de continuar.

```
// Todos os processos em MPI_COMM_WORLD aguardam na barreira até que todos cheguem aqui
MPI_Barrier(MPI_COMM_WORLD);
// Imprime mensagem após a barreira
printf("Todos os processos atingiram a barreira.\n");
```

### Reduções MPI:

As reduções MPI são operações que permitem que os processos combinem seus dados em um único resultado, como somar todos os valores em um array.

```
int data[] = {1, 2, 3, 4, 5};
int sum = 0;
// Redução - soma os valores de 'data' em todos os processos e armazena o resultado em 'sum' no processo 0
MPI_Reduce(&data, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
if (world_rank == 0) {
    // Processo 0 imprime a soma total
    printf("Soma de todos os valores: %d\n", sum);
}
```

### Comunicação Coletiva:

MPI oferece operações de comunicação coletiva que envolvem todos os processos em um comunicador, como MPI_Bcast para difusão de dados ou MPI_Reduce para reduções globais.

```
int data = 42;
// Processo 0 transmite o valor '42' para todos os processos no comunicador MPI_COMM_WORLD
MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
```

### MPI em Clusters:

MPI é frequentemente usado em sistemas distribuídos, como clusters de computadores. Ele permite que processos em diferentes nós se comuniquem e colaborem em uma tarefa paralela.

### Erro e Manipulação de Erro:

MPI fornece mecanismos para detectar erros de comunicação e recuperação de falhas.

### Perfil MPI:

Ferramentas de perfil MPI permitem analisar o desempenho de um programa MPI para otimização.

_Obs: Este texto foi gerado pelo chatGPT_