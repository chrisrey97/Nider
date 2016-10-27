#-*- coding: utf-8 -*-
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

args = argparse.ArgumentParser()
args.add_argument("-c", "--carpeta", required=True)
args.add_argument("-s", "--carpeta_save", required=True)
args = vars(args.parse_args())
archivos = glob.glob(os.path.join(args["carpeta"],"*.jpg"))
print "Detectados",len(archivos),"archivos"

for archivo in archivos:
    imagen = cv2.imread(archivo)
    window_size = (imagen.shape[0]/2,imagen.shape[1]/8)
    step_px = 150
    for (x, y, ventana) in generar_ventanas(imagen, step_px, window_size):
        # if (Ventana muy pequeña)
        if (ventana.shape[0] != window_size[1] or ventana.shape[1] != window_size[0]):
            continue
        nombre_archivo_nuevo = os.path.basename(archivo)[:os.path.basename(archivo).index('.')]+str(x)+str(y)
        cv2.imwrite(os.path.join(args["carpeta_save"],nombre_archivo_nuevo+".jpg"),ventana)
