#ifndef H_SAVEBUTTON
#define H_SAVEBUTTON
#include <listButton.h>

class SaveButton: public ListButton
{
public:

        SaveButton();
        virtual ~SaveButton();

        virtual void clickedLeft();
        virtual void assembleSaveList();
        virtual void trigger(Actor * other);
        virtual void saveFile(string filename);
        virtual void create();
};
#endif
