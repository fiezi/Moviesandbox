#ifndef H_CALLIGRAPHYFILTER
#define H_CALLIGRAPHYFILTER

#include "brushFilter.h"

class CalligraphyFilter: public BrushFilter{

public:

    CalligraphyFilter();
    CalligraphyFilter(DrawTool* myTool);
    virtual ~CalligraphyFilter();

    virtual void filter(vertexData* myVData);
};
#endif // CALLIGRAPHYFILTER
