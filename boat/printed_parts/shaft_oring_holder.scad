use <m3d/fn.scad>

module shaft_oring_holder(h=7, d_bush=5, d_shaft=3, oring_d_int=3, oring_d=1)
{
  eps = 0.01;
  wall = 1;
  spacing = 1;
  d_int_oring = oring_d_int + 2*oring_d;
  d_int = max(d_bush, d_int_oring) + spacing;
  d_ext = d_int + 2*wall;
  difference()
  {
    $fn = fn(70);
    cylinder(d=d_ext, h=h);
    // oring slot
    translate([0,0,wall])
      cylinder(d=d_int_oring, h=h);
    // bush slot
    translate([0,0,2*wall])
      cylinder(d=d_int, h=h);
    // shaft slot
    translate([0,0,-eps])
      cylinder(d=d_shaft+spacing, h=h+2*eps);
  }
}

shaft_oring_holder();
