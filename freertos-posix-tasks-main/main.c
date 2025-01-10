#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stdio.h"

// Manipuladores de tarefas para gerenciamento
TaskHandle_t Task1_Handle;
TaskHandle_t Task2_Handle;
TaskHandle_t Task3_Handle;
TaskHandle_t Task4_Handle;

QueueHandle_t Queue;

void Task1(void *pvParameters); // Envia dados para a fila
void Task2(void *pvParameters); // Recebe dados da fila
void Task3(void *pvParameters); // Monitora a execução da Tarefa 1
void Task4(void *pvParameters); // Realiza um cálculo matemático periódico

int main(void) {
    // Criação de uma fila para comunicação entre tarefas
    Queue = xQueueCreate(5, sizeof(int));
    if (Queue == NULL) {
        printf("Falha ao criar a fila.\n");
        return 1;
    }

    xTaskCreate(Task1, "Task1", configMINIMAL_STACK_SIZE * 2, NULL, 2, &Task1_Handle);
    xTaskCreate(Task2, "Task2", configMINIMAL_STACK_SIZE * 2, NULL, 1, &Task2_Handle);
    xTaskCreate(Task3, "Task3", configMINIMAL_STACK_SIZE * 2, NULL, 2, &Task3_Handle);
    xTaskCreate(Task4, "Task4", configMINIMAL_STACK_SIZE * 2, NULL, 1, &Task4_Handle);
    // Inicia o escalonador do FreeRTOS
    vTaskStartScheduler();

    return 0;
}

// Envia dados para a fila
void Task1(void *pvParameters) {
    int contador = 0;
    for (;;) {
        printf("Tarefa 1: Enviando dado %d para a fila...\n", contador);
        xQueueSend(Queue, &contador, portMAX_DELAY); 
        contador++;
        vTaskDelay(1000 / portTICK_PERIOD_MS); 
    }
}

// Recebe dados da fila
void Task2(void *pvParameters) {
    int valorRecebido;
    for (;;) {
        if (xQueueReceive(Queue, &valorRecebido, portMAX_DELAY)) {
            printf("Tarefa 2: Recebeu dado %d da fila.\n", valorRecebido);
        }
    }
}

//  Monitora a Tarefa 1 e imprime sua atividade periodicamente
void Task3(void *pvParameters) {
    for (;;) {
        printf("Tarefa 3: Monitorando a Tarefa 1...\n");
        vTaskDelay(3000 / portTICK_PERIOD_MS); 
    }
}

// Realiza cálculos matemáticos periodicamente
void Task4(void *pvParameters) {
    for (;;) {
        printf("Tarefa 4: Realizando cálculo matemático: 42 * 2 = %d\n", 42 * 2);
        vTaskDelay(2000 / portTICK_PERIOD_MS); 
    }
}


