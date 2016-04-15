# IOTBasedGarbageMonitoringSystem

Aim:
To monitor the garbage bins eecively using Internt of Things.

Components Required:
 Intel Galileo Board / Arduino Micro-Controller: 01
 Ultrasonic sensors: 04
 LCD display: 01
 Lan Cable / WiFi Adapter: 01
 Buzzer: 01

Description:
The garbage bins are monitored using ultrasonic sensors. The sensors are placed
on the top of every bin (one sensor per bin). Each time during system's initial-
ization, the height of the empty garbage bins are estimated and stored. These
empty garbage bin heights are displayed on LCD after performing each one of
its intialisation steps. As the garbage gets lled into the bins, the new empty
heights of bins are estimated depending on height occupied by the garbage in
the respective bins. Each of these values from all the four bins are updated
into the cloud (through LAN or WiFi), in order to keep track on garbage bin
usage at dierent places. Whenever any bin completely gets lled with garbage,
the buzzer wakes up informing the municipality about the lled garbage bin.
when the buzzer produces the sound, the respective identication number of the
garbage bin that got lled is displayed on the LCD.


Caution:
I have assigned a static IP address to the Galielo Gen2 board in order to use LAN for sending the data to the cloud. If anyone wants to go for DHCP, the code part (where I used the Ethernet SHiels Functions) have to changed accordingly
