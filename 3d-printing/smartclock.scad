$fn=50;

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
        translate([8.5, side_position, -zb/10+4])
            rotate(angle, [0, 1, 0])
            rotate(90, [1, 0, 0])
                linear_extrude(height = r*2, center = true, convexity = 10, twist = 0)
                    scale([(sqrt(2)/2)*1.1,(sqrt(2)/2)*0.8,1])
                        circle(r = 5);
        
        translate([x/3-1-3.5,0,-5]){
            rotate(angle, [0, 1, 0])
                cube([15.5, 21, 10], center = true);
        }
//        cube([new_x*2, new_y*0.2, new_zb*2], center = true);
    }
    translate([-8,new_y/2-40,1]){
        difference(){
            translate([0, 0, -1])
                cube([5, 8, x-2], center = true);
            translate([-2.1, 0, 5]){
                rotate(90, [0, 1, 0])
                    cylinder(d=5, h=2, center=true);
            }
            translate([-2.1, 0, -8]){
                rotate(90, [0, 1, 0])
                    cylinder(d=5, h=2, center=true);
            }
        }
    }
    translate([-8,-new_y/2+40,1]){
        difference(){
            translate([0, 0, -1])
                cube([5, 8, x-2], center = true);
            translate([-2.1, 0, 5]){
                rotate(90, [0, 1, 0])
                    cylinder(d=5, h=2, center=true);
            }
            translate([-2.1, 0, -8]){
                rotate(90, [0, 1, 0])
                    cylinder(d=5, h=2, center=true);
            }
        }
    }
    
    translate([10, -y/3 + 0.7, 0])
        rotate(180+angle, [0, 1, 0])
                esp_mount(3, 2);
    translate([0, 0, -zb/2])
        stands(2, 2, 8, x-5, y-5, zb, 10);
    

    translate([x/3-1-3,15,-5]){
        rotate(angle, [0, 1, 0]){
            difference() {
                cylinder(d=6, h=4, center=true);
                translate([0,0,-0.1])
                    cylinder(d=3, h=4, center=true);
            }
        }
    }
    translate([x/3-1-3,-12.5,-5]){
        rotate(angle, [0, 1, 0]){
            difference() {
                //cylinder(d=4, h=4, center=true);
                cube([6,4,4], center = true);
                translate([0,0,-0.1])
                    cylinder(d=2.5, h=4, center=true);
            }
        }
    }
    difference() {
        translate([10-1.5, y/3, 0]){
            rotate(angle, [0, 1, 0]){
                difference() {
                    cube([24,24,6], center = true);
                    translate([0, -3, 0]){
                        translate([0, -1, -0.1])
                            cube([17.5,26,7], center = true);
                        translate([0, -1, -1])
                            cube([19.5,28,1.2], center = true);
                    }
                }
            }
        }
        translate([-1, y/3, 8]){
            cube([4,24.1,6], center = true);
        }
    }
}

// height_front = front distance to floor
// height_back = back distance to floor
// diameter = radius of stands
// x = size of bottom in x
// y = size of bottom in y
// zb = size of bottom in z
// distance = position: distance from edges
module stands(height_front, height_back, diameter, x, y, zb, distance) {
    difference() {
        union(){
            translate([-x/2+distance, y/2-distance,-height_front/2])
                cylinder(d=diameter, h=height_front, center=true);
            translate([+x/2-distance, y/2-distance,-height_front/2])
                cylinder(d=diameter, h=height_front, center=true);
            translate([-x/2+distance, -y/2+distance,-height_front/2])
                cylinder(d=diameter, h=height_front, center=true);
            translate([+x/2-distance, -y/2+distance,-height_front/2])
                cylinder(d=diameter, h=height_front, center=true);
        }
        angle = atan((height_front-height_back)/zb);
        translate([-x/2, 0, -zb/2-(height_front)])
            rotate(angle, [0, -1, 0])
                cube([x*2, y, zb], center=true);
    }
}

module esp_mount(height, depth) {
    XDim = 25.0;
    YDim = 48.0;
    HD = 5; //are in reality 3.0
    hole_size = 2.5;
    HXO = 2.0;
    HYO = 2.2;
    difference() {
        translate([-XDim/2+HXO, YDim/2-HYO,0])
            cylinder(d=HD, h=height*2, center=true);
        translate([-XDim/2+HXO, YDim/2-HYO,2*height-depth])
            cylinder(d=hole_size, h=height*2, center=true);
    }
    difference() {
        translate([XDim/2-HXO, YDim/2-HYO,0])
            cylinder(d=HD, h=height*2, center=true);
        translate([XDim/2-HXO, YDim/2-HYO,2*height-depth])
            cylinder(d=hole_size, h=height*2, center=true);
    }
    difference() {
        translate([-XDim/2+HXO, -YDim/2+HYO,0])
            cylinder(d=HD, h=height*2, center=true);
        translate([-XDim/2+HXO, -YDim/2+HYO,2*height-depth])
            cylinder(d=hole_size, h=height*2, center=true);
    }
    difference() {
        translate([XDim/2-HXO, -YDim/2+HYO,0])
            cylinder(d=HD, h=height*2, center=true);
        translate([XDim/2-HXO, -YDim/2+HYO,2*height-depth])
            cylinder(d=hole_size, h=height*2, center=true);
    }
}

box(50, 160, 50, 20, 2.5, 5, 32, 133, false);
