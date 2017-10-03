
// Abdeckscheibe
q_l = 85;  // Breite/Höhe Quadrat

// Befestigungsscheibe
q_l_b = 70;

// OLED 1:1
inner_b_ol = 26; // Breite innerer Ausschnitt (Display)
inner_h_ol = 15; // Höhe innerer Ausschnitt (Display)

// Touchfläche mitte (Größer als OLED wg. Usability)
inner_b = 32; // Breite innerer Ausschnitt (Display)
inner_h = 27; // Höhe innerer Ausschnitt (Display)

//LED_Ring
ring_i = 54;
ring_o = 66;

// #### einfache Elemente ####

module lasche(b, h) {    
    translate([-b/2,0,0]) square([b,h]);
}

module trapez(l1, l2, b) {
    diff = (l2-l1)/2;
    translate([-(l1/2),0,0]) polygon([[0,0],[l1,0],[l2-diff,b],[-diff,b]]);
}

module segment(l1, h) {
    translate([0,0.8,0]) {
        trapez(l1,q_l_b-1.0,h-0.8);
        lasche_l = 7;
        translate([0,h-0.8,0]) lasche(q_l_b-8, lasche_l);
    }
}

module lasche_durchlass(l1,h) {
    lasche_l = 1.8;
    translate([0,h-(lasche_l),0]) lasche(q_l_b-7.9,lasche_l);    
}


// ####################################
// Abgeleitete Konstanten - nicht verändern
seg_h_lr = (q_l_b - inner_b) / 2; // Segmenthöhe (links/rechts)
seg_h_ou = (q_l_b - inner_h) / 2; // Segmenthöhe (oben/unten)

// Foliensegmente
module all_segments() {    
    translate([0,inner_h/2,0]) segment(inner_b, seg_h_ou);
    rotate([0,0,180]) translate([0,inner_h/2,0]) segment(inner_b, seg_h_ou);
    rotate([0,0,90]) translate([0,inner_b/2,0]) segment(inner_h, seg_h_lr);
    rotate([0,0,270])  translate([0,inner_b/2,0]) segment(inner_h, seg_h_lr);
}

// Ausschnitte für Laschen der Folie
module all_straps() {
    translate([0,inner_h/2,0]) lasche_durchlass(inner_b, seg_h_ou);
    rotate([0,0,180]) translate([0,inner_h/2,0]) lasche_durchlass(inner_b, seg_h_ou);

    rotate([0,0,90]) translate([0,inner_b/2,0]) lasche_durchlass(inner_h, seg_h_lr);
    rotate([0,0,270])  translate([0,inner_b/2,0]) lasche_durchlass(inner_h, seg_h_lr);
}

module draw_acryl_plane() {
    // Umriss
    difference() {
        square(q_l_b+5, center=true);
        all_straps();
        square([inner_b, inner_h], center=true);
    }
} 

module display() {
    union() {
        color("blue") linear_extrude(2.7) square([inner_b, inner_h], center=true);
        color("black") translate([0,0,2.7]) linear_extrude(0.3) square([inner_b_ol, inner_h_ol], center=true);
    }
    
}

module display_cut() {
    color("white",0.1) linear_extrude(3.1) square([inner_b_ol, inner_h_ol], center=true);
}

module led_ring() {
    linear_extrude(2.5) difference() {
        circle(d=ring_o);
        circle(d=ring_i);
    }
}


module complete() {
    union() {
        // Display
        translate([0,0,0]) display(); 

        // Abdeckung
        difference() {
            translate([0,0,3.1]) color("white", 0.7) linear_extrude(3) square(q_l, center=true);
            translate([0,0,3.05]) display_cut(); 
        }

        // Folie
        translate([0,0,3]) color("white", 0.2) linear_extrude(0.1) color("white", 0.2)  all_segments();
        
        // Haltplatte (mit Löchern)
        color("white", 0.5) difference() {
            linear_extrude(3) draw_acryl_plane();
            if (draw_2d == true) translate([0,0,-20]) import("/home/ian/Coding/Making/Wandthermostat/Adapterring_Wand_LED.stl");
        }


        if (draw_2d != true) {
            color("gray") translate([0,0,-20]) import("/home/ian/Coding/Making/Wandthermostat/Adapterring_Wand_LED.stl");            
        }

    }    
}

module 2d_cuts() {
  union() {   
    //projection(cut=true) complete();
    translate([100,100,0]) projection(cut=true) translate([0,0,-0.5]) complete();
    translate([100,0,0]) projection(cut=true) translate([0,0,-3.05]) complete();
    translate([0,0,0]) projection(cut=true) translate([0,0,-5.5]) complete();
    translate([0,100,0]) projection(cut=true) translate([0,0,0]) display_cut();
  }
}

draw_2d = false;
draw_2d = true;


if (draw_2d == true) {
    color("red") 2d_cuts();
} else {
    complete();
}

