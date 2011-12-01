#include "Animation.h"

namespace sf {
    Animation::Animation() {
        // Just in case ;)
        Clear();
    }
    Animation::~Animation() {
        
    }
    void Animation::Update() {
        // Increment the current frame within the animation that we are on
        CurrentFrame++;
        // And the frame of the current image that we are on
        CurrentImageFrame++;
        // If we have been on the frame as long as we should of
        if (CurrentImageFrame > ImageLengths.at(CurrentImage)) {
            // Go to the next image
            CurrentImage++;
            // We are now on frame 0 of this image
            CurrentImageFrame = 0;
            // If this is more frames then we have
            if (CurrentImage >= Images.size()) {
                // Restart the animation
                CurrentImage = 0;
                CurrentImageFrame = 0;
                CurrentFrame = 0;
            }
            // Change the image
            SetImage(*Images.at(CurrentImage));
        }
    }
    void Animation::Draw(RenderTarget& Target) {
        Target.Draw(*this);
    }
    bool Animation::AddFrame(sf::Image* NewFrame, unsigned int Length) {
        Images.push_back(NewFrame);
        ImageLengths.push_back(Length);
        if (Images.size() == 1) {
            // We shouldn't really have to do this, but if we don't, we end up with wierd problems...
            SetSubRect(sf::Rect<int>(0,0,NewFrame->GetWidth(), NewFrame->GetHeight()));
            SetImage(*Images.at(CurrentImage));
        }
        return true;
    }
    bool Animation::InsertFrame(sf::Image* NewFrame, unsigned int Length, unsigned int Position) {
        std::vector<sf::Image*>::iterator Imgiter = Images.begin();
        std::vector<unsigned int>::iterator Intiter = ImageLengths.begin();
        if ((Position < Images.size()) && (Position >= 0)) {
            Imgiter += Position;
            Intiter += Position;
            Images.insert(Imgiter, NewFrame);
            ImageLengths.insert(Intiter, Length);
        } else {
            std::cout << "Failed to insert frame, the position(" << Position << ") is invalid\n";
            return false;
        }
        return true;
    }
    bool Animation::DeleteFrame(unsigned int Position) {
        std::vector<sf::Image*>::iterator Imgiter = Images.begin();
        std::vector<unsigned int>::iterator Intiter = ImageLengths.begin();
        if ((Position < Images.size()) && (Position >= 0)) {
            Imgiter += Position;
            Intiter += Position;
            Images.erase(Imgiter);
            ImageLengths.erase(Intiter);
        } else {
            std::cout << "Failed to delete frame, the position(" << Position << ") is invalid\n";
            return false;
        }
        return true;
    }
    /// Reset the whole animation, clearing all frames and reverting
    /// to frame 0 (although that is also erased). This DELETES
    /// all of the animation data.
    ///
    /// NOTE: Due to the design of the program, the graphics manager
    /// still stores the image data in memory, so the amount of
    /// memory that this clears is actually quite small.
    void Animation::Clear() {
        Images.clear();
        ImageLengths.clear();
        CurrentFrame = 0;
        CurrentImageFrame = 0;
        CurrentImage = 0;
    }
    sf::Image* Animation::GetFrame(unsigned int Position) {
        if ((Position < Images.size()) && (Position >= 0)) {
            return Images.at(Position);
        } else {
            std::cout << "Failed to get frame, the position(" << Position << ") is invalid\n";
            return NULL;
        }
    }
}