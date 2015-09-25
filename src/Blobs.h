//
//  Blobs.h
//
//  Created by Tobias Ebsen on 22/07/15.
//
//

#pragma once

#include "ofMain.h"

#include "Scan.h"
#include "Plot.h"

namespace ofxSick {

    typedef struct {
        ofVec2f center;
        float mass;
    } Blob;


    class Blobs : public vector<Blob> {
    public:
        
        void update(Plot & scan, float threshold);
        void draw();
        
        void filterByMass(float min, float max);
        void filterShadows(float angle);
        void erase(int index);
        void groupByDistance(float maxDistance);
        
        vector<int> & getLabels();

    protected:
        vector<int> labels;
        int numUniqueLabels;
    };
}