#include "Utils.h"
float to_mm(float v) {
    return v*0.0254f;
}
float to_mil(float v) {
    return v/0.0254f;
}
string get_grid_str(float grid) {
    //39.6875�m, 79.375�m, 158.75�m, ..., 5.08mm
    char mm[40];
    if(to_str(grid).size()>6) //if �m more compact
        sprintf(mm,"%g %s",grid*1000.0f,"um");
    else
        sprintf(mm,"%g %s",grid,"mm");
    return mm;
}
