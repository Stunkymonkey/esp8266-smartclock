$fn=50;
// x = height
// y = widht
// zb = depth bottom
// zt = depth top
// t = thikness
// r = radius of round corner
// dispx = display height
// dispy = display width
module box(x, y, zb, zt, t, r, dispx, dispy) {
    angle = atan(x/(zb-zt));
    move_of_hollow = (zb/2)/cos(90-angle);
    difference(){
        minkowski(){
            difference(){
                cube([x,y,zb], center = true);
                translate([move_of_hollow+(zb/2),0,-(x/2)]) rotate(angle, [0, 1, 0]) cube([10*x,y+1,zb], center = true);
            }
            sphere(r);
        }
        
        //Cut out ledge
        translate ([-zb-1,0,0]) cube([10, dispy, dispx], center = true);
        
        //Hollow inside
        minkowski(){
            difference(){
                cube([x-2*t,y-2*t,zb-2*t], center = true);
                translate([move_of_hollow+(zb/2)-2*t,0,-(x/2)]) rotate(angle, [0, 1, 0]) cube([10*x-2*t,y+1-2*t,zb-2*t], center = true);
            }
            sphere(r);
        }
    }
}

box(5, 16, 5, 2, 0.3, 1, 3.2, 13.3);
