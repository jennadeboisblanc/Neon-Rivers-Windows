import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import java.nio.*; 
import KinectPV2.KJoint; 
import KinectPV2.*; 
import processing.net.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class kinect_server extends PApplet {



//Thomas Sanchez Lengeling- http://codigogenerativo.com/


KinectPV2 kinect;

Server myServer;

long lastCheckedKinect = 0;

byte transmitArray[];

public void setup() {
  
  // Starts a myServer on port 5204
  myServer = new Server(this, 5204);

  //------------ kinect
  kinect = new KinectPV2(this);
  //enable 3d  with (x,y,z) position
  kinect.enableSkeleton3DMap(true);
  kinect.init();
  

  transmitArray = new byte[73];
  transmitArray[0] = 89;
  for (int i = 1; i < 73; i++) {
    transmitArray[i] = 0;
  }
  frameRate(15);
}

public void draw() {
  background(0);
  setSkeletons();
  //testTransmitArray();
  writeSkeletons();
  //text(frameRate, 100, 100);
}

public void testTransmitArray() {
   for (int i = 0; i < 6; i++) {
    if (trackingSkeleton(i)) {
      println(skelCoord2Float(i, 0) + " " + skelCoord2Float(i, 1) + " " + skelCoord2Float(i, 2));
    }
  }
}

public void testNoSkeleton() {
  for (int i = 0; i < 6; i++) {
    setNoSkeleton(i);
  }
  if (trackingSkeleton(2)) {
    println("failed the tracking test");
  }
  else {
    println("tracking succeeded?");
  }
}

// if x coord being transmitted is > 200, not tracked
public boolean trackingSkeleton(int ind) {
  if (skelCoord2Float(ind, 0) > 200) {
    return false;
  }
  return true;
}

public float skelCoord2Float(int ind, int coord) {
  byte[] fl = new byte[4];
  fl[0] = transmitArray[ind*12+coord*4+1];
  fl[1] = transmitArray[ind*12 + 1+coord*4+1];
  fl[2] = transmitArray[ind*12 + 2+coord*4+1];
  fl[3] = transmitArray[ind*12 + 3+coord*4+1];
  return bytearray2float(fl);
}

public void writeSkeletons() {
  myServer.write(transmitArray);
}

public void resetSkeletons() {
  for (int i = 0; i < 6; i++) {
    setNoSkeleton(i);
  }
}

public void setSkeletons() {
  resetSkeletons();
  ArrayList<KSkeleton> skeletonArray =  kinect.getSkeleton3d();
 
  for (int i = 0; i < skeletonArray.size(); i++) {
    KSkeleton skeleton = (KSkeleton) skeletonArray.get(i);
    if (skeleton.isTracked()) {
      KJoint[] joints = skeleton.getJoints();
      setSkeleton(i, joints[KinectPV2.JointType_SpineShoulder]);
    } 
  }
}

public void setSkeleton(int ind, KJoint joint) {
  byte[] floatArrayX = float2ByteArray(joint.getX());
  byte[] floatArrayY = float2ByteArray(joint.getY());
  byte[] floatArrayZ = float2ByteArray(joint.getZ());
  for (int j = 0; j < 4; j++) {
    transmitArray[ind*12 + j+1] = floatArrayX[j];
    transmitArray[ind*12+4 + j+1] = floatArrayY[j];
    transmitArray[ind*12+8 + j+1] = floatArrayZ[j];
  }
}

public void setNoSkeleton(int ind) {
  byte[] b = float2ByteArray(650);
  for (int i = 0; i < 4; i++) { 
    transmitArray[ind*12 + i+1] = b[i];
  }
}

public byte [] float2ByteArray (float value){  
    return ByteBuffer.allocate(4).putFloat(value).array();
}

public float bytearray2float(byte[] b) {
    ByteBuffer buf = ByteBuffer.wrap(b);
    return buf.getFloat();
}
  public void settings() {  size(200, 200); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "kinect_server" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
