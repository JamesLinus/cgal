// ======================================================================
//
// Copyright (c) 2002 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
//
// release       : $CGAL_Revision: CGAL-2.5-I-99 $
// release_date  : $CGAL_Date: 2003/05/23 $
//
// file          : include/CGAL/Manhattan_distance_iso_box_point.h
// package       : ASPAS (3.12)
// maintainer    : Hans Tangelder <hanst@cs.uu.nl>
// revision      : 3.0
// revision_date : 2003/07/10 
// authors       : Hans Tangelder (<hanst@cs.uu.nl>)
// coordinator   : Utrecht University
//
// ======================================================================


#ifndef CGAL_MANHATTAN_DISTANCE_ISO_BOX_POINT_H
#define CGAL_MANHATTAN_DISTANCE_ISO_BOX_POINT_H

#include <CGAL/Kd_tree_rectangle.h>

namespace CGAL {

  template <class SearchTraits, class QueryItem>
  class Manhattan_distance_iso_box_point {

    public:

    typedef typename SearchTraits::Point_d Point_d;
    typedef typename SearchTraits::FT    FT;
    typedef QueryItem                  Query_item;

    
    Manhattan_distance_iso_box_point() {}
      
    
    // obsolete as we no longer store dimension Manhattan_distance_iso_box_point(const int d) : the_dimension(d) {}

    //copy constructor
    Manhattan_distance_iso_box_point(const Manhattan_distance_iso_box_point& d) {}

    ~Manhattan_distance_iso_box_point() {}

    inline FT transformed_distance(const QueryItem& q, const Point_d& p) {
		FT distance = FT(0);
		typename SearchTraits::Construct_cartesian_const_iterator_d construct_it;
                typename SearchTraits::Cartesian_const_iterator_d qmaxit = construct_it(q.max()),
		  qe = construct_it(q.max(),1), qminit = construct_it(q.min()),
		  pit = construct_it(p);
		for (; qmaxit != qe; ++qmaxit) {
			if ((*pit)>(*qmaxit)) distance += 
			(*pit)-(*qmaxit); 
			else if ((*pit)<(*qminit)) distance += 
			(*qminit)-(*pit);	
		}
        	return distance;
    }


    inline FT min_distance_to_rectangle(const QueryItem& q,
					      const Kd_tree_rectangle<SearchTraits>& r) {
		FT distance = FT(0);
		typename SearchTraits::Construct_cartesian_const_iterator_d construct_it;
                typename SearchTraits::Cartesian_const_iterator_d qmaxit = construct_it(q.max()),
		  qe = construct_it(q.max(),1), qminit = construct_it(q.min());
		for (unsigned int i = 0; qmaxit != qe; ++ qmaxit, ++i)  {
			if (r.min_coord(i)>(*qmaxit)) 
			  distance +=(r.min_coord(i)-(*qmaxit)); 
			if (r.max_coord(i)<(*qminit)) 
			  distance += ((*qminit)-r.max_coord(i));
	        }
		return distance;
	}

    inline 
    FT 
    max_distance_to_rectangle(const QueryItem& q,
			      const Kd_tree_rectangle<SearchTraits>& r) {
      FT distance=FT(0);
      typename SearchTraits::Construct_cartesian_const_iterator_d construct_it;
      typename SearchTraits::Cartesian_const_iterator_d qmaxit = construct_it(q.max()),
	qe = construct_it(q.max(),1), qminit = construct_it(q.min());
      for (unsigned int i = 0; qmaxit != qe; ++ qmaxit, ++i)  {
	if ( r.max_coord(i)-(*qminit) >(*qmaxit)-r.min_coord(i) )  
	  distance += (r.max_coord(i)-(*qminit));
	else 
	  distance += ((*qmaxit)-r.min_coord(i));
      }
      return distance;
    }
	
  inline 
  FT 
  transformed_distance(FT d) 
  {
    return d;
  }

  inline 
  FT 
  inverse_of_transformed_distance(FT d) 
  {
    return d;
  }

}; // class Manhattan_distance_iso_box_point

} // namespace CGAL
#endif // MANHATTAN_DISTANCE_ISO_BOX_POINT_H
