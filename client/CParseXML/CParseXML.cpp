//
//  CParseXML.cpp
//  R01C00
//
//  Created by 赵军岩 on 2017/3/12.
//  Copyright © 2017年 赵军岩. All rights reserved.
//

#include "CParseXML.hpp"
#include <string>
#include <iostream>
using namespace std;

const string xmlFilePath = "/myFile/01.myProgram/7312R01C00/client/ConfigFile/config.xml";
CParseXML::CParseXML()
{
    
}

CParseXML::~CParseXML()
{
    
}

void CParseXML::openXMLFile()
{
    if (!m_TixmlDoc.LoadFile(xmlFilePath.c_str())) {
        perror("load xml file failed");
        return;
    }
    TiXmlElement* rootElem = m_TixmlDoc.RootElement();
    cout<<rootElem->Attribute("version")<<endl;
    TiXmlElement* childElem = rootElem->FirstChildElement();
    TiXmlElement* subChildElem = childElem->FirstChildElement();
    cout<<subChildElem->Attribute("ip")<<endl;
}

void CParseXML::closeXMLFile()
{

}
