/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2013 Open Ephys

    ------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef DACoutFPGACANVAS_H_
#define DACoutFPGACANVAS_H_

#include <VisualizerWindowHeaders.h>
#include "DACoutFPGA.h"

#include <vector>


class DACoutFPGACanvasHolder;
class DACoutFPGADisplay;
class Timescale;
class GraphUnit;
class LabelDisplay;
class HistoGraph;
class StatDisplay;


class DACoutFPGACanvas : public Visualizer, public Button::Listener

{
public:
	DACoutFPGACanvas(DACoutFPGA* n);
	~DACoutFPGACanvas();
    
    void paint(Graphics& g);
    void repaintDisplay();
    void refresh();
    void beginAnimation();
    void endAnimation();
    void refreshState();
    void setParameter(int, float) {}
    void setParameter(int, int, int, float) {}
    void update();
    void resized();
    bool keyPressed(const KeyPress& key);
    void buttonClicked(Button* button);
    void startRecording() { }; // unused
    void stopRecording() { }; // unused
    void setBin(int bin_);
    void setBinSize(int binSize_);
    void setData(double data_);
	DACoutFPGA* processor;

private:

	std::vector<std::vector<double>> histoData;
    std::vector<std::vector<float>> minMaxMean;
    void removeUnitOrBox();
    ScopedPointer<Viewport> viewport;
    ScopedPointer<DACoutFPGADisplay> display;
    ScopedPointer<UtilityButton> clearHisto;
    int scrollBarThickness;
    int border = 20;
    int triggerChannel = -1;
    unsigned long spikeBufferSize = 0;
    unsigned long ttlBufferSize = 0;
    int bin = 0;
    int binSize = 0;
    double data = 0;
	DACoutFPGACanvasHolder* holder;
    ScopedPointer<Timescale> scale;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DACoutFPGACanvas);

};

//---------------------------

class DACoutFPGADisplay : public Component
{
    
public:
	DACoutFPGADisplay(DACoutFPGACanvas* c, Viewport* v, DACoutFPGA* p);
	~DACoutFPGADisplay();
    void visibleAreaChanged (const Rectangle<int>& newVisibleArea);
    
    /** Callback method that is called when the viewed component is added, removed or swapped. */
    void viewedComponentChanged (Component* newComponent);
    void resized();
    void paint(Graphics &g);
    void refresh();
    int getNumGraphs();
private:

	DACoutFPGA * processor;
	DACoutFPGACanvas* canvas;
    Viewport* viewport;
    std::vector<GraphUnit*> graphs;
    juce::Colour channelColours[16];
	std::vector<std::vector<double>> histoData;
    Array<float *> minMaxMean;
    int border = 20;
};

//---------------------------

class Timescale : public Component
{
public:
    Timescale(int windowSize_, uint64 sampleRate_, double data = 0, int bin = 0 ,int binSize = 0 );
    ~Timescale();
    void paint(Graphics& g);
    void resized();
    void update(int windowSize, uint64 sR);
    void setBin(int bin_);
    void setData(double data_);
    void setBinSize(int binSize_);
private:
    int windowSize;
    uint64 sampleRate;
    int bin = 0;
    int binSize = 0;
    double data = 0;
    int lowerBin = 0;
    int upperBin = 0;
    
};


//---------------------------


class GraphUnit : public Component
{
public:
	GraphUnit(DACoutFPGA* processor_, DACoutFPGACanvas* canvas_, juce::Colour color_, String name_, float * stats_, std::vector<double> data_);
    ~GraphUnit();
    void paint(Graphics& g);
    void resized();
private:
    LabelDisplay* LD;
    HistoGraph* HG;
    StatDisplay* SD;
    Colour color;
};

//---------------------------

class LabelDisplay : public Component
{
public:
    LabelDisplay(Colour color,String n);
    ~LabelDisplay();
    void paint(Graphics& g);
    void resized();
private:
    Colour color;
    String name;
};

//---------------------------

class HistoGraph : public Component
{
    
public:
	HistoGraph(DACoutFPGA* processor_, DACoutFPGACanvas* canvas_, juce::Colour color_, uint64 bins_, float max_, std::vector<double> histoData_);
    ~HistoGraph();
    
    void paint(Graphics& g);
    void resized();
    
    void select();
    void deselect();
    void clear();
    void mouseMove(const MouseEvent &event);
    
    
private:
    uint64 bins = 0;
    Colour color;
    float max;
	std::vector<double> histoData;
    int valueY=0;
	DACoutFPGA* processor;
	DACoutFPGACanvas* canvas;
    
};

//---------------------------

class StatDisplay : public Component
{
public:
    StatDisplay(DACoutFPGA* display_, juce::Colour c, float * s);
    ~StatDisplay();
    void paint(Graphics& g);
    void resized();
private:
	DACoutFPGA * processor;
    Colour color;
    float const * stats;
    
};


#endif  // DACoutFPGACANVAS_H_
