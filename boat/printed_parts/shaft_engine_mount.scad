use <m3d/fn.scad>

module shaft_engine_mount(d_mnt_eng=9,   h_mnt_eng=5,
                          d_mnt_shaft=3, h_mnt_shaft=10,
                          wall=2, spacing=0.75)
{
  eps = 0.01;
  h = h_mnt_eng + h_mnt_shaft + wall;
  d_ext = d_mnt_eng+spacing+2*wall;
  difference()
  {
    cylinder(d=d_ext, h=h, $fn=fn(50));
    // engine mount in the top
    translate([0,0,h-h_mnt_eng])
      cylinder(d=d_mnt_eng+spacing, h=h_mnt_eng+eps, $fn=fn(100));
    // shaft mount in the bottom
    translate([0,0,-eps])
      cylinder(d=d_mnt_shaft+spacing, h=h_mnt_shaft+eps, $fn=fn(50));
    // M2 screw hole for holding shaft in place
    {
      d_screw = 2+0.25;
      h_screw = 2*d_ext;
      translate([0, h_screw/2, max(h_mnt_shaft-2*d_screw, d_mnt_shaft/2)])
        rotate([90, 0, 0])
          cylinder(d=d_screw, h=h_screw, $fn=fn(50));
    }
  }
}


shaft_engine_mount();
