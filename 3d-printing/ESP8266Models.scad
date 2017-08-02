// ESP8266 based modules models library
//
// V1.1 - 2016-02-20 - OpenSCAD version 2016.10.04 (git 7e0935d)
// DonJuanito
//
// Added holes localisation functions:
//      WemosD1M_USBLocate()
//      WemosD1M_PBLocate()
//      NodeMCU_USBLocate()
//      NodeMCU_PBLocate()
//
// Included Modules :
//  ESP-01
//  ESP-02 (V3)
//  ESP-03
//  ESP-04
//  ESP-05
//  ESP-06
//  ESP-07
//  ESP-08
//  ESP-09
//  ESP-10
//  ESP-11
//  ESP-12F (ESP-12E has the same layout, apart from the antenna path)
//  Witty (pins type selectable)
//  Wemos D1 Mini (pins type selectable)
//  NodeMCU V1.0 (Amica version)
//
// Included Components:
//  LED 5050
//  LDR
//  Male and Female Pinhead Header
//  Pinhead Headers Holes
//  90° SMD push Button
//  Micro Usb Female PCB Connector
//  QFN-32 package
//  WSON package
//  DFN package
//  SOIC-8 (154/208mils) package
//  uFl antenna connector

// Todo:
//  Components:
//      ESP-13
//      ESP-201
//      ESP-210

$fn=36*2;

// --- Defines ----------------------------------

ESP12ANT = 0;
ESP1ANT  = 1;

// --- Utilities --------------------------------

/*
 * Used to make rounded edges on objects
 * This 'edge' function is from http://www.thingiverse.com/thing:393890
 */
module edge(radius, height)
{
    difference()
	{
		translate([radius/2-0.5, radius/2-0.5, 0])
			cube([radius+1, radius+1, height], center = true);

		translate([radius, radius, 0])
			cylinder(h = height+1, r1 = radius, r2 = radius, center = true, $fn = 100);
	}
}

// Adds the coordinates in a vector table of type [ [X,Y] ] and returns the result
// base: 0=X, 1=Y
// lim: Index of the maximum row to add (0 means return the first row result)
function addCoord(v, base=0, lim=0, i=0, r=0) = i<=lim ? 
    addCoord(v, base, lim, i+1, r + v[i][base]) : 
    r;

module DrawPath(path=[], w=0.8, h=0.2) {
    // Draw a continuous 'PCB trace' by following a series of coordinate 'moves'
    // path: [ [X new pos, Y new pos], ... ]
    // w: width of the trace
    // h: height of the 'copper'

    for (i=[0:1:len(path)-1]) {
        translate([ addCoord(v=path, base=0, lim=i-1), 
                    addCoord(v=path, base=1, lim=i-1),
                    h/2]) {
            translate([ path[i][0] ? path[i][0]/2 : 0,
                        path[i][1] ? path[i][1]/2 : 0,
                        0 ])
                cube([  path[i][0] ? abs(path[i][0])+w : w,
                        path[i][1] ? abs(path[i][1])+w : w,
                        h ], center=true);
                        }
    }
}

// --- Discrete Components ----------------------
module LED5050() {
    // LED 5050
    // Rendered as centered on origin, laying over the XY plane

    translate([0,0,0]) {
        difference() {
            union() {
                // +++
                color("Ivory")
                translate([0,0,1.4/2+0.1])
                    cube([5.0, 5.0, 1.4], center=true);
                color("Silver")
                translate([0,0,0])
                    for (i=[-1:2:1])
                        for (j=[-1:1:1])
                            translate([i*((5.0-1.4+0.1)/2), j*1.5, 0.5])
                                cube([1.4, 1.0, 1.0], center=true);
            }
            color("Snow", 0.1)
            translate([0, 0, 1.5-0.75/2+0.001])
                cylinder(d=4.0, h=0.75, center=true);
        }
    }
}

module LDR( LD=5.0, LH=2.2 ) {
    // LDR (small)
    // Rendered as centered on origin, pins on Y axis, laying over the XY plane
    
    CO = LD/10;
    
    translate([0,0,LH/2])
        difference() {
            union() {
                color("SaddleBrown")
                cylinder(d=LD, h=LH, center=true);
            }
            for (i=[-1:2:1])
                translate([i*(LD+5.0-CO)/2, 0, 0])
                    cube([5.0, LD+0.2, LH+CO], center=true);
        }
}

module PINHEADM(n=2, col="DarkGrey") {
    // Pinhead male
    // n is the pins number
    // Rendered as pin '1', pointing towards Z+, base on axis origin towards Y+, 

    PH = 2.5+2.4+6.0;

    for (i=[0:1:n-1])
        translate([0, i*2.54, PH/2-2.5]) {
            // Pins
            color("gold")
            difference() {
                rotate([0,0,45])
                cylinder(d=0.64, h=PH, center=true, $fn=4);
                for (i=[1:-2:-1])
                for (j=[0:1:3])
                    rotate([i*180, 0, j*90])
                    translate([0.64/2+0.35,0, i*PH/2])
                    rotate([0, i*60, 0])
                        cube([1,1,1], center=true);
            }
            // Bases
            color(col)
            translate([0, 0, (2.4-PH)/2+2.5])
                difference() {
                    cube([2.54, 2.54, 2.4], center=true);
                    for (i=[0:1:3])
                        rotate([0, 0, i*90])
                        translate([(2.54+0.75)/2, (2.54+0.75)/2, 0])
                        rotate([0, 0, 45])
                            cube([2,2,3], center=true);
                }
    }
}

module PINHEADF(n=2, BH = 8.5, col="DarkGrey") {
    // Pinhead female
    // n is the pins number. BH is the plastic housing height (8.5:common, 8.0:rare)
    // Rendered as pin '1', pointing towards Z+, base on axis origin towards Y+, 

    PH = 3.3;

    for (i=[0:1:n-1])
        translate([0, i*2.54, -PH/2]) {
            // Pins
            color("gold")
            difference() {
                rotate([0,0,45])
                cylinder(d=0.64, h=PH, center=true, $fn=4);
                for (i=[0:1:3])
                    rotate([180, 0, i*90])
                    translate([0.64/2+0.35,0, PH/2])
                    rotate([0, 60, 0])
                        cube([1,1,1], center=true);
            }
            // Bases
            color(col)
            translate([0, 0, (BH+PH)/2])
                difference() {
                    cube([2.54, 2.54, BH], center=true);
                    translate([0,0,0.2])
                        cube([0.8, 0.8, BH], center=true);
                    translate([0,0, BH/2])
                    difference() {
                        cube([2,2,2], center=true);
                        for (i=[0:1:3])
                            rotate([0, 0, i*90])
                            translate([0,-1*sqrt(2),-1])
                            rotate([45, 0, 0])
                                cube([4,2,2], center=true);
                    }
                }
    }
}


module PINHEADHOLES(n=1, pitch=2.54, hd=0.8, hh=4.0) {
    // Pinhead male
    // n is the pins number, hd=hole diameter, hh=hole 'tube' height
    // Rendered as pin '1', pointing toward Z+, base on axis origin towards Y+, 

    for (i=[0:1:n-1])
        translate([0, i*pitch, 0]) {
            cylinder(d=hd, h=hh, center=true);
    }
}

module PUSHBS(XDim = 6.8, YDim = 2.2, ZDim = 3.5, BXDim = 3.0, BYDim = 1.0, BZDim = 1.5) {
    // Small 90° SMD push button
    // Rendered as centered on X, button base on axis origins, laying over the XY plane
    
    translate([0, -YDim/2, ZDim/2]) {
        color("Ivory")
        cube([XDim, YDim, ZDim], center=true);
        color("Black")
        translate([0, (YDim+BYDim)/2, 0])
            cube([BXDim, BYDim, BZDim], center=true);
    }
}

module mUSBF() {
    // Typical micro USB female PCB connector
    // Rendered as centered on X, connector mouth on axis origins and pointing towards Y+, laying over the XY plane

    mUWAng = 25;
    mUF = [
            [-2.70, 0.00],
            [-3.45, 0.75],
            [-3.45, 1.85],
            [3.45,  1.85],
            [3.45,  0.75],
            [2.70,  0.00],
          ];

    translate([0, 0, 0.75+0.25]) {
        color("Silver")
        difference() {
            union() {
                // Connector body
                translate([0, -5.0/2, 0])
                rotate([90, 0, 0])
                    linear_extrude(height = 5.0, center=true, convexity = 6)
                        translate([0, -0.75, 0])
                            offset(r=0.25)
                                polygon(points = mUF, convexity = 6);
                // Bottom wing
                translate([0, 0, -(0.75+0.25/2)])
                rotate([-mUWAng,0,0])
                translate([0, -2.0/2+0.63, 0])
                    cube([5.40, 2.0, 0.25], center=true);
                // Top wing
                translate([0, 0, (1.85-0.75+0.25/2)])
                rotate([mUWAng,0,0])
                translate([0, -2.0/2+0.63, 0])
                    cube([6.70, 2.0, 0.25], center=true);
                // Left wing
                translate([3.45+0.25/2, 0, 1.85-0.75-1.0/2-0.05])
                rotate([0,0,-mUWAng])
                translate([0, -2.0/2+0.63, 0])
                    cube([0.25, 2.0, 1.00], center=true);
                // Right wing
                translate([-3.45-0.25/2, 0, 1.85-0.75-1.0/2-0.05])
                rotate([0,0,mUWAng])
                translate([0, -2.0/2+0.63, 0])
                    cube([0.25, 2.0, 1.00], center=true);
            }
            // Connector hole
            translate([0, -3.50/2+0.001, 0])
            rotate([90, 0, 0])
                linear_extrude(height = 3.50, center=true, convexity = 6)
                    translate([0, -0.75, 0])
                        polygon(points = mUF, convexity = 6);
        }
        // Contacts
        translate([0, 2.70/2-3.50, 1.85-0.75-0.60/2-0.1]) {
            // Contacts block
            color("DarkGrey")
            translate([0, 0, 0])
                cube([2.60+0.5+0.2, 2.70, 0.60], center=true);
            color("Gold")
            for (i=[0:1:4])
                translate([-2.60/2+i*0.65, 0.001, -(0.60-0.20)/2-0.001])
                    cube([0.50, 2.70, 0.20], center=true);
        }
    }    
}

module QFN32(txt="ESP8266EX", chipc="DimGray") {
    // QFN-32 SMD IC Package
    // Rendered as centered on axis origin, laying over the XY plane, pin 1 in sector X-Y+
    
    MD = 0.2;
    
    translate([0,0,(0.8+0.1)/2]) {
        // Package
        color(chipc)
        cube([5.0, 5.0, 0.8], center=true);
        // Markings
        color("White")
        translate([-5.0/2+0.8, 5.0/2-0.8,(0.8-MD)/2+0.001])
            cylinder(d=0.3, h=MD, center=true);
        color("White")
        translate([0, -0.4,(0.8-MD)/2+0.001])
            linear_extrude(height=MD, center=true, convexity=10)
                text(txt, size=0.5, font="Arial:style=Regular", halign="center", valign="center");
        // Pins
        color("Silver")
        for (i=[0:1:3])
            for (j=[0:1:7])
                rotate([0, 0, i*90])
                translate([(0.5-5.0)/2-0.001, 0.5*3.5-0.5*j, -(0.8-0.2)/2-0.05])
                    cube([0.5, 0.25, 0.2], center=true);
    }
    
}

module WSON(pins=4, txt="8C429", chipc="DimGray") {
    // WSONx package (WSON8)
    // Rendered as centered on axis origin, laying over the XY plane, pin 1 in sector X-Y+
    
    MD = 0.2;
    XDim = 6.0;
    YDim = 1.19 + 1.27*(pins-1);
    ZDim = 0.8;
    
    translate([0,0,(ZDim+0.1)/2]) {
        // Package
        color(chipc)
        cube([XDim, YDim, 0.8], center=true);
        // Markings
        color("White")
        translate([-XDim/2+0.8, YDim/2-0.8,(0.8-MD)/2+0.001])
            cylinder(d=0.3, h=MD, center=true);
        color("White")
        translate([0, 0,(ZDim-MD)/2+0.001])
            linear_extrude(height=MD, center=true, convexity=10)
                text(txt, size=0.5, font="Arial:style=Regular", halign="center", valign="center");
        // Pins
        color("Gold")
        for (i=[0:1:1])
            for (j=[0:1:pins-1])
                rotate([0, 0, i*180])
                translate([(0.6-XDim)/2-0.001, (1.19-YDim)/2+1.27*j, -(ZDim-0.2)/2-0.05])
                    cube([0.6, 0.42, 0.2], center=true);
    }
    
}

module DFN(pins=4, txt="8C429", chipc="DimGray") {
    // DFN (DFN8 based)
    // Rendered as centered on axis origin, laying over the XY plane, pin 1 in sector X-Y-
    
    MD = 0.2;
    XDim = 0.25 + 0.5*(pins-1);
    YDim = 3.0;
    ZDim = 0.8;
    PX = 0.25;
    PXO = 0.25/2;
    PY = 0.40;
    PZ = 0.2;
    PZO = 0.02;
    
    translate([0,0,ZDim/2+PZO]) {
        // Package
        color(chipc)
        cube([XDim, YDim, 0.8], center=true);
        // Markings
        color("White")
        translate([-XDim/2+0.3, -YDim/2+0.3,(0.8-MD)/2+0.001])
            cylinder(d=0.3, h=MD, center=true);
        color("White")
        translate([0, 0,(ZDim-MD)/2+0.001])
            linear_extrude(height=MD, center=true, convexity=10)
                rotate([0,0,90])
                text(txt, size=0.5, font="Arial:style=Regular", halign="center", valign="center");
        // Pins
        color("Gold")
        for (i=[0:1:1])
            for (j=[0:1:pins-1])
                rotate([0, 0, i*180])
                translate([(-XDim)/2+PXO+0.5*j, (PY-YDim)/2-0.001, -(ZDim-PZ)/2-PZO])
                    cube([PX, PY, PZ], center=true);
    }
    
}


module SOIC(pins=4, type=0, txt="25Q08A", chipc="DimGray" ) {
    // SOIC 154/208mils SMD IC Package
    // Rendered as centered on axis origin, laying over the XY plane, pin 1 in sector X-Y+
    // pins: number of pins ON ONE SIDE
    // type: 0=154 (large), 1=208

    CX = 0.735*2 + 1.27*(pins-1);
    CY = [0.0254*154, 0.0254*208]; // 154, 208 mils
    CZ = 1.8;
    CA = 10;
    PX = 0.48;
    PY = 1.36;
    PZO = 0.25;
    PZ = CZ/2+1.5*PZO;
    PA = 60;
    MD = 0.2;
    
    translate([0,0,CZ/2+PZO]) {
        color(chipc)
        difference() {
            // Main body
            cube([CX, CY[type], CZ], center=true);
            // Cutouts
            for (i=[0:1:1])
                rotate([0,0,i*180]) {
                    translate([0, -CY[type]/2, 0.25/2])
                    rotate([-CA,0,0])
                    translate([0, -CY[type]/2, CZ/2])
                        cube([CX+0.2, CY[type], CZ], center=true);
                    translate([0, -CY[type]/2, -0.25/2])
                    rotate([CA,0,0])
                    translate([0, -CY[type]/2, -CZ/2])
                        cube([CX+0.2, CY[type], CZ], center=true);
                }
        }
        // Pins
        color("Silver")
        for (i=[0:1:1])
            rotate([0,0,i*180]) 
            for (j=[0:1:pins-1])
            translate([1.27*(-(pins-1)/2+j), (-PY-CY[type])/2, -(PZ-PZO)/2])
            intersection() {
                cube([PX, PY, PZ], center=true);
                union() {
                    translate([0, PY/2-(PY*0.3)/2,(PZ-PZO)/2])
                        cube([PX, PY*0.3, PZO], center=true);
                    rotate([180,0,0])
                    translate([0, PY/2-(PY*0.3)/2,(PZ-PZO)/2])
                        cube([PX, PY*0.3, PZO], center=true);
                    translate([0, 0, 0])
                    rotate([PA,0,0])
                        cube([PX, PY*1.5, PZO], center=true);
                }
            }
        // Markings
        color("White")
        translate([1.27*(-(pins-1)/2), -(CY[type]-MD)/2+0.6, (CZ-MD)/2+0.001])
            cylinder(d=0.3, h=MD, center=true);
        color("White")
        translate([0, 0,(CZ-MD)/2+0.001])
            linear_extrude(height=MD, center=true, convexity=10)
                text(txt, size=0.8, font="Arial:style=Regular", halign="center", valign="center");
    }
}

module uFl() {
    // uFl antenna connector
    // Rendered as centered on axis origin, laying over the XY plane, central contact pin towards Y-
    
    CPH = 1.25-0.35;
    
    translate([0, 0, 0.35/2]) {
        // Connector
        color("Gold")
        translate([0, 0, (0.35+CPH)/2]) {
            difference() {
                cylinder(d=2.0, h=CPH, center=true);
                color("Ivory")
                cylinder(d=2.0-0.4, h=CPH+0.1, center=true);
            }
            color("Gold")
            cylinder(d=0.5, h=CPH, center=true);
        }
        // Base
        color("Ivory")
        difference() {
            cube([2.6, 2.6, 0.35], center=true);
            translate([2.6/2-0.2, 2.6/2-0.2, 0])
            rotate([0,0,-45])
            translate([0,0.5,0])
                cube([1,1,1], center=true);
        }
        // Pins
        color("Silver") {
            translate([0, -2.6/4-0.2/2, (0.15-0.35)/2-0.001])
                cube([0.6, 0.2+2.6/2, 0.15], center=true);
            PGY = 2.6/2-0.8;
            translate([0, (2.6-PGY)/2+0.2/2, (0.35-0.15)/2+0.001])
                cube([0.6, PGY, 0.15], center=true);
            translate([-2.6/2, 0, (0.15-0.35)/2])
                cube([0.4, 1.8, 0.15], center=true);
            translate([2.6/2, 0, (0.15-0.35)/2])
                cube([0.4, 1.8, 0.15], center=true);
        }
    }
}

// --- ESP-X Modules Sub-circuits ---------------

module ESPAnt(antIdx=0) {
    // ESP Antenna (not a precise drawing)

    AntWW  = [ 0.6, 0.6 ];
    AntXD1 = [ 2.3, 6.4 ];
    AntXD2 = [ 2.3, 1.6 ];
    AntXD3 = [ 0,   3.9 ];
    AntYD1 = [ 5.4, 6.1 ];
    AntYD2 = [ 2.8, 3.9 ];
    AntYD3 = [ 3.6, 0   ];
    Ant = [
        // 0: ESP12F Antenna
       [
        [0,         AntYD1[0]],
        [AntXD2[0], 0],
        [0,         -AntYD1[0]],
        [0,         AntYD1[0]],
        [AntXD1[0], 0],
        [0,         -AntYD2[0]],
        [AntXD1[0], 0],
        [0,         AntYD2[0]],
        [AntXD1[0], 0],
        [0,         -AntYD2[0]],
        [AntXD1[0], 0],
        [0,         AntYD2[0]],
        [AntXD1[0], 0],
        [0,         -AntYD3[0]]
       ],
        // 1: ESP1 Antenna
       [
        [AntXD1[1], 0],
        [-AntXD1[1],0],
        [0,         AntYD1[1]],
        [AntXD2[1], 0],
        [0,         -AntYD2[1]],
        [AntXD2[1], 0],
        [0,         AntYD2[1]],
        [AntXD2[1], 0],
        [0,         -AntYD2[1]],
        [AntXD2[1], 0],
        [0,         AntYD2[1]],
        [AntXD3[1], 0]
       ]
    ];

    DrawPath(path=Ant[antIdx], w=AntWW[antIdx], h=0.2);
}

module ESP8266MOD(txt=["ESP8266MOD","AI-THINKER","","ISM  2.4GHz","PA  +25dBm","802.11b/g/n"]) {
    // Shielded ESP8266 module (common to all shielded mods AFAIK)
    // Rendered X,Y and Z centered on the axis, text bottom towards Y-
    
    XDim = 15.0;
    YDim = 12.1;
    ZDim = 2.4;
    MD = 0.2;
    TS = 0.8;
    
    color("Silver")
    cube([XDim, YDim, ZDim], center=true);
    color("white")
    for (i=[0:1:len(txt)-1])
        translate([-0.5,0.5-(TS+0.2)*i,(ZDim-MD)/2+0.001])
            linear_extrude(height=MD, center=true, convexity=10)
                            text(txt[i], size=TS, font="Arial:style=Bold", halign="left", valign="center");    
}

// --- ESP-X Modules ----------------------------

module ESP01(col="DodgerBlue", pins=0) {
    // ESP-01
    // Only the essential parts are drawn (no SMDs, except LEDs and ICs)
    // Rendered as centered on X, antenna side on X axis, laying over the XY plane

    // pins:
    //  0: No pins
    //  1: Male headers under
    //  2: Male headers over
    //  3: Female headers under
    //  4: Female headers over

    XDim = 14.3;
    YDim = 24.8;
    ZDim = 0.8;
    P1X = -2.54*1.5;
    P1Y = 1.6;
    
    translate([0, -YDim/2,ZDim/2]) {
        // PCB
        difference() {
            color(col)
            cube([XDim, YDim, ZDim], center=true);

            if (pins==0)
                for (i=[0:1:1])
                    translate([P1X, -YDim/2+P1Y+2.54*i, 0])
                        rotate([0,0,-90])
                        PINHEADHOLES(n=4);
            }
           
        // Antenna
        color("Gold")
        translate([-XDim/2+2.0, YDim/2-7.0, ZDim/2])
            ESPAnt(ESP1ANT);

        // Pins
        if (pins==2)
            for (i=[0:1:1])
                translate([P1X, -YDim/2+P1Y+2.54*i, ZDim/2])
                rotate([0,0,-90])
                    PINHEADM(n=4, col="Yellow");
        if (pins==4)
            for (i=[0:1:1])
                translate([P1X, -YDim/2+P1Y+2.54*i, ZDim/2])
                rotate([0,0,-90])
                    PINHEADF(n=4, col="DarkGrey");
        if (pins==1)
            for (i=[0:1:1])
                rotate([0,180,0])
                translate([P1X, -YDim/2+P1Y+2.54*i, ZDim/2])
                rotate([0,0,-90])
                    PINHEADM(n=4, col="Yellow");
        if (pins==3)
            for (i=[0:1:1])
                rotate([0,180,0])
                translate([P1X, -YDim/2+P1Y+2.54*i, ZDim/2])
                rotate([0,0,-90])
                    PINHEADF(n=4, col="DarkGrey");

        // LEDs
        // Power LED
        color("Red", 0.7)
        translate([XDim/2-1.0, YDim/2-7.4, (ZDim+0.58)/2])
            rotate([0,0,90])
            cube([1.60, 0.80, 0.58], center=true);
        // Activity LED
        color("Blue", 0.7)
        translate([XDim/2-4.4, YDim/2-7.4, (ZDim+0.58)/2])
            rotate([0,0,90])
            cube([1.60, 0.80, 0.58], center=true);
 
        // ICs
        translate([5.2-XDim/2, 10.8-YDim/2, ZDim/2])
            QFN32();
        translate([11.6-XDim/2, 10.7-YDim/2, ZDim/2])
        rotate([0,0,180])
            SOIC(pins=4, type=1);
    }
}

module ESP02(col="Red") {
    // ESP-02 (V3)
    // Only the essential parts are drawn (no resistors/capacitors)
    // Rendered as centered on X, antenna connector near axis, laying over the XY plane

    XDim = 14.5;
    YDim = 14.7;
    ZDim = 0.8;
    AXP = 2.0;
    AYP = 2.0;
    CXO = 1.0;
    
    translate([0, -YDim/2,ZDim/2]) {
        difference() {
            // PCB
            color(col)
            cube([XDim, YDim, ZDim], center=true);
            // Sides Pins
            color("Gold")
            for (i=[-1:2:1])
                translate([XDim/2-CXO, (-i)*YDim/2, ZDim/2])
                rotate([0,0,90])
                    PINHEADHOLES(n=4);
        }
        // Antenna connector
        translate([-XDim/2+AXP, YDim/2-AYP, ZDim/2])
            uFl();

        // ICs
        translate([5.22-XDim/2, YDim/2-8.6, ZDim/2])
            QFN32();
        translate([11.2-XDim/2, YDim/2-8.6, ZDim/2])
        rotate([0,0,180])
            SOIC(pins=4, type=0);
    }
}

module ESP03(col="DodgerBlue") {
    // ESP-03
    // Only the essential parts are drawn (no resistors/capacitors)
    // Rendered as centered on X, antenna connector near axis, laying over the XY plane

    XDim = 17.3;
    YDim = 12.1;
    ZDim = 0.8;
    AXP = 1.1;
    AYP = -0.5;
    CXO = 1.27;
    
    translate([0, -YDim/2,ZDim/2]) {
        difference() {
            // PCB
            color(col)
            cube([XDim, YDim, ZDim], center=true);
            // Sides Pins
            color("Gold")
            for (i=[-1:2:1])
                translate([XDim/2-CXO, (-i)*YDim/2, ZDim/2])
                rotate([0,0,90])
                    PINHEADHOLES(n=7, pitch=2.0);

        }
        // Antenna connector
        color("Ivory")
        translate([-XDim/2+AXP, AYP, (ZDim+0.9)/2])
            cube([1.9, 9.1,0.9], center=true);

        // ICs
        translate([7.8-XDim/2, YDim/2-7.1, ZDim/2])
            QFN32();
        translate([14.3-XDim/2, YDim/2-7.2, ZDim/2])
        rotate([0,0,180])
            SOIC(pins=4, type=0);
    }
}

module ESP04(col="DodgerBlue") {
    // ESP-04
    // Only the essential parts are drawn (no resistors/capacitors)
    // Rendered as centered on X, antenna connector near axis, laying over the XY plane

    XDim = 14.7;
    YDim = 12.1;
    ZDim = 0.8;
    AXP = 1.1;
    AYP = -0.5;
    CXO = 1.27;
    
    translate([0, -YDim/2,ZDim/2]) {
        difference() {
            // PCB
            color(col)
            cube([XDim, YDim, ZDim], center=true);
            // Sides Pins
            color("Gold")
            for (i=[-1:2:1])
                translate([XDim/2-CXO, (-i)*YDim/2, ZDim/2])
                rotate([0,0,90])
                    PINHEADHOLES(n=7, pitch=2.0);
        }

        // ICs
        translate([5.2-XDim/2, YDim/2-7.0, ZDim/2])
            QFN32();
        translate([11.8-XDim/2, YDim/2-7.2, ZDim/2])
        rotate([0,0,180])
            SOIC(pins=4, type=0);
    }
}

module ESP05(pins=2, type=5, col="DodgerBlue") {
    // ESP-05 V4 (4 pins) & V5 (5 pins)
    // Only the essential parts are drawn (no resistors/capacitors)
    // Rendered as centered on X, antenna connector near axis, laying over the XY plane

    // pins:
    //  0: No pins
    //  1: Male headers under
    //  2: Male headers over
    //  3: Female headers under
    //  4: Female headers over

    XDim = 14.2;
    YDim = 14.2;
    ZDim = 0.8;
    AXP = 2.54;
    AYP = 2.54;
    OFT = [3.0, 2.1];
    
    translate([0, -YDim/2,ZDim/2]) {
        difference() {
            // PCB
            color(col)
            cube([XDim, YDim, ZDim], center=true);
            // Sides Pins
            color("Gold")
            if (pins==0)
                translate([XDim/2-OFT[type-4], -YDim/2+1.1, ZDim/2])
                rotate([0,0,90])
                    PINHEADHOLES(n=type);
        }
        // Antenna connector
        translate([-XDim/2+AXP, YDim/2-AYP, ZDim/2])
            uFl();

        // ICs
        translate([5.1-XDim/2, -YDim/2+6.3, ZDim/2])
            QFN32();
        translate([11.2-XDim/2, -YDim/2+6.2, ZDim/2])
        rotate([0,0,180])
            SOIC(pins=4, type=0);

        // Pins
        if (pins==2)
            translate([XDim/2-OFT[type-4], -YDim/2+1.1, ZDim/2])
            rotate([0,0,90])
                PINHEADM(n=type, col="Yellow");
        if (pins==4)
            translate([XDim/2-OFT[type-4], -YDim/2+1.1, ZDim/2])
            rotate([0,0,90])
                PINHEADF(n=type, col="DarkGrey");
        if (pins==1)
            rotate([0,180,0])
            translate([XDim/2-OFT[type-4], -YDim/2+1.1, ZDim/2])
            rotate([0,0,90])
                PINHEADM(n=type, col="Yellow");
        if (pins==3)
            rotate([0,180,0])
            translate([XDim/2-OFT[type-4], -YDim/2+1.1, ZDim/2])
            rotate([0,0,90])
                PINHEADF(n=type, col="DarkGrey");
    }
}

module ESP06(col="DodgerBlue") {
    // ESP-06
    // Rendered as centered on X, antenna side on X axis, laying over the XY plane

    XDim = 16.3;
    YDim = 13.1;
    ZDim = 0.8;
    PXD = 1.0;
    PYD = 1.0;
    PZD = 0.1;
    PXO = 2.6;
    PYO = 2.9;
    MPXD = 1.4;
    MPYD = 1.4;
    MPXO = 0.5;
    MPYO = 0.5;
    LPXD = 2.0;
    LPYD = 2.0;
    
    translate([0, -YDim/2,ZDim/2]) {
        // PCB
        color(col)
        cube([XDim, YDim, ZDim], center=true);
        translate([0, 0, (ZDim+2.4)/2])
            ESP8266MOD();

        // Sides Pins
        color("Gold")
        for (i=[0:1:1]) {
            for (j=[-1:1:1])
                rotate([0,0,i*180]) {
                    // Small pads Y-
                    translate([j*PXO, (PYD-YDim)/2+0.001, (PZD-ZDim)/2-0.001])
                        cube([PXD, PYD, PZD], center=true);
                    // Small pads X+
                    translate([(XDim-PYD)/2-0.001, j*PYO, (PZD-ZDim)/2-0.001])
                        cube([PYD, PXD, PZD], center=true);
                }
        }
        color("Gold")
        for (i=[0:1:3])
            rotate([0,0,i*90]) {
                // Medium center ground pads
                translate([MPXO+MPXD/2, MPXO+MPYD/2, (PZD-ZDim)/2-0.001])
                    cube([MPXD, MPYD, PZD], center=true);
            }
        color("Gold")
        for (i=[0:1:1])
            rotate([0,0,i*180]) {
                // Large corner pad X+Y+
                translate([(XDim-LPXD)/2-0.001, (YDim-LPYD)/2-0.001, (PZD-ZDim)/2-0.001])
                    cube([LPYD, LPXD, PZD], center=true);
                // Large corner pad X+Y-
                translate([(XDim-LPXD)/2-0.001, (LPYD-YDim)/2+0.001, (PZD-ZDim)/2-0.001])
                    cube([LPYD, LPXD, PZD], center=true);
            }
    }
}

module ESP07_16(col="DodgerBlue") {
    // ESP-07 16 pins version (old 14 pins version is shorter)
    // Only the essential parts are drawn (no resistors/capacitors)
    // Rendered as centered on X, antenna connector near axis, laying over the XY plane
    
    XDim = 22.0;
    YDim = 16.0;
    ZDim = 0.8;
    AXP = 1.7;
    AYP = 10.4;
    ACXP = 2.4;
    ACYP = 2.2;
    CXO = 1.6;
    L1XP = 4.3;
    L1YP = 8.0;
    L2XP = 4.3;
    L2YP = 13.4;
    WMXP = 13.0;
    WMYP = 0;
    
    translate([0, -YDim/2,ZDim/2]) {
        difference() {
            // PCB
            color(col)
            cube([XDim, YDim, ZDim], center=true);
            // Sides Pins
            color("Gold")
            for (i=[-1:2:1]) {
                translate([XDim/2-CXO, -i*YDim/2, ZDim/2])
                rotate([0,0,90])
                    PINHEADHOLES(n=8, pitch=2.0);
                translate([XDim/2-CXO, -i*YDim/2+i*1.0, ZDim/2])
                rotate([0,0,90])
                    PINHEADHOLES(n=8, pitch=2.0);
            }
        }
        
        // Wifi module
        translate([-XDim/2+WMXP, WMYP, (ZDim+2.4)/2])
            ESP8266MOD();
        
        // Antenna module
        color("Ivory")
        translate([-XDim/2+AXP, -YDim/2+AYP, (ZDim+0.9)/2])
            cube([1.9, 9.1,0.9], center=true);
        // Antenna connector
        translate([-XDim/2+ACXP, -YDim/2+ACYP, ZDim/2])
        rotate([0,0,180])
            uFl();

        // Power LED
        color("Red", 0.7)
        translate([-XDim/2+L1XP, -YDim/2+L1YP, (ZDim+0.58)/2])
            cube([0.8, 1.6, 0.58], center=true);
        // Activity LED
        color("Blue", 0.7)
        translate([-XDim/2+L2XP, -YDim/2+L2YP, (ZDim+0.58)/2])
            cube([0.8, 1.60, 0.58], center=true);
    }
}


module ESP08(col="ForestGreen") {
    // ESP-08
    // Only the essential parts are drawn (no resistors/capacitors)
    // Rendered as centered on X, antenna connector near axis, laying over the XY plane
    
    XDim = 17.0;
    YDim = 16.0;
    ZDim = 0.8;
    CXO = 1.6;
    WMXP = 0.0;
    WMYP = 0.0;
    
    translate([0, -YDim/2,ZDim/2]) {
        difference() {
            // PCB
            color(col)
            cube([XDim, YDim, ZDim], center=true);
            // Sides Pins
            color("Gold")
            for (i=[-1:2:1])
                translate([XDim/2-CXO, -i*YDim/2, ZDim/2])
                rotate([0,0,90])
                    PINHEADHOLES(n=8, pitch=2.0);
        }
        
        // Wifi module
        translate([WMXP, WMYP, (ZDim+2.4)/2])
            ESP8266MOD();
    }
}

module ESP09(col="DodgerBlue") {
    // ESP-09
    // Rendered as centered on X, antenna side on X axis, laying over the XY plane

    XDim = 10.0;
    YDim = 10.0;
    ZDim = 0.8;
    PXD = 0.8;
    PYD = 1.0;
    PZD = 0.1;
    PXO = 1.9;
    PYO = 1.9;
    PBO = 0.2;
    MPXD = 1.6;
    MPYD = 1.6;
    MPXO = 0.3;
    MPYO = 0.3;
    LPXD = 1.6;
    LPYD = 1.6;
    QXO = 4.0;
    QYO = 4.5;
    SXO = 8.8;
    SYO = 3.2;
    
    translate([0, -YDim/2,ZDim/2]) {
        // PCB
        color(col)
        cube([XDim, YDim, ZDim], center=true);

        // ICs
        translate([QXO-XDim/2, -YDim/2+QYO, ZDim/2])
            QFN32();
        translate([SXO-XDim/2, -YDim/2+SYO, ZDim/2])
        rotate([0,0,180])
            DFN(pins=4);

        // Sides Pins
        color("Gold")
        for (i=[0:1:1]) {
            for (j=[-1:1:1])
                rotate([0,0,i*180]) {
                    // Small pads Y-
                    translate([j*PXO, (PYD-YDim)/2+PBO, (PZD-ZDim)/2-0.001])
                        cube([PXD, PYD, PZD], center=true);
                    // Small pads X+
                    translate([(XDim-PYD)/2-PBO, j*PYO, (PZD-ZDim)/2-0.001])
                        cube([PYD, PXD, PZD], center=true);
                }
        }
        color("Gold")
        for (i=[0:1:3])
            rotate([0,0,i*90]) {
                // Medium center ground pads
                translate([MPXO+MPXD/2, MPXO+MPYD/2, (PZD-ZDim)/2-0.001])
                    cube([MPXD, MPYD, PZD], center=true);
            }
        color("Gold")
        for (i=[0:1:1])
            rotate([0,0,i*180]) {
                // Large corner pad X+Y+
                translate([(XDim-LPXD)/2-PBO, (YDim-LPYD)/2-PBO, (PZD-ZDim)/2-0.001])
                    cube([LPYD, LPXD, PZD], center=true);
                // Large corner pad X+Y-
                translate([(XDim-LPXD)/2-PBO, (LPYD-YDim)/2+PBO, (PZD-ZDim)/2-0.001])
                    cube([LPYD, LPXD, PZD], center=true);
            }
    }
}

module ESP10(col="Red") {
    // ESP-04
    // Only the essential parts are drawn (no resistors/capacitors)
    // Rendered as centered on X, antenna connector near axis, laying over the XY plane

    XDim = 15.8;
    YDim = 10.0;
    ZDim = 0.8;
    AXP = 1.1;
    AYP = -0.5;
    CXO = 0;
    QXD = 5.2;
    QYD = 4.0;
    SXD = 11.6;
    SYD = 3.8;
    H1D = 0.8;
    H1XP = 1.0;
    H1YP = 9.2;
    H2D = 0.8;
    H2XP = 12.5;
    H2YP = 8.6;
    
    translate([0, -YDim/2,ZDim/2]) {
        difference() {
            // PCB
            color(col)
            cube([XDim, YDim, ZDim], center=true);
            // Sides Pins
            color("Gold") {
                translate([XDim/2-CXO, (-4*2.0)/2, ZDim/2])
                    PINHEADHOLES(n=5, pitch=2.0);
                translate([-XDim/2+H1XP, -YDim/2+H1YP, 0])
                    cylinder(d=H1D, h=ZDim*2, center=true);
                translate([-XDim/2+H2XP, -YDim/2+H2YP, 0])
                    cylinder(d=H2D, h=ZDim*2, center=true);
            }
        }

        // ICs
        translate([QXD-XDim/2, -YDim/2+QYD, ZDim/2])
            QFN32();
        translate([SXD-XDim/2, -YDim/2+SYD, ZDim/2])
        rotate([0,0,180])
            SOIC(pins=4, type=0, txt="25Q40B");
    }
}

module ESP11(col="DodgerBlue") {
    // ESP-11
    // Only the essential parts are drawn (no resistors/capacitors)
    // Rendered as centered on X, antenna connector near axis, laying over the XY plane

    XDim = 17.3;
    YDim = 12.1;
    ZDim = 0.8;
    AXP = 1.2;
    AYP = 0;
    CYO = 1.8;
    QXP = 7.4;
    QYP = 4.8;
    SXP = 13.2;
    SYP = 5.2;
    
    translate([0, -YDim/2,ZDim/2]) {
        difference() {
            // PCB
            color(col)
            cube([XDim, YDim, ZDim], center=true);
            // Sides Pins
            color("Gold")
            for (i=[0:1:1])
                translate([XDim/2-i*0.8, -YDim/2+CYO, ZDim/2])
                    PINHEADHOLES(n=8, pitch=1.27);

        }
        // Antenna connector
        color("Ivory")
        translate([-XDim/2+AXP, AYP, (ZDim+0.9)/2])
            cube([1.9, 9.1,0.9], center=true);

        // ICs
        translate([QXP-XDim/2, QYP-YDim/2, ZDim/2])
            QFN32();
        translate([SXP-XDim/2, SYP-YDim/2, ZDim/2])
        rotate([0,0,180])
            SOIC(pins=4, type=0, txt="25Q40B");
    }
}

module ESP12F(col="Black") {
    // ESP-12F
    // Only the essential parts are drawn (no resistors/capacitors, no regulator)
    // Rendered as centered on X, antenna side on X axis, laying over the XY plane

    XDim = 16.0;
    YDim = 24.0;
    ZDim = 0.8;
    
    translate([0, -YDim/2,ZDim/2]) {
        // PCB
        color(col)
        cube([XDim, YDim, ZDim], center=true);
        translate([0, -(YDim-15.0)/2 + 1.3, (ZDim+2.4)/2])
        rotate([0,0,-90])
            ESP8266MOD();
        // Antenna
        color("Gold")
        translate([-XDim/2+1.0, YDim/2-6.0, ZDim/2])
            ESPAnt(ESP12ANT);
        // Sides Pins
        color("Gold")
        for (i=[-1:2:1])
        for (j=[0:1:7])
            translate([i*(XDim-1.5+0.001)/2, -YDim/2+1.5+2*j, 0])
                cube([1.5, 1.0, ZDim+0.02], center=true);
        color("Gold")
        for (i=[0:1:5])
            translate([-5+i*2, -YDim/2+1.0/2-0.001, 0])
                cube([1.0, 1.0, ZDim+0.02], center=true);
        // Activity LED
        color("Blue", 0.7)
        translate([XDim/2-1.95, YDim/2-7.08, (ZDim+0.58)/2])
            cube([1.60, 0.80, 0.58], center=true);
    }
}

// --- Witty ------------------------------------
module Witty_Base(pins=true) {
    // Witty Wifi Base Module
    
    XDim = 31.5;
    YDim = 30.0;
    ZDim = 1.14;
    
    translate([0, 0, ZDim/2]) {
        // PCB
        color("DimGray")
        difference() {
            cube([XDim, YDim, ZDim], center=true);
            if (!pins)
                // Pinheads pins holes
                for (i=[-1:2:1])
                    translate([i*14.05, -11.3, -ZDim])
                        PINHEADHOLES(n=8);
        }
        // Top layer
        translate([0, 0, ZDim/2]) {
            // Led 5050
            translate([11.8, 11.2, 0])
                LED5050();
            // LDR
            translate([-12.5, 11.2, 0])
                LDR();
            // Pinheads
            if (pins)
                for (i=[-1:2:1])
                    translate([i*14.05, -11.3, -ZDim])
                    rotate([0,180,0])
                        PINHEADM(n=8, col="Yellow");
            // ESP12F
            translate([0, YDim/2, 0])
                ESP12F();
            
        }
        // Bottom layer
        translate([0, 0, -ZDim/2])
        rotate([0, 180, 0]) {
            // Push button
            translate([1.1, YDim/2, 0])
                PUSHBS();
            // Micro USB connector
            rotate([0,0,180])
            translate([0, YDim/2, 0])
                mUSBF();
        }
    }
    
    
}

// --- Wemos D1 Mini ----------------------------

module WemosD1M(pins=0) {
    // Wemos D1 Mini Module
    
    // Pins:
    //  0: No pins
    //  1: Male headers under
    //  2: Male headers over
    //  3: Female headers under
    //  4: Female headers over

    XDim = 25.6;
    YDim = 34.2;
    ZDim = 1.6;
    BCR = 3.4;
    SCR = 0.8;
    CCX = 2.8;
    CCY = 6.2;
    CCA = 0;
    P1Y = 8.2;
    USBYC = 1.1;
    USBYO = 0.5;
    USBXC = 7.7;
    PBXO = 3.4;
    PBYO = 3.6;
    
    translate([0,0,0]) {
        // PCB
        color("DodgerBlue")
        difference() {
            cube([XDim, YDim, ZDim], center=true);
            translate([-(XDim-CCX)/2-0.1, -(YDim-CCY)/2-0.1,0])
                cube([CCX+0.2, CCY+0.2, ZDim+0.2], center=true);
            translate([-XDim/2+CCX, -YDim/2+CCY,0])
            rotate([0,0,-CCA])
            translate([-CCX, -CCY/2,0])
                cube([CCX*2, CCY, ZDim+0.2], center=true);
            translate([-XDim/2, YDim/2,0])
            rotate([0,0,-90])
                edge(BCR, ZDim+0.2);
            translate([XDim/2, YDim/2,0])
            rotate([0,0,180])
                edge(BCR, ZDim+0.2);
            translate([XDim/2, -YDim/2,0])
            rotate([0,0,90])
                edge(SCR, ZDim+0.2);
            translate([-XDim/2+CCX, -YDim/2,0])
            rotate([0,0,0])
                edge(SCR, ZDim+0.2);
            translate([0, (USBYC-YDim)/2-0.1,0])
                cube([USBXC, USBYC+0.2, ZDim+0.2], center=true);
            
            if (pins==0)
                for (i=[-1:2:1])
                    translate([i*22.86/2, -YDim/2+P1Y, 0])
                        PINHEADHOLES(n=8);
            
        }
        // Top layer
        translate([0, 0, ZDim/2]) {
            // ESP12F
            translate([0, YDim/2, 0])
                ESP12F();
        if (pins==2)
            for (i=[-1:2:1])
                translate([i*22.86/2, -YDim/2+P1Y, 0])
                    PINHEADM(n=8, col="Yellow");

        if (pins==4)
            for (i=[-1:2:1])
                translate([i*22.86/2, -YDim/2+P1Y, 0])
                    PINHEADF(n=8, col="DarkGrey");

        }
        // Bottom layer
        translate([0, 0, -ZDim/2])
        rotate([0, 180, 0]) {
            // Micro USB connector
            rotate([0,0,180])
            translate([0, YDim/2-USBYO, 0])
                mUSBF();

            // Push button
            translate([XDim/2-PBXO, PBYO-YDim/2, 0])
            rotate([0,0,-90])
                PUSHBS(XDim = 4.6, YDim = 2.5, ZDim = 2.3, BXDim = 1.9, BYDim = 1.0, BZDim = 1.0);

        if (pins==1)
            for (i=[-1:2:1])
                translate([i*22.86/2, -YDim/2+P1Y, 0])
                    PINHEADM(n=8, col="Yellow");

        if (pins==3)
            for (i=[-1:2:1])
                translate([i*22.86/2, -YDim/2+P1Y, 0])
                    PINHEADF(n=8, col="DarkGrey");
        }
    }
}

module WemosD1M_USBLocate() {
    // Locates the first children at the center of the USB
    // connector mouth, on the vertical plane of the base
    // of the exterior of the connector lips

    // USB Connector entry :
    // Y short width 5.40
    // Y long width 6.87
    // Z height: 1.85
    YDim = 34.2;
    ZDim = 1.6;
    USBYO = 0.5;
    // Wemos D1 Mini USB Locator Module
    translate([0, 0, -ZDim/2])
    rotate([0, 180, 0])
        rotate([0,0,180])
        translate([0, YDim/2-USBYO - 2.35, 1.175])
            children(0);
//                #cube([6.87,0.001,1.85], center=true);
    
}

module WemosD1M_PBLocate() {
    // Locates the first children at the center of the push
    // button, on the vertical plane of the exterior of the button
    XDim = 25.6;
    YDim = 34.2;
    ZDim = 1.6;
    PBXO = 3.4;
    PBYO = 3.6;

    translate([0, 0, -ZDim/2])
    rotate([0, 180, 0])
        translate([XDim/2-PBXO+1, PBYO-YDim/2, 2.3/2])
        rotate([0,0,-90])
            children(0);
//                #cube([6.87,0.001,1.85], center=true);
    
}


// --- NodeMCU  Mini ----------------------------
module NodeMCU(pins=0, phmc="Yellow", phfc="DarkGrey") {
    // NimdeMCU V1.0 (Amica)
    
    // Pins:
    //  0: No pins
    //  1: Male headers under
    //  2: Male headers over
    //  3: Female headers under
    //  4: Female headers over

    XDim = 25.0;
    YDim = 48.0;
    ZDim = 1.6;
    HD = 3.0;
    HXO = 2.0;
    HYO = 2.2;
    BCR = 2.0;
    SCR = 0.8;
    CCA = 0;
    USBYC = 1.1;
    USBYO = -0.2;
    USBXC = 7.7;
    PBXO = 6.2;
    PBYO = 3.2;
    
    translate([0,0,0]) {
        // PCB
        color("DodgerBlue")
        difference() {
            cube([XDim, YDim, ZDim], center=true);
            translate([-XDim/2, YDim/2,0])
            rotate([0,0,-90])
                edge(BCR, ZDim+0.2);
            translate([XDim/2, YDim/2,0])
            rotate([0,0,180])
                edge(BCR, ZDim+0.2);
            translate([XDim/2, -YDim/2,0])
            rotate([0,0,90])
                edge(BCR, ZDim+0.2);
            translate([-XDim/2, -YDim/2,0])
            rotate([0,0,0])
                edge(BCR, ZDim+0.2);
            
            translate([-XDim/2+HXO, YDim/2-HYO,0])
                cylinder(d=HD, h=ZDim*2, center=true);
            translate([XDim/2-HXO, YDim/2-HYO,0])
                cylinder(d=HD, h=ZDim*2, center=true);
            translate([-XDim/2+HXO, -YDim/2+HYO,0])
                cylinder(d=HD, h=ZDim*2, center=true);
            translate([XDim/2-HXO, -YDim/2+HYO,0])
                cylinder(d=HD, h=ZDim*2, center=true);
            
            if (pins==0)
                for (i=[-1:2:1])
                    translate([i*22.86/2, -(7*2.54), 0])
                        PINHEADHOLES(n=15);
            
        }
        // Top layer
        translate([0, 0, ZDim/2]) {
            // Micro USB connector
            rotate([0,0,180])
            translate([0, YDim/2-USBYO, 0])
                mUSBF();

            // ESP12F
            translate([0, YDim/2, 0])
                ESP12F();
        if (pins==2)
            for (i=[-1:2:1])
                translate([i*22.86/2, -(7*2.54), 0])
                    PINHEADM(n=15, col=phmc);

        if (pins==4)
            for (i=[-1:2:1])
                translate([i*22.86/2, -(7*2.54), 0])
                    PINHEADF(n=15, col=phfc);

            // Push buttons
            translate([PBXO-2.8/2, PBYO-YDim/2, 0])
            rotate([90,0,90])
                translate([0, 1.6, 0])
                    PUSHBS(XDim = 4.0, YDim = 1.6, ZDim = 2.8, BXDim = 2.0, BYDim = 0.8, BZDim = 2.0);
            translate([-PBXO-2.8/2, PBYO-YDim/2, 0])
            rotate([90,0,90])
                translate([0, 1.6, 0])
                    PUSHBS(XDim = 4.0, YDim = 1.6, ZDim = 2.8, BXDim = 2.0, BYDim = 0.8, BZDim = 2.0);

        }
        // Bottom layer
        translate([0, 0, -ZDim/2])
        rotate([0, 180, 0]) {

        if (pins==1)
            for (i=[-1:2:1])
                translate([i*22.86/2, -(7*2.54), 0])
                    PINHEADM(n=15, col=phmc);

        if (pins==3)
            for (i=[-1:2:1])
                translate([i*22.86/2, -(7*2.54), 0])
                    PINHEADF(n=15, col=phfc);
        }
    }
}

module NodeMCU_USBLocate() {
    // Locates the first children at the center of the USB
    // connector mouth, on the vertical plane of the base
    // of the exterior of the connector lips

    // USB Connector entry :
    // Y short width 5.40
    // Y long width 6.87
    // Z height: 1.85
    YDim = 48.0;
    ZDim = 1.6;
    USBYO = -0.2;
    
    translate([0, 0, ZDim/2]) {
        // Micro USB connector
        rotate([0,0,180])
        translate([0, YDim/2-USBYO, 2.3/2])
            children(0);
//            #cube([6.87,0.001,1.85], center=true);
    }    
}

module NodeMCU_PBLocate() {
    // Locates the first TWO children at the center of the push
    // buttons, on the vertical plane of the exterior of the button
    // The first button is located over the USER button and the
    // second over the RESET button

    YDim = 48.0;
    ZDim = 1.6;
    PBXO = 6.2;
    PBYO = 3.2;
    BYDim = 0.8;
    PBZDim = 2.8;
    
        translate([0, 0, ZDim/2]) {
            translate([-PBXO-2.8/2+PBZDim/2, PBYO-YDim/2, BYDim])
            rotate([90,0,90])
                translate([0, 1.6, 0])
                    children(0);
            translate([PBXO-2.8/2+PBZDim/2, PBYO-YDim/2-0.1, BYDim])
            rotate([90,0,90])
                translate([0, 1.6, 0])
                    children(1);
        }
}

// --- Demo modules -----------------------------

module ESPDTxt(txt="XXX", txts=3.0) {
    
    translate([0, txts+0.5, 0])
    color("White")
    linear_extrude(height=0.1, center=true, convexity=10)
        text(txt, size=txts, font="Arial:style=Bold", halign="center", valign="top");
}

 module ESPDPlace(XO=30.0, YO=30.0, n=4) {
    nparts = $children;
    for(i=[0:1:n-1], j=[0:nparts/n])
        if (i+n*j < nparts)
            translate([XO*(i+1), YO*j, 0]) {
                children(i+n*j);
            }
 }

module ESPDemo() {
    XO = 30.0;
    YO = 29.0;

    // Render ESP base Modules
    translate([0, YO, 0])
    ESPDPlace(XO=XO, YO=YO, n=4) {
        ESP01(pins=1);
        ESP02(col="Grey");
        ESP03();
        ESP04();
        ESP05(type=5);
        ESP06();
        ESP07_16();
        ESP08();
        ESP09();
        ESP10();
        ESP11();
        translate([0,24.0/2,0]) ESP12F();
        Witty_Base(pins=true);
        WemosD1M(pins=2);
        NodeMCU(pins=1, phmc="Red", phfc="DarkGrey");
    }


    // Render ESP Modules Names
    translate([0, YO, 0])
    ESPDPlace(XO=XO, YO=YO, n=4) {
        ESPDTxt(txt="ESP01");
        ESPDTxt(txt="ESP02");
        ESPDTxt(txt="ESP03");
        ESPDTxt(txt="ESP04");
        ESPDTxt(txt="ESP05");
        ESPDTxt(txt="ESP06");
        ESPDTxt(txt="ESP07_16");
        ESPDTxt(txt="ESP08");
        ESPDTxt(txt="ESP09");
        ESPDTxt(txt="ESP10");
        ESPDTxt(txt="ESP11");
        translate([0,24.0/2,0]) ESPDTxt(txt="ESP12F");
        translate([0,30.0/2,0]) ESPDTxt(txt="Witty_Base");
        translate([0,34.2/2,0]) ESPDTxt(txt="WemosD1M");
        translate([0,48.0/2,0]) ESPDTxt(txt="NodeMCU");
    }
}

// --- Render -----------------------------------
// Only rendered if SCAD file is opened, not in USE mode...
ESPDemo();
