import numpy
import os.path
from keras.models import Sequential
from keras.layers import Convolution2D, MaxPooling2D
from keras.layers import Activation, Dropout, Flatten, Dense
from keras.preprocessing.image import ImageDataGenerator
from keras.callbacks import ModelCheckpoint

width, height = 150, 150
epochs = 4
train_samples_per_epoch = 320
train_data_dir = './training_data'

print 'Creando Modelo...'
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

print 'Configurando ImageDataGenerator...'
train_datagen = ImageDataGenerator(rescale=1./255, shear_range=0.2, zoom_range=0.2, horizontal_flip=True)
train_generator = train_datagen.flow_from_directory(train_data_dir, target_size=(width, height), batch_size=32, class_mode='binary')
train_generator.class_indices['con_patente'] = 1
print 'Clases:', train_generator.class_indices

if not os.path.isfile('modelo.h5'):
    print 'Entrenando...'
    checkpoint = ModelCheckpoint("training_checkpoints/{epoch:02d}-{acc:.2f}.hdf5", monitor='acc', verbose=2, save_best_only=True, mode='max')
    model.fit_generator(train_generator, samples_per_epoch=train_samples_per_epoch, nb_epoch=epochs, callbacks=[checkpoint])
    model.save_weights('modelo.h5')

print 'Cargando Modelo..'
model.load_weights('modelo.h5')
print 'Evaluando Modelo..'
evaluation = model.evaluate_generator(train_generator, 2000, nb_worker=4, pickle_safe=True)
print 'Model loss:', evaluation[0]
print 'Model accuracy:', evaluation[1]
print 'Guardando..'
with open('modelo.json','w') as jsonfile:
    jsonfile.write(model.to_json())
model.save_weights('modelo.h5')
print 'Fin'
