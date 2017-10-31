import java.nio.*;
//import java.net.*;

/*
Thomas Sanchez Lengeling.
 http://codigogenerativo.com/
 KinectPV2, Kinect for Windows v2 library for processing
 */
import KinectPV2.KJoint;
import KinectPV2.*;
KinectPV2 kinect;

import processing.net.*;

Server myServer;

byte transmitArray[];

void setup() {
  //size(200, 200);
  // Starts a myServer on port 5204
  myServer = new Server(this, 5204);

  //------------ kinect
  kinect = new KinectPV2(this);
  //enable 3d  with (x,y,z) position
  kinect.enableSkeleton3DMap(true);
  kinect.init();

  for (int i = 0; i < 18; i++) {
    transmitArray[i] = 0;
  }
}

void draw() {
  // move to middle of canvas
  // float zVal = 300;
  // float rotX = PI;
  //translate the scene to the center
  //pushMatrix();
  //translate(width/2, height/2, 0);
  //scale(zVal);
  //rotateX(rotX);

  setSkeletons();
  writeSkeletons();
}



void writeSkeletons() {
  myServer.write(transmitArray);
}

void setSkeletons() {
  ArrayList<KSkeleton> skeletonArray =  kinect.getSkeleton3d();
  // send 3 joints (2 hands, spineShoulder), 3 coords for each (x, y, z), 4 bytes per float
  // byte[] skeletonPoints = new byte[3*3*4];
  //individual JOINTS
  for (int i = 0; i < skeletonArray.size(); i++) {
    KSkeleton skeleton = (KSkeleton) skeletonArray.get(i);
    println("num joints: " + skeleton.getJoints().length);
    if (skeleton.isTracked()) {
      KJoint[] joints = skeleton.getJoints();
      setSkeleton(i, joints);
    } else {
      setNoSkeleton(i);
    }
  }
}

void setSkeleton(int ind, KJoint[] joints) {
  transmitArray[ind*4] = byte(255);
  transmitArray[ind*4 + 1] = getXMap(joints[KinectPV2.JointType_SpineShoulder]);
  transmitArray[ind*4 + 2] = getYMap(joints[KinectPV2.JointType_SpineShoulder]);
}

byte getXMap(KJoint joint) {
  return byte(constrain(map(joint.getX(), -1, 1, 0, 255), 0, 255));
}

byte getYMap(KJoint joint) {
  return byte(constrain(map(joint.getY(), -1, 1, 0, 255), 0, 255));
}

byte[] getJointByteArray(KJoint joint) {
  byte[] skeletonPoints = new byte[12];
  byte[] floatArrayX = float2ByteArray(joint.getX());
  byte[] floatArrayY = float2ByteArray(joint.getY());
  byte[] floatArrayZ = float2ByteArray(joint.getZ());
  for (int j = 0; j < 4; j++) {
    skeletonPoints[j] = floatArrayX[j];
    skeletonPoints[4 + j] = floatArrayY[j];
    skeletonPoints[8 + j] = floatArrayZ[j];
  }
  return skeletonPoints;
}



void setNoSkeleton(int ind) {
  // isTracked, x, and y set to 0
  for (int i = 0; i < 3; i++) { 
    transmitArray[ind*4 + i] = 0;
  }
}

public static byte [] float2ByteArray (float value) {
  return ByteBuffer.allocate(4).putFloat(value).array();
}

//byte getByte(float coord) {
//  return byte(constrain(map(coord, -1, 1, 0, 255), 0, 255));
//}