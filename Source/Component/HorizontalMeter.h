/*
  ==============================================================================

    HorizontalMeter.h
    Created: 24 Feb 2024 2:04:05am
    Author:  HomePC

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Gui
{
	class HorizontalMeter : public Component
	{
	public:

		void paint(Graphics& g) override
		{
			auto bounds = getLocalBounds().toFloat();

			g.setColour(Colours::white.withBrightness(0.4f));
			g.fillRoundedRectangle(bounds, 5.f);

			g.setColour(Colours::white);
			const auto scaledX = jmap(level, 0.f, 1.f, 0.f, static_cast<float>(getWidth()));
			g.fillRoundedRectangle(bounds.removeFromLeft(scaledX), 5.f);
		}
		
		void setLevel(const float value) { level = value; }
	private:
		float level = 0.f; 
	};
}