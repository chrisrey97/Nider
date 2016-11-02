import cv2
import numpy
import glob
import os.path
from keras.models import Sequential
from keras.layers import Convolution2D, MaxPooling2D
from keras.layers import Activation, Dropout, Flatten, Dense

epochs = 5
batch_size = 32
width, height = 150, 150

archivos = glob.glob(os.path.join('training_data','con_patente','*.png'))
train_samples = len(archivos)
traindata_x = []
traindata_y = ([True] * train_samples)
for archivo in archivos:
    imagen = cv2.resize(cv2.imread(archivo), (width, height))
    traindata_x.append(imagen)
traindata_x = numpy.asarray(traindata_x)

model = Sequential()
model.add(Convolution2D(32, 3, 3, input_shape=(width, height, 3)))
model.add(Activation('relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Convolution2D(32, 3, 3))
model.add(Activation('relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Convolution2D(64, 3, 3))
model.add(Activation('relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Flatten())
model.add(Dense(64))
model.add(Activation('relu'))
model.add(Dropout(0.5))
model.add(Dense(1))
model.add(Activation('sigmoid'))
model.compile(loss='binary_crossentropy', optimizer='rmsprop', metrics=['accuracy'])

if not os.path.isfile('modelob.h5'):
    print 'Training...'
    model.fit(traindata_x, traindata_y, batch_size=batch_size, nb_epoch=epochs, verbose=1)
    model.save_weights('modelob.h5')
print 'Model Loaded!'
model.load_weights('modelob.h5')
print 'Evaluating..'
evaluation = model.evaluate(traindata_x, traindata_y, verbose=1)
print 'Making Predictions..'
predictions = model.predict(traindata_x, verbose=1)
rounded_preds = [round(x) for x in predictions]
print('Model loss:', evaluation[0])
print('Model accuracy:', evaluation[1])
for x in predictions:
    print 'Prediction: ',x
