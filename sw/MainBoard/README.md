MAIN BOARD:
====

This project is a base project for our robot. It uses ChibiOS.
We have ported the control system management of Microb Tech originally develop
for AVR platform.

We also work with module as it's easier to understand and manage for the futur.

Here we describe the mapping use for this project for OLIMEX E407 Rev.E board:

<li>Encoders LEFT uses TIM3:</li>
    <ul>PB4 AF2</ul>
    <ul>PB5 AF2</ul>

<li>Encoders RIGHT uses TIM4:</li>
<ul>PD12 AF2</ul>
<ul>PD13 AF2</ul>

<li>Motors use TIM9:</li>
<ul>LEFT on PE5 AF3</ul>
<ul>RIGHT on PE6 AF3</ul>

<li>Microshell mapped on SDU2. User functions for now:</li>
<ul>ir : get ir module estimated distances</ul>
<ul>help : display a list off all available commands</ul>

<li>Debug UART on SDU3</li>

<li>IR sensors modules mapped on:</li>
<ul>IR1: PF6</ul>
<ul>IR2: PF7</ul>
<ul>IR3: PF8</ul>

TODO:
==

  * Add module status flag to monitor if they're are initialized or not
  * Rework FatFs to create log file with all sensors values
  * Add support for inertial board
  * CAN
  * I2C
  * Sys state machine
  * Deinit system
