//
//  CParseXML.hpp
//  R01C00
//
//  Created by 赵军岩 on 2017/3/12.
//  Copyright © 2017年 赵军岩. All rights reserved.
//

#ifndef CParseXML_hpp
#define CParseXML_hpp

#include <stdio.h>
#include "tinyxml.h"


class CParseXML
{
public:
    CParseXML();
    ~CParseXML();
    
public:
    
private:
    TiXmlDocument m_TixmlDoc;
};

#endif /* CParseXML_hpp */
