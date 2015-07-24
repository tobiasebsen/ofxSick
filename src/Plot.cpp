//
//  Plot.cpp
//  lidar
//
//  Created by Tobias Ebsen on 22/07/15.
//
//

#include "Plot.h"

using namespace ofxSick;

void Plot::update(ofxSick::Scan &scan, bool flipHoriz, bool flipVert, float dataOffset, bool removeZeros) {

    float flipHmult = flipHoriz ? -1 : 1;
    float flipVmult = flipVert ? -1 : 1;
    
    int channel = -1;
    int m = scan.aDataChannel16.uiFlexArrayLength;
    for (int i=0; i<m; i++) {
        if (scan.aDataChannel16.aFlexArrayData[i].DataChannelHdr.aContentType.data == "DIST1") {
            channel = i;
        }
    }
    
    if (channel == -1) {
        ofLogError("ofxSick::Plot") << "No distance data in scan";
        return;
    }
    
    auto & channel16 = scan.aDataChannel16.aFlexArrayData[channel];
    auto & header16 = channel16.DataChannelHdr;
    int n = channel16.aData.uiFlexArrayLength;
    
    float angle = ofDegToRad(header16.diStartAngle * 0.0001);
    resolution = header16.uiAngleRes * 0.0001;
    float step = ofDegToRad(resolution);
    
    points.resize(n);
    int k = 0;
    uint16_t data;

    for (int i=0; i<n; i++) {
        data = channel16.aData.aFlexArrayData[i];
        if (!removeZeros || data > 0) {
            ofVec2f & point = points[k];
            float d = data + dataOffset;
            point.x = cos(angle) * d * flipHmult;
            point.y = sin(angle) * d * flipVmult;
            k++;
        }
        angle += step;
    }
    points.resize(k);
}

void Plot::draw(bool asLines) {

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, points.data());
    glDrawArrays(asLines ? GL_LINE_STRIP : GL_POINTS, 0, points.size());
    glDisableClientState(GL_VERTEX_ARRAY);
}

size_t Plot::size() const {
    return points.size();
}

ofVec2f & Plot::operator[](int index) {
    return points[index];
}

const ofVec2f & Plot::operator[](int index) const {
    return points.at(index);
}

float Plot::getResolution() {
    return resolution;
}