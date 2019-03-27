# BLE with the ESP32

A lot of work has been done by [Neil Kolban](https://github.com/nkolban/esp32-snippets). Other great resources are:
- https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/
- https://www.dfrobot.com/wiki/index.php/Bluetooth_APP_Control
- https://iotexpert.com/2018/10/19/goble-remote-control-wiced-bluetooth-implementation/
- https://circuitdigest.com/microcontroller-projects/esp32-ble-server-how-to-use-gatt-services-for-battery-level-indication
- https://platformio.org/lib/show/1841/ESP32%20BLE%20Arduino/examples?file=BLE_uart.ino

Content here:

- **GoBLE reciever 0.1** is the first working example, where the GoBLE app is connecting to the ESP32 and the sent data are shown in the serial console.
- **GoBLE reciever 0.2** has many lines removed that are not neccesary for the program to work.
- **GoBLE reciever 0.3** parses the recieved data (multitouch - up to 6 keys!) into an array and the two values positionX and positionY.
