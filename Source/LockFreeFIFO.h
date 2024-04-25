/*
  ==============================================================================

    LockFreeFIFO.h
    Created: 22 Apr 2024 5:17:46pm
    Author:  USER
    https://www.youtube.com/watch?v=EKI0iB1hh6M&t=25s

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
/*
class LockFreeQueue()
{
    public:
    
    ScopedPointer <AbstractFifo> lookFreeFifo;
    Array <float> data;
    int lastReadPoint = 0;
    
    LockFreeQueue(){
        
        lookFreeFifo = new AbstractFifo(512);
        
        //clear
        data.ensureStorageAllocated(512);
        FloatVectorOperations::clear(data.getRawDataPointer(),512);
        
        while(data.size() < 512)
        {
            data.add(0.f);
        }
    }
        
    
    void setTotalSize (int newSize){
        
        lockFreeFifo -> setTotalSize(newSize);
        FloatVectorOperations::clear(data.getRawDataPointer(),newSize);
        
        while(data.size() < newSize)
        {
            data.add(0.f);
        }
    }
    
    void writeTo(const float* writeData, int numToWrite){
        
        int start1, start2, blockSize1, blockSize2;
        
        lockFreeFifo -> prepareToWrite(numToWrite, start1, blockSize1, start2, blockSize2);
        
        if(blockSize1 > 0) FloatVectorOperations::copy(data.getRawDataPointer() + start1, writeData, blockSize1);
        if(blockSize2 > 0) FloatVectorOperations::copy(data.getRawDataPointer() + start2, writeData + blockSize1, blockSize2);
        
        //Move the FIFO write head
        lockFreeFifo -> finishedWrite(numToWrite);
    }
    
    void readFrom(float* readData, int numtoRead){
        
        int start1, start2, blockSize1, blockSize2;
        lockFreeFifo -> prepareToRead(numToRead, start1, blockSize1, start2, blockSize2);
        
        if(blockSize1 > 0) {
            FloatVectorOperations::copy(readData, data.getRawDataPointer() + start1,blockSize1);
            lastReadPoint = start1 + blockSize1;
        }
        
        if(blockSize2 > 0) {
            FloatVectorOperations::copy(data.getRawDataPointer() + start2, writeData + blockSize1, blockSize2);
            lastReadPoint = start2 + blockSize2;
        }
        
        lockFreeFifo -> finishedRead(blockSize1 + blockSize2);
    }
    
    //Write the most recent bit into an array
    int writeToArray(Array<float>* dest, int destPoint){
      
        //Append any new data to a circular way
        
        //DRAIN the excess 
        while(getNumReady() > dest -> size())
        {
            lockFreeFifo -> finishedRead(getNumReady() - dest -> size());
        }
        
        //READ latest data from the LFQ
        const int numToAppend = getNumReady();
        
        //ADD the tail (one buffer's worth) to output
        if(destPoint + numToAppend < des -> size())
        {
            readFrom(&dest -> getRawDataPointer()[desPoint], numToAppend);
        }
        else
        {
            int toTheEnd = dest -> size() - destPoint;
            readFrom(&dest -> getRawDataPointer()[desPoint], toTheEnd);
            readFrom(&dest -> getRawDataPointer()[0], numToAppend - toTheEnd);
        }
        
        return numToAppend;
    }
        
      
        
        
    
    
}
*/