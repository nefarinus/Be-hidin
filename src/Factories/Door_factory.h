#pragma once

#include <string>

class Door_factory {

   private:
   public:
    static void create(double      transform_x,
                       double      transform_y,
                       double      transform_h,
                       double      transform_w,
                       double      transform_scale,
                       std::string sprite_file_path);
};