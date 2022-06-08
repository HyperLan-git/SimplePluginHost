#pragma once

#include <string>

class SimplePluginHost {
   public:
    SimplePluginHost(std::string file);

    ~SimplePluginHost();

   private:
    void* pluginInstance = NULL;
};