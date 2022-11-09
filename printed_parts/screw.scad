eps = 0.01;
angle = 30;
blades = 5;
d = 15;
h = 2;
l = 30;

module blade()
{
  dx = l-d/2;
  off = d*cos(angle) - h*cos(90-angle);
  translate([0, -off/2, 0])
    rotate([angle, 0, 0])
      {
        translate([dx, d/2, 0])
          cylinder(d=d, h=h, $fn=60);
        cube([dx, d, h]);
      }
}


module screw()
{
  module base()
  {
    difference()
    {
      union()
      {
        // core
        cylinder(d=d, h=d, $fn=60);
        // blades
        for(r=[0:blades-1])
          rotate([0, 0, r*360/blades])
            blade();
      }
      // rod hole
      translate([0, 0, -eps])
        cylinder(d=3.5, h=d+2*eps, $fn=60);
    }
  }
  intersection()
  {
    q = d*sin(angle);
    dh = h*sin(90-angle);
    base();
    translate([0, 0, dh])
      cylinder(d=3*l, h=q-dh);
  }
}

screw();
//blade(10, 2, 20);
