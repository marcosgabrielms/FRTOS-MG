# FRTOS-MG
Atividade: Sistema de Monitoramento com 3 tarefas usando o FreeRTOS.

O projeto utiliza o FreeRTOS na BitDogLab para gerenciar três tarefas distintas que interagem entre si por meio de fila de tasks para controlar um LED baseando no estado do botão

As variáveis estão definidas no escopo do arquivo FRROS-MG.c, o compartilhamento de informações entre as tarefas é osbervada através do monitor serial do VScode, no qual mostra quando o botão
não está pressionado e quando está pressionado.

# Criação das Tarefas no FreeRTOS

**Tarefa 1: Leitura do botão**
Criada com uma prioridade baixa.
Executada a cada 100ms.

**Tarefa 2: Processamento e decisão**
Executada dependendo do estado do botão.
Analisa o estado do botão e decide a ação a ser tomada.

**Tarefa 3: Controle do LED**
Executada apenas quando acionada pela Tarefa 2.
Responsável por ligar ou desligar o LED conforme a decisão da Tarefa 2.

# Sincronização entre as Tarefas
A tarefa 2 envia um sinal para a tarefa 3 sempre que detecta uma mudança relevante no estado do botão.

# Considerações
O projeto demostra a aplicação de conceitos fundamentais do FreeRTOS, como multitarefas, comunicação entre tarefas e controle eficiente de hardware embarcado.
