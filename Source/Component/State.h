/*
  ==============================================================================

    State.h
    Created: 24 Apr 2024 4:19:22pm
    Author:  USER

  ==============================================================================
*/

/*
  ==============================================================================

    HorizontalMeter.h
    Created: 24 Feb 2024 2:04:05am
    Author:  HomePC

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LockFreeQueue.h"


namespace NeuronState
{
	class State : public Component
	{
	public:

	    State()
	    {
	       for (int i = 0; i < numLines; i++ )
            {
                std::deque<float> line;
                for (int j = 0; j < numPointsPerLine ; j++)
                {
                    line.push_back(0.f);
                }
                
                lines.push_back(line);     
                    
            }
	       
	    }
	    
	    ~State()
	    {
	    }
	    
	    	    
	    void paint(Graphics& g) override
	    {
	        //g.fillAll(juce::Colours::white); 
            g.setColour(juce::Colours::white);
            
            // Set up drawing parameters
            int marginLeft = 460;
            int marginRight = 40;
            int marginTop = 20;
            int marginBottom = 60;
            int width = getWidth() - marginLeft - marginRight;
            int height = getHeight() - marginTop - marginBottom;
            int xTickCount = 10; // Number of tick marks on X axis
            int yTickCount = 10; // Number of tick marks on Y axis
            float displayTimeInSeconds = 5; //Display for 5 second
            
            
            // Draw X and Y axis
            g.setColour(juce::Colours::white);
            g.drawLine(marginLeft, marginTop + height, marginLeft + width, marginTop + height, 2); // X axis
            g.drawLine(marginLeft, marginTop, marginLeft, marginTop + height, 2); // Y axis

            // Draw X and Y axis labels
            g.setFont(8.0f);
            g.drawText("Time (s)", getWidth()*3 / 4 - 50, getHeight() - marginBottom / 2 - 10, 100, 20, juce::Justification::centred); // X axis label
            g.drawText("Volume (Units)", marginLeft - 80, marginTop / 2 - 10, 100, 20, juce::Justification::centred, true); // Y axis label
            
        
            
            // Draw X axis tick marks and labels
            for (int i = 0; i <= xTickCount; ++i)
            {
                
                float x = marginLeft + i * width / (float)xTickCount;
                timeInSeconds = i* displayTimeInSeconds / xTickCount;
                 // Calculate the time for each tick mark relative to the initial time
                g.drawLine(x, marginTop + height - 5, x, marginTop + height + 5);
                g.drawText(juce::String(timeInSeconds, 2), x - 20, marginTop + height + 10, 40, 20, juce::Justification::centred);  
                
            }

            // Draw Y axis tick marks and labels
            for (int i = 0; i <= yTickCount; ++i)
            {
                float y = marginTop + i * height / (float)yTickCount;
                float value = 1.0f - i / (float)yTickCount; 
                g.drawLine(marginLeft - 5, y, marginLeft + 5, y);
                g.drawText(juce::String((value * 2 -1), 2), marginLeft - 30, y - 10, 20, 20, juce::Justification::centredRight);
            }
            
            
            
            //g.drawText(juce::String(count), getWidth()*3 / 4 - 50, getHeight() - marginBottom / 2, 100, 20, juce::Justification::centred);  
            
           
            
            
            if(state.size()!= 0 )
	        {   /*
	            float max = 0;
                float min = color[0];
    
                for(int i = 0; i < color.size(); i++)
                {
                    if(color[i] > max)
                    {
                        max = color[i];
                    }
                }
        
                for(int i = 0; i < color.size(); i++)
                {
        
                    if (color[i] < min)
                    {
                        min = color[i];
                    }
                }
                */

        
	            
	            
	            for (int i = 0; i< lines.size(); i++)
                {
                
                    
                    while (lines[i].size() > numPointsPerLine )
                    {
                        lines[i].pop_front(); // Remove oldest points if queue size exceeds maximum 
                    
                    }
                    lines[i].push_back(state[i]);
                    
                //g.drawText(juce::String(state.size()), getWidth()*3 / 4 - 50, getHeight() - marginBottom / 2 - 30, 100, 20, juce::Justification::centred);  
                
                }
                
                //Draw 16 Lines
                for (int j = lines[0].size()-1; j < lines[0].size(); j--)
                {
                    count_paint++;
                 
                
                    g.setColour(Colours::blue);
                    for (int i = 0 ; i < lines.size() ; i++)
                    {
                    //g.fillEllipse(((marginLeft)+(i/HZ)* width / displayTimeInSeconds) - 2, (marginTop+height)-((dataQueue[i].y- minEle) / (maxEle - minEle))*height - 2, 4, 4);
                        if(j == lines[i].size()-1 )
                        {
                            g.setColour(Colour::fromHSV(color[i], 0.8f, 0.8f, 1.0f));     
                            g.drawLine(((marginLeft)+(j/HZ)* width / displayTimeInSeconds),(marginTop+height)-((lines[i][j]+1)/2)*height,
                         ((marginLeft)+(j/HZ)* width / displayTimeInSeconds),(marginTop+height)-((lines[i][j]+1)/2)*height,2);
                            g.drawText(juce::String(color[i]), getWidth()*3 / 4 - 50, getHeight() - marginBottom / 2 - 30, 100, 20, juce::Justification::centred);  
                        }
                    
                        else
                        {
                            g.setColour(Colour::fromHSV(color[i] , 0.8f, 0.8f, 1.0f)); 
                            g.drawLine(((marginLeft)+((j+1)/HZ)* width / displayTimeInSeconds),(marginTop+height)-((lines[i][j+1]+1)/2)*height,
                         ((marginLeft)+(j/HZ)* width / displayTimeInSeconds),(marginTop+height)-((lines[i][j]+1)/2)*height,2);
                        }
                        
                    }
                    
                }
                
               
	        
	        }
  
	    }
	    
	    void setState(std::vector<float> value) 
		{ 
		    state = value;
		    count++; 
		}
		
		void setColor(std::vector<float> value)
        {
           color = value;
        }
		
	

		
		double HZ = 30.0;
        
        
	private:
		std::vector<float> state; 
		std::vector<float> color; 
		double count = 0;
		int count_paint = 0;
        float timeInSeconds = 0.f;
        static constexpr int maxDataPoints = 150; // Maximum number of data points to keep
        
        static constexpr int numLines = 16;
        static constexpr int numPointsToShow = 30 * 5;
        static constexpr int numPointsPerLine = 30 * 5; // 5 seconds at 30Hz
        std::vector<std::deque<float>> lines;
        std::deque<Colour> colours;

	
	};
}