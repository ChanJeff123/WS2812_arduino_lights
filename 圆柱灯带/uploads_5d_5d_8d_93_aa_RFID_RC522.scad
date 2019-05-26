module RFID_RC522() {
	color([0, 0.6, 0.8]) translate([0,0,0]) cube([60,40,2]); //pcb	
	color([0.3, 0.3, 0.3]) translate([1,10,-10]) cube([2, 20, 12]); //header
	color([0.8, 0.8, 0.8]) translate([3,35,2]) cube([10, 4, 3]); //resonator
}

RFID_RC522();