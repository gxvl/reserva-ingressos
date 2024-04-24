***Sistema de Reserva de Ingressos***

Este é um sistema de reserva de ingressos concorrente desenvolvido em C. Ele permite que múltiplos compradores reservem ingressos para um evento,
garantindo que o número total de ingressos reservados não exceda a capacidade máxima do evento.

***Funcionamento***
O sistema consiste em duas principais funcionalidades: a geração de ingressos e a reserva de ingressos.

***Geração de Ingressos***
A geração de ingressos inicia apenas quando os 10 ingressos iniciais são comprados. Nesse momento, a geração de ingressos
adicionais é iniciada de forma aleatória em intervalos de tempo.
Cada vez que novos ingressos são gerados, o número total de ingressos disponíveis é atualizado.

***Reserva de Ingressos***
Os compradores podem tentar reservar ingressos a qualquer momento. Eles podem tentar reservar um número aleatório de ingressos 
entre 1 e um máximo permitido por transação.
Se houver ingressos disponíveis suficientes, a reserva é realizada e o número de ingressos disponíveis é atualizado. Caso contrário, os compradores
são notificados de que não há ingressos suficientes e devem tentar novamente.
