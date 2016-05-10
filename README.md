EUROBOT 2016 - The Beach Bots
====

TODO <br><br>

<h4>HW description:</h4>
----

Board : https://www.olimex.com/Products/ARM/ST/STM32-E407/open-source-hardware <br>
Motor Driver : https://www.olimex.com/Products/RobotParts/MotorDrivers/BB-VNH3SP30/open-source-hardware<br>
WiFi IoT module : https://www.olimex.com/Products/IoT/MOD-WIFI-ESP8266/open-source-hardware<br> 
=> Github : https://github.com/espressif/esp8266_at<br>

<h4>SW description:</h4>
----

Need to test those two RTOS:<br>

<li>Freertos : https://github.com/frtos-wpan/freertos-201309/tree/master/FreeRTOS/Demo/E407</li>
<li>ChibiOS : http://chibios.org/dokuwiki/doku.php?id=chibios:documentation:start</li>
    
Chibios seems to be a better alternative than FreeRtos for severals reasons:<br>
<li>maintaining our own hal eats a lot of development time - ChibiOS comes with a hal</li>
<li>pios hal is not being widely used and tested - ChibiOS is</li>
<li>FreeRTOS relies 100% on dynamic memory allocation (heap) - ChibiOS can do static as well as dynamic memory (even mixed)</li>
<li>FreeRTOS api is not type safe (mutex / semaphore / queue) - ChibiOS api is type safe</li>
<li>i have a feeling that ChibiOS is more efficient than FreeRTOS</li>
<li>using ChibiOS hal will make us stop relying on the buggy ST libraries in the longer term</li>

<h5>How to setup the env:</h5>

<li>Install arm gcc:</li>
<ul>brew tap PX4/homebrew-px4</ul>
<ul>brew update</ul>
<ul>brew install gcc-arm-none-eabi</ul>

<li>Install dfu-util:</li>
<ul>brew install dfu-util</ul>

<li>Make the projet:</li>
<ul>Just run make in the project folder</ul>

<li>Program the target:</li>
<ul>Put the device in dfu mode (set the jumper like explain in the manual</ul>
<ul>Run the command: dfu-util -v -d 0483:df11 -a 0 -s 0x08000000 -D prog.hex</ul>

<h4>Usefull links:</h4>
----

<li>https://www.alexwhittemore.com/using-the-stm32f4discovery-on-the-mac-os-x-10-8-4-command-line/</li>
<li>http://www.telecom-robotics.org/2013/08/robotics-101-quick-start/</li>
