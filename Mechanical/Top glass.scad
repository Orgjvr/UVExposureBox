x=310;
y=140;
h=5;

hole_d=3;
head_d=7;
strip_w=10;
strip_from_edge=10;;
dist_from_edge=7;



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

module alignment_holes(mh_d,xy_from_edge,y,x,h,pos=[0,0,0],orientation=[0,0,0])
{
  offset=40;
  translate(pos) rotate(orientation) 
  {  
    translate([x/2-xy_from_edge,offset,-0.1]) rotate ([0,0,0]) cylinder(h+0.2,mh_d/2,mh_d/2);
    translate([-x/2+xy_from_edge,offset,-0.1]) rotate ([0,0,0]) cylinder(h+0.2,mh_d/2,mh_d/2);
    translate([x/2-xy_from_edge,-offset,-0.1]) rotate ([0,0,0]) cylinder(h+0.2,mh_d/2,mh_d/2);
    translate([-x/2+xy_from_edge,-offset,-0.1]) rotate ([0,0,0]) cylinder(h+0.2,mh_d/2,mh_d/2);
  }
}

module handle_holes(mh_d,xy_from_edge,y,x,h,pos=[0,0,0],orientation=[0,0,0])
{
  translate(pos) rotate(orientation) 
  {  
    translate([x/2-xy_from_edge,0,-0.1]) rotate ([0,0,0]) cylinder(h+0.2,mh_d/2,mh_d/2);
    translate([-x/2+xy_from_edge,0,-0.1]) rotate ([0,0,0]) cylinder(h+0.2,mh_d/2,mh_d/2);
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
      translate([0,(s_w+s_s)*i,0]) color("grey") cube([s_l,s_s,s_h]);
    }
  }
}

module Bottom_Glass()
{
  difference()
  {
    translate([-x/2,-y/2,0]) cube([x,y,h]);
    mount_holes(hole_d, dist_from_edge, y,x,h);
    handle_holes(head_d, dist_from_edge, y,x,h);
    alignment_holes(hole_d, dist_from_edge, y,x,h);
  }
//  strip(10,y,1,[-x/2+strip_from_edge,-y/2,h]);
//  strip(10,y,1,[x/2-strip_from_edge-10,-y/2,h]);
}

module Top_Glass()
{
  difference()
  {
    translate([-x/2,-y/2,0]) cube([x,y,h]);
    mount_holes(head_d, dist_from_edge, y,x,h);
    handle_holes(hole_d, dist_from_edge, y,x,h);
    alignment_holes(head_d, dist_from_edge, y,x,h);
  }
//  strip(10,y,1,[-x/2+strip_from_edge,-y/2,h]);
//  strip(10,y,1,[x/2-strip_from_edge-10,-y/2,h]);
}

//projection(cut = false) Bottom_Glass();
//Bottom_Glass();

projection(cut = false) Top_Glass();
//translate([0,0,50])Top_Glass();