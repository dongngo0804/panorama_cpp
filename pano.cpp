#include "pano.h"
#include "panoopencv.cpp"

void Panorama::do_pano(){
        vector<string> path;
        path.push_back("/home/dongnh/Pictures/1.jpg");
        path.push_back("/home/dongnh/Pictures/2.jpg");
        exec(path,"/home/dongnh/Pictures/panoed.jpg");
}
