# Control de Acceso Inteligente para Habitación

## Integrantes
- Luis Fernando Castro 
- William Camilo Obando

---

## Descripción del Proyecto

Este proyecto implementa un sistema embebido para el control de acceso y ventilación de una habitación utilizando un microcontrolador STM32. El sistema gestiona el acceso mediante un teclado y una pantalla OLED, validando una contraseña y controlando un ventilador con PWM basado en temperatura. Además, ofrece un modo de emergencia y control automático del ventilador

---

## Funcionalidades Implementadas

- Ingreso de contraseña desde un teclado matricial
- Control de acceso (bloqueo/desbloqueo de puerta)
- Visualización en pantalla OLED (clave, mensajes de estado)
- Control automático del ventilador por temperatura
- Control PWM del ventilador con distintos niveles (OFF, LOW, MED, HIGH)
- Cambio automático de estados según temporizadores o entradas
- Retroalimentación visual mediante asteriscos en pantalla al ingresar contraseña

---

## Instrucciones de Compilación y Uso

1. **Entorno Requerido:**
   - STM32CubeIDE o VSCode
   - Bibliotecas de drivers

2. **Conexiones:**
   - Teclado matricial a pines GPIO configurados como entradas/salidas
   - Pantalla OLED vía I2C
   - Ventilador conectado a pin PWM (TIM3_CH1)
   - Sensor de temperatura (Pin ADC_IN5)

3. **Pasos para Ejecutar:**
   - Abre el proyecto en STM32CubeIDE
   - Compila y flashea el código a la placa Nucleo 
   - Verifica conexiones con los pines
   - Interactúa con el sistema desde el teclado

---

## Decisiones de Diseño y Explicaciones de Código
---

### 1. `room_control_update(room_control_t *room)`

```c
void room_control_process_key(room_control_t *room, char key) {
    ...
}

´´´
