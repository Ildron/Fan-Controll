# Fan-Controll
Upgrade of an old Heat recovery unit (Enervent TS 200 ) to be controlled by an Arduino one micro controller,
(controls 2 fans ,3 speed's, with solid state relays(SSR), controlling fan on/off , and "damper"

the "damper" are turned every 55 sec(changing input -> output & output -> input air )   , 
The fans are turned of a few sec before each turn to avoid a "breathe sound" in the air-vent 
(aprox 0.5 milion turns/year, SSR will handle this)

inputs/ modes

"fireplace" mode : 
meaning it runs the in-fan on one higher speed than the out fan
to create overpressure / adding air into the house for compensating for the lost air going out of the chimney (when 
fire in the fireplace)

"summer" mode  : dont turn the "damper"


The resaon for choosing to use 2 x solid states relays in serie for turning on/off fans :

precations when using SSR & Capacitive Load:
The supply voltage plus the charge voltage of the capacitor is
applied to both ends of the SSR when it is OFF. Therefore,
use an SSR model with an input voltage rating twice the size
of the supply voltage. Limit the charge current of the capacitor
to less than half the peak inrush current value allowed for the
SSR.

 

schematic 

![image](https://user-images.githubusercontent.com/71104893/164334567-bbfb75ec-faa1-4306-ac89-d071d321af8c.png)

