#include "rmatrix.h"
#include <limits.h>
#include <math.h>

// some defines to inline some code
#define MAPDOUBLE(x, y, nx, ny) \
{ \
    double fx = x; \
    double fy = y; \
    nx = _m11*fx + _m21*fy + _dx; \
    ny = _m12*fx + _m22*fy + _dy; \
}

#define MAPINT(x, y, nx, ny) \
{ \
    double fx = x; \
    double fy = y; \
    nx = rRound(_m11*fx + _m21*fy + _dx); \
    ny = rRound(_m12*fx + _m22*fy + _dy); \
}

/*****************************************************************************
  RMatrix member functions
 *****************************************************************************/
/*!
    \fn RMatrix::RMatrix()
    \internal
*/

/*!
    Constructs an identity matrix.

    All elements are set to zero except \c m11 and \c m22 (specifying
    the scale), which are set to 1.

    \sa reset()
*/

RMatrix::RMatrix()
    : _m11(1.)
    , _m12(0.)
    , _m21(0.)
    , _m22(1.)
    , _dx(0.)
    , _dy(0.)
{
}

/*!
    Constructs a matrix with the elements, \a m11, \a m12, \a m21, \a
    m22, \a dx and \a dy.

    \sa setMatrix()
*/

RMatrix::RMatrix(double m11, double m12, double m21, double m22, double dx, double dy)
    : _m11(m11)
    , _m12(m12)
    , _m21(m21)
    , _m22(m22)
    , _dx(dx)
    , _dy(dy)
{
}


/*!
     Constructs a matrix that is a copy of the given \a matrix.
 */
RMatrix::RMatrix(const RMatrix &matrix)
    : _m11(matrix._m11)
    , _m12(matrix._m12)
    , _m21(matrix._m21)
    , _m22(matrix._m22)
    , _dx(matrix._dx)
    , _dy(matrix._dy)
{
}

/*!
    Sets the matrix elements to the specified values, \a m11, \a m12,
    \a m21, \a m22, \a dx and \a dy.

    Note that this function replaces the previous values. RMatrix
    provide the translate(), rotate(), scale() and shear() convenience
    functions to manipulate the various matrix elements based on the
    currently defined coordinate system.

    \sa RMatrix()
*/

void RMatrix::setMatrix(double m11, double m12, double m21, double m22, double dx, double dy)
{
    _m11 = m11;
    _m12 = m12;
    _m21 = m21;
    _m22 = m22;
    _dx  = dx;
    _dy  = dy;
}


/*!
    \fn double RMatrix::m11() const

    Returns the horizontal scaling factor.

    \sa scale(), {RMatrix#Basic Matrix Operations}{Basic Matrix
    Operations}
*/

/*!
    \fn double RMatrix::m12() const

    Returns the vertical shearing factor.

    \sa shear(), {RMatrix#Basic Matrix Operations}{Basic Matrix
    Operations}
*/

/*!
    \fn double RMatrix::m21() const

    Returns the horizontal shearing factor.

    \sa shear(), {RMatrix#Basic Matrix Operations}{Basic Matrix
    Operations}
*/

/*!
    \fn double RMatrix::m22() const

    Returns the vertical scaling factor.

    \sa scale(), {RMatrix#Basic Matrix Operations}{Basic Matrix
    Operations}
*/

/*!
    \fn double RMatrix::dx() const

    Returns the horizontal translation factor.

    \sa translate(), {RMatrix#Basic Matrix Operations}{Basic Matrix
    Operations}
*/

/*!
    \fn double RMatrix::dy() const

    Returns the vertical translation factor.

    \sa translate(), {RMatrix#Basic Matrix Operations}{Basic Matrix
    Operations}
*/


/*!
    Maps the given coordinates \a x and \a y into the coordinate
    system defined by this matrix. The resulting values are put in *\a
    tx and *\a ty, respectively.

    The coordinates are transformed using the following formulas:

    \snippet doc/src/snippets/code/src_gui_painting_RMatrix.cpp 1

    The point (x, y) is the original point, and (x', y') is the
    transformed point.

    \sa {RMatrix#Basic Matrix Operations}{Basic Matrix Operations}
*/

void RMatrix::map(double x, double y, double *tx, double *ty) const
{
    MAPDOUBLE(x, y, *tx, *ty);
}



/*!
    \overload

    Maps the given coordinates \a x and \a y into the coordinate
    system defined by this matrix. The resulting values are put in *\a
    tx and *\a ty, respectively. Note that the transformed coordinates
    are rounded to the nearest integer.
*/

void RMatrix::map(int x, int y, int *tx, int *ty) const
{
    MAPINT(x, y, *tx, *ty);
}

/*!
    \fn RPoint RMatrix::map(const RPoint &point) const
    \overload

    Creates and returns a RPoint object that is a copy of the given \a
    point, mapped into the coordinate system defined by this
    matrix. Note that the transformed coordinates are rounded to the
    nearest integer.
*/

/*!
    Resets the matrix to an identity matrix, i.e. all elements are set
    to zero, except \c m11 and \c m22 (specifying the scale) which are
    set to 1.

    \sa RMatrix(), isIdentity(), {RMatrix#Basic Matrix
    Operations}{Basic Matrix Operations}
*/

void RMatrix::reset()
{
    _m11 = _m22 = 1.0;
    _m12 = _m21 = _dx = _dy = 0.0;
}
/*!
    \fn bool RMatrix::isIdentity() const

    Returns true if the matrix is the identity matrix, otherwise
    returns false.

    \sa reset()
*/

/*!
    Moves the coordinate system \a dx along the x axis and \a dy along
    the y axis, and returns a reference to the matrix.

    \sa setMatrix()
*/

RMatrix &RMatrix::translate(double dx, double dy)
{
    _dx += dx*_m11 + dy*_m21;
    _dy += dy*_m22 + dx*_m12;
    return *this;
}

/*!
    \fn RMatrix &RMatrix::scale(double sx, double sy)

    Scales the coordinate system by \a sx horizontally and \a sy
    vertically, and returns a reference to the matrix.

    \sa setMatrix()
*/

RMatrix &RMatrix::scale(double sx, double sy)
{
    _m11 *= sx;
    _m12 *= sx;
    _m21 *= sy;
    _m22 *= sy;
    return *this;
}

/*!
    Shears the coordinate system by \a sh horizontally and \a sv
    vertically, and returns a reference to the matrix.

    \sa setMatrix()
*/

RMatrix &RMatrix::shear(double sh, double sv)
{
    double tm11 = sv*_m21;
    double tm12 = sv*_m22;
    double tm21 = sh*_m11;
    double tm22 = sh*_m12;
    _m11 += tm11;
    _m12 += tm12;
    _m21 += tm21;
    _m22 += tm22;
    return *this;
}

const double deg2rad = double(0.017453292519943295769);        // pi/180

/*!
    \fn RMatrix &RMatrix::rotate(double degrees)

    Rotates the coordinate system the given \a degrees
    counterclockwise.

    Note that if you apply a RMatrix to a point defined in widget
    coordinates, the direction of the rotation will be clockwise
    because the y-axis points downwards.

    Returns a reference to the matrix.

    \sa setMatrix()
*/

RMatrix &RMatrix::rotate(double a)
{
    double sina = 0;
    double cosa = 0;
    if (a == 90. || a == -270.)
        sina = 1.;
    else if (a == 270. || a == -90.)
        sina = -1.;
    else if (a == 180.)
        cosa = -1.;
    else{
        double b = deg2rad*a;                        // convert to radians
        sina = sin(b);               // fast and convenient
        cosa = cos(b);
    }
    double tm11 = cosa*_m11 + sina*_m21;
    double tm12 = cosa*_m12 + sina*_m22;
    double tm21 = -sina*_m11 + cosa*_m21;
    double tm22 = -sina*_m12 + cosa*_m22;
    _m11 = tm11; _m12 = tm12;
    _m21 = tm21; _m22 = tm22;
    return *this;
}

/*!
    \fn bool RMatrix::isInvertible() const

    Returns true if the matrix is invertible, otherwise returns false.

    \sa inverted()
*/

/*!
    \fn double RMatrix::determinant() const

    Returns the matrix's determinant.
*/

/*!
    \fn RMatrix RMatrix::invert(bool *invertible) const

    Returns an inverted copy of this matrix.

    Use the inverted() function instead.
*/

/*!
    Returns an inverted copy of this matrix.

    If the matrix is singular (not invertible), the returned matrix is
    the identity matrix. If \a invertible is valid (i.e. not 0), its
    value is set to true if the matrix is invertible, otherwise it is
    set to false.

    \sa isInvertible()
*/

RMatrix RMatrix::inverted(bool *invertible) const
{
    double dtr = determinant();
    if (dtr == 0.0) {
        if (invertible)
            *invertible = false;                // singular matrix
        return RMatrix(true);
    }
    else {                                        // invertible matrix
        if (invertible)
            *invertible = true;
        double dinv = 1.0/dtr;
        return RMatrix((_m22*dinv),        (-_m12*dinv),
                       (-_m21*dinv), (_m11*dinv),
                       ((_m21*_dy - _m22*_dx)*dinv),
                       ((_m12*_dx - _m11*_dy)*dinv),
                       true);
    }
}


/*!
    \fn bool RMatrix::operator==(const RMatrix &matrix) const

    Returns true if this matrix is equal to the given \a matrix,
    otherwise returns false.
*/

bool RMatrix::operator==(const RMatrix &m) const
{
    return _m11 == m._m11 &&
           _m12 == m._m12 &&
           _m21 == m._m21 &&
           _m22 == m._m22 &&
           _dx == m._dx &&
           _dy == m._dy;
}

/*!
    \fn bool RMatrix::operator!=(const RMatrix &matrix) const

    Returns true if this matrix is not equal to the given \a matrix,
    otherwise returns false.
*/

bool RMatrix::operator!=(const RMatrix &m) const
{
    return _m11 != m._m11 ||
           _m12 != m._m12 ||
           _m21 != m._m21 ||
           _m22 != m._m22 ||
           _dx != m._dx ||
           _dy != m._dy;
}

/*!
    \fn RMatrix &RMatrix::operator *=(const RMatrix &matrix)
    \overload

    Returns the result of multiplying this matrix by the given \a
    matrix.
*/

RMatrix &RMatrix::operator *=(const RMatrix &m)
{
    double tm11 = _m11*m._m11 + _m12*m._m21;
    double tm12 = _m11*m._m12 + _m12*m._m22;
    double tm21 = _m21*m._m11 + _m22*m._m21;
    double tm22 = _m21*m._m12 + _m22*m._m22;

    double tdx  = _dx*m._m11  + _dy*m._m21 + m._dx;
    double tdy =  _dx*m._m12  + _dy*m._m22 + m._dy;

    _m11 = tm11; _m12 = tm12;
    _m21 = tm21; _m22 = tm22;
    _dx = tdx; _dy = tdy;
    return *this;
}

/*!
    \fn RMatrix RMatrix::operator *(const RMatrix &matrix) const

    Returns the result of multiplying this matrix by the given \a
    matrix.

    Note that matrix multiplication is not commutative, i.e. a*b !=
    b*a.
*/

RMatrix RMatrix::operator *(const RMatrix &m) const
{
    double tm11 = _m11*m._m11 + _m12*m._m21;
    double tm12 = _m11*m._m12 + _m12*m._m22;
    double tm21 = _m21*m._m11 + _m22*m._m21;
    double tm22 = _m21*m._m12 + _m22*m._m22;

    double tdx  = _dx*m._m11  + _dy*m._m21 + m._dx;
    double tdy =  _dx*m._m12  + _dy*m._m22 + m._dy;
    return RMatrix(tm11, tm12, tm21, tm22, tdx, tdy, true);
}

/*!
    Assigns the given \a matrix's values to this matrix.
*/
RMatrix &RMatrix::operator=(const RMatrix &matrix)
{
    _m11 = matrix._m11;
    _m12 = matrix._m12;
    _m21 = matrix._m21;
    _m22 = matrix._m22;
    _dx  = matrix._dx;
    _dy  = matrix._dy;
    return *this;
}