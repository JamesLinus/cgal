// ============================================================================
//
// Copyright (c) 1999, 2000 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------------
//
// release       : $CGAL_Revision $
// release_date  : $CGAL_Date $
//
// file          : min_quadrilateral_2.h
// chapter       : $CGAL_Chapter: Geometric Optimisation $
// package       : $CGAL_Package: Min_quadrilaterals $
// source        : oops.aw
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Michael Hoffmann <hoffmann@inf.ethz.ch> and
//                 Emo Welzl <emo@inf.ethz.ch>
//
// maintainer    : Michael Hoffmann <hoffmann@inf.ethz.ch>
// coordinator   : ETH
//
// Computing minimum enclosing quadrilaterals of a convex point set
// ============================================================================

#if ! (CGAL_MIN_QUADRILATERAL_2_H)
#define CGAL_MIN_QUADRILATERAL_2_H 1

#include <CGAL/basic.h>
#include <CGAL/Optimisation/assertions.h>
#include <CGAL/functional.h>
#include <CGAL/function_objects.h>
#include <iterator>

#ifdef CGAL_OPTIMISATION_EXPENSIVE_PRECONDITION_TAG
#include <CGAL/Polygon_2_algorithms.h>
#endif



CGAL_BEGIN_NAMESPACE

template < class ForwardIterator, class OutputIterator, class Traits >
OutputIterator
convex_bounding_box_2(
  ForwardIterator f, ForwardIterator l, OutputIterator o, Traits& t)
// PRE:
//   * f != l
//   * value type of ForwardIterator is Traits::Point_2
//   * [f,l) form a the vertices of a convex polygon
//     oriented counterclockwise
//   * OutputIterator accepts ForwardIterator as value type
// POST:
//   writes to o iterators from [f,l) referring to the last points with
//    - smallest y coordinate
//    - largest x coordinate
//    - largest y coordinate
//    - smallest x coordinate
//   in that order.
{
  CGAL_precondition(f != l);

  // make sure that we have two distinct points, such that it
  // can be determined in which quadrant of the polygon we are
  ForwardIterator first;
  do {
    first = f;
    // catch the one-element case:
    if (++f == l) {
      f = first;
      break;
    }
  } while (t.equal_2_object()(*first, *f));

  // Four extremes
  ForwardIterator minx = first;
  ForwardIterator maxx;
  ForwardIterator miny;
  ForwardIterator maxy;

  typedef typename Traits::Less_xy_2        Less_xy_2;
  typedef typename Traits::Less_yx_2        Less_yx_2;
  typedef typename Swap<Less_xy_2,1>::Type  Greater_xy_2;
  typedef typename Swap<Less_yx_2,1>::Type  Greater_yx_2;

  Less_xy_2    less_xy_2    = t.less_xy_2_object();
  Less_yx_2    less_yx_2    = t.less_yx_2_object();
  Greater_xy_2 greater_xy_2 = swap_1(less_xy_2);
  Greater_yx_2 greater_yx_2 = swap_1(less_yx_2);

  if (less_xy_2(*minx, *f) ||
      less_yx_2(*minx, *f) && !less_xy_2(*f, *minx))
    if (less_yx_2(*minx, *f))
      // first quadrant
      for (;;) {
        maxx = f;
        if (++f == l) {
          maxy = minx = miny = maxx;
          break;
        }
        if (less_xy_2(*f, *maxx)) {
          f = maxx;
          for (;;) {
            maxy = f;
            if (++f == l) {
              minx = miny = maxy;
              break;
            }
            if (less_yx_2(*f, *maxy)) {
              f = maxy;
              for (;;) {
                minx = f;
                if (++f == l) {
                  miny = minx;
                  break;
                }
                if (greater_xy_2(*f, *minx)) {
                  f = minx;
                  do
                    miny = f;
                  while (++f != l && !greater_yx_2(*f, *miny));
                  break;
                }
              } // for (;;)
              break;
            } // if (less_yx_2(*f, *maxy))
          } // for (;;)
          break;
        } // if (less_xy_2(*f, *maxx))
      } // for (;;)
    else
      // fourth quadrant
      for (;;) {
        miny = f;
        if (++f == l) {
          maxx = maxy = minx = miny;
          break;
        }
        if (greater_yx_2(*f, *miny)) {
          f = miny;
          for (;;) {
            maxx = f;
            if (++f == l) {
              maxy = minx = maxx;
              break;
            }
            if (less_xy_2(*f, *maxx)) {
              f = maxx;
              for (;;) {
                maxy = f;
                if (++f == l) {
                  minx = maxy;
                  break;
                }
                if (less_yx_2(*f, *maxy)) {
                  f = maxy;
                  do
                    minx = f;
                  while (++f != l && !greater_xy_2(*f, *minx));
                  break;
                }
              } // for (;;)
              break;
            } // if (less_xy_2(*f, *maxx))
          } // for (;;)
          break;
        } // if (greater_yx_2(*f, *miny))
      } // for (;;)
  else
    if (less_yx_2(*f, *minx))
      // third quadrant
      for (;;) {
        minx = f;
        if (++f == l) {
          miny = maxx = maxy = minx;
          break;
        }
        if (greater_xy_2(*f, *minx)) {
          f = minx;
          for (;;) {
            miny = f;
            if (++f == l) {
              maxx = maxy = miny;
              break;
            }
            if (greater_yx_2(*f, *miny)) {
              f = miny;
              for (;;) {
                maxx = f;
                if (++f == l) {
                  maxy = maxx;
                  break;
                }
                if (less_xy_2(*f, *maxx)) {
                  f = maxx;
                  do
                    maxy = f;
                  while (++f != l && !less_yx_2(*f, *maxy));
                  break;
                }
              } // for (;;)
              break;
            } // if (greater_yx_2(*f, *miny))
          } // for (;;)
          break;
        } // if (greater_xy_2(*f, *minx))
      } // for (;;)
    else
      // second quadrant
      for (;;) {
        maxy = f;
        if (++f == l) {
          minx = miny = maxx = maxy;
          break;
        }
        if (less_yx_2(*f, *maxy)) {
          f = maxy;
          for (;;) {
            minx = f;
            if (++f == l) {
              miny = maxx = minx;
              break;
            }
            if (greater_xy_2(*f, *minx)) {
              f = minx;
              for (;;) {
                miny = f;
                if (++f == l) {
                  maxx = miny;
                  break;
                }
                if (greater_yx_2(*f, *miny)) {
                  f = miny;
                  do
                    maxx = f;
                  while (++f != l && !less_xy_2(*f, *maxx));
                  break;
                }
              } // for (;;)
              break;
            } // if (greater_xy_2(*f, *minx))
          } // for (;;)
          break;
        } // if (less_yx_2(*f, *maxy))
      } // for (;;)

  // Output
  *o++ = less_yx_2(*first, *miny) ? first : miny;
  *o++ = less_xy_2(*maxx, *first) ? first : maxx;
  *o++ = less_yx_2(*maxy, *first) ? first : maxy;
  *o++ = less_xy_2(*first, *minx) ? first : minx;
  return o;
} // convex_bounding_box_2(f, l, o, t)

namespace Optimisation {
  // Adds certain redundant functionality for convenience
  template < typename Traits >
  struct Min_quadrilateral_traits_wrapper : public Traits
  {
    typedef Traits                                      Base;
    // types inherited from Traits
    typedef typename Base::Point_2                      Point_2;
    typedef typename Base::Direction_2                  Direction_2;
    // predicates and constructions inherited from Traits
    typedef typename Base::Has_on_negative_side_2       HONS;
    typedef typename Base::Construct_vector_2           CV2;
    typedef typename Base::Construct_direction_2        CD2;
    typedef typename Base::Construct_line_2             Construct_line_2;
    typedef typename Base::Compare_angle_with_x_axis_2  CAWXA;

    Min_quadrilateral_traits_wrapper(const Traits& bt) : Base(bt) {}

    // ---------------------------------------------------------------
    // Right_of_implicit_line_2
    // ---------------------------------------------------------------
    typedef typename Swap<HONS,1>::Type SWHONS;
    typedef Identity<Point_2>           IDP;
    typedef typename Compose<SWHONS,IDP,Construct_line_2>::Type
      Right_of_implicit_line_2;
    
    Right_of_implicit_line_2 right_of_implicit_line_2_object() const {
      return compose(swap_1(has_on_negative_side_2_object()),
                     IDP(),
                     construct_line_2_object());
    }
    
    typedef typename Compose<CD2,CV2>::Type Construct_direction_2;
    
    Construct_direction_2 construct_direction_2_object() const {
      return compose(Base::construct_direction_2_object(),
                     construct_vector_2_object());
    }
    
    template < class Kernel >
    class Rdbmop
    : public CGAL_STD::binary_function< Direction_2, int, Direction_2 >
    {
      typename Kernel::Construct_perpendicular_vector_2   cperpvec;
      typename Kernel::Construct_vector_from_direction_2  cvec;
      typename Kernel::Construct_direction_2              dir;
      typename Kernel::Construct_opposite_direction_2     oppdir;
    public:
    
      Rdbmop() {}
    
      Rdbmop(const Kernel& k)
      : cperpvec(k.construct_perpendicular_vector_2_object()),
        cvec(k.construct_vector_from_direction_2_object()),
        dir(k.construct_direction_2_object()),
        oppdir(k.construct_opposite_direction_2_object())
      {}
    
      Direction_2
      operator()(const Direction_2& d, int i) const
      {
        // FIXME: here I would like to construct a vector from a
        // direction, but this is not in the kernel concept
        // maybe, we can get rid of directions soon...
        CGAL_precondition(i >= 0 && i < 4);
        if (i == 0) return d;
        if (i == 1) return dir(cperpvec(cvec(d), CLOCKWISE));
        if (i == 2) return oppdir(d);
        return dir(cperpvec(cvec(d), COUNTERCLOCKWISE));
      }
    };
    
    typedef Rdbmop<Traits> Rotate_direction_by_multiple_of_pi_2;
    
    Rotate_direction_by_multiple_of_pi_2
    rotate_direction_by_multiple_of_pi_2_object() const
    { return Rotate_direction_by_multiple_of_pi_2(*this); }
    
    typedef std::equal_to<Comparison_result>      EQCR;
    typedef Bind<EQCR,Comparison_result,2>::Type  BEQCR;
    typedef typename Compose<BEQCR,CAWXA>::Type   Less_angle_with_x_axis_2;
    
    Less_angle_with_x_axis_2 less_angle_with_x_axis_2_object() const {
      return compose(bind_2(EQCR(), SMALLER),
                     compare_angle_with_x_axis_2_object());
    }

  };
} // namespace Optimisation

template < class ForwardIterator, class OutputIterator, class BTraits >
OutputIterator
min_rectangle_2(
  ForwardIterator f,
  ForwardIterator l,
  OutputIterator o,
  BTraits& bt)
{
  typedef Optimisation::Min_quadrilateral_traits_wrapper<BTraits> Traits;
  Traits t(bt);
  CGAL_optimisation_expensive_precondition(is_convex_2(f, l));
  CGAL_optimisation_expensive_precondition(
    orientation_2(f, l) == COUNTERCLOCKWISE);

  // check for trivial cases
  if (f == l) return o;
  ForwardIterator tst = f;
  if (++tst == l) {
    // all points are equal
    for (int i = 0; i < 4; ++i) *o++ = *f;
    return o;
  }

  // types from the traits class
  typedef typename Traits::Rectangle_2            Rectangle_2;
  typedef typename Traits::Direction_2            Direction_2;
  typedef typename Traits::Construct_direction_2  Construct_direction_2;
  typedef typename Traits::Construct_rectangle_2  Construct_rectangle_2;

  Construct_direction_2 direction = t.construct_direction_2_object();
  Construct_rectangle_2 rectangle = t.construct_rectangle_2_object();
  typename Traits::Rotate_direction_by_multiple_of_pi_2
    rotate = t.rotate_direction_by_multiple_of_pi_2_object();
  typename Traits::Less_angle_with_x_axis_2
    less_angle = t.less_angle_with_x_axis_2_object();
  typename Traits::Area_less_rectangle_2
    area_less = t.area_less_rectangle_2_object();

  // quadruple of points defining the current rectangle
  ForwardIterator curr[4];
  // initialised to the points defining the bounding box
  convex_bounding_box_2(f, l, curr, t);

  // curr[i] can be advanced (cyclically) until it reaches limit[i]
  ForwardIterator limit[4];
  limit[0] = curr[1], limit[1] = curr[2],
    limit[2] = curr[3], limit[3] = curr[0];

  // quadruple of direction candidates defining the current rectangle
  Direction_2  dir[4];
  for (int i = 0; i < 4; i++) {
    ForwardIterator cp = curr[i];
    if (++cp == l)
      cp = f;
    dir[i] = rotate(direction(*(curr[i]), *cp), i);
  }

  int yet_to_finish = 0;
  for (int i1 = 0; i1 < 4; ++i1) {
    CGAL_optimisation_assertion(limit[i1] != l);
    if (curr[i1] != limit[i1])
      ++yet_to_finish;
  }

  int low = less_angle(dir[0], dir[1]) ? 0 : 1;
  int upp = less_angle(dir[2], dir[3]) ? 2 : 3;

  int event = less_angle(dir[low], dir[upp]) ? low : upp;

  Rectangle_2 rect_so_far =
    rectangle(*(curr[0]), dir[event], *(curr[1]), *(curr[2]), *(curr[3]));

  for (;;) {
    if (++curr[event] == l)
      curr[event] = f;
    ForwardIterator cp = curr[event];
    if (++cp == l)
      cp = f;

    dir[event] = rotate(direction(*(curr[event]), *cp), event);

    if (curr[event] == limit[event])
      if (--yet_to_finish <= 0)
        break;

    if (event < 2)
      low = less_angle(dir[0], dir[1]) ? 0 : 1;
    else
      upp = less_angle(dir[2], dir[3]) ? 2 : 3;

    event = less_angle(dir[low], dir[upp]) ? low : upp;

    Rectangle_2 test_rect = rectangle(*(curr[0]), dir[event],
                                      *(curr[1]), *(curr[2]), *(curr[3]));
    if (t.area_less_rectangle_2_object()(test_rect, rect_so_far))
      rect_so_far = test_rect;

  } // for (;;)

  return t.copy_rectangle_vertices_2(rect_so_far, o);

} // min_rectangle_2( f, l, o , t)
#ifdef CGAL_TRACE
CGAL_END_NAMESPACE
#include <CGAL/IO/Ostream_iterator.h>
#include <CGAL/IO/leda_window.h>
#include <CGAL/leda_real.h>
CGAL_BEGIN_NAMESPACE
#endif // CGAL_TRACE

template < class ForwardIterator, class OutputIterator, class BTraits >
OutputIterator
min_parallelogram_2(ForwardIterator f,
                    ForwardIterator l,
                    OutputIterator o,
                    BTraits& bt)
{
  typedef Optimisation::Min_quadrilateral_traits_wrapper<BTraits> Traits;
  Traits t(bt);
  CGAL_optimisation_expensive_precondition(is_convex_2(f, l));

  // types from the traits class
  typedef typename Traits::Direction_2            Direction_2;
  typedef typename Traits::Parallelogram_2        Parallelogram_2;
  typedef typename Traits::Construct_direction_2  Construct_direction_2;
  typedef typename Traits::Equal_2                Equal_2;

  Equal_2 equal = t.equal_2_object();
  Construct_direction_2 direction = t.construct_direction_2_object();
  typename Traits::Construct_parallelogram_2
    parallelogram = t.construct_parallelogram_2_object();
  typename Traits::Less_angle_with_x_axis_2
    less_angle = t.less_angle_with_x_axis_2_object();
  typename Traits::Area_less_parallelogram_2
    area_less = t.area_less_parallelogram_2_object();
  typename Traits::Right_of_implicit_line_2
    right_of_line = t.right_of_implicit_line_2_object();

  // check for trivial cases
  if (f == l) return o;
  
  ForwardIterator first;
  do {
    first = f;
    if (++f == l) {
      // all points are equal
      for (int i = 0; i < 4; ++i) *o++ = *first;
      return o;
    }
  } while (equal(*first, *f));

  // quadruple of points defining the bounding box
  ForwardIterator curr[4];
  // initialised to the points defining the bounding box
  convex_bounding_box_2(first, l, curr, t);

#ifdef CGAL_TRACE
  /*
  ForwardIterator mmix = std::min_element(first, l, t.less_xy_2_object());
  ForwardIterator mmax = std::max_element(first, l, t.less_xy_2_object());
  ForwardIterator mmiy = std::min_element(first, l, t.less_yx_2_object());
  ForwardIterator mmay = std::max_element(first, l, t.less_yx_2_object());
  CGAL_assertion(!t.less_xy_2_object()(*mmix, *(curr[3])));
  CGAL_assertion(!t.less_xy_2_object()(*(curr[3]), *mmix));
  CGAL_assertion(!t.less_xy_2_object()(*mmax, *(curr[1])));
  CGAL_assertion(!t.less_xy_2_object()(*(curr[1]), *mmax));
  CGAL_assertion(!t.less_yx_2_object()(*mmiy, *(curr[0])));
  CGAL_assertion(!t.less_yx_2_object()(*(curr[0]), *mmiy));
  CGAL_assertion(!t.less_yx_2_object()(*mmay, *(curr[2])));
  CGAL_assertion(!t.less_yx_2_object()(*(curr[2]), *mmay));
  */
#endif

  ForwardIterator low   = curr[0];
  ForwardIterator upp   = curr[2];
  ForwardIterator right = low;
  ForwardIterator left  = upp;

  int yet_to_finish = 2;

  // initialize parallelogram
  ForwardIterator ln = low;
  do
    if (++ln == l)
      ln = first;
  while (equal(*ln, *low));
  Direction_2 d_low = direction(*low, *ln);
  ForwardIterator un = upp;
  do
    if (++un == l)
      un = first;
  while (equal(*un, *upp));
  Direction_2 d_upp = direction(*un, *upp);

  bool low_goes_next = less_angle(d_low, d_upp);
  Direction_2 next_dir = low_goes_next ? d_low : d_upp;

  Direction_2 d_leftright = next_dir;
  for (;;) {
    // compute the next left/right candidate and store it to d_leftright
    ForwardIterator rig = right;
    do
      if (++rig == l)
        rig = first;
    while (equal(*rig, *right));
    Direction_2 d_right = direction(*right, *rig);
  
    ForwardIterator len = left;
    do
      if (++len == l)
        len = first;
    while (equal(*len, *left));
    Direction_2 d_left = direction(*len, *left);
  
    if (less_angle(d_right, d_left))
      if (right_of_line(*rig, *left, next_dir))
        right = rig;
      else {
        d_leftright = d_right;
        break;
      }
    else
      if (right_of_line(*right, *len, next_dir))
        left = len;
      else {
        d_leftright = d_left;
        break;
      }
  } // for (;;)

  Parallelogram_2 para_so_far =
    parallelogram(*low, next_dir, *right, d_leftright, *upp, *left);

  for (;;) {
    if (low_goes_next) {
      low = ln;
      if (low == curr[2])
        if (--yet_to_finish <= 0)
          break;
    } else {
      upp = un;
      if (upp == curr[0])
        if (--yet_to_finish <= 0)
          break;
    }

    // compute the next lower/upper candidate
    ln = low;
    do
      if (++ln == l)
        ln = first;
    while (equal(*ln, *low));
    d_low = direction(*low, *ln);
    un = upp;
    do
      if (++un == l)
        un = first;
    while (equal(*un, *upp));
    d_upp = direction(*un, *upp);

    low_goes_next = less_angle(d_low, d_upp);
    next_dir = low_goes_next ? d_low : d_upp;

    for (;;) {
      // compute the next left/right candidate and store it to d_leftright
      ForwardIterator rig = right;
      do
        if (++rig == l)
          rig = first;
      while (equal(*rig, *right));
      Direction_2 d_right = direction(*right, *rig);
    
      ForwardIterator len = left;
      do
        if (++len == l)
          len = first;
      while (equal(*len, *left));
      Direction_2 d_left = direction(*len, *left);
    
      if (less_angle(d_right, d_left))
        if (right_of_line(*rig, *left, next_dir))
          right = rig;
        else {
          d_leftright = d_right;
          break;
        }
      else
        if (right_of_line(*right, *len, next_dir))
          left = len;
        else {
          d_leftright = d_left;
          break;
        }
    } // for (;;)

    // check whether we found a smaller parallelogram
    Parallelogram_2 test_para =
      parallelogram(*low, next_dir, *right, d_leftright, *upp, *left);

#ifdef CGAL_TRACE
    {
      typedef typename
        std::iterator_traits< ForwardIterator >::value_type Point;
      typedef Polygon_traits_2< typename Traits::Kernel > P_traits;
      typedef std::vector< Point >                   Cont;
      typedef CGAL::Polygon_2< P_traits, Cont >      Polygon_2;
      Polygon_2 p;
      t.copy_parallelogram_vertices_2(test_para, std::back_inserter(p));
      CGAL_assertion(p.is_simple());
      CGAL_assertion(p.is_convex());
      cout << "p_area = " << p.area() << endl;
      for (ForwardIterator ii = first; ii != l; ++ii)
        CGAL_assertion(!p.has_on_unbounded_side(*ii));
    }
#endif // CGAL_TRACE

    if (area_less(test_para, para_so_far))
      para_so_far = test_para;

  } // for (;;)

#ifdef CGAL_TRACE
   typedef typename
     std::iterator_traits< ForwardIterator >::value_type Point;
   Point p[4];
   t.copy_parallelogram_vertices_2(para_so_far, p);
   leda_window w;
   w.init(-50, 450, -35);
   w.display();
   Ostream_iterator< Point, leda_window > oip(w);
   //std::ostream_iterator< Point > oipc(std::cerr, "\n");
   std::copy(first, l, oip);
   w << YELLOW;
   w.set_node_width(7);
   {
     ForwardIterator ii = curr[3];
     while (ii != curr[1]) {
       *oip++ = *ii;
       if (++ii == l) ii = first;
     }
     *oip++ = *ii;
   }
   w.set_node_width(5);
   w << GREEN << para_so_far.p1 << para_so_far.p2
     << para_so_far.p3 << para_so_far.p4;
   {
     typedef typename Traits::Line_2 Line_2;
     Line_2 l1(para_so_far.p1, para_so_far.d1);
     Line_2 l2(para_so_far.p2, para_so_far.d2);
     Line_2 l3(para_so_far.p3, para_so_far.d1);
     Line_2 l4(para_so_far.p4, para_so_far.d2);
     if (l1 == l2) cout << "l1 == l2" << endl;
     if (l1 == l3) cout << "l1 == l3" << endl;
     if (l1 == l4) cout << "l1 == l4" << endl;
     w << BLUE << l1 << l2 << l3 << l4;
   }
   w.set_node_width(3);
   w << RED << p[0] << p[1] << p[2] << p[3];
   w.read_mouse();
   std::cerr << "ZAP" << std::endl;
   *o++ = p[0];
   *o++ = p[1];
   *o++ = p[2];
   *o++ = p[3];
   return o;
 #endif

   return t.copy_parallelogram_vertices_2(para_so_far, o);
 } // min_parallelogram_2(f, l, o , t)
template < class ForwardIterator, class OutputIterator, class BTraits >
OutputIterator
min_strip_2(ForwardIterator f,
            ForwardIterator l,
            OutputIterator o,
            BTraits& bt)
{
  typedef Optimisation::Min_quadrilateral_traits_wrapper<BTraits> Traits;
  Traits t(bt);
  CGAL_optimisation_expensive_precondition(is_convex_2(f, l));

  // types from the traits class
  typedef typename Traits::Direction_2            Direction_2;
  typedef typename Traits::Strip_2                Strip_2;
  typedef typename Traits::Equal_2                Equal_2;
  typedef typename Traits::Construct_direction_2  Construct_direction_2;
  typedef typename Traits::Construct_strip_2      Construct_strip_2;
  typedef typename Traits::Width_less_strip_2     Width_less_strip_2;

  Equal_2 equal = t.equal_2_object();
  Construct_direction_2 direction = t.construct_direction_2_object();
  Construct_strip_2 strip = t.construct_strip_2_object();
  Width_less_strip_2 width_less = t.width_less_strip_2_object();
  typename Traits::Less_angle_with_x_axis_2
    less_angle = t.less_angle_with_x_axis_2_object();

  // check for trivial cases
  if (f == l) return o;
  ForwardIterator first;
  do {
    first = f;
    if (++f == l)
      // strip undefined, if no two distinct points exist
      return o;
  } while (equal(*first, *f));

  // quadruple of points defining the bounding box
  ForwardIterator curr[4];
  // initialised to the points defining the bounding box
  convex_bounding_box_2(first, l, curr, t);

  ForwardIterator low = curr[0];
  ForwardIterator upp = curr[2];

  int yet_to_finish = 2;

  ForwardIterator nlow = low;
  if (++nlow == l)
    nlow = first;
  Direction_2 low_dir = direction(*low, *nlow);
  ForwardIterator nupp = upp;
  if (++nupp == l)
    nupp = first;
  Direction_2 upp_dir = direction(*nupp, *upp);

  bool low_goes_next = less_angle(low_dir, upp_dir);
  Strip_2 strip_so_far = low_goes_next ?
    strip(*low, low_dir, *upp) : strip(*low, upp_dir, *upp);

  for (;;) {
    // compute next direction
    if (low_goes_next) {
      low = nlow;
      if (low == curr[2])
        if (--yet_to_finish <= 0)
          break;
      if (++nlow == l)
        nlow = first;
      low_dir = direction(*low, *nlow);
    } else {
      upp = nupp;
      if (upp == curr[0])
        if (--yet_to_finish <= 0)
          break;
      if (++nupp == l)
        nupp = first;
      upp_dir = direction(*nupp, *upp);
    }

    low_goes_next = less_angle(low_dir, upp_dir);
    Strip_2 test_strip = low_goes_next ?
      strip(*low, low_dir, *upp) : strip(*low, upp_dir, *upp);
    if (width_less(test_strip, strip_so_far))
      strip_so_far = test_strip;

  } // for (;;)

  // return the result
  return t.copy_strip_lines_2(strip_so_far, o);

} // min_strip_2(f, l, o, t)


#ifdef CGAL_REP_CLASS_DEFINED

CGAL_END_NAMESPACE
#include <CGAL/Min_quadrilateral_traits_2.h>
CGAL_BEGIN_NAMESPACE

template < class ForwardIterator, class OutputIterator >
inline
OutputIterator
min_rectangle_2(ForwardIterator f,
         ForwardIterator l,
         OutputIterator o)
{
  typedef typename std::iterator_traits< ForwardIterator >::value_type VT;
  typedef typename Kernel_traits<VT>::Kernel Kernel;
  Min_quadrilateral_default_traits_2<Kernel> t;
  return min_rectangle_2(f, l, o, t);
} // min_rectangle_2(f, l, o)

// backwards compatibility
template < class ForwardIterator, class OutputIterator >
inline
OutputIterator
minimum_enclosing_rectangle_2(ForwardIterator f,
                       ForwardIterator l,
                       OutputIterator o)
{ return min_rectangle_2(f, l, o); }
template < class ForwardIterator, class OutputIterator >
inline
OutputIterator
min_parallelogram_2(ForwardIterator f,
         ForwardIterator l,
         OutputIterator o)
{
  typedef typename std::iterator_traits< ForwardIterator >::value_type VT;
  typedef typename Kernel_traits<VT>::Kernel Kernel;
  Min_quadrilateral_default_traits_2<Kernel> t;
  return min_parallelogram_2(f, l, o, t);
} // min_parallelogram_2(f, l, o)

// backwards compatibility
template < class ForwardIterator, class OutputIterator >
inline
OutputIterator
minimum_enclosing_parallelogram_2(ForwardIterator f,
                       ForwardIterator l,
                       OutputIterator o)
{ return min_parallelogram_2(f, l, o); }
template < class ForwardIterator, class OutputIterator >
inline
OutputIterator
min_strip_2(ForwardIterator f,
         ForwardIterator l,
         OutputIterator o)
{
  typedef typename std::iterator_traits< ForwardIterator >::value_type VT;
  typedef typename Kernel_traits<VT>::Kernel Kernel;
  Min_quadrilateral_default_traits_2<Kernel> t;
  return min_strip_2(f, l, o, t);
} // min_strip_2(f, l, o)

// backwards compatibility
template < class ForwardIterator, class OutputIterator >
inline
OutputIterator
minimum_enclosing_strip_2(ForwardIterator f,
                       ForwardIterator l,
                       OutputIterator o)
{ return min_strip_2(f, l, o); }

#endif // CGAL_REP_CLASS_DEFINED

CGAL_END_NAMESPACE

#endif // ! (CGAL_MIN_QUADRILATERAL_2_H)

// ----------------------------------------------------------------------------
// ** EOF
// ----------------------------------------------------------------------------

