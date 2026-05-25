# 🔢 Proyecto 01 — Grayscale en Ensamblador x64

Programa mixto **ensamblador x64 (MASM) + C++** que lee una imagen en formato **BMP de 24 bits**, convierte cada uno de sus píxeles a un tono de gris equivalente usando la fórmula de luminancia **ITU-R BT.601** ($Y' = 0.299\,R + 0.587\,G + 0.114\,B$), y escribe el resultado en un nuevo archivo BMP. La rutina ensambladora `conversion_grayscale` aplica los pesos como enteros escalados por $2^8$ para evitar el costo de la aritmética en punto flotante.

---

## 📑 Índice

- [🎯 ¿Qué hace el programa?](#-qué-hace-el-programa)
- [🧠 Idea central del algoritmo](#-idea-central-del-algoritmo)
- [📂 Estructura del repositorio](#-estructura-del-repositorio)
- [🚀 Cómo empezar](#-cómo-empezar)
- [🔍 Trazado del ejemplo `pixel(B=200, G=100, R=50)`](#-trazado-del-ejemplo-pixelb200-g100-r50)
- [📘 Instrucciones x86/x64 utilizadas](#-instrucciones-x86x64-utilizadas)
- [📄 Documentación adicional](#-documentación-adicional)

---

## 🎯 ¿Qué hace el programa?

El programa lee el archivo `proyecto/src/input.bmp` (BMP de 24 bits), recorre toda la matriz de píxeles fila por fila, llama a la rutina ensambladora `conversion_grayscale` una vez por fila, y guarda la imagen resultante en `proyecto/src/output.bmp`. Las cabeceras BMP/DIB y el relleno (*padding*) de cada fila se conservan intactos, por lo que el archivo de salida sigue siendo un BMP válido de 24 bits que cualquier visor puede abrir.

La salida en consola es:

```
Comprobando existencia de src/input.bmp...
Convirtiendo a escala de grises...
Exito: Se ha logrado la salida en src/output.bmp
```

El programa valida que el archivo:
- Comience con la firma ASCII `"BM"` (`0x4D42`)
- Tenga `bit_count = 24` (formato de 24 bits sin tabla de colores)

Si alguna validación falla, imprime el error y termina con código `1`.

---

## 🧠 Idea central del algoritmo

### La fórmula de luminancia

Para convertir un píxel RGB a un tono de gris perceptualmente correcto se usa la fórmula del estándar **ITU-R BT.601**:

```
Y' = 0.299·R + 0.587·G + 0.114·B
```

Estos pesos reflejan la sensibilidad del ojo humano: el verde aporta cerca del 59 % de la luminosidad percibida, el rojo el 30 % y el azul apenas el 11 %. Por eso un simple promedio aritmético `(R+G+B)/3` produce escalas de grises incorrectas.

### Optimización con aritmética entera

En lugar de multiplicar por flotantes, los pesos se escalan por $2^8 = 256$ para trabajar con enteros:

| Canal | Peso flotante | Peso escalado |
|-------|---------------|---------------|
| B     | 0.114         | 29            |
| G     | 0.587         | 150           |
| R     | 0.299         | 77            |
| **Σ** | **1.000**     | **256**       |

Como la suma de los pesos enteros es exactamente $256 = 2^8$, la división final se reduce a un desplazamiento a la derecha de 8 bits (`SHR r11d, 8`), una de las instrucciones más rápidas del procesador. El error de redondeo es de a lo sumo ±1 en una escala de 256 niveles, imperceptible visualmente.

### Manejo del formato BMP

El formato BMP de 24 bits tiene tres particularidades importantes que el programa C++ resuelve antes de llamar a ensamblador:

1. **Orden BGR**: cada píxel almacena sus tres bytes en el orden Azul, Verde, Rojo (no RGB).
2. **Padding de filas**: cada fila se rellena con bytes nulos para que su tamaño en bytes sea múltiplo de 4.
3. **Almacenamiento bottom-up**: la primera fila del archivo corresponde a la fila más baja de la imagen.

El programa principal lee toda la matriz a un `vector<uint8_t>`, calcula el `row_stride = width × 3 + padding`, y procesa cada fila pasando a ensamblador solo los píxeles (`width` elementos), copiando el padding sin modificar.

### Flujo de ejecución de `conversion_grayscale`

```
Inicio
 └─ RCX = puntero entrada, RDX = puntero salida, R8 = N píxeles
 └─ TEST R8, R8 → si N == 0 → fin (retornar)

bucle:
 ├─ MOVZX R9D,  [RCX]   → R9D  = B  (azul,  offset 0)
 ├─ MOVZX R10D, [RCX+1] → R10D = G  (verde, offset 1)
 ├─ MOVZX R11D, [RCX+2] → R11D = R  (rojo,  offset 2)
 ├─ IMUL R9D,  29       → R9D  = B*29
 ├─ IMUL R10D, 150      → R10D = G*150
 ├─ IMUL R11D, 77       → R11D = R*77
 ├─ ADD R11D, R10D      → R11D += G*150
 ├─ ADD R11D, R9D       → R11D += B*29
 ├─ SHR R11D, 8         → R11D /= 256
 ├─ MOV [RDX],   R11B   → escribir gris en B
 ├─ MOV [RDX+1], R11B   → escribir gris en G
 ├─ MOV [RDX+2], R11B   → escribir gris en R
 ├─ ADD RCX, 3          → siguiente píxel entrada
 ├─ ADD RDX, 3          → siguiente píxel salida
 ├─ DEC R8
 └─ JNZ bucle           → repetir si quedan píxeles

fin:
 └─ RET
```

---

## 📂 Estructura del repositorio

```
Proyecto01_Grayscale/
├── documentacion/
│   ├── README_compilacion_latex.md         # Cómo compilar el .tex a PDF
│   ├── reporte.pdf                         # Reporte técnico compilado
│   ├── reporte.tex                         # Reporte técnico en LaTeX
│   └── imagenes/
│       ├── input.png                       # Imagen de entrada (referencia)
│       ├── output.png                      # Resultado en escala de grises
│       ├── consola_ejecucion.png           # Salida del programa
│       └── hex_header.png                  # Inspección hex del BMP
│
├── proyecto/
│   ├── README_instalacion.md               # Guía de instalación y puesta en marcha
│   ├── Proyecto01_Grayscale.slnx           # Solución de Visual Studio
│   ├── Proyecto01_Grayscale.vcxproj        # Proyecto MSBuild + MASM (x64)
│   └── src/
│       ├── grayscale.asm                   # Función conversion_grayscale en x64
│       ├── main.cpp                        # Lector/escritor BMP + bucle de filas
│       └── input.bmp                       # Imagen de prueba (24 bits)
│
├── .gitattributes                          # Normalización de finales de línea
├── .gitignore                              # Archivos ignorados por Git
└── README.md                               # Este archivo
```

---

## 🚀 Cómo empezar

La guía detallada con todos los pasos (instalar Git, Visual Studio, habilitar MASM, configurar la plataforma x64, compilar y ejecutar) está en un documento aparte:

➡️ **[Guía de instalación y puesta en marcha](proyecto/README_instalacion.md)**

Resumen rápido para quien ya tiene el entorno listo:

1. Abre el **Símbolo del sistema** (`cmd`) o **Git Bash**, ubícate en la carpeta donde quieras guardar el proyecto y ejecuta:

```bash
git clone git@github.com:7mo-ArquitecturaComputadoras/Proyecto01_Grayscale.git
```

2. Abrir `proyecto/Proyecto01_Grayscale.slnx` en Visual Studio.
3. Seleccionar configuración **Release | x64** (importante: x64, no Win32).
4. Compilar con `Ctrl + Shift + B` y ejecutar con `Ctrl + F5`.
5. Verificar que se creó `proyecto/src/output.bmp` y abrirlo con cualquier visor de imágenes.

> 💡 Para usar tu propia imagen, reemplaza `proyecto/src/input.bmp` por cualquier BMP de 24 bits con el mismo nombre antes de ejecutar.

---

## 🔍 Trazado del ejemplo `pixel(B=200, G=100, R=50)`

Tomemos un píxel con los valores `B=200`, `G=100`, `R=50` y veamos cómo lo procesa `conversion_grayscale`:

### Cálculo paso a paso

| Paso | Operación              | R9D (B)   | R10D (G)  | R11D (acum)   |
|------|------------------------|-----------|-----------|---------------|
| 1    | `MOVZX` (cargar)       | 200       | 100       | 50            |
| 2a   | `IMUL R9D, 29`         | **5 800** | 100       | 50            |
| 2b   | `IMUL R10D, 150`       | 5 800     | **15 000**| 50            |
| 2c   | `IMUL R11D, 77`        | 5 800     | 15 000    | **3 850**     |
| 3a   | `ADD R11D, R10D`       | 5 800     | 15 000    | 18 850        |
| 3b   | `ADD R11D, R9D`        | 5 800     | 15 000    | **24 650**    |
| 4    | `SHR R11D, 8` (÷256)   | —         | —         | **96** (R11B) |

### Verificación con la fórmula original

```
Y' = 0.299·50 + 0.587·100 + 0.114·200
   = 14.95 + 58.70 + 22.80
   = 96.45  →  redondeado a 96 ✓
```

El píxel original (azul vibrante) se convierte en un gris medio-oscuro de valor **96**. Los tres bytes `B, G, R` de salida quedan todos en `0x60` (96 en decimal), formando un píxel gris perceptualmente equivalente.

---

## 📘 Instrucciones x86/x64 utilizadas

### Carga y aritmética

| Instrucción | Operación                                                                                |
|-------------|------------------------------------------------------------------------------------------|
| `TEST`      | AND lógica sin guardar resultado; sólo afecta banderas. Detecta si `R8 == 0` al inicio.   |
| `MOVZX`     | *Move with Zero-eXtend*: carga un byte a un registro de 32 bits rellenando con ceros.    |
| `IMUL`      | *Integer Multiply*: multiplicación entera con un inmediato (forma de 3 operandos).       |
| `ADD`       | Suma entera; acumula los tres productos parciales en `R11D`.                             |
| `SHR`       | *Shift Right*: divide por $2^n$ desplazando bits a la derecha. Aquí `SHR 8 = ÷ 256`.    |
| `MOV`       | Escribe un byte (`R11B`) en la memoria de salida (canales B, G, R).                      |

### Control de flujo

| Instrucción | Operación                                                                          |
|-------------|------------------------------------------------------------------------------------|
| `JZ`        | *Jump if Zero*: salta si la bandera ZF está activada (resultado anterior fue 0).   |
| `JNZ`       | *Jump if Not Zero*: salta si ZF no está activada. Mantiene vivo el bucle.          |
| `DEC`       | Decrementa el contador `R8` en 1 y actualiza ZF.                                   |
| `RET`       | Retorna al llamador (`main.cpp`); rutina `void`, no devuelve valor.                |

---

## 📄 Documentación adicional

| Documento | Descripción |
|---|---|
| 🛠️ [`README_instalacion.md`](proyecto/README_instalacion.md) | Cómo instalar Git, Visual Studio con MASM, configurar la plataforma x64 y ejecutar el programa paso a paso. |
| 📄 [`README_compilacion_latex.md`](documentacion/README_compilacion_latex.md) | Cómo regenerar el PDF del reporte a partir de `reporte.tex` usando TeX Live, Geany o VS Code, tanto en Linux como en Windows. |
| 📕 [`reporte.pdf`](documentacion/reporte.pdf) | Reporte técnico ya compilado, con anatomía del formato BMP, referencias al estándar ITU-R BT.601 y análisis de la rutina ensambladora. |
| 📝 [`reporte.tex`](documentacion/reporte.tex) | Fuente LaTeX del reporte técnico con citas bibliográficas. |

---

> **Autor:** Edson Joel Carrera Avila
