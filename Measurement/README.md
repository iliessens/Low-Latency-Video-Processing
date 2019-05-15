<style>
li {margin-bottom: 5px;}
</style>

# Measurement  
This folder contains the source code for the measurement solution.  
The subfolders are as follows:
* Arduino: contains the main Arduino program. The code is written for Arduino Uno, but should work on other models with minimal adjustments.
The comparator library from http://www.leonardomiliani.com/en/2012/analogcomp-una-libreria-per-gestire-il-comparatore-analogico/ needs to be installed first.
* ComparatorVerify: small test script that measures the period of a signal applied to the comparator. Not that useful.
* DelayVerify: Toggles an output pin when the input causes the comparator to trigger. Using a function generator and an oscilloscope, this allows us measure the delay caused by the comparator and ``micros()`` function. As an extra the [DigitalWriteFast](https://github.com/NicksonYap/digitalWriteFast) library may be used.
* Schema.sch: is an Eagle schematic for the full measurement solution as it was built.

## Matlab
Contains the scripts to connect Matlab to the Arduino measurement solution from the Arduino folder.  
``MeasureLatency`` measures once, ``MultiMeasure`` performs multiple measurements and calculates mean and standard deviation. 

``SetReference`` sends a command to apply a specific reference offset. This should normally not be necesssary.

``ConnectArduino`` and  ``CloseArduino`` provide setup and teardown of the connection.

Lastly, ``SaveResult`` writes a measurement array to a .csv file outside of the GIT repository.
