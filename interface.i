%module pano;
%{
#include "pano.h"
%}

%include "std_vector.i"
%include "std_string.i"
namespace std
{
  %template(Vectord) vector<string>;
};

%inline%{
  std::vector<string> string_vector;
%}

%include "pano.h"
