#ifndef _R_MATRIX_H
#define _R_MATRIX_H

class RMatrix // 2D transform matrix
{
public:
    RMatrix();
    RMatrix(double m11, double m12, double m21, double m22,
            double dx, double dy);
    RMatrix(const RMatrix &matrix);

    void setMatrix(double m11, double m12, double m21, double m22,
                   double dx, double dy);

    double m11() const { return _m11; }
    double m12() const { return _m12; }
    double m21() const { return _m21; }
    double m22() const { return _m22; }
    double dx() const { return _dx; }
    double dy() const { return _dy; }

    void map(int x, int y, int *tx, int *ty) const;
    void map(double x, double y, double *tx, double *ty) const;


    void reset();
    inline bool isIdentity() const;

    RMatrix &translate(double dx, double dy);
    RMatrix &scale(double sx, double sy);
    RMatrix &shear(double sh, double sv);
    RMatrix &rotate(double a);

    double determinant() const { return _m11*_m22 - _m12*_m21; }

    RMatrix inverted(bool *invertible = 0) const;

    bool operator==(const RMatrix &) const;
    bool operator!=(const RMatrix &) const;

    RMatrix &operator*=(const RMatrix &);
    RMatrix operator*(const RMatrix &o) const;
    RMatrix &operator=(const RMatrix &);

private:
    inline RMatrix(bool)
            : _m11(1.)
            , _m12(0.)
            , _m21(0.)
            , _m22(1.)
            , _dx(0.)
            , _dy(0.) {}
    inline RMatrix(double am11, double am12, double am21, double am22, double adx, double ady, bool)
            : _m11(am11)
            , _m12(am12)
            , _m21(am21)
            , _m22(am22)
            , _dx(adx)
            , _dy(ady) {}

    double _m11, _m12;
    double _m21, _m22;
    double _dx, _dy;
};

// ËÄÉáÎåÈë
inline int rRound(double d)
{ return d >= double(0.0) ? int(d + double(0.5)) : int(d - int(d-1) + double(0.5)) + int(d-1); }

#endif // RMATRIX_H
