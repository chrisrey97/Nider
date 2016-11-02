import cv2
import numpy
import keras
import argparse
from keras.models import model_from_json

def generar_ventanas(imagen, pasos, ventana_size):
    for y in xrange(0, imagen.shape[0], pasos):
        for x in xrange(0, imagen.shape[1], pasos):
            yield (x, y, imagen[y:y + ventana_size[1], x:x + ventana_size[0]])

args = argparse.ArgumentParser()
args.add_argument("-m", "--modelo", required=True)
args.add_argument("-w", "--weights", required=True)
args.add_argument("-i", "--imagen", required=True)
args = vars(args.parse_args())

cv2.namedWindow("Test",cv2.WINDOW_NORMAL)
imagen = cv2.imread(args["imagen"])
window_size = (imagen.shape[0]/2,imagen.shape[1]/8)
step_px = 120

print 'Cargando modelo'
with open(args["modelo"],'r') as jsonf:
    modelo = keras.models.model_from_json(jsonf.read())
modelo.load_weights(args["weights"])
print 'Modelo Listo'

for (x, y, ventana) in generar_ventanas(imagen, step_px, window_size):
    if (ventana.shape[0] != window_size[1] or ventana.shape[1] != window_size[0]):
        continue
    ventana = cv2.resize(ventana,(150,150))
    predicciones = modelo.predict(numpy.asarray([ventana]),verbose=2)
    print round(predicciones[0])
    cv2.imshow("Test",ventana)
    cv2.waitKey(0)
