/* This is a cutout for a Magnet Pad Connector, 3 Pin, 2.54 Pitch.

   Includes the breakout pcb.

*/

/* ************************* */

$fn=50;

friction_fit = 0.02;
sliding_fit = 2;

/* ************************* */

//adjustments after printing once
print_adj_y = 1;
print_adj_flange_z = 0.2;
print_adj_flange_y = 1;


pogo_overall_y = 15 + friction_fit;
pogo_overall_x = 4 + friction_fit + print_adj_y;
pogo_overall_z = 4 + friction_fit;

flange_z = 0.8 + friction_fit + print_adj_flange_z;
flange_z_offset = 0.2;

edge_radius = 2;

pcb_x = 10.2 + sliding_fit;
pcb_y = pogo_overall_y; //9.9 actual
pcb_z = 1.6  + friction_fit;

connector_z_offset = pogo_overall_z/2 + pcb_z/2;
connector_x_offset = 2.54;

pogo_and_pcb_z_height = pcb_z + pogo_overall_z;

solder_connection_allowable_z = pogo_overall_z-1;
solder_connection_x = 2.54 * 1.2;

/* ************************* */

draw_pogo_magnet_and_pcb();

/* ************************* */

module draw_pogo_magnet_and_pcb(big_pcb=false)
{
    translate([0,0,pogo_overall_z/2+pcb_z])
    {
        spring_magnet();
        draw_pcb(big_pcb);
    }
}

module draw_pcb(big_pcb=false)
{
    translate([connector_x_offset, 0, -connector_z_offset + 0.01]) 
    {
    cube([pcb_x, pcb_y, pcb_z], center = true);
    
    if (big_pcb==false) translate([2.54,0,solder_connection_allowable_z/2]) cube([solder_connection_x,pcb_y,solder_connection_allowable_z], center = true);
    else translate([0,0,solder_connection_allowable_z/2]) cube([pcb_x,pcb_y,solder_connection_allowable_z+0.2], center = true);
    }
}

module spring_magnet()
{
    //flange
    translate([0,0,-0.2]) cube([pogo_overall_x,pogo_overall_y+print_adj_flange_y,flange_z], center=true);
    
    //body
    translate([0,pogo_overall_y/2-edge_radius,-pogo_overall_z/2]) cylinder(r=edge_radius, h=pogo_overall_z);
    translate([0,-pogo_overall_y/2+edge_radius,-pogo_overall_z/2]) cylinder(r=edge_radius, h=pogo_overall_z);
    translate([0,0,0]) cube([4,15-4,pogo_overall_z], center=true);
}