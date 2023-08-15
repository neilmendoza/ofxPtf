/*
 *  ParallelTransportFrames.cpp
 *
 *  Copyright (c) 2012, Neil Mendoza, http://www.neilmendoza.com
 *  All rights reserved. 
 *  
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions are met: 
 *  
 *  * Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer. 
 *  * Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the distribution. 
 *  * Neither the name of Neil Mendoza nor the names of its contributors may be used 
 *    to endorse or promote products derived from this software without 
 *    specific prior written permission. 
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 *  POSSIBILITY OF SUCH DAMAGE. 
 *
 */
#include "ParallelTransportFrames.h"

namespace nm
{
    ParallelTransportFrames::ParallelTransportFrames() :
        maxPoints(4), maxFrames(numeric_limits<unsigned>::max())
    {
    }
    
    bool ParallelTransportFrames::addPoint(float x, float y, float z)
    {
        return addPoint(glm::vec3(x, y, z));
    }
    
    bool ParallelTransportFrames::addPoint(const glm::vec3& point)
    {
        points.push_back(point);
        while (points.size() > maxPoints) points.pop_front();
        if (points.size() == 3) firstFrame();
        else if (points.size() > 3)
        {
            nextFrame();
            return true;
        }
        return false;
    }
                          
    void ParallelTransportFrames::firstFrame()
    {
        glm::vec3 t = glm::normalize(points[1] - points[0]);
        glm::vec3 n = glm::normalize(glm::cross(t, points[2] - points[0]));
        
        if (glm::any(glm::isnan(n)) || glm::length2(n) == 0.0f)
        {
            int i = fabs(t[0]) < fabs(t[1])?0:1;
            if (fabs(t[2]) < fabs(t[i])) i = 2;

            glm::vec3 v(0.f);
            v[i] = 1.f;
            n = glm::normalize(glm::cross(t, v));
        }

        glm::vec3 b = glm::cross(t, n);

        glm::mat4 m(t[0], t[1], t[2], 0.f,
            b[0], b[1], b[2], 0.0,
            n[0], n[1], n[2], 0.0,
            points[0][0], points[0][1], points[0][2], 1.f);
    
        frames.push_back(m);
        
        prevTangent = t;
        startNormal = n;
    }
    
    void ParallelTransportFrames::nextFrame()
    {
        curTangent = points.back() - points[points.size() - 2];
        glm::vec3 axis;
        float angle = 0;
        
        if(glm::length2(prevTangent) != 0.f && glm::length2(curTangent) != 0.f)
        {
            curTangent = glm::normalize(curTangent);
            
            float dot = glm::dot(prevTangent, curTangent);
            
            if( dot > 1.f ) dot = 1.f; 
            else if( dot < -1.0 ) dot = -1.0;
            
            angle = acos( dot );
            axis = glm::cross(prevTangent, curTangent);
        }
        
        if (glm::length2(axis) != 0.f && angle != 0.f)
        {
            glm::mat4 r = glm::mat4(1.f);
            r = glm::rotate(r, angle, axis);
            glm::mat4 tj = glm::translate(points.back());
            glm::mat4 ti = glm::translate(-points[points.size() - 2]);
            
            frames.push_back(tj * r * ti * frames.back());
        }
        else
        {
            glm::mat4 tr = glm::translate(points.back() - points[points.size() - 2]);
            
            frames.push_back(tr * frames.back());
        }
        prevTangent = curTangent;
        while (frames.size() > maxFrames) frames.pop_front();
    }
    
    glm::mat3 ParallelTransportFrames::normalMatrix() const
    {
        glm::mat3 transform3(frames.back());
        return glm::inverseTranspose(transform3);
    }
    
    ofVec3f ParallelTransportFrames::calcCurrentNormal() const
    {
        return normalMatrix() * getStartNormal();
    }
    
    void ParallelTransportFrames::debugDraw(float axisSize)
    {
        ofSetColor(0, 255, 255);
        ofNoFill();
        for (int i = 0; i < frames.size(); ++i)
        {
            ofPushMatrix();
            ofMultMatrix(frames[i]);
            ofRotateDeg(90, 0, 1, 0);
            ofDrawCircle(0, 0, axisSize * 2.f);
            ofDrawAxis(axisSize);
            ofPopMatrix();
        }
    }
    
    void ParallelTransportFrames::clear()
    {
        points.clear();
        frames.clear();
    }
}
