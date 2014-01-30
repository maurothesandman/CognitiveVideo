#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofBackground(100,100,100);
    vidPlayer.loadMovie("shopMall.mp4");
    vidPlayer.play();
    vidWidth = vidPlayer.getWidth();
    vidHeight = vidPlayer.getHeight();
    

	bLearnBakground = true;
	threshold = 80;
    vidWidth = vidPlayer.getWidth();            //get video width
    vidHeight = vidPlayer.getHeight();          //get video height
    
    
    //set up size of ofPixels objects and type (grayscale)
    diffIMG.allocate(vidWidth, vidHeight, OF_IMAGE_GRAYSCALE); 
    grIMG.allocate(vidWidth, vidHeight, OF_IMAGE_GRAYSCALE);
    bgIMG.allocate(vidWidth, vidHeight, OF_IMAGE_GRAYSCALE);
    meIMG.allocate(vidWidth, vidHeight, OF_IMAGE_GRAYSCALE);
    mhIMG.allocate(vidWidth, vidHeight, OF_IMAGE_GRAYSCALE);
    hmIMG.allocate(vidWidth, vidHeight, OF_IMAGE_COLOR_ALPHA);
    
    c = ofColor::fromHsb(170,255,255,0);
    
}

//--------------------------------------------------------------
void testApp::update(){


    bool bNewFrame = false;
    vidPlayer.update();         //update videoplayer
    bNewFrame = vidPlayer.isFrameNew();         //check if it's a new frame
    
    //if it's a new frame, go to work
	if (bNewFrame){

		///////////// RESET ///////////////////////
        if (bLearnBakground == true){
			bgIMG = grIMG;          //set background to current frame
            
            //black out the motion energy and  
            for(int i = 0; i < vidWidth; i++)
            {
                for(int j = 0; j < vidHeight; j++)
                {
                    meIMG.setColor(i, j, 0);
                    mhIMG.setColor(i, j, 0);
                    hmIMG.setColor(i,j,c);
                }
            }
            bLearnBakground = false;            //reset this to false
		}

        //set current ofPixels to current frame
        grIMG = (vidPlayer.getPixelsRef());
        
        //go through pixels one by one
        for(int i = 0; i < vidWidth; i++)
        {
            for(int j = 0; j < vidHeight; j++)
            {
                
                ///// DIFFERENCE IMG
                //create diffIMG by getting absolute value of current image - background image
                diffIMG.setColor(i,j, (abs(grIMG.getColor(i, j).getBrightness() - bgIMG.getColor(i,j).getBrightness())));
                
                //if absDiff is greater than the threshold, set diffIMG val to white
                if(diffIMG.getColor(i,j).getBrightness() > threshold)
                {
                    diffIMG.setColor(i, j, 255);
                }
                //if absDiff is lower than threshold, set diffIMG val to black
                else if(diffIMG.getColor(i,j).getBrightness() < threshold)
                {
                    diffIMG.setColor(i, j, 0);
                }
                
                

                //MOTION ENERGY & MOTION HISTORY if the diffIMG is white
                if(diffIMG.getColor(i,j).getBrightness() == 255)
                {
                    meIMG.setColor(i, j, 255);          //set motion energy pixel to white
                    mhIMG.setColor(i,j,255);            //set motion history pixel to white
                }
                
                //HEAT MAP:  is diff AND motion energy are white
                if(diffIMG.getColor(i,j).getBrightness() == 255 && meIMG.getColor(i, j).getBrightness() ==255)
                {
                    float hue = hmIMG.getColor(i, j).getHue();
                    float brightness = hmIMG.getColor(i, j).getBrightness();
                    hue -= 10;
                    if(hue <= 10) hue = 0;
                    hmIMG.setColor(i, j, ofColor::fromHsb(hue, 255, 255));
                }
                
                //MOTION HISTORY: fade out the older white over time (motion history image)
                int brightness = mhIMG.getColor(i, j).getBrightness();
                if(brightness >= 10)
                {
                    mhIMG.setColor(i, j, brightness - 10);
                }           
            }
        }
        bgIMG = grIMG;          //set background to current frame
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    
    //set pixels to an image and draw it
    image = diffIMG;            
    image.draw(0,0,vidWidth/2,vidHeight/2);
    
    //set motion energy pixels to an image and draw it
    meImage = meIMG;
    meImage.draw(vidWidth/2 + 20, 0, vidWidth/2, vidHeight/2);
    
    //set motion history pixels to an image and draw it
    mhImage = mhIMG;
    mhImage.draw(0, vidHeight/2 + 20, vidWidth/2, vidHeight/2);
    
    //show original image
    vidPlayer.draw(vidWidth/2 + 20, vidHeight/2 + 20, vidWidth/2, vidHeight/2);
    
    //set heat map pixesl to an image and draw it
    hmImage = hmIMG;
    hmImage.draw(vidWidth/2 + 20, vidHeight/2 + 20, vidWidth/2, vidHeight/2);
    
    
    
	// finally, a report:
	ofSetHexColor(0xffffff);
	stringstream reportStr;
	reportStr << "PRESS SPACE TO RESET" << endl
              << "FPS: " << ofGetFrameRate() << endl
			  << "press ' ' to capture bg" << endl
			  << "threshold " << threshold << " (press: +/-)" << endl
              << "width: " << vidWidth <<", height: " << vidHeight;
	ofDrawBitmapString(reportStr.str(), 20, 600);
    

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch (key){
		case ' ':
			bLearnBakground = true;
			break;
		case '+':
			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
