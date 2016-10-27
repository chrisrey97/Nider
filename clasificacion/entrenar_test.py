from keras.preprocessing.image import ImageDataGenerator
from keras.models import Sequential
from keras.layers import Convolution2D, MaxPooling2D
from keras.layers import Activation, Dropout, Flatten, Dense
from keras.callbacks import ModelCheckpoint

width, height = 150, 150
train_data_dir = "./train_data"
epochs = 50
train_samples = 3200

model = Sequential()
model.add(Convolution2D(32, 3, 3, input_shape=(3, width, height)))
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

train_datagen = ImageDataGenerator(rescale=1./255, shear_range=0.2, zoom_range=0.2, horizontal_flip=True)
train_generator = train_datagen.flow_from_directory(train_data_dir, target_size=(width, height), batch_size=32, class_mode='binary')
checkpoint = ModelCheckpoint("weights-improvement-{epoch:02d}-{acc:.2f}.hdf5", monitor='acc', verbose=2, save_best_only=True, mode='max')
model.fit_generator(train_generator, samples_per_epoch=train_samples, nb_epoch=epochs, callbacks=[checkpoint])

model.save_weights('./first_try.weights.h5')
model.save('./first_try.model.h5')
