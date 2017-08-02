$fn=30;

use <ESP8266Models.scad>;

// x = height
// y = widht
// zb = depth bottom
// zt = depth top
// t = thikness
// r = radius of round corner
// dispx = display height
// dispy = display width
// side = {true == left / false == right}
module box(x, y, zb, zt, t, r, dispx, dispy, side) {
    angle = atan(x/(zb-zt));
    new_x = x-2*r;
    new_y = y-2*r;
    new_zb = zb-2*r;
    move_of_hollow = (zb/2)/cos(90-angle);
    difference(){
        minkowski(){
            difference(){
                cube([new_x,new_y,new_zb], center = true);
                translate([move_of_hollow+(new_zb/2),0,-(new_x/2)])
                    rotate(angle, [0, 1, 0])
                        cube([10*x,y+1,zb], center = true);
            }
            sphere(r);
        }
        
        //Cut out ledge
        translate ([(-zb/2)+1,0,0]) cube([10, dispy, dispx], center = true);
        
        //Hollow inside
        minkowski(){
            difference(){
                cube([new_x-2*t,new_y-2*t,new_zb-2*t], center = true);
                translate([move_of_hollow+(new_zb/2-2*t),0,-(new_x/2)])
                    rotate(angle, [0, 1, 0])
                        cube([10*x-2*t,y+1-2*t,zb-2*t+0.5], center = true);
            }
            sphere(r);
        }
        
        side_position = side ? y/2 : -y/2 ;
        translate([0, side_position, -zb/4])
            rotate(90, [1, 0, 0])
                linear_extrude(height = r*2, center = true, convexity = 10, twist = 0)
                    scale([0.8,0.5,1])
                        circle(r = 5);
                    
        //cube([new_x*2, new_y*0.2, new_zb*2], center = true);
    }
}

box(50, 160, 50, 20, 2, 5, 32, 133, true);
//NodeMCU(pins=1);
