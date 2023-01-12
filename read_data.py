from everywhereml.data import Dataset
from everywhereml.data.collect import SerialCollector
from everywhereml.preprocessing import Pipeline, MinMaxScaler, Window, SpectralFeatures
from pprint import pprint


from everywhereml.sklearn.ensemble import RandomForestClassifier



try:
    imu_dataset = Dataset.from_csv(
        'imu.csv', 
        name='ContinuousMotion', 
        #target_name_column='target_name'
    )
    
except FileNotFoundError:
    imu_collector = SerialCollector(
        port='/dev/ttyUSB3', 
        baud=115200, 
        start_of_frame='IMU:', 
        feature_names=['ax', 'ay', 'az', 'gx', 'gy', 'gz']
    )
    imu_dataset = imu_collector.collect_many_classes(
        dataset_name='ContinuousMotion', 
        duration=30
    )
    
    # save dataset to file for later use
    imu_dataset.df.to_csv('imu.csv', index=False)

imu_dataset.plot.features_pairplot(n=300)

sampling_frequency = 104
mean_gesture_duration_in_millis = 1000
window_length = sampling_frequency * mean_gesture_duration_in_millis // 1000

imu_pipeline = Pipeline(name='ContinousMotionPipeline', steps=[
    MinMaxScaler(),
    # shift can be an integer (number of samples) or a float (percent)
    Window(length=window_length, shift=0.3),
    # order can either be 1 (first-order features) or 2 (add second-order features)
    SpectralFeatures(order=2)
])
    
imu_dataset.apply(imu_pipeline)
imu_classifier = RandomForestClassifier(n_estimators=20, max_depth=20)
imu_train, imu_test = imu_dataset.split(test_size=0.3)
imu_classifier.fit(imu_train)


imu_classifier = RandomForestClassifier(n_estimators=20, max_depth=20)
imu_train, imu_test = imu_dataset.split(test_size=0.3)
imu_classifier.fit(imu_train)
print('Score on test set: %.2f' % imu_classifier.score(imu_test))

#print(imu_pipeline.to_arduino_file(
#    'sketches/IMUClassify/Pipeline.h', 
#    instance_name='pipeline'
#))

print(imu_classifier.to_arduino_file(
    'sketches/IMUClassify/Classifier.h', 
    instance_name='forest', 
    class_map=imu_dataset.class_map
))



