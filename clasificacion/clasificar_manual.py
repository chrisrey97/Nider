import os
import cv2
import glob
import argparse

args = argparse.ArgumentParser()
args.add_argument("-c", "--carpeta", required=True)
args = vars(args.parse_args())
archivos = glob.glob(os.path.join(args["carpeta"],"*.jpg"))
print "Detectados",len(archivos),"archivos"
cv2.namedWindow("Clasificar Manual",cv2.WINDOW_NORMAL)

for archivo in archivos:
    imagen = cv2.imread(archivo)
    cv2.imshow("Clasificar Manual",imagen)
    tecla = cv2.waitKey(0)
    if tecla == 65362: #Flecha Arriba
        print os.path.join(args["carpeta"],"con_patente",os.path.basename(archivo))
        os.rename(archivo,os.path.join(args["carpeta"],"con_patente",os.path.basename(archivo)))
    elif tecla == 65364: #Flecha Abajo
        print os.path.join(args["carpeta"],"sin_patente",os.path.basename(archivo))
        os.rename(archivo,os.path.join(args["carpeta"],"sin_patente",os.path.basename(archivo)))
    elif tecla == 27: #Escape
        break
