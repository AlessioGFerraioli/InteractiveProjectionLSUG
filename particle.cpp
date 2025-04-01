﻿#include "particle.h"

particle::particle()
{
}

void particle::setup(string iconPath)
{
	// initialize with random position
	pos.x = ofRandomWidth();
	pos.y = ofRandomHeight();
	// and random velocity
	vel.x = ofRandom(-4, 4);
	vel.y = ofRandom(-4, 4);
	// and random angle of rotation
	angle = ofRandom(-10, 10);

	//initialize the force to zero (will update the force in update())
	frc = ofVec2f(0, 0);

	// initialize the random unique valuye
	uniqueVal.x = ofRandom(-1000, 1000);
	uniqueVal.y = ofRandom(-1000, 1000);

	// initialize random size
	size = ofRandom(1, 3);

	// inizializza drag che conunteracts speed
	drag = ofRandom(0.97, 0.99);

	icon.load(iconPath);

}

void particle::update(float speed, float noise, bool repulsive, float forceIntensity)
{
	// update the force with noise. we use unique val for have a specific force for each particle; also use ofGetElapsedTimeMillis so the force cahanges every frame
	frc.x = ofSignedNoise(uniqueVal.x, ofGetElapsedTimeMillis());
	frc.y = ofSignedNoise(uniqueVal.y, ofGetElapsedTimeMillis());


	// use the drag to reduce velocity
	vel *= drag;

	// add the repel or attractive force depending on bool repulsive, multiplied by forceIntensity
	if (repulsive == false) {
		vel += repelFrc * forceIntensity; // i use the plus sign to attract instead of repel
	}
	else {
		vel -= repelFrc * forceIntensity;
	}

	// use the force to increment velocity times the noise factor
	vel += frc * noise;

	// RESET AT BORDER
	// if particle hits the border it reappers on the other side
	if (pos.x + vel.x > ofGetWidth()) {
		pos.x -= ofGetWidth();
	}
	else if (pos.x + vel.x < 0) {
		pos.x += ofGetWidth();
	}

	if (pos.y + vel.y > ofGetHeight()) {
		pos.y -= ofGetHeight();
	}
	else if (pos.y + vel.y < 0) {
		pos.y += ofGetHeight();
	}


	// UPDATE POSITION
	// update position depending on velocity and speed
	pos += vel * speed;

	// UPDATE ROTATION
	// update the angle of rotation with random noise
	angle = sin(cos(uniqueVal.y) * ofGetElapsedTimeMillis()*0.001)*15; //ofSignedNoise(uniqueVal.x, ofGetElapsedTimeMillis()) * 0.3 + 

}

void particle::draw(float sizeDot)
{

	// here we choose how to draw particles
	// draw them as circles, for example
	//ofDrawCircle(pos.x, pos.y, size * sizeDot);
	// i will draw them as my witch icon


	ofPushMatrix();
	ofTranslate(pos.x + icon.getWidth()/2, pos.y + icon.getHeight()*0.75, 0);//move pivot to centre
		ofRotate(angle, 0, 0, 1);//rotate from centre
		ofPushMatrix();
		ofTranslate(- pos.x - icon.getWidth()/2, -pos.y - icon.getHeight() * 0.75, 0);//move back by the centre offset
			icon.draw(pos.x, pos.y, size * sizeDot, size * sizeDot); // draw icon
		ofPopMatrix();
	ofPopMatrix();
}

void particle::repel(vector<ofVec2f> repelPt)
{
	// every frame reset the repel force to zero
	repelFrc.set(0, 0);

	for (int i = 0; i < repelPt.size(); i++) {

		float dist = pos.distance(repelPt[i]); // use the distance function of ofVec2f

		// se la distanza � bassa, la forza agisce, altrimenti no
		if (dist < 150) {

			ofVec2f newRepelPt;
			newRepelPt = repelPt[i] - pos; // calculate the direction and intensity of the repel frc as the difference between the repelpoint and the particle
			repelFrc += newRepelPt * 0.0001;

		}
	}
}
