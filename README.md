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

Need to test those to RTOS:<br>

    * Freertos : https://github.com/frtos-wpan/freertos-201309/tree/master/FreeRTOS/Demo/E407
    * ChibiOS : http://chibios.org/dokuwiki/doku.php?id=chibios:documentation:start
    
Chibios seems to be a better alternative than FreeRtos for severals reasons:<br>
<li>maintaining our own hal eats a lot of development time - ChibiOS comes with a hal</li>
<li>pios hal is not being widely used and tested - ChibiOS is</li>
<li>FreeRTOS relies 100% on dynamic memory allocation (heap) - ChibiOS can do static as well as dynamic memory (even mixed)</li>
<li>FreeRTOS api is not type safe (mutex / semaphore / queue) - ChibiOS api is type safe</li>
<li>i have a feeling that ChibiOS is more efficient than FreeRTOS</li>
<li>using ChibiOS hal will make us stop relying on the buggy ST libraries in the longer term</li>
