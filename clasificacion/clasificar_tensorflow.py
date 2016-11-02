import cv2
import numpy
import argparse
import tensorflow

def generar_ventanas(imagen, pasos, ventana_size):
    for y in xrange(0, imagen.shape[0], pasos):
        for x in xrange(0, imagen.shape[1], pasos):
            yield (x, y, imagen[y:y + ventana_size[1], x:x + ventana_size[0]])

args = argparse.ArgumentParser()
args.add_argument('-i', '--imagen', required=True)
args = vars(args.parse_args())
cv2.namedWindow("Test",cv2.WINDOW_NORMAL)
imagen = cv2.imread(args["imagen"])
window_size = (imagen.shape[0]/2,imagen.shape[1]/8)
step_px = 120

#image_data = tensorflow.gfile.FastGFile(args['imagen'],'r').read()

labels = [line.rstrip() for line in tensorflow.gfile.GFile('inception/labels.txt')]
with tensorflow.gfile.FastGFile('inception/inceptionr.pb','r') as model_file:
    graph_def = tensorflow.GraphDef()
    graph_def.ParseFromString(model_file.read())
    tensorflow.import_graph_def(graph_def,name='')

with tensorflow.Session() as sess:
    softmax_tensor = sess.graph.get_tensor_by_name('final_result:0')
    for (x, y, ventana) in generar_ventanas(imagen, step_px, window_size):
        if (ventana.shape[0] != window_size[1] or ventana.shape[1] != window_size[0]):
            continue
        cv2.imshow("Test",ventana)
        cv2.waitKey(1)
        ventana_data = numpy.array(cv2.imencode('.jpeg', ventana)[1]).tostring()
        predictions = sess.run(softmax_tensor, {'DecodeJpeg/contents:0': ventana_data})
        top_predictions = predictions[0].argsort()[-len(predictions[0]):][::-1]
        for node_id in top_predictions:
            human_string = labels[node_id]
            score = predictions[0][node_id]
            print('%s (score = %.5f)' % (human_string, score))
        print '============================='
        cv2.waitKey(0)
