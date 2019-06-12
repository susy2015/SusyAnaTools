//follow root dPhi calculation: https://root.cern.ch/root/html/src/ROOT__Math__VectorUtil.h.html#60
float my_dPhi(float phi1, float phi2)
{
const float pi = 3.1415926;
float dPhi = phi1 - phi2;
if(dPhi > pi) dPhi = dPhi - 2*pi;
else if (dPhi <= -pi) dPhi = dPhi + 2*pi;
return fabs(dPhi);
}

struct my_jet_struct {float jet_pt; float jet_dPhi;};
bool pt_descending_sort (my_jet_struct i, my_jet_struct j)
{ return (i.jet_pt > j.jet_pt); }
