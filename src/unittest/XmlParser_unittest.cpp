#include <tinyxml.h>
#include <iostream>

using namespace std;


int main(int argc, char** argv)
{
    char* conf = new char[1024];
    TiXmlDocument docSrc("GlobalConf.xml");
   
    docSrc.LoadFile(); 
    docSrc.Parse(conf);

    TiXmlElement* root = docSrc.RootElement();
    TiXmlNode* node = NULL;

    for (node = root->FirstChild("Config-Item");
            node;
            node = node->NextSibling("Config-Item"))
    {
        TiXmlElement* configElement = node->ToElement();
        if (NULL == configElement)
        {
            delete [] conf;
            conf = NULL;
            return -1;
        }

        if (NULL != configElement->Attribute("name"))
        {
            cout << "name: " << string(configElement->Attribute("name")) << endl;
            printf("**name: %s***\n", configElement->Attribute("name"));
        }

        if (NULL != configElement->Attribute("desc"))
        {
            cout << "desc: " << string(configElement->Attribute("desc")) << endl;
            printf("**desc: %s***\n", configElement->Attribute("desc"));
        }

        if (NULL != configElement->Attribute("ref"))
        {
            cout << "ref: " << string(configElement->Attribute("ref")) << endl;
        }
    }

    if (conf != NULL)
    {
        delete [] conf;
    }

    return 0;
}
