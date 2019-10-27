namespace dv {
inline double
SignedDistanceToPlane(const double normal[3],
                      const double origin[3],
                      const double x[3])
{
  return normal[0] * (x[0] - origin[0]) + normal[1] * (x[1] - origin[1]) +
         normal[2] * (x[2] - origin[2]);
}
}
