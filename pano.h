#ifndef PANOCLASS_H
#define PANOCLASS_H

#include <vector>
#include <iostream>
#include <string.h>

using namespace std;

class Panorama{

public:
int exec(vector<string> paths, string save_path);
void do_pano();
};

#endif
