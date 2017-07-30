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
        
        //cube([new_x*2, new_y*0.2, new_zb*2], center = true);
    }
}

box(50, 160, 50, 20, 1, 5, 32, 133);
