$fn=40;
n_elements = 6;
thickness = 5;

ring_inner_radius = 15;
// ###

n_angle = 360 / n_elements;


// ### Grundelemente

module spiral(start_angle, end_angle) {
    
    r = ring_inner_radius;
    function spiral(r, t) = let(r = (r + thickness * (t / n_angle))) [r * sin(t), r * cos(t)];
    
    inner = [for(t = [start_angle : end_angle]) spiral(r - thickness, t) ];
    
    outer = [for(t = [end_angle : -1 : start_angle]) spiral(r, t) ];

    polygon(concat(inner, outer));
    
}


module ring(radius,t) {
    difference() {
        circle(radius+t);
        circle(radius);        
    }
}



difference() {
 ring(ring_inner_radius-0.01, thickness+0.02);   
    intersection() {
        ring(ring_inner_radius,thickness);
        for (an = [0 : n_angle*2 : 360 ]) {
            #rotate([0,0,an]) spiral(0,n_angle*2);            
        }
    }
}