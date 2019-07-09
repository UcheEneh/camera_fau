***************************************************************************************************************************
                                                INSTALLATION GUIDE
***************************************************************************************************************************
1.	QT Installation Guide
-	https://www.youtube.com/watch?v=IZxNXNfwyKA
-	https://retifrav.github.io/blog/2017/03/29/installing-qt-for-the-first-time/


2.	QT for Android
-	http://doc.qt.io/qt-5/androidgs.html
-	https://retifrav.github.io/blog/2017/12/28/qt-for-android/
	
  step 1: https://www.youtube.com/watch?v=PU_po2mWwt8
  step 2: For Android Virtual Device (AVD):
    - Go to android studio sdk manager and install the missing armeabi and x86 images for the different android platforms 
  
  Android Sdk toolkit:
  To open in android studio:
  - cd /root/android-studio/bin 
  - use ./studio.sh

  To open using just the sdkmanager:
  - cd /root/android-sdk/tools/bin
  - then	./sdkmanager


3.	Openssl
Openssl is important for sending the image and information to the online web host.

  Build openssl version OpenSSL 1.0.2k (QT version)
  -	https://www.howtoforge.com/tutorial/how-to-install-openssl-from-source-on-linux/

  Note: using vim: https://www.cyberciti.biz/faq/linux-unix-vim-save-and-quit-command/
  

**************************************************************************************************************************
                                                 APPS 
**************************************************************************************************************************
1.	camera_v1: created using Qt Widgets
2.	camera_v2: created using QML

After setting up QT and android deployment on linux, open the project from the QT creator software (the '.pro' file in each foders)
-	Build and Run 
-	Deploying the app:
	1: On Desktop: Deploys directly after building and running on the qt creator software
	2: On Phone:
		(Method 1) After setting up the QT for android and changing the android phone to Developer mode, deploy the project throught the QT creator software with the deploy Kit in Android mode and the phone connected to the computer via USB
		(Method 2) Deploy through installing the “android-deploy-debug.apk” file in the build folder

Structure of the app:
•	Take picture
•	Crop picture
•	Open edit page to input information about the picture
	
 apk: select Release and build the program, then go to the build folder 

e.g.
root/build-simplecamera_vMine-Android_for_armeabi_v7a_GCC_4_9_Qt_5_11_2_for_Android_ARMv7-Release/android-build/build/outputs/apk/android-build-debug.apk

The apk file can be copied to the phone and installed directly


***************************************************************************************************************************
                                Save information on the online Database
***************************************************************************************************************************

Information inputted on the Edit page are sent to the sql database on the free online web-host created using AWARDSPACE:

	https://cp1.awardspace.net/beta/login/?next=file-manager&notice=login

Login details:
- Client ID: 2896427	
- pw: Awardspace1!

In both camera_versions, Name, category and date are sent to the sql database, while images are stored in the File Manager folder named "fau5.atwebpages.com" on the web-host page.

SQL DB: https://cp1.awardspace.net/beta/database-manager/#mysql-databases
File Manager: https://cp1.awardspace.net/beta/file-manager/

Images are stored with the information given about them, so in the web-host folder “Images”, an image would be named as: 
	'star_Celestial bodies_17_06_2019.jpg'
Where 
-	Name: star
-	Category: Celestial bodies
-	Date: 17_06_2019


***************************************************************************************************************************
                                               Problems with QT App
***************************************************************************************************************************
1. camera_v1: created using Qt Widgets
Problem:
	The camera viewfinder doesn’t show the current camera feed diplay, so if the camera app is opened, the live image isn’t shown

2. camera_v2: created using QML
Problem:
	• App Freezes for android OS less than android 8

3. Both
	• For some reason, the captured Image on phone is rotates automatically after crop is applied. This doesn't occur when deployed to Desktop
	• Adding non-rectangular bounding boxes for the crop


***************************************************************************************************************************
                                      Extra:  OPENCV Installation for QT
***************************************************************************************************************************

Tutorials:
-	http://www.codebind.com/cpp-tutorial/install-opencv-ubuntu-cpp/
-	http://www.daslhub.org/unlv/wiki/doku.php?id=opencv_install_ubuntu
-	https://www.learnopencv.com/install-opencv3-on-ubuntu/

Downloads: 
opencv: https://opencv.org/releases.html
opencv_contrib: https://github.com/opencv/opencv_contrib/releases
opencv_android: https://sourceforge.net/projects/opencvlibrary/files/opencv-android/

Example:
https://www.youtube.com/watch?v=2Pboq2LFoaI

After opencv installation, to set up for QT: 
Just include the following in the .pro file (check the camera_frontend.pro in camera_v1)

-	For Desktop application
INCLUDEPATH += /usr/local/include/
LIBS += `pkg-config --libs opencv`


-	For Android app:
include(../../QtProject/shared/opencv_android.pri)
