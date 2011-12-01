#ifndef H_IMPORTBITMAPBUTTON
#define H_IMPORTBITMAPBUTTON

#include "loadButton.h"

class ImportBitmapButton: public LoadButton{

public:

    ImportBitmapButton();
    virtual ~ImportBitmapButton();

    virtual void clickedLeft();
    virtual void mouseDrag();
    virtual void assembleLoadList();
    virtual void placeScrollBar();
    virtual void trigger(MsbObject*other);
    virtual void loadFile(string filename);

    virtual void assembleImage(FIBITMAP* myBitmap, int imageWidth, int imageHeight, float flip);
    virtual Vector3f genNormal(FIBITMAP* myBitmap, int w, int h, int width, int height, float flip);

    virtual void create();
};
#endif // IMPORTBITMAPBUTTON
