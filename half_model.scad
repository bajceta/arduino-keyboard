cherry=15;
cherrylip=1.5;
cherryspace=1.8;

capclearh=20;
capclearw=15;

$fn=50;
knuckLen = 100;
keyw= 19.7;
keyl = 19.7;
keyd = cherrylip + 6;
transl=(keyl-cherry)/2;
transw=(keyw-cherry)/2;
wald=0.6;

module key(){
    difference(){
        union(){
            cube([keyd,keyl,keyw]);
            //keycap simulator
            //translate([-capclearh,transl/2+cherrylip*2,transw]){
            //      cube([capclearh,capclearw,capclearw]);
            //}
            //end keycap
        }
        translate([0,transl,transw]){
            cube([keyd,cherry,cherry]);
            translate([cherrylip,-cherryspace/2-wald/2,-cherryspace/2-wald/2]){
                minkowski(){
                    cube([keyd,cherry+wald,cherry+wald]);
                    rotate([0,90,0])
                        translate([-cherryspace/2,cherryspace/2,0])
                        cylinder(r=cherryspace,h=keyd);
                }
            }
        }
    }
}

columns=6;
rows=4;
reduce=0;
module row(){
    for ( i = [0 : columns-1] ){
        translate([0, i*(keyl-reduce), 0])
            key();
    }
}
rotate([0,-90,0])
for ( i = [0 : rows-1] ){
    translate([0, 0,i*(keyl-reduce)])
        row();
}

