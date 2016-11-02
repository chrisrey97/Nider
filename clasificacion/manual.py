import os
import cv2
import glob
import numpy
import argparse

def generar_ventanas(imagen, pasos, ventana_size):
    for y in xrange(0, imagen.shape[0], pasos):
        for x in xrange(0, imagen.shape[1], pasos):
            yield (x, y, imagen[y:y + ventana_size[1], x:x + ventana_size[0]])

def mouse_callback(event,x,y,flags,param):
    global puntos_patente, imagen_completa
    if len(puntos_patente) == 4:
        return
    if event == cv2.EVENT_LBUTTONDOWN:
        cv2.line(imagen_completa , (0, y),(x, y), (0, 0, 255), 5)
        cv2.line(imagen_completa , (x, y),(imagen_completa.shape[0], y), (0, 0, 255), 5)
        cv2.line(imagen_completa , (x, y),(x, imagen_completa.shape[1]), (0, 0, 255), 5)
        cv2.line(imagen_completa , (x, 0),(x, y), (0, 0, 255), 5)
        puntos_patente.append((x, y))

def intersectan(x, y, a, b):
    if (x <= b[0][0] and b[0][0] <= a.shape[1]+x) and (y <= b[0][1] and b[0][1] <= a.shape[0]+y):
        if (x <= b[1][0] and b[1][0] <= a.shape[1]+x) and (y <= b[1][1] and b[1][1] <= a.shape[0]+y):
            if (x <= b[2][0] and b[2][0] <= a.shape[1]+x) and (y <= b[2][1] and b[2][1] <= a.shape[0]+y):
                if (x <= b[3][0] and b[3][0] <= a.shape[1]+x) and (y <= b[3][1] and b[3][1] <= a.shape[0]+y):
                    return True
    return False

args = argparse.ArgumentParser()
args.add_argument('-c', '--carpeta', required=True)
args = vars(args.parse_args())
cv2.namedWindow('Test',cv2.WINDOW_NORMAL)
cv2.setMouseCallback('Test',mouse_callback)
archivos = glob.glob(os.path.join(args['carpeta'],'*.jpg'))
print 'Detectados',len(archivos),'archivos'
network_size = (300,300)
imagen_completa = None
puntos_patente = []

for archivo in archivos:
    imagen_completa = cv2.imread(archivo)
    imagen_completaw = imagen_completa.copy()
    window_size = (imagen_completa.shape[0]/2,imagen_completa.shape[1]/8)
    step_px = 50
    puntos_patente = []
    while len(puntos_patente) < 4:
        cv2.imshow('Test',imagen_completa)
        cv2.waitKey(1)
    for (x, y, ventana) in generar_ventanas(imagen_completaw, step_px, window_size):
        if (ventana.shape[0] != window_size[1] or ventana.shape[1] != window_size[0]):
            continue
        if intersectan(x, y, ventana, puntos_patente):
            ventana = cv2.cvtColor(ventana,cv2.COLOR_BGR2GRAY)
            ventana = cv2.GaussianBlur(ventana,(5,5),0)
            #_, ventana  = cv2.threshold(ventana, 80, 255, cv2.THRESH_BINARY)
            nombre_archivo_nuevo = os.path.basename(archivo)[:os.path.basename(archivo).index('.')]+str(x)+str(y)
            #cv2.imwrite(os.path.join('con_patente',nombre_archivo_nuevo+'.jpg'),ventana)
        else:
            ventana = cv2.cvtColor(ventana,cv2.COLOR_BGR2GRAY)
            ventana = cv2.GaussianBlur(ventana,(5,5),0)
            #_, ventana  = cv2.threshold(ventana, 80, 255, cv2.THRESH_BINARY)
            nombre_archivo_nuevo = os.path.basename(archivo)[:os.path.basename(archivo).index('.')]+str(x)+str(y)
            cv2.imwrite(os.path.join('sin_patente',nombre_archivo_nuevo+'.jpg'),ventana)
