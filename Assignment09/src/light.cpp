#include "light.h"

lightSource::lightSource()
{
    
}

entityLight::entityLight()
{
    for(int i=0;i<4;i++)
        emissive[i]=0.1;
    for(int i=0;i<4;i++)
        materialAmbient[i]=0.0;
    for(int i=0;i<4;i++)
        materialDiffuse[i]=0.7;
    for(int i=0;i<4;i++)
        materialSpecular[i]=0.3;
    shine = 30;
}
