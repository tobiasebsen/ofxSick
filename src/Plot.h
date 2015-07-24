//
//  Plot.h
//
//  Created by Tobias Ebsen on 22/07/15.
//
//

#pragma once
#include "ofMain.h"
#include "Scan.h"

namespace ofxSick {

    class Plot {
    public:
        void update(Scan & scan, bool flipHoriz = false, bool flipVert = false, float dataOffset = 0.f, bool removeZeros = false);
        void draw(bool asLines = false);
        
        size_t size() const;
        ofVec2f & operator[] (int index);
        const ofVec2f & operator[] (int index) const;
        
        float getResolution();

    protected:
        vector<ofVec2f> points;
        float resolution;
    };
}