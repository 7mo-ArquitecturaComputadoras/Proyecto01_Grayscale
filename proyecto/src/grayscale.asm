; ============================================================
; Autor: Edson Joel Carrera Avila
; grayscale.asm
; ============================================================

.code

conversion_grayscale PROC

    test r8, r8         ; Evalúa si el contador de píxeles es cero
                        ; (test hace AND sin guardar resultado, sólo afecta flags)
    jz fin              ; Si r8 == 0, no hay píxeles que procesar → saltar al final

bucle:
    ; ---------------------------------------------------------
    ; PASO 1: Cargar los tres canales del píxel actual
    ; El formato BGR almacena los bytes en el orden: B, G, R
    ; movzx extiende el byte a 32 bits rellenando con ceros
    ; (evita basura en bits superiores al operar con imul)
    ; ---------------------------------------------------------
    movzx r9d,  byte ptr [rcx]     ; r9d  = B (canal Azul,  offset 0)
    movzx r10d, byte ptr [rcx+1]   ; r10d = G (canal Verde, offset 1)
    movzx r11d, byte ptr [rcx+2]   ; r11d = R (canal Rojo,  offset 2)

    ; ---------------------------------------------------------
    ; PASO 2: Aplicar los pesos de luminancia a cada canal
    ;   B: 0.114 × 256 =  29 aprox.
    ;   G: 0.587 × 256 = 150 aprox.
    ;   R: 0.299 × 256 =  77 aprox.
    ;   Suma total de pesos: 29+150+77 = 256 (= 2^8)
    ; ---------------------------------------------------------
    imul r9d,  29                  ; r9d  = B * 29
    imul r10d, 150                 ; r10d = G * 150
    imul r11d, 77                  ; r11d = R * 77

    ; ---------------------------------------------------------
    ; PASO 3: Sumar los tres canales ponderados
    ; Se acumula todo en r11d para minimizar instrucciones
    ; ---------------------------------------------------------
    add r11d, r10d                 ; r11d = R*77 + G*150
    add r11d, r9d                  ; r11d = R*77 + G*150 + B*29

    ; ---------------------------------------------------------
    ; PASO 4: Normalizar dividiendo por 256 (shift derecho de 8 bits)
    ; Como los pesos suman 256, esto equivale a dividir por 256,
    ; obteniendo el valor final de gris en el rango [0, 255].
    ; El byte resultante queda accesible como r11b (byte bajo de r11d)
    ; ---------------------------------------------------------
    shr r11d, 8                    ; r11d >>= 8  →  r11b = valor de gris [0..255]

    ; ---------------------------------------------------------
    ; PASO 5: Escribir el valor de gris en los 3 canales de salida
    ; Se usa r11b (parte baja de r11d, 8 bits) para escribir
    ; el mismo valor de gris en B, G y R del píxel de salida
    ; ---------------------------------------------------------
    mov byte ptr [rdx],   r11b     ; Canal B de salida = gris
    mov byte ptr [rdx+1], r11b     ; Canal G de salida = gris
    mov byte ptr [rdx+2], r11b     ; Canal R de salida = gris

    ; ---------------------------------------------------------
    ; PASO 6: Avanzar ambos punteros al siguiente píxel
    ; Cada píxel ocupa 3 bytes (B, G, R), así que se suma 3
    ; ---------------------------------------------------------
    add rcx, 3                     ; rcx → siguiente píxel de entrada
    add rdx, 3                     ; rdx → siguiente píxel de salida

    ; ---------------------------------------------------------
    ; PASO 7: Decrementar contador y repetir si quedan píxeles
    ; ---------------------------------------------------------
    dec r8                         ; r8-- (un píxel menos por procesar)
    jnz bucle                      ; Si r8 != 0, volver al inicio del bucle

fin:
    ret

conversion_grayscale ENDP

END