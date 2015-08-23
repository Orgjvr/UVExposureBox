x=310;
y=140;
h=5;

hole_d=3;
strip_w=10;
strip_from_edge=2.5;
dist_from_edge=strip_w*1.5+strip_from_edge;



module mount_holes(mh_d,xy_from_edge,y,x,h,pos=[0,0,0],orientation=[0,0,0])
{
  translate(pos) rotate(orientation) 
  {  
    translate([x/2-xy_from_edge,y/2-xy_from_edge,-0.1]) rotate ([0,0,0]) cylinder(h+0.2,mh_d/2,mh_d/2);
    translate([-x/2+xy_from_edge,y/2-xy_from_edge,-0.1]) rotate ([0,0,0]) cylinder(h+0.2,mh_d/2,mh_d/2);
    translate([x/2-xy_from_edge,-y/2+xy_from_edge,-0.1]) rotate ([0,0,0]) cylinder(h+0.2,mh_d/2,mh_d/2);
    translate([-x/2+xy_from_edge,-y/2+xy_from_edge,-0.1]) rotate ([0,0,0]) cylinder(h+0.2,mh_d/2,mh_d/2);
  }
}

module strip(s_l,s_s,s_c,pos=[0,0,0],orientation=[0,0,0])
{
  s_w=10;
  s_h=2;
  translate(pos) rotate(orientation) 
  {
    for (i = [0:s_c-1]) 
    {  
      translate([0,(s_w+s_s)*i,0]) cube([s_l,s_w,s_h]);
    }
  }
}

module Bottom_Panel()
{
  difference()
  {
    translate([-x/2,-y/2,0]) cube([x,y,h]);
    mount_holes(hole_d, dist_from_edge, y,x,h);
  }
  strip(x,10,7,[-x/2,-y/2+strip_from_edge,h]);
}

projection(cut = false) Bottom_Panel();
//Bottom_Panel();