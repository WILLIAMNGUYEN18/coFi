# coFi

Authored By: William Nguyen, Zach Davis and Even Whitcomb

Utilizing NodeMCU ESP8266 and Keurig
Developing IoT keurig

Rather than using the buttons, we will have the wires connected to a circuit that will relay the same button signals (brew, power, etc) to the NodeMCU. The NodeMCU itself hosts a web server for the online interaction. We will be developing the code for the interaction and improving the website.

Even Whitcomb is designing/developing the website. William Nguyen is working on the code for the NodeMCU. Zach Davis is working on the electrical circuiting between the NodeMCU and the keurig

For the Keurig:
redirect the wiring of the buttons to the circuit/NodeMCU


For the Code:
We need to be able to connect (read?) specific pins on the NodeMCU so that we can read the following signals:

1. Power button to connect to wifi
2. Sense when water is ready (either LED connection or temperature sensor)
3. Web button press to brew
4. Menu buttons
  * left button
  * right button
  * menu button
