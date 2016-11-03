import cv2
import numpy

cv2.namedWindow("ALTA",cv2.WINDOW_NORMAL)
cv2.namedWindow("BAJA",cv2.WINDOW_NORMAL)

camara_alta = cv2.VideoCapture('data/corregido_alta.mp4')
camara_baja = cv2.VideoCapture('data/corregido_baja.mp4')

_, frame_alta = camara_alta.read()
_, frame_baja = camara_baja.read()

index_baja = 1
index_alta = 1

while(True):
    cv2.imshow("ALTA",frame_alta)
    cv2.imshow("BAJA",frame_baja)
    key = cv2.waitKey(0)
    if key & 0xFF == ord('a'):
        _, frame_alta = camara_alta.read()
        index_alta += 1
    elif key & 0xFF == ord('b'):
        _, frame_baja = camara_baja.read()
        index_baja += 1
    else:
        _, frame_alta = camara_alta.read()
        _, frame_baja = camara_baja.read()
        index_alta += 1
        index_baja += 1
    print 'ALTA',index_alta,'BAJA',index_baja
