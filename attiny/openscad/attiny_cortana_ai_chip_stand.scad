
/* ************************************************************* */
// Some easy constants
scale_multiplier = 1.25;//1.25; //match with cortana print

friction_fit = 0.2;
sliding_fit  = 2;

nozzle_diameter = 0.4;

$fn = 500;

shell_thickness = 2;
slot_depth = 4;

/* ************************************************************* */
// Measurements

slot_x = 26.76 + sliding_fit;
slot_y = 6 + sliding_fit;
slot_round_diameter = slot_y;

insert_x = 60;
insert_y = 60.66;
insert_height = 23.27;

base_diameter = slot_x + 14;
base_height = slot_depth + (17 / scale_multiplier);

inner_diameter = base_diameter - shell_thickness*2;

ridge_height = 4;
ridge_chamfer_diameter = base_diameter - 3;

outer_ridge_diameter = ridge_chamfer_diameter - 7;
inner_ridge_diameter = outer_ridge_diameter   - 3;

connector_x = (31.9 + friction_fit) / scale_multiplier;
connector_y = (9.07 + friction_fit) / scale_multiplier;

switch_hole_diameter = 12 / scale_multiplier;

/* ************************************************************* */
// Draw What?

//translate([-insert_x/2,-insert_y/2-1.7,0]) import("BackFanInsert_r1_3.stl");

draw_upper_shell();

draw_lower_shell();

//draw_cortana_reference();

//draw_ai_slot_insert();

//cord_hole("top");

/* ************************************************************* */
// Draw Parts


module cord_hole(section="bottom")
{
    translate([0,outer_ridge_diameter,ridge_height/2])
    {
        if (section=="top")
        {
            //widen
        translate([0,-10,-ridge_height/2])
            cube([4,10,4], center=true);
        }
        
        //slot
        rotate([90,0,0])
        cylinder(d=4, h=10);
    }
}
module draw_trinket()
{
    trinket_width = 27.1 + friction_fit;
    trinket_height = 15.36 + friction_fit;
    trinket_depth = 1.72 + friction_fit;
    trinket_inset = 4;

    usb_width  = 7.92 + friction_fit;
    usb_height = 8.37 + sliding_fit;
    usb_depth  = 3.4 + friction_fit;
    
    trinket_to_wall_thickness = nozzle_diameter * 2;

    rotate([0,0,90])
    translate([-trinket_to_wall_thickness,-trinket_height/2,-trinket_depth/2])
    union()
    {
        cube([trinket_width,trinket_height,trinket_depth]);
        translate([trinket_width-usb_height,trinket_height/2-usb_width/2,0]) cube([usb_height+trinket_inset,usb_width,usb_depth]);
    }
}

module draw_lower_shell()
{
    scale(scale_multiplier)
    union()
    {
        //bottom
        difference()
        {
            translate([0,0,0]) cylinder (h = shell_thickness * 2, d1=base_diameter + shell_thickness*2, d2=base_diameter + shell_thickness, center = true);
            translate([0,0,shell_thickness/2 + 0.01]) cylinder (h = shell_thickness, d=base_diameter + friction_fit, center = true);
            
            //back port
            scale(1 / scale_multiplier) cord_hole(section="bottom");
        }
        
        //cool details
        //TBD
        
        //trinket capture ledge
        //capture_height = 0.5;
        //scale(1 / scale_multiplier) translate([0,0,capture_height/2]) cube([18,2,capture_height], center=true);
        
        
    }
}

module draw_ai_slot_insert()
{
    z_offset = base_height-slot_depth/2 + shell_thickness;
    
    usb_rim_depth = 0.78 + friction_fit;
    slot_depth = (0.5 + usb_rim_depth) / scale_multiplier;
    
    slot_x_new = slot_x - friction_fit;
    slot_y_new = slot_y - friction_fit;
    slot_round_diameter_new = slot_round_diameter - friction_fit;
    
    usb_rim_x = (14.37+friction_fit)/scale_multiplier;
    usb_rim_y = (6.58+friction_fit)/scale_multiplier;
    
    scale(scale_multiplier)
    difference()
    {
        //slot
        union()
        {
            translate([0,0,z_offset]) cube([slot_x_new - slot_round_diameter_new, slot_y_new, slot_depth + 0.1], center=true);
            translate([-slot_x_new/2+slot_round_diameter_new/2,0,z_offset]) cylinder (h = slot_depth + 0.1, d=slot_round_diameter_new, center = true);
            translate([+slot_x_new/2-slot_round_diameter_new/2,0,z_offset]) cylinder (h = slot_depth + 0.1, d=slot_round_diameter_new, center = true);
        }
    
        //usb port
        translate([0,0,z_offset])  cube([connector_x, connector_y, slot_depth + 0.1 +0.01], center = true);
        
        //usb rim
        translate([0,0,z_offset+usb_rim_depth])  cube([usb_rim_x, usb_rim_y, slot_depth + 0.1 +0.01], center = true);
    }
        
}

module draw_ai_slot()
{
    z_offset = base_height-slot_depth/2 + shell_thickness;
    
    //slot
    translate([0,0,z_offset]) cube([slot_x - slot_round_diameter, slot_y, slot_depth + 0.1], center=true);
    translate([-slot_x/2+slot_round_diameter/2,0,z_offset]) cylinder (h = slot_depth + 0.1, d=slot_round_diameter, center = true);
    translate([+slot_x/2-slot_round_diameter/2,0,z_offset]) cylinder (h = slot_depth + 0.1, d=slot_round_diameter, center = true);
    
    //usb port
    translate([0,0,z_offset - shell_thickness])  cube([connector_x, connector_y, slot_depth + 0.1], center = true);
}

module draw_cortana_reference()
{
    translate([0,0,32]) rotate([90,0,0]) 
    union()
    {
        translate([0,0,0]) rotate([0,180,0]) import("Cortana_Chip_Assembly_-_Cortana_Chip_Top-2.STL");
        translate([0,0.5,0]) rotate([0,0,0]) import("Cortana_Chip_Assembly_-_Cortana_Chip_Bottom.STL");
    }
}

module draw_switch_cyl(diameter)
{
    hole_depth = shell_thickness*2;
    
    //switch_hole_diameter
    translate([0,-inner_diameter/2+hole_depth/2-0.01,base_height/2]) rotate([90,0,0]) cylinder(h=hole_depth, d=diameter);
}

module draw_switch_outline()
{
    switch_outline_pad = 2.25;
    draw_switch_cyl(switch_hole_diameter+switch_outline_pad);
}

module draw_switch_holes()
{
    translate([0,-0.01,0]) draw_switch_cyl(switch_hole_diameter);
    translate([0,2,0]) draw_switch_cyl(switch_hole_diameter+4);
}

module draw_upper_shell()
{
    
    scale(scale_multiplier)
    difference()
    {
        union()
        {
            draw_base();
            draw_switch_outline();
        }
        
        //bottom cutaway
        translate([0,0,base_height/2 - slot_depth]) cylinder (h = base_height + 0.2, d=inner_diameter, center = true);
        
        //ai slot
        draw_ai_slot();
        
        //back slot
        translate([0,0,-0.01]) scale(1 / scale_multiplier) cord_hole(section="top");
        
        //switch hole
        draw_switch_holes();
    }
    
        //scale(scale_multiplier) draw_switch_hole();
    
        //flatten side
    
        // button
}

module draw_base()
{
    //base
    translate([0,0,base_height/2]) cylinder (h = base_height, d=base_diameter, center = true);
    //ridge
    difference()
    {
        translate([0,0,base_height + ridge_height/2]) cylinder (h = ridge_height, d1=base_diameter, d2=ridge_chamfer_diameter, center = true);
        
        translate([0,0,base_height + ridge_height/2 + 0.1]) cylinder (h = ridge_height, d1=inner_ridge_diameter, d2=outer_ridge_diameter, center = true);
    }
}
