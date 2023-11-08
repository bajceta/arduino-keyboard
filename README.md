arduino-cli core install arduino:avr
arduino-cli lib install Mouse
arduino-cli lib install Keyboard

arduino-cli compile --fqbn arduino:avr:micro

arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:micro 
