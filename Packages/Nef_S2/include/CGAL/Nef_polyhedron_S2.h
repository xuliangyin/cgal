// Copyright (c) 1997-2000  Max-Planck-Institute Saarbruecken (Germany).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org); you may redistribute it under
// the terms of the Q Public License version 1.0.
// See the file LICENSE.QPL distributed with CGAL.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $Source$
// $Revision$ $Date$
// $Name$
//
// Author(s)     : Michael Seel       <seel@mpi-sb.mpg.de>
//                 Peter Hachenberger <hachenberger@mpi-sb.mpg.de>

#ifndef CGAL_NEF_POLYHEDRON_S2_H
#define CGAL_NEF_POLYHEDRON_S2_H

#include <CGAL/basic.h>
#include <CGAL/Handle_for.h>
#include <CGAL/Random.h>
#include <CGAL/Nef_S2/Sphere_map.h>
#include <CGAL/Nef_S2/SM_decorator.h>
#include <CGAL/Nef_S2/SM_io_parser.h>
#include <CGAL/Nef_S2/SM_point_locator.h>
#include <CGAL/Nef_S2/SM_overlayer.h>

#include <vector>
#include <list>
#undef _DEBUG
#define _DEBUG 121
#include <CGAL/Nef_S2/debug.h>

CGAL_BEGIN_NAMESPACE

template <typename K> class Nef_polyhedron_S2;
template <typename K> class Nef_polyhedron_S2_rep;
template <typename K> class Nef_polyhedron_3;
template <typename K, typename I> class SNC_items;

template <typename K>
std::ostream& operator<<(std::ostream&, const Nef_polyhedron_S2<K>&); 
template <typename K>
std::istream& operator>>(std::istream&, Nef_polyhedron_S2<K>&);


template <typename Items_>
class Nef_polyhedron_S2_rep { 

  friend class Nef_polyhedron_S2<Items_>;
  
  typedef Items_                                       Items;
  typedef Nef_polyhedron_S2_rep<Items>                 Self;
  typedef typename Items::Sphere_kernel                Sphere_kernel;
  typedef typename Items::Mark                         Mark;
  typedef CGAL::Sphere_map<Sphere_kernel,Items>        Sphere_map;
  typedef CGAL::SM_const_decorator<Sphere_map>         Const_decorator;
  typedef CGAL::SM_decorator<Sphere_map>               Decorator;
  typedef CGAL::SM_overlayer<Decorator>                Overlayer;
  typedef CGAL::SM_point_locator<Const_decorator>      Locator;

  Sphere_map sm_; 
  
public:
  Nef_polyhedron_S2_rep() : sm_() {}
  Nef_polyhedron_S2_rep(const Self& R) : sm_() {}
  ~Nef_polyhedron_S2_rep() { sm_.clear(); }
};

/*{\Moptions print_title=yes }*/ 
/*{\Manpage {Nef_polyhedron_S2}{K}
{Nef Polyhedra in the sphere surface}{N}}*/

/*{\Mdefinition An instance of data type |\Mname| is a subset of $S_2$
that is the result of forming complements and intersections starting
from a finite set |H| of half-spaces. |\Mtype| is closed under all
binary set operations |intersection|, |union|, |difference|,
|complement| and under the topological operations |boundary|,
|closure|, and |interior|.

The template parameter |Kernel| is specified via a kernel concept. 
|Kernel| must be a model of the concept |NefSphereKernelTraits_2|.
}*/

template <typename Items_>
class Nef_polyhedron_S2 : public Handle_for< Nef_polyhedron_S2_rep<Items_> >, 
			  public Nef_polyhedron_S2_rep<Items_>::Const_decorator { 
  
public:
  /*{\Mtypes 7}*/
  typedef Items_                                     Items;
  typedef Nef_polyhedron_S2<Items>                   Self;
  typedef Nef_polyhedron_S2_rep<Items>               Rep;
  typedef Handle_for< Nef_polyhedron_S2_rep<Items> > Base;
  typedef typename Rep::Sphere_kernel                Sphere_kernel;
  typedef typename Rep::Sphere_map                   Sphere_map;

  typedef typename Sphere_kernel::Sphere_point   Sphere_point;
  /*{\Mtypemember points in the sphere surface.}*/

  typedef typename Sphere_kernel::Sphere_segment Sphere_segment;
  /*{\Mtypemember segments in the sphere surface.}*/

  typedef typename Sphere_kernel::Sphere_circle  Sphere_circle;
  /*{\Mtypemember oriented great circles modeling spherical half-spaces}*/

  typedef typename Sphere_kernel::Sphere_direction Sphere_direction;

  typedef typename Rep::Mark Mark;
  /*{\Xtypemember marking set membership or exclusion.}*/

  typedef typename Items::Kernel Kernel;

  enum Boundary { EXCLUDED=0, INCLUDED=1 };
  /*{\Menum construction selection.}*/

  enum Content { EMPTY=0, COMPLETE=1 };
  /*{\Menum construction selection}*/

  const Sphere_map& sphere_map() const { return ptr()->sm_; }
protected:
  Sphere_map& sphere_map() { return ptr()->sm_; } 

  struct AND { bool operator()(bool b1, bool b2)  const { return b1&&b2; }  };
  struct OR { bool operator()(bool b1, bool b2)   const { return b1||b2; }  };
  struct DIFF { bool operator()(bool b1, bool b2) const { return b1&&!b2; } };
  struct XOR { bool operator()(bool b1, bool b2)  const 
               { return (b1&&!b2)||(!b1&&b2); } };   

  typedef Nef_polyhedron_S2_rep<Items>          Nef_rep;
  typedef typename Nef_rep::Decorator       Decorator;
  typedef typename Nef_rep::Const_decorator Const_decorator;
  typedef typename Nef_rep::Overlayer       Overlayer;
  typedef typename Nef_rep::Locator         Locator;

  friend std::ostream& operator<< <>
      (std::ostream& os, const Nef_polyhedron_S2<Items>& NP);
  friend std::istream& operator>> <>
      (std::istream& is, Nef_polyhedron_S2<Items>& NP);

  typedef typename Decorator::SVertex_handle         SVertex_handle;
  typedef typename Decorator::SHalfedge_handle       SHalfedge_handle;
  typedef typename Decorator::SHalfloop_handle       SHalfloop_handle;
  typedef typename Decorator::SFace_handle           SFace_handle;

  typedef typename Decorator::SVertex_const_handle   SVertex_const_handle;
  typedef typename Decorator::SHalfedge_const_handle SHalfedge_const_handle;
  typedef typename Decorator::SHalfloop_const_handle SHalfloop_const_handle;
  typedef typename Decorator::SFace_const_handle     SFace_const_handle;

  typedef typename Decorator::SVertex_iterator       SVertex_iterator;
  typedef typename Decorator::SHalfedge_iterator     SHalfedge_iterator;
  typedef typename Decorator::SHalfloop_iterator     SHalfloop_iterator;
  typedef typename Decorator::SFace_iterator         SFace_iterator;

  typedef typename Const_decorator::SVertex_const_iterator   
                                                    SVertex_const_iterator;
  typedef typename Const_decorator::SHalfedge_const_iterator 
                                                    SHalfedge_const_iterator;
  typedef typename Const_decorator::SHalfloop_const_iterator 
                                                    SHalfloop_const_iterator;
  typedef typename Const_decorator::SFace_const_iterator     
                                                    SFace_const_iterator;
  typedef typename Const_decorator::Constructor_parameter Constructor_parameter;

  typedef std::list<Sphere_segment>  SS_list;
  typedef typename SS_list::const_iterator SS_iterator;

  typedef SNC_items<Kernel, Mark>                      SNC_items;
  friend class Nef_polyhedron_3<SNC_items>;

public:
  /*{\Mcreation 3}*/

  Nef_polyhedron_S2(Content sphere = EMPTY) : Base(Nef_rep())
  /*{\Mcreate creates an instance |\Mvar| of type |\Mname|
  and initializes it to the empty set if |sphere == EMPTY|
  and to the whole sphere if |sphere == COMPLETE|.}*/
  {
    set_sm(&sphere_map());
    Decorator D(&sphere_map());
    SFace_handle sf=D.new_sface();
    D.mark(sf) = bool(sphere);
  }


  Nef_polyhedron_S2(const Sphere_circle& c, 
		    Boundary circle = INCLUDED) : Base(Nef_rep()) {
  /*{\Mcreate creates a Nef polyhedron |\Mvar| containing the half-sphere
  left of |c| including |c| if |circle==INCLUDED|, excluding |c| if 
  |circle==EXCLUDED|.}*/  
    
    set_sm(&sphere_map());
    TRACEN("Nef_polyhedron_S2(): construction from circle "<<c);
    Decorator D(&sphere_map());
    Overlayer O(&sphere_map()); 
    O.create(c);
    SHalfloop_handle h = D.shalfloop();
    if ( D.circle(h) != c ) h = D.twin(h);
    D.mark(D.face(h)) = true;
    D.mark(h) = bool(circle);
  }


  template <class Forward_iterator>
  Nef_polyhedron_S2(Forward_iterator first, Forward_iterator beyond,
    Boundary b = INCLUDED) : Base(Nef_rep())
  /*{\Mcreate creates a Nef polyhedron |\Mvar| from the set of sphere
    segments in the iterator range |[first,beyond)|. If the set of sphere
    segments is a simple polygon that separates the sphere surface
    into two regions, then the polygonal region that is left of the
    segment |*first| is selected. The polygonal region includes its
    boundary if |b = INCLUDED| and excludes the boundary
    otherwise. |Forward_iterator| has to be an iterator with value
    type |Sphere_segment|.}*/
  { TRACEN("Nef_polyhedron_S2(): creation from segment range");
    CGAL_assertion(first!=beyond);
    set_sm(&sphere_map());
    Overlayer D(&sphere_map());
    Sphere_segment s = *first;
    D.create_from_segments(first,beyond);
    SHalfedge_iterator e;
    CGAL_forall_shalfedges(e,D) {
      Sphere_circle c(D.circle(e));
      if ( c == s.sphere_circle() ) break;
    }
    if ( e != SHalfedge_iterator() ) {
      if ( D.circle(e) != s.sphere_circle() ) e = D.twin(e);
      CGAL_assertion( D.circle(e) == s.sphere_circle() );
      D.set_marks_in_face_cycle(e,bool(b));
      if ( D.number_of_sfaces() > 2 ) D.mark(D.face(e)) = true;
      else                            D.mark(D.face(e)) = !bool(b);
      return;
    }
    D.simplify();
  }

  Nef_polyhedron_S2(const Nef_polyhedron_S2<Items>& N1) : Base(N1) {
    set_sm(&sphere_map());
  }
  Nef_polyhedron_S2& operator=(const Nef_polyhedron_S2<Items>& N1)
  { Base::operator=(N1); set_sm(&sphere_map()); return (*this); }
  ~Nef_polyhedron_S2() {}

  template <class Forward_iterator>
  Nef_polyhedron_S2(Forward_iterator first, Forward_iterator beyond, 
    double p) : Base(Nef_rep())
  /*{\Xcreate creates a random Nef polyhedron from the arrangement of
  the set of circles |S = set[first,beyond)|. The cells of the arrangement
  are selected uniformly at random with probability $p$. \precond $0 < p
  < 1$.}*/
  { CGAL_assertion(0<=p && p<=1);
    CGAL_assertion(first!=beyond);
    set_sm(&sphere_map());
    Overlayer D(&sphere_map());
    D.create_from_circles(first, beyond); D.simplify();
    SVertex_iterator v; SHalfedge_iterator e; SFace_iterator f;
    CGAL_forall_svertices(v,D)
      D.mark(v) = ( default_random.get_double() < p ? true : false );
    CGAL_forall_shalfedges(e,D)
      D.mark(e) = ( default_random.get_double() < p ? true : false );
    CGAL_forall_sfaces(f,D)
      D.mark(f) = ( default_random.get_double() < p ? true : false );
    D.simplify();
  }

protected:
  Nef_polyhedron_S2(const Sphere_map& H, bool clone=true) : Base(Nef_rep()) 
  /*{\Xcreate makes |\Mvar| a new object.  If |clone==true| then the
  underlying structure of |H| is copied into |\Mvar|.}*/
  {     
    if (!clone) return;
    ptr()->sm_ = H; 
    set_sm(&sphere_map());
  }
  
  void clone_rep() { *this = Nef_polyhedron_S2<Items>(sphere_map()); }

  /*{\Moperations 4 3 }*/
  public:

  void clear(Content plane = EMPTY)
  { *this = Nef_polyhedron_S2(plane); }
  /*{\Mop makes |\Mvar| the empty set if |plane == EMPTY| and the
  full plane if |plane == COMPLETE|.}*/

  bool is_empty() const
  /*{\Mop returns true if |\Mvar| is empty, false otherwise.}*/
  { Const_decorator D(&sphere_map());
    TRACEN("is_empty()"<<*this);
    SFace_const_iterator f = D.sfaces_begin();
    return (D.number_of_svertices()==0 &&
            D.number_of_sedges()==0 &&
            D.number_of_sloops()==0 &&
            D.number_of_sfaces()==1 &&
            D.mark(f) == false);
  }

  bool is_plane() const
  /*{\Mop returns true if |\Mvar| is the whole plane, false otherwise.}*/
  { Const_decorator D(&sphere_map());
    SFace_const_iterator f = D.sfaces_begin();
    return (D.number_of_svertices()==0 &&
            D.number_of_sedges()==0 &&
            D.number_of_sloops()==0 &&
            D.number_of_sfaces()==1 &&
            D.mark(f) == true);
  }

  void extract_complement()
  { TRACEN("extract complement");
    if ( is_shared() ) clone_rep();
    Overlayer D(&sphere_map());
    SVertex_iterator v;
    SHalfedge_iterator e;
    SFace_iterator f;
    CGAL_forall_svertices(v,D) D.mark(v) = !D.mark(v);
    CGAL_forall_sedges(e,D) D.mark(e) = !D.mark(e);
    CGAL_forall_sfaces(f,D) D.mark(f) = !D.mark(f);

    if ( D.has_shalfloop() ) 
      D.mark(D.shalfloop()) = !D.mark(D.shalfloop());
  }

  void extract_interior()
  { TRACEN("extract interior");
    if ( is_shared() ) clone_rep();
    Overlayer D(&sphere_map());
    SVertex_iterator v;
    SHalfedge_iterator e;
    CGAL_forall_svertices(v,D) D.mark(v) = false;
    CGAL_forall_sedges(e,D) D.mark(e) = false;
    if ( D.has_sloop() ) D.mark(D.shalfloop()) = false;
    D.simplify();
  }


  void extract_boundary()
  { TRACEN("extract boundary");
    if ( is_shared() ) clone_rep();
    Overlayer D(&sphere_map());
    SVertex_iterator v;
    SHalfedge_iterator e;
    SFace_iterator f;
    CGAL_forall_svertices(v,D) D.mark(v) = true;
    CGAL_forall_sedges(e,D)    D.mark(e) = true;
    CGAL_forall_sfaces(f,D)    D.mark(f) = false;
    if ( D.has_sloop() ) D.mark(D.shalfloop()) = true;
    D.simplify();
  }

  void extract_closure()
  /*{\Xop converts |\Mvar| to its closure. }*/
  { TRACEN("extract closure");
    extract_complement();
    extract_interior();
    extract_complement();
  }

  void extract_regularization()
  /*{\Xop converts |\Mvar| to its regularization. }*/
  { TRACEN("extract regularization");
    extract_interior();
    extract_closure();
  }

  /*{\Mtext \headerline{Constructive Operations}}*/

  Nef_polyhedron_S2<Items> complement() const
  /*{\Mop returns the complement of |\Mvar| in the plane.}*/
  { Nef_polyhedron_S2<Items> res = *this;
    res.extract_complement();
    return res;
  }


  Nef_polyhedron_S2<Items> interior() const
  /*{\Mop returns the interior of |\Mvar|.}*/
  { Nef_polyhedron_S2<Items> res = *this;
    res.extract_interior();
    return res;
  }

  Nef_polyhedron_S2<Items> closure() const
  /*{\Mop returns the closure of |\Mvar|.}*/
  { Nef_polyhedron_S2<Items> res = *this;
    res.extract_closure();
    return res;
  }

  Nef_polyhedron_S2<Items> boundary() const
  /*{\Mop returns the boundary of |\Mvar|.}*/
  { Nef_polyhedron_S2<Items> res = *this;
    res.extract_boundary();
    return res;
  }

  Nef_polyhedron_S2<Items> regularization() const
  /*{\Mop returns the regularized polyhedron (closure of interior).}*/
  { Nef_polyhedron_S2<Items> res = *this;
    res.extract_regularization();
    return res;
  }


  Nef_polyhedron_S2<Items> intersection(const Nef_polyhedron_S2<Items>& N1) const
  /*{\Mop returns |\Mvar| $\cap$ |N1|. }*/
  { Nef_polyhedron_S2<Items> res(sphere_map(),false); // empty
    Overlayer D(&res.sphere_map());
    D.subdivide(&sphere_map(),&N1.sphere_map());
    AND _and; D.select(_and); D.simplify();
    return res;
  }


  Nef_polyhedron_S2<Items> join(const Nef_polyhedron_S2<Items>& N1) const
  /*{\Mop returns |\Mvar| $\cup$ |N1|. }*/
  { Nef_polyhedron_S2<Items> res(sphere_map(),false); // empty
    Overlayer D(&res.sphere_map());
    D.subdivide(&sphere_map(),&N1.sphere_map());
    OR _or; D.select(_or); D.simplify();
    return res;
  }

  Nef_polyhedron_S2<Items> difference(const Nef_polyhedron_S2<Items>& N1) const
  /*{\Mop returns |\Mvar| $-$ |N1|. }*/
  { Nef_polyhedron_S2<Items> res(sphere_map(),false); // empty
    Overlayer D(&res.sphere_map());
    D.subdivide(&sphere_map(),&N1.sphere_map());
    DIFF _diff; D.select(_diff); D.simplify();
    return res;
  }    

  Nef_polyhedron_S2<Items> symmetric_difference(
    const Nef_polyhedron_S2<Items>& N1) const
  /*{\Mop returns the symmectric difference |\Mvar - T| $\cup$ 
          |T - \Mvar|. }*/
  { Nef_polyhedron_S2<Items> res(sphere_map(),false); // empty
    Overlayer D(&res.sphere_map());
    D.subdivide(&sphere_map(),&N1.sphere_map());
    XOR _xor; D.select(_xor); D.simplify();
    return res;
  }

  /*{\Mtext Additionally there are operators |*,+,-,^,!| which
  implement the binary operations \emph{intersection}, \emph{union},
  \emph{difference}, \emph{symmetric difference}, and the unary
  operation \emph{complement} respectively. There are also the
  corresponding modification operations |*=,+=,-=,^=|.}*/

  Nef_polyhedron_S2<Items>  operator*(const Nef_polyhedron_S2<Items>& N1) const
  { return intersection(N1); }

  Nef_polyhedron_S2<Items>  operator+(const Nef_polyhedron_S2<Items>& N1) const
  { return join(N1); }

  Nef_polyhedron_S2<Items>  operator-(const Nef_polyhedron_S2<Items>& N1) const
  { return difference(N1); }

  Nef_polyhedron_S2<Items>  operator^(const Nef_polyhedron_S2<Items>& N1) const
  { return symmetric_difference(N1); }

  Nef_polyhedron_S2<Items>  operator!() const
  { return complement(); }
   
  Nef_polyhedron_S2<Items>& operator*=(const Nef_polyhedron_S2<Items>& N1)
  { this = intersection(N1); return *this; }

  Nef_polyhedron_S2<Items>& operator+=(const Nef_polyhedron_S2<Items>& N1)
  { this = join(N1); return *this; }

  Nef_polyhedron_S2<Items>& operator-=(const Nef_polyhedron_S2<Items>& N1)
  { this = difference(N1); return *this; }

  Nef_polyhedron_S2<Items>& operator^=(const Nef_polyhedron_S2<Items>& N1)
  { this = symmetric_difference(N1); return *this; }

  /*{\Mtext There are also comparison operations like |<,<=,>,>=,==,!=|
  which implement the relations subset, subset or equal, superset, superset
  or equal, equality, inequality, respectively.}*/

  bool operator==(const Nef_polyhedron_S2<Items>& N1) const
  { return symmetric_difference(N1).is_empty(); }

  bool operator!=(const Nef_polyhedron_S2<Items>& N1) const
  { return !operator==(N1); }  

  bool operator<=(const Nef_polyhedron_S2<Items>& N1) const
  { return difference(N1).is_empty(); } 

  bool operator<(const Nef_polyhedron_S2<Items>& N1) const
  { return difference(N1).is_empty() && !N1.difference(*this).is_empty(); } 

  bool operator>=(const Nef_polyhedron_S2<Items>& N1) const
  { return N1.difference(*this).is_empty(); } 

  bool operator>(const Nef_polyhedron_S2<Items>& N1) const   
  { return N1.difference(*this).is_empty() && !difference(N1).is_empty(); } 


  /*{\Mtext \headerline{Exploration - Point location - Ray shooting}
  As Nef polyhedra are the result of forming complements 
  and intersections starting from a set |H| of half-spaces that are
  defined by oriented lines in the plane, they can be represented by
  an attributed plane map $M = (V,E,F)$. For topological queries
  within |M| the following types and operations allow exploration
  access to this structure.}*/

  /*{\Mtypes 3}*/
  typedef Const_decorator Topological_explorer;

  typedef Const_decorator Explorer;
  /*{\Mtypemember a decorator to examine the underlying plane map. 
  See the manual page of |Explorer|.}*/

  typedef typename Locator::Object_handle Object_handle;
  /*{\Mtypemember a generic handle to an object of the underlying
  plane map. The kind of object |(vertex, halfedge, face)| can 
  be determined and the object can be assigned to a corresponding
  handle by the three functions:\\
  |bool assign(SVertex_const_handle& h, Object_handle)|\\
  |bool assign(SHalfedge_const_handle& h, Object_handle)|\\
  |bool assign(SFace_const_handle& h, Object_handle)|\\
  where each function returns |true| iff the assignment to
  |h| was done.}*/


  /*{\Moperations 3 1 }*/

  bool contains(Object_handle h) const
  /*{\Mop  returns true iff the object |h| is contained in the set
  represented by |\Mvar|.}*/
  { Locator PL(&sphere_map()); return PL.mark(h); }

  bool contained_in_boundary(Object_handle h) const
  /*{\Mop  returns true iff the object |h| is contained in the $1$-skeleton
  of |\Mvar|.}*/
  { SVertex_const_handle v;
    SHalfedge_const_handle e;
    return  ( CGAL::assign(v,h) || CGAL::assign(e,h) );
  }


  Object_handle locate(const Sphere_point& p) const
  /*{\Mop  returns a generic handle |h| to an object (face, halfedge, vertex) 
  of the underlying plane map that contains the point |p| in its relative 
  interior. The point |p| is contained in the set represented by |\Mvar| if 
  |\Mvar.contains(h)| is true. The location mode flag |m| allows one to choose
  between different point location strategies.}*/
  { 
    Locator PL(&sphere_map());
    return PL.locate(p); 
  }

  struct INSET {
    const Const_decorator& D;
    INSET(const Const_decorator& Di) : D(Di) {}
    bool operator()(SVertex_const_handle v) const { return D.mark(v); }
    bool operator()(SHalfedge_const_handle e) const { return D.mark(e); }
    bool operator()(SHalfloop_const_handle l) const { return D.mark(l); }
    bool operator()(SFace_const_handle f) const { return D.mark(f); }
  };

  Object_handle ray_shoot(const Sphere_point& p, 
			  const Sphere_direction& d) const
  /*{\Mop returns a handle |h| with |\Mvar.contains(h)| that can be
  converted to a |SVertex_/SHalfedge_/SFace_const_handle| as described
  above. The object returned is intersected by the ray starting in |p|
  with direction |d| and has minimal distance to |p|.  The operation
  returns the null handle |NULL| if the ray shoot along |d| does not hit
  any object |h| of |\Mvar| with |\Mvar.contains(h)|.}*/
  { 
    Locator PL(&sphere_map());
    return PL.ray_shoot(p,d,INSET(PL));
  }

  struct INSKEL {
    bool operator()(SVertex_const_handle) const { return true; }
    bool operator()(SHalfedge_const_handle) const { return true; }
    bool operator()(SHalfloop_const_handle) const { return true; }
    bool operator()(SFace_const_handle) const { return false; }
  };

  Object_handle ray_shoot_to_boundary(const Sphere_point& p, 
				      const Sphere_direction& d) const
  /*{\Mop returns a handle |h| that can be converted to a
  |SVertex_/SHalfedge_const_handle| as described above. The object
  returned is part of the $1$-skeleton of |\Mvar|, intersected by the
  ray starting in |p| with direction |d| and has minimal distance to
  |p|.  The operation returns the null handle |NULL| if the ray shoot
  along |d| does not hit any $1$-skeleton object |h| of |\Mvar|. The
  location mode flag |m| allows one to choose between different point
  location strategies.}*/
  { 
    Locator PL(&sphere_map());
    return PL.ray_shoot(p,d,INSKEL());
  }


  //  Explorer explorer() const 
  /*{\Mop returns a decorator object which allows read-only access of
  the underlying plane map. See the manual page |Explorer| for its 
  usage.}*/
  //  { return Explorer(const_cast<Sphere_map*>(&sphere_map())); }

  /*{\Mtext\headerline{Input and Output}
  A Nef polyhedron |\Mvar| can be visualized in an open GL window. The 
  output operator is defined in the file 
  |CGAL/IO/Nef_\-poly\-hedron_2_\-Win\-dow_\-stream.h|.
  }*/

  /*{\Mimplementation Nef polyhedra are implemented on top of a halfedge
  data structure and use linear space in the number of vertices, edges
  and facets.  Operations like |empty| take constant time. The
  operations |clear|, |complement|, |interior|, |closure|, |boundary|,
  |regularization|, input and output take linear time. All binary set
  operations and comparison operations take time $O(n \log n)$ where $n$
  is the size of the output plus the size of the input.

  The point location and ray shooting operations are implemented in
  the naive way. The operations run in linear query time without
  any preprocessing.}*/

  /*{\Mexample Nef polyhedra are parameterized by a standard CGAL
  kernel. 

  \begin{Mverb}
  #include <CGAL/Homogeneous.h>
  #include <CGAL/leda_integer.h>
  #include <CGAL/Nef_polyhedron_S2.h>
  #include <CGAL/SM_items.h>

  using namespace CGAL;
  typedef  Homogeneous<leda_integer>   Kernel;
  typedef  SM_items<Kernel, bool>      SM_items;
  typedef  Nef_polyhedron_S2<SM_items> Nef_polyhedron;
  typedef  Nef_polyhedron::Sphere_circle Sphere_circle;

  int main()
  {
    Nef_polyhedron N1(Sphere_circle(1,0,0));
    Nef_polyhedron N2(Sphere_circle(0,1,0), Nef_polyhedron::EXCLUDED);
    Nef_polyhedron N3 = N1 * N2; // line (*)
    return 0;
  }
  \end{Mverb}
  After line (*) |N3| is the intersection of |N1| and |N2|.}*/


}; // end of Nef_polyhedron_S2

template <typename Items>
std::ostream& operator<<
 (std::ostream& os, const Nef_polyhedron_S2<Items>& NP)
{
  os << "Nef_polyhedron_S2\n";
  typedef typename Nef_polyhedron_S2<Items>::Explorer Decorator;
  typedef typename Nef_polyhedron_S2<Items>::Sphere_map Sphere_map;
  CGAL::SM_io_parser<Decorator> O(os, Decorator(&NP.sphere_map())); 
  O.print();
  return os;
}

template <typename Items>
std::istream& operator>>
  (std::istream& is, Nef_polyhedron_S2<Items>& NP)
{
  typedef typename Nef_polyhedron_S2<Items>::Decorator Decorator;
  CGAL::SM_io_parser<Decorator> I(is, Decorator(NP.sphere_map())); 
  if ( I.check_sep("Nef_polyhedron_S2") ) I.read();
  else {
    std::cerr << "Nef_polyhedron_S2 input corrupted." << std::endl;
    NP = Nef_polyhedron_S2<Items>();
  }
  typename Nef_polyhedron_S2<Items>::Topological_explorer D(NP.explorer());
  D.check_integrity_and_topological_planarity();
  return is;
}


CGAL_END_NAMESPACE
#endif //CGAL_NEF_POLYHEDRON_S2_H


