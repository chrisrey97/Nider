# Nider

## Construir Nider

### Dependencias

#### CUDA (Opcional)

1. Asegúrate de que usas la última versión de los drivers oficiales de nvidia **(version >= 365.35)**

2. Descarga el [CUDA Toolkit 8 ](https://developer.nvidia.com/cuda-downloads)

3. Ejecuta el fichero descargado y sigue las instrucciones de instalación que se muestran en la consola

#### OpenCV

1. Clona el [repositorio](https://github.com/opencv/opencv/tree/e0435d3b8a871b54f9a78bb8662e47e47c36b05c) desde Github *(Hasta el instante [#7356](https://github.com/opencv/opencv/pull/7356))*

2. Instala las dependencias:

    ``sudo apt-get install cmake libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev libbz2-dev libwebp-dev``

3. Configura OpenCV
	* Ejecuta en el directorio de OpenCV el siguiente comando:
	* Opcional: Si vas a usar CUDA añade al comando la siguiente *flag:* **-DWITH_CUDA=ON**

      ``cmake -DBUILD_EXAMPLES=OFF -DBUILD_TESTS=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_opencv_python2=OFF -DBUILD_opencv_python3=OFF``

4. Compila e Instala OpenCV
  * Ejecuta en el directorio de OpenCV el siguiente comando:

      ``make && sudo make install``

## Obtener Nider

1. Clona el [repositorio](https://github.com/capgadsx/nider) desde Github

## Compilar Nider

1. Un consejo para mantener el orden en el directorio es crear dos carpetas: **build** y **data**, en *build* se almacenaran los ficheros necesarios para compilar el proyecto y en *data* se almacenaran los datos necesarios para ejecutarlo

2. Ingresa al directorio *build* y ejecuta los siguientes comandos:

      1. ``cmake ../``
        * Opcional: Si vas a usar CUDA añade al anterior comando la siguiente *flag:* **-DWITH_CUDA=ON**

      2. ``make``

3. ¡Listo!. Si se instalaron todas las dependencias correctamente no debería haber problemas al compilar el proyecto y deberías poder ver un ejecutable llamado *nider* en la carpeta *build*

## Usar Nider

*PRÓXIMAMENTE* :+1: :tada: