# Nider [![Build Status](https://travis-ci.org/capgadsx/nider.svg?branch=master)](https://travis-ci.org/capgadsx/nider) [![License](https://img.shields.io/badge/License-Apache%20v2-blue.svg)](https://github.com/capgadsx/nider/blob/master/LICENCE.md)

## Construir Nider

### Dependencias

#### OpenCV

1. Clona el [repositorio](https://github.com/opencv/opencv/tree/e0435d3b8a871b54f9a78bb8662e47e47c36b05c) desde Github *(Hasta el instante [#7356](https://github.com/opencv/opencv/pull/7356))*

2. Instala las dependencias:

    ``sudo apt-get install cmake libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev libbz2-dev libwebp-dev``

3. Configura OpenCV
	* Ejecuta en el directorio de OpenCV el siguiente comando:

      ``cmake -DWITH_IPP=OFF -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF -DBUILD_PERF_TESTS=OFF``

4. Compila e Instala OpenCV
  * Ejecuta en el directorio de OpenCV el siguiente comando:

      ``make && sudo make install``

#### Tensorflow

1. Instala las dependencias:

    ``sudo pip install --upgrade pip``

    ``sudo pip install numpy six protobuf``

    Opcional: Si quieres usar la aceleracion por GPU, es necesario instalar CUDA Toolkit v7.5 y cuDNN v5

2. Elige la version a instalar:

    * Linux 64bits, CPU:

      ``export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-0.11.0rc1-cp27-none-linux_x86_64.whl``

    * Linux 64bits, CPU+GPU:

      ``export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow-0.11.0rc1-cp27-none-linux_x86_64.whl``

3. Instala Tensorflow:

    ``sudo pip install --upgrade $TF_BINARY_URL``

## Compilar Nider

1. Un consejo para mantener el orden en el directorio es crear dos carpetas: **build** y **data**, en *build* se almacenaran los ficheros necesarios para compilar el proyecto y en *data* se almacenaran los datos necesarios para ejecutarlo

2. Ingresa al directorio *build* y ejecuta los siguientes comandos:

      1. ``cmake ../``

      2. ``make``

3. ¡Listo!. Si se instalaron todas las dependencias correctamente no debería haber problemas al compilar el proyecto y deberías poder ver un ejecutable llamado *nider* en la carpeta *build*

## Usar Nider

*PRÓXIMAMENTE* :+1: :tada:
