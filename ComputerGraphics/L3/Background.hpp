#pragma once
#include <AGL3Drawable.hpp>

// ==========================================================================
// Drawable object: no-data only: vertex/fragment programs
// ==========================================================================
class Background : public AGLDrawable {
public:
   Background();
   void setShaders();
   void draw(float scale, float time);
};