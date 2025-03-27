#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Definição dos pinos para a BitDogLab RP2040
#define BOTAO_A_PIN  5    // Botão A (GP4)
#define LED_G_PIN    11   // LED RGB (Vermelho - GP18)
#define LED_B_PIN 12

// Declaração das filas para comunicação entre tarefas
QueueHandle_t xFilaBotao;
QueueHandle_t xFilaLED;

// Tarefa 1: Leitura do botão
void vTarefaLeituraBotao(void *pvParameters) {
    bool estadoBotao;

    while (1) {
        estadoBotao = gpio_get(BOTAO_A_PIN);  // Lê o estado do botão
        printf("Botão A: %s\n", estadoBotao ? "Solto" : "Pressionado");  
        xQueueSend(xFilaBotao, &estadoBotao, portMAX_DELAY); // Envia estado para a fila
        gpio_put(LED_B_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(100)); // Executa a cada 100ms
        
    }
}

// Tarefa 2: Processamento do botão
void vTarefaProcessamentoBotao(void *pvParameters) {
    bool estadoBotao;
    bool comandoLED;

    while (1) {
        if (xQueueReceive(xFilaBotao, &estadoBotao, portMAX_DELAY)) {
            comandoLED = (estadoBotao == false);  // Botão pressionado (LOW) aciona LED
            xQueueSend(xFilaLED, &comandoLED, portMAX_DELAY); // Envia comando ao LED
        }
    }
}

// Tarefa 3: Controle do LED
void vTarefaControleLED(void *pvParameters) {
    bool estadoLED;

    while (1) {
        if (xQueueReceive(xFilaLED, &estadoLED, portMAX_DELAY)) {
            gpio_put(LED_G_PIN, estadoLED);  //  LOW apaga, HIGH acende
        }
    }
}

int main() {
    // Inicializa comunicação serial
    stdio_init_all(); 

    // Configuração dos pinos
    gpio_init(BOTAO_A_PIN);
    gpio_set_dir(BOTAO_A_PIN, GPIO_IN);
    gpio_pull_up(BOTAO_A_PIN); // Habilita pull-up interno

    gpio_init(LED_G_PIN);
    gpio_set_dir(LED_G_PIN, GPIO_OUT);
    gpio_put(LED_G_PIN, !gpio_get(BOTAO_A_PIN)); // LED começa apagado

    // Criação das filas para comunicação entre tarefas
    xFilaBotao = xQueueCreate(5, sizeof(bool));
    xFilaLED = xQueueCreate(5, sizeof(bool));

    if (xFilaBotao == NULL || xFilaLED == NULL) {
        printf("Erro ao criar filas.\n");
        return 1;
    }

    // Criação das tarefas do FreeRTOS
    xTaskCreate(vTarefaLeituraBotao, "Leitura Botao", 256, NULL, 1, NULL);
    xTaskCreate(vTarefaProcessamentoBotao, "Processamento Botao", 256, NULL, 2, NULL);
    xTaskCreate(vTarefaControleLED, "Controle LED", 256, NULL, 1, NULL);

    // Inicia o scheduler do FreeRTOS
    vTaskStartScheduler();

    // Loop de segurança (não deve chegar aqui)
    while (1);
}
