# Nider [![Build Status](https://travis-ci.org/capgadsx/Nider.svg?branch=master)](https://travis-ci.org/capgadsx/Nider) [![License](https://img.shields.io/badge/License-Apache%20v2-blue.svg)](https://github.com/capgadsx/Nider/blob/master/LICENCE.md)

## Construir Nider

### Dependencias

#### OpenCV

1. Clona el [repositorio](https://github.com/opencv/opencv/tree/e0435d3b8a871b54f9a78bb8662e47e47c36b05c) desde Github *(Hasta el instante [#7356](https://github.com/opencv/opencv/pull/7356))*

2. Instala las dependencias:

    ``sudo apt-get install cmake libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev libbz2-dev libwebp-dev``

3. Configura OpenCV
	* Ejecuta en el directorio de OpenCV el siguiente comando:

      ``cmake -DWITH_IPP=OFF -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_opencv_python2=OFF -DBUILD_opencv_python3=OFF``

4. Compila e Instala OpenCV
  * Ejecuta en el directorio de OpenCV el siguiente comando:

      ``make && sudo make install``

## Obtener Nider

1. Clona el [repositorio](https://github.com/capgadsx/nider) desde Github

## Compilar Nider

1. Un consejo para mantener el orden en el directorio es crear dos carpetas: **build** y **data**, en *build* se almacenaran los ficheros necesarios para compilar el proyecto y en *data* se almacenaran los datos necesarios para ejecutarlo

2. Ingresa al directorio *build* y ejecuta los siguientes comandos:

      1. ``cmake ../``

      2. ``make``

3. ¡Listo!. Si se instalaron todas las dependencias correctamente no debería haber problemas al compilar el proyecto y deberías poder ver un ejecutable llamado *nider* en la carpeta *build*

## Usar Nider

*PRÓXIMAMENTE* :+1: :tada:
