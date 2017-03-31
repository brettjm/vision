#include "tracker.h"

#define TEST 1

  //these HSV values are what the trackbars in the window are initialized to
  int iLowH = 0;//0;
  int iHighH = 0;//0;

  int iLowS = 0;//0; 
  int iHighS = 255;//255;

  int iLowV = 210;//210;
  int iHighV = 255;//255;
  

  //create global matrices so that they dont lost their values in between function callbacks
  Mat imgLines;
  

  //global because the "greens" cant lost their values while blue is being tracked and vice versa
  int blueMomentX = 0;
  int blueMomentY = 0;
  int greenMomentX = 0;
  int greenMomentY = 0;
  int firsttime = 0;

  //create an object for each color you want to track
  color_tracker red;
  color_tracker yellow;
  color_tracker green;
  color_tracker blue;
  color_tracker purple;
  color_tracker orange;

string colorToTrack;

 int main( int argc, char** argv )
 {
    if (argc < 2) {
      cout << "Must provide 1 command line argument to tracker" << endl;
      return 1;
    }
    colorToTrack = string(argv[1]);

    ros::init(argc, argv, colorToTrack + "tracker");
    ros::NodeHandle n;

    if (TEST) {
      namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
  	  //Create trackbars in "Control" window
  	  createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
  	  createTrackbar("HighH", "Control", &iHighH, 179);

  	  createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
  	  createTrackbar("HighS", "Control", &iHighS, 255);

  	  createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
  	  createTrackbar("HighV", "Control", &iHighV, 255);
    }

    //initialize the "last" values until they receive info
    red.iLastY = -1;
    red.iLastX = -1;
    yellow.iLastY = -1;
    yellow.iLastX = -1;
    green.iLastY = -1;
    green.iLastX = -1;
    blue.iLastY = -1;
    blue.iLastX = -1;
    purple.iLastY = -1;
    purple.iLastX = -1;
    orange.iLastY = -1;
    orange.iLastX = -1;

    //initialize each color to its same color to be drawn as the tracking line
    red.line_color = Scalar(0,0,255);
    yellow.line_color = Scalar(0,120,120);
    green.line_color = Scalar(0,255,0);
    blue.line_color = Scalar(255,0,0);
    red.hue = "red";
    yellow.hue = "yellow";
    green.hue = "green";
    blue.hue = "blue";
    purple.hue = "purple";
    orange.hue = "orange";

    if (colorToTrack == "red") {
      red.chatter_pub = n.advertise<geometry_msgs::Pose2D>("red_chatter",100);//publisher is called chatter_pub
    }
    else if (colorToTrack == "yellow") {
      yellow.chatter_pub = n.advertise<geometry_msgs::Pose2D>("yellow_chatter",100);//publisher is called chatter_pub
    }
    else if (colorToTrack == "green") {
      green.chatter_pub = n.advertise<geometry_msgs::Pose2D>("green_chatter",100);//publisher is called chatter_pub
    }
    else if (colorToTrack == "blue") {
      blue.chatter_pub = n.advertise<geometry_msgs::Pose2D>("blue_chatter",100);//publisher is called chatter_pub
    }
    else if (colorToTrack == "purple") {
      purple.chatter_pub = n.advertise<geometry_msgs::Pose2D>("purple_chatter",100);//publisher is called chatter_pub
    }
    else if (colorToTrack == "orange") {
      orange.chatter_pub = n.advertise<geometry_msgs::Pose2D>("orange_chatter",100);//publisher is called chatter_pub
    }
    else {
      cout << "COLOR SPELLED WRONG! COLOR NAME MUST BE ALL LOWER CASE!" << endl;
      return 1;
    }

    image_transport::ImageTransport it(n);
    image_transport::Subscriber sub = it.subscribe("/usb_cam_away/image_raw", 1, imageCallback);//call the callback function every time recieve a subscription
    
    ros::spin();
    if (TEST) {
      cv::destroyWindow("view");
    }


   return 0;
}

// These values can be obtained by running contours. These need to be changed everytime because the field moves around.
const int smallestX = 91;
const int largestX = 730;
const int smallestY = 18;
const int largestY = 438;

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {

    //this if statement is just a hack cuz i was too lazy to figure out variableS
    //the contents of the if statement should only be called once at the beginning so imgLines doesnt repeatedly get rewritten
    if(firsttime == 0)
    {
          Mat imgTmp = cv_bridge::toCvShare(msg, "bgr8")->image; //Capture an image from the camera in bgr format and store in imgTmp   
          imgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );//Create a black image with the size as the camera output
          imgLines = Scalar(255, 255, 255);
          imgLines = Mat(imgLines, Rect(smallestX, smallestY, largestX - smallestX, largestY - smallestY));
          imgTmp = Mat(imgTmp, Rect(smallestX, smallestY, largestX - smallestX, largestY - smallestY));
          imwrite("field_background.png", imgTmp);
    }
    firsttime = 1;

    Mat imgOriginal;//create matrix called imgOriginal that gets newly created for every frame

    imgOriginal = cv_bridge::toCvShare(msg, "bgr8")->image;//capture image from camera and store in imgOriginal matrix in bgr format
    
    //crop the image 
    imgOriginal = Mat(imgOriginal, Rect(smallestX, smallestY, largestX - smallestX, largestY - smallestY));
    if (TEST) {
      imshow("original", imgOriginal);
    }

    //assign high and low values for thresholds
    //keep in this section because you want these values to change everytime callback function is called so that the 
    //threshold values stay current with how the sliders are changing
    red.LowH = iLowH + 0; //121 15
    red.HighH = iHighH + 0; //179 20
    red.LowS = iLowS;
    red.HighS = iHighS;
    red.LowV = iLowV - 5; // 10
    red.HighV = iHighV;

    yellow.LowH = iLowH + 0;//0
    yellow.HighH = iHighH + 139;//40
    yellow.LowS = iLowS;
    yellow.HighS = iHighS - 186;
    yellow.LowV = iLowV - 16;//62
    yellow.HighV = iHighV;

    green.LowH = iLowH + 55;//80
    green.HighH = iHighH + 105;//100
    green.LowS = iLowS;//46
    green.HighS = iHighS - 120;
    green.LowV = iLowV + 10;
    green.HighV = iHighV;
   
    blue.LowH = iLowH + 95;//95
    blue.HighH = iHighH + 110;//110
    blue.LowS = iLowS;
    blue.HighS = iHighS;
    blue.LowV = iLowV;
    blue.HighV = iHighV;

    purple.LowH = iLowH + 100;//100
    purple.HighH = iHighH + 145;//120
    purple.LowS = iLowS;
    purple.HighS = iHighS;
    purple.LowV = iLowV;
    purple.HighV = iHighV;


    orange.LowH = iLowH + 20;//100
    orange.HighH = iHighH + 35;//120
    orange.LowS = iLowS;
    orange.HighS = iHighS;
    orange.LowV = iLowV;
    orange.HighV = iHighV;


 
    //BACKGROUND SUBRTRACTION===============================
    //background_sub(imgOriginal);

    if (TEST) {
      imshow("After background_sub", imgOriginal);
    }


    //FIND INFO ON COLORS====================================
    if (colorToTrack == "red") {
      threshold(imgOriginal, red);
    }
    else if (colorToTrack == "yellow") {
      threshold(imgOriginal, yellow);
    }
    else if (colorToTrack == "green") {
      threshold(imgOriginal, green);
    }
    else if (colorToTrack == "blue") {
      threshold(imgOriginal, blue);
    }
    else if (colorToTrack == "purple") {
      threshold(imgOriginal, purple);
    }
    else if (colorToTrack == "orange") {
      threshold(imgOriginal, orange);
    }

    /*struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    cout << spec.tv_nsec / 1.0E6 << " ms" << endl;*/

    if (TEST) {
      cv::waitKey(15);
    }
  }

  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }

}


void threshold(Mat imgOriginal, color_tracker& color)
{

   Mat imgHSV;
   cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

   Mat imgThresholded;
   inRange(imgHSV, Scalar(color.LowH, color.LowS, color.LowV), Scalar(color.HighH, color.HighS, color.HighV), imgThresholded); //Threshold the image with values from trackbars and store in imgThresholded matrix
 
   int opening_size = 0;
   if(color.hue == "red" || color.hue == "yellow")
   {
    opening_size = 7; //7 is barely smaller than the golf ball
   }
   else
   {
    opening_size = 9;// the shapes on the robot are bigger
   }


   //morphological opening (removes small objects from the foreground)
   erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(opening_size, opening_size)) );
   dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(opening_size, opening_size)) ); 
   //morphological closing (removes small holes from the foreground)
   dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(opening_size, opening_size)) ); 
   erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(opening_size, opening_size)) );

   Mat imgMasked;
   imgOriginal.copyTo(imgMasked, imgThresholded);//mask the black and white thresholded image with original to show color of object being tracked

  if (TEST) {
    imshow("masked Image", imgMasked); //show the thresholded image
  }
   
   contour_pos(imgThresholded, color);

}



void contour_pos(Mat imgThresholded, color_tracker& color)
{
       

      Mat canny_output;
      vector<vector<Point> > contours;
      vector<Vec4i> hierarchy;


      /// Detect edges using canny
      Canny( imgThresholded, canny_output, 75, 250, 3 );
      /// Find contours
      findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );


      // Draw contours
      Mat drawing = Mat::zeros( imgThresholded.size(), CV_8UC3 );
      for( int i = 0; i< contours.size(); i++ )
      {

            //find the number of vertices for the ith contour
            double perimeter = arcLength(contours.at(i), 1);
            vector<Point> vertices;
            approxPolyDP(contours.at(i), vertices, .02*perimeter, 1);


            Moments oMoments = moments(contours.at(i));
            double dM01 = oMoments.m01;//add up all pixel Y? positions
            double dM10 = oMoments.m10;//add up all pixel X? positions
            double dArea = oMoments.m00;//calculate area of color
            
            double posX = dM10 / dArea;
            double posY = dM01 / dArea; 
            
            double adjPosX = posX;
            double adjPosY = posY*(-1) + 490;//flip the y axis so its oriented like we are used to
//ROS_INFO("%lf", dArea);
            

            if(color.hue != "red" && (dArea < 100 || dArea > 500))//if the area is super big or super small you know you can ignore it
            {
              continue;//you could also do filtering by the number of vertices if you want
            }
            else if (color.hue == "red" && (dArea < 30 || dArea > 100)) {//nathan put this to 200 originally
              continue;
            }
            else if(dArea >= color.iLastArea)//save the appropraite X and Y and areas for either the big or small shape
            {
              color.bigBlobArea = dArea;
              color.bigBlobX = adjPosX;
              color.bigBlobY = adjPosY;
              color.smallBlobArea = color.iLastArea;
              color.smallBlobX = color.iLastX;
              color.smallBlobY = color.iLastY;
            }
            else
            {
              color.smallBlobArea = dArea;
              color.smallBlobX = adjPosX;
              color.smallBlobY = adjPosY;
              color.bigBlobArea = color.iLastArea;
              color.bigBlobX = color.iLastX;
              color.bigBlobY = color.iLastY;
            }


            drawContours( drawing, contours, i, 255, 1, 8, hierarchy, 0, Point() );

            color.iLastArea = dArea;
            color.iLastX = adjPosX;
            color.iLastY = adjPosY;
            
            
            i++;//THIS IS A HACK BECAUSE findContours() FINDS DUPLICATES OF EVERY CONTOUR. SO SKIP THE DUPLICATES!!!!!!!!!!    
      }

          //calculate the angle of the robot
          int opposite = color.smallBlobY - color.bigBlobY;
          int adjacent = color.smallBlobX - color.bigBlobX;
          int angle = (atan2(opposite, adjacent))*(180/3.141);
          if(angle < 0)
          {
            angle = angle + 360;//this makes it 0-360 rather than 0-180 and 0 to -180
          }

          //publish message
          geometry_msgs::Pose2D pub_msg;
          pub_msg.x = color.bigBlobX;
          pub_msg.y = color.bigBlobY - 75;
          pub_msg.theta = angle;
          color.chatter_pub.publish(pub_msg);//publish the msg that you built
      if (TEST) {  
        imshow( "Contours", drawing );
      }

}


void background_sub(Mat& imgOriginal)
{
  //imwrite("background.png", imgOriginal );
  //cvtColor(imgOriginal, imgOriginal, CV_HSV2BGR);

  Mat imgBackground = imread("field_background.png");
  Mat imgDifference;
  absdiff(imgOriginal, imgBackground, imgDifference);

  Mat imgcolor;
  cvtColor( imgDifference, imgcolor, CV_HSV2BGR );

  Mat imgGRAY;
  cvtColor( imgcolor, imgGRAY, COLOR_BGR2GRAY );

  Mat imgthresh;
  threshold(imgGRAY, imgthresh, 45, 255, CV_THRESH_BINARY); 

  Mat imgMasked;
  imgOriginal.copyTo(imgMasked, imgthresh);
  //imshow("difference", imgMasked);

  imgOriginal = imgMasked;
}