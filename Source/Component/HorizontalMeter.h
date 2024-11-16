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
	/*

		void paint(Graphics& g) override
		{
			auto bounds = getLocalBounds().toFloat();

			g.setColour(Colours::white.withBrightness(0.4f));
			g.fillRoundedRectangle(bounds, 5.f);

			g.setColour(Colours::white);
			const auto scaledX = jmap(level, 0.f, 1.f, 0.f, static_cast<float>(getWidth()));
			g.fillRoundedRectangle(bounds.removeFromLeft(scaledX), 5.f);
		}
	*/
	    HorizontalMeter()
	    {
	        open = true;
	       
	    }
	    
	    ~HorizontalMeter()
	    {
	    }
	    
	    
	    
	    void paint(Graphics& g) override
	    {
	        g.fillAll(juce::Colour::fromHSL(0.0f,0.0f,0.1f,1.0f)); 
            g.setColour(juce::Colours::white);
            
            // Set up drawing parameters
            int marginLeft = 60;
            int marginRight = 440;
            int marginTop = 20;
            int marginBottom = 60;
            int width = getWidth() - marginLeft - marginRight;
            int height = getHeight() - marginTop - marginBottom;
            int xTickCount = 10; // Number of tick marks on X axis
            int yTickCount = 10; // Number of tick marks on Y axis
            float displayTimeInSeconds = maxDataPoints/HZ; //Display for 5 second
            
            /*
            // Draw X and Y axis
            g.setColour(juce::Colours::white);
            g.drawLine(marginLeft, marginTop + height, marginLeft + width, marginTop + height, 2); // X axis
            g.drawLine(marginLeft, marginTop, marginLeft, marginTop + height, 2); // Y axis

            // Draw X and Y axis labels
            g.setFont(8.0f);
            g.drawText("Time (s)", getWidth() / 4 - 50, getHeight() - marginBottom / 2 - 10, 100, 20, juce::Justification::centred); // X axis label
            g.drawText("Output", marginLeft / 2 - 50, marginTop / 2 - 10, 100, 20, juce::Justification::centred, true); // Y axis label
            
        
            
            // Draw X axis tick marks and labels
            for (int i = 0; i <= xTickCount; ++i)
            {
                
                float x = marginLeft + i * width / (float)xTickCount;
                //display for certain second
                //timeInSeconds = i* displayTimeInSeconds / xTickCount + (int(count/HZ/int(displayTimeInSeconds)))*int(displayTimeInSeconds) ;
                //display until current time
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
                g.drawText(juce::String(value, 2), marginLeft - 30, y - 10, 20, 20, juce::Justification::centredRight);

            }
            */
        
            //Initialize the queue
            if(open==true)
	        {
	            for(int i = 0 ; i < 150 ; i++)
	            {
	                juce::Point<float> point((marginLeft),(marginTop+height));
	                dataQueue.push_back(point);
	            }
                open = false;
	        }
            
	        
	        x_current = (marginLeft)+(count/HZ)* width / displayTimeInSeconds ;
            y_current = (marginTop+height)-(level*height);
	        
	        juce::Point<float> point(x_current, y_current);
	        
	        dataQueue.push_back(point);
	        while (dataQueue.size() > maxDataPoints)
            {
                front = dataQueue.front();
                dataQueue.pop_front(); // Remove oldest points if queue size exceeds maximum
            }
	        

            g.setColour(juce::Colour::fromHSL(0.12f,0.97f,0.45f,1.0f));
            
            //Normalize y axis
            float maxEle = 0;
            float minEle = dataQueue[0].y;
            
            for(int i = 0 ; i < dataQueue.size() ; i++)
	        {
	            if(dataQueue[i].y > maxEle)
	            {
	                maxEle =dataQueue[i].y;
	            }
	        }
	        
	        for(int i = 0 ; i < dataQueue.size() ; i++)
	        {
	            if(dataQueue[i].y < minEle)
	            {
	                minEle =dataQueue[i].y;
	            }
	        }
            

            if(maxEle != minEle) 
            {
                for (int i = dataQueue.size()-1 ; i < dataQueue.size() ; i--)
                {
                   
                    if(i == dataQueue.size()-1 )
                    {
                            
                       g.drawLine(((marginLeft)+(i/HZ)* width / displayTimeInSeconds),(marginTop+height)-((dataQueue[i].y- minEle) / (maxEle - minEle))*height,
                         ((marginLeft)+(i/HZ)* width / displayTimeInSeconds),(marginTop+height)-((dataQueue[i].y- minEle) / (maxEle - minEle))*height,2.5);
                    }
                    
                    else
                    {
                       g.drawLine(((marginLeft)+((i+1)/HZ)* width / displayTimeInSeconds),(marginTop+height)-((dataQueue[i+1].y- minEle) / (maxEle - minEle))*height,
                         ((marginLeft)+(i/HZ)* width / displayTimeInSeconds),(marginTop+height)-((dataQueue[i].y- minEle) / (maxEle - minEle))*height,2.5);
                    }
                        
                 }
             }
                 
             else
             {
                for (int i = dataQueue.size()-1 ; i < dataQueue.size() ; i--)
                {
                    if(i == dataQueue.size()-1 )
                    {
                            
                       g.drawLine(((marginLeft)+(i/HZ)* width / displayTimeInSeconds),dataQueue[i].y,
                         ((marginLeft)+(i/HZ)* width / displayTimeInSeconds),dataQueue[i].y,2.5);
                    }
                    
                    else
                    {
                       g.drawLine(((marginLeft)+((i+1)/HZ)* width / displayTimeInSeconds),dataQueue[i+1].y,
                         ((marginLeft)+(i/HZ)* width / displayTimeInSeconds),dataQueue[i].y,2.5);
                    }
                        
                 }       
              }
	    }
	    		
		void setLevel(const float value) 
		{ 
		    level = value;
		    count++; 
		}
		
		
		double HZ = 10.0;
        int maxDataPoints = 150; // Maximum number of data points to keep
        
	private:
		float level = 0.f; 
		double count = 0;
		int count_paint = 0;
		float x_current = 0.f;
        float y_current = 0.f;
        float x_last = 0.f;
        float y_last = 0.f;
        bool open = false;
        float timeInSeconds = 0.f;
        std::deque<juce::Point<float>> dataQueue;
        juce::Point<float> front;
        
	
	};
}