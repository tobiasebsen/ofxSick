//
//  Tracker.cpp
//
//  Created by Tobias Ebsen on 12/08/15.
//
//

#include "Tracker.h"

using namespace ofxSick;

Tracker::Tracker() {
    uidcounter = 0;
    timeout = 1000*200; // 2/10 second
}

void Tracker::update(uint64_t now) {

    for (int j=0; j<this->size(); ) {
        TrackedBlob & blob = this->at(j);
        if (blob.time_last < now) { // estimation
            //blob.center += blob.velocity * ofGetElapsedTimef();
            //ofNotifyEvent(updateBlob, blob);
        }
        if (now - blob.time_last > timeout) {
            ofNotifyEvent(removeBlob, blob);
            this->erase(this->begin()+j);
        }
        else {
            j++;
        }
    }
}

void Tracker::track(vector<Blob> &newblobs, float tolerance) {
    
    uint64_t now = ofGetElapsedTimeMicros();

    for (int i=0; i<newblobs.size(); i++) {

        int index = -1;
        float mindist = FLT_MAX;

        for (int j=0; j<this->size(); j++) {
            float distance = newblobs[i].center.distance(this->at(j).center);
            if (distance <= tolerance && distance < mindist) {
                index = j;
                mindist = distance;
            }
        }
        
        if (index != -1) {
            TrackedBlob & blob = this->at(index);
            ofVec2f vel = newblobs[i].center - blob.center;
            vel *= ((double)(now - blob.time_last)) / 1000000.;
            blob.time_last = now;
            blob.velocity = vel;
            blob.acceleration = vel - blob.velocity;
            blob.center = newblobs[i].center;
            blob.mass = newblobs[i].mass;
            ofNotifyEvent(updateBlob, blob);
        }
        else {
            TrackedBlob blob;
            blob.uid = uidcounter++;
            blob.center = newblobs[i].center;
            blob.mass = newblobs[i].mass;
            blob.time_first = now;
            blob.time_last = now;
            this->push_back(blob);
            ofNotifyEvent(addBlob, blob);
        }
    }

    update(now);
}

void Tracker::draw(bool drawUid) {
    for (int j=0; j<this->size(); j++) {
        TrackedBlob & blob = this->at(j);
        ofEllipse(blob.center, blob.mass, blob.mass);
        if (drawUid)
            ofDrawBitmapString(ofToString(blob.uid), blob.center);
        ofLine(blob.center, blob.center + blob.velocity * 100.);
    }
}
