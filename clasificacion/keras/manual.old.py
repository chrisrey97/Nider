import os
import cv2
import glob
import time
import numpy
import argparse

def generar_ventanas(imagen, pasos, ventana_size):
    for y in xrange(0, imagen.shape[0], pasos):
        for x in xrange(0, imagen.shape[1], pasos):
            yield (x, y, imagen[y:y + ventana_size[1], x:x + ventana_size[0]])

def mouse_callback(event,x,y,flags,param):
    global puntos_patente, imagen
    if len(puntos_patente) == 2:
        return
    if event == cv2.EVENT_LBUTTONDOWN:
        cv2.line(imagen , (0, y),(x, y), (0, 0, 255), 5)
        cv2.line(imagen , (x, y),(imagen.shape[0], y), (0, 0, 255), 5)
        cv2.line(imagen , (x, y),(x, imagen.shape[1]), (0, 0, 255), 5)
        cv2.line(imagen , (x, 0),(x, y), (0, 0, 255), 5)
        puntos_patente.append((x, y))

args = argparse.ArgumentParser()
args.add_argument("-c", "--carpeta", required=True)
args.add_argument("-s", "--carpeta_save", required=True)
args = vars(args.parse_args())
imagen = None
puntos_patente = []
cv2.namedWindow("Test",cv2.WINDOW_NORMAL)
cv2.setMouseCallback("Test",mouse_callback)
archivos = glob.glob(os.path.join(args["carpeta"],"*.jpg"))
print "Detectados",len(archivos),"archivos"

for archivo in archivos:
    print archivo
    imagen = cv2.imread(archivo)
    imagenw = imagen.copy()
    window_size = (imagen.shape[0]/2,imagen.shape[1]/8)
    step_px = 120
    puntos_patente = []
    while len(puntos_patente) < 2:
        cv2.imshow("Test",imagen)
        cv2.waitKey(1)
    counter = 1
    c_total = sum(1 for i in generar_ventanas(imagenw, step_px, window_size))
    for (x, y, ventana) in generar_ventanas(imagenw, step_px, window_size):
        counter += 1
        if (ventana.shape[0] != window_size[1] or ventana.shape[1] != window_size[0]):
            continue
        if (x <= puntos_patente[0][0] and puntos_patente[0][0] <= x+ventana.shape[1]) and (y <= puntos_patente[0][1] and puntos_patente[0][1] <= y+ventana.shape[0]) and (x <= puntos_patente[1][0] and puntos_patente[1][0] <= x+ventana.shape[1]) and (y <= puntos_patente[1][1] and puntos_patente[1][1] <= y+ventana.shape[0]):
            nombre_archivo_nuevo = os.path.basename(archivo)[:os.path.basename(archivo).index('.')]+str(x)+str(y)
            cv2.imwrite(os.path.join(args["carpeta_save"],"con_patente",nombre_archivo_nuevo+".jpg"),ventana)
        else:
            nombre_archivo_nuevo = os.path.basename(archivo)[:os.path.basename(archivo).index('.')]+str(x)+str(y)
            cv2.imwrite(os.path.join(args["carpeta_save"],"sin_patente",nombre_archivo_nuevo+".jpg"),ventana)
        copia_im = imagen.copy()
        cv2.putText(copia_im,str(counter)+"/"+str(c_total), (imagen.shape[0]/2,imagen.shape[1]/2), cv2.FONT_HERSHEY_SIMPLEX, 4, 255, 4)
        cv2.imshow("Test",copia_im)
        cv2.waitKey(1)
    os.rename(archivo, os.path.join(args["carpeta"],"procesados",os.path.basename(archivo)))
