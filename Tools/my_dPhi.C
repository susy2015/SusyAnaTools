float my_dPhi(float phi1, float phi2)
{
const float pi = 3.1415926;
float dPhi = phi1 - phi2;
if(dPhi > pi) dPhi = dPhi - 2*pi;
else if (dPhi <= -pi) dPhi = dPhi + 2*pi;
return fabs(dPhi);
}
