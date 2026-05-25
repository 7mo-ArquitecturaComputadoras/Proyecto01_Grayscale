# 🛠️ Instalación y Puesta en Marcha

Guía paso a paso para clonar, compilar y ejecutar el **Proyecto 01 — Grayscale** en un equipo nuevo. El proyecto está escrito en **ensamblador x64 (MASM)** con una interfaz en **C++** y se compila con **Visual Studio** en **Windows**.

---

## 📑 Índice

- [📚 ¿Qué herramientas necesitas?](#-qué-herramientas-necesitas)
- [🔗 Enlaces de descarga](#-enlaces-de-descarga)
- [1️⃣ Instalar Git](#1️⃣-instalar-git)
- [2️⃣ Instalar Visual Studio con MASM](#2️⃣-instalar-visual-studio-con-masm)
- [3️⃣ Clonar el repositorio](#3️⃣-clonar-el-repositorio)
- [4️⃣ Abrir la solución en Visual Studio](#4️⃣-abrir-la-solución-en-visual-studio)
- [5️⃣ Habilitar MASM en el proyecto](#5️⃣-habilitar-masm-en-el-proyecto)
- [6️⃣ Compilar el proyecto](#6️⃣-compilar-el-proyecto)
- [7️⃣ Ejecutar y observar el resultado](#7️⃣-ejecutar-y-observar-el-resultado)
- [❗ Problemas comunes](#-problemas-comunes)

---

## 📚 ¿Qué herramientas necesitas?

Antes de empezar, conviene entender qué hace cada programa:

- **Git**: Sistema de control de versiones. Sirve para descargar (clonar) el repositorio a tu computadora.
- **Visual Studio**: Entorno integrado de desarrollo (IDE) de Microsoft. Incluye el compilador, el linker y, lo más importante para este proyecto, **MASM** (`ml64.exe`), el ensamblador de 64 bits que traduce `grayscale.asm` a un ejecutable.
- **MASM** (*Microsoft Macro Assembler*): No se instala por separado, viene incluido dentro de la carga de trabajo *Desktop development with C++* de Visual Studio.

> 💡 No se requiere ningún compilador externo ni librerías adicionales. Todo lo que necesitas está dentro de Visual Studio.

> 💡 Para probar el programa con tu propia imagen, necesitarás un editor que sepa guardar en BMP de 24 bits (Paint de Windows, GIMP, Photoshop, IrfanView, etc.).

---

## 🔗 Enlaces de descarga

| Herramienta | Windows |
|---|---|
| **Git** | [https://git-scm.com/download/win](https://git-scm.com/download/win) |
| **Visual Studio Community** (gratuito) | [https://visualstudio.microsoft.com/es/downloads/](https://visualstudio.microsoft.com/es/downloads/) |

> ⚠️ Este proyecto solo se compila en **Windows**, porque el código usa la convención de llamadas Microsoft x64 con MASM. No funciona nativamente en Linux ni macOS.

---

## 1️⃣ Instalar Git

1. Entra a [https://git-scm.com/download/win](https://git-scm.com/download/win).
2. Descarga el instalador `.exe` (la descarga inicia automáticamente).
3. Ejecuta el instalador y acepta las opciones por defecto pulsando **"Next"** en cada pantalla.

Para verificar la instalación, abre el **Símbolo del sistema** (escribe `cmd` en el menú Inicio) y ejecuta:

```cmd
git --version
```

Si aparece un número de versión, todo está listo.

---

## 2️⃣ Instalar Visual Studio con MASM

1. Entra a [https://visualstudio.microsoft.com/es/downloads/](https://visualstudio.microsoft.com/es/downloads/).
2. Descarga **Visual Studio Community** (la versión gratuita).
3. Ejecuta el instalador. Aparecerá una ventana llamada **"Visual Studio Installer"**.
4. En la pestaña **"Cargas de trabajo"** (*Workloads*), marca la casilla:

   ✅ **Desarrollo para el escritorio con C++** (*Desktop development with C++*)

   > 🔑 Esta casilla es **obligatoria**: dentro de ella viene MASM x64 (`ml64.exe`), el ensamblador que compila el archivo `.asm`. Sin esta carga de trabajo, el proyecto **no compilará**.

5. Haz clic en **"Instalar"** y espera. La descarga e instalación puede tardar entre 30 minutos y 2 horas según tu conexión.

> ⏳ Visual Studio ocupa entre 8 y 15 GB de espacio en disco con esta carga de trabajo.

---

## 3️⃣ Clonar el repositorio

Abre el **Símbolo del sistema** (`cmd`) o **Git Bash**, ubícate en la carpeta donde quieras guardar el proyecto y ejecuta:

```bash
git clone git@github.com:7mo-ArquitecturaComputadoras/Proyecto01_Grayscale.git
```

---

## 4️⃣ Abrir la solución en Visual Studio

1. Entra a la carpeta `proyecto/` dentro del repositorio.
2. Haz doble clic sobre **`Proyecto01_Grayscale.slnx`**.
3. Visual Studio se abrirá y cargará automáticamente el proyecto, incluidos los archivos `src/grayscale.asm` y `src/main.cpp`.

> 💡 El archivo `.slnx` es la versión moderna de los `.sln` clásicos. Si tu versión de Visual Studio no lo reconoce, abre directamente el `.vcxproj`.

---

## 5️⃣ Habilitar MASM en el proyecto

La primera vez que abras la solución, es posible que Visual Studio no reconozca las directivas `PROC`, `.code`, etc. Para activar MASM:

1. En el **Explorador de soluciones** (panel derecho), haz clic derecho sobre el proyecto **Proyecto01_Grayscale**.
2. Selecciona **"Generar dependencias"** → **"Personalizaciones de compilación…"** (*Build Customizations…*).
3. En la lista que aparece, marca la casilla:

   ✅ **masm(.targets, .props)**

4. Pulsa **"Aceptar"**.

> ⚠️ Si esta casilla **no aparece** en la lista, significa que MASM no se instaló. Vuelve al paso 2️⃣ y verifica que marcaste la carga de trabajo *Desarrollo para el escritorio con C++*.

---

## 6️⃣ Compilar el proyecto

1. En la barra superior, selecciona la configuración **Release** (o **Debug**) y la plataforma **x64**.

   > 🔑 Es **obligatorio** usar **x64**, porque las rutinas en `grayscale.asm` están escritas con la convención de llamadas Microsoft x64 (`RCX`, `RDX`, `R8`). En **Win32** no compilará.

2. Pulsa `Ctrl + Shift + B` o ve al menú **Compilar** → **Compilar solución**.
3. En la ventana de salida (parte inferior) debe aparecer:

   ```
   ========== Compilación: 1 correctos, 0 incorrectos ==========
   ```

El ejecutable se generará en `proyecto/x64/Debug/Proyecto01_Grayscale.exe` (o `Release` según la configuración elegida).

> 💡 Las carpetas `Debug/`, `Release/` y `x64/` están incluidas en el `.gitignore` y no se suben al repositorio: cada quien las genera localmente al compilar.

---

## 7️⃣ Ejecutar y observar el resultado

El programa lee `src/input.bmp` (imagen de prueba incluida en el repositorio), aplica la conversión a escala de grises usando la rutina ensambladora, y guarda el resultado en `src/output.bmp`.

1. Pulsa `Ctrl + F5` para ejecutar sin depuración.
2. Observa la consola: debe imprimir tres mensajes (comprobación, conversión, éxito).
3. Abre `proyecto/src/output.bmp` con cualquier visor de imágenes (Paint, IrfanView, etc.) para ver el resultado.

### Ejemplo de ejecución

```
Comprobando existencia de src/input.bmp...
Convirtiendo a escala de grises...
Exito: Se ha logrado la salida en src/output.bmp
```

### Probar con tu propia imagen

1. Toma cualquier imagen (`.jpg`, `.png`, etc.).
2. Ábrela en Paint o GIMP y guárdala como BMP de 24 bits (sin compresión).
3. Renómbrala a `input.bmp` y reemplaza el archivo en `proyecto/src/`.
4. Ejecuta de nuevo el programa: la nueva `output.bmp` será su versión en grises.

> ⚠️ El programa **valida** que el BMP sea de 24 bits. Si la imagen es de 8, 16 o 32 bits, imprimirá:
> `Error: La imagen debe ser de 24 bits.`

### Inspección con el depurador

Para ver el contenido de los registros durante la conversión:

1. Abre `proyecto/src/grayscale.asm` en el editor.
2. Coloca un *breakpoint* en la línea `shr r11d, 8`.
3. Pulsa `F5` para iniciar la depuración (en Debug | x64).
4. Cuando se detenga, ve a **Depurar** → **Ventanas** → **Registros**.
5. Observa `R9D` (B×29), `R10D` (G×150) y `R11D` (suma ponderada).
6. Pulsa `F10` para ejecutar el `SHR` y ver `R11B` con el valor de gris en [0, 255].

---

## ❗ Problemas comunes

| Síntoma | Causa probable | Solución |
|---|---|---|
| `Error: No se encontro 'src/input.bmp'` | El archivo de entrada no está en la ruta esperada | Verifica que `proyecto/src/input.bmp` exista; el directorio de trabajo al ejecutar debe ser `proyecto/` |
| `Error: La imagen debe ser de 24 bits` | La imagen no está en formato BMP 24bpp | Vuelve a guardar la imagen en Paint o GIMP eligiendo "Mapa de bits de 24 bits" |
| `Error: El archivo no tiene un formato BMP valido` | El archivo no comienza con `42 4D` ("BM") | Asegúrate de que sea un BMP real, no un JPG/PNG renombrado |
| `error LNK2019: unresolved external symbol conversion_grayscale` | El símbolo no se exporta o `extern "C"` no coincide | Verifica que `grayscale.asm` declare `conversion_grayscale PROC` y que `main.cpp` la declare con `extern "C"` |
| `error MSB6006: "ml64.exe" exited with code 1` | Ruta del `.asm` rota o sintaxis incorrecta | Verifica que `proyecto/src/grayscale.asm` exista y que el `.vcxproj` lo incluya como `<MASM>` |
| **masm(.targets, .props)** no aparece | Falta la carga de trabajo *Desarrollo C++* | Abre **Visual Studio Installer**, pulsa **"Modificar"** y agrégala |
| Compilación falla en **Win32** | Código escrito en 64 bits | Cambia la plataforma a **x64** en la barra superior |
| La imagen de salida sale negra o con franjas | Cálculo de padding incorrecto al copiar filas | Verifica que el `vcxproj` esté compilando `main.cpp` y que `row_stride = width × 3 + padding` |
| `git` no se reconoce como comando | Git no se instaló o no se agregó al PATH | Reinstala Git marcando *"Git from the command line and also from 3rd-party software"* |
| El `.slnx` no abre | Versión de Visual Studio anterior a 2022 17.10 | Abre directamente `Proyecto01_Grayscale.vcxproj` |
| El programa cierra inmediatamente al ejecutar con `Ctrl + F5` | Visual Studio cerró la ventana al terminar | Ejecuta desde una terminal (`cmd`) o agrega un `system("pause")` temporal al final de `main.cpp` |

---

> **Autor:** Edson Joel Carrera Avila
