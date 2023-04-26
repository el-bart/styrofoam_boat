use <m3d/fn.scad>

eps = 0.01;

module rudder_hold()
{
  module slot(slot_h, dh=0)
  {
    h = slot_h + dh;
    // top-down and center
    hull()
    {
      d = 6;
      for(dy=[-1,+1])
        translate([0, dy*(36/2-d/2), 0])
          cylinder(d=d, h=h, $fn=fn(50));
      cylinder(d=8+2, h=h);
    }
    // left-right
    hull()
    {
      d = 5;
      for(dx=[-1,+1])
        translate([dx*(20/2-d/2), 0, 0])
          cylinder(d=d, h=h, $fn=fn(50));
    }
  }

  core_size = [25, 40, 4];
  slot_h = 1.5 + 0.5;
  difference()
  {
    translate(-0.5*[core_size[0], core_size[1], 0])
      cube(core_size);
    translate([0, 0, core_size[2]-slot_h])
      slot(slot_h=slot_h, dh=eps);
  }
}

module rudder_body()
{
  h = 137;
  translate([0, 0, -h])
    resize([5, 40, h])
      cylinder(d=5, h=h, $fn=fn(100));
}


module rudder()
{
  rotate([0, 180, 0])
  {
    rudder_hold();
    rudder_body();
  }
}


rudder();
