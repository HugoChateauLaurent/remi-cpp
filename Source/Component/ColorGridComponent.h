#pragma once

#include <vector>
#include <juce_gui_basics/juce_gui_basics.h>

class ColorGridComponent : public juce::Component
{
public:
    // Constructor accepting a vector of colors, number of rows, and number of columns
    ColorGridComponent(const std::vector<juce::Colour>& colors, int rows, int cols)
        : colorData(colors), numRows(rows), numCols(cols)
    {
        // Ensure the number of colors matches the grid dimensions
        jassert(colorData.size() == numRows * numCols);
    }

    // Method to update the colors displayed in the grid
    void updateColors(const std::vector<juce::Colour>& newColors)
    {
        jassert(newColors.size() == static_cast<size_t>(numRows * numCols));
        colorData = newColors;
        repaint();
    }

    // Override of the paint method to render the grid
    void paint(juce::Graphics& g) override
    {
        auto area = getLocalBounds();
        auto cellWidth = area.getWidth() / numCols;
        auto cellHeight = area.getHeight() / numRows;

        for (int row = 0; row < numRows; ++row)
        {
            for (int col = 0; col < numCols; ++col)
            {
                auto colorIndex = row * numCols + col;
                auto color = colorData[colorIndex];

                auto cellArea = juce::Rectangle<int>(col * cellWidth, row * cellHeight, cellWidth, cellHeight);
                g.setColour(color);
                g.fillRect(cellArea);
            }
        }
    }

private:
    std::vector<juce::Colour> colorData; // Vector holding the colors for the grid
    int numRows;                         // Number of rows in the grid
    int numCols;                         // Number of columns in the grid

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ColorGridComponent)
};
