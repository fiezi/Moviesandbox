#ifndef H_VIDEOTEXTUREACTOR
#define H_VIDEOTEXTUREACTOR

#include "actor.h"
#include "ofVideoPlayer.h"

class VideoTextureActor: public Actor{

public:

    VideoTextureActor();
    virtual ~VideoTextureActor();

    void registerProperties();

    virtual void setup();

    virtual void trigger(MsbObject* other);

    virtual void update(double deltaTime);

    virtual void stop();

    virtual void create();


    // load video
    void loadMovie(string fileName);

    ofVideoPlayer*   player;
    bool            bPlaying;
    bool            bReload;

    float           playSpeed;
    string          videoInfo;              //video.xml location

};
#endif // VIDEOTEXTUREACTOR
