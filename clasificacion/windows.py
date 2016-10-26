#-*- coding: utf-8 -*-
import cv2
import time
import numpy
import argparse

def generar_ventanas(imagen, pasos, ventana_size):
    for y in xrange(0, imagen.shape[0], pasos):
        for x in xrange(0, imagen.shape[1], pasos):
            yield (x, y, imagen[y:y + ventana_size[1], x:x + ventana_size[0]])

args = argparse.ArgumentParser()
args.add_argument("-i", "--imagen", required=True)
args = vars(args.parse_args())

cv2.namedWindow("test", cv2.WINDOW_NORMAL)
imagen = cv2.imread(args["imagen"])
window_size = (imagen.shape[0]/2,imagen.shape[1]/8)
step_px = 150

for (x, y, ventana) in generar_ventanas(imagen, step_px, window_size):
    # if (Ventana muy pequeña)
    if (ventana.shape[0] != window_size[1] or ventana.shape[1] != window_size[0]):
        continue
    draw = imagen.copy()
    cv2.rectangle(draw, (x, y), (x + window_size[0], y + window_size[1]), (0, 255, 0), 2)
    cv2.imshow("test",draw)
    cv2.waitKey(0)