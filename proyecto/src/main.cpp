// ============================================================
// Autor: Edson Joel Carrera Avila
// main.cpp
// ============================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>

extern "C" void conversion_grayscale(uint8_t* input, uint8_t* output, uint64_t pixel_count);
using namespace std;

// Forzamos a que el compilador no agregue padding a las estructuras
#pragma pack(push, 1)
struct BMPHeader {
    uint16_t file_type{0x4D42}; // Firma 'BM'
    uint32_t file_size{0};
    uint16_t reserved1{0};
    uint16_t reserved2{0};
    uint32_t offset_data{0};
};

struct DIBHeader {
    uint32_t size{0};
    int32_t width{0};
    int32_t height{0};
    uint16_t planes{1};
    uint16_t bit_count{0};
    uint32_t compression{0};
    uint32_t size_image{0};
    int32_t x_pixels_per_meter{0};
    int32_t y_pixels_per_meter{0};
    uint32_t colors_used{0};
    uint32_t colors_important{0};
};
#pragma pack(pop)

int main() {
    const string input_filename = "src/input.bmp";
    const string output_filename = "src/output.bmp";

    cout << "Comprobando existencia de " << input_filename << "..." << endl;

    // 1. Comprobar que exista input.bmp
    ifstream file(input_filename, ios::binary);
    if (!file) {
        cout << "Error: No se encontro '" << input_filename << "' en el directorio de trabajo." << endl;
        return 1;
    }

    BMPHeader bmp_header;
    file.read(reinterpret_cast<char*>(&bmp_header), sizeof(bmp_header));

    if (bmp_header.file_type != 0x4D42) {
        cout << "Error: El archivo no tiene un formato BMP valido." << endl;
        return 1;
    }

    DIBHeader dib_header;
    file.read(reinterpret_cast<char*>(&dib_header), sizeof(dib_header));

    // 2. Comprobar que sea un archivo de 24 bits
    if (dib_header.bit_count != 24) {
        cout << "Error: La imagen debe ser de 24 bits. " << endl;
        return 1;
    }

    cout << "Convirtiendo a escala de grises..." << endl;

    // Navegar hasta el inicio de los píxeles
    file.seekg(bmp_header.offset_data, ios::beg);

    int width = dib_header.width;
    int height = abs(dib_header.height);

    // Calcular el padding de BMP
    int row_width_bytes = width * 3;
    int padding = (4 - (row_width_bytes % 4)) % 4;
    int row_stride = row_width_bytes + padding;

    // Leer todos los datos de los píxeles
    vector<uint8_t> input_data(row_stride * height);
    file.read(reinterpret_cast<char*>(input_data.data()), input_data.size());
    file.close();

    vector<uint8_t> output_data(input_data.size());

    // 3. Usar grayscale.asm procesando fila por fila
    for (int y = 0; y < height; ++y) {
        uint8_t* row_in = input_data.data() + (y * row_stride);
        uint8_t* row_out = output_data.data() + (y * row_stride);

        // Pasamos la fila exacta a ASM (rcx = row_in, rdx = row_out, r8 = width)
        conversion_grayscale(row_in, row_out, width);

        // Copiar los bytes de padding al archivo de salida sin modificarlos
        for(int p = 0; p < padding; ++p) {
            row_out[row_width_bytes + p] = row_in[row_width_bytes + p];
        }
    }

    // 4. Crear output.bmp
    ofstream out_file(output_filename, ios::binary);
    if (!out_file) {
        cerr << "Error: No se pudo crear el archivo " << output_filename << endl;
        return 1;
    }

    // Escribir cabeceras intactas
    out_file.write(reinterpret_cast<const char*>(&bmp_header), sizeof(bmp_header));
    out_file.write(reinterpret_cast<const char*>(&dib_header), sizeof(dib_header));

    // Escribir los píxeles procesados en la ubicación correcta
    out_file.seekp(bmp_header.offset_data, ios::beg);
    out_file.write(reinterpret_cast<const char*>(output_data.data()), output_data.size());
    out_file.close();

    // 5. Interfaz mínima (aviso de salida)
    cout << "Exito: Se ha logrado la salida en " << output_filename << endl;

    return 0;
}
