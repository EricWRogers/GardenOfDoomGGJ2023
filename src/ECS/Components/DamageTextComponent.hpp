#pragma once
#include <cstring>
#include <iostream>

namespace DamageText
{
    // max size of unsigned short 65,535
    const unsigned char CHARTEXTLENGTH = 7;
}
struct DamageTextComponent {
    float countDown = 0.1f;
    char text[DamageText::CHARTEXTLENGTH];
};

namespace DamageText
{
    inline void SetText(DamageTextComponent& _component, const unsigned short &_damage)
    {
        sprintf(_component.text, "%d", _damage);

        /*for(int i = 0; i < CHARTEXTLENGTH; i++)
        {
            if (_component.text[i] == '\0')
            {
                _component.length = i;
            }
                
            
            //std::cout << _component.text[i] << std::endl;
        }*/
    }
}