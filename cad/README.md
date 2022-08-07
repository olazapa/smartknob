# CAD

The attached .f3z archive contains Fusion 360 project files for SmartKnob adjusted for GM2804H-100T motor. Please note that the introduced changes are in form of a quick and dirty proof-of-concept rather than a proper redesign so expect some issues and gross shortcuts (e.g. ignoring any parametrerised design principles and not using properly defined dimentions).

Main differences compared to original design:
- Modified dimentions of mount base, rotor spacer and knob assembly to fit larger GM2804H-100T motor
- Mount base now also serves a purpose of an adapter to fix GM2804H-100T to the PCB (no need to modify the original PCB design to fit new motor). On the orignal PCB design there are screw holes for the orignal motor, which are not compatible with screw holes for GM2804H-100T. The modified mount base is still mounted to PCB with original screw holes but now also contains new screw holes for GM2804H-100T - the motor now needs to be mounted to mount base first, before mounting the base to PCB.
- As GM2804H-100T does not come with a magnet, it requires a separately sourced magnet to be glued to the motor shaft. The modified mount base has space for fitting a 10x5x5 diametric magnet.
- Mount base column for mounting screen platform had to be reduced in diameter to fit through a narrower opening in the separately sourced magnet (5mm hole compared to 5.9mm in the original motor). This makes the column a bit weaker and prone to breaking when too much force is applied when attaching screen platform.

NOTE: This design is NOT ready for production using 3d printing services as it hasn't been thouroughly tested, individual tweaks may still be required.

## Known issues
- I have experienced issues with the precision of magnetic encoder - the angle of motor shaft was incorrectly measured (angle changing inconsitently with relation to actual rotation) on a diagonal in very specific place. It may have been caused either by too strong magnetic field from the 5mm thick neodymium magnet, mount base not having enough space for magnetic sensor causing some bending or some interference from display cables. The solution was to raise the whole mount base by 1-2 mm with some washers between mount base and PCB. The uploaded design does NOT account for that issue.
- The tolerances are pretty tight for a FDM printer, especially in areas where mount base meets screen platform. I had to remove the buldge at the top of mount base column to be able to attach the screen platform. Again, the uploaded design does NOT account for that issue.

## Online View
 - [Complete SmartKnob View assembly](https://a360.co/3Szahmc)
