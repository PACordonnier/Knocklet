# Abstract PFE 1619

Domotic is more and more present in our homes. It is easy and cheap to getsmart lights, to monitor our own electrical or heating consumption using one's own smartphone or to close all one's curtains simply by pressing a button on a remote.

However, improvements need to be made, studies have shown that 70% of domotic devices are considered stressful. User experience does not satisfy the users and 50% of them wish to have a better solution to pilot their houses.

Indeed, in order to switch on the lights, it is often easier to get up from the couch and press the switch rather than pick up your phone, unlock it, launch the app and press the right button. When using a remote, the major issue is not to loose it between two pillows.

Currently, several actors exist in the domotic marketplace :
- Devices manufacturers (Philips Hue, Z-Wave Alliance ...)
* Controllers (Jeedom, Somfy, Fibaro)
* Vocal actionners (Amazon Echo Google Home)
* Physical actionners

One just need to look at the Kickstarter homepage or the  start-up presenting at the CES to discover new ways to interact with your home devices.

Knocklet, our project, is a connected wristband which can be located in a smarthouse. It reacts to any tap against a hard surface. It connects to a domotic box and therefore can control any lamps, thermostats, power outlets and even smartphones !

Its principle is simple. Once worn, the wristband detects any knock against a wall, the closest locating module will then get the information and transmit it to the domotic box that will act according to the user's configuration.

The actionner presents many benefits for the users :

* Connected devices no longer need remotes to be controlled.
* Actions can be changed from one room to another.
* Each action from each wristband is customizable. Everyone having a wristband can have his own controls.
* Global commands for the whole house are possible. For example it is possible to turn off all the lights from anywhere in the house.

The numbers of Knocks for each action can be configured on a web interface using the Knocklet plugin of the domotic box. Existing scenarios can be associated to actions for each room and each wristband of the Knocklet system.

The wristband and the module are communicating through Bluetooth. The received signal strength (RSSI) integrated within the protocol allows the wristband to be located regarding the position of the module and therefore the room it is in. The wristband uses Bluetooth Low Energy technology that allows a tiny power consumption and an improved battery life. Using WiFi the module then sends the information to the domotic box.
