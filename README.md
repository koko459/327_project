# 327_project
Notes:

Project Deriverables: 
For this assignment we were tasked with creating a project using the MSP430. Thus, we decided to create a fire detection system that would alert one in case of a fire.

Features:
-A fire sensor.
-An LCD Display that reacts depending on the current temperature.
-A buzzer that alerts one in case of high temperatures.
Additional feature (Not fully included):
-A GPS tracking device to notify one on the current location of the fire.


Software features & implementation:
  The system is temperature dependant, thus, at normal temperatures (below 60-50 C), the message "Temperature outside is at normal levels! No need to panic." Will display on the terminal as well as the LCD Display. Once the fire sensor is exposed to high enough temperatures (50-60 C) it will then display an alert message to the terminal and a message will also display on the LCD panel. This will also set off the buzzer alarm indicating that the current temperatures are above normal and there is risk for a fire. Other features were added besides fire detection, such as tracking lower temperatures around freezing (Below 5 C). However, this will only display a warning message to the LCD as there is no need to alarm the user. There are also other pre-scripted messages that are also temperature dependent.  
  
  Additionally, we initially had hoped to implement a SIM Card device that would alert the user in case of a fire through text by providing the alert as well as specific coordinates for the location. However, due to hardware challenges beyond our control, we switched the approach to using an email alert system instead. We implemented python code to read the serial terminal and send a pre-scripted email in case of a fire. The GPS and fire sensor both worked individually, however we determined that the GPS coordinates were not accurate and decided to no longer include this feature in the overall system. However, we are attaching the code to use the GPS module individually with the MSP430.  


Diagrams:
This includes how we had originally planned to implement the GPS data using a leader-follower system between two MSP430s to share the data. 
<img width="622" alt="Screenshot 2024-04-26 at 9 28 48 PM" src="https://github.com/koko459/327_project/assets/70616464/5138f2cb-dd77-4bfb-9e53-71a92162ae43">

<img width="630" alt="Screenshot 2024-04-26 at 10 22 01 PM" src="https://github.com/koko459/327_project/assets/70616464/3568d02b-8b98-4f49-b86c-45f0e0150c25">


  
  Below is a link to our demo video as well as a demo of what we expected the final product to be.
  Link: https://rice.box.com/s/lwhm5ou6oyjsq2amn85zxqehftekbmmu

Acknowledgements:
The code for I2C and LCD were originally from Andrei-cb, which was then modified and provided to us by Dr. Joe Young. We then modified the code we got from Dr. Joe Young to fit our needs. 
Additionally, a lot of research was done to complete this project, and we referenced a wide variety of sources, such as Stackoverflow forums, Youtube videos, Reddit threads, and several documentations (MSP430 User Guide and etc.), in order to enhance our understanding and help with debugging. 
