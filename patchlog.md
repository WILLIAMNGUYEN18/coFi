
June 10th
For the Keurig:
redirect the wiring of the buttons to the circuit/NodeMCU


For the Code:

We are successfully compiling. We haven't uploaded code yet. We need to figure out how to interact with pins to read signals and send signals. We need to figure out a method of transferring webite code to the NodeMCU code in the arduino IDE


Following signals that need to be read.
1. Power button to connect to wifi
2. Sense when water is ready (either LED connection or temperature sensor)
3. Web button press to brew
4. Menu buttons
  * left button
  * right button
  * menu button


7/21/18 Update Log:
Zach will describe how the keurig I/O should work so that we can code around it. Need to assign outputs to gpio pin. Any GPIO pin will work. Input will also be a gpio pin. ()
Inputs:
  NECESSARY
  * Brewing Light

Output:
  NECESSARY:
  * Brew Button

  ADDITIONAL:
  * Cup Button (Size of drinks, left and right)
  * On/Off Switch

Digital .write command and the corresponding pin number

Completed the brewing button output (paralleling the structure for LEDON and LEDOFF)
Can do similar structuring for any output (left and right, and on/off)

Will need to learn structure for brewing light.
in main loop, throw in a check for if the brewing light digital pin is high.
if brew light is on, water is ready to brew (is boiled). It means you can go through with brewing.
So we will need to do a check for if brew light is true, and brew button will have to give user feedback on brew light being off, until it is on.

7/28/18 Update
Keurig changed to smaller model
* has built in cup detector
* will need to add another if statement regarding if cup is present (along with if the water is finished heating up)


8/11/18
Using external HTML file rather than string
* Will is working on successfully reading an additional html file into a string to be used rather than page
  * Will figured out the issues with the original code and successfully uploaded it.
  * Starting to load the html onto the SPIFFS
  * Need to read the html from SPIFFS
* Even is working on creating an HTML file suitable to our needs
  * Added CSS changes for UI (Attempting to effectively accommodate for mobile usage as well)
* Zach is working on decomposing the newer (smaller) keurig for use
  * Zach is testing the signals on the smaller Keurig
  * Zach noticed the light that detects when the Keurig slot has a cup inside (cup detector) is flashing
    * this could lead to failing to sample (sampling the cup detector when it's off)
    * Contemplating converting the cup detector to a solid signal OR increasing the sample rate to successfully detect the flashing high signal.

8/27/18
Website showing on read index.txt (uploaded to SPIFFs). Corresponding pins found for button presses

* Even starting to create a separate css page
  * exploring javascript alarms
* Zach has reassembled the keurig with wires (that will attach to NodeMCU and relay sensor data) neatly siphoned out of the keurig.
  * Now identifying different wires and pins
* Will has successfully uploaded html to SPIFFs and reads it to a string for website usage.
  * Resolved some errors regarding format of html

9/9/18
* Will
  * Adjust to have the exact buttons we want ()
  * Create checks to ensure that: there is cup (blinking light detection)
* Zach
  * has effectively labeled and setup the hardware. 
  * He can connect the keurig to the NodeMCU at any point
* Even
  * Setting up alarm clock/timer to be used for brewing

  9/22/18
Zach
*  to simulate the lid switch, a transistor was swapped out for an octo-coupler, for stability(?).
*  May potentially need some isolation circuitry. light pin should be an input, however it seems to be supplying power to the brew light button.
Will
*  Reduced delay times

If you remove D4 (the brew light pin), then it works correctly as the brew light is off (power is off by default)
However, if D4 is connected on startup, the brew light is already on (suggesting power being supplied?) which results in unsuccessful boot order
D4 is leading to difficulties with the site. Potentially may remove light pin in future iteration

Found out that the power button was not plugged in. Also the keurig turns on automatically once lid is cycled.
On/Off now functioning. specific case for power already being on/off (based off on/off being pressed rather than actual state currently)

