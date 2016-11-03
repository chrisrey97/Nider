import os
import cv2
import glob
import argparse

args = argparse.ArgumentParser()
args.add_argument('-c', '--carpeta', required=True)
args = vars(args.parse_args())

archivos = glob.glob(os.path.join(args['carpeta'],'*.jpg'))
archivos.extend(glob.glob(os.path.join(args['carpeta'],'*.png')))
print 'Detectados',len(archivos),'archivos'

counter = 0
for archivo in archivos:
    imagen = cv2.imread(archivo)
    cv2.imwrite(os.path.join(args['carpeta'], os.path.basename(archivo)[:os.path.basename(archivo).index('.')]+'.jpg'), cv2.cvtColor(imagen,cv2.COLOR_BGR2GRAY))
    counter += 1
    print '['+str(counter)+'/'+str(len(archivos))+']'
