/* Cortana Data Chip w/ Circuits

Author: Dustin Westaby

This cortana data chip uses a Neopixel jewel, arduino trinket, and a female USB socket.
*/
/* ***************************************************** */
scale_multiplier = 1.25;

sliding_fit = 2;
friction_fit = 0.2;

/* ***************************************************** */
overall_height = 58.57;
overall_width  = 33.37;
overall_depth  = 5.1;

center_diam = 22.8;

neo_ring_diam = 22.9 + friction_fit;
neo_ring_height = 0.8 + friction_fit;

/* ***************************************************** */
draw_shells();
//draw_center_ring(neo_ring_diam, overall_depth-neo_ring_height);

/* ***************************************************** */
module draw_usb_socket()
{
    overhang_x = 12;
    usb_male_x = 14.83 + friction_fit + overhang_x;
    usb_male_y = 12    + friction_fit;
    usb_male_z = 4.55  + friction_fit;
    
    usb_inset_holes_diameter = 2.67 - friction_fit;
    usb_inset_hole_spacing = 3;
    usb_inset_hole_edge_offset = 2.5;
    usb_inset_hole_height = 1.2;
    
    //this is a male socket
    difference()
    {
        //outline
        union()
        {
            //connector
            translate([-overhang_x/2,0,usb_male_z/2]) cube([usb_male_x, usb_male_y, usb_male_z], center=true);
            
            //wire channels
            translate([7.5,-(usb_male_y-5)/2,1.75]) cube([4,usb_male_y-5,1.5]);
        }
        
        //holes
        translate([
        usb_inset_hole_edge_offset+usb_inset_holes_diameter/2,
        usb_inset_hole_spacing,
        -0.1]) cylinder($fn=100, d=usb_inset_holes_diameter, h=usb_inset_hole_height);
        
        translate([
        usb_inset_hole_edge_offset+usb_inset_holes_diameter/2,
        -usb_inset_hole_spacing,
        -0.1]) cylinder($fn=100, d=usb_inset_holes_diameter, h=usb_inset_hole_height);
    }
    
}

module draw_center_ring(d, z)
{
    translate([0,0.25,z]) cylinder($fn=100, d=neo_ring_diam, h=neo_ring_height);
}

module draw_shells()
{
    difference()
    {
        union()
        {
            translate([0,0,overall_depth]) rotate([0,180,0]) scale(scale_multiplier) import("Cortana_Chip_Assembly_-_Cortana_Chip_Top-2.STL");
            //translate([0,0.32,overall_depth]) rotate([0,0,0]) scale(scale_multiplier) import("Cortana_Chip_Assembly_-_Cortana_Chip_Bottom.STL");
        }
        
        //neoring
        draw_center_ring(neo_ring_diam, overall_depth-neo_ring_height);
        
        //usb
        translate([0,-20-2.2,3]) rotate([0,0,90]) draw_usb_socket();
    }
}

