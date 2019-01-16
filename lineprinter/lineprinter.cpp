// lineprinter.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include "pch.h"
#include <iostream>
#include <Windows.h>
#include "getopt.h"

void usage(const char * self) {
	fprintf(stderr, "Usage: %s -p printer (-f file)\n", self);
	exit(1);
}

int main(int argc, char *argv[]) {
	int ret = 1;

	int opt;
	char * printerName = NULL;
	char * fileName = NULL;

	while ((opt = getopt(argc, argv, "f:p:")) != -1) {
		switch (opt) {
			case 'f':
				if (fileName != NULL) {
					usage(argv[0]);
				}
				fileName = optarg;
				break;
			case 'p':
				if (printerName != NULL) {
					usage(argv[0]);
				}
				printerName = optarg;
				break;
			default:
				usage(argv[0]);
		}
	}

	if (printerName == NULL) {
		usage(argv[0]);
	}

	FILE* inputFile;
	if (fileName != NULL) {
		if (fopen_s(&inputFile, fileName, "rb") != 0) {
			perror("Failed to open input file");
			return 1;
		}
	} else {
		inputFile = stdin;
	}

	HANDLE printerHandle;
	if (!OpenPrinterA(printerName, &printerHandle, NULL)) {
		fprintf(stderr, "Failed opening printer\n");
		goto closefile;
	}

	DOC_INFO_1 docInfo;
	docInfo.pDatatype = (LPTSTR) L"RAW";
	docInfo.pOutputFile = NULL;
	docInfo.pDocName = (LPTSTR) L"Print job";

	if (!StartDocPrinter(printerHandle, 1, (LPBYTE) &docInfo)) {
		fprintf(stderr, "Failed starting job\n");
		goto closeprinter;
	}

	if (!StartPagePrinter(printerHandle)) {
		fprintf(stderr, "Failed starting page\n");
		goto closedoc;
	}

	while (true) {
		uint8_t buf[1024];
		size_t read = fread(buf, 1, 1024, inputFile);
		if (read == 0) {
			if (ferror(inputFile)) {
				fprintf(stderr, "Error reading from input\n");
			} else {
				ret = 0;
			}
			break;
		}

		DWORD printed;
		if (!WritePrinter(printerHandle, buf, read, &printed)) {
			fprintf(stderr, "Failed writing to printer\n");
			break;
		}
	}

	EndPagePrinter(printerHandle);
closedoc:
	EndDocPrinter(printerHandle);
closeprinter:
	ClosePrinter(printerHandle);
closefile:
	fclose(inputFile);
	return ret;
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
