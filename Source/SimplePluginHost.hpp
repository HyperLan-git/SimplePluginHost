#pragma once

#include <string>

class SimplePluginHost {
   public:
    SimplePluginHost(std::string file);

    const float** update();

    ~SimplePluginHost();

   private:
    void* pluginInstance = NULL;
    void* window = NULL;
};