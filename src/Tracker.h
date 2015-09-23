//
//  Tracker.h
//
//  Created by Tobias Ebsen on 12/08/15.
//
//

#pragma once

#include "ofMain.h"
#include "Blobs.h"

namespace ofxSick {

    typedef struct : public Blob {
        int uid;
        ofVec2f acceleration;
        ofVec2f velocity;
        uint64_t time_first;
        uint64_t time_last;
    } TrackedBlob;

    
    class Tracker : public vector<TrackedBlob> {
    public:
        Tracker();

        void update(uint64_t now = ofGetElapsedTimeMicros());
        void track(vector<Blob> & blobs, float tolerance);
        void draw(bool drawUid = true);

        ofEvent<TrackedBlob> addBlob;
        ofEvent<TrackedBlob> updateBlob;
        ofEvent<TrackedBlob> removeBlob;

    protected:
        int uidcounter;
        uint32_t timeout;
    };
}