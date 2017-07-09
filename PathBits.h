#ifndef __PATH_BITS_H
#define __PATH_BITS_H

#include "Vec.h"

namespace AmrReconstruction3
{
  // la seule coherence a verifier est celle entre PathBits<>::fromPath() et GridInfo<>::branch()

  // transformation d'un empilement de booleens en serie de bits
  template<unsigned int D> struct PathBits;
  template<> struct PathBits<0>
  {
    static inline unsigned int fromPath(Vec<bool,0>) {return 0;}
  };
  template<unsigned int D> struct PathBits
  {
    static inline unsigned int fromPath(const Vec<bool,D>& path)
    {
      //return ( PathBits<D-1>::fromPath(path) << 1 ) | ( path.val ? 1 : 0 ) ;
      return ( path.val ? (1<<(D-1)) : 0 ) | PathBits<D-1>::fromPath(path);
    }
  };

}; // namespace AmrReconstruction3

#endif
/* ===================================================================================================================*/


#ifdef _TEST_PathBits

#include <iostream>
using namespace std;

using namespace AmrReconstruction3;

template <typename T, unsigned int D>
inline ostream & operator << (ostream& out, const Vec<T,D>& v)
{
  v.toStream(out);
  return out;
}

int main()
{
  Vec<double,3> center(0.5);
  Vec<double,3> p( 0.7 , Vec<double,2>(0.1) );
  Vec<bool,3> path;

  path = p > center;
  cout<<"Center = "<<center<<endl;
  cout<<"Point = "<<p<<endl;
  cout<<"Path = "<<path<<endl;
  cout<<"Path bits = "<<PathBits<3>::fromPath(path)<<endl;

  Vec<bool,4> _1011( true, Vec<bool,3>(false, Vec<bool,2>(true,Vec<bool,1>(true))));
  _1011.toStream(cout); cout<<" = "<< PathBits<4>::fromPath(_1011)<<endl;

  return 0;
}

#endif
