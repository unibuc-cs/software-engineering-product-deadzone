#pragma once

#include <vector>
#include <string>

#include "Texture.h"

class Flipbook
{
public:
	Flipbook();
	~Flipbook() = default;

	Flipbook(const Flipbook& other) = default;
	Flipbook& operator= (const Flipbook& other) = default;

	inline void setFramesPerSecond(const double& framesPerSecond) { this->framesPerSecond = framesPerSecond; }
	inline void addFrame(const std::string& texture) { keyFrames.push_back(texture); }
	inline void setLoop(bool loop) { this->loop = loop; }

	Texture2D& getTextureAtTime(double time) const;
	Texture2D& getTextureAtIndex(int index) const;
	int getNumFrames() const;
	bool getIsFinished(double time) const;

private:
	double framesPerSecond;
	std::vector<std::string> keyFrames;
	bool loop;

	double getTotalDuration() const;
	int getKeyFrameIndexAtTime(double time) const;
};

