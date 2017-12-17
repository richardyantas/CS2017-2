#ifndef UTILS_H
#define UTILS_H

namespace utils{

	float norm(point3 u, point3 v){
	  return sqrt( pow(u.x-v.x,2)+pow(u.y-v.y,2)+pow(u.z-v.z,2) );
	}


}


#endif