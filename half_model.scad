cherry=14.0;
cherrylip=1.8;
cherryspace=1.8;

capclearh=20;
capclearw=15;

$fn=50;
knuckLen = 100;
keyl = 19.2;
keyd = cherrylip + 6;
transl=(keyl-cherry)/2;
transw=(keyl-cherry)/2;
wald=0.6;
mod1=0.3;

module support(){
    rotate([0,90,0])
        difference(){
            cylinder(r=3.7,h=4);
            cylinder(r=3.4,h=4);
        }
}
module key(){
    difference(){
        union(){
            /* cube([keyd,keyl,keyl]); */

            minkowski(){
                cube([keyd,keyl-(2*cherryspace)-2*mod1,keyl-(2*cherryspace)-2*mod1]);
                rotate([0,90,0])
                    translate([-cherryspace-mod1,cherryspace+mod1,0])
                    cylinder(r=cherryspace+mod1,h=1);
            }
            //keycap simulator
            //translate([-capclearh,transl/2+cherrylip*2,transw]){
            //      cube([capclearh,capclearw,capclearw]);
            //}
            //end keycap
        }
        translate([0,transl,transw]){
            cube([keyd,cherry,cherry]);
            translate([cherrylip,-cherryspace/2-wald/2-mod1/2,-cherryspace/2-wald/2-mod1/2]){
                minkowski(){
                    cube([keyd,cherry+wald+mod1,cherry+wald+mod1]);
                    rotate([0,90,0])
                        translate([-cherryspace/2,cherryspace/2,0])
                        cylinder(r=cherryspace-mod1,h=keyd);
                }
            }
        }
    }
    difference(){
        union(){
            support();
            translate([0,keyl,0])
                support();
            translate([0,0,keyl])
                support();
            translate([0,keyl,keyl])
                support();
        }
        difference(){
            translate([-2 ,-10,-10])
                cube([keyd,keyl+20,keyl+20]);
            cube([keyd+3,keyl,keyl]);
        }
    }
}

rows=4;
reduce=0.7;
r=[7,6,6,6];
module row(columns){
    for ( i = [0 : columns-1] ){
        translate([0, i*(keyl-reduce), 0])
            key();
    }
}

rotate([0,-90,0])
    for ( i = [0 : rows-1] ){
        translate([0, 0,i*(keyl-reduce)])
            row(r[i]);
    }
/* key(); */
